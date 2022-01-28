#ifndef PROPERTYSimpleTransformComponent_H
#define PROPERTYSimpleTransformComponent_H

#include <timeline/models/baseclip.h>
#include <timeline/models/effectgridpropertymodel.h>

#include <QObject>
#include <QSize>

#include "BaseComponent.h"


class SimpleTransformComponent : public BaseComponent {
    Q_OBJECT
public:
    explicit SimpleTransformComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &data) override;
    void onOfMessage(QString msg) override;
    void onUnBind() override;

    void onOfParamsChanged(QString key, SkyVariant value) override;
    void bindClip(BaseClip *clip);
signals:
    void sourceChange(
                      double width,
                      double height,
                      int videoWidth,
                      int videoHeight);
    void uiChange(int transx, int transy, float scaleX,float scaleY);
    void displayTransformView(bool ready, bool inTime);

public slots:
    void changeTranslate(float transx, float transy);
    void changeTranslateX(bool left);
    void changeTranslateY(bool top);
    void changeTranslateOnTouchRelease(float start_transx, float start_transy, float transx, float transy);
    void updateUI();
    void changeScale(float scaleX, float scaleY);


    void changeScaleOnTouchRelease(float startScaleX, float scaleX,float startScaleY, float scaleY);


private:
    bool m_ready = false;
    bool m_init = false;

    double m_curTransX = 0;
    double m_curTransY = 0;
    float m_curScaleX = 1.0;
    float m_curScaleY = 1.0;
    float m_curHeight = 1.0;
    float m_curWidth = 1.0;

    ParamSettingInfo m_paramTransX;
    ParamSettingInfo m_paramTransY;
    ParamSettingInfo m_paramScaleX;
    ParamSettingInfo m_paramScaleY;

    void initParamInfo(QJsonObject &doc);
    void onPreviewProgressChange(double pts);
    void onEditDestory();
};

#endif // PROPERTYSimpleTransformComponent_H
