#include "baseclip.h"
#include "TimelineDef.h"
#include <QVariant>
#include <QImage>
#include <QStandardPaths>
#include <QFile>
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include <src/timeline/models/skyresourcebean.h>
#include <src/project/SEProject.h>
#include <base/utils/JsonUtils.h>
#include <base/utils/SkyVariantUtil.h>
#include <timeline/venus/AsyncTimelineFrameMgr.h>

SKYMEDIA_USING_NAMESPACE

BaseClip::BaseClip(const SkyResourceBean &bean, SkyClip *clip) : QObject(nullptr), m_resBean(bean), m_skyClip(clip) {
    loadSkyEffect();
}

void BaseClip::onWordContentChange(QString wordContent) {
    m_wordContent = wordContent;
    auto timelineModel = SEProject::current()->dom()->timelineController()->timelineModel();
    timelineModel->notifyFromClip(this);
}

double BaseClip::during() const {
    auto range = m_skyClip->getTrackRange();
    return range.end - range.begin;
}

double BaseClip::sourceDuring() const {
    auto range = m_skyClip->getTrimRange();
    return range.end - range.begin;
}

double BaseClip::resouceTime() const {
    return m_skyClip->getResource().getDuration();
}

double BaseClip::startTime() const {
    return m_skyClip->getTrackRange().begin;
}

double BaseClip::endTime() const {
    return m_skyClip->getTrackRange().end;
}

double BaseClip::cropStartTime() const {
    return m_skyClip->getTrimRange().begin;
}

double BaseClip::cropEndTime() const {
    return m_skyClip->getTrimRange().end;
}

SkyResourceBean BaseClip::resourceBean() const {
    return m_resBean;
}

QString BaseClip::entryPath() const {
    return m_resBean.entryPath();
}

QString BaseClip::wordContent() const {
    return m_wordContent;
}

BaseClip::~BaseClip() {
    qDebug() << "~ destroy clip!!" << this;
    m_asyncProcessModel = nullptr;
    qDeleteAll(m_effectList);
    m_effectList.clear();

    if (m_skyClip) {
        m_skyClip = nullptr;
    }
}

SkyEffectWrapper *BaseClip::addEffect(SkyResourceBean &resource) {
    SkyEffectWrapper *wraper = findEffect(resource.entryPath());
    if (wraper != nullptr) {
        qDebug() << "Exit same Effect, Exit!" << resource.entryPath();
        return nullptr;
    }

    wraper = new SkyEffectWrapper(this, resource);
    SkyEffect *skyEffect = wraper->addEffect();
    if (skyEffect != nullptr) {
        m_effectList.append(wraper);
        if (wraper->isVenusEffect()) {
            m_hasVenusEffect = true;
        }
        emit ofParamChanged(this, "Clip:AddEffect", 0);
        return wraper;
    }
    delete wraper;
    return nullptr;
}

SkyEffectWrapper *BaseClip::addEffectFromConfig(SkyResourceBean &resource, SkyVariant &config) {
    SkyEffect* effect = skyClip()->addEffectFromConfig(config);
    if(effect != nullptr){
        auto wraper = new SkyEffectWrapper(this,resource,effect);
        m_effectList.insert(0, wraper);
        if (wraper->isVenusEffect()) {
            m_hasVenusEffect = true;
        }
        emit ofParamChanged(this, "Clip:AddEffect", 0);
        return wraper;
    }else{
        return addEffect(resource);
    }
}

bool BaseClip::moveEffectPos(int fromIndex, int toIndex){
    QList<SkyEffectWrapper *> excludeOverlayList;
    for (int i = 0; i < m_effectList.size(); ++i) {
        if (m_effectList[i]->resourceType() != OverlayResource) {
            excludeOverlayList.append(m_effectList[i]);
        }
    }
    if(excludeOverlayList.size() <= fromIndex || fromIndex < 0){
        return false;
    }
    excludeOverlayList.move(fromIndex,toIndex);
    for(int i=0;i<excludeOverlayList.size();++i){
        excludeOverlayList[i]->moveIndex(i);
    }

    loadSkyEffect();
    return true;
}


void BaseClip::updateCurFocusEffect(SkyEffectWrapper *effect){
    m_curFocusEffect = effect;
    QVector<int> d;
    d.append(RoleKeyFrameTimes);
    emit clipDataChanged(this, d);
}

SkyEffectWrapper *BaseClip::findEffect(const QString effectPath) {
    QFileInfo file(effectPath);
    for (auto wraper : m_effectList) {
        if (wraper != nullptr && QFileInfo(wraper->getOfEffectPath()) == file) {
            qDebug() << "Find Effect " << effectPath;
            return wraper;
        }
    }
    return nullptr;
}

