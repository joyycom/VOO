#include "FashionlineInputListCreator.h"
#include "SkyPack.h"

#include <QImageReader>
#include <QJsonObject>
#include <QSemaphore>
#include <string>
#ifdef Q_OS_WINDOWS
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#endif
#include "base/utils/JsonUtils.h"
#include "base/utils/UploadUtils.h"
#include "base/utils/LuaUtil.h"

#include <base/utils/SkyVariantUtil.h>

static SkyPack *s_skyPackThread = nullptr;
SkyPack *SkyPack::pack(QString outputDir,
                       QString videoName,
                       QString packName,
                       QString coverPath,
                       int cover,
                       bool video,
                       bool pack,
                       int templateType,
                       QString fashionlineCfgDir) {
    if (s_skyPackThread && s_skyPackThread->isRunning()) {
        s_skyPackThread->quit();
    }
    s_skyPackThread =
        new SkyPack(outputDir, videoName, packName, coverPath, cover, video, pack, templateType, fashionlineCfgDir);
    s_skyPackThread->setObjectName("SkyPack");
    s_skyPackThread->start();
    connect(s_skyPackThread, &QThread::finished, s_skyPackThread, &QObject::deleteLater);
    return s_skyPackThread;
}

bool SkyPack::shouldBeCancelled(void *userData) {
    SkyPack *source = (SkyPack *)userData;
    return source->m_cancel;
}

bool SkyPack::isPacking() {
    return s_skyPackThread != nullptr;
}

void SkyPack::setSharedPref(SharedPref *sharedPref) {
    m_sharedPref = sharedPref;
}

void SkyPack::onProgress(void *userData, double currentTime, double totalTime) {
    SkyPack *source = (SkyPack *)userData;
    source->onVideoProgress(currentTime);
    qDebug() << "SkyPack Export Progress Time:" << currentTime << " Total:" << totalTime;
}

void SkyPack::onFinish(void *userData) {
    SkyPack *source = (SkyPack *)userData;
    qDebug() << "SkyPack Export Finish!!!!!!!!!!!";
    source->onVideoFinish();
}

void SkyPack::onError(void *userData, SkyError error) {
    qDebug() << "SkyPack Export onError!!!!!!!!!!!" << error;
    SkyPack *source = (SkyPack *)userData;
    source->onVideoError();
}

int64_t SkyPack::getBitRate(SkyTimeline *timeline) {
    auto videoParams = timeline->getVideoParams();
    int width = videoParams.width;
    int height = videoParams.height;
    int64_t bitRate = (width * height) * (videoParams.frameRate / 30.0) / (540.0 * 960.0 / 4000);
    return bitRate * 1000.0;
}

SkyPack::SkyPack(QString outputDir,
                 QString videoName,
                 QString packName,
                 QString coverPath,
                 int cover,
                 bool video,
                 bool pack,
                 int templateType,
                 QString fashionlineCfg)
    : m_outputDir(outputDir),
      m_videoName(videoName),
      m_packName(packName),
      m_coverPath(coverPath),
      m_extrackCover(cover),
      m_extrackVideo(video),
      m_extrackPack(pack),
      m_templateType(templateType),
      m_fashionlineCfgDir(fashionlineCfg) {
    m_mmkv = MMKV::mmkvWithID((SEProject::current()->title() + ".export").toStdString());
}

void SkyPack::cancel() {
    m_cancel = true;
}

uint qHash(const PreProcessRes &key) {
    return key.originalPath.toUInt();
}

