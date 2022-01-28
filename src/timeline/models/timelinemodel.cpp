#include "timelinemodel.h"

#include <base/utils/SkyVariantUtil.h>
#include <base/utils/threadpools.h>
#include <base/utils/fileutils.h>
#include <math.h>

#include <QDebug>
#include <QList>
#include <QModelIndex>
#include <QStandardPaths>
#include <QVector>
#include <QDir>

#include "TimelineDef.h"
#include "skyresourcebean.h"
#include "src/project/SEProject.h"
#include "src/ui/preview/SkyTimelineView.h"
#include "videoclip.h"
#include <timeline/venus/AsyncTimelineFrameMgr.h>

static const quintptr NO_PARENT_ID = quintptr(-1);
TimelineModel::TimelineModel(QObject *parent) : QAbstractItemModel(parent) {
    QObject::connect(this, &TimelineModel::dataChanged, this, &TimelineModel::onDataChanged);
    QObject::connect(this, &TimelineModel::removeClipData, [=](BaseClip *clip) {
        m_inputDeviceList.remove(clip);
    });
}

TimelineModel::~TimelineModel() {
    qInfo() << "TimelineModel::~TimelineModel begin";
    AsyncTimelineFrameMgr::getIns()->destroyTask();

    qDeleteAll(m_trackList);
    m_trackList.clear();
    m_inputDeviceList.clear();
    qInfo() << "TimelineModel::~TimelineModel";
}

int TimelineModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        if (parent.internalId() != NO_PARENT_ID) {
            return 0;
        }
        return m_trackList.at(parent.row())->count();
    }
    return m_trackList.count();
}

int TimelineModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant TimelineModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.parent().isValid()) {
        // 处理轨道里列数据
        TimelineTrack *track = m_trackList.at(index.internalId());
        return track->get(index.row(), role);
    } else {
        // 处理视频轨道
    }
    return QVariant();
}

BaseClip *TimelineModel::data(const QModelIndex &index) {
    if (!index.isValid()) {
        return nullptr;
    }
    if (index.parent().isValid()) {
        int trackIndex = index.internalId();
        int clipIndex = index.row();
        if (trackIndex < m_trackList.size()) {
            TimelineTrack *track = m_trackList.at(trackIndex);
            if (clipIndex < track->count()) {
                BaseClip *clip = track->get(clipIndex);
                return clip;
            }
        }

    } else {
    }
    return nullptr;
}

QModelIndex TimelineModel::index(int row, int column, const QModelIndex &parent) const {
    if (column > 0) {
        return QModelIndex();
    }
    QModelIndex result;
    if (parent.isValid()) {
        if (row < m_trackList.at(parent.row())->count()) {
            result = createIndex(row, column, parent.row());
        }
    } else if (row < m_trackList.count()) {
        result = createIndex(row, column, NO_PARENT_ID);
    }
    return result;
}

QModelIndex TimelineModel::makeIndex(int trackIndex, int clipIndex) const {
    return index(clipIndex, 0, index(trackIndex));
}

QModelIndex TimelineModel::parent(const QModelIndex &index) const {
    if (!index.isValid() || index.internalId() == NO_PARENT_ID) {
        return QModelIndex();
    } else {
        return createIndex(index.internalId(), 0, NO_PARENT_ID);
    }
}

QHash<int, QByteArray> TimelineModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RoleResource] = TRACK_RESOURCE;
    roles[RoleType] = TRACK_TYPE;
    roles[RoleFrameRate] = TRACK_FRAME_RATE;
    roles[RoleDuration] = TRACK_DURING;
    roles[RoleStartTime] = TRACK_BEGIN_TIME;
    roles[RoleEndTime] = TRACK_END_TIME;
    roles[RoleCropStartTime] = TRACK_CROP_START_TIME;
    roles[RoleCropEndTime] = TRACK_CROP_END_TIME;
    roles[RoleResourceTime] = TRACK_RESOURCE_TIME;
    roles[RoleThumbnails] = TRACK_THUMBNAILS;
    roles[RolePreEndTime] = TRACK_PRE_END_TIME;
    roles[RoleNextStartTime] = TRACK_NEXT_START_TIME;
    roles[RoleTrackType] = TRACK_TRACK_TYPE;
    roles[RoleClipType] = TRACK_CLIP_TYPE;
    roles[RoleIconPath] = TRACK_ICON_PATH;
    roles[RoleResouceType] = TRACK_RESOURCE_TYPE;
    roles[RoleName] = TRACK_RESOURCE_NAME;
    roles[RoleMinTransitionDuring] = TRACK_MIN_TRANSITION_DURING;
    roles[RoleMaxTransitionDuring] = TRACK_MAX_TRANSITION_DURING;
    roles[RoleTransitionDuring] = TRACK_TRANSITION_DURING;
    roles[RoleVolume] = TRACK_VOLUME;
    roles[RoleWordContent] = TRACK_WORD_CONTENT;
    roles[RoleSpeed] = TRACK_SPEED;
    roles[RoleFreezeDuration] = TRACK_FREEZE_DURATION;
    roles[RoleCurvedSpeed] = TRACK_CURVED_SPEED;
    roles[RoleAvgSpeed] = TRACK_AVG_SPEED;
    roles[RoleHasInputDevice] = TRACK_HAS_INPUT_DEVICE;
    roles[RoleKeyFrameTimes] = TRACK_KEY_FRAME_TIMES;
    roles[RoleGroupInfos] = TRACK_KEY_GroupInfos;

    return roles;
}

