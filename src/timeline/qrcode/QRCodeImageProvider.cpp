#include "QRCodeImageProvider.h"
#include <QDebug>
#include <QObject>
#include <project/SEProject.h>
#include <src/base/utils/AppPath.h>
#include <src/timeline/image/SkyImageResponse.h>
#include <string>
#include <vector>
#include "QrCode.hpp"

using namespace qrcodegen;
QRCodeImageProvider::QRCodeImageProvider() {
}

QQuickImageResponse *QRCodeImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    QString url = QUrl::fromPercentEncoding(id.toUtf8());
    int queryIndex = url.lastIndexOf("?");
    QString path = url.left(queryIndex);
    QString query = url.right(url.length() - queryIndex - 1);
    QStringList paramLists = query.split("&");
    QMap<QString, QString> params;
    for (QString p : paramLists) {
        QStringList keyValues = p.split("=");
        if (keyValues.length() == 2) {
            params.insert(keyValues[0], keyValues[1]);
        }
    }

    int width = params.value("width", 0).toInt();
    int height = params.value("height", 0).toInt();

    SkyImageResponse *response = new SkyImageResponse();

    std::vector<QrSegment> segs = QrSegment::makeSegments(path.toStdString().c_str());
    QrCode qr1 = QrCode::encodeSegments(segs, QrCode::Ecc::HIGH, 7, 11, 2, false);
    QImage img = QImage(qr1.getSize(), qr1.getSize(), QImage::Format_RGB888);

    for (int y = 0; y < qr1.getSize(); y++) {
        for (int x = 0; x < qr1.getSize(); x++) {
            if (qr1.getModule(x, y) == 0)
                img.setPixel(x, y, qRgb(255, 255, 255));
            else
                img.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    img = img.scaled(width, height, Qt::KeepAspectRatio);
    response->onSuccess(img);
    emit response->finished();
    return response;
}

void QRCodeImageProvider::clear() {
}
