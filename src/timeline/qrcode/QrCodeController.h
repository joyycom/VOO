#ifndef QrCodeController_H
#define QrCodeController_H

#include <QObject>
#include "QrCode.hpp"
#include "src/project/SkyPack.h"

class QrCodeController : public QObject {
    Q_OBJECT
public:
    explicit QrCodeController(QObject *parent = nullptr);

signals:
    void qrcodeLinkReady(QString url);
    void qrcodeLinkFail();

public slots:
    void updateQrcode();
    void coverChanged(QString cover);
    void exportProgressChanged(double exportProgress);
    void exportCancel();
    void exportError();
    void exportFinishChanged(QString outputDir);

private:
    SkyPack *m_skyPack;
    bool m_cancel = false;
};

#endif // QrCodeController_H
