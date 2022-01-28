#ifndef LABELCOMPONENT_H
#define LABELCOMPONENT_H

#include "BaseComponent.h"

class LabelComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(QString labelText READ labelText NOTIFY labelTextChanged)
public:
    LabelComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString labelText() const;
signals:
    void labelTextChanged(QString text);

private:
    QString m_labelText;
};

#endif // LABELCOMPONENT_H
