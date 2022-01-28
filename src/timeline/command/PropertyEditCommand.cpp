#include "PropertyEditCommand.h"

#include <base/utils/SkyVariantUtil.h>
#include <timeline/models/videoclip.h>
#include "project/SEProject.h"

#include <QDebug>

OfParamEditCommand::OfParamEditCommand(TimelineModel *timelineModel,
                                       QModelIndex &index,
                                       QString &effectPath,
                                       QMap<QString, SkyVariant> map,
                                       QMap<QString, SkyVariant> preMap)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_effectPath(effectPath),
      m_updateParams(map),
      m_preParams(preMap) {
    setText(QString("OfParamEditCommand: "));
}

void OfParamEditCommand::undo() {
    QMap<QString, SkyVariant> map;
    updateOfParamValues(m_preParams, map);
    removeKeyFrames();
}

void OfParamEditCommand::redo() {
    m_preParams = updateOfParamValues(m_updateParams, m_preParams);
    tryAddKeyFrames(m_updateParams);
}

QMap<QString, SkyVariant> OfParamEditCommand::updateOfParamValues(QMap<QString, SkyVariant> &paramMap,
                                                                  QMap<QString, SkyVariant> &historyParams) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        return QMap<QString, SkyVariant>();
    }
    qDebug() << "OfParamEditCommand redo Clip:" << clip;
    auto *wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        qDebug() << "updateOfParamValue Failed! skyEffect is NULL Clip:";
        return QMap<QString, SkyVariant>();
    }
    historyParams = wrapper->updateOfParamValues(paramMap, historyParams);

    return historyParams;
}

void OfParamEditCommand::tryAddKeyFrames(QMap<QString, SkyVariant> &paramMap) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        return;
    }
    qDebug() << "OfParamEditCommand tryAddKeyFrames :" << clip;
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        qDebug() << "tryAddKeyFrames Failed! skyEffect is NULL Clip:";
        return;
    }

    auto curTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    auto clipStartTime = clip->startTime();
    auto clipEndTime = clip->endTime();
    if (curTime < clipStartTime || curTime > clipEndTime) {
        return;
    }

    m_curClipTime = curTime;

    for (QString key : paramMap.keys()) {
        if (wrapper->tryAddKeyFrame(key, curTime, paramMap[key])) {
            m_hadAddedKeyFrameKeys.append(key);
            emit clip->ofParamChanged(clip, "Clip:KeyFrame", key.toUtf8().data());
        }
    }
    qDebug() << "OfParamEditCommand tryAddKeyFrames :" << clip << " Keys:" << paramMap.keys();
}

void OfParamEditCommand::removeKeyFrames() {
    if (m_curClipTime < 0 || m_hadAddedKeyFrameKeys.isEmpty()) {
        return;
    }
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        return;
    }
    qDebug() << "OfParamEditCommand tryAddKeyFrames :" << clip;
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        qDebug() << "tryAddKeyFrames Failed! skyEffect is NULL Clip:";
        return;
    }

    for (QString key : m_hadAddedKeyFrameKeys) {
        wrapper->removeKeyFrame(key, m_curClipTime);
        emit clip->ofParamChanged(clip, "Clip:KeyFrame", key.toUtf8().data());
    }
}

AddOfKeyFrameCommand::AddOfKeyFrameCommand(TimelineModel *timelineModel,
                                           QModelIndex &index,
                                           QString &effectPath,
                                           QList<QString> keys,
                                           QList<double> values)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_effectPath(effectPath) {
    m_ofKeys.append(keys);
    m_curOfValues.append(values);
    setText(QString("AddOfKeyFrameCommand: Keys"));
}

void AddOfKeyFrameCommand::undo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return;
    }
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        setObsolete(true);
        return;
    }

    for (auto key : m_ofKeys) {
        qDebug() << "removeKeyFrame key " << key << " Time:" << m_curClipTime;
        if (wrapper->isCurKeyFrame(key, m_curClipTime)) {
            wrapper->removeKeyFrame(key, m_curClipTime);
            emit clip->ofParamChanged(clip, "Clip:KeyFrame", key.toUtf8().data());
        }
    }
}

void AddOfKeyFrameCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return;
    }
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        setObsolete(true);
        return;
    }
    auto curTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    auto clipStartTime = clip->startTime();
    auto clipEndTime = clip->endTime();
    if (curTime < clipStartTime || curTime > clipEndTime) {
        qDebug() << "enableKeyFrame Failed Time: " << curTime << " Clip:" << clip->toString();
        setObsolete(true);
        SEProject::current()->dom()->propertyEditController()->showToast(
            QObject::tr("Current time must in current Clip!"));
        return;
    }

    m_curClipTime = curTime;
    for (int i = 0; i < m_ofKeys.size(); ++i) {
        auto key = m_ofKeys[i];
        qDebug() << "enableKeyFrame key " << key << " Time:" << curTime;
        auto curValue = wrapper->getOfParamValue(key);
        //如果OF参数没有数值 将影响进行关键帧操作 所以设置外部传入的默认
        if (curValue.isNull()) {
            QMap<QString, SkyVariant> updates;
            updates.insert(key, m_curOfValues[i]);
            QMap<QString, SkyVariant> history;
            wrapper->updateOfParamValues(updates, history);
        }
        if (!wrapper->isCurKeyFrame(key, curTime)) {
            wrapper->enableKeyFrame(key, curTime);
            emit clip->ofParamChanged(clip, "Clip:KeyFrame", key.toUtf8().data());
        }
    }
}

RemoveOfKeyFrameCommand::RemoveOfKeyFrameCommand(TimelineModel *timelineModel,
                                                 QModelIndex &index,
                                                 QString &effectPath,
                                                 QList<QString> keys)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_effectPath(effectPath) {
    m_ofKeys.append(keys);
    setText(QString("RemoveOfKeyFrameCommand: Keys"));
}

void RemoveOfKeyFrameCommand::undo() {

    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return;
    }
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        setObsolete(true);
        return;
    }

    for (auto key : m_ofKeys) {
        qDebug() << "enableKeyFrame key " << key << " Time:" << m_curClipTime;
        if (!wrapper->isCurKeyFrame(key, m_curClipTime)) {
            wrapper->enableKeyFrame(key, m_curClipTime);
            emit clip->ofParamChanged(clip, "Clip:KeyFrame", key.toUtf8().data());
        }
    }
}

void RemoveOfKeyFrameCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return;
    }
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        setObsolete(true);
        return;
    }

    auto curTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    auto clipStartTime = clip->startTime();
    auto clipEndTime = clip->endTime();
    if (curTime < clipStartTime || curTime > clipEndTime) {
        qDebug() << "enableKeyFrame Failed Time: " << curTime << " Clip:" << clip->toString();
        setObsolete(true);
        SEProject::current()->dom()->propertyEditController()->showToast(
            QObject::tr("Current time must in current Clip!"));
        return;
    }

    setObsolete(true);
    for (auto key : m_ofKeys) {
        qDebug() << "removeKeyFrame key " << key << " Time:" << time;
        if (wrapper->isCurKeyFrame(key, curTime) && wrapper->removeKeyFrame(key, curTime)) {
            m_curClipTime = curTime;
            setObsolete(false);
            emit clip->ofParamChanged(clip, "Clip:KeyFrame", key.toUtf8().data());
        } else {
        }
    }
}

ClipVolumeChangedCommand::ClipVolumeChangedCommand(TimelineModel *timelineModel,
                                                   QModelIndex &index,
                                                   double preVolume,
                                                   double volume)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      mVolume(volume),
      m_preVolume(preVolume) {
    setText(QString("ClipVolumeChangedCommand: (%1,%2) setVolume").arg(index.internalId()).arg(index.row()));
}

void ClipVolumeChangedCommand::redo() {
    m_preVolume = setVolume(m_preVolume, mVolume);
    qDebug() << "ClipVolumeChangedCommand redo: Volume:" << m_preVolume << " ->" << mVolume;
}

double ClipVolumeChangedCommand::setVolume(double preVolume, double volume) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        return volume;
    }
    if (preVolume < 0) {
        preVolume = clip->skyClip()->getVolume();
    }
    clip->skyClip()->setVolume(volume);

    m_timelineModel->notifyDataChanged(m_modelIndex.internalId(), m_modelIndex.row(), m_modelIndex.row());

    emit clip->ofParamChanged(clip, "Clip:Volume", volume);
    return preVolume;
}

void ClipVolumeChangedCommand::undo() {
    double volume = setVolume(mVolume, m_preVolume);
    qDebug() << "ClipVolumeChangedCommand undo: Volume:" << volume << " Result:" << (volume == mVolume);
}

TransitionDuringChangedCommand::TransitionDuringChangedCommand(TimelineModel *timelineModel,
                                                               QModelIndex &index,
                                                               double during,
                                                               double preDuring)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_during(during),
      m_preDuring(preDuring) {
    setText(QString("TransitionDuringChangedCommand: (%1,%2) setDuring").arg(index.internalId()).arg(index.row()));
}

void TransitionDuringChangedCommand::redo() {
    setDuring(m_during, m_preDuring);
    qDebug() << "TransitionDuringChangedCommand redo: During:" << m_preDuring << " ->" << m_during;
}