QModelIndex TimelineModel::addResource(TrackType type,
                                       SkyResourceBean &resource,
                                       const SkyTimeRange &_trimRange,
                                       int trackIndex,
                                       double timestamp,
                                       const SkyVariant &config) {
    SkyTimeRange trimRange;
    if (_trimRange.end <= _trimRange.begin) {
        trimRange = resource.defaultTimeRange();
    } else {
        trimRange = _trimRange;
    }
    SkyTimeRange trackRange(timestamp, timestamp + trimRange.end - trimRange.begin);
    return addResource(type, resource, trimRange, trackRange, trackIndex, config);
}

QModelIndex TimelineModel::addResource(TrackType type,
                                       SkyResourceBean &resource,
                                       const SkyTimeRange &_trimRange,
                                       const SkyTimeRange &trackRange,
                                       int trackIndex,
                                       const SkyVariant &config) {

    SkyTimeRange trimRange;
    if (_trimRange.end <= _trimRange.begin) {
        trimRange = resource.defaultTimeRange();
    } else {
        trimRange = _trimRange;
    }
    qDebug() << "addResource beginInsertRows NEW Track! trackIndex" << trackIndex << " Place Time: ["
             << trackRange.begin << "," << trackRange.end << "]  Crop: [" << trimRange.begin << ", " << trimRange.end
             << "] ";
    if (trimRange.end <= trimRange.begin || trackRange.end <= trackRange.begin) {
        return QModelIndex();
    }

    TimelineTrack *targetTrack = nullptr;
    int clipIndex = -1;
    if (trackIndex >= 0 && trackIndex < m_trackList.count()) {
        TimelineTrack *track = m_trackList.at(trackIndex);
        if (track != nullptr && track->type() == type) {
            clipIndex = track->tryAddResource(resource, trimRange, trackRange, config);
            if (clipIndex >= 0) {
                targetTrack = track;
            }
        }
    } else {
        for (int i = 0; i < m_trackList.count(); ++i) {
            TimelineTrack *track = m_trackList.at(i);
            if (track != nullptr && track->type() == type) {
                clipIndex = track->tryAddResource(resource, trimRange, trackRange, config);
                if (clipIndex >= 0) {
                    targetTrack = track;
                    trackIndex = i;
                    break;
                }
            }
        }
    }
    if (targetTrack == nullptr) {
        trackIndex = 0;
        beginInsertRows(QModelIndex(), trackIndex, trackIndex);
        targetTrack = new TimelineTrack(getCurrentTimeline(), type);
        if (targetTrack->moveToIndex(m_trackList.size())) {
            m_trackList.insert(trackIndex, targetTrack);
        }
        QObject::connect(targetTrack, &TimelineTrack::clipDataChanged, this, &TimelineModel::onClipDataChanged);
        clipIndex = targetTrack->tryAddResource(resource, trimRange, trackRange, config);
        endInsertRows();
        qDebug() << "addResource beginInsertRows NEW Track! trackIndex" << trackIndex << " Clip:" << clipIndex;
    } else {
        beginInsertRows(index(trackIndex), clipIndex, clipIndex);
        endInsertRows();
        notifyDataChanged(trackIndex, clipIndex - 1, clipIndex + 1);
        qDebug() << "addResource beginInsertRows TrackIndex:trackIndex" << trackIndex << " Clip:" << clipIndex;
    }
    for (int i = 0; i < m_trackList.count(); ++i) {
        qDebug() << "addResource trace:" << i << " Track:" << m_trackList.at(i)->toString();
    }

    return makeIndex(trackIndex, clipIndex);
}

SkyResourceBean TimelineModel::removeTransitionResource(SkyResourceBean &resource, QModelIndex &target) {
    BaseClip *targetClip = data(target);
    if (targetClip->clipType() != VideoClipType && targetClip->clipType() != ImageClipType) {
        return SkyResourceBean();
    }
    VideoClip *clip = (VideoClip *)targetClip;
    SkyResourceBean preTransitionResourceBean;
    TransitionClip *transitionClip = clip->getTransition();
    if (transitionClip != nullptr) {
        preTransitionResourceBean = transitionClip->resourceBean();
        emit removeClipData(transitionClip);
    }

    clip->removeTransition();
    return preTransitionResourceBean;
}

