#include "baseskyundocommand.h"
#include <QDebug>
#include <QFile>
#include <QModelIndex>
#include <QModelIndex>
#include "../models/videoclip.h"
#include <src/project/SEProject.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <timeline/venus/AsyncTimelineFrameMgr.h>

BaseSkyUndoCommand::BaseSkyUndoCommand(TimelineModel *timelineModel)
    : QUndoCommand(""),
      m_timelineModel(timelineModel) {
    qDebug() << "BaseSkyUndoCommand:" << this;
}

AddResourceCommand::AddResourceCommand(TimelineModel *timelineModel,
                                       const SkyResourceBean& resBean,
                                       int trackIndex,
                                       const SkyTimeRange& timeRange,
                                       double startTime,
                                       QModelIndex modelIndex,
                                       const SkyVariant& config)
    : BaseSkyUndoCommand(timelineModel),
      m_trackIndex(trackIndex),
      m_timeRange(timeRange),
      m_startTime(startTime),
      m_resBean(resBean),
      m_modelIndex(modelIndex),
      m_config(config) {
    setText("AddResourceCommand:" + resBean.name());
    qDebug() << "AddResourceCommand:" << resBean.toString();
}

void AddResourceCommand::redo() {
    if (m_resBean.resouceType() == TransitionResource) {
        QPair<SkyResourceBean, QModelIndex> pair =
            m_timelineModel->addTransitionResource(m_resBean, m_startTime, m_modelIndex);
        m_modelIndex = pair.second;
        m_preResBean = pair.first;
        QModelIndex index = pair.second;
        if (index.isValid()) {
            m_timelineModel->notifyDataChanged(index.internalId(), index.row() - 1, index.row() + 1);
        }
        qDebug() << "[AddResourceCommand] redo pre Transition " << m_preResBean.toString();
    } else {
        m_modelIndex = m_timelineModel->addResource(m_resBean.trackType(), m_resBean, m_timeRange, m_trackIndex, m_startTime, m_config);
        if (m_modelIndex.isValid() && m_timelineModel->rowCount(m_modelIndex.parent()) == 1) {
            m_newTrack = true;
        }
    }

    if (m_modelIndex.isValid()) {
        SEProject::current()->dom()->timelineController()->setSelection(
            m_modelIndex.internalId(), m_modelIndex.row(), m_resBean.resouceType() == TransitionResource);
        qDebug() << "[AddResourceCommand] Redo Add Resouce! During:" << m_timelineModel->data(m_modelIndex)->during()
                 << m_resBean.toString() << "NewTrack?" << m_newTrack;
    } else {
        setObsolete(true);
        qDebug() << "[AddResourceCommand] Redo Add Resouce! Failed!!:";
    }
}

void AddResourceCommand::undo() {
    if (m_modelIndex.isValid() && m_modelIndex.parent().isValid()) {
        if (m_resBean.resouceType() == TransitionResource) {
            if (m_preResBean.isValidResource()) {
                qDebug() << "[AddResourceCommand] undo cur Transition " << m_resBean.toString();
                QPair<SkyResourceBean, QModelIndex> pair =
                    m_timelineModel->addTransitionResource(m_preResBean, m_startTime, m_modelIndex);
                QModelIndex index = pair.second;
                if (index.isValid()) {
                    m_timelineModel->notifyDataChanged(index.internalId(), index.row() - 1, index.row() + 1);
                }
                qDebug() << "[AddResourceCommand] undo recover Transition " << m_preResBean.toString();
            } else {
                SkyResourceBean resource = m_timelineModel->removeTransitionResource(m_resBean, m_modelIndex);
                if (resource.isValidResource()) {
                    m_timelineModel->notifyDataChanged(
                        m_modelIndex.internalId(), m_modelIndex.row() - 1, m_modelIndex.row() + 1);
                }
                qDebug() << "[AddResourceCommand] undo remove Transition " << m_resBean.toString();
            }
        } else {
            m_timelineModel->removeClip(m_modelIndex, m_newTrack);
            qDebug() << "[AddResourceCommand] undo remove Clip! !" << m_newTrack;
        }
        SEProject::current()->dom()->timelineController()->setSelection(-1, -1);
    }
}

RemoveClipCommand::RemoveClipCommand(TimelineModel *timelineModel, QModelIndex index, bool isTransition)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_isTransition(isTransition) {
    setText(QString("RemoveClipCommand:(%1,%2)").arg(index.internalId()).arg(index.row()));
}

void RemoveClipCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip != nullptr) {
        if (!m_isTransition) {
            m_cropTimeRange = clip->skyClip()->getTrimRange();
            m_trackRange = clip->skyClip()->getTrackRange();
            m_resBean = (clip->resourceBean());
            m_bakClipConfig = clip->backupClipConfig();
            m_timelineModel->removeClip(m_modelIndex);
        } else if (clip->clipType() == VideoClipType || clip->clipType() == ImageClipType) {
            //移除转场
            m_trackRange = SkyTimeRange(clip->startTime(),clip->endTime());
            SkyResourceBean resourceBean = clip->resourceBean();
            m_resBean = m_timelineModel->removeTransitionResource(resourceBean, m_modelIndex);
            if (m_resBean.isValidResource()) {
                m_timelineModel->notifyDataChanged(m_modelIndex.internalId(), m_modelIndex.row(), m_modelIndex.row());
            }
        }
        SEProject::current()->dom()->timelineController()->setSelection(-1, -1);
    }
}

void RemoveClipCommand::undo() {
    if (m_resBean.resouceType() == TransitionResource) {
        qDebug() << "[RemoveClipCommand] undo cur Transition " << m_resBean.toString();
        QPair<SkyResourceBean, QModelIndex> pair =
            m_timelineModel->addTransitionResource(m_resBean, m_trackRange.begin, m_modelIndex);
        QModelIndex index = pair.second;
        if (index.isValid()) {
            m_timelineModel->notifyDataChanged(index.internalId(), index.row(), index.row());

            SEProject::current()->dom()->timelineController()->setSelection(index.internalId(), index.row(), true);
        }
        qDebug() << "[RemoveClipCommand] undo recover Transition " << m_resBean.toString();
    } else {
        m_modelIndex = m_timelineModel->addResource(
            m_resBean.trackType(), m_resBean, m_cropTimeRange,m_trackRange, m_modelIndex.internalId(), m_bakClipConfig);
        if (m_modelIndex.isValid()) {
            SEProject::current()->dom()->timelineController()->setSelection(
                m_modelIndex.internalId(), m_modelIndex.row(), false);
        }
    }

    qDebug() << "Add Video Resouce! hasAudioStream:" << m_resBean.toString();
}

MoveClipCommand::MoveClipCommand(TimelineModel *timelineModel,
                                 QModelIndex index,
                                 double preStartTime,
                                 double startTime,
                                 int trackIndex,
                                 bool newTrack)
    : BaseSkyUndoCommand(timelineModel),
      m_preStartTime(preStartTime),
      m_startTime(startTime),
      m_trackIndex(trackIndex),
      m_newTrack(newTrack),
      m_modelIndex(index) {
    setText(QString("MoveClipCommand:(%1,%2)").arg(index.internalId()).arg(index.row()));
    qDebug() << "MoveClipCommand:: : " << m_modelIndex.internalId() << "," << m_modelIndex.row() << " --> "
             << trackIndex << " NewTrack:" << newTrack << " Time:" << startTime;
}

void MoveClipCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip != nullptr) {
        if (m_preStartTime < 0) {
            m_preStartTime = clip->startTime();
            if ((clip->clipType() == VideoClipType || clip->clipType() == ImageClipType) &&
                ((VideoClip*)clip)->getTransition() != nullptr) {
                auto preClipIndex = m_timelineModel->makeIndex(m_modelIndex.internalId(), m_modelIndex.row() - 1);
                auto preClip = m_timelineModel->data(preClipIndex);
                if (preClip != nullptr) {
                    m_preStartTime = preClip->endTime();
                    qDebug() << "MoveClipCommand::redo HadTransition prestartTime use pre Clip End Time! PreClip: "
                             << preClip->toString();
                }
            }
        }

        qDebug() << "MoveClipCommand::redo MoveClipCommand Clip: " << clip->toString();
        m_preModelIndex = m_timelineModel->moveClip(m_modelIndex, m_startTime, m_trackIndex, m_newTrack);
        if (m_preModelIndex.isValid()) {
            SEProject::current()->dom()->timelineController()->setSelection(
                m_preModelIndex.internalId(), m_preModelIndex.row(), false);
        }
        qDebug() << " MoveClipCommand From:" << m_modelIndex.internalId() << "," << m_modelIndex.row() << " -> "
                 << m_preModelIndex.internalId() << "," << m_preModelIndex.row();
        setObsolete(!m_preModelIndex.isValid());
    }
}

