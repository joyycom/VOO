#ifndef NameComponent_H
#define NameComponent_H

#include "BaseComponent.h"

class NameComponent : public BaseComponent {
    Q_OBJECT
public:
    NameComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE QString labelText() const;

private:
    QString m_name = "";
    QString m_labelName = "";
};

#endif // NameComponent_H