QPair<SkyResourceBean, QModelIndex> TimelineModel::addTransitionResource(SkyResourceBean &resource,
                                                                         double timestamp,
                                                                         QModelIndex &target) {
    SkyResourceBean preTransitionResourceBean;
    BaseClip *targetClip = data(target);
    if (targetClip != nullptr) {
        TransitionClip *transitionClip = ((VideoClip *)targetClip)->getTransition();
        if (transitionClip != nullptr) {
            preTransitionResourceBean = transitionClip->resourceBean();
            emit removeClipData(transitionClip);
            ((VideoClip *)targetClip)->removeTransition();
        }
        if (((VideoClip *)targetClip)->addTransition(resource)) {
            qDebug() << "Add Transition Success??";
            QPair<SkyResourceBean, QModelIndex> pair(preTransitionResourceBean, target);
            return pair;
        }
    }

    QList<VideoClip *> clips;
    for (int i = 0; i < m_trackList.size(); ++i) {
        if (m_trackList.at(i)->type() == VideoTrack) {
            for (int j = 0; j < m_trackList.at(i)->count(); ++j) {

                if (m_trackList.at(i)->get(j)->startTime() <= timestamp &&
                    m_trackList.at(i)->get(j)->endTime() > timestamp) {
                    qDebug() << "addTransitionResource Index:" << j
                             << " During:" << (m_trackList.at(i)->get(j)->startTime() - timestamp);
                    if (j < m_trackList.at(i)->count() - 1 &&
                        (m_trackList.at(i)->get(j + 1)->startTime() - m_trackList.at(i)->get(j)->endTime()) < 0.001) {
                        clips.append((VideoClip *)m_trackList.at(i)->get(j + 1));
                    }
                    if (j > 0 &&
                        (m_trackList.at(i)->get(j)->startTime() - m_trackList.at(i)->get(j - 1)->endTime()) < 0.001) {
                        clips.append((VideoClip *)m_trackList.at(i)->get(j));
                    }
                }
            }
        }
    }

    qSort(clips.begin(), clips.end(), [&timestamp](const BaseClip *a, const BaseClip *b) -> bool {
        return abs(a->startTime() - timestamp) < abs(b->startTime() - timestamp);
    });

    for (VideoClip *clip : clips) {
        qDebug() << "addTransitionResource Clip:" << (clip->startTime() - timestamp);
    }

    for (VideoClip *clip : clips) {
        TransitionClip *transitionClip = clip->getTransition();
        if (transitionClip != nullptr) {
            preTransitionResourceBean = transitionClip->resourceBean();
        }
        if (clip->addTransition(resource)) {
            qDebug() << "Add Transition Success??";
            targetClip = clip;
            break;
        }
    }

    for (int i = 0; i < m_trackList.size(); ++i) {
        if (m_trackList.at(i)->type() == VideoTrack) {
            for (int j = 0; j < m_trackList.at(i)->count(); ++j) {
                if (m_trackList.at(i)->get(j) == targetClip) {
                    QPair<SkyResourceBean, QModelIndex> pair(preTransitionResourceBean, makeIndex(i, j));
                    return pair;
                }
            }
        }
    }
    return QPair<SkyResourceBean, QModelIndex>();
}

void TimelineModel::addTrack(TimelineTrack *track) {
    m_trackList.insert(0, track);
}

QString TimelineModel::firstVideoTrackResource() {
    QString string = "";
    for (int i = 0; i < m_trackList.size(); ++i) {
        if (m_trackList.at(i)->type() == VideoTrack && m_trackList.at(i)->count() > 0) {
            string = m_trackList.at(i)->get(0)->resourceBean().entryPath();
        }
    }
    return string;
}

void TimelineModel::onClipDataChanged(TimelineTrack *track, int clipIndex) {
    int trackIndex = -1;
    for (int i = 0; i < m_trackList.size(); ++i) {
        if (m_trackList.at(i) == track) {
            trackIndex = i;
            break;
        }
    }
    if (trackIndex == -1) {
        qWarning() << "onClipDataChanged Failed!:" << trackIndex << "," << clipIndex;
        return;
    }

    auto clip = track->get(clipIndex);
    emit clipDataChanged(clip);

    qDebug() << "onClipDataChanged:" << trackIndex << "," << clipIndex;
    notifyDataChanged(trackIndex, clipIndex, clipIndex);
}

void TimelineModel::cropClip(const QModelIndex &index, double clipStartTime, double clipEndTime) {
    if (!index.isValid() || !index.parent().isValid()) {
        return;
    }

    TimelineTrack *track = m_trackList.at(index.internalId());
    int colIndex = index.row();
    BaseClip *clip = track->get(colIndex);
    double preEndTime = clip->endTime();
    double preStartTime = clip->startTime();

    track->cropClip(colIndex, clipStartTime, clipEndTime);

    int targetIndex = -1;
    if (clip->clipType() == VideoClipType || clip->clipType() == ImageClipType) {
        if (abs(preEndTime - clip->endTime()) > 0.001 && colIndex + 1 < track->count()) {
            targetIndex = colIndex + 1;

        } else if (abs(preStartTime - clip->startTime()) > 0.001) {
            targetIndex = colIndex;
        }
    }

    if (targetIndex >= 0) {
        tryRemoveTransition(index.internalId(), targetIndex);
    }
    int beginIndex = colIndex;
    int endIndex = colIndex;
    if (abs(preEndTime - clipEndTime) > 0.001) {
        endIndex = colIndex + 1;
    } else {
        beginIndex = colIndex - 1;
    }
    notifyDataChanged(index.internalId(), beginIndex, endIndex);
}

