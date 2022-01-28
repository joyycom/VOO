#pragma once

#include "SkyImageResponse.h"
#include <skymedia/SkyVideoDecoder.h>
#include <QTime>

SKYMEDIA_USING_NAMESPACE

class ImageRequest final {
public:
    ImageRequest(double key,
                 QString path,
                 QString imagePath,
                 double time,
                 QSize size,
                 SkyImageResponse *response,
                 SkyVideoDecoder &decorder);
    ImageRequest(const ImageRequest &request);

    bool loadImageFromCache();

    QImage decodeImageFromVideo();

    void saveImageToCache(const QImage &image) const;

    void cancel();


    QString imagePath() const;


    double key() const;

    QString resPath() const;

    double time() const;

    QTime addTime() const;

    bool operator==(const ImageRequest &rhs);

    bool operator<(const ImageRequest &rhs) const;

private:
    static const int HIDE_DELAY_TIME = 3600;
    double m_key;
    QString m_resPath;
    QString m_imagePath;
    double m_time;
    QSize m_size;
    SkyImageResponse *m_response;
    SkyVideoDecoder m_decoder;
};

