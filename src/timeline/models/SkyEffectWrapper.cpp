#include "SkyEffectWrapper.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>

#include "project/SEProject.h"
#include <base/utils/AppPath.h>
#include <base/utils/HandleMap.h>
#include <timeline/venus/AsyncTimelineFrameMgr.h>
#include "src/base/utils/OrangeFilterUtils.h"
#include <base/utils/AppConstance.h>
#include <base/utils/SkyVariantUtil.h>

#include "orangefilter/of_effect.h"

#include <base/utils/fileutils.h>
#include <base/utils/threadpools.h>

static SkyString onMessageBack(void *userData, const char *msg) {

    int64_t handler = (int64_t)userData;
    void *wrapper = HandleMap::getInstance().lockHandle(handler);
    if (wrapper == nullptr) {
        qDebug() << "onMessageBack: Wrapper is NULL";
        return "";
    }

    auto content = QString::fromUtf8(msg);
    ((SkyEffectWrapper *)wrapper)->onOfMessage(content);
    SkyString result(0);
    return result;
}

static void onCreateEffect(void *userData, int contextID, int effectID) {
    qDebug() << "onCreateEffect:" << userData;
    int64_t handler = (int64_t)userData;
    void *wrapper = HandleMap::getInstance().lockHandle(handler);
    if (wrapper == nullptr) {
        return;
    }
    ((SkyEffectWrapper*)wrapper)->onEffectCreated(contextID,effectID);
    qDebug() << "onCreateEffect: Handler:" << userData << " Clip:" << wrapper;
}

static void onDestory(void *userData) {
    qDebug() << "onDestory:" << userData;
}

SkyEffectWrapper::SkyEffectWrapper(QObject *ownerClip, SkyResourceBean resource, SkyEffect *effect)
    : QObject(ownerClip),
      m_effect(effect),
      m_ownerClip(ownerClip),
      m_resBean(resource) {
    registerOfMsgCallback();
    collectKeyFrameList();
    tryFixSystemFontPath();
    auto previewController = SEProject::current()->dom()->previewController();
    QObject::connect(previewController,
                     &PreviewController::videoProgressChanged,
                     this,
                     &SkyEffectWrapper::onTimelinePlayProgressChanged);
}

SkyEffectWrapper::SkyEffectWrapper(QObject *ownerClip, SkyEffect *effect)
    : QObject(ownerClip),
      m_effect(effect),
      m_ownerClip(ownerClip) {
    registerOfMsgCallback();

    restoreUserData();
    saveUserData();

    effect->getOFWrapper()->setMessageCallback(ofCallback);
    effect->getOFWrapper()->setNeedsUpdateDuration(true);

    tryAddVenusProcessor();
    collectKeyFrameList();
    tryFixSystemFontPath();

    auto previewController = SEProject::current()->dom()->previewController();
    QObject::connect(previewController,
                     &PreviewController::videoProgressChanged,
                     this,
                     &SkyEffectWrapper::onTimelinePlayProgressChanged);
}

QString SkyEffectWrapper::getOfEffectPath() {
    return m_resBean.entryPath();
}

SkyResourceBean &SkyEffectWrapper::skyResource() {
    return m_resBean;
}

void SkyEffectWrapper::registerOfMsgCallback() {
    auto handler = HandleMap::getInstance().allocHandle(this);
    qDebug() << "SkyEffectWrapper:registerOfMsgCallback " << this << " Handler:" << handler;
    ofCallback.userData = (void *)handler;
    ofCallback.onMessageBack = onMessageBack;
    ofCallback.onCreateEffect = onCreateEffect;
    ofCallback.onDestory = onDestory;
}

SkyEffect *SkyEffectWrapper::skyEffect() {
    return m_effect;
}