SkyTimeline *TimelineModel::getCurrentTimeline() {
    SkyTimeline *tl = SEProject::current()->dom()->timeline();
    if (tl != nullptr) {
        return tl;
    } else {
        qInfo("current timeline is null!");
        return nullptr;
    }
}

QModelIndex TimelineModel::moveClip(const QModelIndex &index, double time, int trackIndex, bool newTrack) {
    if (!index.isValid() || !index.parent().isValid()) {
        qDebug() << "moveClip Invalid?? QModelIndex?";
        return QModelIndex();
    }

    int rowIndex = index.internalId();
    int colIndex = index.row();
    TimelineTrack *track = m_trackList.at(rowIndex);
    BaseClip *nextClip = track->get(colIndex + 1);
    BaseClip *clip = track->get(colIndex);
    //单个片段单独新增相邻轨道时做优化
    if (newTrack && track->count() == 1 && (rowIndex == trackIndex || trackIndex - rowIndex == 1)) {
        newTrack = false;
        trackIndex = rowIndex;
    }

    qDebug() << "moveClip: MoveTo:" << time << " TrackIndex:" << rowIndex << "->" << trackIndex << " New:" << newTrack
             << " Clip:" << track->get(colIndex)->toString();

    if (rowIndex == trackIndex && !newTrack) {
        int targetColIndex = track->moveClip(colIndex, time);
        if (targetColIndex >= 0) {
            if (targetColIndex == colIndex) {
                notifyDataChanged(trackIndex, colIndex, -1);
            } else {
                beginMoveRows(index.parent(),
                              colIndex,
                              colIndex,
                              index.parent(),
                              (colIndex - targetColIndex) == 1 ? targetColIndex : targetColIndex + 1);
                endMoveRows();
                int min = fmin(targetColIndex, colIndex);
                int max = fmax(targetColIndex, colIndex);
                notifyDataChanged(trackIndex, min, max + 1);
            }

            tryRemoveClipTransition(trackIndex, clip);
            tryRemoveClipTransition(trackIndex, nextClip);
            return makeIndex(rowIndex, targetColIndex);
        } else {
            notifyDataChanged(rowIndex, colIndex, colIndex);
            qDebug() << "moveClip Same Track Failed!!:" << time << " TrackIndex:" << rowIndex;
            return QModelIndex();
        }
    }

    if (!newTrack && trackIndex < m_trackList.size() && m_trackList.at(trackIndex)->type() != track->type()) {
        qDebug() << "moveClip: Move Clip Failed! Type is different! From:" << track->type()
                 << " To:" << m_trackList.at(trackIndex)->type();
        notifyDataChanged(rowIndex, colIndex, colIndex);
        return QModelIndex();
    }

    SkyResourceBean resource = clip->resourceBean();
    SkyTimeRange range = clip->skyClip()->getTrimRange();
    SkyTimeRange trackRange(time, clip->during() + time);
    SkyVariant clipConfig = clip->skyClip()->saveConfig();

    TimelineTrack *targetTrack = nullptr;

    if (newTrack || trackIndex >= m_trackList.size()) {
        beginInsertRows(QModelIndex(), trackIndex, trackIndex);
        targetTrack = new TimelineTrack(getCurrentTimeline(), track->type());
        if (targetTrack->moveToIndex(m_trackList.size() - trackIndex)) {
            m_trackList.insert(trackIndex, targetTrack);
        }
        QObject::connect(targetTrack, &TimelineTrack::clipDataChanged, this, &TimelineModel::onClipDataChanged);
        endInsertRows();
        qDebug() << "addResource beginInsertRows NEW Track! trackIndex" << trackIndex;
    } else {
        targetTrack = m_trackList.at(trackIndex);
    }
    int clipIndex = targetTrack->tryAddResource(resource, range, trackRange, clipConfig);
    if (clipIndex >= 0) {
        tryRemoveClipTransition(trackIndex, clip);
        tryRemoveClipTransition(trackIndex, nextClip);

        emit removeClipData(clip);

        if (newTrack && trackIndex <= rowIndex) {
            rowIndex++;
        }
        QModelIndex fixIndexPos = makeIndex(rowIndex, colIndex);

        beginRemoveRows(fixIndexPos.parent(), colIndex, colIndex);
        qDebug() << "moveClip: Remove Clip" << resource.toString();
        track->removeClip(colIndex);
        endRemoveRows();

        BaseClip *newClip = targetTrack->get(clipIndex);

        newClip->loadSkyEffect();

        beginInsertRows(this->index(trackIndex), clipIndex, clipIndex);
        endInsertRows();

        notifyDataChanged(rowIndex, colIndex - 1, colIndex);
        notifyDataChanged(trackIndex, clipIndex - 1, clipIndex + 1);

        for (int i = 0; i < m_trackList.count(); ++i) {
            qDebug() << "moveClip trace:" << i << " Track:" << m_trackList.at(i)->toString();
        }

        return makeIndex(trackIndex, clipIndex);
    } else {
        notifyDataChanged(rowIndex, colIndex, colIndex);
        qWarning() << "addClip Failed! Remove ";
        return QModelIndex();
    }
    qDebug() << "addClip Failed! retry ";
}