void SkyPack::run() {
    init();
    //创建新的timeline对象，不能和之前的timeline产生冲突
    SEProject::current()->save();
    m_rootDir = SEProject::current()->rootPath();
    m_tempOutputDir = AppPath::tempPath(SEProject::current()->title());
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    m_newTimeline = SkyTimeline::create(timeline->getCaches());
    m_newTimeline->loadConfig(timeline->saveConfig());

    namedSkyTimeline(m_newTimeline);

    m_imagePreDir = QFileInfo(QDir::tempPath(), "VOO/SkyPack/" + SEProject::current()->title() + "_image").filePath();
    m_resDir = m_tempOutputDir + "/res";
    m_effectsDir = m_tempOutputDir + "/effects";
    m_fontsDir = m_tempOutputDir + "/fonts";
    m_animatiosDir = m_tempOutputDir + "/animations";
    QDir outDir(m_outputDir);
    QDir imageDir(m_imagePreDir);
    QDir tempDir(m_tempOutputDir);
    if (outDir.exists(m_outputDir)) {
        outDir.removeRecursively();
    }
    outDir.mkpath(m_outputDir);
    if (imageDir.exists(m_imagePreDir)) {
        imageDir.removeRecursively();
    }
    imageDir.mkpath(m_imagePreDir);
    if (tempDir.exists(m_tempOutputDir)) {
        tempDir.removeRecursively();
    }
    if (!tempDir.exists(m_resDir)) {
        tempDir.mkpath(m_resDir);
    }
    if (!tempDir.exists(m_effectsDir)) {
        tempDir.mkpath(m_effectsDir);
    }
    if (!tempDir.exists(m_fontsDir)) {
        tempDir.mkpath(m_fontsDir);
    }
    if (!tempDir.exists(m_animatiosDir)) {
        tempDir.mkpath(m_animatiosDir);
    }
    //统计总进度时长
    collectTotalProgress();
    //导出封面
    outputCover();
    //导出模板
    QString packPath = QFileInfo(m_outputDir, m_packName + ".zip").filePath();
    if (m_extrackPack && !m_cancel) {
        //复制资源
        outputPack();
        emit outputPackFinishChanged(m_tempOutputDir, m_resList, m_newTimeline);
        //移除空轨
        for (int i = 0; i < m_newTimeline->numberOfTracks(); i++) {
            SkyTrack *track = m_newTimeline->getTrackAt(i);
            if (track->numberOfClips() == 0) {
                m_newTimeline->removeTrack(track);
            }
        }
        //不同模板类型处理
        processTemplateType();

        SkyString jsonHolder = m_newTimeline->saveConfig(m_tempOutputDir.toUtf8().data()).transToJsonString(true);
        QString json = QString::fromUtf8(jsonHolder.c_str());
        auto pro = SEProject::current()->proj();
        QFileInfo skyFile(pro->entryFile());
        if (skyFile.suffix() == "sky") {
            FileUtils::write(QFileInfo(m_tempOutputDir, skyFile.fileName()).filePath(), json);
        } else {
            FileUtils::write(QFileInfo(m_tempOutputDir, "timeline.sky").filePath(), json);
        }
        //复制封面
        QString _cover = pro->thumb();
        QString cover = QFileInfo(pro->thumb()).fileName();
        FileUtils::copyFile(
            pro->thumb(), QFileInfo(m_tempOutputDir, QFileInfo(pro->thumb()).fileName()).filePath(), true);
        //复制工程配置
        QString projPath = pro->projFilePath(pro->projectDir());
        FileUtils::copyFile(projPath, QFileInfo(m_tempOutputDir, QFileInfo(projPath).fileName()).filePath(), true);

        if (QFile::exists(packPath)) {
            QFile::remove(packPath);
        }
        //压缩图片资源
        imageMin();
        FileUtils::makeFileReadable(m_tempOutputDir);
        ofluaToLua(m_tempOutputDir); //避免阻塞网页解析进度，暂时放开文本
        //对lua文件进行加密，暂时屏蔽
        //        luaToOflua(m_tempOutputDir);
        ZipUtils::compressFolder(m_tempOutputDir, packPath);
    }
    //导出视频
    QString videoPath = QFileInfo(m_outputDir, m_videoName + ".mp4").filePath();
    outputVideo(videoPath);
    //删除文件夹
    tempDir.removeRecursively();
    QDir(QFileInfo(QDir::tempPath(), "VOO/SkyPack").filePath()).removeRecursively();
    if (m_cancel) {
        emit cancelChanged();
    } else if (m_error) {
        emit errorChanged();
    } else {
        emit finishChanged(m_outputDir, videoPath, packPath);
    }
    SEProject::current()->dom()->previewController()->getSkyTimelineView()->destoryTimeline(m_newTimeline);
    m_newTimeline = nullptr;
    m_process->close();
    delete m_process;
    m_process = nullptr;
    s_skyPackThread = nullptr;
    qDebug() << "SkyPack,end";
    return;
}

void SkyPack::init() {
    m_process = new QProcess(0);
    qDebug() << "SkyPack,initFFmpeg:" << m_process;
}

QString SkyPack::ffmpegProgram() {
#ifdef Q_OS_WINDOWS
    return QCoreApplication::applicationDirPath().append("/ffmpeg.exe");
#else
    return QCoreApplication::applicationDirPath().append("/../..").append("/ffmpeg");
#endif
}

QString SkyPack::imageMinProgram() {
    return "";
}

