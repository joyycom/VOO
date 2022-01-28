#ifndef TRANSITIONCOMPONENT_H
#define TRANSITIONCOMPONENT_H

#include "BaseComponent.h"

class TransitionComponent : public BaseComponent {
    Q_OBJECT
public:
    TransitionComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE double getDuring();

    Q_INVOKABLE void setDuring(double volume);

    Q_INVOKABLE void setDuring(double preDuring, double during);

    Q_INVOKABLE double getMaxDuring();

    Q_INVOKABLE double getMinDuring();

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void duringChanged();

private:
    double m_maxDuring = 2;
    double m_minDuring = 0;
};

#endif // TRANSITIONCOMPONENT_H
