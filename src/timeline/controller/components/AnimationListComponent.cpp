#include "AnimationListComponent.h"
#include <QDebug>
#include <QDir>
#include <QUrl>
#include <QCoreApplication>
#include "src/base/http/BaseRsp.h"
#include "src/base/utils/SharedPref.h"
#include <base/utils/JsonUtils.h>
#include <base/utils/AppPath.h>
#include "src/base/http/SEHttpClient.h"
#include "src/base/utils/ZipUtils.h"
#include "src/base/utils/AppConstance.h"
#include "project/SEProject.h"
#pragma execution_character_set("utf-8");

AnimationBean::AnimationBean(const QJsonObject &object) {
    m_id = object.value("id").toString();
    m_type = object.value("type").toString();
    m_iconURL = object.value("cover").toString();
    m_dynamicIcon = object.value("preview").toString();
    m_name = object.value("name").toString();
    m_source = object.value("source").toString();
    QUrl modelSource(m_source);
    m_localPath = type2Path(m_type) + modelSource.fileName().left(modelSource.fileName().size() - 4);
}

bool AnimationBean::isDownload() const {
    return QFile(m_localPath).exists();
}

QString AnimationBean::type2Path(const QString &type) {
    QString path;
    if (type == "") {
        return "";
    } else if (type == "StickerEnterAnimation") {
        path = AppPath::animationsPath("sticker/enter_animation/");
    } else if (type == "StickerExitAnimation") {
        path = AppPath::animationsPath("sticker/exit_animation/");
    } else if (type == "StickerLoopAnimation") {
        path = AppPath::animationsPath("sticker/loop_animation/");
    } else if (type == "VideoEnterAnimation") {
        path = AppPath::animationsPath("video/enter_animation/");
    } else if (type == "VideoExitAnimation") {
        path = AppPath::animationsPath("video/exit_animation/");
    } else if (type == "VideoLoopAnimation") {
        path = AppPath::animationsPath("video/loop_animation/");
    } else if (type == "TextEnterAnimation") {
        path = AppPath::animationsPath("text/enter_animation/");
    } else if (type == "TextExitAnimation") {
        path = AppPath::animationsPath("text/exit_animation/");
    } else if (type == "TextLoopAnimation") {
        path = AppPath::animationsPath("text/loop_animation/");
    }

    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path);
    }
    return path;
}

void AnimationBean::download(std::function<void(const bool, const QString &, const QString &)> complateHandler) {
    QString Dir = type2Path(m_type);
    QString downloadFile = Dir + QUrl(m_source).fileName();
    QFile *file = new QFile(downloadFile);
    if (file->exists()) {
        file->remove();
    }
    file->open(QIODevice::ReadWrite);
    SEHttpClient(m_source).download(
        [=](const QByteArray &data) {
            file->write(data);
            qDebug() << "AnimationComponent  downloading...";
        },
        [=, localPath = m_localPath] {
            file->flush();
            file->close();
            qDebug() << "AnimationComponent  downloaded...";
            // unzip
            if (ZipUtils::decompressFromZip(downloadFile, localPath)) {
                file->deleteLater();
            }
            complateHandler(true, Dir, downloadFile);
        });
}

void AnimationsListModel::addData(const QList<AnimationBean> &data) {
    if (data.size() > 0) {
        int preSize = m_datas.size();
        int curSize = m_datas.size() + data.size() - 1;
        beginInsertRows(QModelIndex(), preSize, curSize);
        m_datas.append(data);
        endInsertRows();
    }
}

AnimationBean AnimationsListModel::dataWith(int index) {
    return m_datas[index];
}

void AnimationsListModel::clearAll() {
    if (m_datas.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_datas.size() - 1);
        m_datas.clear();
        endRemoveRows();
    }
}

void AnimationsListModel::setSelectedIndex(int i) {
    int preIndex = m_selectedIndex;
    m_selectedIndex = i;
    QModelIndex modelIndex = index(i);
    QVector<int> vector;
    vector.append(SelectedRole);
    emit dataChanged(modelIndex, modelIndex, vector);
    emit dataChanged(index(preIndex), index(preIndex), vector);
}

int AnimationsListModel::findAnimationBeanIndex(QString id) {
    for (int i = 0; i < m_datas.size(); ++i) {
        AnimationBean animation = m_datas[i];
        if (animation.id() == id) {
            return i;
        }
    }
    return 0;
}

AnimationListComponent::AnimationListComponent(QObject *parent) : BaseComponent(parent) {
}

