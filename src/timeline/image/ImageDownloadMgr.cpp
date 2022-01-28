#include "ImageDownloadMgr.h"

#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QCryptographicHash>

#include <base/utils/AppPath.h>

#include <base/http/SEHttpClient.h>

ImageDownloadMgr::ImageDownloadMgr() : QObject(nullptr)
{

}

ImageDownloadMgr* ImageDownloadMgr::instance(){
    static ImageDownloadMgr instance;
    return &instance;
}


void ImageDownloadMgr::downloadImage(QString url){
    auto localPath = getLocalImagePath(url);
    if(QFileInfo(localPath).exists()){
        emit loadImgSuccess(url, localPath);
    }
    if(downloadingList.contains(url)){
        return;
    }

    downloadingList.append(url);

    QFile *file = new QFile(localPath + ".tmp");
    if (file->exists()) {
        file->remove();
    }

    qDebug()<<"BasicImageDataSource::downloadPreview:"<<url;

    QString fileName = QFileInfo(localPath).fileName();
    file->open(QIODevice::ReadWrite);
    SEHttpClient(url).download(
                [=](const QByteArray &data) {
        file->write(data);
    },
    [=] {
        file->flush();
        file->close();

        bool result = file->rename(localPath);
        qDebug() << __FUNCTION__ << "Preview downloaded..." << url << " Rename:" << fileName
                 << " Result:" << result;
        auto localCacheFile = QFileInfo(localPath);
        if (localCacheFile.exists() && localCacheFile.size() > 10) {
            emit loadImgSuccess(url, localPath);
        }
        delete file;
        downloadingList.removeOne(url);
    },
    [=](const QString &msg) {
        qInfo() << __FUNCTION__ << "Preview downloaded Failed! Error:" << msg;
        downloadingList.removeOne(url);
    });
}

QString ImageDownloadMgr::getLocalImagePath(QString url){
    auto qUrl = QUrl(url);
    if(qUrl.isLocalFile()){
        return qUrl.toLocalFile();
    }
    QString fileName = QUrl(url).fileName();
    QString md5 = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5).toHex();
    QString imgPath = AppPath::mkPathIfNeed(AppPath::thumbsPath(QString("AnimatedImg/")));
    int index = fileName.lastIndexOf('.');
    return index>0?imgPath + md5 + fileName.right(index):imgPath + md5;
}
