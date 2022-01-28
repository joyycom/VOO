#ifndef JsonUtils_H
#define JsonUtils_H

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QMetaObject>
#include <QMetaProperty>
#include <QJsonObject>

class JsonUtils {

public:
    QObject *fromJson(const QMetaObject &meta, QString &json);
    QObject *fromJson(const QMetaObject *meta, QJsonObject &jsonObject);
    QVariant jsonValueToProperty(QObject *object, QMetaProperty &property, QJsonValue value);
    static QJsonDocument jsonObjectFromFile(const QString path);
};

#endif
