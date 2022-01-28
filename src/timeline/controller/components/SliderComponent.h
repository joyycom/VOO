#ifndef SLIDERCOMPONENT_H
#define SLIDERCOMPONENT_H

#include "BaseComponent.h"

class SliderComponent : public BaseComponent {
    Q_OBJECT
    Q_PROPERTY(double sliderValue READ sliderValue WRITE setSliderValue NOTIFY sliderValueChanged)
public:
    SliderComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE void setSliderValue(double value);

    Q_INVOKABLE void setSliderValue(double preValue, double curVal);

    Q_INVOKABLE double sliderValue();

    Q_INVOKABLE double sliderMaxValue();

    Q_INVOKABLE double sliderMinValue();

    Q_INVOKABLE QString sliderText() const;

    Q_INVOKABLE QString sliderDisplayText(double value);

    Q_INVOKABLE QString sliderDisplayTextPrefix();

    Q_INVOKABLE QString sliderDisplayTextSuffix();

    Q_INVOKABLE double sliderValueFromText(QString text);

    Q_INVOKABLE double stepSize();

    Q_INVOKABLE int decimals();

    Q_INVOKABLE QString sliderKey();

    Q_INVOKABLE QList<QString> sliderKeys();

    Q_INVOKABLE void enableSliderKeyFrames();

    Q_INVOKABLE void removeSliderKeyFrames();

    Q_INVOKABLE bool isCurSliderKeyFrame();

    Q_INVOKABLE bool isKeyFrameEnable();

    Q_INVOKABLE QString sliderKeyFrameTweenType();

    Q_INVOKABLE void updateSliderKeyFrameTweenType(QString tween);

    void onOfParamsChanged(QString key, SkyVariant value) override;

signals:
    void sliderValueChanged(double value);

private:
    bool m_keyFrameEnable = true;
    QString m_sliderText = "";
    QList<ParamSettingInfo> m_sliderParams;
    ParamSettingInfo m_displayParam;
};

#endif // SLIDERCOMPONENT_H
