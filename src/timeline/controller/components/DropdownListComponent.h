#ifndef DropdownListComponent_H
#define DropdownListComponent_H

#include "BaseComponent.h"

class DropdownListComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(double dataIndex READ curDataIndex WRITE selectDataIndex NOTIFY curDataIndexChanged)
    Q_PROPERTY(QStringList dataList READ dataList NOTIFY dataListChanged)

public:
    DropdownListComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString dropdownText() const{
        return m_paramText;
    }

    Q_INVOKABLE int curDataIndex() const{
        return m_curIndex;
    }

    Q_INVOKABLE QStringList dataList() const{
        return m_dataList;
    }

    Q_INVOKABLE void selectDataIndex(int index);

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void curDataIndexChanged(int index);

    void dataListChanged(QStringList list);


private:
    void updateCurDataIndex();

    int m_curIndex = 0;
    QString m_paramText = "";
    QStringList m_dataList;
    QStringList m_dataValueList;
    ParamSettingInfo m_ofParam;
};

#endif // DropdownListComponent_H
