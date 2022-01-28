#include "ExportDialogDataSource.h"

#include <QJsonDocument>
#include "base/http/BaseRsp.h"
#include "base/http/SEHttpClient.h"
#include "base/utils/AppConstance.h"
#include "base/utils/AppPath.h"
#include "base/utils/UploadUtils.h"
#include "base/utils/threadpools.h"
#include "base/utils/unitconverutils.h"
#include "project/SEProject.h"
#include "user/Login.h"
#include <user/LoginController.h>

using namespace std;
ExportDialogDataSource::ExportDialogDataSource(QObject *parent) : QObject(parent) {
    MMKV *mmkv = MMKV::mmkvWithID((getTitle() + ".export").toStdString());
    m_sharedPref = new SharedPref(mmkv);
    removeTempFile();
    getHttpData();
    LoginController* loginController = LoginController::instance();
    if(loginController){
        connect(loginController, &LoginController::loginSuccess, [=] {
            getHttpData();
        });
    }

}

ExportDialogDataSource::~ExportDialogDataSource() {

    qDebug() << "ExportDialogDataSource::~ExportDialogDataSource";
    delete m_sharedPref;
    m_sharedPref = nullptr;
    if (!SEProject::isQuit()) {
        SEProject::current()->dom()->setExportSource(nullptr);
    }
    removeTempFile();
}

/*
 *获取模板类型和团队模板
 */
void ExportDialogDataSource::getHttpData() {
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/backend/listSourceTypes").get([=](const QString &response) {
        BaseRsp rsp(response);
        if (rsp.isSuccess()) {
            QString lastJson = m_sharedPref->readString("export_template_type_data");
            QJsonValue templateJson = rsp.data().toObject().take("template");
            QJsonArray tempArray = templateJson.toArray();
            QString json = QJsonDocument(tempArray).toJson();
            if (json != lastJson) {
                m_sharedPref->setString("export_template_type_data", json);
                QVariantList list;
                for (int i = 0; i < tempArray.size(); i++) {
                    auto item = tempArray.at(i);
                    list.append(item.toObject().take("label").toString());
                }
                emit templateTypeChanged(list);
            }
        }
    });
    QString otp = QString::fromStdString(Login::instance()->getOtp(AppConstance::uid()));
    if (Login::instance()->isLogin() && !otp.isEmpty()) {
        SEHttpClient(AppConstance::serverDomain() + "/v1/sky/team/listUserTeamsInVms")
            .addParam("sign", otp)
            .get([=](const QString &response) {
                BaseRsp rsp(response);
                if (rsp.isSuccess()) {
                    QString lastJson = SharedPref::instance()->readString("export_template_team_data");
                    QVariantList list;
                    list.append(tr("Public template library"));
                    teamIds.clear();
                    teamIds.append("0");
                    QJsonArray array = rsp.data().toArray();
                    QString json = QJsonDocument(array).toJson();
                    if (json != lastJson) {
                        SharedPref::instance()->setString("export_template_team_data", json);
                        for (int i = 0; i < array.size(); i++) {
                            QJsonValue item = array.at(i);
                            QJsonValue teamJson = item.toObject().take("team");
                            QJsonValue memberJson = item.toObject().take("member");
                            QJsonObject teamObj = teamJson.toObject();
                            QJsonObject memberObj = memberJson.toObject();
                            int role = memberObj.take("role").toInt();
                            if (role == 1 || role == 2) {
                                list.append(teamObj.take("name").toString());
                                teamIds.append(teamObj.take("id").toString());
                            }
                        }
                        emit templateTeamChanged(list);
                    }
                }
            });
    }
}

bool ExportDialogDataSource::isPacking() {
    void *source = SEProject::current()->dom()->exportSource();
    if (source == nullptr) {
        return false;
    }
    ExportDialogDataSource *exportSource = (ExportDialogDataSource *)source;
    return exportSource->m_skyPack != nullptr && exportSource->m_skyPack->isPacking();
}

void ExportDialogDataSource::onQmlLoaded() {
    SEProject::current()->dom()->setExportSource(this);
}

