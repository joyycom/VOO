#ifndef VOLUMECOMPONENT_H
#define VOLUMECOMPONENT_H

#include "BaseComponent.h"

class VolumeComponent : public BaseComponent {
    Q_OBJECT
public:
    VolumeComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE double getVolume();

    Q_INVOKABLE void setVolume(double volume);

    Q_INVOKABLE void setVolumeByUser(double preVolume, double volume);

    Q_INVOKABLE double getMaxVolume();

    Q_INVOKABLE double getMinVolume();

signals:

private:
    double m_maxVolume = 2;
    double m_minVolume = 0;
};

#endif // VOLUMECOMPONENT_H
