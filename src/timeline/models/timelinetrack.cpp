#include "audioclip.h"
#include "effectclip.h"
#include "imageclip.h"
#include "skyresourcebean.h"
#include "timelinetrack.h"
#include "videoclip.h"
#include <QDebug>
#include <limits.h>
#include <math.h>
#include <src/project/SEProject.h>
#include <timeline/venus/AsyncTimelineFrameMgr.h>
#include "base/utils/AppConstance.h"

TimelineTrack::TimelineTrack(SkyTimeline *timeline, TrackType type)
    : QObject(0),
      m_skyTimeline(timeline),
      m_trackType(type) {
    qDebug() << "TimelineTrack INIT:" << timeline;
    if (type == AudioTrack) {
        m_skyTrack = timeline->appendAudioTrack();
        qDebug() << " TimelineTrack Audio SkyTrack:" << (SkyAudioTrack *)m_skyTrack;
    } else {
        m_skyTrack = timeline->appendVideoTrack();
        qDebug() << " TimelineTrack Video SkyTrack:" << (SkyVideoTrack *)m_skyTrack;
    }
}

TimelineTrack::TimelineTrack(SkyTrack *skyTrack, TrackType type)
    : QObject(0),
      m_skyTrack(skyTrack),
      m_skyTimeline(skyTrack->getTimeline()),
      m_trackType(type) {
}

TimelineTrack::~TimelineTrack() {
    qDeleteAll(m_clipList);
    m_clipList.clear();
    m_skyTrack = nullptr;
    m_skyTimeline = nullptr;
    qDebug() << "release : TimelineTrack";
}

void TimelineTrack::removeBlankTrack() {
    if (m_skyTrack->numberOfClips() == 0) {
        m_skyTimeline->removeTrack(m_skyTrack);
        m_skyTrack = nullptr;
    }
}

QVariant TimelineTrack::get(int pos, int role) const {
    if (pos < 0 || pos >= m_clipList.size()) {
        return QVariant();
    }
    switch (role) {
        case RolePreEndTime:
            return preEndTime(pos);
        case RoleNextStartTime:
            return nextStartTime(pos);
        case RoleTrackType:
            return type();
        default:
            return m_clipList.at(pos)->get(role);
    }
}

SkyTrack *TimelineTrack::skyTrack() const {
    return m_skyTrack;
}

double TimelineTrack::preEndTime(int pos) const {
    double preEndTime = 0;
    if (pos >= 1) {
        preEndTime = m_clipList.at(pos - 1)->endTime();
    }
    return preEndTime;
}

double TimelineTrack::nextStartTime(int pos) const {
    double nextStartTime = -1;
    if (pos < m_clipList.size() - 1) {
        nextStartTime = m_clipList.at(pos + 1)->startTime();
    }
    return nextStartTime;
}

BaseClip *TimelineTrack::makeClip(const SkyResourceBean &bean, SkyClip *skyClip) {
    BaseClip *clip = nullptr;
    ResourceType m_resourceType = bean.resouceType();
    if (m_resourceType == AudioResource) {
        clip = new AudioClip(bean, skyClip);
    } else if (m_resourceType == ImageResource) {
        clip = new ImageClip(bean, skyClip);
    } else if (m_resourceType == VideoResource) {
        clip = new VideoClip(bean, skyClip);
    } else if (bean.isEffectResource()) {
        clip = new EffectClip(bean, skyClip);
    }
    QObject::connect(clip, &BaseClip::clipDataChanged, this, &TimelineTrack::onClipDataChanged);
    return clip;
}

bool TimelineTrack::moveToIndex(int index) {
    return m_skyTimeline->moveTrack(m_skyTrack, index);
}

BaseClip *TimelineTrack::get(int i) const {
    if (i < 0 || i >= m_clipList.size()) {
        return nullptr;
    }
    return m_clipList.at(i);
}

int TimelineTrack::count() const {
    return m_clipList.size();
}

TrackType TimelineTrack::type() const {
    return m_trackType;
}

bool TimelineTrack::isAudioTrack() const {
    return m_trackType == AudioTrack;
}

bool TimelineTrack::isVideoTrack() const {
    return m_trackType == VideoTrack;
}

bool TimelineTrack::isEffectTrack() const {
    return m_trackType == EffectTrack;
}

bool static clipSort(BaseClip *clip1, BaseClip *clip2) {
    return (clip1->startTime() < clip2->startTime());
}

