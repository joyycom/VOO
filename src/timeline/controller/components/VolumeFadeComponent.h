#ifndef VolumeFadeComponent_H
#define VolumeFadeComponent_H

#include "BaseComponent.h"

class VolumeFadeComponent : public BaseComponent {
    Q_OBJECT
public:
    VolumeFadeComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE double getDuring();

    Q_INVOKABLE void setDuring(double during);

    Q_INVOKABLE void setDuringByUser(double preVolume, double volume);

    Q_INVOKABLE double getMaxDuring();

    Q_INVOKABLE double getMinDuring();

    Q_INVOKABLE QString  paramTitle() const{
        return translate(mParamTitle);
    };

    void onClipParamsChanged() override;

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void maxDurationChanged(double during);

    void fadeDuringChanged(double during);


private:

    bool isFadeIn(){
        return mFadeType.compare("FadeIn",Qt::CaseInsensitive) == 0;
    };

    QString mParamTitle = "";
    QString mFadeType = "FadeIn";
    double m_maxDuring = 10;
    double m_minDuring = 0;
};

#endif // VolumeFadeComponent_H
