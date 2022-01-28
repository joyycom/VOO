#include "asyncprocessmodel.h"

#include <QDebug>

AsyncProcessModel::AsyncProcessModel(QObject *parent) : QAbstractListModel(parent) {
}

AsyncProcessModel::~AsyncProcessModel() {
    m_list.clear();
    qInfo()<<"AsyncProcessModel::~AsyncProcessModel"<<this;
}

int AsyncProcessModel::rowCount(const QModelIndex &parent) const {
    return m_list.count();
}

QVariant AsyncProcessModel::data(const QModelIndex &index, int role) const {
    QPair<double, double> item = m_list.at(index.row());
    if (role == AsyncStartTimeRole) {
        return item.first;
    } else if (role == AsyncEndTimeRole) {
        return item.second;
    }
    return QVariant();
}

QHash<int, QByteArray> AsyncProcessModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AsyncStartTimeRole] = "asyncStartTime";
    roles[AsyncEndTimeRole] = "asyncEndTime";
    return roles;
}

void AsyncProcessModel::updateProgress(double time) {
    int preSize = m_list.size();
    if (preSize > 0) {
        beginRemoveRows(QModelIndex(), 0, preSize - 1);
        m_list.clear();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), 0, 1);
    m_list.append(QPair<double, double>(0, 0));
    m_list.append(QPair<double, double>(time, -1));
    endInsertRows();
}
