#include "JsonUtils.h"
#include "fileutils.h"
#include <QDebug>

QObject *JsonUtils::fromJson(const QMetaObject &meta, QString &json) {

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "jsonObjectFromFile Error! "
                   << " Error:" << error.errorString();
        return nullptr;
    }
    auto jsonObject = doc.object();
    return fromJson(&meta, jsonObject);
}

QObject *JsonUtils::fromJson(const QMetaObject *meta, QJsonObject &jsonObject) {
    QObject *object = meta->newInstance();

    int propertyStart = QObject::staticMetaObject.propertyCount();
    for (int i = propertyStart; i < meta->propertyCount(); ++i) {
        QMetaProperty property = meta->property(i);
        if (!property.isWritable())
            continue;
        if (!jsonObject.contains(property.name()))
            continue;

        auto value = jsonValueToProperty(object, property, jsonObject.value(property.name()));
        property.write(object, value);
    }
    return object;
}

QVariant JsonUtils::jsonValueToProperty(QObject *object, QMetaProperty &property, QJsonValue value) {
    auto type = property.userType();
    auto typeName = QString(property.typeName());
    if (value.isArray()) {
        // todo array
        return "";
    } else if (value.isObject()) {
        switch (type) {
            default:
                auto jsonObject = value.toObject();
                return QVariant::fromValue(fromJson(QMetaType::metaObjectForType(type), jsonObject));
                break;
        }
        // todo other meta object types
        return "";
    } else {
        // primitive types
        return value.toVariant();
    }
}

QJsonDocument JsonUtils::jsonObjectFromFile(const QString path) {
    QString jsonContent = FileUtils::read(path);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonContent.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "jsonObjectFromFile Error! Path:" << path << " Error:" << error.errorString();
    }
    return doc;
}
