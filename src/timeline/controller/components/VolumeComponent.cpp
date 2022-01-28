#include "VolumeComponent.h"

#include <QJsonArray>
#include <QDebug>

#include <timeline/controller/timelinecontroller.h>

#include <project/SEProject.h>

#include <timeline/command/PropertyEditCommand.h>

VolumeComponent::VolumeComponent(QObject *parent) : BaseComponent(parent) {
}

void VolumeComponent::onBindQml(QJsonObject &paramInfo) {
    m_maxVolume = paramInfo.value("maxValue").toDouble(2);
    m_minVolume = paramInfo.value("minValue").toDouble(0);
}

void VolumeComponent::onUnBind() {
}

double VolumeComponent::getVolume() {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "getVolume CLIP is NULL??";
        return 0;
    }

    double volume = clip->skyClip()->getVolume();
    qDebug() << "getVolume " << volume;
    return volume;
}

void VolumeComponent::setVolume(double volume) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "setVolume CLIP is NULL??";
        return;
    }
    if (clip->skyClip()->getVolume() == volume) {
        qDebug() << "setVolume Failed  Volume is Same " << volume;
        return;
    }

    clip->skyClip()->setVolume(volume);
    if (clip->clipType() == AudioClipType) {
        SEProject::current()->dom()->timelineController()->timelineModel()->notifyFromClip(clip);
    }
}
void VolumeComponent::setVolumeByUser(double preVolume, double volume) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "setVolume CLIP is NULL??";
        return;
    }
    if (preVolume == volume) {
        return;
    }

    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    ClipVolumeChangedCommand *command =
        new ClipVolumeChangedCommand(timeline->timelineModel(), index, preVolume, volume);
    timeline->addUndoCommand(command);
}

double VolumeComponent::getMaxVolume() {
    return m_maxVolume;
}

double VolumeComponent::getMinVolume() {
    return m_minVolume;
}
