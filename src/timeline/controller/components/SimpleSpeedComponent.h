#ifndef SIMPLE_SPEED_COMPONENT_H
#define SIMPLE_SPEED_COMPONENT_H

#include "BaseComponent.h"

class SimpleSpeedComponent : public BaseComponent {
    Q_OBJECT
public:
    SimpleSpeedComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE double getSpeed();

    Q_INVOKABLE void setSpeed(double speed);

    Q_INVOKABLE void setSpeedByUser(double preSpeed, double speed);

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void simpleSpeedChanged(double speed);

private:
};

#endif // SIMPLE_SPEED_COMPONENT_H
