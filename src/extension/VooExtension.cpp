#include "VooExtension.h"
#include <QJsonObject>

void VooExtension::init() {
    VooSocketServer::instance();
}

RecvBody VooExtension::messageToRecv(const QString &message) {
    RecvBody recv;
    if (message.isEmpty()) {
        return recv;
    }
    QJsonObject object = QJsonDocument::fromJson(message.toUtf8()).object();
    recv.api = object.take("api").toString();
    recv.args = object.take("args");
    return recv;
}

QString VooExtension::wrapSend(const QString &event, const QJsonValue &data) {
    QJsonObject send;
    send.insert("event", event);
    send.insert("data", data);
    return QJsonDocument(send).toJson(QJsonDocument::Compact);
}

void VooExtension::printLog(int level, const QString &msg) {
    QJsonObject data;
    data.insert("level", level);
    data.insert("msg", msg);
    QString socketMessage = wrapSend(EVENT_PrintLog, data);
    emit VooSocketServer::instance()->sendSocket(socketMessage);
}

void VooExtension::windowChanged(WindowType type) {
    QJsonObject data;
    data.insert("type", type);
    QString socketMessage = wrapSend(EVENT_WindowChanged, data);
    emit VooSocketServer::instance()->sendSocket(socketMessage);
}