bool BaseClip::removeEffect(QString effectPath) {
    auto wraper = findEffect(effectPath);
    if (wraper != nullptr) {
        auto index = m_effectList.indexOf(wraper);
        wraper->removeEffect();
        m_effectList.removeOne(wraper);
        if(m_curFocusEffect == wraper){
            m_curFocusEffect = nullptr;
        }
        delete wraper;
        emit ofParamChanged(this, "Clip:RemoveEffect", index);

        QVector<int> d;
        d.append(RoleKeyFrameTimes);
        emit clipDataChanged(this, d);
        return true;
    }
    return false;
}

QVariantList BaseClip::getKeyFrameTimes() const{
    QSet<double> keyFrameSets;
    SkyEffectWrapper* curEffect = m_curFocusEffect;
    if(curEffect == nullptr){
        curEffect = skyEffectWrapper();
    }
    QVariantList variants;
    if(curEffect != nullptr){
        for(auto item:curEffect->getKeyFrameTimes()){
            variants.append(item);
        }
    }
    return variants;
}

QList<SkyEffectWrapper *> BaseClip::findEffectListByType(ResourceType type) {
    QList<SkyEffectWrapper *> list;
    for (auto wrapper : m_effectList) {
        if (wrapper->resourceType() == type) {
            list.append(wrapper);
        }
    }
    return list;
}

QList<SkyEffectWrapper *> BaseClip::effectList() {
    return m_effectList;
}


QVariant BaseClip::get(int role) const {
    switch (role) {
        case RoleStartTime:
            return startTime();
        case RoleDuration:
            return during();
        case RoleResource:
            return resourceBean().entryPath();
        case RoleEndTime:
            return endTime();
        case RoleCropStartTime:
            return cropStartTime();
        case RoleCropEndTime:
            return cropEndTime();
        case RoleResourceTime:
            return resouceTime();
        case RoleThumbnails:
            return "";
        case RoleClipType:
            return clipType();
        case RoleResouceType:
            return resourceBean().resouceType();
        case RoleName:
            return resourceBean().name();
        case RoleIconPath:
            return resourceBean().thumb();
        case RoleVolume:
            return (int)(skyClip()->getVolume() * 100);
        case RoleWordContent:
            return wordContent();
        case RoleSpeed:
            return getSpeed();
        case RoleCurvedSpeed:
            return getCurvedSpeed();
        case RoleAvgSpeed:
            return getAvgSpeed();
        case RoleFreezeDuration:
            return resourceBean().isFreezeDuration();
        case RoleMinTransitionDuring:
            return -1;
        case RoleMaxTransitionDuring:
            return -1;
        case RoleTransitionDuring:
            return -1;
        case RoleHasInputDevice:
            return getInputId();
        case RoleKeyFrameTimes:
            return getKeyFrameTimes();
        case RoleGroupInfos:
            return getGroupInfos();
        default:
            return QVariant();
    }
}

QString BaseClip::toString() const {
    return QString("BaseClip Path:%1 Range:[%2,%3) Crop:[%4,%5) Type:%6")
        .arg(resourceBean().entryPath())
        .arg(startTime())
        .arg(endTime())
        .arg(cropStartTime())
        .arg(cropEndTime())
        .arg(clipType());
}

void BaseClip::setResourceBean(const SkyResourceBean bean) {
    m_resBean = bean;
}

QString BaseClip::updateResource(QString resourcePath){
    SkyResourceBean resBean(resourcePath, "",m_resBean.resouceType());
    QString preResourcePath = "";
    if(m_skyClip && m_skyClip->updateResource(resBean.skyResouce())){
        preResourcePath = m_resBean.entryPath();
        m_resBean = resBean;
        emit ofParamChanged(this,"-1:updateResource",resourcePath.toUtf8().data(),nullptr);
    }
    return preResourcePath;
}

void BaseClip::saveInstanceState() {
    QMap<QString, SkyVariant> data;
    onSaveInstanceState(data);
    SkyVariant variant = SkyVariant::makeObject();
    for (QString key : data.keys()) {
        variant.insert(key.toStdString().c_str(), data.value(key));
    }
    if(mVolumeFadeInDuring>0){
        variant.insert("VolumeFadeIn",mVolumeFadeInDuring);
    }
    if(mVolumeFadeOutDuring>0){
        variant.insert("VolumeFadeOut",mVolumeFadeOutDuring);
    }
    m_skyClip->setUserData("sky_clip_data", variant);
}

