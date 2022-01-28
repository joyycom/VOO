#include "UpdateResourcePreviewPlayer.h"
#include <QPainter>

#include "project/SEProject.h"
#include "base/utils/threadpools.h"

UpdateResourcePreviewPlayer::UpdateResourcePreviewPlayer(QQuickItem *parent) {
}

UpdateResourcePreviewPlayer::~UpdateResourcePreviewPlayer() {
}

double UpdateResourcePreviewPlayer::currentTime() {
    return m_currentTime;
}

void UpdateResourcePreviewPlayer::setTime(double currentTime, double startTime, double endTime) {
    m_startTime = startTime;
    if (endTime > m_maxTime) {
        m_endTime = m_maxTime;
    } else {
        m_endTime = endTime;
    }
    if (currentTime > m_endTime) {
        m_currentTime = m_endTime;
    } else {
        m_currentTime = currentTime;
    }
    emit currentTimeChanged();
    getImage(m_currentTime);
}

void UpdateResourcePreviewPlayer::setPath(const QString &path) {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    SkyResourceBean resBean(path, "", VideoResource);
    auto resouce = resBean.skyResouce();
    m_maxTime = resouce.getDuration() - 0.0000001;
    m_decoder = timeline->openVideoDecoder(resouce);
}

void UpdateResourcePreviewPlayer::paint(QPainter *painter) {
    if (!isVisible() || m_image.isNull()) {
        return;
    }
    QRectF rect = this->boundingRect();
    int width = m_image.width();
    int height = m_image.height();
    if (width > height) {
        height /= width / rect.width();
        width = rect.width();
    } else {
        width /= height / rect.height();
        height = rect.height();
    }
    painter->drawImage(QRectF((rect.width() - width) / 2, (rect.height() - height) / 2, width, height), m_image);
}

void UpdateResourcePreviewPlayer::getImage(double time, bool force) {
    if (!m_imageFlag && !force) {
        return;
    }
    m_imageFlag = false;
    ThreadPools::instance()->exec([this, time] {
        SkyVideoFrame frame = m_decoder.retrieveVideo(time, SkyRetrieveMode_Accurate);
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

QString UpdateResourcePreviewPlayer::formatTime(double time) {
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

void UpdateResourcePreviewPlayer::updateImage() {
    getImage(std::min(m_currentTime, m_endTime), true);
}