void SkyPack::execProcess(PreProcessRes &preRes) {
    QString rootDir = QFileInfo(m_rootDir).filePath() + "/";
    QFileInfo originalFile(preRes.originalPath);
    QFileInfo newFile(preRes.newPath);
    if (preRes.originalPath.startsWith(rootDir)) {
        QFile::remove(preRes.newPath);
        preRes.newPath = QFileInfo(newFile.path(), newFile.completeBaseName() + "." + originalFile.suffix()).filePath();
        QFile::copy(preRes.originalPath, preRes.newPath);
        return;
    }
    m_process->setProgram(ffmpegProgram());
    switch (preRes.type) {
        case Video: {
            SkyClip *clip = preRes.clip;
            auto range = clip->getTrimRange();
            auto videoParams = m_newTimeline->getVideoParams();
            auto videoStream = clip->getResource().firstVideoStream();
            int width = videoStream.width;
            int height = videoStream.height;
            width -= width % 16;
            height -= height % 16;
            if (QFileInfo(preRes.originalPath).suffix() == "mp4") {
                m_process->setArguments(QStringList() << "-y"
                                                      << "-v"
                                                      << "quiet"
                                                      //裁剪
                                                      << "-ss" << QString::number(range.begin) << "-to"
                                                      << QString::number(range.end) << "-i"
                                                      << preRes.originalPath
                                                      //分辨率缩放
                                                      << "-s"
                                                      << QString::number(width) + "x" + QString::number(height)
                                                      //帧率
                                                      << "-r"
                                                      << QString::number(videoParams.frameRate)
                                                      //去除B帧
                                                      << "-bf"
                                                      << "0"
                                                      << "-c:v"
                                                      << "libx264"
                                                      << "-preset"
                                                      << "medium"
                                                      << "-strict"
                                                      << "2"
                                                      << "-pix_fmt"
                                                      << "yuv420p"
                                                      << "-x264-params"
                                                      << "crf=25:bframes=0"
                                                      << "-c"
                                                      << "copy" << preRes.newPath);
            } else {
                m_process->setArguments(QStringList() << "-y"
                                                      << "-v"
                                                      << "quiet"
                                                      //裁剪
                                                      << "-ss" << QString::number(range.begin) << "-to"
                                                      << QString::number(range.end) << "-i"
                                                      << preRes.originalPath
                                                      //分辨率缩放
                                                      << "-s"
                                                      << QString::number(width) + "x" + QString::number(height)
                                                      //帧率
                                                      << "-r"
                                                      << QString::number(videoParams.frameRate)
                                                      //去除B帧
                                                      << "-bf"
                                                      << "0"
                                                      << "-c:v"
                                                      << "libx264"
                                                      << "-preset"
                                                      << "medium"
                                                      << "-strict"
                                                      << "2"
                                                      << "-pix_fmt"
                                                      << "yuv420p"
                                                      << "-x264-params"
                                                      << "crf=25:bframes=0" << preRes.newPath);
            }
            execProcessStart();
        } break;
        case Audio:
            m_process->setArguments(QStringList() << "-y"
                                                  << "-v"
                                                  << "quiet"
                                                  << "-i"
                                                  << preRes.originalPath
                                                  //码率64k
                                                  << "-b:a"
                                                  << "64k"
                                                  //采样率44100
                                                  << "-ar"
                                                  << "44100"
                                                  // aac编码
                                                  << "-acodec"
                                                  << "mp3" << preRes.newPath);
            execProcessStart();
            break;
        case Image: {
            QFile::copy(preRes.originalPath, preRes.newPath);
            if (!preRes.prePath.isEmpty()) {
                QFile::copy(preRes.originalPath, preRes.prePath);
            }
        } break;
        default:
            return;
    }
    qDebug() << "SkyPack,execFFmpeg end";
}

void SkyPack::execProcessStart() {
    qDebug() << "SkyPack," << m_process->program() << ":" << m_process->arguments();
    m_process->start();
    m_process->waitForStarted();
    m_process->setReadChannel(QProcess::StandardError);
    while (m_process->state() != QProcess::NotRunning) {
        m_process->waitForReadyRead();
        QString output = QString::fromLocal8Bit(m_process->readAllStandardError());
        qDebug() << "SkyPack Process output:" << output;
    }
    m_process->waitForFinished();
    m_process->reset();
}

