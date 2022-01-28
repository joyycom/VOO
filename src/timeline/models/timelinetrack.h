#ifndef TRACKGROUP_H
#define TRACKGROUP_H

#include "baseclip.h"
#include "TimelineDef.h"
#include "skyresourcebean.h"

#include <QObject>

#include <skymedia/SkyTimeline.h>

SKYMEDIA_USING_NAMESPACE

/**
 *
 * @brief
 */
class TimelineTrack : public QObject {
    Q_OBJECT
public:
    explicit TimelineTrack(SkyTimeline *timeline, TrackType type);

    TimelineTrack(SkyTrack *skyTrack, TrackType type);

    ~TimelineTrack();

    void removeBlankTrack();

    int count() const;

    TrackType type() const;

    SkyTrack *skyTrack() const;

    BaseClip *get(int i) const;

    QVariant get(int pos, int role) const;

    bool moveToIndex(int index);

    bool isVideoTrack() const;

    bool isAudioTrack() const;

    bool isEffectTrack() const;

    /**
     * @brief tryAddResource 尝试添加资源， 在timestamp时间点 尝试添加资源，其他资源位置不变
     * @param resource
     * @param timestamp 资源插入时间点，
     * @return 插入位置；-1表示插入失败
     */
    int tryAddResource(SkyResourceBean &resource, double timestamp);

    int tryAddResource(SkyResourceBean &resource, const SkyTimeRange &trimRange, double timestamp);

    int tryAddResource(const SkyResourceBean &resource,
                       const SkyTimeRange &trimRange,
                       const SkyTimeRange &trackRange,
                       const SkyVariant &config);

    void updateClipResource(int pos,
                            const SkyResourceBean &resource,
                            double startTime,
                            double endTime,
                            bool isReuse,
                            const SkyVariant &config);

    void cropClip(int pos, double cropStartTime, double cropEndTime);

    int moveClip(int pos, double timestamp);

    int splitClip(int pos, double timestamp);

    bool mergeClip(int pos);

    void removeClip(const int pos);

    QString toString();

    void saveInstanceState();
    void restoreInstanceState();

private slots:
    void onClipDataChanged(BaseClip *clip);

signals:
    void clipDataChanged(TimelineTrack *track, int clipIndex);

private:
    double preEndTime(int pos) const;
    double nextStartTime(int pos) const;
    BaseClip *makeClip(const SkyResourceBean &bean, SkyClip *clip);

    SkyTrack *m_skyTrack;
    SkyTimeline *m_skyTimeline;
    TrackType m_trackType = VideoTrack;
    QList<BaseClip *> m_clipList;
};

#endif // TRACKGROUP_H
