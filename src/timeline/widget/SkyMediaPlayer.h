#ifndef SKYMEDIAPLAYER_H
#define SKYMEDIAPLAYER_H

#include <QMediaPlayer>

class SkyMediaPlayer : public QMediaPlayer {
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ getVideoSurface WRITE setVideoSurface)
public:
    SkyMediaPlayer(QObject *parent = 0, Flags flags = 0);

public slots:
    void setVideoSurface(QAbstractVideoSurface *surface);
    QAbstractVideoSurface *getVideoSurface();

public:
signals:

private:
    QAbstractVideoSurface *m_surface;
};

#endif // SKYMEDIAPLAYER_H