void SkyPack::collectTotalProgress() {
    //所有片段的时间
    if (m_extrackPack) {
        for (int i = 0; i < m_newTimeline->numberOfTracks(); i++) {
            SkyTrack *track = m_newTimeline->getTrackAt(i);
            for (int j = 0; j < track->numberOfClips(); j++) {
                SkyClip *clip = track->getClipAt(j);
                auto range = clip->getTrimRange();
                m_totalTime += (range.end - range.begin);
            }
        }
    }
    //导出封面，给它1秒
    if (m_extrackCover >= 0) {
        m_totalTime += 1;
    }
    //导出视频，处理比较慢，给它两倍
    if (m_extrackVideo) {
        m_totalTime += m_newTimeline->getDuration() * 2;
    }
    updateProgress(0);
}

void SkyPack::updateProgress(double time) {
    if (m_totalTime <= 0) {
        return;
    }
    m_currentTime += time;
    emit progressChanged(m_currentTime / m_totalTime);
}

void SkyPack::onVideoProgress(double time) {
    if (m_videoTime == 0) {
        updateProgress(time * 2);
    } else {
        updateProgress((time - m_videoTime) * 2);
    }
    m_videoTime = time;
}

void SkyPack::onVideoError() {
    m_error = true;
}

void SkyPack::onVideoFinish() {
    updateProgress((m_newTimeline->getDuration() - m_videoTime) * 2);
}

void SkyPack::outputPack() {
    //遍历所有片段
    m_resList.clear();
    SkyObjectList objectList = m_newTimeline->getAllSubObjects();
    for (int i = 0; i < objectList.size(); i++) {
        SkyObject *obj = objectList[i];
        if (obj->getObjectType() == SkyObjectType_Clip) {
            SkyClip *clip = (SkyClip *)obj;
            updateClipData(clip);
        } else if (obj->getObjectType() == SkyObjectType_Effect || obj->getObjectType() == SkyObjectType_Transition) {
            SkyEffect *effect = (SkyEffect *)obj;
            updateEffectData(effect);
        }
    }
}

void SkyPack::updateClipData(SkyClip *clip) {
    SkyResource res = clip->getResource();
    PreProcessRes preRes = PreProcessRes();
    preRes.clip = clip;
    preRes.originalPath = res.getPath().c_str();
    bool isSkymedia = preRes.originalPath.startsWith("skymedia://");
    bool isInList = m_resList.contains(preRes);
    //找出片段的类型，重命名
    if (isSkymedia) {
    } else if (isInList) {
        auto listData = m_resList.find(preRes);
        preRes.type = listData->type;
        preRes.newPath = listData->newPath;
    } else {
        auto resType = clip->getUserData("ResType");
        if (!resType.isNull()) {
            preRes.type = parseResourecType(resourceTypeFromName(resType.toString()));
            switch (preRes.type) {
                case Video:
                    getVideoPreRes(preRes);
                    break;
                case Audio:
                    getAudioPreRes(preRes);
                    break;
                case Image:
                    getImagePreRes(preRes);
                    break;
                default:
                    break;
            }
        } else if (clip->isVideoClip()) {
            getVideoPreRes(preRes);
        } else if (clip->isAudioClip()) {
            getAudioPreRes(preRes);
        } else {
            return;
        }
    }
    if (!isSkymedia && !preRes.newPath.isEmpty()) {
        //执行FFmpeg来压缩裁剪
        if (!isInList) {
            execProcess(preRes);
        }
        //更新timeline里的资源路径
        bool result = clip->updateResource(SkyResource(preRes.newPath.toUtf8().data()));
        if (result) {
            m_resList.insert(preRes);
        }
    }
    //处理片段usesrData里的路径
    SkyVariant resType = clip->getUserData("ResType");
    SkyVariant resName = clip->getUserData("ResName");
    SkyVariant resPath = clip->getUserData("ResPath");
    SkyVariant newData = updateUserData(m_resList, clip, resType, resName, resPath);
    clip->setUserData("ResName", newData.at(0));
    clip->setUserData("ResPath", newData.at(1));
    //更新片段时间范围
    auto range = clip->getTrimRange();
    clip->updateResource(clip->getResource(), SkyTimeRange(0, range.end - range.begin));
    updateProgress(range.end - range.begin);
}

