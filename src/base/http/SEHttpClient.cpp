#include "SEHttpClient.h"

#include <QDebug>
#include <QFile>
#include <QHash>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRandomGenerator>
#include <QTextCodec>
#include <QUrlQuery>


#include "BuildConfig.h"
#include "QReplyTimeout.h"
#include "src/base/utils/AppConstance.h"

struct SEHttpClientPrivate {
    SEHttpClientPrivate(const QString &url)
        : url(url),
          networkAccessManager(NULL),
          useInternalNetworkAccessManager(true),
          debug(false) {
    }

    QString url;                     // Request URL
    QUrlQuery params;                // Request params
    QJsonValue jsonParams;          // Request json params
    QHash<QString, QString> headers; // Request headers
    QNetworkAccessManager *networkAccessManager;
    bool useInternalNetworkAccessManager; // Is using Qt QNetworkAccessManager
    bool debug;
};

SEHttpClient::SEHttpClient(const QString &url) : d(new SEHttpClientPrivate(url)) {
    d->headers["Appkey"] = AppConstance::appKey();
    d->headers["Language"] = AppConstance::languages().first();
#ifdef Q_OS_WIN
    d->headers["Platform"] = "win";
#else
    d->headers["Platform"] = "mac";
#endif
    d->headers["Version"] = AppConstance::appVer();
    d->headers["Uid"] = QString::number(AppConstance::uid());
    d->headers["Deviceid"] = AppConstance::deviceID();
    d->headers["Machaine"] = AppConstance::machaineName();
    d->headers["Country"] = AppConstance::country();
    d->headers["X-traceid"] = getCommonTraceId();
}

QString SEHttpClient::getCommonTraceId() {
    return QString::number(QDateTime::currentDateTime().toTime_t()) + "_" +
           QString::number(QRandomGenerator::global()->bounded(1000, 90000000));
}

SEHttpClient::~SEHttpClient() {
    // qDebug() << "~SEHttpClient";
    delete d;
}

SEHttpClient &SEHttpClient::useManager(QNetworkAccessManager *manager) {
    d->networkAccessManager = manager;
    d->useInternalNetworkAccessManager = false;
    return *this;
}

// When debug is true, the debug mode will be used, and the requested URL and parameters will be output when the request
// is executed
SEHttpClient &SEHttpClient::debug(bool debug) {
    d->debug = debug;
    return *this;
}

// add parameters
SEHttpClient &SEHttpClient::addParam(const QString &name, const QString &value) {
    d->params.addQueryItem(name, value);
    return *this;
}

SEHttpClient &SEHttpClient::addJsonParam(const QJsonValue& param) {
    d->jsonParams = param;
    return *this;
}

// add access header
SEHttpClient &SEHttpClient::addHeader(const QString &header, const QString &value) {
    d->headers[header] = value;
    return *this;
}

SEHttpClient &SEHttpClient::addFormHeader() {
    return addHeader("content-type", "application/x-www-form-urlencoded");
}

// Perform GET request
void SEHttpClient::get(std::function<void(const QString &)> successHandler,
                       std::function<void(const QString &)> errorHandler,
                       const char *encoding) {
    execute(false, successHandler, errorHandler, encoding);
}

// Perform POST request
QNetworkReply* SEHttpClient::post(std::function<void(const QString &)> successHandler,
                        std::function<void(const QString &)> errorHandler,
                        const char *encoding) {
    return execute(true, successHandler, errorHandler, encoding);
}

// Use GET to download, call back readyRead() when there is data to read, in most cases you should save the data to a
// file in readyRead()
void SEHttpClient::download(std::function<void(const QByteArray &)> readyRead,
                            std::function<void()> finishHandler,
                            std::function<void(const QString &)> errorHandler) {
    // If it is a GET request and the parameters are not empty, encode the requested parameters and put them behind the
    // URL
    if (!d->params.isEmpty()) {
        d->url += "?" + d->params.toString(QUrl::FullyEncoded);
    }

    if (d->debug) {
        qDebug() << QString("URL: %1?%2").arg(d->url).arg(d->params.toString());
    }

    QUrl urlx(d->url);
    QNetworkRequest request(urlx);
    bool internal = d->useInternalNetworkAccessManager;
    QNetworkAccessManager *manager = internal ? new QNetworkAccessManager() : d->networkAccessManager;
    QNetworkReply *reply = manager->get(request);

    // Call back when there is data to read readyRead()
    QObject::connect(reply, &QNetworkReply::readyRead, [=] {
        readyRead(reply->readAll());
    });
    QReplyTimeout timeout(reply, 10000);


    // end of request
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        if (reply->error() == QNetworkReply::NoError && NULL != finishHandler) {
            finishHandler();
        }

        // release resources
        reply->deleteLater();
        if (internal) {
            manager->deleteLater();
        }
    });

    // request error handling
    QObject::connect(
        reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] {
            if (NULL != errorHandler) {
                errorHandler(reply->errorString());
            }
        });
}