void MoveClipCommand::undo() {
    BaseClip *clip = m_timelineModel->data(m_preModelIndex);
    if (clip != nullptr) {
        int targetIndex = m_modelIndex.internalId();
        int curIndex = m_preModelIndex.internalId();
        if (curIndex <= targetIndex && m_newTrack) {
            targetIndex++;
        }
        qDebug() << "MoveClipCommand::undo MoveClipCommand Clip: " << clip->toString();
        QModelIndex modelIndex = m_timelineModel->moveClip(m_preModelIndex, m_preStartTime, targetIndex, false);
        if (m_newTrack && m_timelineModel->rowCount(m_preModelIndex.parent()) == 0) {
            m_timelineModel->removeEmptyTrack(curIndex);
        }
        SEProject::current()->dom()->timelineController()->setSelection(
            m_modelIndex.internalId(), m_modelIndex.row(), false);

        qDebug() << " MoveClipCommand From:" << curIndex << "," << m_preModelIndex.row() << " -> "
                 << modelIndex.internalId() << "," << modelIndex.row() << " TargetIndex:" << m_trackIndex;
    }
}

QModelIndex MoveClipCommand::getMovePos() {
    return m_preModelIndex;
}

double MoveClipCommand::getPreStartTime() {
    return m_preStartTime;
}

CropClipCommand::CropClipCommand(TimelineModel *timelineModel, QModelIndex index, SkyTimeRange range)
    : BaseSkyUndoCommand(timelineModel),
      m_targetClipTimeRange(range),
      m_modelIndex(index) {
    setText(QString("CropClipCommand:(%1,%2)").arg(index.internalId()).arg(index.row()));
}

void CropClipCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip != nullptr) {
        m_preClipTimeRange = clip->skyClip()->getTrackRange();
        m_timelineModel->cropClip(m_modelIndex, m_targetClipTimeRange.begin, m_targetClipTimeRange.end);
        qDebug() << "CropClipCommand::redo Crop Clip: " << clip->toString();
    }
}

void CropClipCommand::undo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip != nullptr) {
        m_timelineModel->cropClip(m_modelIndex, m_preClipTimeRange.begin, m_preClipTimeRange.end);
        qDebug() << "CropClipCommand::undo  Recover Crop Clip: " << clip->toString();
    }
}

SplitClipCommand::SplitClipCommand(TimelineModel *timelineModel, int trackIndex, int clipIndex, double timestamp)
    : BaseSkyUndoCommand(timelineModel),
      m_trackIndex(trackIndex),
      m_clipIndex(clipIndex),
      m_timestamp(timestamp) {
    setText(QString("SplitClipCommand:(%1,%2)").arg(trackIndex).arg(clipIndex));
}

void SplitClipCommand::redo() {
    m_modelIndex = m_timelineModel->splitClip(m_trackIndex, m_clipIndex, m_timestamp);
    setObsolete(!m_modelIndex.isValid());
    if (m_modelIndex.isValid()) {
        auto timelineController = SEProject::current()->dom()->timelineController();
        timelineController->setSelection(m_modelIndex.internalId(), m_modelIndex.row() + 1, false);
        auto curClip = timelineController->curSelectedClip();
        if(curClip != nullptr){
            SEProject::current()->dom()->previewController()->seekVideo(curClip->startTime());
        }
    }
    qDebug() << "CropClipCommand::redo Split Clip Success? " << m_modelIndex.isValid();
}

void SplitClipCommand::undo() {
    bool result = m_timelineModel->mergeClip(m_modelIndex);
    if (result) {
        SEProject::current()->dom()->timelineController()->setSelection(
            m_modelIndex.internalId(), m_modelIndex.row(), false);
    }
    qDebug() << "CropClipCommand::undo Merge Clip Success? " << result;
}

CopyClipCommand::CopyClipCommand(TimelineModel *timelineModel, BaseClip *clip)
    : BaseSkyUndoCommand(timelineModel),
      m_clip(clip) {
}

void CopyClipCommand::redo() {
    m_modelIndex = m_timelineModel->copyClip(m_clip);
    SEProject::current()->dom()->timelineController()->setSelection(m_modelIndex.internalId(), m_modelIndex.row(), false);
}

void CopyClipCommand::undo() {
    m_timelineModel->removeClip(m_modelIndex, true);
    SEProject::current()->dom()->timelineController()->setSelection(-1, -1);
}

