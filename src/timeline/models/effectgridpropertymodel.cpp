#include "effectgridpropertymodel.h"
#include <QJsonObject>
#include <qlogging.h>
#include <QDebug>

EffectGridItem::EffectGridItem(QJsonObject jsonObj) {
    m_name = jsonObj["name"].toString();
    m_iconURL = jsonObj["icon"].toString();
}

QString EffectGridItem::icon() const {
    return m_iconURL;
}

QString EffectGridItem::name() const {
    return m_name;
}

EffectGridPropertyModel::EffectGridPropertyModel(QObject *parent) : QAbstractListModel(parent) {
}

EffectGridPropertyModel::EffectGridPropertyModel(EffectGridPropertyModel &model) {
    datas.append(model.datas);
}

EffectGridPropertyModel::~EffectGridPropertyModel() {
    datas.clear();
}

int EffectGridPropertyModel::rowCount(const QModelIndex &parent) const {
    qDebug() << "EffectGridPropertyModel: " << this << ".rowCount = " << datas.count();
    return datas.count();
}

QVariant EffectGridPropertyModel::data(const QModelIndex &index, int role) const {
    qDebug() << "EffectGridPropertyModel: data.isValid = " << index.isValid() << " role = " << role;
    if (!index.isValid())
        return QVariant();

    const EffectGridItem &data = datas[index.row()];
    if (role == NameRole) {
        qDebug() << "EffectGridPropertyModel: name role";
        return data.name();
    } else if (role == IconRole) {
        qDebug() << "EffectGridPropertyModel: icon role";
        return data.icon();
    }

    return QVariant();
}

QHash<int, QByteArray> EffectGridPropertyModel::roleNames() const {
    qDebug() << "EffectGridPropertyModel: roleNames ";
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IconRole] = "icon";
    return roles;
}

void EffectGridPropertyModel::setDataWithJSON(QJsonArray jsonArray) {
    datas.clear();
    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject jsonObj = jsonArray[i].toObject();
        EffectGridItem item(jsonObj);
        datas.append(item);
    }
}