void SkyPack::updateEffectData(SkyEffect *effect) {
    SkyVariant params = effect->getParams();
    SkyVariant resType = effect->getUserData("ResType");
    SkyVariant resName = effect->getUserData("ResName");
    SkyVariant resPath = effect->getUserData("ResPath");
    SkyVariant newData = updateUserData(m_resList, nullptr, resType, resName, resPath);
    effect->setUserData("ResName", newData.at(0));
    effect->setUserData("ResPath", newData.at(1));
    SkyVariant ofParam = params.find("ofParam");
    if (!ofParam.isNull()) {
        ResourceType type = resourceTypeFromName(resType.toString());
        if (type != TextResource) {
            ofParam.insert("0:CustomFontPath", SkyVariant::makeNull());
            ofParam.insert("0:SystemFontDir", SkyVariant::makeNull());
            ofParam.insert("0:SystemFontNames", SkyVariant::makeNull());
        } else {
            if (!ofParam.find("0:CustomFontPath").isNull()) {
                ofParam.insert("0:SystemFontDir", SkyVariant::makeNull());
                ofParam.insert("0:SystemFontNames", SkyVariant::makeNull());
            }
        }
    }
    SkyStringList keys = params.getAllKeys();
    for (int i = 0; i < keys.size(); i++) {
        const char *key = keys.c_strAt(i);
        SkyVariant value = params[key];
        if (value.getType() == SkyVariantType_String && value.hasPathTag()) {
            QString keyStr(key);
            QString path = value.toString();
            if (keyStr == "effectPath") {
                QFileInfo effectFileInfo(path);
                QString originalPath = effectFileInfo.absoluteDir().absolutePath();
                QString newPath = copyFile(params, key, originalPath, Effects);
                QString newFile = QFileInfo(newPath, effectFileInfo.fileName()).filePath();
                params.insert(key, SkyVariant::makePath(newFile.toUtf8().data()));
                break;
            }
            copyFile(ofParam, key, path, Other);
        }
    }
    SkyStringList ofkeys = ofParam.getAllKeys();
    for (int i = 0; i < ofkeys.size(); i++) {
        const char *key = ofkeys.c_strAt(i);
        SkyVariant value = ofParam[key];
        if (value.getType() == SkyVariantType_String && value.hasPathTag()) {
            QString keyStr(key);
            QString path = value.toString();
            if (keyStr == "0:CustomFontPath") {
                copyFile(ofParam, key, value.toString(), Font);
            } else if (keyStr == "0:SystemFontDir") {
                QString systemFontNames = ofParam.find("0:SystemFontNames").toString();
                QString newsystemFontDir;
                QStringList systemFonts = systemFontNames.split(",");
                for (int fontIndex = 0; fontIndex < systemFonts.size(); fontIndex++) {
                    QString systemFont = systemFonts.at(fontIndex);
                    QString originalPath = QFileInfo(path, systemFont).filePath();
                    copyFile(ofParam, key, originalPath, SystemFont);
                }
                break;
            } else if (keyStr == "0:EnterAnimDir") {
                copyFile(ofParam, key, path, Animations);
            } else if (keyStr == "0:ExitAnimDir") {
                copyFile(ofParam, key, path, Animations);
            }
            copyFile(ofParam, key, path, Other);
        }
    }
    effect->updateParams(params);
    //输入器路径
    SkyInputList *inputList = effect->getInputList();
    for (int i = 0; i < inputList->numberOfItems(); i++) {
        SkyInputItem *inputItem = inputList->getItemAt(i);
        QString path = inputItem->getResource().getPath().c_str();
        if (path.isEmpty() || path.startsWith("skymedia://")) {
            continue;
        }
        PreProcessRes inputPreRes = PreProcessRes();
        inputPreRes.originalPath = path;
        if (m_resList.contains(inputPreRes)) {
            auto listData = m_resList.find(inputPreRes);
            inputPreRes.type = listData->type;
            inputPreRes.newPath = listData->newPath;
        } else {
            getInputPreRes(inputPreRes);
            m_resList.insert(inputPreRes);
            FileUtils::copyFile(inputPreRes.originalPath, inputPreRes.newPath, true);
        }
        QString newPath = inputPreRes.newPath.remove(QFileInfo(m_rootDir).filePath() + "/");
        inputItem->setAsResource(SkyResource(newPath.toUtf8().data()));
    }
}

