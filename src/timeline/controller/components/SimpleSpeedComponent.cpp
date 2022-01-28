#include "SimpleSpeedComponent.h"

#include <QJsonArray>
#include <QDebug>

#include <timeline/controller/timelinecontroller.h>

#include <project/SEProject.h>

#include <timeline/command/PropertyEditCommand.h>
#include "base/utils/AppConstance.h"

SimpleSpeedComponent::SimpleSpeedComponent(QObject *parent) : BaseComponent(parent) {
}

void SimpleSpeedComponent::onBindQml(QJsonObject &paramInfo) {
    emit simpleSpeedChanged(getSpeed());
}

void SimpleSpeedComponent::onUnBind() {
}

void SimpleSpeedComponent::setSpeed(double speed) {
    auto clip = curClip();
    if (clip == nullptr) {
        return;
    }
    if (abs(speed - getSpeed()) < FLOAT_DEVIATION) {
        return;
    }

    clip->setSpeed(speed);
    SEProject::current()->dom()->timelineController()->timelineModel()->notifyFromClip(clip);
    qDebug() << "setClipCurvedSpeed: Speed: " << clip->getSpeed()
             << " Timeline During:" << clip->skyClip()->getTimeline()->getDuration();
    clip->toString();
}

double SimpleSpeedComponent::getSpeed() {
    auto clip = curClip();
    if (clip == nullptr) {
        return 1;
    }
    return clip->getSpeed();
}

void SimpleSpeedComponent::setSpeedByUser(double preSpeed, double speed) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        qDebug() << "setVolume CLIP is NULL??";
        return;
    }
    if (preSpeed == speed) {
        return;
    }

    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    ClipSpeedChangedCommand *speedCommand = new ClipSpeedChangedCommand(timeline->timelineModel(), index, preSpeed, speed);
    ClipCurvedSpeedChangedCommand *curvedSpeedCmd =
        new ClipCurvedSpeedChangedCommand(timeline->timelineModel(), index, "", QList<QPointF>());
    QList<BaseSkyUndoCommand*> list;
    list.append(speedCommand);
    list.append(curvedSpeedCmd);
    GroupSkyUndoCommand* command = new GroupSkyUndoCommand(timeline->timelineModel(),list);
    timeline->addUndoCommand(command);
}

void SimpleSpeedComponent::onOfParamsChanged(QString key, SkyVariant value) {
    qDebug() << "SimpleSpeedComponent onOfParamsChanged " << key << " Value:" << value.toBool() << " " << getSpeed();
    if (key.compare("Clip:CurvedSpeed", Qt::CaseInsensitive) == 0 && value == true) {
        emit simpleSpeedChanged(getSpeed());
    } else if (key.compare("Clip:Speed") == 0) {
        emit simpleSpeedChanged(getSpeed());
    }
}
