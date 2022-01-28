#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

typedef std::shared_ptr<QWebSocketServer> QWebSocketServerPtr;
typedef std::shared_ptr<QWebSocket> QWebSocketPtr;

class VooSocketServer : public QObject {
    Q_OBJECT

public:
    static VooSocketServer *instance() {
        static VooSocketServer server;
        return &server;
    };

signals:
    void sendSocket(const QString &message);

private slots:
    void onNewConnection();

private:
    VooSocketServer();
    QWebSocketServerPtr mWebsocketServer;
};
