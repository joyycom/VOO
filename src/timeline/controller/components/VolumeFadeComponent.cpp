#include "VolumeFadeComponent.h"

#include <QJsonArray>
#include <QDebug>

#include <timeline/controller/timelinecontroller.h>

#include <project/SEProject.h>

#include <timeline/command/PropertyEditCommand.h>

VolumeFadeComponent::VolumeFadeComponent(QObject *parent) : BaseComponent(parent) {
}

void VolumeFadeComponent::onBindQml(QJsonObject &paramInfo) {
    mParamTitle = paramInfo["paramTitle"].toString();
    m_maxDuring = paramInfo.value("maxValue").toDouble(2);
    m_minDuring = paramInfo.value("minValue").toDouble(0);
    mFadeType = paramInfo.value("FadeType").toString();
    m_maxDuring = fmin(curClip()->during() / 2 ,10);
}

void VolumeFadeComponent::onUnBind() {
}

double VolumeFadeComponent::getDuring() {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "getVolume CLIP is NULL??";
        return 0;
    }

    if(isFadeIn()){
        return clip->getVolumeFadeInDuring();
    }else{
        return clip->getVolumeFadeOutDuring();
    }
}

void VolumeFadeComponent::setDuring(double during) {

}


void VolumeFadeComponent::setDuringByUser(double preDuring, double during) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "setVolume CLIP is NULL??";
        return;
    }
    if (preDuring == during) {
        return;
    }

    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    ClipVolumeFadeChangedCommand *command =
        new ClipVolumeFadeChangedCommand(timeline->timelineModel(), index,mFadeType, preDuring, during);
    timeline->addUndoCommand(command);
}

double VolumeFadeComponent::getMaxDuring() {
    return m_maxDuring;
}

double VolumeFadeComponent::getMinDuring() {
    return m_minDuring;
}

void VolumeFadeComponent::onClipParamsChanged() {
    double maxDuring = fmin(curClip()->during() / 2 ,10);
    if(abs(maxDuring - m_maxDuring) > 0.01){
        m_maxDuring = maxDuring;
        emit maxDurationChanged(m_maxDuring);
    }
}

void VolumeFadeComponent::onOfParamsChanged(QString key, SkyVariant value){
    if(key == "Clip:VolumeFadeIn" && isFadeIn()){
        emit fadeDuringChanged(getDuring());
    }else if(key == "Clip:VolumeFadeOut"){
        emit fadeDuringChanged(getDuring());
    }
}
