#ifndef SIMPLELISTMODEL_H
#define SIMPLELISTMODEL_H

#include <QAbstractListModel>
#include <functional>
#include <QList>
#include <QHash>
#include <QDebug>

template <class T>
class SimpleListModel : public QAbstractListModel {

public:
    using CallbackT = std::function<QVariant(const T &, int)>;
    SimpleListModel();
    SimpleListModel(QList<T> data, QHash<int, QByteArray> roleNames, const CallbackT &roleValueCallback);
    //    SimpleListModel(SimpleListModel<T> const &model);
    // SimpleListModel &operator=(SimpleListModel<T> const &/*r*/hs);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setDatas(QList<T> data);
    void setRoleNames(QHash<int, QByteArray> roleNames);
    void setRoleValueCallback(const CallbackT &roleValueCallback);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<T> m_datas;
    QHash<int, QByteArray> m_roleNames;
    CallbackT m_roleValueCallback;
};

#include "simplelistmodel.h"

template <class T>
SimpleListModel<T>::SimpleListModel() {
}

template <class T>
SimpleListModel<T>::SimpleListModel(QList<T> data,
                                    QHash<int, QByteArray> roleNames,
                                    const CallbackT &roleValueCallback) {
    m_datas = data;
    m_roleNames = roleNames;
    m_roleValueCallback = roleValueCallback;
}

// template <class T>
// SimpleListModel<T>::SimpleListModel(const SimpleListModel& model) {
//    m_datas = model.m_datas;
//    m_roleNames = model.m_roleNames;
//    m_roleValueCallback = model.m_roleValueCallback;
//}

// template <class T>
// template <class T>
// SimpleListModel<T> & SimpleListModel<T>::operator=(const SimpleListModel<T>& model) {
//    return SimpleListModel<T>(model);
//}

template <class T>
int SimpleListModel<T>::rowCount(const QModelIndex &parent) const {
    qDebug() << "chengj: rowCount = " << m_datas.count();
    return m_datas.count();
}

template <class T>
QVariant SimpleListModel<T>::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_datas.count())
        return QVariant();

    const T &data = m_datas[index.row()];
    return m_roleValueCallback(data, role);
}

template <class T>
QHash<int, QByteArray> SimpleListModel<T>::roleNames() const {
    return m_roleNames;
}

template <class T>
void SimpleListModel<T>::setDatas(QList<T> data) {
    m_datas = data;
}

template <class T>
void SimpleListModel<T>::setRoleNames(QHash<int, QByteArray> roleNames) {
    m_roleNames = roleNames;
}

template <class T>
void SimpleListModel<T>::setRoleValueCallback(const CallbackT &roleValueCallback) {
    m_roleValueCallback = roleValueCallback;
}

#endif // SIMPLELISTMODEL_H
