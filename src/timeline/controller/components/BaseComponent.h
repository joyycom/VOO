#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QDebug>
#include "ComponentBean.h"
#include <timeline/models/baseclip.h>

class BaseComponent : public QObject {
    Q_OBJECT
public:
    explicit BaseComponent(QObject *parent = nullptr);

    ~BaseComponent();

    void bindQml(QJsonObject &data, BaseClip *clip, SkyEffectWrapper* wrapper);
    void ofMessage(QString msg);
    void unBind();

    virtual void onBindQml(QJsonObject &data);
    virtual void onOfMessage(QString msg);
    virtual void onUnBind();

    SkyVariant getOfParamValue(QString key);

    SkyVariant getOfParamValue(QString key, SkyVariant defVal, bool forceRefresh = false);

    bool updateOfParamValueBySilent(QString key, SkyVariant variant);

    bool updateOfParamValuesBySilent(QMap<QString, SkyVariant> paramMap);

    Q_INVOKABLE void updateOfParamIntValueBySilent(QString key, int value);

    bool updateOfParamValueByUser(QString key, SkyVariant variant, SkyVariant preValue = SkyVariant::makeNull());

    bool updateOfParamValuesByUser(QMap<QString, SkyVariant> paramMap, QMap<QString, SkyVariant> preValues);

    Q_INVOKABLE void updateOfParamIntValueByUser(QString key, int value, int preValue);

    void sendOfMessage(QString json);

    Q_INVOKABLE int getOfParamIntValue(QString key, int defValue, bool forceRefresh = false);

    Q_INVOKABLE void updateOfParamVariantValueBySilent(QString key, QVariant value);

    Q_INVOKABLE void updateOfParamVariantValueByUser(QString key, QVariant value, QVariant preValue);

    Q_INVOKABLE QVariant getOfParamVariantValue(QString key, QVariant defValue);

    virtual void onOfParamsChanged(QString key, SkyVariant value);

    virtual void onClipParamsChanged();

    Q_INVOKABLE void enableKeyFrames(QStringList keys, QList<double> defaultValues);

    Q_INVOKABLE void removeKeyFrames(QStringList keys);

    Q_INVOKABLE void enableKeyFrame(QString key, double defaultValue = 0);

    Q_INVOKABLE void removeKeyFrame(QString key);

    Q_INVOKABLE bool isCurKeyFrame(QString key);

    Q_INVOKABLE QString keyFrameTweenType(QString key);

    Q_INVOKABLE void updateKeyFrameTweenType(QString key, QString tween);

    Q_INVOKABLE void updateKeyFrameTweenTypes(QStringList key, QString tween);

    bool isCurKeyFrame(QStringList keys);

    Q_INVOKABLE bool hasInputConfig();

    Q_INVOKABLE QString extraInputConfig();

    Q_INVOKABLE void inputConfigEditingFinished(QString json);

    Q_INVOKABLE bool isEnableInput();

    Q_INVOKABLE bool hadBindComponent() {
        return m_curClip != nullptr;
    }

    Q_INVOKABLE virtual void onComponentShow(){}

    Q_INVOKABLE virtual void onComponentHide(){}

    void setComponentBean(ComponentBean* bean){
        m_componentBean = bean;
    }

    void setTranslations(QJsonObject& bean){
        m_translations = bean;
    }

    QString translate(QString text) const;

protected:
    BaseClip *curClip();
    SkyEffectWrapper* curEffect();

    BaseClip *m_curClip = nullptr;
    SkyEffectWrapper* m_curEffect = nullptr;
    ComponentBean* m_componentBean = nullptr;
    QJsonObject m_translations;

//    void setOfParamsDefaultValues(QMap<QString, SkyVariant> paramMap);

signals:
    void bindComponent();
    void ofParamChanged(QString key);
    void keyFrameStateChanged(QString key);
    void inputStatusChanged(bool enable);
    void inputDeviceChanged();
private slots:
    void ofParamsChanged(BaseClip *clip, QString key, SkyVariant value, SkyEffectWrapper* effect);
    void clipDestroyed(QObject *object);

private:
    void checkKeyFrameChanged(QString key, SkyVariant value);
};

#endif // BASECOMPONENT_H