int TimelineTrack::tryAddResource(SkyResourceBean &resource, double timestamp) {
    return tryAddResource(resource, resource.defaultTimeRange(), timestamp);
}

int TimelineTrack::tryAddResource(SkyResourceBean &resource, const SkyTimeRange &trimRange, double timestamp) {
    SkyTimeRange placeRange(timestamp, timestamp + trimRange.end - trimRange.begin);
    return tryAddResource(resource, trimRange, placeRange, SkyVariant::makeNull());
}

int TimelineTrack::tryAddResource(const SkyResourceBean &resource,
                                  const SkyTimeRange &trimRange,
                                  const SkyTimeRange &placeRange,
                                  const SkyVariant &config) {
    int targetIndex = -1;
    SkyTimeRange range = placeRange;
    for (int i = m_clipList.count() - 1; i >= 0; --i) {
        BaseClip *clip = m_clipList.at(i);
        if (range.begin - clip->endTime() >= -0.001) {
            targetIndex = i;
            if (range.begin < clip->endTime()) {
                range.begin = clip->endTime();
                range.end = range.begin + (placeRange.end - placeRange.begin);
            }
            break;
        }
    }

    //当前Track 放不下
    if (targetIndex < m_clipList.count() - 1 && m_clipList.at(targetIndex + 1)->startTime() < range.end) {
        qWarning() << "tryAddResource Failed! Track:" << m_skyTrack << " targetIndex:" << targetIndex;
        return -1;
    }
    SkyClip *skyClip = m_skyTrack->appendClip(resource.skyResouce());
    if (!config.isNull()) {
        skyClip->loadConfig(config);
    }

    if (!m_skyTrack->placeClip(skyClip, trimRange, range.begin, SkyPlaceClipStrategy_Strict)) {
        qWarning() << "tryAddResource Failed! Track:" << m_skyTrack << " ClipCount:" << m_skyTrack->numberOfClips()
                   << " PlaceTime:" << range.begin;
        m_skyTrack->removeClip(skyClip, true);
        return -1;
    }

    BaseClip *targetClip = makeClip(resource, skyClip);
    m_clipList.append(targetClip);

    if (type() == VideoTrack || type() == EffectTrack) {
        targetClip->addDefaultEffect();
    }

    std::sort(m_clipList.begin(), m_clipList.end(), clipSort);
    for (int i = 0; i < m_clipList.count(); ++i) {
        BaseClip *clip = m_clipList.at(i);
        if (targetClip == clip) {
            targetIndex = i;
        }
        qDebug() << "tryAddResource Trace Index:" << i << " " << clip->toString();
    }
    qDebug() << "tryAddResource Done! Track:" << m_skyTrack << " Index:" << targetIndex
             << " Clip: " << targetClip->toString();
    return targetIndex;
}

void TimelineTrack::updateClipResource(int pos,
                                       const SkyResourceBean &resource,
                                       double startTime,
                                       double endTime,
                                       bool isReuse,
                                       const SkyVariant &config) {
    BaseClip *clip = m_clipList.at(pos);
    BaseClip *newClip;
    SkyClip *skyClip = clip->skyClip();
    if (isReuse) {
        if (startTime >= 0 && endTime >= startTime) {
            SkyTimeRange trimRange(startTime, endTime);
            skyClip->updateResource(resource.skyResouce(), trimRange);
        } else {
            skyClip->updateResource(resource.skyResouce());
        }
        newClip = makeClip(resource, skyClip);
        ((VideoClip *)newClip)->updateVideoOverlayEffectScaleParam();
    } else {
        SkyTimeRange trackRange = skyClip->getTrackRange();
        int index = skyClip->getIndex();
        m_skyTrack->removeClip(skyClip, true);
        skyClip = m_skyTrack->appendClip(resource.skyResouce());
        if (!config.isNull()) {
            skyClip->loadConfig(config);
        }
        m_skyTrack->moveClip(skyClip, index);
        if (startTime >= 0 && endTime >= startTime) {
            SkyTimeRange trimRange(startTime, endTime);
            m_skyTrack->placeClip(skyClip, trimRange, trackRange.begin, SkyPlaceClipStrategy_Strict);
        } else {
            m_skyTrack->placeClip(skyClip, trackRange.begin, SkyPlaceClipStrategy_Strict);
        }
        newClip = makeClip(resource, skyClip);
        newClip->addDefaultEffect();
    }
    m_clipList.replace(pos, newClip);
    delete clip;
}

