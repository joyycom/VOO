#include "QrCodeController.h"

#include <QDebug>
#include <string>
#include <vector>

#include "QrCode.hpp"
#include "base/utils/threadpools.h"
#include "src/base/utils/AppConstance.h"
#include "src/base/utils/AppPath.h"
#include "src/base/utils/UploadUtils.h"
#include "src/project/SEProject.h"

using namespace qrcodegen;
QrCodeController::QrCodeController(QObject *parent) : QObject(parent) {
}

void QrCodeController::updateQrcode() {
    qDebug() << QTime::currentTime() << "qrcode begin!";
    //    SharedPref::instance()->readJsonArray("remote_url_cache");
    QString tempDir = AppPath::tempPath() + "/qrcode_pack";
    // do pack
    m_skyPack = SkyPack::pack(tempDir, "", "pack", "", -1, false, true, 0,"");
    connect(m_skyPack, SIGNAL(coverChanged(QString)), this, SLOT(coverChanged(QString)));
    connect(m_skyPack, SIGNAL(progressChanged(double)), this, SLOT(exportProgressChanged(double)));
    connect(m_skyPack, SIGNAL(cancelChanged()), this, SLOT(exportCancel()));
    connect(m_skyPack, SIGNAL(errorChanged()), this, SLOT(exportError()));
    qDebug() << QTime::currentTime() << "qrcode pack begin";
    QObject::connect(m_skyPack, &SkyPack::finishChanged, [=](QString &outputDir, QString &, QString &) {
        ThreadPools::instance()->mainThread(
            [this, outputDir] {
                exportFinishChanged(outputDir);
            },
            0);
    });
}

void QrCodeController::coverChanged(QString cover) {
    qDebug() << QTime::currentTime() << "qrcode coverChanged";
}
void QrCodeController::exportProgressChanged(double exportProgress) {
    qDebug() << QTime::currentTime() << "qrcode exportProgress = " << exportProgress;
}
void QrCodeController::exportCancel() {
    m_cancel = true;
    qDebug() << QTime::currentTime() << "qrcode exportCancel";
    emit qrcodeLinkFail();
}
void QrCodeController::exportError() {
    qDebug() << QTime::currentTime() << "qrcode exportError";
    emit qrcodeLinkFail();
}

void QrCodeController::exportFinishChanged(QString outputDir) {
    qDebug() << QTime::currentTime() << "qrcode pack success , path = " << outputDir;
    // tempDir + "/" + SEProject::current()->title()
    QString zipPath = outputDir + "/pack.zip";
    // do upload
    qDebug() << QTime::currentTime() << "qrcode upload begin";
    UploadUtils::upload(
        zipPath,
        [=](qint64, qint64) {
        },
        [=](const bool &success, const QString &str) {
            if (m_cancel) {
                return;
            }
            if (success) {
                qDebug() << QTime::currentTime() << "qrcode upload success";
                emit qrcodeLinkReady(str);
            } else {
                qDebug() << QTime::currentTime() << "qrcode upload fail : " << str;
                emit qrcodeLinkFail();
            }
        });
}