bool TimelineModel::removeClip(const QModelIndex &index, bool deleteTrackWhenEmpty) {
    if (!index.isValid() || !index.parent().isValid()) {
        return false;
    }
    int rowIndex = index.internalId();
    int colIndex = index.row();
    TimelineTrack *track = m_trackList.at(rowIndex);
    BaseClip *clip = track->get(colIndex);

    tryRemoveTransition(rowIndex, colIndex + 1);
    tryRemoveTransition(rowIndex, colIndex);

    qDebug() << "Remove Clip" << clip->toString();
    emit removeClipData(clip);

    beginRemoveRows(index.parent(), colIndex, colIndex);
    track->removeClip(colIndex);
    endRemoveRows();

    if (deleteTrackWhenEmpty && track->count() == 0) {
        beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
        m_trackList.removeAt(rowIndex);
        delete track;
        endRemoveRows();
        qDebug() << "Remove Track " << rowIndex << " Track Count:" << m_trackList.size();
    } else {
        notifyDataChanged(rowIndex, colIndex, colIndex + 1);
    }
    return true;
}

void TimelineModel::notifyDataChanged(int trackIndex, int clipIndexFrom, int clipIndexTo) {
    if (trackIndex == -1) {
        beginResetModel();
        endResetModel();
        return;
    }
    if (clipIndexFrom == -1) {
        clipIndexFrom = 0;
    }
    if (clipIndexTo == -1 || clipIndexTo >= m_trackList.at(trackIndex)->count()) {
        clipIndexTo = m_trackList.at(trackIndex)->count() - 1;
    }
    if (trackIndex >= m_trackList.size() || clipIndexFrom > clipIndexTo ||
        clipIndexFrom >= m_trackList.at(trackIndex)->count()) {
        return;
    }
    QModelIndex indexFrom = makeIndex(trackIndex, clipIndexFrom);
    QModelIndex indexTo = makeIndex(trackIndex, clipIndexTo);
    emit dataChanged(indexFrom, indexTo);
}

QVariantList TimelineModel::getClipEdgeList(int trackIndex, int clipIndex) {
    QVariantList data;
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        for (int j = 0; j < track->count(); j++) {
            if (i == trackIndex && j == clipIndex) {
                continue;
            }
            BaseClip *clip = track->get(j);
            data.append(clip->startTime());
            data.append(clip->endTime());
        }
    }
    return data;
}

void TimelineModel::notifyFromClip(BaseClip *clip) {
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        for (int j = 0; j < track->count(); j++) {
            if (clip == track->get(j)) {
                notifyDataChanged(i, j, j);
                return;
            }
        }
    }
}

bool TimelineModel::hasClip() {
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        if (track->count() > 0) {
            return true;
        }
    }
    return false;
}

QModelIndex TimelineModel::splitClip(const int trackIndex, const int clipIndex, const double timestamp) {
    TimelineTrack *targetTrack = nullptr;
    int clipPos = clipIndex;
    int trackPos = trackIndex;
    qDebug() << "splitClip Start Track:" << trackIndex << " Clip:" << clipIndex << " Time:" << timestamp;
    if (trackIndex < 0 || clipIndex < 0) {
        for (int i = 0; i < m_trackList.count(); ++i) {
            TimelineTrack *track = m_trackList.at(i);
            if (track != nullptr && track->type() == VideoTrack) {
                for (int j = 0; j < track->count(); ++j) {
                    BaseClip *clip = track->get(j);
                    if (clip->startTime() < timestamp && timestamp < clip->endTime()) {
                        trackPos = i;
                        targetTrack = track;
                        clipPos = j;
                        break;
                    }
                }
            }
        }
    } else {
        targetTrack = m_trackList.at(trackIndex);
    }
    qDebug() << "splitClip Track:" << trackPos << " Clip:" << clipPos;
    if (targetTrack != nullptr) {
        if (targetTrack->splitClip(clipPos, timestamp) >= 0) {
            qDebug() << "Split Clip " << clipPos << " Time:" << timestamp;
            beginInsertRows(this->index(trackPos), clipPos, clipPos);
            endInsertRows();
            notifyDataChanged(trackPos, clipPos + 1, clipPos + 1);
            return makeIndex(trackPos, clipPos);
        }
    }
    return QModelIndex();
}

bool TimelineModel::mergeClip(const QModelIndex &index) {
    int trackIndex = index.internalId();
    int clipIndex = index.row();
    qDebug() << "mergeClip Track:" << trackIndex << " Clip:" << clipIndex;
    TimelineTrack *track = m_trackList.at(trackIndex);
    if (track->mergeClip(clipIndex)) {
        qDebug() << "Merge TrackIndex: " << trackIndex << " ClipIndex:" << clipIndex;
        beginRemoveRows(index.parent(), clipIndex, clipIndex);
        endRemoveRows();
        notifyDataChanged(trackIndex, clipIndex, clipIndex);
        auto clip = track->get(clipIndex);
        clip->ofParamChanged(clip, "Clip:CurvedSpeed", true);
        return true;
    }
    return false;
}

