#ifndef CURVED_SPEED_COMPONENT_H
#define CURVED_SPEED_COMPONENT_H

#include "BaseComponent.h"

#include <QJsonObject>
#include <QList>
#include <QList>
#include <QList>
#include <QModelIndex>
#include <QPoint>
#include <QQmlListProperty>
#include <QVariantList>

class CurvedSpeedBean {
public:
    QString name;
    QString thumb;
    QList<QPointF> orignalPoints;
    QList<QPointF> curPoints;

    CurvedSpeedBean(const QJsonObject &object);

    CurvedSpeedBean(QString n, QString icon, QList<QPointF> points);

    void updatePoints(QList<QPointF> points);
};

class CurvedSpeedListModel : public QAbstractListModel {
    Q_OBJECT
public:
    ~CurvedSpeedListModel();
    enum CurvedSpeedModel {
        NameRole = Qt::UserRole + 1,
        ThumbRole = Qt::UserRole + 2,
        PointsRole = Qt::UserRole + 3,
        SelectedRole = Qt::UserRole + 4
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    CurvedSpeedBean &data(int index);

    CurvedSpeedBean &curData();

    void setDatas(QList<CurvedSpeedBean> data);

    void selectCurvedSpeedBeanByName(QString name, int defaultIndex);

    void setSelectedIndex(int index);

    void updateCurPoints(QList<QPointF> points);

    int curSelectCurvedIndex() {
        return m_selectedIndex;
    }

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<CurvedSpeedBean> m_datas;
    int m_selectedIndex = 0;
};

const static QString OF_PARAMS_SELECT_CURVED_NAME = "Clip:CurvedSpeedName";

class CurvedSpeedComponent : public BaseComponent {
    Q_OBJECT
public:
    CurvedSpeedComponent(QObject *parent = nullptr);

    ~CurvedSpeedComponent();

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE void selectCurvedSpeed(int index);

    Q_INVOKABLE QVariantList bzizerPoints();

    Q_INVOKABLE QVariant listModel();

    Q_INVOKABLE void resetPoints();

    Q_INVOKABLE void updateProgress(double progress);

    Q_INVOKABLE void updatePoints(QVariantList list);

    template <typename T>
    static QVariantList toVariantList(QList<T> list);

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void bzizerPointsChanged(QVariantList points);
    void resourceProgressChanged(double progress);

public slots:
    void onPreviewProgressChanged(double progress);

private:
    void setClipCurvedSpeed(int selectIndex, QList<QPointF> points);

    QList<CurvedSpeedBean> loadCurvedSpeedConfig(QString path);
    CurvedSpeedListModel *m_model = new CurvedSpeedListModel();
};

#endif // CURVED_SPEED_COMPONENT_H