SkyEffectWrapper::~SkyEffectWrapper() {
    auto previewController = SEProject::current()->dom()->previewController();
    QObject::disconnect(previewController,
                        &PreviewController::videoProgressChanged,
                        this,
                        &SkyEffectWrapper::onTimelinePlayProgressChanged);

    HandleMap::getInstance().freeHandle(this);
    m_effect = nullptr;
    m_ownerClip = nullptr;
    qDebug() << "SkyEffectWrapper onDestroy";
}

SkyEffect *SkyEffectWrapper::addEffect() {
    if (m_effect != nullptr) {
        return m_effect;
    }

    BaseClip *baseClip = ((BaseClip *)m_ownerClip);
    SkyClip *skyClip = baseClip->skyClip();

    QJsonObject jsonObject;
    SkyVariant skyParams = SkyVariant::makeObject();

    QString effectPath = m_resBean.entryPath();
    if (!QFile(effectPath).exists()) {
        qDebug() << "Add Filter Failed: File NOT EXISTS! " << effectPath;
        return nullptr;
    }

    skyParams.insert("effectPath", SkyVariant::makePath(effectPath.toUtf8().data()));
    if(m_resBean.resouceType() == TextResource) {
        auto ofParam = SkyVariant::makeObject();
        ofParam.insert("0:SystemFontDir", SkyVariant::makePath(AppPath::systemFontPath().toUtf8().data()));
        ofParam.insert("0:SystemFontNames", OrangeFilterUtils::getLocalTTF().toStdString().c_str());
        skyParams.insert("ofParam", ofParam);
    }

    SkyEffectDescriptor descroptor;
    descroptor.className = SkyOrangeEffectClassName;
    descroptor.params = skyParams;
    descroptor.timeRange = SkyTimeRange(0, -1);
    m_effect = skyClip->addEffect(descroptor);

    saveUserData();

    QStringList inputListConfig;

    auto owerType = ((BaseClip *)m_ownerClip)->clipType();
    if (owerType == VideoClipType || owerType == ImageClipType) {
        if (m_resBean.resouceType() == OverlayResource) {
            m_effect->setPriority(PIC_IN_PIC_BG_EFFECT_PRIORITY);
            inputListConfig.append("background");
            inputListConfig.append("user");
        } else {
            m_effect->setPriority(DEFAULT_EFFECT_PRIORITY);
            inputListConfig.append("user");
        }
    } else {
        m_effect->setPriority(DEFAULT_EFFECT_PRIORITY);
        inputListConfig.append("background");
    }

    auto uiconfig = m_resBean.getUIConfig().object();
    auto inputListUiConfig = uiconfig.value("inputList").toArray();
    if(!inputListUiConfig.isEmpty()){
        inputListConfig.clear();
        for(auto input:inputListUiConfig){
            auto inputStr = input.toString("");
            if(!inputStr.isEmpty()){
                inputListConfig.append(input.toString());
            }
        }
    }

    SkyInputList *inputList = m_effect->getInputList();
    inputList->ensureNumber(inputListConfig.size());
    for(int i=0;i<inputListConfig.size();++i){
        if( inputListConfig[i].compare("user") == 0){
            inputList->getItemAt(i)->setAsUser();
        }else if(inputListConfig[i].compare("background") == 0){
            inputList->getItemAt(i)->setAsBackground();
        }else if(inputListConfig[i].compare("gap") == 0){
            SkyResource gapResource = SkyResource::makeGap();
            inputList->getItemAt(i)->setAsResource(gapResource);
        }else{
            auto resPath = m_resBean.resourceDir()+"/"+inputListConfig[i];
            SkyResource inputRes(resPath.toUtf8().data());
            inputList->getItemAt(i)->setAsResource(inputRes);
        }
    }

    inputList->commit();
    m_effect->getOFWrapper()->setMessageCallback(ofCallback);
    m_effect->getOFWrapper()->setNeedsUpdateDuration(true);

    qDebug() << "addEffect:" << m_effect << " Path:" << effectPath;

    tryAddVenusProcessor();

    trySetGroupInfo(uiconfig);

    return m_effect;
}

