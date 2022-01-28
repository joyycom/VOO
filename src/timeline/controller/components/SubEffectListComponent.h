#ifndef SubEffectListComponent_H
#define SubEffectListComponent_H

#include "BaseComponent.h"
#include <QAbstractListModel>
#include <QModelIndex>

class SubEffectBean{
public:
    SkyEffectWrapper* effectWrapper;
    ComponentBean* componentBean;

    SubEffectBean(SkyEffectWrapper* wrapper,ComponentBean* component):effectWrapper(wrapper),componentBean(component){

    }
};


static QList<QString> s_subEffectSupportUIType = {"Slider","Image","ColorSelector","Text","EditTransform","Switch","DropdownList","EffectInputList","EffectGroupEditor"};

class EffectListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum EffectRole {
        IconRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        PathRole = Qt::UserRole + 3,
        EffectTypeRole = Qt::UserRole + 4,
        QmlRole = Qt::UserRole + 5,
        IsSelectedRole = Qt::UserRole + 6,
        QmlHeightRole = Qt::UserRole + 7,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return m_datas.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setSelectedIndex(int index);

    void updateData(const QList<SubEffectBean> &data);

    void insertResource(int index, SubEffectBean &data);

    void moveResource(int index,int toIndex);

    void removeResource(int index);

    SubEffectBean &data(int index);

    int curSelectedIndex() const{
        return m_curSelectedIndex;
    }

    void clearAll();


protected:
    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[IconRole] = "icon";
        roles[NameRole] = "name";
        roles[PathRole] = "path";
        roles[EffectTypeRole] = "effectType";
        roles[QmlRole] = "qml";
        roles[IsSelectedRole] = "isSelected";
        roles[QmlHeightRole] = "qmlHeight";
        return roles;
    }

private:
    QList<SubEffectBean> m_datas;
    int m_curSelectedIndex = -1;
};

class SubEffectListComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(EffectListModel *effectModel READ effectModel NOTIFY effectListChanged)
public:
    SubEffectListComponent(QObject *parent = nullptr);
    ~SubEffectListComponent();

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE EffectListModel *effectModel();

    Q_INVOKABLE void setSelected(int index);

    Q_INVOKABLE void removeSubEffect(int index);

    Q_INVOKABLE void attachComponent(int index, QObject* component);

    Q_INVOKABLE void moveSubEffect(int index, int targetIndex);

    Q_INVOKABLE void cleanSelected();

    Q_INVOKABLE void setContentY(int contentY);

    void onComponentHide() override;

    void onComponentShow() override;

    void onOfParamsChanged(QString key, SkyVariant value) override;
signals:
    void effectListChanged();

    void contentYChanged(int contentY);

private:
    void updateEffectList();

    ComponentBean* makeComponentBean(QJsonObject& jsonObj);

    int m_lastSelectedIndex = -1;
    int m_lastContentY = -1;

    QString m_lastSelectedKey = "";
    EffectListModel *m_effectModel = new EffectListModel;
};

#endif // SubEffectListComponent_H