void BaseClip::restoreInstanceState() {
    SkyVariant variant = m_skyClip->getUserData("sky_clip_data");
    QMap<QString, SkyVariant> data;
    std::vector<SkyString> holder(variant.size());
    variant.getAllKeys(holder.data(), holder.size());
    for (int i = 0; i < holder.size(); ++i) {
        data.insert(holder[i].c_str(), variant[holder[i].c_str()]);
    }
    mVolumeFadeInDuring = variant.getDouble("VolumeFadeIn",0);
    mVolumeFadeOutDuring = variant.getDouble("VolumeFadeOut",0);
    onRestoreInstanceState(data);
}

void BaseClip::onSaveInstanceState(QMap<QString, SkyVariant> &bundle) {
    if (m_inputDeviceId) {
        bundle.insert("input_device_id", SkyVariant(m_inputDeviceId));
    }
}

void BaseClip::onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) {
    if (bundle.contains("input_device_id")) {
        m_inputDeviceId = bundle.take("input_device_id").toInt64();
    }
}

SkyEffect *BaseClip::skyEffect() {
    auto wrapper = skyEffectWrapper();
    if (wrapper != nullptr) {
        return wrapper->skyEffect();
    }
    return nullptr;
}

SkyEffectWrapper *BaseClip::skyEffectWrapper() const{
    return nullptr;
}

SkyEffect *BaseClip::addDefaultEffect() {
    return nullptr;
}

void BaseClip::loadSkyEffect() {
    int num = m_skyClip->numberOfEffects();
    m_effectList.clear();
    m_curAudioEffect = nullptr;
    if (num == 0) {
        return;
    }
    std::vector<SkyEffect *> effects(num);
    m_skyClip->getEffects(effects.data(), num);
    for (int i = 0; i < num; ++i) {
        if(effects[i]->isVideoEffect()){
            auto wrapper = new SkyEffectWrapper(this, effects[i]);
            if (wrapper->isVenusEffect()) {
                m_hasVenusEffect = true;
            }
            m_effectList.append(wrapper);
            qDebug() << "LoadSkyEffect:" << wrapper->toString();

        }else{
            m_curAudioEffect = effects[i];
        }
    }
}

SkyVariant BaseClip::backupClipConfig() const{
    if(m_skyClip == nullptr){
        return SkyVariant::makeNull();
    }
    return m_skyClip->saveConfig();
}

void BaseClip::recoverClipConfig(const SkyVariant& config){
    if(m_skyClip == nullptr){
        return ;
    }
    m_skyClip->loadConfig(config, true);
    loadSkyEffect();
}

QJsonDocument BaseClip::getUIConfig() {
    return m_resBean.getUIConfig();
}

void BaseClip::setCurvedPointsList(const QList<QPointF> &points) {
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return;
    }
    if (points.isEmpty()) {
        SkyCurve curve;
        clip->setSpeedCurve(curve);
        emit ofParamChanged(this, "Clip:CurvedSpeed", false);
        return;
    }

    if (hasVenusEffect()) {
        AsyncTimelineFrameMgr::getIns()->updateClipProcessTask(this);
    }
    SkyCurve skyCurve;
    for (QPointF pt : points) {
        skyCurve.setItem(pt.x(), pt.y(), "bezier");
    }
    clip->setSpeedCurve(skyCurve);
    qDebug() << "===========Clip:" << toString() << " Timeline:" << clip->getTimeline()->getDuration()
             << " Points:" << points;

    updateVolumeFadeEffectParam();

    emit ofParamChanged(this, "Clip:CurvedSpeed", true);
}

QList<QPointF> BaseClip::getCurvedPoints() const {
    QList<QPointF> curPoints;
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return curPoints;
    }
    auto curve = clip->getSpeedCurve();
    int size = curve.numberOfItems();
    for (int i = 0; i < size; i++) {
        auto item = curve.getItemAt(i);
        curPoints.append(QPointF(item.time, item.value.toDouble()));
    }
    return curPoints;
}

double BaseClip::getPlayPTS(double sourcePts) {
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return 0;
    }
    return clip->trackTSFromTrimTS(sourcePts);
}

double BaseClip::getSourcePTS(double time) {
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return 0;
    }
    return clip->trimTSFromTrackTS(time);
}

void BaseClip::setSpeed(double speed) {
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return;
    }
    if (speed == getSpeed()) {
        return;
    }
    if (hasVenusEffect()) {
        AsyncTimelineFrameMgr::getIns()->updateClipProcessTask(this);
    }
    clip->setSpeed(speed);
    emit ofParamChanged(this, "Clip:Speed", speed);

    updateVolumeFadeEffectParam();

    qDebug() << "=====setSpeed!!" << speed;
}

double BaseClip::getSpeed() const {
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return 1.0;
    }
    if (!clip->getSpeedCurve().isEmpty()) {
        return 1.0;
    }
    return clip->getSpeed();
}