QString SkyPack::copyFile(SkyVariant &params, const char *key, QString path, Type type) {
    QString keyStr(key);
    QFileInfo fileInfo = QFileInfo(path);
    PreProcessRes preRes = PreProcessRes();
    preRes.originalPath = path;
    if (fileInfo.isDir()) {
        if (!m_resList.contains(preRes)) {
            switch (type) {
                case Effects:
                    getEffectsPreRes(preRes);
                    break;
                case Animations:
                    getAnimPreRes(preRes);
                    break;
                default:
                    getOtherDirPreRes(preRes);
                    break;
            }
            FileUtils::copyDirectoryFiles(preRes.originalPath, preRes.newPath, true);
            m_resList.insert(preRes);
        } else {
            auto listEffectsData = m_resList.find(preRes);
            preRes.type = listEffectsData->type;
            preRes.newPath = listEffectsData->newPath;
        }
        QString newPath = preRes.newPath.remove(m_tempOutputDir + "/");
        params.insert(key, SkyVariant::makePath(newPath.toUtf8().data()));
        return newPath;
    } else {
        if (!m_resList.contains(preRes)) {
            switch (type) {
                case Font:
                    getFontPreRes(preRes, false);
                    break;
                case SystemFont:
                    getFontPreRes(preRes, true);
                    break;
                default:
                    getOtherFilePreRes(preRes);
                    break;
            }
            FileUtils::copyFile(preRes.originalPath, preRes.newPath, true);
            m_resList.insert(preRes);
        } else {
            auto listFontData = m_resList.find(preRes);
            preRes.type = listFontData->type;
            preRes.newPath = listFontData->newPath;
        }
        QString newPath;
        if (type == SystemFont) {
            newPath = QFileInfo(preRes.newPath).path().remove(m_tempOutputDir + "/");
        } else {
            newPath = preRes.newPath.remove(m_tempOutputDir + "/");
        }
        params.insert(key, SkyVariant::makePath(newPath.toUtf8().data()));
        return newPath;
    }
}

void SkyPack::getVideoPreRes(PreProcessRes &preRes) {
    videoTypeIndex++;
    preRes.type = Video;
    preRes.newPath = m_resDir + "/video" + QString::number(videoTypeIndex) + ".mp4";
}

void SkyPack::getAudioPreRes(PreProcessRes &preRes) {
    audioTypeIndex++;
    preRes.type = Audio;
    preRes.newPath = m_resDir + "/audio" + QString::number(audioTypeIndex) + ".mp3";
}

void SkyPack::getImagePreRes(PreProcessRes &preRes) {
    imageTypeIndex++;
    preRes.type = Image;
    QString rootDir = QFileInfo(m_rootDir).filePath() + "/";
    QFileInfo originalFile(preRes.originalPath);
    QString suffix = originalFile.suffix().toLower();
    bool imagePre =
        !preRes.originalPath.startsWith(rootDir) && (suffix == "png" || suffix == "jpg" || suffix == "jpeg");
    preRes.newPath = m_resDir + "/image" + QString::number(imageTypeIndex) + "." + suffix;
    if (imagePre) {
        preRes.prePath = m_imagePreDir + "/image" + QString::number(imageTypeIndex) + "." + suffix;
    }
}

void SkyPack::getInputPreRes(PreProcessRes &preRes) {
    inputTypeIndex++;
    preRes.type = Input;
    preRes.newPath =
        m_resDir + "/input" + QString::number(inputTypeIndex) + "." + QFileInfo(preRes.originalPath).suffix();
}

void SkyPack::getEffectsPreRes(PreProcessRes &preRes) {
    effectsTypeIndex++;
    preRes.type = Effects;
    preRes.newPath = m_effectsDir + "/effects" + QString::number(effectsTypeIndex);
}

void SkyPack::getFontPreRes(PreProcessRes &preRes, bool systemFont) {
    preRes.type = Font;
    QFileInfo fileInfo(preRes.originalPath);
    if (systemFont) {
        preRes.newPath = m_fontsDir + "/SystemFont/" + fileInfo.fileName();
    } else {
        preRes.newPath = m_fontsDir + "/" + fileInfo.fileName();
    }
}

void SkyPack::getAnimPreRes(PreProcessRes &preRes) {
    animationsTypeIndex++;
    preRes.type = Animations;
    preRes.newPath = m_animatiosDir + "/animation" + QString::number(animationsTypeIndex);
}

void SkyPack::getOtherDirPreRes(PreProcessRes &preRes) {
    otherDirTypeIndex++;
    preRes.type = Other;
    preRes.newPath = m_resDir + "/other" + QString::number(otherDirTypeIndex);
}

void SkyPack::getOtherFilePreRes(PreProcessRes &preRes) {
    otherFileTypeIndex++;
    preRes.type = Other;
    preRes.newPath =
        m_resDir + "/other" + QString::number(otherFileTypeIndex) + "." + QFileInfo(preRes.originalPath).suffix();
}

