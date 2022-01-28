#include "PreviewController.h"

#include <project/SEProject.h>

#include <QMediaPlayer>

PreviewController::PreviewController(QObject *parent) : QObject(parent) {
    SEProject::current()->dom()->setPreviewController(this);
    qDebug() << "PreviewController::PreviewController" << this << " Dom:" << SEProject::current()->dom();
    notifyFrameConfig();
}

PreviewController::~PreviewController() {
}

void PreviewController::setSkyTimelineView(SkyTimelineView *timelineView) {
    if (m_skyTimelineView != nullptr) {
        disconnect(m_skyTimelineView, SIGNAL(progressUpdated()), this, SLOT(onProgressChanged()));
    }
    m_skyTimelineView = timelineView;
    connect(m_skyTimelineView, SIGNAL(progressUpdated()), this, SLOT(onProgressChanged()));
    connect(m_skyTimelineView, SIGNAL(playEnd()), this, SLOT(onPlayEnd()));
    qDebug() << "setSkyTimelineView:" << timelineView;
    if (m_skyTimelineView != nullptr) {
        m_skyTimelineView->initTimer();
    }
}

void PreviewController::setSkyTimelineView2(SkyTimelineView2 *timelineView) {
    m_skyTimelineView2 = timelineView;
}

void PreviewController::setMiniPlayerView(MiniPlayerView *view) {
    m_miniPlayerView = view;
}

SkyTimelineView *PreviewController::getSkyTimelineView() {
    return m_skyTimelineView;
}

SkyTimelineView2 *PreviewController::getSkyTimelineView2() {
    return m_skyTimelineView2;
}

MiniPlayerView *PreviewController::getMiniPlayerView() {
    return m_miniPlayerView;
}

void PreviewController::play(SkyTimeRange range) {
    if (m_previewType == PreviewTimeline || m_previewType == PreviewResourceEffect) {
        if (m_skyTimelineView == nullptr) {
            return;
        }
        if (range.end > range.begin && range.begin >= 0) {
            m_playRange.begin = range.begin;
            m_playRange.end = range.end;

            seekVideo(range.begin);
        } else {
            m_playRange.begin = 0;
            m_playRange.end = 0;
            if (m_skyTimelineView->isPlayEnd()) {
                seekVideo(0);
            }
        }

        m_skyTimelineView->play();
        emit playingStateChanged(isPlaying());
    } else {
        if (m_skyMediaPlayer == nullptr) {
            return;
        }
        m_skyMediaPlayer->play();
        emit playingStateChanged(isPlaying());
    }
}

void PreviewController::refreshPreview() {
    if (m_skyTimelineView == nullptr) {
        return;
    }
    m_skyTimelineView->refreshView();
}

void PreviewController::stop() {
    if (m_previewType == PreviewTimeline || m_previewType == PreviewResourceEffect) {
        if (m_skyTimelineView == nullptr) {
            return;
        }
        m_playRange.begin = 0;
        m_playRange.end = 0;
        m_skyTimelineView->stop();
        emit playingStateChanged(isPlaying());
    } else {
        if (m_skyMediaPlayer == nullptr) {
            return;
        }
        m_skyMediaPlayer->pause();
        emit playingStateChanged(isPlaying());
    }
}

void PreviewController::mute() {
    if (m_previewType == PreviewTimeline || m_previewType == PreviewResourceEffect) {
        if (m_skyTimelineView == nullptr) {
            return;
        }
        m_skyTimelineView->setMute(true);
    } else {
        if (m_skyMediaPlayer == nullptr) {
            return;
        }
        m_skyMediaPlayer->setMuted(true);
    }
}

void PreviewController::unmute() {
    if (m_previewType == PreviewTimeline || m_previewType == PreviewResourceEffect) {
        if (m_skyTimelineView == nullptr) {
            return;
        }
        m_skyTimelineView->setMute(false);
    } else {
        if (m_skyMediaPlayer == nullptr) {
            return;
        }
        m_skyMediaPlayer->setMuted(false);
    }
}

void PreviewController::seekVideo(double timePts) {
    if (m_skyTimelineView == nullptr || isnan(timePts)) {
        return;
    }
    QTime start = QTime::currentTime();
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    //要往回退一点点
    if (timePts >= timeline->getDuration()) {
        timePts = std::max(0.0, timeline->getDuration() - 0.0000001);
    }
    timeline->seekTo(timePts);
    if (isPlaying()) {
        emit videoProgressChanged(videoProgress());
    } else {
    }
    qDebug() << "seekVideo onTimerFired Spend:" << start.msecsTo(QTime::currentTime());
}

bool PreviewController::isPlaying() const {
    if (m_previewType == PreviewTimeline || m_previewType == PreviewResourceEffect) {
        if (m_skyTimelineView == nullptr) {
            return false;
        }
        return m_skyTimelineView->getPlayState() == SkyPlayState_Playback;
    } else {
        if (m_skyMediaPlayer == nullptr) {
            return false;
        }
        return m_skyMediaPlayer->state() == m_skyMediaPlayer->PlayingState;
    }
}