double BaseClip::getAvgSpeed() const {
    double avgSpeed = getCurvedSpeed();
    if (avgSpeed == 0) {
        avgSpeed = getSpeed();
    }
    return avgSpeed;
}

void BaseClip::attachAsyncProcessModel(AsyncProcessModel *model) {
    m_asyncProcessModel = model;
}

double BaseClip::getCurvedSpeed() const {
    SkyClip *clip = skyClip();
    if (clip == nullptr) {
        return 0.0;
    }
    auto skyCurve = clip->getSpeedCurve();
    if (skyCurve.isEmpty()) {
        return 0.0;
    }
    auto trackRange = clip->getTrackRange();
    auto trimRange = clip->getTrimRange();
    if (trackRange.end - trackRange.begin < 0.0001) {
        return 0;
    }

    return (trimRange.end - trimRange.begin) / (trackRange.end - trackRange.begin);
}

void BaseClip::updateAsyncProgress(double progress) {
    if (m_asyncProcessModel) {
        m_asyncProcessModel->updateProgress(progress);
    }
}

qint64 BaseClip::getInputId() const {
    return m_inputDeviceId;
}

void BaseClip::setInputId(qint64 id) {
    m_inputDeviceId = id;
    emit inputDeviceChanged();
}

void BaseClip::removeInputDevice() {
    m_inputDeviceId = 0;
    emit inputDeviceChanged();
}

bool BaseClip::equalsInputDeviceId(qint64 id) {
    return m_inputDeviceId == id;
}


void BaseClip::setSelectCurvedSpeedName(QString name){
    if(m_skyClip != nullptr){
        m_skyClip->setUserData("CurveName",name.toUtf8().data());
    }
}

QString BaseClip::getSelectCurvedSpeedName(){
    if(m_skyClip != nullptr){
        return QString::fromUtf8(m_skyClip->getUserData("CurveName").toString());
    }
    return "";
}

void BaseClip::setVolumeFadeInDuring(double d){
    mVolumeFadeInDuring = d;
    addAudioEffect();
    updateVolumeFadeEffectParam();
    emit ofParamChanged(this, "Clip:VolumeFadeIn",mVolumeFadeInDuring);
}

void BaseClip::setVolumeFadeOutDuring(double d){
    mVolumeFadeOutDuring = d;
    addAudioEffect();
    updateVolumeFadeEffectParam();
    emit ofParamChanged(this, "Clip:VolumeFadeOut",mVolumeFadeOutDuring);
}

void BaseClip::addAudioEffect(){
    if(m_curAudioEffect == nullptr){
        SkyEffectDescriptor descroptor;
        descroptor.className = SkyVolumeEffectClassName;
        descroptor.timeRange = SkyTimeRange(0, -1);
        m_curAudioEffect = m_skyClip->addEffect(descroptor, SkyEffectCategory_Audio);
    }else{

    }
}

void BaseClip::updateVolumeFadeEffectParam(){
    if(m_curAudioEffect == nullptr){
        return;
    }
    double maxFadeDuring = during()/2;
    if(mVolumeFadeInDuring > maxFadeDuring){
        mVolumeFadeInDuring = maxFadeDuring;
        emit ofParamChanged(this, "Clip:VolumeFadeIn",mVolumeFadeInDuring);
    }
    if(mVolumeFadeOutDuring > maxFadeDuring){
        mVolumeFadeOutDuring = maxFadeDuring;
        emit ofParamChanged(this, "Clip:VolumeFadeOut",mVolumeFadeOutDuring);
    }

    SkyCurve curve;
    auto cropRange = m_skyClip->getTrimRange();
    auto trackRange = m_skyClip->getTrackRange();
    if(mVolumeFadeInDuring > 0){
        double fadeInEndTime = getSourcePTS(trackRange.begin + mVolumeFadeInDuring) + cropRange.begin;
        curve.setItem(cropRange.begin,0,"linear");
        curve.setItem(fadeInEndTime,1,"linear");
    }
    if(mVolumeFadeOutDuring > 0){
        double fadeOutStartTime = getSourcePTS(trackRange.end - mVolumeFadeOutDuring) + cropRange.begin;
        curve.setItem(fadeOutStartTime,1,"linear");
        curve.setItem(cropRange.end,0,"linear");
    }
    m_curAudioEffect->setParamCurve("volume",curve);
}

QStringList BaseClip::getGroupInfos() const{
    QStringList groups;
    for (auto wrapper : m_effectList) {
        auto groupInfo = wrapper->getFashionLineGroupInfo();
        if (groupInfo.trimmed().length() > 0) {
            groups.append(groupInfo);
        }
    }
    return groups;
}
