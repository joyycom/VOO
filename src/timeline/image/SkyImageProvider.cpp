#include "SkyImageProvider.h"
#include "project/SEProject.h"
#include <QDebug>
#include <QDir>
#include <project/SEProject.h>
#include <QCryptographicHash>
#include <base/utils/AppPath.h>

class DecodeImageResponse : public QQuickImageResponse {
public:
    virtual QQuickTextureFactory *textureFactory() const override {
        if (image.isNull()) {
            return nullptr;
        } else {
            return QQuickTextureFactory::textureFactoryForImage(image);
        }
    }
    QSize imageSize;
    QImage image;
    QString cachePath;
    SkyAsyncTask task;

public Q_SLOTS:
    void cancel() override {
        task.cancel();
    }
};

struct ImageResponseCallback {
    static void onFinish(void *userData, SkyVideoFrame frame, SkyFrameOrigin origin) {
        DecodeImageResponse* response = (DecodeImageResponse*)userData;
        if (origin == SkyFrameOrigin_FromDecoder) {
            QSize imageSize = response->imageSize;
            QImage image(frame.getRGBA8Data(), frame.getWidth(), frame.getHeight(), QImage::Format_RGBA8888);
            double rate = fmin(image.width() / imageSize.width(), image.height() / imageSize.height());
            QImage scaleImage = image.scaled(image.width() / rate, image.height() / rate);
            scaleImage.save(response->cachePath, nullptr, 95);
            
            response->image = scaleImage;
            emit response->finished();
        } else if (origin == SkyFrameOrigin_FromCache) {
            // do nothing
        }
    }

    static SkyVideoFrame onFetchFromCaches(void *userData) {
        DecodeImageResponse* imageResponse = (DecodeImageResponse*)userData;
        QImage image(imageResponse->cachePath);
        if (image.isNull()) {
            return SkyVideoFrame();
        } else {
            imageResponse->image = image;
            emit imageResponse->finished();
            
            uint8_t color[4] = {0};
            return SkyVideoFrame::makeRGBA(color, 1, 1);
        }
    }

    static void onError(void *userData, SkyError error) {
        DecodeImageResponse* imageResponse = (DecodeImageResponse*)userData;
        emit imageResponse->finished();
    }
};


SkyImageProvider::SkyImageProvider() {
    SEProject::current()->setImageLoader(this);
}

SkyImageProvider::DecodeInfo SkyImageProvider::findDecodeInfo(const QString& path) {
    std::lock_guard<QMutex> locker(mLock);
    if (m_videoDecodeInfos.contains(path)) {
        return m_videoDecodeInfos[path];
    } else {
        SkyResource resource(path.toUtf8().data());
        QString md5 = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5).toHex();
        QString thumbDirPath = AppPath::thumbsPath(md5);
        
        QDir cacheDir(thumbDirPath);
        cacheDir.mkpath(thumbDirPath);
        
        DecodeInfo decodeInfo;
        decodeInfo.resource = resource;
        decodeInfo.thumbDirPath = thumbDirPath;
        
        m_videoDecodeInfos.insert(path, decodeInfo);
        return decodeInfo;
    }
}

QQuickImageResponse *SkyImageProvider::doRequestImage(const QString& path, double time, const QSize &size) {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    DecodeInfo decodeInfo = findDecodeInfo(path);
    SkyResource resource = decodeInfo.resource;
    
    DecodeImageResponse* imageResponse = new DecodeImageResponse();
    imageResponse->imageSize = size;
    
    if (timeline == nullptr || !resource.hasVideoStream()) {
        emit imageResponse->finished();
        return imageResponse;
    }
    
    SkyVideoStream videoStream = resource.firstVideoStream();
    int frameIndex = (int)(videoStream.frameRate * time + 0.5);
    
    QString thumbDirPath = decodeInfo.thumbDirPath;
    QString cachePath = QString("%1/%2_%3_%4.jpg")
            .arg(thumbDirPath)
            .arg(frameIndex, 5, 10, QLatin1Char('0'))
            .arg(size.width())
            .arg(size.height());
    
    QImage image(cachePath);
    if (image.isNull()) {
        imageResponse->cachePath = cachePath;
        SkyDecodeCallback callback;
        callback.userData = imageResponse;
        callback.onFinish = ImageResponseCallback::onFinish;
        callback.onFetchFromCaches = ImageResponseCallback::onFetchFromCaches;
        callback.onError = ImageResponseCallback::onError;
        imageResponse->task = timeline->retrieveVideoAsync(resource, time, SkyRetrieveMode_Accurate, callback);
    } else {
        imageResponse->image = image;
        emit imageResponse->finished();
    }
    return imageResponse;
}

