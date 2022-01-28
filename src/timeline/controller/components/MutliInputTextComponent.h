#ifndef MutliInputTextComponent_H
#define MutliInputTextComponent_H

#include "BaseComponent.h"

class MutliInputTextComponent : public BaseComponent {
    Q_OBJECT
public:
    MutliInputTextComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString labelText() const;

    Q_INVOKABLE void setText(QString preText, QString text);

    Q_INVOKABLE void setTextBySlient(QString text);

    Q_INVOKABLE QString getText();

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void textValueChanged(QString text);

private:
    QString m_labelText = "";
    ParamSettingInfo m_paramInfo;
};

#endif // MutliInputTextComponent_H
