#include "BasicImageDataSource.h"
#include <QDebug>
#include <QFileInfo>
#include <QCryptographicHash>
#include <timeline/controller/components/AnimationListComponent.h>
#include <timeline/image/ImageDownloadMgr.h>
#include "src/base/http/SEHttpClient.h"

BasicImageDataSource::BasicImageDataSource(QObject *parent) : QObject(parent) {
    connect(ImageDownloadMgr::instance(),&ImageDownloadMgr::loadImgSuccess,this,&BasicImageDataSource::imageDownloadSuccess);
}

BasicImageDataSource::~BasicImageDataSource() {
    disconnect(ImageDownloadMgr::instance(),&ImageDownloadMgr::loadImgSuccess,this,&BasicImageDataSource::imageDownloadSuccess);
}

bool BasicImageDataSource::isDownload() const {
    auto fileInfo = QFileInfo(m_localSrc);
    return fileInfo.exists() && fileInfo.size() > 10;
}

void BasicImageDataSource::loadAnimatedImg(QString imgUrl) {
    if (m_imgUrl != imgUrl || m_localSrc == "") {
        m_imgUrl = imgUrl;
        m_localSrc = ImageDownloadMgr::instance()->getLocalImagePath(imgUrl);
    }

    if (isDownload()) {
        emit loadAnimatedImgSuccess("file:/" + m_localSrc);
    } else {
        ImageDownloadMgr::instance()->downloadImage(imgUrl);
    }
}

void BasicImageDataSource::imageDownloadSuccess(QString url,QString filePath){
    if(m_imgUrl == url){
        if (isDownload()) {
            emit loadAnimatedImgSuccess("file:/" + m_localSrc);
        }
    }

}

QString BasicImageDataSource::localAnimatedImgSrc() const {
    if (isDownload()) {
        return "file:/" + m_localSrc;
    }
    return "";
}