void ExportDialogDataSource::uploadLocalCover(QString coverPath) {
    m_coverPath = QUrl(coverPath).toLocalFile();
    emit updateCover(0, m_coverPath);
}

void ExportDialogDataSource::uploadTimelineCover(int coverType) {
    if (coverType <= 0) {
        return;
    }
    int type = 0;
    if (coverType == 1) {
        type = 1;
    } else if (coverType == 2) {
        type = 0;
    } else if (coverType == 3) {
        type = 2;
    }
    QString coverPath = AppPath::tempPath("export_cover" + QString::number(type) + ".jpg");
    if (QFile::exists(coverPath)) {
        m_coverPath = coverPath;
        emit updateCover(coverType, m_coverPath);
        return;
    }
    ThreadPools::instance()->exec([this, coverType, type, coverPath] {
        SEProject::current()->dom()->timelineController()->setOutputMode(true);
        QImage image = SEProject::current()->dom()->previewController()->getSkyTimelineView()->snapFrame(type);
        SEProject::current()->dom()->timelineController()->setOutputMode(false);
        if (!image.isNull()) {
            image.save(coverPath, "JPG");
            m_coverPath = coverPath;
            emit updateCover(coverType, m_coverPath);
        }
    });
}

void ExportDialogDataSource::setOutput(QString output) {
    m_sharedPref->setString("export_output_path", output);
    SharedPref::instance()->setString("export_output_path", output);
}

QString ExportDialogDataSource::getOutput() {
    QString outputPath = m_sharedPref->readString("export_output_path");
    if (!outputPath.isEmpty()) {
        return outputPath;
    }
    outputPath = SharedPref::instance()->readString("export_output_path");
    if (!outputPath.isEmpty()) {
        return outputPath;
    }
    return AppPath::output();
}

void ExportDialogDataSource::setVideoName(QString videoName) {
    m_sharedPref->setString("export_video_name", videoName);
}

QString ExportDialogDataSource::getVideoName() {
    QString videoName = m_sharedPref->readString("export_video_name");
    if (!videoName.isEmpty()) {
        return videoName;
    }
    return getTitle() + "_video";
}

void ExportDialogDataSource::setCoverType(int coverType) {
    m_sharedPref->setInt("export_cover_type", coverType);
}

// -1不导出封面，1本地，2首帧，3当前帧，4尾帧
int ExportDialogDataSource::getCoverType() {
    int coverType = m_sharedPref->readInt("export_cover_type", -1);
    if (coverType == 0) {
        QString coverPath = getCoverPath();
        if (!QFile::exists(coverPath)) {
            coverType = -1;
        }
    }
    uploadTimelineCover(coverType);
    return coverType;
}

void ExportDialogDataSource::setCoverPath(QString coverPath) {
    m_sharedPref->setString("export_cover_path", coverPath);
}

QString ExportDialogDataSource::getCoverPath() {
    return m_sharedPref->readString("export_cover_path");
}

void ExportDialogDataSource::setOutputPackCheck(bool checked) {
    m_sharedPref->setBool("export_output_pack_check", checked);
}

bool ExportDialogDataSource::getOutputPackCheck() {
    return m_sharedPref->readBool("export_output_pack_check", false);
}

void ExportDialogDataSource::setOutputVMSCheck(bool checked) {
    m_sharedPref->setBool("export_output_vms_check", checked);
}

bool ExportDialogDataSource::getOutputVMSCheck() {
    return m_sharedPref->readBool("export_output_vms_check", false);
}

void ExportDialogDataSource::setTemplateOverwriteCheck(bool checked) {
    m_sharedPref->setBool("export_template_overwrite_check", checked);
}

bool ExportDialogDataSource::getTemplateOverwriteCheck() {
    return m_sharedPref->readBool("export_template_overwrite_check", true);
}

void ExportDialogDataSource::setTemplateCnName(QString cnName) {
    m_sharedPref->setString("export_template_cn_name", cnName);
}

QString ExportDialogDataSource::getTemplateCnName() {
    return m_sharedPref->readString("export_template_cn_name");
}

void ExportDialogDataSource::setTemplateEnName(QString enName) {
    m_sharedPref->setString("export_template_en_name", enName);
}