void TimelineModel::seekTo(const double time) {
    getCurrentTimeline()->seekTo(time);
}

TimelineTrack *TimelineModel::getTrackFromIndex(int index) {
    if (index < m_trackList.count()) {
        return m_trackList.at(index);
    } else {
        return nullptr;
    }
}

void TimelineModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
    int beginRow = topLeft.internalId();
    int beginCol = topLeft.row();
    int endRow = bottomRight.internalId();
    int endCol = bottomRight.row();

    for (int row = beginRow; row <= endRow && row < m_trackList.size(); ++row) {
        TimelineTrack *track = m_trackList.at(row);
        for (int col = beginCol; col <= endCol && col < track->count(); ++col) {
            emit clipDataChanged(track->get(col));
        }
    }
}

void TimelineModel::saveInstanceState() {
    for (TimelineTrack *track : m_trackList) {
        track->skyTrack()->setUserData("TrackTypeKey", nameFromTrackType(track->type()));
        track->saveInstanceState();
    }
    //输入器配置
    QFile file(SEProject::current()->proj()->inputList());
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray inputDeviceConfig;
        for (InputDevice inputDevice : m_inputDeviceList.values()) {
            inputDeviceConfig.push_back(inputDevice.toJson());
        }
        QString json = QJsonDocument(inputDeviceConfig).toJson();
        QTextStream stream(&file);
        stream << json << endl;
        file.close();
    }
}

void TimelineModel::restoreInstanceState() {
    int count = getCurrentTimeline()->numberOfTracks();
    for (int i = 0; i < count; ++i) {
        SkyTrack *skyTrack = getCurrentTimeline()->getTrackAt(i);
        TrackType type = VideoTrack;
        if (!skyTrack->getUserData("TrackTypeKey").isNull()) {
            type = trackTypeFromName(skyTrack->getUserData("TrackTypeKey").toString());
        } else if (!skyTrack->getUserData("sky_owner_type").isNull()) {
            type = (TrackType)skyTrack->getUserData("sky_owner_type").toInt();
        } else {
            if (skyTrack->numberOfClips() > 0) {
                SkyResource resource = skyTrack->getClipAt(0)->getResource();
                if (resource.isStatic() && QString::fromUtf8(resource.getPath().c_str()).contains("gap")) {
                    type = EffectTrack;
                } else if (resource.hasVideoStream()) {
                    type = VideoTrack;
                } else {
                    type = AudioTrack;
                }
                qDebug() << "restoreInstanceState First Resource:" << resource.getPath().c_str() << " Type:" << type;
            }
        }
        if (type == VideoTrack && skyTrack->isAudioTrack()) {
            type = AudioTrack;
            qDebug() << "TrackType is Error! AudioTrack " << skyTrack->isAudioTrack();
        } else if (type == AudioTrack && skyTrack->isVideoTrack()) {
            type = VideoTrack;
            qDebug() << "TrackType is Error! VideoTrack " << skyTrack->isVideoTrack();
        }

        TimelineTrack *track = new TimelineTrack(skyTrack, type);
        QObject::connect(track, &TimelineTrack::clipDataChanged, this, &TimelineModel::onClipDataChanged);
        track->restoreInstanceState();
        int trackIndex = 0;
        beginInsertRows(QModelIndex(), trackIndex, trackIndex);
        m_trackList.insert(trackIndex, track);
        endInsertRows();
        qDebug() << "restoreInstanceState SkyTrack ClipCount:" << track->count();
    }
    qDebug() << "restoreInstanceState SkyTrack:" << m_trackList.count();
    //输入器配置
    QFile file(SEProject::current()->proj()->inputList());
    if (file.open(QIODevice::ReadOnly)) {
        QString json = QString::fromUtf8(file.readAll());
        QJsonArray inputDeviceConfig = QJsonDocument::fromJson(json.toUtf8()).array();
        QList<InputDevice> tempList;
        for (auto config : inputDeviceConfig) {
            InputDevice inputDevice = InputDevice::makeInputDeviceFromJson(config.toObject());
            tempList.push_back(inputDevice);
        }
        for (auto track : m_trackList) {
            for (int i = 0; i < track->count(); i++) {
                BaseClip *clip = track->get(i);
                bool equals = false;
                for (auto inputDevice : tempList) {
                    if (inputDevice.equalsClipId(clip->getInputId())) {
                        inputDevice.setId(clip->getInputId());
                        inputDevice.removeInfos();
                        m_inputDeviceList.insert(clip, inputDevice);
                        equals = true;
                        break;
                    }
                }
                if (!equals) {
                    clip->removeInputDevice();
                }
            }
        }
        file.close();
    }
}

bool TimelineModel::removeEmptyTrack(int row) {
    if (row < 0 || row >= m_trackList.size()) {
        return false;
    }
    TimelineTrack *track = m_trackList.at(row);
    if (track->count() == 0) {
        beginRemoveRows(QModelIndex(), row, row);
        m_trackList.removeAt(row);
        delete track;
        endRemoveRows();
        return true;
    }
    return false;
}