double PreviewController::videoProgress() const {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (timeline == nullptr || timeline->getDuration() < 0.000001) {
        return 0;
    }
    double currentTime = timeline->getCurrentTime();
    double duration = timeline->getDuration();
    if (currentTime >= duration) {
        SEProject::current()->dom()->previewController()->seekVideo(currentTime);
        currentTime = timeline->getCurrentTime();
    }
    return currentTime / duration;
}

void PreviewController::onProgressChanged() {
    if (m_previewType == PreviewTimeline) {
        SkyTimeline *timeline = SEProject::current()->dom()->timeline();
        if (m_playRange.end > m_playRange.begin && m_playRange.begin >= 0 &&
            timeline->getCurrentTime() > m_playRange.end) {
            stop();
        }
        auto timelineController = SEProject::current()->dom()->timelineController();
        if (!(timelineController && timelineController->getOutputMode())) {
            emit videoProgressChanged(videoProgress());
        }
    } else {
        emit resoucePreviewProgressChanged(previewProgress());
    }
}

void PreviewController::onPlayEnd() {
    stop();
}

double PreviewController::during() const {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (timeline == nullptr) {
        return 0;
    }
    double during = timeline->getDuration();
    return during;
}

QString PreviewController::formatTime(double time) const {
    int hours = time / 3600;
    int seconds = (int)time % 60;
    int min = (time - hours * 3600) / 60;
    int frames = (time - (int)time) * 30;
    return QString("%1:%2:%3:%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(min, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(frames, 2, 10, QChar('0'));
}

void PreviewController::startPreviewResource(SkyTimeRange &range) {
    if (m_skyTimelineView == nullptr) {
        return;
    }

    m_previewTimeRange.begin = range.begin;
    m_previewTimeRange.end = range.end;

    m_previewType = PreviewResourceEffect;
    emit previewTypeChanged(m_previewType);
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    timeline->seekTo(range.begin);
    m_skyTimelineView->playRange(range, 1);
    if (!isPlaying()) {
        m_skyTimelineView->play();
        emit playingStateChanged(isPlaying());
    }

    emit resoucePreviewProgressChanged(previewProgress());
}

void PreviewController::stopPreview() {
    if (m_skyMediaPlayer != nullptr) {
        m_skyMediaPlayer->stop();
    }
    if (m_skyTimelineView != nullptr) {
        SkyTimeRange range(0, -1);
        m_skyTimelineView->playRange(range, 1);
    }
    m_previewType = PreviewTimeline;
    m_skyTimelineView->stop();
    emit previewTypeChanged(m_previewType);
    emit playingStateChanged(isPlaying());
}

int PreviewController::previewType() const {
    return m_previewType;
}

QString PreviewController::previewResourcePath() const {
    return m_previewResourcePath;
}

void PreviewController::startPreviewVideoResource(const SkyResourceBean &bean) {
    if (m_skyTimelineView != nullptr) {
        m_skyTimelineView->stop();
    }
    switch (bean.resouceType()) {
        case ImageResource:
            m_previewType = PreviewResourceImage;
            break;
        case VideoResource:
            m_previewType = PreivewResourceVideo;
            break;
        case AudioResource:
            m_previewType = PreviewResourceAudio;
            break;
        default:
            m_previewType = PreviewResourceEffect;
    }
    m_previewResourcePath = bean.entryPath();
    if (m_previewType == PreivewResourceVideo || m_previewType == PreviewResourceAudio) {
        m_skyMediaPlayer->setMedia(QUrl::fromLocalFile(bean.entryPath()));
        m_skyMediaPlayer->play();
    } else {
    }

    emit previewTypeChanged(m_previewType);
    emit videoProgressChanged(videoProgress());
}

void PreviewController::attachMediaPlayer(QObject *mediaPlayer, QObject *resoucePreview) {
    qDebug() << "attachMediaPlayer:" << mediaPlayer << " resoucePreview: " << resoucePreview;
    m_skyMediaPlayer = (SkyMediaPlayer *)mediaPlayer;
    // QUndoStack::redoTextChanged
}

//"1:1 1080*1080", "16:9 1920*1080", "9:16 1080*1920", "4:3 1440*1080", "3:4 1080*1440"
QVariantList PreviewController::changeFrame(int frameIndex) {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    SkyVideoParams params = timeline->getVideoParams();
    switch (frameIndex) {
        case 0:
            params.width = 1080;
            params.height = 1080;
            break;
        case 1:
            params.width = 1920;
            params.height = 1080;
            break;
        case 2:
            params.width = 1080;
            params.height = 1920;
            break;
        case 3:
            params.width = 1440;
            params.height = 1080;
            break;
        case 4:
            params.width = 1080;
            params.height = 1440;
            break;
        default:
            break;
    }
    timeline->setVideoParams(params);
    SEProject::current()->dom()->propertyEditController()->updateUI();
    QVariantList retValue;
    retValue.append(params.width);
    retValue.append(params.height);
    return retValue;
}

QVariantList PreviewController::getFrameConfig() {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    SkyVideoParams params = timeline->getVideoParams();
    int width = params.width;
    int height = params.height;
    int fps = params.frameRate;
    int selectIndex = 1;
    int fpsIndex = 2;
    if (width == 1080 && height == 1080) {
        selectIndex = 0;
    } else if (width == 1920 && height == 1080) {
        selectIndex = 1;
    } else if (width == 1080 && height == 1920) {
        selectIndex = 2;
    } else if (width == 1440 && height == 1080) {
        selectIndex = 3;
    } else if (width == 1080 && height == 1440) {
        selectIndex = 4;
    } else {
        selectIndex = -1;
    }
    if (fps == 15) {
        fpsIndex = 0;
    } else if (fps == 24) {
        fpsIndex = 1;
    } else if (fps == 30) {
        fpsIndex = 2;
    } else if (fps == 60) {
        fpsIndex = 3;
    }
    QVariantList retValue;
    retValue.append(selectIndex);
    retValue.append(fpsIndex);
    retValue.append(width);
    retValue.append(height);
    return retValue;
}

double PreviewController::changeVideoConfig(int width, int height, int fpsIndex) {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    SkyVideoParams params = timeline->getVideoParams();
    params.width = (width >> 1) << 1;
    params.height = (height >> 1) << 1;
    switch (fpsIndex) {
        case 0:
            params.frameRate = 15;
            break;
        case 1:
            params.frameRate = 24;
            break;
        case 2:
            params.frameRate = 30;
            break;
        case 3:
            params.frameRate = 60;
            break;
        default:
            params.frameRate = 30;
            break;
    }
    timeline->setVideoParams(params);
    SEProject::current()->dom()->propertyEditController()->updateUI();
    emit SEProject::current()->dom()->timelineController()->fpsChanged();
    return (double)params.width / params.height;
}

void PreviewController::notifyFrameConfig() {
    emit frameConfig();
}

void PreviewController::setFrameConfig(int width, int height) {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    SkyVideoParams params = timeline->getVideoParams();
    params.width = (width >> 1) << 1;
    params.height = (height >> 1) << 1;
    timeline->setVideoParams(params);
    SEProject::current()->dom()->propertyEditController()->updateUI();
    emit SEProject::current()->dom()->timelineController()->fpsChanged();
    notifyFrameConfig();
}

// void PreviewController::onMediaPlayerPlayStateChanged(int state){
//    if(state == QMediaPlayer::StoppedState && m_skyMediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia){
//        m_skyMediaPlayer->setPosition(0);
//        m_skyMediaPlayer->pause();
//        qDebug()<<"onMediaPlayerPlayStateChanged ";
//    }else{
//        qDebug()<<"onMediaPlayerPlayStateChanged State:"<<state << "  mediaStatus:"<<m_skyMediaPlayer->mediaStatus();
//        emit playingStateChanged(isPlaying());
//    }
//}

double PreviewController::previewProgress() const {
    if (m_previewType == PreviewResourceEffect) {
        if (m_previewTimeRange.end < m_previewTimeRange.begin) {
            return videoProgress();
        }
        SkyTimeline *timeline = SEProject::current()->dom()->timeline();
        double current = timeline->getCurrentTime();
        //修复当前时间小于预览开始时间，出现负数的情况
        if (current < m_previewTimeRange.begin) {
            timeline->seekTo(m_previewTimeRange.begin);
            current = timeline->getCurrentTime();
        }
        return (current - m_previewTimeRange.begin) / (m_previewTimeRange.end - m_previewTimeRange.begin);
    } else if (m_previewType == PreivewResourceVideo || m_previewType == PreviewResourceAudio) {
        if (m_skyMediaPlayer != nullptr && m_skyMediaPlayer->duration() > 0) {
            double position = fmin(m_skyMediaPlayer->position(), m_skyMediaPlayer->duration());
            return position / (double)m_skyMediaPlayer->duration();
        }
    }
    return 0;
}

double PreviewController::previewDuring() const {
    if (m_previewType == PreviewResourceEffect) {
        if (m_previewTimeRange.end < m_previewTimeRange.begin) {
            return during();
        } else {
            return m_previewTimeRange.end - m_previewTimeRange.begin;
        }
    } else if (m_previewType == PreivewResourceVideo || m_previewType == PreviewResourceAudio) {
        if (m_skyMediaPlayer != nullptr) {
            return m_skyMediaPlayer->duration() / 1000.0;
        }
    }
    return 0;
}
