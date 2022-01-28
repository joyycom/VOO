#include "StoreDataSource.h"
#include "StoreCategoryModel.h"
#include "StoreResourceModel.h"
#include "base/http/SEHttpClient.h"
#include "base/http/BaseRsp.h"
#include "base/utils/SharedPref.h"
#include "base/utils/AppConstance.h"
#include "base/utils/JsonUtils.h"
#include "base/utils/fileutils.h"
#include "base/utils/threadpools.h"
#include "extension/VooSocketServer.h"
#include "extension/VSRun.h"
#include "user/Login.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDir>
#include <QTextCodec>

StoreDataSource::StoreDataSource(QObject *parent) : QObject(parent) {
}

StoreDataSource::~StoreDataSource() {
    m_allCategorys.clear();
    m_allResources.clear();
    if (m_curCategoryListModel) {
        delete m_curCategoryListModel;
        m_curCategoryListModel = nullptr;
    }
    if (m_currentResources) {
        delete m_currentResources;
        m_currentResources = nullptr;
    }
    if (m_localVideoResource) {
        delete m_localVideoResource;
        m_localVideoResource = nullptr;
    }
    if (m_localAudioResource) {
        delete m_localAudioResource;
        m_localAudioResource = nullptr;
    }
    if (m_localMoreResource) {
        delete m_localMoreResource;
        m_localMoreResource = nullptr;
    }
}

void StoreDataSource::loadData() {
    //读取缓存视频、音频资源
    QJsonArray ja_video = SharedPref::instance()->readJsonArray("cache_video_local");
    m_localVideoResource->clearAll();
    LocalResourceModel emptyVideo;
    m_localVideoResource->addData(emptyVideo);
    for (int i = 0; i < ja_video.size(); i++) {
        LocalResourceModel model(ja_video.at(i).toObject());
        m_localVideoResource->addData(model);
    }

    m_localAudioResource->clearAll();
    QJsonArray ja_audio = SharedPref::instance()->readJsonArray("cache_audio_local");
    LocalResourceModel emptyAudio;
    m_localAudioResource->addData(emptyAudio);
    for (int i = 0; i < ja_audio.size(); i++) {
        LocalResourceModel model(ja_audio.at(i).toObject());
        m_localAudioResource->addData(model);
    }

    QJsonArray ja_more = SharedPref::instance()->readJsonArray("cache_more_local");
    m_localMoreResource->clearAll();
    LocalResourceModel emptyMore;
    m_localMoreResource->addData(emptyMore);
    for (int i = 0; i < ja_more.size(); i++) {
        LocalResourceModel model(ja_more.at(i).toObject());
        m_localMoreResource->addData(model);
    }

    //读取本地配置+远端配置
    QString cacheResponse = SharedPref::instance()->readString("cache_store_cate_config");
    bool needRefresh = !(cacheResponse.length() > 100);
    if (cacheResponse.length() > 100) {
        qDebug() << "getMaterialCate local:" << cacheResponse.length();
        onLoadCategoryFinish(cacheResponse);
    }
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/Material/getMaterialCate")
        .get([this, needRefresh](const QString &response) {
            SharedPref::instance()->setString("cache_store_cate_config", response);
            if (needRefresh) {
                qDebug() << "getMaterialCate remote:" << response;
                this->onLoadCategoryFinish(response);
            }
        });
}

CategoryListModel *StoreDataSource::categoryListModel() {
    return m_curCategoryListModel;
}
StoreResourceListModel *StoreDataSource::resources() {
    return m_currentResources;
}
LocalResourceListModel *StoreDataSource::localVideoResource() {
    return m_localVideoResource;
}
int StoreDataSource::localVideoDataCount() {
    return m_localVideoResource->dataCount();
}
LocalResourceListModel *StoreDataSource::localAudioResource() {
    return m_localAudioResource;
}
int StoreDataSource::localAudioDataCount() {
    return m_localAudioResource->dataCount();
}
LocalResourceListModel *StoreDataSource::localMoreResource() {
    return m_localMoreResource;
}
int StoreDataSource::localMoreDataCount() {
    return m_localMoreResource->dataCount();
}
QString StoreDataSource::selectTopCategoryID() {
    return m_topCategoryId;
}