void SkyEffectWrapper::tryAddVenusProcessor() {
    QJsonDocument doc = m_resBean.getUIConfig();
    auto venus = doc.object().value("venus").toArray();
    if (venus.isEmpty()) {
        return;
    }

    m_isVenusEffect = true;
    if(m_effect->getUserData("FrameCallbackParams").isNull()){
        SkyVariant params = SkyVariant::makeObject();
        SkyVariant venusApis = SkyVariant::makeArray();
        for (int i = 0; i < venus.size(); ++i) {
            venusApis.append(venus[i].toString().toUtf8().data());
        }
        params.insert("venus", venusApis);

        SkyFrameDataDescriptor descriptor;
        descriptor.className = "VenusFrameProcessor";
        descriptor.name = "";
        descriptor.params = params;
        descriptor.inputKeys.append(SkyInputKey_OriginVideoFrame);
        descriptor.cachesStrategy = SkyCachesStrategy_MemoryAndDisk;
        m_effect->addFrameDataCallback(descriptor);

        m_effect->setUserData("FrameCallbackParams", params);
        m_effect->setPriority(VENUS_FRONT_EFFECT_PRIORITY);
    }

    AsyncTimelineFrameMgr::getIns()->updateClipProcessTask((BaseClip *)m_ownerClip);
}

void SkyEffectWrapper::trySetGroupInfo(QJsonObject &uiconfig){
    QJsonArray paramList = uiconfig["paramList"].toArray();
    for(int i=0;i<paramList.size();++i){
        auto paramInfo = paramList.at(i).toObject();
        if(paramInfo.value("uiType").toString() == "EffectGroupEditor"){
            auto paramKey = paramInfo["paramKey"].toString("EffectGroupKey");
            auto paramData = getUserData(paramKey);
            if(!paramData.isEmpty()){
                return;
            }
            auto defValue = paramInfo["defValue"].toString("");
            if(!defValue.isEmpty()){
                setUserData(paramKey, defValue);
                qDebug()<<"setDefault Group "<<paramKey<<" ->"<<defValue;
            }
            return;
        }
    }
}

void SkyEffectWrapper::removeEffect() {
    if (m_effect != nullptr) {
        SkyOFMessageCallback callback;
        SkyOFWrapper *wrapper = m_effect->getOFWrapper();
        if (wrapper) {
            wrapper->setMessageCallback(callback);
        }
        m_effect->removeFromOwner();
        m_effect = nullptr;

        if (m_isVenusEffect) {
            AsyncTimelineFrameMgr::getIns()->removeClipProcessTask(((BaseClip *)m_ownerClip)->skyClip());
        }
    }
}

bool SkyEffectWrapper::isVenusEffect() {
    return m_isVenusEffect;
}

bool SkyEffectWrapper::isAudioEffect() {
    return m_effect->isAudioEffect();
}

bool SkyEffectWrapper::isVideoEffect(){
    return m_effect->isVideoEffect();
}

void SkyEffectWrapper::onOfMessage(QString msg) {
    ThreadPools::instance()->mainThread([=]{
        SEProject::current()->dom()->propertyEditController()->receiveOfMessage((BaseClip *)m_ownerClip, msg);
    });
}

void SkyEffectWrapper::saveUserData() {
    if(m_effect == nullptr || !m_resBean.isValidResource()){
        return;
    }
    m_resBean.saveSkyVariant(m_effect);
}

void SkyEffectWrapper::restoreUserData() {
    if(m_effect == nullptr){
        return;
    }

    m_resBean = SkyResourceBean::restoreSkyVariant(m_effect);
    if(!m_resBean.isValidResource()){
        auto effectPath = m_effect->getParams().getString("effectPath", "");
        auto resourceType = m_effect->getInputList()->numberOfItems() == 2?OverlayResource:EffectResource;
        m_resBean = SkyResourceBean(QString::fromUtf8(effectPath),"",resourceType);
        qDebug() << "restoreInstanceState isValidResource " << m_resBean.toString() << " "<<&m_resBean;
    }else{
        qDebug() << "restoreInstanceState ValidResource " << m_resBean.toString()<< " "<<&m_resBean;
    }

}