void SEHttpClient::upload(const QString &path,
                          std::function<void(qint64, qint64)> progressHandler,
                          std::function<void(const QString &)> successHandler,
                          std::function<void(const QString &)> errorHandler,
                          const char *encoding,
                          bool usePut) {
    if (d->debug) {
        qDebug() << QString("URL: %1").arg(d->url);
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QFile *file = new QFile(path);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    // If the file fails to open, release the resource and return
    if (!file->open(QIODevice::ReadOnly)) {
        if (NULL != errorHandler) {
            errorHandler(QString("File %1 open failed: %2").arg(path).arg(file->errorString()));
            multiPart->deleteLater();
            return;
        }
    }

    // Indicates file upload
    QString disposition = QString("form-data; name=\"file\"; filename=\"%1\"").arg(file->fileName());
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(disposition));
    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    // upload customized params
    auto list = d->params.queryItems();
    for (int i = 0; i < list.size(); i++) {
        auto item = list.at(i);
        QHttpPart httpPart;
        QString paramDisposition = QString("form-data; name=\"%1\";").arg(item.first);
        httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(paramDisposition));
        httpPart.setBody(item.second.toUtf8().data());
        multiPart->append(httpPart);
    }

    bool internal = d->useInternalNetworkAccessManager;
    QNetworkRequest request(QUrl(d->url));

    // add the request header to the request
    QHashIterator<QString, QString> iter(d->headers);
    while (iter.hasNext()) {
        iter.next();
        request.setRawHeader(iter.key().toUtf8(), iter.value().toUtf8());
    }

    QNetworkAccessManager *manager = internal ? new QNetworkAccessManager() : d->networkAccessManager;
    QNetworkReply *reply;
    if (usePut) {
        reply = manager->put(request, file);
    } else {
        reply = manager->post(request, multiPart);
    }
    multiPart->setParent(reply);

    // Read all response data at one time at the end of the request
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        file->close();

        // release resources
        reply->deleteLater();
        if (internal) {
            manager->deleteLater();
        }
        if (reply->error() == QNetworkReply::NoError && NULL != successHandler) {
            successHandler(readResponse(reply, encoding)); // Successful execution
        }
    });
    QObject::connect(reply, &QNetworkReply::uploadProgress, [=](qint64 bytesSent, qint64 bytesTotal) {
        if (NULL != progressHandler) {
            progressHandler(bytesSent, bytesTotal);
        }
    });
    // timeout
    QReplyTimeout timeout(reply, 10000);


    // request error handling
    QObject::connect(
        reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] {
        file->close();
        if (NULL != errorHandler) {
                errorHandler(reply->errorString());
            }
        });
}

// execute the requested helper function
QNetworkReply* SEHttpClient::execute(bool posted,
                           std::function<void(const QString &)> successHandler,
                           std::function<void(const QString &)> errorHandler,
                           const char *encoding) {
    // If it is a GET request and the parameters are not empty, encode the requested parameters and put them behind the
    // URL
    if (!posted && !d->params.isEmpty()) {
        d->url += "?" + d->params.toString(QUrl::FullyEncoded);
    }

    if (d->debug) {
        qDebug() << QString("URL: %1?%2").arg(d->url).arg(d->params.toString());
    }

    QUrl urlx(d->url);
    QNetworkRequest request(urlx);

    // add the request header to the request
    QHashIterator<QString, QString> iter(d->headers);
    while (iter.hasNext()) {
        iter.next();
        request.setRawHeader(iter.key().toUtf8(), iter.value().toUtf8());
    }

    // Note: The member data of the SEHttpClient object cannot be used in the Lambda expression, because it may be
    // destructed before the network access is over. So if you want to use its related data, define a local variable to
    // save its data, and then access this local variable in the Lambda expression

    // If you do not use an external manager, create a new one, and it will be deleted automatically after the access is
    // completed
    bool internal = d->useInternalNetworkAccessManager;
    QNetworkAccessManager *manager = internal ? new QNetworkAccessManager() : d->networkAccessManager;
    QNetworkReply *reply;
    if (posted) {
        if (d->jsonParams.isNull()) {
            reply = manager->post(request, d->params.toString(QUrl::FullyEncoded).toUtf8());
        } else {
            request.setRawHeader("content-type", "application/json");
            QJsonDocument httpDocum;
            if(d->jsonParams.isArray()){
                httpDocum.setArray(d->jsonParams.toArray());
            }else{
                 httpDocum.setObject(d->jsonParams.toObject());
            }

            QByteArray httpData = httpDocum.toJson(QJsonDocument::Compact);
            reply = manager->post(request, httpData);
        }
    } else {
        reply = manager->get(request);
    }
    QString url = d->url;
    // Read all response data at one time at the end of the request
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        if (reply->error() == QNetworkReply::NoError && NULL != successHandler) {
            successHandler(readResponse(reply, encoding)); // Successful execution
        }

        qDebug()<<"request Finish "<<url;

        // release resources
        reply->deleteLater();
        if (internal) {
            manager->deleteLater();
        }
    });

    qDebug()<<"request start "<<url;

    QReplyTimeout timeout(reply,10000);

    // request error handling
    QObject::connect(
        reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=](QNetworkReply::NetworkError error) {
            if (NULL != errorHandler) {
			    qDebug()<<"Network Error? "<<url<<" Error:"<<error << " Reply:";
                auto error = reply->error();
                auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
                errorHandler(reply->errorString());
            }
        });
    return reply;
}

QString SEHttpClient::readResponse(QNetworkReply *reply, const char *encoding) {
    QByteArray data = reply->readAll();
    return QString::fromUtf8(data);
}
