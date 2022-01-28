#include <QImage>
#include <QTimer>
#include <QSize>
#include <QQuickPaintedItem>

#include "timeline/image/SkyImageLoader.h"
#include "timeline/models/baseclip.h"

class UpdateResourcePreviewPlayer : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString path WRITE setPath)
    Q_PROPERTY(double currentTime READ currentTime NOTIFY currentTimeChanged)
public:
    UpdateResourcePreviewPlayer(QQuickItem *parent = 0);
    ~UpdateResourcePreviewPlayer();
    Q_INVOKABLE double currentTime();
    Q_INVOKABLE void setTime(double currentTime, double startTime, double endTime);
    Q_INVOKABLE void setPath(const QString &path);
    Q_INVOKABLE QString formatTime(double time);
    Q_INVOKABLE void updateImage();

public:
    virtual void paint(QPainter *painter) override;

signals:
    void currentTimeChanged();

private:
    void getImage(double time, bool force = false);
    SkyVideoDecoder m_decoder;
    QImage m_image;
    bool m_playingState = false;
    bool m_imageFlag = true;
    double m_currentTime = 0;
    double m_startTime = 0;
    double m_endTime = 0;
    double m_maxTime = 0;
};
