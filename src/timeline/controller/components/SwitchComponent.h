#ifndef SwitchComponent_H
#define SwitchComponent_H

#include "BaseComponent.h"

class SwitchComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(double sliderValue READ switchValue WRITE setSwitchValue NOTIFY switchValueChanged)
public:
    SwitchComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString switchText() const;

    Q_INVOKABLE bool switchValue();

    Q_INVOKABLE void setSwitchValue(bool value);

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void switchValueChanged(bool value);

private:
    QString m_switchText = "";
    ParamSettingInfo m_ofParams;
};

#endif // SwitchComponent_H
