#ifndef SKYIMAGELOADER_H
#define SKYIMAGELOADER_H

#include "imagerequest.h"
#include "SkyImageResponse.h"

#include <QMap>
#include <QQuickTextureFactory>
#include <QSize>
#include <QString>
#include <QThread>
#include <QThreadPool>
#include <QMutex>
#include <set>
#include <QSet>
#include <set>
#include <QList>

#include <skymedia/SkyTimeline.h>
#include <skymedia/SkyResource.h>
#include <skymedia/SkyVideoDecoder.h>

SKYMEDIA_USING_NAMESPACE

class VideoCoverLoader : public QObject {
    Q_OBJECT
public:
    VideoCoverLoader(QString path);
    ~VideoCoverLoader();

    SkyImageResponse *loadImage(double time, const QSize &size, double key);

private:
signals:
    void notifyDecodeImage();
private slots:
    void profromDecodeImage();

private:
    bool ensureVideoDecoder();

    QList<ImageRequest> mWaitingRequest;

    ImageRequest *mDecodingRequest = nullptr;
    QMutex mListLock;
    SkyTimeline* m_timeline;
    QThread *m_decodeThread = new QThread();
    QString m_path;
    SkyResource m_resource;
    SkyVideoDecoder m_decoder;
    SkyVideoStream m_videoStream;
    QString mThumbDirPath;
    double m_curRequestKey = 0;
};

#endif // SKYIMAGELOADER_H
