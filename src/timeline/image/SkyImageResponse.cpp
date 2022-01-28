#include "SkyImageResponse.h"
#include <QDebug>

SkyImageResponse::SkyImageResponse() {
}

QQuickTextureFactory *SkyImageResponse::textureFactory() const {
    return factory;
}

void SkyImageResponse::onSuccess(QImage &image) {
    factory = QQuickTextureFactory::textureFactoryForImage(image);
}

SkyImageResponse::~SkyImageResponse() {
}

void SkyImageResponse::cancel() {
    qDebug() << "SkyImageResponse Cancel!" << this;
    isCanceled = true;
}