double TransitionDuringChangedCommand::setDuring(double during, double preDuring) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr || !(clip->clipType() == VideoClipType || clip->clipType() == ImageClipType)) {
        qDebug() << "TransitionDuringChangedCommand : Select NULL?" << clip;
        return during;
    }
    VideoClip *videoClip = (VideoClip *)clip;
    TransitionClip *transitionClip = videoClip->getTransition();
    if (transitionClip == nullptr || transitionClip->skyTransition() == nullptr) {
        qDebug() << "TransitionDuringChangedCommand transitionClip: NULL??";
        return during;
    }
    transitionClip->skyTransition()->setDuration(during);
    emit transitionClip->ofParamChanged(transitionClip, "Clip:TransitionDuring", during);

    m_timelineModel->notifyDataChanged(m_modelIndex.internalId(), m_modelIndex.row() - 1, m_modelIndex.row());
    qDebug() << "TransitionDuringChangedCommand transitionClip: " << transitionClip;
    return preDuring;
}

void TransitionDuringChangedCommand::undo() {
    double during = setDuring(m_preDuring, m_during);
    qDebug() << "TransitionDuringChangedCommand undo: During:" << during << " Result:" << (during == m_during);
}

ClipSpeedChangedCommand::ClipSpeedChangedCommand(TimelineModel *timelineModel,
                                                 QModelIndex &index,
                                                 double preVolume,
                                                 double volume)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      mSpeed(volume),
      m_preSpeed(preVolume) {
    setText(QString("ClipVolumeChangedCommand: (%1,%2) setVolume").arg(index.internalId()).arg(index.row()));
}

void ClipSpeedChangedCommand::redo() {
    m_preSpeed = setSpeed(m_preSpeed, mSpeed);
    qDebug() << "ClipVolumeChangedCommand redo: Volume:" << m_preSpeed << " ->" << mSpeed;
}

double ClipSpeedChangedCommand::setSpeed(double preSpeed, double speed) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        return speed;
    }
    if (preSpeed < 0) {
        preSpeed = clip->skyClip()->getSpeed();
    }
    clip->setSpeed(speed);

    m_timelineModel->notifyDataChanged(m_modelIndex.internalId(), m_modelIndex.row(), -1);

    return preSpeed;
}

void ClipSpeedChangedCommand::undo() {
    double speed = setSpeed(mSpeed, m_preSpeed);
    qDebug() << "ClipVolumeChangedCommand undo: Volume:" << speed << " Result:" << (speed == mSpeed);
}

ClipCurvedSpeedChangedCommand::ClipCurvedSpeedChangedCommand(TimelineModel *timelineModel,
                                                             QModelIndex &index,
                                                             QString selectedName,
                                                             QList<QPointF> curList)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_curList(curList),
      m_curSelectedName(selectedName) {
    setText(QString("ClipCurvedSpeedChangedCommand: (%1,%2) setSpeed").arg(index.internalId()).arg(index.row()));
}

void ClipCurvedSpeedChangedCommand::redo() {
    m_preList = setSpeed(m_curList, m_curSelectedName, m_preSelectedName);
    qDebug() << "ClipCurvedSpeedChangedCommand redo: Volume:" << m_preList << " ->" << m_curList;
}

QList<QPointF> ClipCurvedSpeedChangedCommand::setSpeed(QList<QPointF> curList, QString curName, QString &preName) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return curList;
    }

    preName = clip->getSelectCurvedSpeedName();
    clip->setSelectCurvedSpeedName(curName);
    emit clip->ofParamChanged(clip, "Clip:CurvedSpeedName", curName.toUtf8().data());

    auto preList = clip->getCurvedPoints();
    clip->setCurvedPointsList(curList);

    m_timelineModel->notifyDataChanged(m_modelIndex.internalId(), m_modelIndex.row(), -1);
    return preList;
}

void ClipCurvedSpeedChangedCommand::undo() {
    m_curList = setSpeed(m_preList, m_preSelectedName, m_curSelectedName);
    qDebug() << "ClipCurvedSpeedChangedCommand undo: Speed:" << m_preList << " ->" << m_curList;
}

UpdateKeyFrameTweenCommand::UpdateKeyFrameTweenCommand(TimelineModel *timelineModel,
                                                       QModelIndex &index,
                                                       QString &effectPath,
                                                       QList<QString> &keys,
                                                       QString &tweenName)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_effectPath(effectPath),
      m_curTweenName(tweenName),
      m_curKeys(keys) {
    setText(QString("UpdateKeyFrameTweenCommand: Keys"));
}

