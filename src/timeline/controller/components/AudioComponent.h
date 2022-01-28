#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "BaseComponent.h"

class AudioComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(QString labelText READ labelText NOTIFY labelTextChanged)
public:
    AudioComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString labelText() const;
signals:
    void labelTextChanged(QString text);

private:
    QString m_labelText;
};

#endif // AUDIOCOMPONENT_H
