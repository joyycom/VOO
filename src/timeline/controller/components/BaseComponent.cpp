#include "BaseComponent.h"
#include <QDebug>
#include <base/utils/SkyVariantUtil.h>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <timeline/models/effectclip.h>
#include <skymedia/SkyCurve.h>

BaseComponent::BaseComponent(QObject *parent) : QObject(parent) {
}

void BaseComponent::bindQml(QJsonObject &data, BaseClip *clip, SkyEffectWrapper *wrapper) {
    m_curClip = clip;
    m_curEffect = wrapper;
    qDebug() << "bindQml Begin!!! This:" << this << " Clip:" << clip << " Effect:" << wrapper;
    if (m_curClip != nullptr) {
        connect(m_curClip, &BaseClip::inputDeviceChanged, this, &BaseComponent::inputDeviceChanged);
        connect(m_curClip, &BaseClip::ofParamChanged, this, &BaseComponent::ofParamsChanged);
        connect(m_curClip, &QObject::destroyed, this, &BaseComponent::clipDestroyed);
    }
    onBindQml(data);
    emit bindComponent();
    qDebug() << "bindQml DONE!!! This :" << this << " Clip:" << clip << " Effect:" << wrapper;
}

void BaseComponent::ofMessage(QString msg) {
    qDebug() << "of message receive : " << msg;
    onOfMessage(msg);
}

BaseComponent::~BaseComponent() {
    if (m_componentBean != nullptr) {
        m_componentBean->unBindQml();
    }
}

BaseClip *BaseComponent::curClip() {
    return m_curClip;
}

void BaseComponent::unBind() {
    qDebug() << "unBind This2222:" << this;

    if (m_curClip != nullptr) {
        disconnect(m_curClip, &BaseClip::inputDeviceChanged, this, &BaseComponent::inputDeviceChanged);
        disconnect(m_curClip, &BaseClip::ofParamChanged, this, &BaseComponent::ofParamsChanged);
        disconnect(m_curClip, &QObject::destroyed, this, &BaseComponent::clipDestroyed);
        qDebug() << "unBind This222:" << this << " Clip:" << m_curClip; //<< " Effect:" << m_curEffect;
    }

    qDebug() << "unBind This333:" << this;
    onUnBind();
    m_curClip = nullptr;
    m_curEffect = nullptr;
    m_componentBean = nullptr;
}

void BaseComponent::onBindQml(QJsonObject &data) {
}

void BaseComponent::onOfMessage(QString msg) {
}

void BaseComponent::onUnBind() {
}

void BaseComponent::onClipParamsChanged() {
}

QString BaseComponent::translate(QString text) const {
    return m_translations.value(text).toString(text);
}



SkyVariant BaseComponent::getOfParamValue(QString key) {
    auto *wrapper = m_curEffect;
    if (wrapper == nullptr) {
        qDebug() << "getOfParamValue Failed! skyEffect is NULL Clip" << m_curClip << " This:" << this;
        return SkyVariant::makeNull();
    }

    return wrapper->getOfParamValue(key);
}

SkyVariant BaseComponent::getOfParamValue(QString key, SkyVariant defVal, bool forceRefresh) {
    auto variant = getOfParamValue(key);
    return variant.isNull()?defVal:variant;
}

void BaseComponent::updateOfParamIntValueByUser(QString key, int value, int preValue) {
    updateOfParamValueByUser(key, value, preValue);
}

bool BaseComponent::updateOfParamValueByUser(QString key, SkyVariant variant, SkyVariant preValue) {
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(key, variant);
    QMap<QString, SkyVariant> preValueMap;
    preValueMap.insert(key, preValue);
    return updateOfParamValuesByUser(paramMap, preValueMap);
}

bool BaseComponent::updateOfParamValuesByUser(QMap<QString, SkyVariant> paramMap,
                                              QMap<QString, SkyVariant> preValueMap) {
    if (m_curClip == nullptr || m_curEffect == nullptr) {
        qDebug() << "updateOfParamValues Had NOT BIND? " << m_curClip << " This:" << this;
        return false;
    }

    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    auto effectPath = m_curEffect->skyResource().entryPath();
    auto command = new OfParamEditCommand(timeline->timelineModel(), index, effectPath, paramMap, preValueMap);
    timeline->addUndoCommand(command);
    return true;
}