CopyClipImageCommand::CopyClipImageCommand(TimelineModel *timelineModel, BaseClip *clip, QString image)
    : BaseSkyUndoCommand(timelineModel),
      m_clip(clip),
      m_image(image) {
}

void CopyClipImageCommand::redo() {
    m_modelIndex = m_timelineModel->copyClipImage(m_clip, m_image);
    SEProject::current()->dom()->timelineController()->setSelection(m_modelIndex.internalId(), m_modelIndex.row(), false);
}

void CopyClipImageCommand::undo() {
    m_timelineModel->removeClip(m_modelIndex, true);
    SEProject::current()->dom()->timelineController()->setSelection(-1, -1);
}

AddSubEffectCommand::AddSubEffectCommand(TimelineModel *timelineModel, SkyResourceBean resBean, QModelIndex index)
    : BaseSkyUndoCommand(timelineModel),
      m_resBean(resBean),
      m_modelIndex(index) {
}

void AddSubEffectCommand::undo() {
    if (m_preResBean.isValidResource()) {
        addEffect(QPair<SkyResourceBean, SkyVariant>(m_preResBean, m_preParams));
    } else {
        auto deleteEffect = QPair<SkyResourceBean, SkyVariant>(m_resBean, SkyVariant::makeNull());
        m_timelineModel->removeSubEffect(m_modelIndex, deleteEffect);
    }
}

void AddSubEffectCommand::redo() {
    auto result = addEffect(QPair<SkyResourceBean, SkyVariant>(m_resBean, SkyVariant::makeNull()));
    m_preParams = result.second;
    m_preResBean = result.first;
}

QPair<SkyResourceBean, SkyVariant> AddSubEffectCommand::addEffect(QPair<SkyResourceBean, SkyVariant> effect) {
    QPair<SkyResourceBean, SkyVariant> deletedEffect;
    auto result = m_timelineModel->addSubEffect(m_modelIndex, effect, deletedEffect);
    setObsolete(!result);
    return deletedEffect;
}

RemoveSubEffectCommand::RemoveSubEffectCommand(TimelineModel *timelineModel, SkyResourceBean resBean, QModelIndex index)
    : BaseSkyUndoCommand(timelineModel),
      m_resBean(resBean),
      m_modelIndex(index) {
}

void RemoveSubEffectCommand::undo() {
    QPair<SkyResourceBean, SkyVariant> deletedEffect;
    QPair<SkyResourceBean, SkyVariant> addEffect(m_resBean, m_preParams);
    auto result = m_timelineModel->addSubEffect(m_modelIndex, addEffect, deletedEffect);
    setObsolete(!result);
}

void RemoveSubEffectCommand::redo() {
    auto deleteEffect = QPair<SkyResourceBean, SkyVariant>(m_resBean, SkyVariant::makeNull());
    auto result = m_timelineModel->removeSubEffect(m_modelIndex, deleteEffect);
    m_preParams = deleteEffect.second;
    setObsolete(!result);
}


GroupSkyUndoCommand::GroupSkyUndoCommand(TimelineModel *timelineModel, QList<BaseSkyUndoCommand*> cmds)
    :BaseSkyUndoCommand(timelineModel),m_cmds(cmds){

}

GroupSkyUndoCommand::~GroupSkyUndoCommand(){
    qDeleteAll(m_cmds);
    m_cmds.clear();
}

void GroupSkyUndoCommand::undo() {
    bool isObsolete = true;
    for(auto cmd:m_cmds){
        cmd->undo();
        if(!cmd->isObsolete()){
            isObsolete = false;
        }
    }
    setObsolete(isObsolete);
}

void GroupSkyUndoCommand::redo() {
    bool isObsolete = true;
    for(auto cmd:m_cmds){
        cmd->redo();
        if(!cmd->isObsolete()){
            isObsolete = false;
        }
    }
    setObsolete(isObsolete);
}

MoveSubEffectIndexCommand::MoveSubEffectIndexCommand(TimelineModel *timelineModel, QModelIndex index, int fromEffectIndex,int toIndex)
    : BaseSkyUndoCommand(timelineModel),
      m_modelIndex(index),
      m_fromIndex(fromEffectIndex),
      m_toIndex(toIndex){
}

void MoveSubEffectIndexCommand::undo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return ;
    }
    clip->moveEffectPos(m_toIndex,m_fromIndex);
    emit clip->ofParamChanged(clip, "Clip:MoveEffectIndex",m_toIndex);
}