bool TimelineModel::tryRemoveTransition(int trackIndex, int clipIndex) {
    if (trackIndex >= m_trackList.size() || trackIndex < 0) {
        return false;
    }
    TimelineTrack *track = m_trackList.at(trackIndex);
    if (clipIndex >= track->count() || clipIndex < 0) {
        return false;
    }

    BaseClip *clip = track->get(clipIndex);
    return tryRemoveClipTransition(trackIndex, clip);
}

bool TimelineModel::tryRemoveClipTransition(int trackIndex, BaseClip *clip) {
    if (clip != nullptr && (clip->clipType() == VideoClipType || clip->clipType() == ImageClipType)) {
        VideoClip *videoClip = (VideoClip *)clip;
        TransitionClip *transitionClip = videoClip->getTransition();
        if (transitionClip != nullptr) {
            emit removeClipData(transitionClip);
            videoClip->removeTransition();
            if (trackIndex >= 0) {
                int clipIndex = -1;
                auto track = m_trackList.at(trackIndex);
                for (int i = 0; i < track->count(); ++i) {
                    if (clip == track->get(i)) {
                        clipIndex = i;
                        break;
                    }
                }
                if (clipIndex >= 0) {
                    notifyDataChanged(trackIndex, clipIndex - 1, clipIndex);
                }
            }
            return true;
        }
    }
    return false;
}

bool TimelineModel::rewindVideoClip(const QModelIndex &index, QString path) {
    BaseClip *clip = data(index);
    if (clip == nullptr || clip->clipType() != VideoClipType) {
        return false;
    }
    VideoClip *videoClip = (VideoClip *)clip;
    if (videoClip->rewindVideoClip(path)) {
        emit dataChanged(index, index);
        return true;
    } else {
        return false;
    }
}

QModelIndex TimelineModel::copyClip(BaseClip *clip) {
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        for (int j = 0; j < track->count(); j++) {
            if (clip == track->get(j)) {
                int newTrackIndex = i + 1;
                TimelineTrack *targetTrack = new TimelineTrack(getCurrentTimeline(), track->type());
                if (targetTrack->moveToIndex(m_trackList.size() - newTrackIndex)) {
                    m_trackList.insert(newTrackIndex, targetTrack);
                }
                QObject::connect(targetTrack, &TimelineTrack::clipDataChanged, this, &TimelineModel::onClipDataChanged);
                auto config = clip->backupClipConfig();
                SkyTimeRange trimRange = clip->skyClip()->getTrimRange();
                SkyTimeRange trackRange = clip->skyClip()->getTrackRange();
                int clipIndex = targetTrack->tryAddResource(clip->resourceBean(), trimRange, trackRange, config);
                beginInsertRows(QModelIndex(), newTrackIndex, newTrackIndex);
                endInsertRows();
                notifyDataChanged(newTrackIndex, -1, -1);
                return makeIndex(newTrackIndex, clipIndex);
            }
        }
    }
    return QModelIndex();
}

QModelIndex TimelineModel::copyClipImage(BaseClip *clip, QString image) {
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        for (int j = 0; j < track->count(); j++) {
            if (clip == track->get(j)) {
                int newTrackIndex = i + 1;
                TimelineTrack *targetTrack = new TimelineTrack(getCurrentTimeline(), track->type());
                if (targetTrack->moveToIndex(m_trackList.size() - newTrackIndex)) {
                    m_trackList.insert(newTrackIndex, targetTrack);
                }
                QObject::connect(targetTrack, &TimelineTrack::clipDataChanged, this, &TimelineModel::onClipDataChanged);
                QFile imageFile(image);
                SkyResourceBean resouce(image, imageFile.fileName(), ImageResource, "");
                int clipIndex = targetTrack->tryAddResource(resouce, SkyTimeRange(0, 2), clip->startTime());
                ThreadPools::instance()->mainThread([this, newTrackIndex] {
                    beginInsertRows(QModelIndex(), newTrackIndex, newTrackIndex);
                    endInsertRows();
                });
                return makeIndex(newTrackIndex, clipIndex);
            }
        }
    }
    return QModelIndex();
}

void TimelineModel::removeBlankTrack() {
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        track->removeBlankTrack();
    }
}

bool TimelineModel::hasClipInTime(BaseClip *clip, double time) {
    for (int i = 0; i < m_trackList.count(); i++) {
        TimelineTrack *track = m_trackList.at(i);
        for (int j = 0; j < track->count(); j++) {
            BaseClip *baseClip = track->get(j);
            if (baseClip != clip && baseClip->startTime() < time && baseClip->endTime() > time) {
                return true;
            }
        }
    }
    return false;
}

