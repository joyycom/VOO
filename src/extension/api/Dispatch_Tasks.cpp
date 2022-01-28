#include "Dispatch_Tasks.h"
#include <QJsonObject>
#include <QDebug>

void PrintLog::exec(const QJsonValue &data) {
    switch (data.type()) {
        case QJsonValue::Bool:
            qDebug() << data.toBool();
            break;
        case QJsonValue::Double:
            qDebug() << data.toDouble();
            break;
        case QJsonValue::String:
            qDebug() << data.toString();
            break;
        case QJsonValue::Object: {
            QJsonObject object = data.toObject();
            if (object.contains("level") && object.contains("msg")) {
                int level = object.take("level").toInt();
                QString msg = object.take("msg").toString();
                switch (level) {
                    case LogLevel::Debug:
                        qDebug() << msg;
                        break;
                    case LogLevel::Info:
                        qInfo() << msg;
                        break;
                    case LogLevel::Warn:
                        qWarning() << msg;
                        break;
                    case LogLevel::Error:
                        qCritical() << msg;
                        break;
                    default:
                        qDebug() << msg;
                        break;
                }
            } else {
                qDebug() << data;
            }
            break;
        }
        default:
            qDebug() << data;
            break;
    }
}