void TimelineTrack::onClipDataChanged(BaseClip *targetClip) {
    int targetIndex = -1;
    for (int i = 0; i < m_clipList.count(); ++i) {
        BaseClip *clip = m_clipList.at(i);
        if (targetClip == clip) {
            targetIndex = i;
            break;
        }
    }
    if (targetIndex == -1) {
        qWarning() << "NOT FOUND CLIP:" << targetClip->toString();
        return;
    }
    qDebug() << "onClipDataChanged Clip Index:" << targetIndex << " " << targetClip->toString();
    emit clipDataChanged(this, targetIndex);
}

void TimelineTrack::cropClip(int pos, double clipStartTime, double clipEndTime) {
    BaseClip *clip = m_clipList.at(pos);
    double targetCropStart = clip->cropStartTime();
    double targetCropEnd = clip->cropEndTime();
    double preEndTime = clip->endTime();
    double preStartTime = clip->startTime();

    bool limitClipResource = clip->clipType() == VideoClipType || clip->clipType() == AudioClipType;

    qDebug() << "cropClip Start StartTime:" << clip->toString() << " -> Range:[" << clipStartTime << "," << clipEndTime
             << "]"
             << " Speed:" << clip->getAvgSpeed();

    if (limitClipResource) {
        double speed = clip->getAvgSpeed();
        if (preStartTime == clipStartTime) {
            targetCropEnd = (clipEndTime - clipStartTime) * speed + targetCropStart;
            if (targetCropEnd > clip->resouceTime()) {
                targetCropEnd = clip->resouceTime();
                clipEndTime = clipStartTime + (targetCropEnd - targetCropStart) / speed;
            }
        } else {
            targetCropStart = targetCropEnd - (clipEndTime - clipStartTime) * speed;
            if (targetCropStart < 0) {
                targetCropStart = 0;
                clipStartTime = clipEndTime - (targetCropEnd - targetCropStart) / speed;
            }
        }
    } else {
        targetCropStart = 0;
        targetCropEnd = clipEndTime - clipStartTime + targetCropStart;
    }

    qDebug() << "cropClip clipStartTime:" << clipStartTime << "  targetCropStart:" << targetCropStart
             << "  targetCropEnd:" << targetCropEnd;
    SkyTimeRange clipRange(targetCropStart, targetCropEnd);

    if (m_skyTrack->placeClip(clip->skyClip(), clipRange, clipStartTime, SkyPlaceClipStrategy_Strict)) {
        clip->updateVolumeFadeEffectParam();
        if (clip->hasVenusEffect()) {
            AsyncTimelineFrameMgr::getIns()->updateClipProcessTask(clip);
        }
        qDebug() << "cropClip Success Clip:" << clip->toString() << " Speed:" << clip->getAvgSpeed();
    } else {
        qWarning() << "cropClip Failed! Clip:" << clip->toString();
    }
}

int TimelineTrack::moveClip(int pos, double timestamp) {
    BaseClip *clip = m_clipList.at(pos);

    int targetIndex = -1;
    m_clipList.removeAt(pos);
    for (int i = 0; i < m_clipList.count(); i++) {
        BaseClip *c = m_clipList.at(i);
        if (abs(timestamp + clip->during() - c->startTime()) < 0.0000001) {
            timestamp = c->startTime() - clip->during();
            break;
        } else if (abs(timestamp - c->endTime()) < 0.0000001) {
            timestamp = c->endTime();
            break;
        }
    }
    for (int i = m_clipList.count() - 1; i >= 0; --i) {
        BaseClip *c = m_clipList.at(i);
        if (timestamp >= c->endTime()) {
            targetIndex = i;
            break;
        }
    }

    qDebug() << "moveClip Track:" << m_skyTrack << " Index:" << targetIndex << " Clip: " << clip->toString();

    //当前Track 放不下
    if (targetIndex < m_clipList.count() - 1 &&
        m_clipList.at(targetIndex + 1)->startTime() < timestamp + clip->during()) {
        qWarning() << "tryAddResource Failed!22 Track:" << m_skyTrack << " ClipCount:" << m_skyTrack->numberOfClips()
                   << " Clip:" << clip;
        m_clipList.insert(pos, clip);
        return -1;
    }

    if (!m_skyTrack->placeClip(clip->skyClip(), timestamp, SkyPlaceClipStrategy_Strict)) {
        m_clipList.insert(pos, clip);
        qDebug() << "tryAddResource Failed!33 Track:" << m_skyTrack << " ClipCount:" << m_skyTrack->numberOfClips()
                 << " Clip:" << clip;
        return -1;
    }

    m_clipList.insert(pos, clip);
    std::sort(m_clipList.begin(), m_clipList.end(), clipSort);
    for (int i = 0; i < m_clipList.count(); ++i) {
        BaseClip *c = m_clipList.at(i);
        if (clip == c) {
            targetIndex = i;
        }
        qDebug() << "moveClip Trace Index:" << i << " " << clip->toString();
    }
    qDebug() << "moveClip Done! Track:" << m_skyTrack << " Index:" << targetIndex << " Clip: " << clip->toString();
    return targetIndex;
}

