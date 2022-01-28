#ifndef PROPERTYTRANSFORMCOMPONENT_H
#define PROPERTYTRANSFORMCOMPONENT_H

#include <timeline/models/baseclip.h>
#include <timeline/models/effectgridpropertymodel.h>

#include <QObject>
#include <QSize>

#include "BaseComponent.h"

struct TransformSelect {
    QRect *rect;
    int rotate;
};
const QString PARAM_KEY_TRANS_X = "TransX";
const QString PARAM_KEY_TRANS_Y = "TransY";
const QString PARAM_KEY_SCALE_X = "ScaleX";
const QString PARAM_KEY_SCALE_Y = "ScaleY";
const QString PARAM_KEY_ROTATE = "Rotate";

class TransformComponent : public BaseComponent {
    Q_OBJECT
public:
    explicit TransformComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &data) override;
    void onOfMessage(QString msg) override;
    void onUnBind() override;

    void onOfParamsChanged(QString key, SkyVariant value) override;
    void bindClip(BaseClip *clip);
    TransformSelect getSelectRect(QJsonObject &doc, int previewWidth, int previewHeight);
signals:
    void sourceChange(bool isEffects,
                      double x,
                      double y,
                      double widthScale,
                      double heightScale,
                      int videoWidth,
                      int videoHeight);
    void uiChange(int transx, int transy, float scaleX,float scaleY, float rotate);
    void displayTransformView(bool ready, bool inTime);

public slots:
    void changeRotate(float rotate);
    void changeTranslate(float transx, float transy);
    void changeTranslateX(bool left);
    void changeTranslateY(bool top);
    void changeScale(float scaleX, float scaleY);


    void changeScaleOnTouchRelease(float startScaleX, float scaleX,float startScaleY, float scaleY);

    void changeRotateOnTouchRelease(float startRotate, float rotate);
    void changeTranslateOnTouchRelease(float start_transx, float start_transy, float transx, float transy);
    void updateSourceChanged();

    void updateUIChanged();

    double maxTransX() {
        return m_paramTransX.maxValue;
    }
    double minTransX() {
        return m_paramTransX.minValue;
    }

    double maxTransY() {
        return m_paramTransY.maxValue;
    }
    double minTransY() {
        return m_paramTransY.minValue;
    }

    double maxScaleX() {
        return m_paramScaleX.maxValue;
    }

    double minScaleX() {
        return m_paramScaleX.minValue;
    }

    double maxScaleY() {
        return m_paramScaleY.maxValue;
    }
    double minScaleY() {
        return m_paramScaleY.minValue;
    }

private:
    bool m_ready = false;
    bool m_init = false;
    double m_aspect = 1;
    double m_ratio = -1;
    double centerPointX = 0.5;
    double centerPointY = 0.5;
    double m_curTransX = 0;
    double m_curTransY = 0;
    float m_curRotate = 0;
    float m_curScaleX = 1.0;
    float m_curScaleY = 1.0;
    ParamSettingInfo m_paramTransX;
    ParamSettingInfo m_paramTransY;
    ParamSettingInfo m_paramScaleX;
    ParamSettingInfo m_paramScaleY;
    ParamSettingInfo m_paramRotate;

    void initParamInfo(QJsonObject &doc);
    void onPreviewProgressChange(double pts);
    void onEditDestory();
};

#endif // PROPERTYTRANSFORMCOMPONENT_H