void BaseComponent::sendOfMessage(QString json) {
    if (m_curClip == nullptr) {
        qDebug() << "updateOfParamValues Had NOT BIND? " << m_curClip << " This:" << this;
        return;
    }
    SkyEffect *skyEffect = m_curClip->skyEffect();
    if (skyEffect == nullptr) {
        qDebug() << "getOfParamValue Failed! skyEffect is NULL Clip" << m_curClip << " This:" << this;
        return;
    }
    skyEffect->getOFWrapper()->sendMessage(json.toUtf8());
}



int BaseComponent::getOfParamIntValue(QString key, int defValue, bool forceRefresh) {
    SkyVariant variant = getOfParamValue(key, defValue, forceRefresh);

    int value = variant.isNull() ? defValue : variant.toInt();
    qDebug() << "getOfParamIntValue " << this << " Key:" << key << " Value:" << value;
    return value;
}

void BaseComponent::ofParamsChanged(BaseClip *clip, QString key, SkyVariant value, SkyEffectWrapper *effect) {
    if (m_curClip == clip && (effect == nullptr || effect == m_curEffect)) {
        checkKeyFrameChanged(key, value);
        onOfParamsChanged(key, value);
        emit ofParamChanged(key);
    } else {
    }
}

void BaseComponent::checkKeyFrameChanged(QString key, SkyVariant keyFrameKey) {
    if (key == "Clip:KeyFrame") {
        if (keyFrameKey.getType() == SkyVariantType_String) {
            emit keyFrameStateChanged(keyFrameKey.toString());
        }
    }
}

void BaseComponent::updateOfParamVariantValueBySilent(QString key, QVariant value) {
    updateOfParamValueBySilent(key, SkyVariantUtil::toSkyVariant(value));
}

void BaseComponent::updateOfParamVariantValueByUser(QString key, QVariant value, QVariant preValue) {
    updateOfParamValueByUser(key, SkyVariantUtil::toSkyVariant(value), SkyVariantUtil::toSkyVariant(preValue));
}

QVariant BaseComponent::getOfParamVariantValue(QString key, QVariant defValue) {
    if (key.startsWith("Clip:")) {
        return "";
    }
    auto def = SkyVariantUtil::toSkyVariant(defValue);
    return SkyVariantUtil::toQVariant(getOfParamValue(key, def));
}

SkyEffectWrapper *BaseComponent::curEffect() {
    return m_curEffect;
}

void BaseComponent::clipDestroyed(QObject *object) {
    disconnect(m_curClip, &BaseClip::inputDeviceChanged, this, &BaseComponent::inputDeviceChanged);
    disconnect(m_curClip, &BaseClip::ofParamChanged, this, &BaseComponent::ofParamsChanged);
    disconnect(m_curClip, &QObject::destroyed, this, &BaseComponent::clipDestroyed);
    m_curClip = nullptr;
}

void BaseComponent::onOfParamsChanged(QString key, SkyVariant skyVariant) {
    qDebug() << "onOfParamsChanged " << this << " Key:" << key << " Value:" << SkyVariantUtil::toString(skyVariant);
}

void BaseComponent::updateOfParamIntValueBySilent(QString key, int value) {
    updateOfParamValueBySilent(key, value);
}

bool BaseComponent::updateOfParamValueBySilent(QString key, SkyVariant variant) {
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(key, variant);
    return updateOfParamValuesBySilent(paramMap);
}

bool BaseComponent::updateOfParamValuesBySilent(QMap<QString, SkyVariant> paramMap) {
    auto *wrapper = m_curEffect;
    if (wrapper == nullptr) {
        qDebug() << "updateOfParamValuesBySilent Effect NULL? " << wrapper << " This:" << this;
        return false;
    }

    //    QMap<QString, SkyVariant> curParams = wrapper->getOfParams();

    //    QMap<QString, SkyVariant> updateParams;
    //    for (QString key : paramMap.keys()) {
    //        SkyVariant variant = paramMap[key];
    //        if (!SkyVariantUtil::varEquals(variant, curParams.value(key))) {
    //            qDebug() << "updateOfParamValues Key " << key << " Val:" <<
    //            SkyVariantUtil::toString(curParams.value(key))
    //                     << " ->" << SkyVariantUtil::toString(variant);
    //            updateParams.insert(key, variant);

    //        } else {
    //            qDebug() << "updateOfParamValues Ignore Key " << key << " Val:" <<
    //            SkyVariantUtil::toString(paramMap[key]);
    //        }
    //    }

    //    if (updateParams.isEmpty()) {
    //        qDebug() << "updateOfParamValues Value is Empty!";
    //        return true;
    //    }

    SEProject::current()->dom()->previewController()->stop();
    QMap<QString, SkyVariant> historyParams;
    wrapper->updateOfParamValues(paramMap, historyParams);
    return true;
}