void StoreDataSource::showToast(QString msg, int during) {
    emit toastContentChanged(msg, during);
}

void StoreDataSource::selectTopCategory(QString cate) {
    m_topCategoryId = cate;
    updateCurrentCategoryView();
    m_allResources.clear();
    selectSubCategory(0); // normal select first sub category

    loadTopCategoryData(cate);
    if (cate == "AUDIO") {
        loadTopCategoryData("MUSIC");
    }
}

void StoreDataSource::loadTopCategoryData(QString cate) {
    // read data from server
    QString cacheData = SharedPref::instance()->readString("cache_store_" + cate + "_config");
    bool hasCache = cacheData.length() > 50;
    if (hasCache) {
        qDebug() << "getMaterialByCate " << cate << " local:" << cacheData.length();
        onLoadCategoryDataFinish(cate, cacheData);
        selectSubCategory(0);
    }
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/Material/getMaterialByCate")
        .addParam("cate", cate)
        .addParam("page", "1")
        .addParam("num", "9999")
        .get([this, cate, cacheData](const QString &response) {
            qDebug() << "http get : " << AppConstance::serverDomain() << " Cate:" << cate
                     << " response: " << response.length();
            if (cacheData != response) {
                SharedPref::instance()->setString("cache_store_" + cate + "_config", response);
                this->onLoadCategoryDataFinish(cate, response);
                this->selectSubCategory(0);
            }
        });
}

QString StoreDataSource::pascalCase(QString input) {
    QString firstChar = input.at(0).toUpper();
    QString lowerChars = input.mid(1);
    return firstChar + lowerChars;
}

void StoreDataSource::newEffect(const QString &path, const QString &targetpath, const QMap<QString, QString> &map) {
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        QDir dir(path);
        QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo listFileInfo = list.at(i);
            QString filePath = listFileInfo.absoluteFilePath();
            if (listFileInfo.isDir()) {
                newEffect(filePath, QFileInfo(targetpath, listFileInfo.fileName()).filePath(), map);
            } else {
                newEffect(filePath, targetpath, map);
            }
        }
    } else {
        QDir dir(targetpath);
        if (!dir.exists()) {
            dir.mkpath(targetpath);
        }
        QString dst;
        if (fileInfo.suffix() == "template") {
            dst = QFileInfo(dir, fileInfo.completeBaseName()).filePath();
        } else {
            dst = QFileInfo(dir, fileInfo.fileName()).filePath();
        }
        writeTemplate(path, dst, map);
    }
}

void StoreDataSource::writeTemplate(const QString &src, const QString &dst, const QMap<QString, QString> &map) {
    QFile srcFile(src);
    if (QFileInfo(src).suffix() == "template" && srcFile.open(QIODevice::ReadOnly)) {
        QString content = QString::fromUtf8(srcFile.readAll());
        for (QString key : map.keys()) {
            QString value = map.value(key);
            content.replace("{{" + key + "}}", value);
        }
        QFile dstFile(dst);
        if (dstFile.exists()) {
            dstFile.remove();
        }
        if (dstFile.open(QIODevice::WriteOnly)) {
            dstFile.write(content.toUtf8());
            dstFile.flush();
            dstFile.close();
        }
        srcFile.close();
    } else {
        srcFile.copy(dst);
    }
}

void StoreDataSource::addResourceToTimeline(int index, int trackIndex, double time) {
    StoreResourceModel model = m_currentResources->dataWith(index);
    if (model.avaliable()) {
        const QString path = model.getLocalDir();
        qDebug() << "local path = " << path;
        if (path == nullptr) {
            return;
        }
        SEProjectProj proj(SEProjectProj::findProjectFile(path));

        SkyResourceBean item = createResBean(model.id(), model.name(), proj.entryFile(), model.cover());
        SEProject::current()->dom()->timelineController()->addResource(item, trackIndex, time);
        //        emit resourceListChanged(m_currentResources);
    }
}