QString ExportDialogDataSource::getTemplateEnName() {
    return m_sharedPref->readString("export_template_en_name");
}

QVariantList ExportDialogDataSource::getTemplateTypeData() {
    QVariantList list;
    QString json = m_sharedPref->readString("export_template_type_data");
    if (json.isEmpty()) {
        list.append(tr("E-commerce"));
        list.append(tr("Short video"));
        return list;
    }
    QJsonArray array = QJsonDocument::fromJson(json.toUtf8()).array();
    for (int i = 0; i < array.size(); i++) {
        auto item = array.at(i);
        list.append(item.toObject().take("label").toString());
    }
    return list;
}

QVariantList ExportDialogDataSource::getTemplateTeamData() {
    QVariantList list;
    list.append(tr("Public template library"));
    teamIds.clear();
    teamIds.append("0");
    QString json = SharedPref::instance()->readString("export_template_team_data");
    if (json.isEmpty()) {
        return list;
    }
    QJsonArray array = QJsonDocument::fromJson(json.toUtf8()).array();
    for (int i = 0; i < array.size(); i++) {
        QJsonValue item = array.at(i);
        QJsonValue teamJson = item.toObject().take("team");
        QJsonValue memberJson = item.toObject().take("member");
        QJsonObject teamObj = teamJson.toObject();
        QJsonObject memberObj = memberJson.toObject();
        int role = memberObj.take("role").toInt();
        if (role == 1 || role == 2) {
            list.append(teamObj.take("name").toString());
            teamIds.append(teamObj.take("id").toString());
        }
    }
    return list;
}

void ExportDialogDataSource::setTemplateType(int index) {
    m_sharedPref->setInt("export_template_type", index);
}

// 0是电商，1是短视频
int ExportDialogDataSource::getTemplateType() {
    return m_sharedPref->readInt("export_template_type", 1);
}

void ExportDialogDataSource::setTemplateTeam(int index) {
    m_sharedPref->setInt("export_template_team", index);
}

int ExportDialogDataSource::getTemplateTeam() {
    return m_sharedPref->readInt("export_template_team", 0);
}

QString ExportDialogDataSource::getE_CommerceCfgPath() {
    return m_sharedPref->readString("Export_E_CommerceCfgPath_" + SEProject::current()->proj()->id());
}

void ExportDialogDataSource::setE_CommerceCfgPath(QString cfgDirPath) {
    QString key = "Export_E_CommerceCfgPath_" + SEProject::current()->proj()->id();
    m_sharedPref->setString(key, cfgDirPath);
}

QString ExportDialogDataSource::optimizeOutputFolder(QString outputAddress, QString outputFolder) {
    QString path = QFileInfo(outputAddress, outputFolder).filePath();
    if (!QFile::exists(path)) {
        return outputFolder;
    }
    for (int i = 2; i < 100; i++) {
        QString newPath = path + "(" + QString::number(i) + ")";
        if (!QFile::exists(newPath)) {
            return outputFolder + "(" + QString::number(i) + ")";
        }
    }
    return outputFolder;
}

QString ExportDialogDataSource::getCover() {
    if (QFile::exists(m_coverPath)) {
        return m_coverPath;
    }
    QString coverPath = SEProject::current()->proj()->thumb();
    if (!QFile::exists(coverPath)) {
        SEProject::current()->save();
    }
    if (QFile::exists(coverPath)) {
        return coverPath;
    } else {
        return nullptr;
    }
}

QString ExportDialogDataSource::getTitle() {
    return SEProject::current()->title();
}

double ExportDialogDataSource::getDuration() {
    return SEProject::current()->dom()->previewController()->during();
}

QString ExportDialogDataSource::getBitRate() {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    int64_t videoBitRate = SkyPack::getBitRate(timeline);
    return QString("%0kbps").arg(videoBitRate / 1000);
}

int ExportDialogDataSource::getFps() {
    return SEProject::current()->dom()->timeline()->getVideoParams().frameRate;
    ;
}

QString ExportDialogDataSource::getFrame() {
    auto videoParams = SEProject::current()->dom()->timeline()->getVideoParams();
    return QString("%0x%1").arg(videoParams.width).arg(videoParams.height);
}