void BaseComponent::enableKeyFrames(QStringList keys, QList<double> defaultValues) {
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    auto effectPath = m_curEffect->skyResource().entryPath();
    AddOfKeyFrameCommand *command =
        new AddOfKeyFrameCommand(timeline->timelineModel(), index, effectPath, keys, defaultValues);
    timeline->addUndoCommand(command);
}

void BaseComponent::removeKeyFrames(QStringList keys) {
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    auto effectPath = m_curEffect->skyResource().entryPath();
    RemoveOfKeyFrameCommand *command = new RemoveOfKeyFrameCommand(timeline->timelineModel(), index, effectPath, keys);
    timeline->addUndoCommand(command);
}

void BaseComponent::enableKeyFrame(QString key, double defaultValue) {
    QStringList keys;
    keys.append(key);
    QList<double> values;
    values.append(defaultValue);
    enableKeyFrames(keys, values);
}

void BaseComponent::removeKeyFrame(QString key) {
    QStringList keys;
    keys.append(key);
    removeKeyFrames(keys);
}

bool BaseComponent::isCurKeyFrame(QString key) {
    QStringList keys;
    keys.append(key);
    return isCurKeyFrame(keys);
}

void BaseComponent::updateKeyFrameTweenType(QString key, QString tween) {
    QStringList keys;
    keys.append(key);
    updateKeyFrameTweenTypes(keys, tween);
}

QString BaseComponent::keyFrameTweenType(QString key) {
    auto *wrapper = m_curEffect;
    if (wrapper == nullptr || m_curClip == nullptr) {
        qDebug() << "isCurKeyFrame Effect NULL? " << wrapper << " This:" << this;
        return "";
    }
    auto curTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    auto clipStartTime = m_curClip->startTime();
    auto clipEndTime = m_curClip->endTime();
    if (curTime < clipStartTime || curTime > clipEndTime) {
        return "";
    }

    return wrapper->keyFrameTweenType(key, curTime);
}

void BaseComponent::updateKeyFrameTweenTypes(QStringList keys, QString tween) {
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    auto effectPath = m_curEffect->skyResource().entryPath();
    UpdateKeyFrameTweenCommand *command =
        new UpdateKeyFrameTweenCommand(timeline->timelineModel(), index, effectPath, keys, tween);
    timeline->addUndoCommand(command);
}

bool BaseComponent::isCurKeyFrame(QStringList keys) {
    auto *wrapper = m_curEffect;
    if (wrapper == nullptr || m_curClip == nullptr) {
        qDebug() << "isCurKeyFrame Effect NULL? " << wrapper << " This:" << this;
        return false;
    }
    auto curTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    auto clipStartTime = m_curClip->startTime();
    auto clipEndTime = m_curClip->endTime();
    if (curTime < clipStartTime || curTime > clipEndTime) {
        return false;
    }
    for (auto key : keys) {
        if (!wrapper->isCurKeyFrame(key, curTime)) {
            return false;
        }
    }
    return true;
}

bool BaseComponent::hasInputConfig() {
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    InputDevice inputDevice = timeline->timelineModel()->getInputDeviceFromClip(timeline->curSelectedClip());
    return !inputDevice.isNull();
}

QString BaseComponent::extraInputConfig() {
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    InputDevice inputDevice = timeline->timelineModel()->getInputDeviceFromClip(timeline->curSelectedClip());
    return QJsonDocument(inputDevice.toJson()).toJson();
}

void BaseComponent::inputConfigEditingFinished(QString json) {
    auto jsonObject = QJsonDocument::fromJson(json.toUtf8());
    if (jsonObject.isNull()) {
        return;
    }
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    InputDevice inputDevice = InputDevice::makeInputDeviceFromJson(jsonObject.object());
    if (!inputDevice.isNull()) {
        timeline->timelineModel()->updateInputDevice(timeline->curSelectedClip(), inputDevice);
    }
}

bool BaseComponent::isEnableInput() {
    return true;
}
