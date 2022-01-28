#ifndef BASESKYUNDOCOMMAND_H
#define BASESKYUNDOCOMMAND_H

#include <QUndoStack>
#include <QUndoCommand>

#include <timeline/models/timelinemodel.h>

enum SkyCmdId {
    AddResourceCmd,
    RemoveClipCmd,
    MoveClipCmd,
    CropClipCmd,
    SplitClipCmd,
    OfParamEditCmd,
    ClipVolumeChangedCmd,
    TransitionDuringChangedCmd,
    RewindVideoCmd,
    CopyClipCmd,
    CopyClipPictureCmd,
    ClipSpeedChangedCmd,
    ClipCurvedSpeedChangedCmd,
    AddSubEffectCmd,
    RemoveSubEffectCmd,
    AddOfKeyFrameCmd,
    RemoveOfKeyFrameCmd,
    GroupCmd,
    UpdateKeyFrameTweenCmd,
    MoveSubEffectIndexCmd,
    FadeVolumeChangedCmd,
    CropVideoCmd,
    ClipUpdateResourceCmd
};

class BaseSkyUndoCommand : public QUndoCommand {
public:
    BaseSkyUndoCommand(TimelineModel *timelineModel);

protected:
    TimelineModel *m_timelineModel;

private:
};

class AddResourceCommand : public BaseSkyUndoCommand {
public:
    AddResourceCommand(TimelineModel *timelineModel,
                       const SkyResourceBean& resBean,
                       int trackIndex,
                       const SkyTimeRange& timeRange,
                       double startTime,
                       QModelIndex modelIndex,
                       const SkyVariant& config = SkyVariant::makeNull());

    void undo() override;

    void redo() override;

    int id() const override {
        return AddResourceCmd;
    };

private:
    int m_trackIndex;
    SkyTimeRange m_timeRange;
    double m_startTime;
    bool m_newTrack = false;
    SkyResourceBean m_resBean;
    SkyResourceBean m_preResBean;
    QModelIndex m_modelIndex;
    SkyVariant m_config;
};

class RemoveClipCommand : public BaseSkyUndoCommand {
public:
    RemoveClipCommand(TimelineModel *timelineModel, QModelIndex index, bool isTransition);

    void undo() override;

    void redo() override;

    int id() const override {
        return RemoveClipCmd;
    };

private:
    SkyTimeRange m_cropTimeRange;
    SkyTimeRange m_trackRange;
    SkyResourceBean m_resBean;
    QModelIndex m_modelIndex;
    bool m_isTransition;
    SkyVariant m_bakClipConfig;
};

class MoveClipCommand : public BaseSkyUndoCommand {
public:
    MoveClipCommand(TimelineModel *timelineModel, QModelIndex index, double preStartTime, double startTime, int trackIndex, bool newTrack);

    void undo() override;

    void redo() override;

    int id() const override {
        return MoveClipCmd;
    };

    QModelIndex getMovePos();
    double getPreStartTime();

private:
    double m_preStartTime;
    double m_startTime;
    int m_trackIndex;
    bool m_newTrack = false;
    QModelIndex m_preModelIndex;
    QModelIndex m_modelIndex;
};

class CropClipCommand : public BaseSkyUndoCommand {
public:
    CropClipCommand(TimelineModel *timelineModel, QModelIndex index, SkyTimeRange timeRange);

    void undo() override;

    void redo() override;

    int id() const override {
        return CropClipCmd;
    };

private:
    SkyTimeRange m_preClipTimeRange;
    SkyTimeRange m_targetClipTimeRange;
    QModelIndex m_modelIndex;
};

class SplitClipCommand : public BaseSkyUndoCommand {
public:
    SplitClipCommand(TimelineModel *timelineModel, int trackIndex, int clipIndex, double timestamp);

    void undo() override;

    void redo() override;

    int id() const override {
        return SplitClipCmd;
    };

private:
    int m_trackIndex;
    int m_clipIndex;
    double m_timestamp;
    QModelIndex m_modelIndex;
};

class CopyClipCommand : public BaseSkyUndoCommand {
public:
    CopyClipCommand(TimelineModel *timelineModel, BaseClip *clip);

    void undo() override;

    void redo() override;

    int id() const override {
        return CopyClipCmd;
    };

private:
    BaseClip *m_clip;
    QModelIndex m_modelIndex;
};

class CopyClipImageCommand : public BaseSkyUndoCommand {
public:
    CopyClipImageCommand(TimelineModel *timelineModel, BaseClip *clip, QString image);

    void undo() override;

    void redo() override;

    int id() const override {
        return CopyClipPictureCmd;
    };

private:
    BaseClip *m_clip;
    QString m_image;
    QModelIndex m_modelIndex;
};

class AddSubEffectCommand : public BaseSkyUndoCommand {
public:
    AddSubEffectCommand(TimelineModel *timelineModel, SkyResourceBean resBean, QModelIndex index);

    void undo() override;

    void redo() override;

    QPair<SkyResourceBean, SkyVariant> addEffect(QPair<SkyResourceBean, SkyVariant> effect);

    int id() const override {
        return AddSubEffectCmd;
    };

private:
    SkyResourceBean m_resBean;
    SkyResourceBean m_preResBean;
    SkyVariant m_preParams;
    QModelIndex m_modelIndex;
};

class RemoveSubEffectCommand : public BaseSkyUndoCommand {
public:
    RemoveSubEffectCommand(TimelineModel *timelineModel, SkyResourceBean resBean, QModelIndex index);

    void undo() override;

    void redo() override;

    int id() const override {
        return RemoveSubEffectCmd;
    };

private:
    SkyResourceBean m_resBean;
    SkyVariant m_preParams;
    QModelIndex m_modelIndex;
};

class GroupSkyUndoCommand:public BaseSkyUndoCommand{
public:
    GroupSkyUndoCommand(TimelineModel *timelineModel, QList<BaseSkyUndoCommand*> cmds);

    ~GroupSkyUndoCommand();

    void undo() override;

    void redo() override;

    int id() const override {
        return GroupCmd;
    };
private:
    QList<BaseSkyUndoCommand*> m_cmds;
};


class MoveSubEffectIndexCommand : public BaseSkyUndoCommand {
public:
    MoveSubEffectIndexCommand(TimelineModel *timelineModel, QModelIndex index, int fromEffectIndex,int toIndex);

    void undo() override;

    void redo() override;

    int id() const override {
        return MoveSubEffectIndexCmd;
    };

private:
    QModelIndex m_modelIndex;
    int m_fromIndex;
    int m_toIndex;
};

class CropVideoCommand : public BaseSkyUndoCommand {
public:
    CropVideoCommand(TimelineModel *timelineModel,
                     QModelIndex index,
                     double rotation,
                     int radioIndex,
                     double oriX,
                     double oriY,
                     double x,
                     double y,
                     double w,
                     double h);

    void redo() override;

    void undo() override;

    int id() const override {
        return CropVideoCmd;
    };

private:
    QModelIndex m_index;
    double m_rotation;
    double m_lastRotation;
    int m_radioIndex;
    SkyVariant m_lastRadioIndex;
    double m_oriX;
    SkyVariant m_lastOriX;
    double m_oriY;
    SkyVariant m_lastOriY;
    double m_x;
    double m_lastX;
    double m_y;
    double m_lastY;
    double m_w;
    double m_lastW;
    double m_h;
    double m_lastH;
};

#endif // BASESKYUNDOCOMMAND_H
