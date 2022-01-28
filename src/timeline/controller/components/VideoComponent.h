#ifndef VIDEOCOMPONENT_H
#define VIDEOCOMPONENT_H

#include "BaseComponent.h"
#include <QAbstractListModel>
#include <QModelIndex>


class VideoComponent : public BaseComponent {
    Q_OBJECT
public:
    VideoComponent(QObject *parent = nullptr);
    ~VideoComponent();

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE void setVolume(double volume);

    Q_INVOKABLE void setVolumeByUser(double preVolume, double volume);

    Q_INVOKABLE double getVolume();

    Q_INVOKABLE void resetVideoAdjustmentParams();
    Q_INVOKABLE void resetVideoPictureParams();


    void onOfParamsChanged(QString key, SkyVariant value) override;

public slots:
    bool isVideoClip();

private:
};

#endif // VIDEOCOMPONENT_H