void ExportDialogDataSource::updatePackSize() {
    //只获取视频大小
    bool cover = false;
    bool pack = false;
    bool video = true;
    int64_t size = 0;
    if (cover) {
        QString coverPath = SEProject::current()->rootPath() + "/cover.jpg";
        if (!QFile::exists(coverPath)) {
            SEProject::current()->save();
        }
        QFile file(coverPath);
        size += file.size();
    }
    if (video) {
        SkyTimeline *timeline = SEProject::current()->dom()->timeline();
        SkyEncodingParams encodingParams;
        size += (SkyPack::getBitRate(timeline) + encodingParams.audioBitRate) * getDuration() / 8;
    }
    if (pack) {
        QString packPath = AppPath::tempPath("pack.zip");
        if (!QFile::exists(packPath)) {
            m_lastCover = cover;
            m_lastVideo = video;
            m_lastPack = pack;
            auto skyPack = SkyPack::pack(AppPath::tempPath("pack"), "", "pack", "", -1, false, true, 0, "");
            QObject::connect(skyPack, &SkyPack::finishChanged, [=](QString &, QString &, QString &) {
                updatePackSize();
            });
            return;
        } else {
            QFile file(packPath);
            size += file.size();
        }
    }
    emit packSizeChanged(UnitConverUtils::formatSize(size));
}

void ExportDialogDataSource::cancel() {
    m_cancel = true;
    if (m_skyPack) {
        m_skyPack->cancel();
    }
}

bool ExportDialogDataSource::hasClip() {
    return SEProject::current()->dom()->timelineController()->hasClip();
}

QVariantList ExportDialogDataSource::inputDeviceList() {
    auto timelineModel = SEProject::current()->dom()->timelineController()->timelineModel();
    auto list = timelineModel->inputDeviceList();
    QMap<QString, QList<int>> data;
    QList<BaseClip *> keys = list.keys();
    for (int i = 0; i < keys.size(); i++) {
        BaseClip *clip = keys.at(i);
        InputDevice input = list.value(clip);
        QJsonObject json = input.toJson();
        if (input.getType() != "string") {
            json.remove("id");
        }
        QString key = QJsonDocument(json).toJson(QJsonDocument::Compact);
        QList<int> clips = data.value(key);
        clips.append(i);
        data.insert(key, clips);
    }
    QVariantList returnList;
    qint64 _id = QDateTime::currentDateTime().toMSecsSinceEpoch();
    for (QString key : data.keys()) {
        auto jsonObject = QJsonDocument::fromJson(key.toUtf8());
        if (jsonObject.isNull()) {
            continue;
        }
        InputDevice input = InputDevice::makeInputDeviceFromJson(jsonObject.object());
        qint64 id = _id++;
        input.setId(id);
        for (int clipIndex : data.value(key)) {
            input.addClipId(keys.at(clipIndex)->getInputId());
        }
        m_inputDeviceList.append(input);
        returnList.push_back(id);
    }
    return returnList;
}

QString ExportDialogDataSource::getFileNameFromId(qint64 id) {
    for (InputDevice inputDevice : m_inputDeviceList) {
        if (inputDevice.getId() == id) {
            return inputDevice.getTitle();
        }
    }
    return "";
}

QString ExportDialogDataSource::getJsonFromId(qint64 id) {
    for (InputDevice inputDevice : m_inputDeviceList) {
        if (inputDevice.getId() == id) {
            QString json = QJsonDocument(inputDevice.toJson()).toJson(QJsonDocument::JsonFormat::Indented);
            return json.remove(QRegExp("\\s"));
        }
    }
    return "";
}

