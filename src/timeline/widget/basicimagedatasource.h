#ifndef BASICIMAGEDATASOURCE_H
#define BASICIMAGEDATASOURCE_H

#include <QObject>
#include <base/utils/AppPath.h>

extern QString mkPathIfNeed(QString dir);

class BasicImageDataSource : public QObject {
    Q_OBJECT
public:
    explicit BasicImageDataSource(QObject *parent = nullptr);
    ~BasicImageDataSource();

    bool isDownload() const;

public slots:

    void loadAnimatedImg(const QString imgUrl);

    QString localAnimatedImgSrc() const;

    void imageDownloadSuccess(QString url,QString path);

signals:
    void loadAnimatedImgSuccess(QString imgPath);

private:
    QString m_imgUrl = "";
    QString m_localSrc = "";
};

#endif // BASICIMAGEDATASOURCE_H