void SkyPack::processTemplateType() {
    if (m_templateType != 0) {
        // not 电商
        return;
    }

    FashionlineInputListCreator creator(m_rootDir, m_tempOutputDir, m_newTimeline, m_fashionlineCfgDir);    
    connect(&creator,SIGNAL(exportLogInfo(int,QString)),this,SIGNAL(exportLogInfo(int,QString)));
    creator.createInputListFile();
}

void SkyPack::outputCover() {
    if (m_extrackCover == -1 || m_cancel) {
        return;
    }
    //如果外部传入的封面图有误，这里拿首帧
    if (m_extrackCover == 3 && !QFile::exists(m_coverPath)) {
        m_extrackCover = 1;
    }
    if (m_extrackCover != 3) {
        m_coverPath = QFileInfo(m_tempOutputDir, m_videoName + "-cover.jpg").filePath();
        SEProject::current()->dom()->timelineController()->setOutputMode(true);
        QImage image =
            SEProject::current()->dom()->previewController()->getSkyTimelineView()->snapFrame(m_extrackCover);
        SEProject::current()->dom()->timelineController()->setOutputMode(false);
        if (!image.isNull()) {
            if (QFile::exists(m_coverPath)) {
                QFile::remove(m_coverPath);
            }
            image.save(m_coverPath, "JPG");
            emit coverChanged(m_coverPath);
        }
    }
    updateProgress(1);
}

SkyVariant SkyPack::updateUserData(QSet<PreProcessRes> &preResList,
                                   SkyClip *clip,
                                   SkyVariant &resType,
                                   SkyVariant &resName,
                                   SkyVariant &resPath) {
    SkyVariant returnValue = SkyVariant::makeArray();
    if (!resPath.isNull()) {
        QString path = resPath.toString();
        if (path.length() == 0) {
            returnValue.append("");
            returnValue.append("");
            return returnValue;
        }
        if (!QFileInfo::exists(path)) {
            if (QFileInfo::exists(m_rootDir + "/" + path)) {
                path = m_rootDir + "/" + path;
            }
        }

        QFileInfo userDataFileInfo(path);
        PreProcessRes userDataPreRes = PreProcessRes();
        userDataPreRes.clip = clip;
        userDataPreRes.originalPath = userDataFileInfo.filePath();
        userDataPreRes.type = parseResourecType(resourceTypeFromName(resType.toString()));
        switch (userDataPreRes.type) {
            case Video:
            case Audio:
            case Image:
                break;
            default:
                userDataPreRes.originalPath = userDataFileInfo.absoluteDir().absolutePath();
                break;
        }
        if (!preResList.contains(userDataPreRes)) {
            switch (userDataPreRes.type) {
                case Video:
                    getVideoPreRes(userDataPreRes);
                    execProcess(userDataPreRes);
                    break;
                case Audio:
                    getAudioPreRes(userDataPreRes);
                    execProcess(userDataPreRes);
                    break;
                case Image:
                    getImagePreRes(userDataPreRes);
                    execProcess(userDataPreRes);
                    break;
                default:
                    getEffectsPreRes(userDataPreRes);
                    FileUtils::copyDirectoryFiles(userDataPreRes.originalPath, userDataPreRes.newPath, true);
                    break;
            }
            preResList.insert(userDataPreRes);
        } else {
            auto listEffectsData = preResList.find(userDataPreRes);
            userDataPreRes.type = listEffectsData->type;
            userDataPreRes.newPath = listEffectsData->newPath;
        }
        QString newPath = userDataPreRes.newPath;
        switch (userDataPreRes.type) {
            case Video:
                newPath.remove(m_tempOutputDir + "/");
                break;
            case Audio:
                newPath.remove(m_tempOutputDir + "/");
                break;
            case Image:
                newPath.remove(m_tempOutputDir + "/");
                break;
            default:
                newPath.remove(m_tempOutputDir + "/").append(+"/" + userDataFileInfo.fileName());
                break;
        }
        QString fileExtension = FileUtils::getFileExtension(resName.toString());
        if (!fileExtension.isEmpty() && fileExtension == FileUtils::getFileExtension(userDataPreRes.newPath)) {
            QFileInfo userDataName(userDataPreRes.newPath);
            returnValue.append(userDataName.fileName().toUtf8().data());
        } else {
            returnValue.append(resName);
        }
        returnValue.append(SkyVariant::makePath(newPath.toUtf8().data()));
    }
    return returnValue;
}

Type SkyPack::parseResourecType(ResourceType resourceType) {
    switch (resourceType) {
        case ResourceType::VideoResource:
            return Video;
        case ResourceType::AudioResource:
            return Audio;
        case ResourceType::ImageResource:
            return Image;
        case ResourceType::EffectResource:
            return Effects;
        case ResourceType::TextResource:
            return Font;
        default:
            return Effects;
    }
}

