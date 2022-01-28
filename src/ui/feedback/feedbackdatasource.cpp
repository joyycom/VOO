#include "feedbackdatasource.h"
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QJsonDocument>
#include "src/base/utils/AppConstance.h"
#include "src/base/http/SEHttpClient.h"
#include "src/base/utils/AppPath.h"
#include <QCryptographicHash>
#include "src/base/utils/qaesencryption.h"
#include "src/base/utils/ZipUtils.h"
#include "src/base/utils/fileutils.h"
#include "base/utils/UnitConverUtils.h"

FeedBackDataSource::FeedBackDataSource(QObject *parent) : QObject(parent) {
}

FeedBackDataSource::~FeedBackDataSource() {
    QDir tmpdir(AppPath::tempPath("tmpUpload"));
    if (tmpdir.exists()) {
        tmpdir.removeRecursively();
    }
}

QString FeedBackDataSource::uploadFile() const {
    return m_uploadFile;
}

void FeedBackDataSource::setUploadFile(const QUrl &uploadFile) {
    QString filePath = uploadFile.toLocalFile();
    if (!QFileInfo::exists(filePath))
        return;

    m_uploadFile = filePath;
    QFileInfo info(filePath);
    QString fileName = info.fileName();
    qint64 fileSize = 0;
    if (info.exists()) {
        fileSize = info.size();
    }
    emit uploadFileChanged(fileName, UnitConverUtils::formatSize(fileSize));
}

void FeedBackDataSource::startFeedback(const QString &pickedScene,
                                       const QString &problemDescribe,
                                       const QString &contactNum) {
    // pack nyy
    QString feedbackInfo = pickedScene + ": " + problemDescribe;
    QJsonObject nyyJO = packNyy(feedbackInfo, contactNum);
    QString nyy = QString(QJsonDocument(nyyJO).toJson(QJsonDocument::Compact));
    qDebug() << "nyy: " << nyy;
    // nyy = nyyEncrypt(nyy);
    // qDebug() << "nyyEncrypt: " <<nyy;

    // pack the log files to log.zip
    const QString tmpUpload = AppPath::tempPath("tmpUpload");
    QDir tmpDir(AppPath::tempPath());
    if (!tmpDir.exists(tmpUpload)) {
        tmpDir.mkdir(tmpUpload);
    }
    QDir tmpUpDir(tmpUpload);
    if (!tmpUpDir.isEmpty()) {
        tmpUpDir.removeRecursively();
        tmpDir.mkdir(tmpUpload);
    }
    QString resDirPath = AppPath::logPath();

    QDir logDir(resDirPath);
    QFileInfoList list = logDir.entryInfoList(QDir::Files,QDir::Name | QDir::IgnoreCase | QDir::Reversed);
    for(int i=0;i<10 && i<list.size();++i){
        QFileInfo file = list[i];
        FileUtils::copyFile(file.absoluteFilePath(),tmpUpload+"/"+file.fileName(),true);
    }

    QString crashReportPath = AppPath::crashPath("reports");

    QDir crashDir(crashReportPath);

    QFileInfoList reportList = crashDir.entryInfoList(QDir::Files,QDir::Time | QDir::IgnoreCase | QDir::Reversed);
    for(int i=0;i<3 && i<reportList.size();++i){
        QFileInfo file = reportList[i];
        FileUtils::copyFile(file.absoluteFilePath(),tmpUpload+"/"+file.fileName(),true);
    }
    if (!uploadFile().isEmpty()) {
        QString attachedFile = uploadFile();
        QFileInfo info(attachedFile);
        qDebug() << "attached path: " << attachedFile;
        FileUtils::copyFile(attachedFile, tmpUpload + "/" + info.fileName(), true);
    }

    QString compresslogdir = tmpUpload;
    QString compressOutput = tmpUpload + ".zip";
    QString uploadPath = "";
    QString zip = ZipUtils::compressFolder(compresslogdir, compressOutput);
    if (zip.length() > 0) {
        QDir tmpLogDir(compresslogdir);
        if (tmpLogDir.exists()) {
            tmpLogDir.removeRecursively();
        }
        uploadPath = compressOutput;
    } else {
        qDebug() << "compress log fail! ";
    }

    // start to upload
    SEHttpClient(FEEDBACK_URL)
        .addParam("nyy", nyy)
        .upload(
            uploadPath,
            [=](qint64 bytesSent, qint64 bytesTotal) {
            },
            [](const QString &response) {
                qDebug() << "startFeedback:" << response;
            },
            [](const QString &error) {
                qDebug() << "startFeedback,error:" << error;
            });

    emit startedFeedback();
}

QJsonObject FeedBackDataSource::packNyyData(const QString &feedbackInfo, const QString &contactNum) {
    QJsonObject nyyData;
    nyyData.insert("reportType", "UFB");
    nyyData.insert("productVer", AppConstance::appVerWithBuildNum());
    nyyData.insert("uid", QString::number(AppConstance::uid()));
    nyyData.insert("phoneType", "PC");
    nyyData.insert("osVer", AppConstance::osVer());
    nyyData.insert("guid", AppConstance::deviceID());
    nyyData.insert("networkState", "unknown");
    nyyData.insert("marketChannel", "PC");
    nyyData.insert("serviceProvider", "unknown");
    nyyData.insert("feedback", feedbackInfo);
    nyyData.insert("contactInfo", contactNum);
    return nyyData;
}

QJsonObject FeedBackDataSource::packNyy(const QString &feedbackInfo, const QString &contactNum) {
    QJsonObject nyy;
    nyy.insert("appId", "skyeditor");
    nyy.insert("sign", "");
    QJsonObject nyyData = packNyyData(feedbackInfo, contactNum);
    nyy.insert("data", nyyData);
    return nyy;
}

