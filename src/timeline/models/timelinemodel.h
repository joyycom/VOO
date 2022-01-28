#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include <skymedia/SkyTimeline.h>

#include <QAbstractItemModel>
#include <QObject>

#include "skyresourcebean.h"
#include "timelinetrack.h"
SKYMEDIA_USING_NAMESPACE

class TimelineModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit TimelineModel(QObject *parent = nullptr);
    ~TimelineModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    QModelIndex makeIndex(int trackIndex, int clipIndex) const;
    BaseClip *data(const QModelIndex &index);
    void addTrack(TimelineTrack *track);
    QString firstVideoTrackResource();

    QModelIndex addResource(TrackType type,
                            SkyResourceBean &resouce,
                            const SkyTimeRange &range,
                            int trackIndex = -1,
                            double timestamp = 0,
                            const SkyVariant &config = SkyVariant::makeNull());

    QModelIndex addResource(TrackType type,
                            SkyResourceBean &resouce,
                            const SkyTimeRange &trimRange,
                            const SkyTimeRange &trackRange,
                            int trackIndex,
                            const SkyVariant &config = SkyVariant::makeNull());

    QPair<SkyResourceBean, QModelIndex> addTransitionResource(SkyResourceBean &resource,
                                                              double timestamp,
                                                              QModelIndex &target);

    SkyResourceBean removeTransitionResource(SkyResourceBean &resource, QModelIndex &target);

    void addEffect(double timestamp);

    void cropClip(const QModelIndex &index, double cropStartTime = -1, double cropEndTime = -1);

    QModelIndex moveClip(const QModelIndex &index, double time, int trackIndex, bool newTrack);

    bool removeClip(const QModelIndex &index, bool deleteTrackWhenEmpty = false);

    QModelIndex splitClip(const int trackIndex, const int clipIndex, const double timestamp);

    bool mergeClip(const QModelIndex &index);

    void seekTo(const double time);

    TimelineTrack *getTrackFromIndex(int index);

    bool removeEmptyTrack(int index);

    void saveInstanceState();
    void restoreInstanceState();
    void notifyDataChanged(int rowIndex, int clipIndexFrom, int clipIndexTo);

    QVariantList getClipEdgeList(int trackIndex, int clipIndex);

    void notifyFromClip(BaseClip *clip);

    bool hasClip();

    bool rewindVideoClip(const QModelIndex &index, const QString path);

    QModelIndex copyClip(BaseClip *clip);

    QModelIndex copyClipImage(BaseClip *clip, QString image);

    void removeBlankTrack();

    bool hasClipInTime(BaseClip *clip, double time);

    bool addSubEffect(const QModelIndex &index,
                      QPair<SkyResourceBean, SkyVariant> &addEffect,
                      QPair<SkyResourceBean, SkyVariant> &deletedEffect);

    bool removeSubEffect(const QModelIndex &index, QPair<SkyResourceBean, SkyVariant> &removedEffect);

    void addInputDevice(BaseClip *clip);

    void updateInputDevice(BaseClip *clip, const InputDevice &inputDevice);

    void removeInputDevice(BaseClip *clip);

    InputDevice getInputDeviceFromClip(BaseClip *clip);

    QMap<BaseClip *, InputDevice> inputDeviceList();

    Q_INVOKABLE void checkTrackError(int trackIndex, int clipCount);

    Q_INVOKABLE void checkClipError(int trackIndex, int clipIndex, double startTime, double duration);

    bool hasVideoClip();

    void updateClipResource(int trackIndex,
                            int clipIndex,
                            const QString &path,
                            double startTime,
                            double endTime,
                            bool isReuse,
                            const SkyVariant &config);

signals:
    void removeClipData(BaseClip *clip);
    void clipDataChanged(BaseClip *clip);
    void watchError(QString error);

private slots:
    void onClipDataChanged(TimelineTrack *track, int clipIndex);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    SkyTimeline *getCurrentTimeline();

    bool tryRemoveTransition(int trackIndex, int clipIndex);
    bool tryRemoveClipTransition(int trackIndex, BaseClip *clip);

    QList<TimelineTrack *> m_trackList;
    QMap<BaseClip *, InputDevice> m_inputDeviceList;
};

#endif // TIMELINEMODEL_H
