#ifndef EFFECTGRIDPROPERTYMODEL_H
#define EFFECTGRIDPROPERTYMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>

class EffectGridItem {
public:
    //    EffectGridItem(QString localPath);
    EffectGridItem(QJsonObject jsonObj);

    QString name() const;
    QString icon() const;

private:
    QString m_name;
    QString m_iconURL;
};

class EffectGridPropertyModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum EffectGridItemRole { NameRole = Qt::UserRole + 1, IconRole = Qt::UserRole + 2 };

    explicit EffectGridPropertyModel(QObject *parent = nullptr);
    EffectGridPropertyModel(EffectGridPropertyModel &model);
    ~EffectGridPropertyModel();
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setDataWithJSON(QJsonArray jsonArray);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<EffectGridItem> datas;
};

#endif // EFFECTGRIDPROPERTYMODEL_H
