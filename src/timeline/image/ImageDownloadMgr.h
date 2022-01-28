#ifndef IMAGEDOWNLOADMGR_H
#define IMAGEDOWNLOADMGR_H

#include <QObject>

class ImageDownloadMgr : public QObject
{
    Q_OBJECT
public:

    static ImageDownloadMgr* instance();

    void downloadImage(QString url);

    QString getLocalImagePath(QString url);

signals:
    void loadImgSuccess(QString url, QString imgPath);

private:
    ImageDownloadMgr();

    QList<QString> downloadingList;
};

#endif // IMAGEDOWNLOADMGR_H
