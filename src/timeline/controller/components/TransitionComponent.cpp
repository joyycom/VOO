#include "TransitionComponent.h"

#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <timeline/controller/timelinecontroller.h>
#include <timeline/models/TransitionClip.h>

#include <QDebug>
#include <QJsonArray>

TransitionComponent::TransitionComponent(QObject *parent) : BaseComponent(parent) {
}

void TransitionComponent::onBindQml(QJsonObject &paramInfo) {
    m_maxDuring = paramInfo.value("maxValue").toString("2").toDouble();
    m_minDuring = paramInfo.value("minValue").toString("0.1").toDouble();
    BaseClip *clip = curClip();
    if(clip->clipType() == TransitionClipType){
        ((TransitionClip*)clip)->setConfigTransition(m_minDuring,m_maxDuring);
    }
    qDebug() << "onBindQml " << this << " MaxValue:" << m_maxDuring << " MinValue:" << m_minDuring;
}

void TransitionComponent::onUnBind() {
}

double TransitionComponent::getDuring() {
    BaseClip *clip = curClip();
    if (clip == nullptr) {
        qDebug() << "getDuring CLIP is NULL?? " << this;
        return 0;
    }
    if (clip == nullptr || clip->resourceBean().resouceType() != TransitionResource) {
        qDebug() << "getDuring CLIP is NULL?? Resource:" << clip->resourceBean().toString();
        return 0;
    }
    TransitionClip *transClip = (TransitionClip *)clip;
    double during = transClip->skyTransition()->getDuration();
    qDebug() << "getDuring " << during << " " << this;
    return during;
}

void TransitionComponent::setDuring(double during) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->clipType() != TransitionClipType) {
        qDebug() << "setDuring CLIP is NULL??";
        return;
    }
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    timeline->setTransitionDuring(during);

    if (during < m_minDuring) {
        during = m_minDuring;
    } else if (during > m_maxDuring) {
        during = m_maxDuring;
    }
    ((TransitionClip*)clip)->skyTransition()->setDuration(during);
    emit clip->ofParamChanged(clip, "Clip:TransitionDuring",during);
}

void TransitionComponent::setDuring(double preDuring, double during) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->clipType() != TransitionClipType) {
        qDebug() << "setDuring CLIP is NULL??";
        return;
    }
    if (during < m_minDuring) {
        during = m_minDuring;
    } else if (during > m_maxDuring) {
        during = m_maxDuring;
    }
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    TransitionDuringChangedCommand *command =
        new TransitionDuringChangedCommand(timeline->timelineModel(), index, during, preDuring);
    timeline->addUndoCommand(command);
}

double TransitionComponent::getMaxDuring() {
    return m_maxDuring;
}

double TransitionComponent::getMinDuring() {
    return m_minDuring;
}

void TransitionComponent::onOfParamsChanged(QString key, SkyVariant value){
    if(key == "Clip:TransitionDuring"){
        emit duringChanged();
    }
}
