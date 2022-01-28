#include "VideoComponent.h"
#include "timeline/models/videoclip.h"
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <timeline/controller/timelinecontroller.h>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <base/utils/SkyVariantUtil.h>

VideoComponent::VideoComponent(QObject *parent) : BaseComponent(parent) {
}

VideoComponent::~VideoComponent() {
}

void VideoComponent::onBindQml(QJsonObject &paramInfo) {
    QMap<QString, SkyVariant> defaultValues;
    defaultValues.insert("0:Brightness", 0);
    defaultValues.insert("0:Contrast", 0);
    defaultValues.insert("0:Saturation", 0);
    defaultValues.insert("0:Sharpen", 0);
    defaultValues.insert("0:Highlight", 0);
    defaultValues.insert("0:Shadow", 0);
    defaultValues.insert("0:Fade", 0);
    defaultValues.insert("0:Temperature", 0);
    defaultValues.insert("0:Tone", 0);

    defaultValues.insert("0:Opacity", 100);
    defaultValues.insert("0:Beauty", 0);
    defaultValues.insert("0:ThinFace", 0);
    defaultValues.insert("0:BlendMode", 0);

//    setOfParamsDefaultValues(defaultValues);

}

void VideoComponent::onUnBind() {
}

bool VideoComponent::isVideoClip() {
    if (curClip() == nullptr) {
        qDebug() << "chengj: curClip() is null";
        return true;
    }
    return curClip()->resourceBean().resouceType() == VideoResource;
}

void VideoComponent::setVolume(double volume) {
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
}

void VideoComponent::setVolumeByUser(double preVolume, double volume) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "setVolume CLIP is NULL??";
        return;
    }

    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    ClipVolumeChangedCommand *command =
        new ClipVolumeChangedCommand(timeline->timelineModel(), index, preVolume, volume);
    timeline->addUndoCommand(command);
}

double VideoComponent::getVolume() {
    if (curClip() == nullptr) {
        qDebug() << "setIntParam failed: curClip is null, this = " << this;
        return 1.0;
    }
    return curClip()->skyClip()->getVolume();
}

void VideoComponent::resetVideoAdjustmentParams() {
    QMap<QString, SkyVariant> defaultValues;
    defaultValues.insert("0:Brightness", 0);
    defaultValues.insert("0:Contrast", 0);
    defaultValues.insert("0:Saturation", 0);
    defaultValues.insert("0:Sharpen", 0);
    defaultValues.insert("0:Highlight", 0);
    defaultValues.insert("0:Shadow", 0);
    defaultValues.insert("0:Fade", 0);
    defaultValues.insert("0:Temperature", 0);
    defaultValues.insert("0:Tone", 0);
    QMap<QString, SkyVariant> preParams;
    QMap<QString, SkyVariant> params;
    for (QString key : defaultValues.keys()) {
        auto cur = defaultValues[key];
        auto pre = getOfParamValue(key);
        if (cur != pre) {
            preParams.insert(key, pre);
            params.insert(key, cur);
        }
    }
    updateOfParamValuesByUser(params, preParams);
}

void VideoComponent::resetVideoPictureParams() {
    QMap<QString, SkyVariant> defaultValues;
    defaultValues.insert("0:Opacity", 100);
    defaultValues.insert("0:Beauty", 0);
    defaultValues.insert("0:ThinFace", 0);
    defaultValues.insert("0:BlendMode", 0);
    defaultValues.insert("0:Scale", 1.0);
    defaultValues.insert("0:Rotate", 0);
    defaultValues.insert("0:TransX", 0);
    defaultValues.insert("0:TransY", 0);
    QMap<QString, SkyVariant> preParams;
    QMap<QString, SkyVariant> params;
    for (QString key : defaultValues.keys()) {
        auto cur = defaultValues[key];
        auto pre = getOfParamValue(key);
        if (cur != pre) {
            preParams.insert(key, pre);
            params.insert(key, cur);
        }
    }
    updateOfParamValuesByUser(params, preParams);
}
void VideoComponent::onOfParamsChanged(QString key, SkyVariant value) {

}
