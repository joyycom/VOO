#include "CropPreviewPlayer.h"
#include <QPainter>

#include "project/SEProject.h"
#include "base/utils/threadpools.h"

CropPreviewPlayer::CropPreviewPlayer(QQuickItem *parent) {
    m_timer = new QTimer(nullptr);
    m_timer->setInterval(FRAME_TIME);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerFired()));
    auto timelineController = SEProject::current()->dom()->timelineController();
    m_clip = timelineController->curSelectedClip();
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    m_decoder = timeline->openVideoDecoder(m_clip->resourceBean().skyResouce());
    if (timelineController->cursorTime() < m_clip->startTime() ||
        timelineController->cursorTime() > m_clip->endTime()) {
        setCurrentTime(0);
    } else {
        setCurrentTime(timelineController->cursorTime() - m_clip->startTime());
    }
}

CropPreviewPlayer::~CropPreviewPlayer() {
    if (m_timer != nullptr) {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = nullptr;
    }
}

void CropPreviewPlayer::reset() {
    stop();
    m_imageFlag = true;
    auto timelineController = SEProject::current()->dom()->timelineController();
    if (timelineController->cursorTime() < m_clip->startTime() ||
        timelineController->cursorTime() > m_clip->endTime()) {
        setCurrentTime(0);
    } else {
        setCurrentTime(timelineController->cursorTime() - m_clip->startTime());
    }
}

void CropPreviewPlayer::setCurrentTime(double time) {
    m_currentTime = time;
    emit currentTimeChanged();
    getImage(m_currentTime);
}

double CropPreviewPlayer::currentTime() {
    return m_currentTime;
}

double CropPreviewPlayer::resourceTime() {
    return m_clip->during();
}

void CropPreviewPlayer::play() {
    if (m_currentTime >= resourceTime()) {
        m_currentTime = 0;
    }
    m_timer->start();
    m_playingState = true;
    emit playingStateChanged();
}

void CropPreviewPlayer::stop() {
    m_timer->stop();
    m_playingState = false;
    emit playingStateChanged();
}

bool CropPreviewPlayer::isPlaying() {
    return m_playingState;
}

void CropPreviewPlayer::paint(QPainter *painter) {
    if (!isVisible() || m_image.isNull()) {
        return;
    }
    painter->drawImage(this->boundingRect(), m_image);
}

void CropPreviewPlayer::onTimerFired() {
    if (m_currentTime >= resourceTime()) {
        stop();
        return;
    }
    getImage(m_currentTime);
    m_currentTime += (double)FRAME_TIME / 1000;
    emit currentTimeChanged();
}

void CropPreviewPlayer::getImage(double time) {
    if (!m_imageFlag) {
        return;
    }
    m_imageFlag = false;
    ThreadPools::instance()->exec([this, time] {
        SkyVideoFrame frame = m_decoder.retrieveVideo(time + m_clip->cropStartTime(), SkyRetrieveMode_Accurate);
        if (frame.isGPUTexture() || frame.getWidth() == 0) {
            m_imageFlag = true;
            return;
        }
        QImage image(frame.getRGBA8Data(), frame.getWidth(), frame.getHeight(), QImage::Format_RGBA8888);
        if (image.isNull()) {
            m_imageFlag = true;
            return;
        }
        ThreadPools::instance()->mainThread([=] {
            m_image = image;
            update();
            m_imageFlag = true;
        });
    });
}

QString CropPreviewPlayer::formatTime(double time) {
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