QString SkyEffectWrapper::toString() {
    return QString("SkyEffectWrapper Path:%1 Owner:%2")
        .arg(m_resBean.entryPath())
        .arg(m_ownerClip != nullptr ? ((BaseClip *)m_ownerClip)->toString() : "NULL");
}

SkyVariant SkyEffectWrapper::getParams() {
    if (m_effect == nullptr) {
        return SkyVariant::makeNull();
    }
    return m_effect->getParams();
}

SkyVariant SkyEffectWrapper::saveConfig(){
    if (m_effect == nullptr) {
        return SkyVariant::makeNull();
    }
    return m_effect->saveConfig();
}

SkyVariant SkyEffectWrapper::getKeyFrameParams() {
    if (m_effect == nullptr) {
        return SkyVariant::makeNull();
    }
    collectKeyFrameList();
    SkyVariant keyFrames = SkyVariant::makeObject();
    for (QString key : m_keyFrameKeyList) {
        auto skyCurve = m_effect->getParamCurve(("ofParam/" + key).toUtf8().data());
        if (!skyCurve.isEmpty()) {
            keyFrames.insert(("ofParam/" + key).toUtf8().data(), skyCurve.saveConfig());
        }
    }
    return keyFrames;
}

void SkyEffectWrapper::updateParams(SkyVariant params) {
    if (m_effect != nullptr) {
        m_effect->updateParams(params);
    }
}

void SkyEffectWrapper::enableKeyFrame(QString key, double curTime) {
    auto skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "enableKeyFrame Effect NULL? "
                 << " This:" << this;
        return;
    }

    BaseClip *clip = (BaseClip *)m_ownerClip;
    auto sourcePTS = clip->getSourcePTS(curTime);

    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        m_keyFrameKeyList.append(key);
    }

    auto tween = keyFrameTweenType(key,curTime);
    if(tween.isEmpty()){
        tween = "linear";
    }
    skyCurve.setItem(sourcePTS, getOfParamValue(key), tween.toUtf8().data());

    skyEffect->setParamCurve(("ofParam/" + key).toUtf8().data(), skyCurve);

    qDebug() << "enableKeyFrame key " << key << " time:" << sourcePTS;

    QList<QString> keys;
    keys.append(key);
    sendOfParamChangedEvent(keys);


    QVector<int> d;
    d.append(RoleKeyFrameTimes);
    emit clip->clipDataChanged(clip, d);
}

bool SkyEffectWrapper::isCurKeyFrame(QString key, double curTime) {
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "isCurKeyFrame Effect NULL? "
                 << " This:" << this;
        return false;
    }
    BaseClip *clip = (BaseClip *)m_ownerClip;
    auto sourcePTS = clip->getSourcePTS(curTime);

    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        return false;
    }
    int num = skyCurve.numberOfItems();
    for (int i = 0; i < num; ++i) {
        if (abs(skyCurve.getItemAt(i).time - sourcePTS) < TIME_DEVIATION) {
            return true;
        }
    }

    return false;
}

bool SkyEffectWrapper::removeKeyFrame(QString key, double curTime) {
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "updateOfParamValuesBySilent Effect NULL? "
                 << " This:" << this;
        return false;
    }
    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        return false;
    }

    BaseClip *clip = (BaseClip *)m_ownerClip;
    auto sourcePTS = clip->getSourcePTS(curTime);

    skyCurve.removeItemAtTime(sourcePTS);
    if (skyCurve.isEmpty()) {
        m_keyFrameKeyList.removeOne(key);
    }
    skyEffect->setParamCurve(("ofParam/" + key).toUtf8().data(), skyCurve);

    QList<QString> keys;
    keys.append(key);
    sendOfParamChangedEvent(keys);


    qDebug() << "removeKeyFrame key " << key << " time:" << curTime;

    QVector<int> d;
    d.append(RoleKeyFrameTimes);
    emit clip->clipDataChanged(clip, d);

    return true;
}

