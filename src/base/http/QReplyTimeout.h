#ifndef QReplyTimeout_H
#define QReplyTimeout_H

#include <QObject>
#include <QTimer>
#include <QNetworkReply>

class QReplyTimeout : public QObject {

    Q_OBJECT

public:
    QReplyTimeout(QNetworkReply *reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) { // 启动单次定时器
            QTimer::singleShot(timeout, this, SLOT(onTimeout()));
        }
    }

signals:
    void timeout(); // 超时信号 - 供进一步处理

private slots:
    void onTimeout() { // 处理超时
        QNetworkReply *reply = static_cast<QNetworkReply *>(parent());
        if (reply->isRunning()) {
            reply->abort();
            reply->deleteLater();
            emit timeout();
        }
    }
};

#endif