bool TimelineModel::addSubEffect(const QModelIndex &index,
                                 QPair<SkyResourceBean, SkyVariant> &addEffect,
                                 QPair<SkyResourceBean, SkyVariant> &deletedEffect) {
    auto resource = addEffect.first;
    auto resourceType = resource.resouceType() == MoreResource ? EffectResource : resource.resouceType();

    if (resourceType != EffectResource && resourceType != FilterResource) {
        return false;
    }
    auto clip = data(index);
    if (clip != nullptr && (clip->clipType() == VideoClipType || clip->clipType() == ImageClipType)) {
        //        auto effectList = clip->findEffectListByType(resourceType);
        //        for (int i = 0; i < effectList.size(); ++i) {
        //            auto deleteResource = effectList[i]->skyResource();
        //            auto deletedParams = effectList[i]->getParams();
        //            if (clip->removeEffect(effectList[i]->skyResource().entryPath())) {
        //                deletedEffect.first = deleteResource;
        //                deletedEffect.second = deletedParams;
        //                qInfo() << "delete Pre Effect " << deleteResource.toString();
        //            }
        //        }

        SkyEffectWrapper *addEffectWrapper = nullptr;
        if (!addEffect.second.isNull()) {
            auto config = addEffect.second;
            addEffectWrapper = clip->addEffectFromConfig(resource, config);
        } else {
            addEffectWrapper = clip->addEffect(resource);
        }
        return addEffectWrapper != nullptr;
    }
    return false;
}

bool TimelineModel::removeSubEffect(const QModelIndex &index, QPair<SkyResourceBean, SkyVariant> &removedEffect) {
    auto clip = data(index);
    if (clip != nullptr) {

        auto wrapper = clip->findEffect(removedEffect.first.entryPath());
        if (wrapper != nullptr) {
            removedEffect.second = wrapper->saveConfig();
            return clip->removeEffect(removedEffect.first.entryPath());
        }
    }
    return false;
}

void TimelineModel::addInputDevice(BaseClip *clip) {
    if (clip->getInputId()) {
        return;
    }
    InputDevice inputDevice = InputDevice::makeInputDevice(clip->resourceBean(), clip->skyClip());
    m_inputDeviceList.insert(clip, inputDevice);
    clip->setInputId(inputDevice.getId());
}

void TimelineModel::updateInputDevice(BaseClip *clip, const InputDevice &inputDevice) {
    if (clip->getInputId()) {
        m_inputDeviceList.insert(clip, inputDevice);
    }
}

void TimelineModel::removeInputDevice(BaseClip *clip) {
    if (m_inputDeviceList.contains(clip)) {
        m_inputDeviceList.remove(clip);
    }
    clip->removeInputDevice();
}

InputDevice TimelineModel::getInputDeviceFromClip(BaseClip *clip) {
    return m_inputDeviceList.value(clip);
}

QMap<BaseClip *, InputDevice> TimelineModel::inputDeviceList() {
    return m_inputDeviceList;
}

void TimelineModel::checkTrackError(int trackIndex, int clipCount) {
    if (trackIndex >= m_trackList.count()) {
        emit watchError(QString() + "track index out of bounds,count:" + QString::number(m_trackList.count()) +
                        ",trackIndex:" + QString::number(trackIndex) + ",clipCount:" + QString::number(clipCount));
        return;
    }
    TimelineTrack *track = m_trackList.at(trackIndex);
    if (track->count() != clipCount) {
        emit watchError(QString() + "different count of clips,track->count():" + QString::number(track->count()) +
                        ",trackIndex:" + QString::number(trackIndex) + ",clipCount:" + QString::number(clipCount));
    }
}
void TimelineModel::checkClipError(int trackIndex, int clipIndex, double startTime, double duration) {
    if (trackIndex >= m_trackList.count()) {
        return;
    }
    TimelineTrack *track = m_trackList.at(trackIndex);
    if (clipIndex >= track->count()) {
        emit watchError(QString() + "clip index out of bounds,count:" + QString::number(track->count()) +
                        ",trackIndex:" + QString::number(trackIndex) + ",clipIndex:" + QString::number(clipIndex));
        return;
    }
    BaseClip *clip = track->get(clipIndex);
    if (abs(clip->startTime() - startTime) > 0.0000001) {
        emit watchError(QString() + "clip startTime error,clip:" + clip->toString() +
                        ",trackIndex:" + QString::number(trackIndex) + ",clipIndex:" + QString::number(clipIndex) +
                        ",startTime:" + QString::number(startTime));
    }
    if (abs(clip->during() - duration) > 0.0000001) {
        emit watchError(QString() + "clip duration error,clip:" + clip->toString() +
                        ",trackIndex:" + QString::number(trackIndex) + ",clipIndex:" + QString::number(clipIndex) +
                        ",duration:" + QString::number(duration));
    }
}

bool TimelineModel::hasVideoClip() {
    for (auto track : m_trackList) {
        if (track->type() == VideoTrack && track->count() > 0) {
            return true;
        }
    }
    return false;
}

void TimelineModel::updateClipResource(int trackIndex,
                                       int clipIndex,
                                       const QString &path,
                                       double startTime,
                                       double endTime,
                                       bool isReuse,
                                       const SkyVariant &config) {
    TimelineTrack *track = m_trackList.at(trackIndex);
    SkyResourceBean resBean(path, "", FileUtils::isVideoExtension(path) ? VideoResource : ImageResource);
    track->updateClipResource(clipIndex, resBean, startTime, endTime, isReuse, config);
    notifyDataChanged(trackIndex, clipIndex, clipIndex);
}