QString SkyEffectWrapper::keyFrameTweenType(QString key, double curTime){
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "keyFrameTweenType Effect NULL? "<< " This:" << this;
        return "";
    }
    BaseClip *clip = (BaseClip *)m_ownerClip;
    if(curTime<clip->startTime() || curTime > clip->endTime()){
        return "";
    }

    auto sourcePTS = clip->getSourcePTS(curTime);

    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        return "";
    }
    double preKeyFrameTime = 0;
    int num = skyCurve.numberOfItems();
    for (int i = 0; i < num; ++i) {
        if (preKeyFrameTime <= sourcePTS && sourcePTS < skyCurve.getItemAt(i).time + TIME_DEVIATION) {
            return QString::fromUtf8(skyCurve.getItemAt(i).tweenName);
        }
        preKeyFrameTime = skyCurve.getItemAt(i).time;
    }
    return "";
}

QString SkyEffectWrapper::updateKeyFrameTweenType(QString key,QString tween, double curTime){
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "keyFrameTweenType Effect NULL? "<< " This:" << this;
        return "";
    }
    BaseClip *clip = (BaseClip *)m_ownerClip;
    if(curTime<clip->startTime() || curTime > clip->endTime()){
        return "";
    }

    auto sourcePTS = clip->getSourcePTS(curTime);

    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        return "";
    }
    double preKeyFrameTime = 0;
    int num = skyCurve.numberOfItems();
    for (int i = 0; i < num; ++i) {
        if (preKeyFrameTime <= sourcePTS && sourcePTS < skyCurve.getItemAt(i).time + TIME_DEVIATION) {
            QString preTweenName = QString::fromUtf8(skyCurve.getItemAt(i).tweenName);
            skyCurve.setItem(skyCurve.getItemAt(i).time,skyCurve.getItemAt(i).value,tween.toUtf8().data());
            skyEffect->setParamCurve(("ofParam/" + key).toUtf8().data(), skyCurve);

            qDebug() << "updateKeyFrameTweenType key:"<<key <<" Tween "<< preTweenName << " -> "<<tween;
            return preTweenName;
        }
        preKeyFrameTime = skyCurve.getItemAt(i).time;
    }

    QList<QString> keys;
    keys.append(key);
    sendOfParamChangedEvent(keys);

    return "";
}

bool SkyEffectWrapper::tryAddKeyFrame(QString key, double curTime, SkyVariant value) {
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "updateOfParamValuesBySilent Effect NULL? "
                 << " This:" << this;
        return false;
    }
    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        return false;
    }

    BaseClip *clip = (BaseClip *)m_ownerClip;
    auto sourcePTS = clip->getSourcePTS(curTime);

    auto tween = keyFrameTweenType(key,curTime);
    if(tween.isEmpty()){
        tween = "linear";
    }

    skyCurve.setItem(sourcePTS, value, tween.toUtf8().data());

    skyEffect->setParamCurve(("ofParam/" + key).toUtf8().data(), skyCurve);

    QList<QString> keys;
    keys.append(key);
    sendOfParamChangedEvent(keys);

    QVector<int> d;
    d.append(RoleKeyFrameTimes);
    emit clip->clipDataChanged(clip, d);
    return true;
}

QMap<QString, SkyVariant> SkyEffectWrapper::getOfParamsForce() const {
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "getOfParams Failed! skyEffect is NULL Clip"
                 << " This:" << this;
        return QMap<QString, SkyVariant>();
    }
    m_ofParams.clear();
    SkyVariant params = skyEffect->getParams().find("ofParam");
    std::vector<SkyString> list(params.size());
    params.getAllKeys(list.data(), list.size());
    for (auto &key : list) {
        m_ofParams.insert(key.c_str(), params.find(key.c_str()));
    }
    return m_ofParams;
}

