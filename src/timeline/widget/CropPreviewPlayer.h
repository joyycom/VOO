#include <QImage>
#include <QTimer>
#include <QSize>
#include <QQuickPaintedItem>

#include "timeline/image/SkyImageLoader.h"
#include "timeline/models/baseclip.h"

#define FRAME_TIME 1000 / 30
class CropPreviewPlayer : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(double currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingStateChanged)
public:
    CropPreviewPlayer(QQuickItem *parent = 0);
    ~CropPreviewPlayer();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setCurrentTime(double time);
    Q_INVOKABLE double currentTime();
    Q_INVOKABLE double resourceTime();
    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isPlaying();
    Q_INVOKABLE QString formatTime(double time);

public:
    virtual void paint(QPainter *painter) override;

signals:
    void currentTimeChanged();
    void playingStateChanged();

public slots:
    void onTimerFired();

private:
    void getImage(double time);
    SkyVideoDecoder m_decoder;
    BaseClip *m_clip = nullptr;
    QTimer *m_timer = nullptr;
    QImage m_image;
    bool m_playingState = false;
    bool m_imageFlag = true;
    double m_currentTime = 0;
};
