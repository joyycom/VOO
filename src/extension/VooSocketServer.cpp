#include "VooSocketServer.h"
#include <QDebug>

#include "Dispatch.h"
#include "base/utils/threadpools.h"

VooSocketServer::VooSocketServer()
    : mWebsocketServer(new QWebSocketServer(QStringLiteral("VooSocketServer"), QWebSocketServer::NonSecureMode)) {
    //不能循环太多，影响性能
    for (int port = 22222; port < 22333; port++) {
        if (this->mWebsocketServer->listen(QHostAddress::Any, port)) {
            qDebug() << "VooSocketServer: success to listen " << port;
            QObject::connect(mWebsocketServer.get(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
            break;
        } else {
            qDebug() << "VooSocketServer: failed to listen " << port;
        }
    }
}

void VooSocketServer::onNewConnection() {
    qDebug() << "VooSocketServer::onNewConnection";
    QWebSocketPtr socket(this->mWebsocketServer->nextPendingConnection());
    Dispatch *dispatch = new Dispatch();
    QObject::connect(dispatch, &Dispatch::sendSocket, [=](const QString &message) {
        socket->sendTextMessage(message);
    });
    QObject::connect(this, &VooSocketServer::sendSocket, [=](const QString &message) {
//        qDebug() << "VooSocketServer::sendTextMessage: " << message;
        if (ThreadPools::instance()->isMainThread()) {
            socket->sendTextMessage(message);
        } else {
            ThreadPools::instance()->mainThread([=] {
                socket->sendTextMessage(message);
            });
        }
    });
    QObject::connect(socket.get(), &QWebSocket::textMessageReceived, [=](const QString &message) {
        qDebug() << "VooSocketServer::processTextMessage: " << message;
        dispatch->exec(socket.get(), message);
    });
    QObject::connect(socket.get(), &QWebSocket::disconnected, [=] {
        qDebug() << "VooSocketServer::socketDisconnected";
		dispatch->deleteLater();
    });
}