void StoreDataSource::selectThirdCategory(int subIndex, int thirdIndex) {
    int fixedSubIndex = subIndex;
    if (m_topCategoryId == "VIDEO" || m_topCategoryId == "AUDIO" || m_topCategoryId == "MORE") {
        fixedSubIndex -= 1;
    }
    emit showResourceList();
    m_currentResources->clearAll();
    selectResourceIndex(-1);

    for (int i = 0; i < m_allCategorys.count(); i++) {
        CategoryModel cate = m_allCategorys.at(i);
        if (cate.id() == m_topCategoryId) {

            qDebug() << "selectSubCategory " << cate.subCategory().at(fixedSubIndex).cateId;
            QString cateId = cate.subCategory().at(fixedSubIndex).cateId;
            QString subCateId = cate.subCategory().at(fixedSubIndex).subCateIds.at(thirdIndex);

            for (int j = 0; j < m_allResources.size(); j++) {
                if (m_allResources.at(j).subCate().contains(subCateId) && m_allResources.at(j).cate() == cateId) {
                    m_currentResources->addData(m_allResources.at(j));
                }
            }
        }
    }

    m_curCategoryListModel->setSelectedIndex(subIndex, thirdIndex);

    emit resourceListChanged(m_currentResources);
}

void StoreDataSource::openNewVideo(QString filePath) {
    filePath = QUrl(filePath).toLocalFile();
    if (!QFile(filePath).exists()) {
        qWarning() << "File NOT EXISTS";
        return;
    }

    if (m_localVideoResource->isExistFile(filePath)) {
        qWarning() << "File had EXIST in local resource list!" << filePath;
        return;
    }

    LocalResourceModel video(filePath);
    m_localVideoResource->addData(video);
    emit videoListChanged(m_localVideoResource);

    QJsonArray ja_video;
    for (int i = 1; i < m_localVideoResource->dataCount(); i++) {
        ja_video.push_back(m_localVideoResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_video_local", ja_video);
}

void StoreDataSource::selectResourceIndex(int index) {
    int preSelectedIndex = m_currentResources->curSelectedIndex();
    m_currentResources->setSelectedIndex(index);
    if (preSelectedIndex >= 0 && preSelectedIndex < m_currentResources->rowCount()) {
        StoreResourceModel model = m_currentResources->dataWith(preSelectedIndex);
        if (model.avaliable()) {
            SkyResourceBean item = createResBean(model.id(), model.name(), model.getLocalDir(), model.cover());
            SEProject::current()->dom()->resourcePreviewController()->stopPreviewResource(item);
        }
    }
    if (index < 0) {
        return;
    }
    StoreResourceModel model = m_currentResources->dataWith(index);
    if (model.avaliable()) {
        if (index >= 0 && index < m_currentResources->rowCount()) {
            StoreResourceModel model = m_currentResources->dataWith(index);
            SkyResourceBean item = createResBean(model.id(), model.name(), model.getLocalDir(), model.cover());
            if (item.isEffectResource() && item.isVenusEffect()) {
                showToast(tr("Cannot preview AI effect!"));
            } else {
                SEProject::current()->dom()->resourcePreviewController()->startPreviewResource(item);
            }
        }
    } else {
        model.download([=](const bool &success, const QString &path) {
            qDebug() << "download " << success << " path = " << path;
            emit resourceDownloadComplate(index);
            QVector<int> roles;
            roles.push_back(StoreResourceListModel::ExistRole);
            roles.push_back(StoreResourceListModel::IsVenusEffectRole);
            QModelIndex qIndex = m_currentResources->index(index);
            m_currentResources->dataChanged(qIndex, qIndex, roles);
            int preSelectedIndex = m_currentResources->curSelectedIndex();
            if (preSelectedIndex == index && model.avaliable()) {
                selectResourceIndex(index);
            }
        });
    }
}

void StoreDataSource::deleteLocalVideoResource(QVariantList list) {
    std::sort(list.begin(), list.end(), [=](QVariant &left, QVariant &right) {
        return left > right;
    });
    for (auto index : list) {
        if (index.toInt() > 0) {
            m_localVideoResource->deleteData(index.toInt());
        }
    }
    emit videoListChanged(m_localVideoResource);
    QJsonArray ja_video;
    for (int i = 1; i < m_localVideoResource->dataCount(); i++) {
        ja_video.push_back(m_localVideoResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_video_local", ja_video);
}

void StoreDataSource::deleteLocalAudioResource(QVariantList list) {
    std::sort(list.begin(), list.end(), [=](QVariant &left, QVariant &right) {
        return left > right;
    });
    for (auto index : list) {
        if (index.toInt() > 0) {
            m_localAudioResource->deleteData(index.toInt());
        }
    }
    emit audioListChanged(m_localAudioResource);
    QJsonArray ja_audio;
    for (int i = 1; i < m_localAudioResource->dataCount(); i++) {
        ja_audio.push_back(m_localAudioResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_audio_local", ja_audio);
}

void StoreDataSource::deleteLocalMoreResource(QVariantList list) {
    std::sort(list.begin(), list.end(), [=](QVariant &left, QVariant &right) {
        return left > right;
    });
    for (auto index : list) {
        if (index.toInt() > 0) {
            m_localMoreResource->deleteData(index.toInt());
        }
    }
    emit moreListChanged(m_localMoreResource);
    QJsonArray ja_more;
    for (int i = 1; i < m_localMoreResource->dataCount(); i++) {
        ja_more.push_back(m_localMoreResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_more_local", ja_more);
}

void StoreDataSource::selectLocalResourceIndex(LocalResourceListModel *localResource, int index) {
    int preSelectedIndex = localResource->curSelectedIndex();
    localResource->setSelectedIndex(index);
    if (preSelectedIndex >= 0 && preSelectedIndex < localResource->rowCount()) {
        LocalResourceModel model = localResource->dataWith(preSelectedIndex);
        SkyResourceBean item = createResBean("Local", model.fileName(), model.filePath(), model.fileCover());
        SEProject::current()->dom()->resourcePreviewController()->stopPreviewResource(item);
    }
    if (index < 0) {
        return;
    }
    LocalResourceModel model = localResource->dataWith(index);
    if (index >= 0 && index < localResource->rowCount()) {
        LocalResourceModel model = localResource->dataWith(index);
        SkyResourceBean item = createResBean("Local", model.fileName(), model.filePath(), model.fileCover());
        if (item.isEffectResource() && item.isVenusEffect()) {
            showToast(tr("Cannot preview AI effect!"));
        } else {
            SEProject::current()->dom()->resourcePreviewController()->startPreviewResource(item);
        }
    }
}

void StoreDataSource::selectLocalAudioResourceIndex(int index) {
    selectLocalResourceIndex(m_localAudioResource, index);
}

void StoreDataSource::selectLocalMoreResourceIndex(int index) {
    selectLocalResourceIndex(m_localMoreResource, index);
}

void StoreDataSource::selectLocalVideoResourceIndex(int index) {
    selectLocalResourceIndex(m_localVideoResource, index);
}

void StoreDataSource::addLocalVideoResource(QVariantList list, int trackIndex, double time) {
    for (auto index : list) {
        LocalResourceModel model = m_localVideoResource->dataWith(index.toInt());
        SkyResourceBean item = createResBean("Local", model.fileName(), model.filePath(), model.fileCover());
        SEProject::current()->dom()->timelineController()->addResource(item, trackIndex, time);
    }
}

void StoreDataSource::openNewAudio(QString filePath) {
    filePath = QUrl(filePath).toLocalFile();
    if (!QFile(filePath).exists()) {
        qWarning() << "openNewAudio File NOT EXISTS";
        return;
    }
    if (m_localAudioResource->isExistFile(filePath)) {
        qWarning() << "File had EXIST in local resource list!" << filePath;
        return;
    }

    LocalResourceModel audio(filePath);
    m_localAudioResource->addData(audio);
    emit audioListChanged(m_localAudioResource);

    QJsonArray ja_audio;
    for (int i = 1; i < m_localAudioResource->dataCount(); i++) {
        ja_audio.push_back(m_localAudioResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_audio_local", ja_audio);
}

bool StoreDataSource::openNewMore(QString filePath) {
    filePath = QUrl(filePath).toLocalFile();
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qWarning() << "File NOT EXISTS";
        showToast("File NOT Exists!");
        return false;
    }
    if (m_localMoreResource->isExistFile(filePath)) {
        qWarning() << "File had EXIST in local resource list!" << filePath;
        return false;
    }

    if (fileInfo.isFile()) {
        if (fileInfo.fileName().endsWith(".ofeffect", Qt::CaseInsensitive)) {
            LocalResourceModel more(fileInfo.dir(), fileInfo.filePath(), fileInfo.completeBaseName());
            m_localMoreResource->addData(more);
        } else {
            qWarning() << "File Format is not Support!" << filePath;
            showToast(tr("File Format is not Support"));
            return false;
        }
    } else {
        QDir dir(filePath);
        QFileInfoList list = dir.entryInfoList(QStringList() << "*.ofeffect");
        if (list.isEmpty()) {
            return false;
        }
        for (int i = 0; i < list.count(); i++) {
            QFileInfo fileInfo = list.at(i);
            LocalResourceModel more(
                dir, fileInfo.filePath(), list.count() == 1 ? nullptr : fileInfo.completeBaseName());
            m_localMoreResource->addData(more);
        }
    }

    emit moreListChanged(m_localMoreResource);
    QJsonArray ja_more;
    for (int i = 1; i < m_localMoreResource->dataCount(); i++) {
        ja_more.push_back(m_localMoreResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_more_local", ja_more);
    return true;
}

void StoreDataSource::addLocalAudioResource(QVariantList list, int trackIndex, double time) {
    for (auto index : list) {
        LocalResourceModel model = m_localAudioResource->dataWith(index.toInt());
        SkyResourceBean item = createResBean("Local", model.fileName(), model.filePath(), model.fileCover());
        SEProject::current()->dom()->timelineController()->addResource(item, trackIndex, time);
    }
}

void StoreDataSource::addLocalMoreResource(QVariantList list, int trackIndex, double time) {
    for (auto index : list) {
        LocalResourceModel model = m_localMoreResource->dataWith(index.toInt());
        QString iconPath = "qrc:/image/assets/icon_default_effect.png";
        if (QFileInfo::exists(model.fileCover())) {
            iconPath = QString("file:/%1").arg(model.fileCover());
        }
        SkyResourceBean item = createResBean("Local", model.fileName(), model.filePath(), iconPath);
        SEProject::current()->dom()->timelineController()->addResource(item, trackIndex, time);
    }
}

void StoreDataSource::addSubEffectToVideoClip(QVariantList list, int trackIndex, int clipIndex) {
    for (auto index : list) {
        if (m_topCategoryId == "MORE") {
            LocalResourceModel model = m_localMoreResource->dataWith(index.toInt());
            QString iconPath = "qrc:/image/assets/icon_default_effect.png";
            if (QFileInfo::exists(model.fileCover())) {
                iconPath = QString("file:/%1").arg(model.fileCover());
            }

            SkyResourceBean item = createResBean("Local", model.fileName(), model.filePath(), iconPath);
            SEProject::current()->dom()->timelineController()->addEffect(item, trackIndex, clipIndex);
        } else {
            StoreResourceModel model = m_currentResources->dataWith(index.toInt());
            SkyResourceBean item = createResBean(model.id(), model.name(), model.getLocalDir(), model.cover());
            SEProject::current()->dom()->timelineController()->addEffect(item, trackIndex, clipIndex);
        }
    }
}

SkyResourceBean StoreDataSource::createResBean(QString id, QString name, QString path, QString modelThumb) {
    QString thumb = "";
    ResourceType type = EffectResource;
    if (m_topCategoryId == "VIDEO") {
        if (FileUtils::isImageExtension(path)) {
            type = ImageResource;
        } else {
            type = VideoResource;
        }
        thumb = "qrc:/image/assets/store_video.png";
    } else if (m_topCategoryId == "AUDIO") {
        type = AudioResource;
        thumb = "qrc:/image/assets/clip_audio.png";
    } else if (m_topCategoryId == "WORDS") {
        type = TextResource;
        thumb = "qrc:/image/assets/clip_text.png";
    } else if (m_topCategoryId == "STICKER") {
        type = StickerResource;
        thumb = "qrc:/image/assets/clip_sticker.png";
    } else if (m_topCategoryId == "EFFECT") {
        type = EffectResource;
        thumb = "qrc:/image/assets/clip_effect.png";
    } else if (m_topCategoryId == "TRANSTIONS") {
        type = TransitionResource;
        thumb = "qrc:/image/assets/store_transition.png";
    } else if (m_topCategoryId == "FILTER") {
        type = FilterResource;
        thumb = "qrc:/image/assets/clip_filter.png";
    } else if (m_topCategoryId == "MORE") {
        type = MoreResource;
        thumb = "qrc:/image/assets/clip_sticker.png";
    }
    if (modelThumb != nullptr) {
        thumb = modelThumb;
    }
    qDebug() << "createResBean local path = " << path << " NAME=" << name;
    return SkyResourceBean(path, name, type, thumb, id);
}

void StoreDataSource::selectSubCategory(int index) {
    int fixedIndex = index;
    selectResourceIndex(-1);
    if (m_topCategoryId == "VIDEO" && index == 0) {
        emit showVideoLocalList();
        emit videoListChanged(m_localVideoResource);
    } else if (m_topCategoryId == "AUDIO" && index == 0) {
        emit showAudioLocalList();
        emit audioListChanged(m_localAudioResource);
    } else if (m_topCategoryId == "MORE" && index == 0) {
        emit showMoreLocalList();
        emit moreListChanged(m_localMoreResource);
    } else {
        if (m_topCategoryId == "VIDEO" || m_topCategoryId == "AUDIO" || m_topCategoryId == "MORE") {
            fixedIndex -= 1;
        }
        selectLocalVideoResourceIndex(-1);
        selectLocalAudioResourceIndex(-1);
        selectLocalMoreResourceIndex(-1);
        emit showResourceList();
        m_currentResources->clearAll();

        for (int i = 0; i < m_allCategorys.count(); i++) {
            CategoryModel cate = m_allCategorys.at(i);
            if (cate.id() == m_topCategoryId && cate.subCategory().size() > fixedIndex) {
                qDebug() << "selectSubCategory " << cate.subCategory().at(fixedIndex).cateId;
                for (int j = 0; j < m_allResources.size(); j++) {
                    if (m_allResources.at(j).subCate().contains(cate.subCategory().at(fixedIndex).cateId) ||
                        m_allResources.at(j).cate() == cate.subCategory().at(fixedIndex).cateId) {
                        m_currentResources->addData(m_allResources.at(j));
                    }
                }
            }
        }

        emit resourceListChanged(m_currentResources);
    }
    m_curCategoryListModel->setSelectedIndex(index, -1);
}

QString StoreDataSource::getResourcePath(int index) {
    StoreResourceModel model = m_currentResources->dataWith(index);
    if (model.avaliable()) {
        QString path = model.getLocalDir();
        qDebug() << "getResourcePath local path = " << path;

        return path;
    } else {
        return "";
    }
}

void StoreDataSource::onLoadCategoryFinish(const QString &response) {
    m_allCategorys.clear();

    BaseRsp *rsp = new BaseRsp(response);
    if (rsp->isSuccess()) {
        QJsonArray cates = rsp->data().toArray();
        for (int i = 0; i < cates.count(); i++) {
            QJsonObject cate = cates.at(i).toObject();

            //二级分类
            QJsonArray secondCates = cate.value("subCates").toArray();
            QList<SubCategory> secondVerctorSubCates;
            for (int j = 0; j < secondCates.count(); j++) {
                QJsonObject subCate = secondCates.at(j).toObject();
                SubCategory subModel;
                subModel.cateId = subCate.value("cateId").toString();
                subModel.cate = subCate.value("cate").toString();

                //三级分类
                QJsonArray thirdCates = subCate.value("subCates").toArray();
                for (int k = 0; k < thirdCates.count(); k++) {
                    QJsonObject tinyCate = thirdCates.at(k).toObject();
                    subModel.subCate.push_back(tinyCate.value("cate").toString());
                    subModel.subCateIds.push_back(tinyCate.value("cateId").toString());
                }
                secondVerctorSubCates.push_back(subModel);
            }

            qDebug() << "Add Cates :" << cate.value("cate").toString()
                     << " SecondCategory: " << secondVerctorSubCates.size();

            CategoryModel model(cate.value("cateId").toString(), cate.value("cate").toString(), secondVerctorSubCates);

            m_allCategorys.push_back(model);
        }
    } else {
        qDebug() << rsp->message();
    }

    combindAudioAndMusicCategory();

    updateCurrentCategoryView();
    selectTopCategory(m_topCategoryId); // normal select first sub category
    //    selectSubCategory(0);
    delete rsp;
    rsp = nullptr;
}

void StoreDataSource::combindAudioAndMusicCategory() {
    int musicIndex = -1, audioIndex = -1;
    for (int i = 0; i < m_allCategorys.size(); ++i) {
        if (m_allCategorys.at(i).id() == "MUSIC") {
            musicIndex = i;
        } else if (m_allCategorys.at(i).id() == "AUDIO") {
            audioIndex = i;
        }
    }
    if (musicIndex == -1 || audioIndex == -1) {
        qDebug() << "Music Or Audio is Empty!";
        return;
    }

    CategoryModel audioModel = m_allCategorys[audioIndex];
    CategoryModel musicModel = m_allCategorys[musicIndex];

    QStringList audioCategories;
    QStringList audioCategoryIds;
    for (SubCategory category : audioModel.subCategory()) {
        audioCategories.append(category.cate);
        audioCategoryIds.append(category.cateId);
    }
    SubCategory audioSubCategory;
    audioSubCategory.cateId = audioModel.id();
    audioSubCategory.cate = audioModel.title();
    audioSubCategory.subCate = audioCategories;
    audioSubCategory.subCateIds = audioCategoryIds;

    qDebug() << "Audio Category " << audioSubCategory.cateId << " Cate:" << audioSubCategory.cate;

    QStringList thirdCategories;
    QStringList thirdCategoryIds;
    for (SubCategory category : musicModel.subCategory()) {
        thirdCategories.append(category.cate);
        thirdCategoryIds.append(category.cateId);
    }
    SubCategory musicSubCategory;
    musicSubCategory.cateId = musicModel.id();
    musicSubCategory.cate = musicModel.title();
    musicSubCategory.subCate = thirdCategories;
    musicSubCategory.subCateIds = thirdCategoryIds;

    qDebug() << "Music Category " << musicSubCategory.cateId << " Cate:" << musicSubCategory.cate;

    QList<SubCategory> categories;
    categories.append(audioSubCategory);
    categories.append(musicSubCategory);
    audioModel.updateSubCategory(categories);

    m_allCategorys.replace(audioIndex, audioModel);
    m_allCategorys.removeAt(musicIndex);

    qDebug() << "Music Audio Category Combine Success!";
}

void StoreDataSource::updateCurrentCategoryView() {
    m_curCategoryListModel->clearAll();
    //过滤出当前分类数据
    for (int i = 0; i < m_allCategorys.count(); i++) {
        CategoryModel cate = m_allCategorys.at(i);
        if (cate.id() == m_topCategoryId) {
            for (int j = 0; j < cate.subCategory().count(); j++) {
                m_curCategoryListModel->addData(cate.subCategory().at(j));
            }
            break;
        }
    }

    //部分数据改造，例如增加本地分类
    if (m_topCategoryId == "VIDEO" || m_topCategoryId == "AUDIO" || m_topCategoryId == "MORE") {
        SubCategory subModel;
        subModel.cateId = "";
        subModel.cate = tr("Local");
        m_curCategoryListModel->insertData(0, subModel);
    }
    emit categoryListChanged(m_curCategoryListModel);
}

void StoreDataSource::onLoadCategoryDataFinish(QString cate, const QString &response) {
    BaseRsp *rsp = new BaseRsp(response);
    //获取远端配置资源列表
    QString preCate = m_allResources.size() > 0 ? m_allResources.at(0).cate() : "";
    if ((cate == "AUDIO" || cate == "MUSIC") && (preCate == "AUDIO" || preCate == "MUSIC") && preCate != cate) {

    } else {
        m_allResources.clear();
    }

    if (rsp->isSuccess()) {
        QJsonArray cates = rsp->data().toArray();
        for (int i = 0; i < cates.count(); i++) {
            QJsonObject subCate = cates.at(i).toObject();
            StoreResourceModel model(subCate);
            m_allResources.push_back(model);
        }
    } else {
        qDebug() << rsp->message();
    }
    delete rsp;
    rsp = nullptr;
}

void StoreDataSource::addNewEffect(QUrl url) {
    auto targetDir = url.toLocalFile();
    QString defaultEffectPath = QCoreApplication::applicationDirPath().append("/effects/Empty");
    QFileInfo info(targetDir);
    SEProjectProj proj(QFileInfo(targetDir, "template.proj").filePath());
    auto login = Login::instance();
    if (login->isLogin()) {
        proj.setAnchor(AppConstance::nickname());
    }
    proj.setTitle(info.baseName());
    proj.setType(Effect);
    proj.updateCreateTimestamp();

    QMap<QString, QString> map;
    map.insert("name", pascalCase(info.baseName()));
    map.insert("id", proj.id());
    map.insert("title", proj.title());
    map.insert("remoteId", proj.remoteId());
    map.insert("version", proj.version());
    map.insert("anchor", proj.anchor());
    map.insert("summary", proj.summary());
    map.insert("thumb", "");
    map.insert("type", proj.stringWithProjType(proj.type()));
    map.insert("inputConfig", proj.inputConfig());
    map.insert("createTimestamp", QString::number(proj.createTimestamp().toTime_t()));
    map.insert("lastChangedTimestamp", QString::number(proj.lastChangedTimestamp().toTime_t()));
    map.insert("ofFile", "bundle://effect.ofeffect");

    newEffect(defaultEffectPath, targetDir, map);
    LocalResourceModel more(targetDir, targetDir + "/effect.ofeffect", info.completeBaseName());
    m_localMoreResource->addData(more);
    emit moreListChanged(m_localMoreResource);
    QJsonArray ja_more;
    for (int i = 1; i < m_localMoreResource->dataCount(); i++) {
        ja_more.push_back(m_localMoreResource->dataWith(i).toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_more_local", ja_more);
    VSRun::run(QStringList() << targetDir, QStringList() << EXTENSIONS_OF_EFFECT);
}

void StoreDataSource::editEffect(int index) {
    if (index <= 0 || index >= m_localMoreResource->rowCount()) {
        return;
    }
    auto bean = m_localMoreResource->dataWith(index);
    QFileInfo fileInfo(bean.filePath());
    QString dir = fileInfo.absoluteDir().absolutePath();
    VSRun::run(QStringList() << dir, QStringList() << EXTENSIONS_OF_EFFECT, [=](const bool result) {
        if (!result) {
            showToast(tr("Visual Studio Code is not installed"));
        }
    });
}

void StoreDataSource::uploadEffect(int index) {
}

void StoreDataSource::openAudioDir(int index) {
    openResourceDir(m_localAudioResource, index);
}

void StoreDataSource::openVideoDir(int index) {
    openResourceDir(m_localVideoResource, index);
}

void StoreDataSource::openEffectDir(int index) {
    openResourceDir(m_localMoreResource, index);
}

void StoreDataSource::openResourceDir(LocalResourceListModel *model, int index) {
    if (index <= 0 || index >= model->rowCount()) {
        return;
    }
    auto bean = model->dataWith(index);
    QFileInfo fileInfo(bean.filePath());
    QString dir = fileInfo.absoluteDir().absolutePath();
    QProcess process;
    process.setWorkingDirectory(dir);
    QString program = "explorer";
#ifdef Q_OS_WIN
    program = "explorer";

#else
    program = "open";
#endif
    QString cmd = program + " .";
    process.start(cmd);
    process.setReadChannel(QProcess::StandardError);
    process.waitForStarted();
    process.waitForFinished();
    qDebug() << "Output2:" << QString::fromLocal8Bit(process.readAllStandardError());
}
