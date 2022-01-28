#ifndef Layout_COMPONENT_H
#define Layout_COMPONENT_H

#include <QObject>
#include <QJsonObject>
#include <QMap>

#include <timeline/models/baseclip.h>
#include "BaseComponent.h"

class BaseLayoutComponent:public BaseComponent{
    Q_OBJECT
public:
    BaseLayoutComponent(QObject *parent = nullptr);

    void onUnBind() override;

    Q_INVOKABLE int count();

    Q_INVOKABLE QString childQml(int index);

    Q_INVOKABLE void attachComponent(int index, QObject *component);


    void onComponentShow() override;

    void onComponentHide() override;
};


class StackLayoutComponent : public BaseLayoutComponent {
    Q_OBJECT
public:

    void onBindQml(QJsonObject &data) override;

    Q_INVOKABLE QString subTitle(int index);

    Q_INVOKABLE void setCurIndex(int index);
    Q_INVOKABLE int getCurIndex();
private:

    QString m_cacheTitleKey = "";
    int m_curIndex = 0;
};

class ColumnLayoutComponent : public BaseLayoutComponent {
    Q_OBJECT
public:


    Q_INVOKABLE int childItemHeight(int index);
private:

};

#endif // Layout_COMPONENT_H
