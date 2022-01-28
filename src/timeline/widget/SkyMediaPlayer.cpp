#include "SkyMediaPlayer.h"

SkyMediaPlayer::SkyMediaPlayer(QObject *parent, Flags flags) : QMediaPlayer(parent, flags) {
}

void SkyMediaPlayer::setVideoSurface(QAbstractVideoSurface *surface) {
    qDebug() << "Changing surface:" << surface;
    m_surface = (QAbstractVideoSurface *)surface;
    setVideoOutput(m_surface);
}

QAbstractVideoSurface *SkyMediaPlayer::getVideoSurface() {
    return m_surface;
}
