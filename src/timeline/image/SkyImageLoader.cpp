#include "SkyImageLoader.h"
#include "SkyImageResponse.h"

#include <project/SEProject.h>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QDir>
#include <base/utils/AppPath.h>
#include <QApplication>
#include "base/utils/AppConstance.h"

VideoCoverLoader::VideoCoverLoader(QString path) : m_path(path) {
    m_decodeThread->setObjectName("VideoCoverLoader-"+path);
    m_decodeThread->start();
    connect(m_decodeThread, &QThread::finished, m_decodeThread, &QObject::deleteLater);
    qDebug() << "VideoCoverLoader: Init " << this;
    moveToThread(m_decodeThread);
    connect(this, SIGNAL(notifyDecodeImage()), this, SLOT(profromDecodeImage()));
    m_resource = SkyResource(path.toUtf8().data());
    m_videoStream = m_resource.firstVideoStream();

    QString md5 = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5).toHex();
    mThumbDirPath = AppPath::thumbsPath(md5);
    QDir cacheDir(mThumbDirPath);
    cacheDir.mkpath(mThumbDirPath);

    if (!ensureVideoDecoder()) {
        qWarning() << "ensureVideoDecoder, Obtain Video Encoder Failed!";
        return;
    }


    qDebug() << "VideoCoverLoader Path:" << path << " SaveDir:" << mThumbDirPath
             << " Video During:" << m_videoStream.duration<<"m_decodeThread:"<<m_decodeThread->isFinished() <<"  VideoCover:"<< this;
}

VideoCoverLoader::~VideoCoverLoader() {
    disconnect(this, SIGNAL(notifyDecodeImage()), this, SLOT(profromDecodeImage()));

    mListLock.lock();
    mWaitingRequest.clear();
    mListLock.unlock();
    mDecodingRequest = nullptr;
    if (m_decodeThread) {
        m_decodeThread->quit();
        m_decodeThread = nullptr;
    }

    qDebug() << "Destroy VideoCoverLoader Done:" << this;
}

bool imageRequestLessThan(const ImageRequest &v1,const ImageRequest &v2) {
    return v1.time() < v2.time();
}

SkyImageResponse *VideoCoverLoader::loadImage(double time, const QSize &size, double key) {
    if (m_videoStream.streamIndex == -1 || time > m_videoStream.duration || time < 0 ||  isnan(time)) {
        qWarning() << "loadImage Failed! Stream InValid Time:" << time << " Path:" << m_path;
        return new SkyImageResponse();
    }

    int frameIndex = (int)(m_videoStream.frameRate * time + 0.5);

    QString imagePath = QString("%1/%2_%3_%4.jpg")
            .arg(mThumbDirPath)
            .arg(frameIndex, 5, 10, QLatin1Char('0'))
            .arg(size.width())
            .arg(size.height());

    SkyImageResponse *response = nullptr;
    if (QFile(imagePath).exists()) {
        response = new SkyImageResponse();
        ImageRequest request(key, m_path, imagePath, time, size, response, m_decoder);
        if(request.loadImageFromCache()){
            return response;
        }
    }
    qDebug() << "VideoCoverLoader: loadImage " << this << " mThumbDirPath:"<<mThumbDirPath;
    if (!ensureVideoDecoder()) {
        qWarning() << "loadImage Failed! Obtain Video Encoder Failed!";
        return new SkyImageResponse();
    }
    if (!mListLock.tryLock(100)) {
        qWarning() << "tryLock List Failed! Skip!";
        return new SkyImageResponse();
    }
    if(mDecodingRequest == nullptr && mWaitingRequest.size() > 0){
        qInfo()<<" Decode Thread: isFinished"<<m_decodeThread->isFinished()<<" isRunning:"<<m_decodeThread->isRunning()<<" Tasks:"<<m_decodeThread;
    }
    if (abs(m_curRequestKey - key) > FLOAT_DEVIATION) {
        qWarning() << "Remove Pre Tasks Scale:" << m_curRequestKey << " -> " << key
                   << " Deleted:" << mWaitingRequest.size();
        m_curRequestKey = key;
        for (ImageRequest req : mWaitingRequest) {
            req.cancel();
        }
        mWaitingRequest.clear();
    }

    response = new SkyImageResponse();

    ImageRequest request(key, m_path, imagePath, time, size, response, m_decoder);

    mWaitingRequest.append(request);

    qSort(mWaitingRequest.begin(), mWaitingRequest.end(), imageRequestLessThan);

    mListLock.unlock();
    emit notifyDecodeImage();

    return response;
}

bool VideoCoverLoader::ensureVideoDecoder() {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (m_decoder.isAvailable() && timeline == m_timeline) {
        return true;
    }

    if (timeline) {
        m_decoder = timeline->openVideoDecoder(m_resource);
        m_timeline = timeline;
        qDebug()<<"openVideoDecoder "<<&m_decoder<<" isAvailable "<<m_decoder.isAvailable() << " URL:"<<m_resource.getPath().c_str()<<" ImageLoader:"<<this;
        return m_decoder.isAvailable();
    }
    return false;
}

void VideoCoverLoader::profromDecodeImage() {
    while (true) {
        mListLock.lock();
        if (mWaitingRequest.size() > 0) {
            ImageRequest request = mWaitingRequest.first();

            mWaitingRequest.removeFirst();

            mListLock.unlock();
            
            mDecodingRequest = &request;

            if (QFile(request.imagePath()).exists()) {
                if(request.loadImageFromCache()){
                    continue;
                }
            }
            QImage image = request.decodeImageFromVideo();
            request.saveImageToCache(image);
            mDecodingRequest = nullptr;
        } else {
            mListLock.unlock();
            break;
        }
    }
    qDebug() << "profromDecodeImage Exit:" << this;
}
