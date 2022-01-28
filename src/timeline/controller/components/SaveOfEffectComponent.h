#ifndef SaveOfEffectComponent_H
#define SaveOfEffectComponent_H

#include "BaseComponent.h"

class SaveOfEffectComponent : public BaseComponent {
    Q_OBJECT
public:
    SaveOfEffectComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString labelText() const;

    Q_INVOKABLE void saveEffect();
signals:

private:
    QString m_labelText;
};

#endif // SaveOfEffectComponent_H