void ExportDialogDataSource::startExport(QString savePath,
                                         QString videoName,
                                         int coverType,
                                         bool exportTemplate,
                                         bool doContribute,
                                         QVariantList inputDeviceList,
                                         QString templateCnName,
                                         QString templateEnName,
                                         int templateType,
                                         int templateTeam,
                                         bool templateOverwrite,
                                         QString eCommerceCfgPath) {
    if (savePath.isEmpty()) {
        if (coverType >= 0 || exportTemplate) {
            savePath = QFileInfo(getOutput(), getTitle()).filePath();
        } else {
            savePath = getOutput();
        }
    }
    if (videoName.isEmpty()) {
        videoName = getVideoName();
    }
    QString packName = "pack";
    if (!templateCnName.isEmpty()) {
        packName = templateCnName;
    } else if (!templateEnName.isEmpty()) {
        packName = templateEnName;
    }
    QString outputDir = QFileInfo(AppPath::tempPath("pack"), SEProject::current()->title()).filePath();

    m_skyPack = SkyPack::pack(outputDir,
                              videoName,
                              packName,
                              m_coverPath,
                              coverType > 0 ? 3 : -1,
                              true,
                              exportTemplate,
                              templateType,
                              eCommerceCfgPath);
    m_skyPack->setSharedPref(m_sharedPref);
    connect(m_skyPack, SIGNAL(coverChanged(QString)), this, SIGNAL(coverChanged(QString)));
    connect(m_skyPack, SIGNAL(progressChanged(double)), this, SIGNAL(exportProgressChanged(double)));
    connect(m_skyPack, SIGNAL(cancelChanged()), this, SIGNAL(exportCancel()));
    connect(m_skyPack, SIGNAL(errorChanged()), this, SIGNAL(exportError()));
    connect(m_skyPack, SIGNAL(exportLogInfo(int, QString)), this, SLOT(onExportLogInfo(int, QString)));

    //复制输入器配置
    QObject::connect(m_skyPack,
                     &SkyPack::outputPackFinishChanged,
                     [this, inputDeviceList](QString &baseDir, QSet<PreProcessRes> &resList, SkyTimeline *timeline) {
                         if (m_cancel || inputDeviceList.isEmpty()) {
                             return;
                         }
                         auto timelineModel = SEProject::current()->dom()->timelineController()->timelineModel();
                         auto inputList = timelineModel->inputDeviceList();
                         auto clipList = inputList.values();
                         m_skyPack->namedSkyTimeline(SEProject::current()->dom()->timeline());
                         for (auto json : inputDeviceList) {
                             InputDevice inputDevice = InputDevice::makeInputDeviceFromJson(
                                 QJsonDocument::fromJson(json.toString().toUtf8()).object());
                             PreProcessRes res = PreProcessRes();
                             res.originalPath = inputDevice.getPath();
                             if (m_skyPack->containsResList(resList, res)) {
                                 QString newPath = m_skyPack->getResPathFromList(resList, res);
                                 inputDevice.setPath(newPath.remove(baseDir + "/"));
                             } else {
                                 res.originalPath = QFileInfo(inputDevice.getPath()).dir().path();
                                 if (m_skyPack->containsResList(resList, res)) {
                                     QString newPath = m_skyPack->getResPathFromList(resList, res);
                                     inputDevice.setPath(newPath.remove(baseDir + "/"));
                                 }
                             }
                             for (BaseClip *clip : inputList.keys()) {
                                 if (inputDevice.equalsClipId(clip->getInputId())) {
                                     //设置文本参数
                                     if (clip->resourceBean().resouceType() == TextResource) {
                                         for (auto effect : clip->effectList()) {
                                             QString resType = effect->getUserData("ResType");
                                             ResourceType type = resourceTypeFromName(resType.toUtf8());
                                             if (type == TextResource) {
                                                 inputDevice.addObjName(effect->skyEffect()->getName().c_str());
                                                 break;
                                             }
                                         }
                                         inputDevice.addUpdateParam("filterIndex", 0);
                                         inputDevice.addUpdateParam("paramKey", "Text");
                                         inputDevice.addUpdateParam("value", clip->wordContent());
                                     } else {
                                         inputDevice.addObjName(clip->skyClip()->getName().c_str());
                                     }
                                 }
                             }
                             m_inputList.push_back(inputDevice.toJson());
                         }
                         QFile inputListConf(baseDir + "/inputList.conf");
                         if (inputListConf.open(QIODevice::WriteOnly)) {
                             inputListConf.write(QJsonDocument(m_inputList).toJson());
                             inputListConf.flush();
                             inputListConf.close();
                         }
                     });
    QObject::connect(m_skyPack,
                     &SkyPack::finishChanged,
                     [this,
                      savePath,
                      packName,
                      videoName,
                      coverType,
                      exportTemplate,
                      doContribute,
                      templateCnName,
                      templateEnName,
                      templateType,
                      templateTeam,
                      templateOverwrite](QString &, QString &videoPath, QString &packPath) {
                         if (m_cancel) {
                             emit exportCancel();
                             return;
                         }
                         QString videoOutputPath = QFileInfo(savePath, videoName + ".mp4").filePath();
                         QString packOutputPath = QFileInfo(savePath, packName + ".zip").filePath();
                         if (coverType >= 0 || exportTemplate) {
                             QDir targetDir(savePath);
                             if (!targetDir.exists() && !targetDir.mkdir(targetDir.absolutePath())) {
                                 return;
                             }
                             FileUtils::copyFile(packPath, packOutputPath, true);
                         }
                         FileUtils::copyFile(videoPath, videoOutputPath, true);
                         if (coverType >= 0) {
                             //复制封面
                             QFileInfo coverFile(getCover());
                             QString coverPath =
                                 QFileInfo(savePath, videoName + "-cover." + coverFile.suffix()).filePath();
                             FileUtils::copyFile(coverFile.filePath(), coverPath, true);
                         }
                         //删除临时导出文件夹
                         QDir packDir(AppPath::tempPath("pack"));
                         packDir.removeRecursively();
                         if (exportTemplate && doContribute) {
                             uploadPack(savePath,
                                        QFileInfo(savePath, packName + ".zip").filePath(),
                                        videoOutputPath,
                                        m_inputList,
                                        templateCnName,
                                        templateEnName,
                                        templateType,
                                        templateTeam < teamIds.size() ? teamIds.at(templateTeam) : "0",
                                        templateOverwrite);
                         } else {
                             emit exportFinishChanged(savePath);
                         }
                     });
}

