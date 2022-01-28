#ifndef SEHttpClient_H
#define SEHttpClient_H

#include <qglobal.h>

#include <QJsonObject>
#include <QJsonArray>
#include <functional>

class QString;
class QByteArray;
struct SEHttpClientPrivate;
class QNetworkReply;
class QNetworkAccessManager;

class SEHttpClient {
public:
    SEHttpClient(const QString &url);
    ~SEHttpClient();

    /**
     * @brief creates a thread every time a QNetworkAccessManager object is created. When accessing the network
     * frequently, in order to save thread resources, You can use QNetworkAccessManager passed down, it will not be
     * deleted by SEHttpClient. If you do not use useManager() to pass in a QNetworkAccessManager, SEHttpClient will
     * automatically create one and delete it after the network access is completed.
     * @param manager QNetworkAccessManager object
     * @return returns a reference to SEHttpClient, which can be used for chained calls
     */
    SEHttpClient &useManager(QNetworkAccessManager *manager);

    /**
     * @brief parameter debug is true to use debug mode, and output the requested URL and parameters when the request is
     * executed
     * @param debug whether to enable debug mode
     * @return returns a reference to SEHttpClient, which can be used for chained calls
     */
    SEHttpClient &debug(bool debug);

    /**
     * @brief add parameters
     * @param name The name of the parameter
     * @param value parameter value
     * @return returns a reference to SEHttpClient, which can be used for chained calls
     */
    SEHttpClient &addParam(const QString &name, const QString &value);
    SEHttpClient &addJsonParam(const QJsonValue& param);

    /**
     * @brief add access header
     * @param header to access the name of the header
     * @param value access to the value of the header
     * @return returns a reference to SEHttpClient, which can be used for chained calls
     */
    SEHttpClient &addHeader(const QString &header, const QString &value);

    /**
     * @brief adds the header information used in the POST form, which is equivalent to addHeader("content-type",
     * "application/x-www-form-urlencoded")
     * @return returns a reference to SEHttpClient, which can be used for chained calls
     */
    SEHttpClient &addFormHeader();

    /**
     * @brief executes GET request
     * @param successHandler callback lambda function for successful request
     * @param errorHandler callback lambda function for request failure
     * @param encoding Request response encoding
     */
    void get(std::function<void(const QString &)> successHandler,
             std::function<void(const QString &)> errorHandler = NULL,
             const char *encoding = "UTF-8");

    /**
     * @brief executes POST request
     * @param successHandler callback lambda function for successful request
     * @param errorHandler callback lambda function for request failure
     * @param encoding Request response encoding
     */
    QNetworkReply* post(std::function<void(const QString &)> successHandler,
              std::function<void(const QString &)> errorHandler = NULL,
              const char *encoding = "UTF-8");

    /**
     * @brief uses GET to download, and calls back readyRead() when there is data to read. In most cases, you should
     * save the data to a file in readyRead()
     * @param readyRead callback lambda function when there is data to read
     * @param finishHandler callback lambda function after request processing is completed
     * @param errorHandler callback lambda function for request failure
     */
    void download(std::function<void(const QByteArray &)> readyRead,
                  std::function<void()> finishHandler = NULL,
                  std::function<void(const QString &)> errorHandler = NULL);

    /**
     * @brief upload file
     * @param path The path of the file to be uploaded
     * @param successHandler callback lambda function for successful request
     * @param errorHandler callback lambda function for request failure
     * @param encoding Request response encoding
     */
    void upload(const QString &path,
                std::function<void(qint64, qint64)> progressHandler = NULL,
                std::function<void(const QString &)> successHandler = NULL,
                std::function<void(const QString &)> errorHandler = NULL,
                const char *encoding = "UTF-8",
                const bool usePut = false);

private:
    /**
     * @brief performs the requested auxiliary function
     * @param posted true means POST request, false means GET request
     * @param successHandler callback lambda function for successful request
     * @param errorHandler callback lambda function for request failure
     * @param encoding Request response encoding
     */
    QNetworkReply* execute(bool posted,
                 std::function<void(const QString &)> successHandler,
                 std::function<void(const QString &)> errorHandler,
                 const char *encoding);

    /**
     * @brief read the server response data
     * @param reply requested QNetworkReply object
     * @param encoding The encoding of the request response, UTF-8 is used by default
     * @return string of server-side response
     */
    QString readResponse(QNetworkReply *reply, const char *encoding = "UTF-8");

    SEHttpClientPrivate *d;

private:
    QString getCommonTraceId();
};

#endif // SEHttpClient_H
