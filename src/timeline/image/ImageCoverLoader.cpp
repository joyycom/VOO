#include "ImageCoverLoader.h"
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QDir>
#include <base/utils/AppPath.h>
#include <QDebug>

ImageCoverLoader::ImageCoverLoader() {
}

SkyImageResponse *ImageCoverLoader::loadImage(QString path, QSize &size) {
    SkyImageResponse *response = new SkyImageResponse();
    QString md5 = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5).toHex();
    //    QString cachePath = AppPath::;
    QString cachePath = AppPath::thumbsPath(md5);
    QDir cacheDir(cachePath);
    cacheDir.mkpath(cachePath);

    QString imagePath =
        QString("%1/%2_%3_%4.jpg").arg(cachePath).arg(0, 5, 10, QLatin1Char('0')).arg(size.width()).arg(size.height());
    if (QFile(imagePath).exists()) {
        QImage image(imagePath);
        response->onSuccess(image);
        qDebug() << "loadImage status: " << image.isNull();
    } else {
        if (!QFile(path).exists()) {
            response->cancel();
            qWarning() << "InValid Path: " << path;
            return response;
        }
        QImage image(path,"jpg,png,webp,bmp");
        auto rate = fmin(image.width() * 1.0f / size.width(), image.height() * 1.0f / size.height());
        if (rate <= 0) {
            response->cancel();
            qWarning() << "InValid Path: " << path;
            return response;
        }
        QImage scaleImage = image.scaled(image.width() / rate, image.height() / rate);
        scaleImage.save(imagePath, nullptr, 95);
        response->onSuccess(scaleImage);
    }
    emit response->finished();
    qDebug() << "loadImage Done?: " << path;
    return response;
}
