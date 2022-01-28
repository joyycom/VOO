#ifndef ASYNC_PROCESS_MODEL_H
#define ASYNC_PROCESS_MODEL_H
#include <QAbstractItemModel>
#include <QPair>
#include <QList>

class AsyncProcessModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum StoreResourceListRole { AsyncStartTimeRole = Qt::UserRole + 1, AsyncEndTimeRole = Qt::UserRole + 2 };
    explicit AsyncProcessModel(QObject *parent = nullptr);
    ~AsyncProcessModel();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void updateProgress(double time);

private:
    QList<QPair<double, double>> m_list;
};
#endif