void MoveSubEffectIndexCommand::redo() {
    BaseClip *clip = m_timelineModel->data(m_modelIndex);
    if (clip == nullptr) {
        setObsolete(true);
        return ;
    }
    clip->moveEffectPos(m_fromIndex,m_toIndex);
    emit clip->ofParamChanged(clip, "Clip:MoveEffectIndex",m_fromIndex);
}

CropVideoCommand::CropVideoCommand(TimelineModel *timelineModel,
                                   QModelIndex index,
                                   double rotation,
                                   int radioIndex,
                                   double oriX,
                                   double oriY,
                                   double x,
                                   double y,
                                   double w,
                                   double h)
    : BaseSkyUndoCommand(timelineModel),
      m_index(index),
      m_rotation(rotation),
      m_radioIndex(radioIndex),
      m_oriX(oriX),
      m_oriY(oriY),
      m_x(x),
      m_y(y),
      m_w(w),
      m_h(h) {
}

void CropVideoCommand::redo() {
    //备份属性
    BaseClip *clip = m_timelineModel->data(m_index);
    if (clip == nullptr || (clip->clipType() != VideoClipType && clip->clipType() != ImageClipType)) {
        return;
    }
    SkyVideoClip *videoClip = ((SkyVideoClip *)clip->skyClip());
    auto transform = videoClip->getCropTransform();
    m_lastRotation = transform.rotation;
    m_lastRadioIndex = videoClip->getUserData("crop_radio_index");
    m_lastOriX = videoClip->getUserData("crop_x");
    m_lastOriY = videoClip->getUserData("crop_y");
    auto cropRect = videoClip->getCropRect();
    m_lastX = cropRect.x;
    m_lastY = cropRect.y;
    m_lastW = cropRect.width;
    m_lastH = cropRect.height;
    //开始裁剪
    transform.rotation = -m_rotation * M_PI / 180;
    videoClip->setCropTransform(transform);
    videoClip->setCropRect(SkyRect(m_x, m_y, m_w, m_h));
    auto stream = clip->skyClip()->getResource().firstVideoStream();
    if (stream.width > 0 && stream.height > 0) {
        double newWidth = stream.width * m_w;
        double newHeight = stream.height * m_h;
        if (newWidth / newHeight > (double)stream.width / stream.height) {
            videoClip->resize(stream.width * stream.height / stream.height, stream.height, SkyScaleMode_AspectFit);
        } else {
            videoClip->resize(stream.width, stream.width * stream.height / stream.width, SkyScaleMode_AspectFit);
        }
    }
    videoClip->setUserData("crop_radio_index", m_radioIndex);
    videoClip->setUserData("crop_x", m_oriX);
    videoClip->setUserData("crop_y", m_oriY);

    if (clip->hasVenusEffect()) {
        AsyncTimelineFrameMgr::getIns()->updateClipProcessTask(clip);
    }
}

void CropVideoCommand::undo() {
    BaseClip *clip = m_timelineModel->data(m_index);
    if (clip == nullptr || (clip->clipType() != VideoClipType && clip->clipType() != ImageClipType)) {
        return;
    }
    SkyVideoClip *videoClip = ((SkyVideoClip *)clip->skyClip());
    SkyTransform2D transform;
    transform.rotation = m_lastRotation;
    videoClip->setCropTransform(transform);
    videoClip->setCropRect(SkyRect(m_lastX, m_lastY, m_lastW, m_lastH));
    auto stream = clip->skyClip()->getResource().firstVideoStream();
    if (stream.width > 0 && stream.height > 0) {
        double newWidth = stream.width * m_lastW;
        double newHeight = stream.height * m_lastH;
        if (newWidth / newHeight > (double)stream.width / stream.height) {
            videoClip->resize(stream.width * stream.height / stream.height, stream.height, SkyScaleMode_AspectFit);
        } else {
            videoClip->resize(stream.width, stream.width * stream.height / stream.width, SkyScaleMode_AspectFit);
        }
    }
    videoClip->setUserData("crop_radio_index", m_lastRadioIndex);
    videoClip->setUserData("crop_x", m_lastOriX);
    videoClip->setUserData("crop_y", m_lastOriY);

    if (clip->hasVenusEffect()) {
        AsyncTimelineFrameMgr::getIns()->updateClipProcessTask(clip);
    }
}