void AnimationListComponent::onBindQml(QJsonObject &params) {
    if (params.isEmpty() || !params.contains("paramTitle") || !params.contains("paramSettingInfo")) {
        return;
    }
    m_paramTitle = params["paramTitle"].toString();
    QJsonObject paramInfo = params["paramSettingInfo"].toObject();
    auto path = paramInfo["path"].toString();
    if(!path.isEmpty()){
        m_url = AppConstance::serverDomain() + path;
    } else {
        m_url = paramInfo["url"].toString();
    }
    m_animationKey = paramInfo["animationKey"].toString();
    m_durationKey = paramInfo["durationKey"].toString();
    m_effectiveTime = paramInfo["effectiveTime"].toString();
    QString exclusiveKeys = paramInfo["exclusiveKeys"].toString();
    if (!exclusiveKeys.isEmpty()) {
        m_exclusiveKeys = exclusiveKeys.split(";", QString::SkipEmptyParts);
    }
    if(m_durationKey == EnterAnimationDuringKey){
        m_oppositeKey = ExitAnimationDuringKey;
    }else if(m_durationKey == ExitAnimationDuringKey){
        m_oppositeKey = EnterAnimationDuringKey;
    }

    updateAnimationMaxDuration();

    qDebug() << "animationlist bindqml " << m_url << m_animationKey << m_durationKey;

    if (m_url.isEmpty()) {
        return;
    }
    QString typeName = m_url.split("type=")[1];
    QString cacheResponse = SharedPref::instance()->readString("cache_component_" + typeName + "_config");
    if (!cacheResponse.isEmpty()) {
        // qDebug() << "get " +typeName+ " local:" << cacheResponse;
        loadDatas(cacheResponse);
    }

    SEHttpClient(m_url).get(
        [=](const QString &response) {
            if (response != cacheResponse) {
                SharedPref::instance()->setString("cache_component_" + typeName + "_config", response);
                // qDebug() << "get " +typeName+ " remote:" << response;
                loadDatas(response);
            }
        },
        [=](const QString &error) {
            qWarning() << "get " + typeName + " remote Error:" << error;
        });
}

void AnimationListComponent::loadDatas(const QString &response) {
    BaseRsp *rsp = new BaseRsp(response);
    m_listModels->clearAll();
    QList<AnimationBean> list;
    list.insert(0, AnimationBean("qrc:/image/assets/animation_none.png", tr("None")));
    if (rsp->isSuccess()) {
        QJsonArray animationItems = rsp->data().toObject().value("list").toArray();
        for (int i = 0; i < animationItems.count(); ++i) {
            QJsonObject animationItem = animationItems.at(i).toObject();
            list.append(animationItem);
            // qDebug() << "animations load: " << model.type() << model.name() <<" localPath: " << model.localPath();
        }
    } else {
        qDebug() << "animationlist loadData rsp failed: " << rsp->message();
    }
    m_listModels->addData(list);
    m_listModels->setSelectedIndex(getCurSelectedIndex());

//    QMap<QString, SkyVariant> defaultValues;
//    defaultValues.insert(m_durationKey, 1.0);
//    defaultValues.insert(m_animationKey, "");
//    setOfParamsDefaultValues(defaultValues);

    onClipParamsChanged();
    delete rsp;
    rsp = nullptr;
}

void AnimationListComponent::onUnBind() {
}

double AnimationListComponent::minDuration() {
    return 0.0;
}

double AnimationListComponent::maxDuration() {
    return m_maxDuration;
}

AnimationsListModel *AnimationListComponent::listModel() const {
    return m_listModels;
}

void AnimationListComponent::clickDownload(int index) {
    AnimationBean model(m_listModels->dataWith(index));
    emit onDownloading(true, index);
    emit listModelChanged();
    model.download([=](const bool &success, const QString &path, const QString &name) {
        qDebug() << "Animation_download: " << success << " path = " << path << "name = " << name;

        emit onDownloading(false, index);
        emit listModelChanged();
    });
}

void AnimationListComponent::onAnimationTypeChange(int itemIndex) {
    AnimationBean model(m_listModels->dataWith(itemIndex));
    if (model.isDownload() || itemIndex == 0) {
        changeAnimationType(itemIndex);
    } else {
        emit listModelChanged();
        emit onDownloading(true, itemIndex);
        model.download([=](const bool &success, const QString &path, const QString &name) {
            qDebug() << "Animation_download: " << success << " path = " << path << "name = " << name;
            emit onDownloading(false, itemIndex);
            changeAnimationType(itemIndex);
        });
    }
}

