#include "imagerequest.h"
#include "base/utils/AppConstance.h"
#include <QDebug>
#include <QTime>

ImageRequest::ImageRequest(double key,
                           QString path,
                           QString imagePath,
                           double time,
                           QSize size,
                           SkyImageResponse *response,
                           SkyVideoDecoder &decorder)
    : m_key(key),
      m_resPath(path),
      m_imagePath(imagePath),
      m_time(time),
      m_size(size),
      m_response(response),
      m_decoder(decorder) {
    qDebug() << "ImageRequest INIT m_resPath:" << m_resPath << " m_imagePath:" << m_imagePath << " m_time:" << m_time;
}

ImageRequest::ImageRequest(const ImageRequest &rhs)
    : m_key(rhs.m_key),
      m_resPath(rhs.m_resPath),
      m_imagePath(rhs.m_imagePath),
      m_time(rhs.m_time),
      m_size(rhs.m_size),
      m_response(rhs.m_response),
      m_decoder(rhs.m_decoder) {
}

bool ImageRequest::operator==(const ImageRequest &rhs) {
    return abs(m_key - rhs.m_key) < TIME_DEVIATION && m_resPath == rhs.m_resPath && m_imagePath == rhs.m_imagePath &&
           abs(m_time - rhs.m_time) < TIME_DEVIATION;
}

bool ImageRequest::operator<(const ImageRequest &rhs) const {
    return m_time < rhs.m_time;
}

bool ImageRequest::loadImageFromCache() {
    QTime start = QTime::currentTime();
    QImage image(m_imagePath);
    if (image.isNull()) {
        qDebug() << "loadImageFromCache failed:" << m_imagePath;
        return false;
    }
    m_response->onSuccess(image);
    emit m_response->finished();
    m_response = nullptr;
    qDebug() << "loadImageFromCache Success:" << m_imagePath << " Spend:" << start.msecsTo(QTime::currentTime());
    return true;
}

QImage ImageRequest::decodeImageFromVideo() {
    QTime start = QTime::currentTime();
    SkyVideoFrame frame = m_decoder.retrieveVideo(m_time, SkyRetrieveMode_Accurate);
    if (frame.isGPUTexture() || frame.getWidth() == 0) {
        emit m_response->finished();
        m_response = nullptr;
        qDebug() << "decodeImageFromVideo failed GPUTexture:" << m_imagePath;
        return QImage();
    }
    QImage image(frame.getRGBA8Data(), frame.getWidth(), frame.getHeight(), QImage::Format_RGBA8888);
    double rate = fmin(image.width() / m_size.width(), image.height() / m_size.height());
    if(rate < 0.001 || image.isNull()){
        return QImage();
    }
    QImage scaleImage = image.scaled(image.width() / rate, image.height() / rate);
    m_response->onSuccess(scaleImage);
    emit m_response->finished();
    m_response = nullptr;
    qDebug() << "decodeImageFromVideo Success:" << m_imagePath << " Spend:" << start.msecsTo(QTime::currentTime());
    return scaleImage;
}

void ImageRequest::saveImageToCache(const QImage &image) const {
    if (image.isNull()) {
        return;
    }
    QTime start = QTime::currentTime();
    bool save = image.save(m_imagePath, nullptr, 95);
    qDebug() << "saveImageToCache:" << m_imagePath << " " << save << " Spend:" << start.msecsTo(QTime::currentTime());
}

void ImageRequest::cancel() {
    emit m_response->finished();
    m_response = nullptr;
}

QString ImageRequest::imagePath() const {
    return m_imagePath;
}

double ImageRequest::key() const {
    return m_key;
}

QString ImageRequest::resPath() const {
    return m_resPath;
}

double ImageRequest::time() const {
    return m_time;
}