QQuickImageResponse *SkyImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    QString url = QUrl::fromPercentEncoding(id.toUtf8());
    int queryIndex = url.lastIndexOf("?");
    QString path = url.left(queryIndex);
    if (!QFile::exists(path)) {
        SkyImageResponse* response = new SkyImageResponse();
        emit response->finished();
        return response;
    }
    
    QString query = url.right(url.length() - queryIndex - 1);
    QStringList paramLists = query.split("&");
    QMap<QString, QString> params;
    for (QString p : paramLists) {
        QStringList keyValues = p.split("=");
        if (keyValues.length() == 2) {
            params.insert(keyValues[0], keyValues[1]);
        }
    }

    double time = params.value("time", 0).toDouble();
    int type = params.value("type", 0).toInt();
    int width = params.value("width", 0).toInt();
    int height = params.value("height", 0).toInt();
    int track = params.value("track", 0).toInt();
    int clip = params.value("clip", 0).toInt();
    double speed = params.value("speed", 0).toDouble();
    double curvedSpeed = params.value("curvedSpeed", 0).toDouble();
    double frameInterval = params.value("frameInterval", 0).toDouble();


    qDebug() << "requestImageResponse Query:" << query;
    QSize size(width, height);
    if (type == 2) {
        return m_imageLoader.loadImage(path, size);
    } else if (type == 1) {
        double targetTime = time;
        if (curvedSpeed > 0) {
            targetTime = curvedSpeed * time;
            auto timelineController = SEProject::current()->dom()->timelineController();
            auto timelineModel = timelineController->timelineModel();
            if (timelineModel != nullptr) {
                BaseClip *c = timelineModel->data(timelineModel->makeIndex(track, clip));
                if (c != nullptr) {
                    auto timelineStamp = time + c->startTime() - c->cropStartTime() / curvedSpeed;
                    if (timelineStamp >= c->startTime() && timelineStamp < c->endTime()) {
                        targetTime = c->getSourcePTS(timelineStamp);
                        qDebug() << " getSourcePTS1 time:" << time << " -> timelineStamp:" << timelineStamp
                                 << " ---> targetTime:" << targetTime << " CLip:" << c->toString()
                                 << " Source start:"<<c->getSourcePTS(c->startTime())<<" Source end:"<<c->getSourcePTS(c->endTime())
                                 << " Play start:"<<c->getPlayPTS(c->cropStartTime())<<" end:"<<c->getPlayPTS(c->cropEndTime());
                    } else if (timelineStamp < c->startTime()) {
                        auto preOffset = (c->startTime() - timelineStamp)/(c->cropEndTime() / curvedSpeed - (c->endTime() - c->startTime()));
                        targetTime = (1-preOffset)*c->cropStartTime();
                        qDebug() << " getSourcePTS2 time:" << time << " -> timelineStamp:" << timelineStamp
                                 << " ---> targetTime:" << targetTime << " CLip:" << c->toString() <<" preOffset:"<< preOffset;
                    } else {
                        auto offset = (timelineStamp - c->endTime()) / ((c->sourceDuring() - c->cropStartTime()) / curvedSpeed - (c->endTime() - c->startTime()));
                        targetTime = c->cropEndTime() + offset*(c->sourceDuring() - c->cropEndTime());
                        qDebug() << " getSourcePTS3 time:" << time << " -> timelineStamp:" << timelineStamp
                                 << " ---> targetTime:" << targetTime << " CLip:" << c->toString() <<" offset:"<< offset;
                    }

                } else {
                    qDebug() << " Clip isNULL ?? " << track << " ---> " << clip;
                }
            }
        } else if (speed > 0) {
            targetTime = speed * time;
        }
        return doRequestImage(path, targetTime, size);
    }
}

void SkyImageProvider::clear() {
    std::lock_guard<QMutex> locker(mLock);
    m_videoDecodeInfos.clear();
}
