#ifndef SimpleComponent_H
#define SimpleComponent_H

#include "BaseComponent.h"

class SimpleComponent : public BaseComponent {
    Q_OBJECT
public:
    SimpleComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

private:
};

#endif // SimpleComponent_H
