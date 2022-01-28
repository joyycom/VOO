#include "UploadUtils.h"
#include "threadpools.h"

#include <base/utils/JsonUtils.h>
#include <base/utils/fileutils.h>

#include <QDebug>
#include <QFile>

#include "src/base/http/BaseRsp.h"
#include "src/base/http/SEHttpClient.h"
#include "src/base/utils/AppConstance.h"
#include "src/base/utils/AppPath.h"
#include "src/base/utils/SharedPref.h"
#include "src/base/utils/ZipUtils.h"

void UploadUtils::upload(QString filePath,
                         std::function<void(qint64, qint64)> progressHandler,
                         std::function<void(const bool, const QString &)> complateHandler, int retryCount) {
    QFile f(filePath);
    if (!f.exists()) {
        complateHandler(false, "file not found");
        return;
    }
    ThreadPools::instance()->mainThread([=]{
        QString extension = FileUtils::getFileExtension(filePath);
        qDebug() << "upload uploadURL Begin:" << " filePath:"<<filePath;
        SEHttpClient(AppConstance::serverDomain() + VOO_UPLOAD_PATH +"?fileExtension=" + extension)
            .post([filePath, progressHandler, complateHandler, retryCount](const QString &response) {
                BaseRsp ossurl(response);
                if(ossurl.isSuccess()){
                    complateHandler(false, "Error:"+ossurl.message());
                    return;
                }
                QString uploadURL = ossurl.data().toObject().value("signedUrl").toString();
                QString serverSaveUrl = ossurl.data().toObject().value("permanentUrl").toString();
                QString contentType = ossurl.data().toObject().value("contentType").toString();
                qDebug() << "upload uploadURL:" << uploadURL << " filePath:"<<filePath;
                SEHttpClient(uploadURL)
                    .addHeader("Content-Type", contentType)
                    .upload(
                        filePath,
                        [progressHandler](qint64 bytesSent, qint64 bytesTotal) {
                            progressHandler(bytesSent, bytesTotal);
                        },
                        [filePath, serverSaveUrl, complateHandler](const QString &response) {
                            qInfo() << "upload result:" << response;
                            complateHandler(true, serverSaveUrl);
                        },
                        [=](const QString &error) {
                            qWarning() << "upload error:" << error << " Retry:"<<retryCount;
                            if(retryCount <= 0){
                                complateHandler(false, error);
                            }else{
                                upload(filePath,progressHandler,complateHandler,retryCount - 1);
                            }
                        },
                        "UTF-8",
                        true);
        },[=](const QString& error){
            qWarning() << "upload uploadURL Error:" << error << " filePath:"<<filePath<< " Retry:"<<retryCount;
            if(retryCount <= 0){
                complateHandler(false, error);
            }else{
                upload(filePath,progressHandler,complateHandler,retryCount - 1);
            }
        });
    });

}
