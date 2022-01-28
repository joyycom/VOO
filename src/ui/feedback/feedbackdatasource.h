#ifndef FEEDBACKDATASOURCE_H
#define FEEDBACKDATASOURCE_H

#include <QObject>
#include <QUrl>
#include <QJsonObject>

class FeedBackDataSource : public QObject {
    Q_OBJECT

public:
    explicit FeedBackDataSource(QObject *parent = nullptr);
    ~FeedBackDataSource();

signals:
    void uploadFileChanged(QString fileName, QString fileSize);
    void startedFeedback();

public slots:
    QString uploadFile() const;
    void setUploadFile(const QUrl &uploadFile);
    void startFeedback(const QString &pickedScene, const QString &problemDescribe, const QString &contactNum);

private:
    QString m_uploadFile;

    QJsonObject packNyyData(const QString &feedbackInfo, const QString &contactNum);
    QJsonObject packNyy(const QString &feedbackInfo, const QString &contactNum);
    QString nyyEncrypt(const QString &nyy);
};

#endif // FEEDBACKDATASOURCE_H