void SkyPack::imageMin() {
    FileUtils::copyDirectoryFiles(m_imagePreDir, m_resDir, true);
}

void SkyPack::ofluaToLua(const QString &path) {
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        QDir dir(path);
        QFileInfoList localEntryInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        for (int i = 0; i < localEntryInfoList.size(); ++i) {
            auto filePath = localEntryInfoList.at(i).absoluteFilePath();
            ofluaToLua(filePath);
        }
    } else if (fileInfo.suffix() == "oflua") {
        QString dst = QFileInfo(fileInfo.dir().path(), fileInfo.completeBaseName() + ".lua").filePath();
        if (LuaUtil::decrypt(path.toStdString(), dst.toStdString())) {
            QFile::remove(path);
        }
    } else if (fileInfo.suffix() == "ofeffect") {
        QString newLua = FileUtils::read(fileInfo.filePath()).replace(".oflua", ".lua");
        FileUtils::write(fileInfo.filePath(), newLua);
    }
}

bool SkyPack::containsResList(const QSet<PreProcessRes> &resList, const PreProcessRes &res) {
    return resList.contains(res);
}

QString SkyPack::getResPathFromList(const QSet<PreProcessRes> &resList, const PreProcessRes &res) {
    return resList.find(res)->newPath;
}

void SkyPack::luaToOflua(const QString &path) {
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        QDir dir(path);
        QFileInfoList localEntryInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        for (int i = 0; i < localEntryInfoList.size(); ++i) {
            auto filePath = localEntryInfoList.at(i).absoluteFilePath();
            luaToOflua(filePath);
        }
    } else if (fileInfo.suffix() == "lua") {
        QString dst = QFileInfo(fileInfo.dir().path(), fileInfo.completeBaseName() + ".oflua").filePath();
        if (LuaUtil::encrypt(path.toStdString(), dst.toStdString())) {
            QFile::remove(path);
        }
    }
}

void SkyPack::outputVideo(QString videoPath) {
    if (!m_extrackVideo || m_cancel) {
        return;
    }
    QString tempPath = QFileInfo(m_tempOutputDir, m_videoName + ".mp4").filePath();
    SkyEncodingCallback callback;
    callback.userData = this;
    callback.onError = SkyPack::onError;
    callback.onFinish = SkyPack::onFinish;
    callback.shouldBeCancelled = SkyPack::shouldBeCancelled;
    callback.onProgress = SkyPack::onProgress;

    int64_t videoBitRate = getBitRate(m_newTimeline);

    SEProject::current()->dom()->timelineController()->setOutputMode(true);
    SEProject::current()->dom()->previewController()->getSkyTimelineView()->outputVideo(
        videoBitRate, tempPath, callback);
    SEProject::current()->dom()->timelineController()->setOutputMode(false);
    //设置视频封面
    if (QFile::exists(m_coverPath)) {
        m_process->setProgram(ffmpegProgram());
        m_process->setArguments(QStringList() << "-y"
                                              << "-v"
                                              << "quiet"
                                              << "-i" << tempPath << "-i" << m_coverPath << "-map"
                                              << "1"
                                              << "-map"
                                              << "0"
                                              << "-c"
                                              << "copy"
                                              << "-disposition:0"
                                              << "attached_pic" << videoPath);
        execProcessStart();
    } else {
        QFile::rename(tempPath, videoPath);
    }
}

void SkyPack::namedSkyTimeline(SkyTimeline *timeline) {
    int namedIndex = 0;
    auto subObjects = timeline->getAllSubObjects();
    for (int i = 0; i < subObjects.size(); i++) {
        namedSkyObject(subObjects[i], ++namedIndex);
    }
}

void SkyPack::namedSkyObject(SkyObject *obj, int index) {
    if (obj == nullptr) {
        return;
    }
    auto type = obj->getObjectType();
    QString objType = "";
    switch (type) {
        case SkyObjectType_Timeline:
            objType = "Timeline";
            break;
        case SkyObjectType_Track:
            objType = "Track";
            break;
        case SkyObjectType_Clip:
            objType = "Clip";
            break;
        case SkyObjectType_Effect:
            objType = "Effect";
            break;
        case SkyObjectType_Transition:
            objType = "Transition";
            break;
        default:
            objType = "None";
    }
    QString objName = QString("%1_%2").arg(objType).arg(index);
    obj->setName(objName.toUtf8().data());
}