void TimelineTrack::removeClip(const int pos) {
    BaseClip *clip = m_clipList.at(pos);
    m_clipList.removeAt(pos);
    m_skyTrack->removeClip(clip->skyClip(), true);
    delete clip;
}

int TimelineTrack::splitClip(int pos, double timestamp) {
    if (pos >= m_clipList.size()) {
        return -1;
    }
    BaseClip *clip = m_clipList.at(pos);
    double volumeFadeIn = clip->getVolumeFadeInDuring();
    double volumeFadeOut = clip->getVolumeFadeOutDuring();
    SkyClip *newSkyClip = m_skyTrack->splitClip(clip->skyClip(), timestamp);
    if (newSkyClip == nullptr) {
        qDebug() << "splitClip Failed!" << clip->toString();
        return -1;
    }

    BaseClip *newClip = makeClip(clip->resourceBean(), newSkyClip);

    clip->setVolumeFadeInDuring(0);
    clip->setVolumeFadeOutDuring(volumeFadeOut);
    newClip->setVolumeFadeInDuring(volumeFadeIn);

    qDebug() << "splitClip: Done! " << toString();
    m_clipList.insert(pos, newClip);

    if ((clip->clipType() == VideoClipType || clip->clipType() == ImageClipType) &&
        ((VideoClip *)clip)->getTransition() != nullptr) {
        auto transitionClip = ((VideoClip *)clip)->getTransition();
        auto transitionRes = transitionClip->resourceBean();
        auto transitionDuring = transitionClip->during();
        ((VideoClip *)clip)->removeTransition();
        if (!((VideoClip *)newClip)->addTransition(transitionRes, transitionDuring)) {
            qDebug() << "splitClip: addTransition Failed! " << clip->toString();
        }
    }

    clip->ofParamChanged(clip, "Clip:CurvedSpeed", true);

    if (clip->hasVenusEffect()) {
        AsyncTimelineFrameMgr::getIns()->updateClipProcessTask(clip);
    }
    return pos;
}

bool TimelineTrack::mergeClip(int pos) {
    if (pos >= m_clipList.size() - 1) {
        return false;
    }
    BaseClip *clip1 = m_clipList.at(pos);
    BaseClip *clip2 = m_clipList.at(pos + 1);
    qDebug() << "mergeClip Pre" << clip1->toString() << " Clip2:" << clip2->toString();
    if (m_skyTrack->mergeClips(clip1->skyClip(), clip2->skyClip())) {
        m_clipList.removeAt(pos);
        delete clip1;
        return true;
    } else {
        return false;
    }
}

QString TimelineTrack::toString() {
    QString str = "";
    for (int i = 0; i < m_clipList.size(); ++i) {
        str = str + m_clipList.at(i)->toString();
        str += " ; ";
    }
    return str;
}

void TimelineTrack::saveInstanceState() {
    for (BaseClip *clip : m_clipList) {
        clip->resourceBean().saveSkyVariant(clip->skyClip());
        clip->saveInstanceState();
    }
}

void TimelineTrack::restoreInstanceState() {
    int count = m_skyTrack->numberOfClips();
    for (int i = 0; i < count; ++i) {
        SkyClip *skyClip = m_skyTrack->getClipAt(i);

        auto resource = SkyResourceBean::restoreSkyVariant(skyClip);
        if (!resource.isValidResource()) {
            SkyResource skyResource = skyClip->getResource();
            resource = skyResource;
        }
        qDebug() << "restoreInstanceState Resource:" << resource.toString();
        BaseClip *clip = makeClip(resource, skyClip);
        clip->restoreInstanceState();
        //        if(clip->skyEffectWrapper() == nullptr){
        //            clip->addDefaultEffect();
        //        }
        m_clipList.append(clip);
    }
}
