#ifndef PROPERTYEDITCOMMAND_H
#define PROPERTYEDITCOMMAND_H

#include "baseskyundocommand.h"

#include <QList>
#include <QMap>
#include <QPointF>
#include <QString>

#include <skymedia/SkyEffect.h>
#include <skymedia/SkyVariant.h>

class OfParamEditCommand : public BaseSkyUndoCommand {
public:
    OfParamEditCommand(TimelineModel *timelineModel,
                       QModelIndex &index,
                       QString &effectPath,
                       QMap<QString, SkyVariant> map,
                       QMap<QString, SkyVariant> preMap);

    void undo() override;

    void redo() override;

    int id() const override {
        return OfParamEditCmd;
    };

    QMap<QString, SkyVariant> updateOfParamValues(QMap<QString, SkyVariant> &paramMap,
                                                  QMap<QString, SkyVariant> &historyParams);

    void tryAddKeyFrames(QMap<QString, SkyVariant> &paramMap);
    void removeKeyFrames();

private:
    QModelIndex m_modelIndex;
    QString m_effectPath;
    QMap<QString, SkyVariant> m_updateParams;
    QMap<QString, SkyVariant> m_preParams;
    QList<QString> m_hadAddedKeyFrameKeys;
    double m_curClipTime = -1;
};

class AddOfKeyFrameCommand : public BaseSkyUndoCommand {
public:
    AddOfKeyFrameCommand(TimelineModel *timelineModel,
                         QModelIndex &index,
                         QString &effectPath,
                         QList<QString> key,
                         QList<double> values);

    void undo() override;

    void redo() override;

    int id() const override {
        return AddOfKeyFrameCmd;
    };

private:
    QModelIndex m_modelIndex;
    QString m_effectPath;
    QList<QString> m_ofKeys;
    QList<double> m_curOfValues;
    double m_curClipTime = -1;
};

class RemoveOfKeyFrameCommand : public BaseSkyUndoCommand {
public:
    RemoveOfKeyFrameCommand(TimelineModel *timelineModel, QModelIndex &index, QString &effectPath, QList<QString> keys);

    void undo() override;

    void redo() override;

    int id() const override {
        return RemoveOfKeyFrameCmd;
    };

private:
    QModelIndex m_modelIndex;
    QString m_effectPath;
    QList<QString> m_ofKeys;
    double m_curClipTime = -1;
};

class ClipVolumeChangedCommand : public BaseSkyUndoCommand {
public:
    ClipVolumeChangedCommand(TimelineModel *timelineModel, QModelIndex &index, double preVolume, double volume);

    void undo() override;

    void redo() override;

    int id() const override {
        return ClipVolumeChangedCmd;
    };

private:
    double setVolume(double preVolume, double volume);

    QModelIndex m_modelIndex;
    double mVolume;
    double m_preVolume;
};

class ClipSpeedChangedCommand : public BaseSkyUndoCommand {
public:
    ClipSpeedChangedCommand(TimelineModel *timelineModel, QModelIndex &index, double preSpeed, double speed);

    void undo() override;

    void redo() override;

    int id() const override {
        return ClipSpeedChangedCmd;
    };

private:
    double setSpeed(double preSpeed, double speed);

    QModelIndex m_modelIndex;
    double mSpeed;
    double m_preSpeed;
};

class ClipCurvedSpeedChangedCommand : public BaseSkyUndoCommand {
public:
    ClipCurvedSpeedChangedCommand(TimelineModel *timelineModel,
                                  QModelIndex &index,
                                  QString selectedName,
                                  QList<QPointF> curList);

    void undo() override;

    void redo() override;

    int id() const override {
        return ClipCurvedSpeedChangedCmd;
    };

private:
    QList<QPointF> setSpeed(QList<QPointF> curList, QString curName, QString &preName);

    QModelIndex m_modelIndex;
    QList<QPointF> m_preList;
    QList<QPointF> m_curList;
    QString m_preSelectedName;
    QString m_curSelectedName;
};

class TransitionDuringChangedCommand : public BaseSkyUndoCommand {
public:
    TransitionDuringChangedCommand(TimelineModel *timelineModel, QModelIndex &index, double during, double preDuring);

    void undo() override;

    void redo() override;

    int id() const override {
        return TransitionDuringChangedCmd;
    };

private:
    double setDuring(double during, double preDuring);
    QModelIndex m_modelIndex;
    double m_during;
    double m_preDuring;
};

class UpdateKeyFrameTweenCommand : public BaseSkyUndoCommand {
public:
    UpdateKeyFrameTweenCommand(TimelineModel *timelineModel,
                               QModelIndex &index,
                               QString &effectPath,
                               QList<QString> &key,
                               QString &tweenName);

    void undo() override;

    void redo() override;

    int id() const override {
        return UpdateKeyFrameTweenCmd;
    };

private:
    QModelIndex m_modelIndex;
    QString m_effectPath;
    QList<QString> m_preTweenNames;
    QString m_curTweenName;
    QList<QString> m_curKeys;
    double m_curClipTime = -1;
};

class ClipVolumeFadeChangedCommand : public BaseSkyUndoCommand {
public:
    ClipVolumeFadeChangedCommand(TimelineModel *timelineModel,
                                 QModelIndex &index,
                                 QString type,
                                 double preDuring,
                                 double during);

    void undo() override;

    void redo() override;

    int id() const override {
        return FadeVolumeChangedCmd;
    };

private:
    double setVolumeFadeDuring(double preDuring, double during);

    QModelIndex m_modelIndex;
    QString m_fadeType;
    double m_during;
    double m_preDuring;
};

class ClipUpdateResourceCommand : public BaseSkyUndoCommand {
public:
    ClipUpdateResourceCommand(TimelineModel *timelineModel,
                              const QModelIndex &index,
                              const QString &resPath,
                              double startTime,
                              double endTime,
                              bool isReuse);

    void undo() override;

    void redo() override;

    int id() const override {
        return ClipUpdateResourceCmd;
    };

private:
    void updateResource(const QString &resource, double startTime, double endTime, bool isReuse, const SkyVariant &config);

    QModelIndex m_modelIndex;
    QString m_resoucePath;
    QString m_preResourcePath;
    double m_startTime;
    double m_preStartTime;
    double m_endTime;
    double m_preEndTime;
    bool m_isReuse;
    SkyVariant m_config;
};
#endif // PROPERTYEDITCOMMAND_H