QMap<QString, SkyVariant> SkyEffectWrapper::getOfParams() const {
    if (m_ofParams.isEmpty()) {
        getOfParamsForce();
    }
    return m_ofParams;
}

SkyVariant SkyEffectWrapper::getOfParamValue(QString key) const {
    QMap<QString, SkyVariant> map = getOfParams();
    if (map.contains(key)) {
        return map.value(key, SkyVariant::makeNull());
    }
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        return SkyVariant::makeNull();
    }
    auto ofWrapper = skyEffect->getOFWrapper();
    auto data = ofWrapper->getOFParam(key.toUtf8().data());
    return data;
}

SkyVariant SkyEffectWrapper::getOfParamValue(QString key, const SkyVariant& defVal) const {
    SkyVariant variant = getOfParamValue(key);
    return variant.isNull()?defVal:variant;
}

QList<double> SkyEffectWrapper::getKeyFrameTimes() const {
    QList<double> times;
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "getOfParams Failed! skyEffect is NULL Clip"
                 << " This:" << this;
        return times;
    }
    QSet<double> timeSets;

    for (int i = 0; i < m_keyFrameKeyList.size(); ++i) {
        QString key = m_keyFrameKeyList[i];
        auto set = QSet<double>::fromList(getKeyFrameTimes(key));
        timeSets = timeSets.unite(set);
    }
    return timeSets.toList();
}

void SkyEffectWrapper::collectKeyFrameList() {
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "getOfParams Failed! skyEffect is NULL Clip"
                 << " This:" << this;
        return;
    }
    m_keyFrameKeyList.clear();
    SkyVariant params = skyEffect->getParams().find("ofParam");
    std::vector<SkyString> list(params.size());
    params.getAllKeys(list.data(), list.size());
    for (auto keyCstr : list) {
        QString key = QString::fromUtf8(keyCstr.c_str());
        auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
        if (!skyCurve.isEmpty()) {
            m_keyFrameKeyList.append(key);
        }
    }
    qDebug() << "collectKeyFrameList "<<m_keyFrameKeyList;
}

QList<double> SkyEffectWrapper::getKeyFrameTimes(QString key) const {
    SkyEffect *skyEffect = m_effect;
    if (skyEffect == nullptr) {
        qDebug() << "updateOfParamValuesBySilent Effect NULL? "
                 << " This:" << this;
        return QList<double>();
    }
    BaseClip *clip = (BaseClip *)m_ownerClip;
    auto skyCurve = skyEffect->getParamCurve(("ofParam/" + key).toUtf8().data());
    if (skyCurve.isEmpty()) {
        return QList<double>();
    }
    QList<double> keyFrames;
    int num = skyCurve.numberOfItems();
    auto trimRange = clip->skyClip()->getTrimRange();
    for (int i = 0; i < num; ++i) {
        auto sourceTime = skyCurve.getItemAt(i).time;
        if (sourceTime >= trimRange.begin && sourceTime <= trimRange.end) {
            keyFrames.append(clip->getPlayPTS(sourceTime));
        }
    }

    qDebug() << "getKeyFrameTimes key " << key << " keyFrames:" << keyFrames;
    return keyFrames;
}