void ExportDialogDataSource::removeTempFile() {
    for (int i = 0; i < 3; i++) {
        QString coverPath = AppPath::tempPath("export_cover" + QString::number(i) + ".jpg");
        if (QFile::exists(coverPath)) {
            QFile::remove(coverPath);
        }
    }
}

void ExportDialogDataSource::uploadPack(QString savePath,
                                        QString templateFile,
                                        QString videoFile,
                                        QJsonArray inputList,
                                        QString zhName,
                                        QString enName,
                                        int templateType,
                                        QString teamId,
                                        bool overwrite) {
    m_bytesSent1 = m_bytesSent2 = m_bytesSent3 = 0;
    m_contributeUploadFlag = 0;
    ThreadPools::instance()->mainThread(
        [=] {
            qint64 fileBytesTotal = QFile(templateFile).size() + QFile(videoFile).size() + QFile(getCover()).size();
            UploadUtils::upload(
                templateFile,
                [=](qint64 bytesSent, qint64) mutable {
                    if (m_cancel) {
                        return;
                    }
                    if (bytesSent > 0) {
                        m_bytesSent1 = bytesSent;
                        qint64 bytesSentTotal = m_bytesSent1 + m_bytesSent2 + m_bytesSent3;
                        if (bytesSentTotal >= 0 && fileBytesTotal > 0) {
                            emit contributeProgressChanged((double)bytesSentTotal / fileBytesTotal);
                        }
                    }
                },
                [=](const bool &success, const QString &packUrl) {
                    if (m_cancel) {
                        return;
                    }
                    if (success) {
                        contribute(savePath,
                                   packUrl,
                                   nullptr,
                                   nullptr,
                                   inputList,
                                   zhName,
                                   enName,
                                   templateType,
                                   teamId,
                                   overwrite);
                    } else {
                        emit exportError();
                    }
                });
            UploadUtils::upload(
                videoFile,
                [=](qint64 bytesSent, qint64) mutable {
                    if (m_cancel) {
                        return;
                    }
                    if (bytesSent > 0) {
                        m_bytesSent2 = bytesSent;
                        qint64 bytesSentTotal = m_bytesSent1 + m_bytesSent2 + m_bytesSent3;
                        if (bytesSentTotal >= 0 && fileBytesTotal > 0) {
                            emit contributeProgressChanged((double)bytesSentTotal / fileBytesTotal);
                        }
                    }
                },
                [this, savePath, inputList, zhName, enName, templateType, teamId, overwrite](const bool &success,
                                                                                             const QString &videoUrl) {
                    if (m_cancel) {
                        return;
                    }
                    if (success) {
                        contribute(savePath,
                                   nullptr,
                                   nullptr,
                                   videoUrl,
                                   inputList,
                                   zhName,
                                   enName,
                                   templateType,
                                   teamId,
                                   overwrite);
                    } else {
                        emit exportError();
                    }
                });
            UploadUtils::upload(
                getCover(),
                [=](qint64 bytesSent, qint64) mutable {
                    if (m_cancel) {
                        return;
                    }
                    if (bytesSent > 0) {
                        m_bytesSent3 = bytesSent;
                        qint64 bytesSentTotal = m_bytesSent1 + m_bytesSent2 + m_bytesSent3;
                        if (bytesSentTotal >= 0 && fileBytesTotal > 0) {
                            emit contributeProgressChanged((double)bytesSentTotal / fileBytesTotal);
                        }
                    }
                },
                [this, savePath, inputList, zhName, enName, templateType, teamId, overwrite](const bool &success,
                                                                                             const QString &coverUrl) {
                    if (m_cancel) {
                        return;
                    }
                    if (success) {
                        contribute(savePath,
                                   nullptr,
                                   coverUrl,
                                   nullptr,
                                   inputList,
                                   zhName,
                                   enName,
                                   templateType,
                                   teamId,
                                   overwrite);
                    } else {
                        emit exportError();
                    }
                });
        },
        0);
}