void AnimationListComponent::changeAnimationType(int itemIndex) {
    auto animation =m_listModels->dataWith(itemIndex);

    auto animationData = curClip()->skyClip()->getUserData("Animation");
    if(animationData.isNull()){
        animationData = SkyVariant::makeObject();
    }
    animationData.insert((m_effectiveTime+"Id").toUtf8().data(),animation.id().toUtf8().data());
    curClip()->skyClip()->setUserData("Animation",animationData);

    auto animationPath = itemIndex == 0 ? SkyVariant("") : SkyVariant::makePath(animation.localPath().toUtf8().data());
    SkyVariant preAnimationPath = getOfParamValue(m_animationKey);
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_animationKey, animationPath);
    paramMap.insert(m_durationKey, durationForSection());
    QMap<QString, SkyVariant> preValueMap;
    preValueMap.insert(m_animationKey, preAnimationPath);
    for (auto key : m_exclusiveKeys) {
        if (!key.isEmpty()) {
            paramMap.insert(key, "");
            preValueMap.insert(key, getOfParamValue(key));
        }
    }

    qDebug() << "chengj: animation path is " << animation.localPath();
    if (updateOfParamValuesByUser(paramMap, preValueMap)) {
        m_listModels->setSelectedIndex(itemIndex);
        if (itemIndex > 0) {
            SkyTimeRange range = calcPreviewAnimationTimeRange();
            SEProject::current()->dom()->previewController()->play(range);
        }
        qDebug() << "chengj: onAnimationTypeChange success selected animation:" << animationPath.toString();
    }
    emit listModelChanged();
}

int AnimationListComponent::getCurSelectedIndex() {
    QString key = m_animationKey;
    QString path = QString::fromLocal8Bit(getOfParamValue(key).toString());
    if (path.trimmed().isEmpty()) {
        return 0;
    }

    auto animationData = curClip()->skyClip()->getUserData("Animation");
    if(animationData.isNull()){
        return 0;
    }
    auto selectedId = animationData.getString((m_effectiveTime+"Id").toUtf8().data(),"");

    return m_listModels->findAnimationBeanIndex(selectedId);
}

void AnimationListComponent::onOfParamsChanged(QString key, SkyVariant skyVariant) {
    if (m_animationKey == key) {
        int index = m_listModels->findAnimationBeanIndex(QString::fromLocal8Bit(skyVariant.toString()));
        m_listModels->setSelectedIndex(index);
    } else if (m_durationKey == key) {
        float duration = skyVariant.toDouble();
        emit durationForSectionChanged(duration);
    } else if(key == m_oppositeKey){
        updateAnimationMaxDuration();
    }
}

void AnimationListComponent::updateAnimationMaxDuration(){
    BaseClip *clip = curClip();
    double maxDuring = 10.0;
    if (clip != nullptr) {
        maxDuring = fmin(clip->during(), 60);
    }
    double oppositeDuring = 0;
    if(m_oppositeKey.length() > 0){
        oppositeDuring = getOfParamVariantValue(m_oppositeKey,0.0).toDouble();
    }
    double curDuring = getOfParamVariantValue(m_durationKey,0).toDouble();

    if(curDuring + oppositeDuring > maxDuring){
        oppositeDuring = maxDuring * oppositeDuring / (curDuring + oppositeDuring);
    }
    maxDuring = maxDuring - oppositeDuring;
    if (abs(m_maxDuration - maxDuring) > TIME_DEVIATION) {
        m_maxDuration = maxDuring;
        emit maxDurationChanged();

        if(curDuring > maxDuring){
            curDuring = maxDuring;
            updateOfParamVariantValueBySilent(m_durationKey,curDuring);
        }
    }
}

void AnimationListComponent::onClipParamsChanged() {
    updateAnimationMaxDuration();
}

AnimationListComponent::~AnimationListComponent() {
    delete m_listModels;
}

SkyTimeRange AnimationListComponent::calcPreviewAnimationTimeRange() {
    BaseClip *clip = curClip();
    if (clip == nullptr) {
        return SkyTimeRange();
    }

    double startTime = clip->startTime();
    double endTime = clip->endTime();
    double during = durationForSection();
    if (m_effectiveTime.contains("front")) {
        endTime = fmin(startTime + during, endTime);
        return SkyTimeRange(startTime, endTime);
    } else if (m_effectiveTime.contains("rear")) {
        startTime = fmax(startTime, endTime - during);
        return SkyTimeRange(startTime, endTime);
    } else if (m_effectiveTime.contains("full")) {
        return SkyTimeRange(startTime, endTime);
    } else {
        return SkyTimeRange();
    }
}

double AnimationListComponent::defDurationForSection() {
    return 1.0;
}

double AnimationListComponent::durationForSection() {
    SkyVariant skyVariant = getOfParamValue(m_durationKey);
    double during = skyVariant.isNull() ? defDurationForSection() : skyVariant.toDouble();
    return during;
}

void AnimationListComponent::onAnimationDurationChange(float duration) {
    QString key = m_durationKey;
    auto durationValue = SkyVariant(duration);
    if (updateOfParamValueBySilent(key, durationValue)) {
    }
}

void AnimationListComponent::onAnimationDurationChangeByUser(float preDuration, float duration) {
    QString key = m_durationKey;
    if (updateOfParamValueByUser(key, duration, preDuration)) {
        SkyTimeRange range = calcPreviewAnimationTimeRange();
        SEProject::current()->dom()->previewController()->play(range);
    }
}