QMap<QString, SkyVariant> SkyEffectWrapper::updateOfParamValues(QMap<QString, SkyVariant> &paramMap,
                                                                QMap<QString, SkyVariant> &historyParams) {
    SkyEffect *skyEffect = m_effect;
    BaseClip *owner = (BaseClip *)m_ownerClip;
    if (skyEffect == nullptr || owner == nullptr) {
        qDebug() << "updateOfParamValue Failed! skyEffect is NULL Clip:";
        return QMap<QString, SkyVariant>();
    }
    SkyVariant params = skyEffect->getParams();
    if (params.isNull()) {
        params = SkyVariant::makeObject();
    }
    SkyVariant ofParam = params.find("ofParam");
    if (ofParam.isNull()) {
        ofParam = SkyVariant::makeObject();
    }

    QList<QString> updatedKeys;

    for (QString key : paramMap.keys()) {
        SkyVariant preVal = ofParam.find(key.toStdString().c_str());
        SkyVariant curVal = paramMap.value(key);
        if (!historyParams.contains(key)) {
            historyParams.insert(key, preVal);
        }
        if (SkyVariantUtil::varEquals(curVal, m_ofParams.value(key))) {
            continue;
        }
        updatedKeys.append(key);
        m_ofParams.insert(key, curVal);
        ofParam.insert(key.toStdString().c_str(), curVal);
    }
    params.insert("ofParam", ofParam);
    skyEffect->updateParams(params);

    for (QString key : updatedKeys) {
        emit owner->ofParamChanged(owner, key, paramMap.value(key, SkyVariant::makeNull()), this);
    }

    sendOfParamChangedEvent(updatedKeys);

    qDebug() <<"Wrapper:"<<this<<" Effect:"<<m_resBean.name()<< " updateOfParamValue Success " << historyParams.size() << " Keys:" << historyParams.keys()
             << " OfParams: " << ofParam.transToJsonString(false).c_str();
    return historyParams;
}

void SkyEffectWrapper::moveIndex(int targetIndex){
    if(m_effect == nullptr){
        return;
    }
    int preIndex = m_effect->getIndex();
    m_effect->moveToIndex(targetIndex);
    qDebug()<<"MoveToIndex:"<<preIndex<<"->"<<targetIndex<<" Result:"<<m_effect->getIndex();
}


void SkyEffectWrapper::tryFixSystemFontPath(){
    if(m_effect == nullptr){
        return;
    }
    SkyVariant params = m_effect->getParams();
    if (params.isNull()) {
        params = SkyVariant::makeObject();
    }
    SkyVariant ofParam = params.find("ofParam");
    if (ofParam.isNull()) {
        ofParam = SkyVariant::makeObject();
    }
    auto systemFontDir = ofParam.find("0:SystemFontDir");
    if(!systemFontDir.isNull() && !QFileInfo(systemFontDir.toString()).exists()){
        ofParam.insert("0:SystemFontDir", SkyVariant::makePath(AppPath::systemFontPath().toUtf8().data()));
        params.insert("ofParam", ofParam);
        m_effect->updateParams(params);
    }
}

void SkyEffectWrapper::onTimelinePlayProgressChanged(double progress) {
    if (m_ownerClip == nullptr) {
        return;
    }
    if (m_keyFrameKeyList.isEmpty()) {
        return;
    }
    auto dom = SEProject::current()->dom();
    auto curTime = dom->timeline()->getCurrentTime();

    if (dom->propertyEditController() == nullptr || dom->propertyEditController()->curSelectedClip() != m_ownerClip) {
        m_lastKeyFrameStates.clear();
        return;
    }

    auto owner = ((BaseClip *)m_ownerClip);

    auto startTime = owner->startTime();
    auto endTime = owner->endTime();
    if (curTime < startTime || curTime > endTime) {
        m_lastKeyFrameStates.clear();
        qDebug() << " onTimelinePlayProgressChanged Changed: " << curTime << " ->" << owner->toString();
        return;
    }
    auto preOfParams = m_ofParams;
    getOfParamsForce();
    SkyVariant ofParams = m_effect->getParams().find("ofParam");
    for (auto key : m_keyFrameKeyList) {
        auto lastIsKeyFrame = m_lastKeyFrameStates.value(key);
        auto isKeyFrame = isCurKeyFrame(key,curTime);
        if(isKeyFrame != lastIsKeyFrame){
            m_lastKeyFrameStates.insert(key,isKeyFrame);
            emit owner->ofParamChanged(owner, "Clip:KeyFrame",key.toUtf8().data());
        }

        auto preValue = preOfParams.value(key, SkyVariant::makeNull());
        auto curValue = ofParams.find(key.toUtf8().data());
        if (preValue != curValue && !curValue.isNull()) {
            qDebug() << " onTimelinePlayProgressChanged Changed: " << key << " ->" << curValue.toDouble();
            emit owner->ofParamChanged(owner, key, curValue, this);
        } else {
        }
    }
}