void ExportDialogDataSource::contribute(QString savePath,
                                        QString packUrl,
                                        QString coverUrl,
                                        QString videoUrl,
                                        QJsonArray inputList,
                                        QString zhName,
                                        QString enName,
                                        int templateType,
                                        QString teamId,
                                        bool overwrite) {
    if (!packUrl.isEmpty()) {
        m_packUrl = packUrl;
    }
    if (!coverUrl.isEmpty()) {
        m_coverUrl = coverUrl;
    }
    if (!videoUrl.isEmpty()) {
        m_videoUrl = videoUrl;
    }
    if (++m_contributeUploadFlag < 3) {
        return;
    }
    QJsonObject params;
    params.insert("id", SEProject::current()->proj()->remoteId());
    params.insert("cover", m_coverUrl);
    params.insert("sourceUrl", m_packUrl);
    params.insert("sourcePreview", m_videoUrl);
    params.insert("type", templateType);
    QJsonObject languageName;
    languageName.insert("zh", zhName);
    languageName.insert("en", enName);
    params.insert("languageName", languageName);
    QJsonObject extConf;
    extConf.insert("inputList", inputList);
    params.insert("extConf", extConf);
    params.insert("teamId", teamId);
    params.insert("overwrite", overwrite);
    qInfo() << "ExportDialogDataSource,contribute_request:" << params;
    QString otp = QString::fromStdString(Login::instance()->getOtp(AppConstance::uid()));
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/template/saveOrUpdateTemplate?sign=" + otp)
        .addJsonParam(params)
        .post([=](const QString &response) {
            BaseRsp rsp(response);
            qInfo() << "ExportDialogDataSource,contribute_response:" << response;
            if (rsp.isSuccess()) {
                SEProject::current()->proj()->setRemoteId(rsp.data().toObject().take("id").toString());
            }
        });
    emit exportFinishChanged(savePath);
}

void ExportDialogDataSource::onExportLogInfo(int type, QString log) {
    switch (type) {
        case QtWarningMsg:
            qWarning() << log;
            exportLogs.append("Warning: " + log);
            break;
        case QtFatalMsg:
            qWarning() << log;
            exportLogs.append("Fatal: " + log);
            break;
        case QtInfoMsg:
            qInfo() << log;
            break;
        case QtDebugMsg:
            qDebug() << log;
            break;
        default:
            break;
    }
}