void UpdateKeyFrameTweenCommand::undo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return;
    }
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        setObsolete(true);
        return;
    }
    for (int i = 0; i < m_curKeys.size(); ++i) {
        auto key = m_curKeys.at(i);
        auto preTweenName = m_preTweenNames.at(i);
        wrapper->updateKeyFrameTweenType(key, preTweenName, m_curClipTime);
    }
}

void UpdateKeyFrameTweenCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return;
    }
    auto wrapper = clip->findEffect(m_effectPath);
    if (wrapper == nullptr) {
        setObsolete(true);
        return;
    }
    auto curTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    auto clipStartTime = clip->startTime();
    auto clipEndTime = clip->endTime();
    if (curTime < clipStartTime || curTime > clipEndTime) {
        qDebug() << "enableKeyFrame Failed Time: " << curTime << " Clip:" << clip->toString();
        setObsolete(true);
        SEProject::current()->dom()->propertyEditController()->showToast(
            QObject::tr("Current time must in current Clip!"));
        return;
    }

    m_preTweenNames.clear();
    for (int i = 0; i < m_curKeys.size(); ++i) {
        auto key = m_curKeys.at(i);
        auto preTweenName = wrapper->updateKeyFrameTweenType(key, m_curTweenName, curTime);
        if (preTweenName.isEmpty()) {
            setObsolete(true);
            return;
        }
        m_preTweenNames.append(preTweenName);
    }
    m_curClipTime = curTime;
}

ClipVolumeFadeChangedCommand::ClipVolumeFadeChangedCommand(TimelineModel *timelineModel,
                                                           QModelIndex &index,
                                                           QString type,
                                                           double preDuring,
                                                           double during)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_fadeType(type),
      m_during(during),
      m_preDuring(preDuring) {
    setText(QString("ClipVolumeFadeChangedCommand: (%1,%2)").arg(index.internalId()).arg(index.row()));
}

void ClipVolumeFadeChangedCommand::redo() {
    m_preDuring = setVolumeFadeDuring(m_preDuring, m_during);
    qDebug() << "ClipVolumeChangedCommand redo: Volume:" << m_preDuring << " ->" << m_during;
}

double ClipVolumeFadeChangedCommand::setVolumeFadeDuring(double preDuring, double during) {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        return during;
    }

    if (m_fadeType.compare("FadeIn", Qt::CaseInsensitive) == 0) {
        if (preDuring < 0) {
            preDuring = clip->getVolumeFadeInDuring();
        }
        clip->setVolumeFadeInDuring(during);
    } else {
        if (preDuring < 0) {
            preDuring = clip->getVolumeFadeOutDuring();
        }
        clip->setVolumeFadeOutDuring(during);
    }
    return preDuring;
}

void ClipVolumeFadeChangedCommand::undo() {
    double during = setVolumeFadeDuring(m_during, m_preDuring);
    qDebug() << "ClipVolumeFadeChangedCommand undo: during:" << during << " Result:" << (during == m_during);
}

ClipUpdateResourceCommand::ClipUpdateResourceCommand(TimelineModel *timelineModel,
                                                     const QModelIndex &index,
                                                     const QString &resPath,
                                                     double startTime,
                                                     double endTime,
                                                     bool isReuse)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_resoucePath(resPath),
      m_startTime(startTime),
      m_endTime(endTime),
      m_isReuse(isReuse) {
    setText(QString("ClipUpdateResourceCommand: (%1,%2)").arg(index.internalId()).arg(index.row()));
}

void ClipUpdateResourceCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip) {
        m_preResourcePath = clip->resourceBean().entryPath();
        auto skyClip = clip->skyClip();
        auto range = skyClip->getTrimRange();
        m_preStartTime = range.begin;
        m_preEndTime = range.end;
        m_config = skyClip->saveConfig();
        updateResource(m_resoucePath, m_startTime, m_endTime, m_isReuse, SkyVariant::makeNull());
    }
    qDebug() << "ClipUpdateResourceCommand redo: Resource:" << m_preResourcePath << " ->" << m_resoucePath;
}

void ClipUpdateResourceCommand::updateResource(const QString &resourcePath,
                                               double startTime,
                                               double endTime,
                                               bool isReuse,
                                               const SkyVariant &config) {
    int trackIndex = m_modelIndex.internalId();
    int clipIndex = m_modelIndex.row();
    m_timelineModel->updateClipResource(trackIndex, clipIndex, resourcePath, startTime, endTime, isReuse, config);
    SEProject::current()->dom()->timelineController()->setSelection(trackIndex, clipIndex);
}

void ClipUpdateResourceCommand::undo() {
    updateResource(m_preResourcePath, m_preStartTime, m_preEndTime, false, m_config);
    qDebug() << "ClipUpdateResourceCommand undo: Resource:" << m_resoucePath << " -> " << m_preResourcePath;
}