QString SkyEffectWrapper::getFashionLineGroupInfo(){
    return getUserData("fashionLineGroup");
}

void SkyEffectWrapper::setUserData(QString key,QString value){
    skyEffect()->setUserData(key.toUtf8().data(),value.toUtf8().data());
    if(key == "fashionLineGroup"){
        auto owner = ((BaseClip *)m_ownerClip);
        QVector<int> d;
        d.append(RoleGroupInfos);
        emit owner->clipDataChanged(owner,d);
    }
}

QString SkyEffectWrapper::getUserData(QString key){
    return QString::fromUtf8(skyEffect()->getUserData(key.toUtf8().data()).toString());
}


void SkyEffectWrapper::onEffectCreated(int contextId, int effectId){
    m_ofContextID = contextId;
    m_ofEffectID = effectId;

    qDebug() << "onCreateEffect: m_ofContextID:" << m_ofContextID << " m_ofEffectID:" << m_ofEffectID;

}

void SkyEffectWrapper::saveOfEffectParamFile(){
    if(m_ofContextID < 0 || m_ofEffectID < 0){
        return;
    }
    auto effectPath = m_resBean.entryPath();
    SkyVariant params = m_effect->getParams();
    if (params.isNull()) {
        params = SkyVariant::makeObject();
    }
    SkyVariant ofParam = params.find("ofParam");

    bool isChanged = false;
    QString resourceDir = m_resBean.resourceDir();
    auto keys = ofParam.getAllKeys();
    for(int i=0;i<keys.size();++i){
        auto key = keys.c_strAt(i);
        auto paramValue = ofParam.find(key);
        if(paramValue.hasPathTag()){
            QString path = QString::fromUtf8(paramValue.toString());
            if(path.startsWith(resourceDir)){

            }else{
                QFileInfo file(path);
                QString filePath = resourceDir+"/res/"+file.fileName();
                FileUtils::copyFile(path,filePath,true);
                QString absPath = "res/"+file.fileName();
                ofParam.insert(key,absPath.toUtf8().data());
                isChanged = true;
                qInfo() << "saveOfEffectParamFile: path:" <<path<<" ->"<<filePath;
            }
        }
    }

    if(isChanged){
        params.insert("ofParam", ofParam);

        m_effect->updateParams(params);

        ThreadPools::instance()->mainThread([=]{
            OF_Result result = OF_SaveEffectToFile(m_ofContextID,m_ofEffectID,effectPath.toUtf8().data());
            qDebug()<<"saveOfEffectParamFile:"<<result;
        },33);
    }else{
        OF_Result result = OF_SaveEffectToFile(m_ofContextID,m_ofEffectID,effectPath.toUtf8().data());
        qDebug()<<"saveOfEffectParamFile:"<<result;
    }
}


void SkyEffectWrapper::sendOfParamChangedEvent(QList<QString> keys){
    if(keys.isEmpty()){
        return;
    }
    QJsonObject json;
    json.insert("event","ofParamChanged");
    QJsonArray changedParams;
    for(QString key:keys){
        changedParams.append(key.right(key.length() - 2));
    }
    json.insert("params",changedParams);
    QString jsonStr = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_effect->getOFWrapper()->sendMessage(jsonStr.toUtf8());
}
