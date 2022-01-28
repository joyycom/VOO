#ifndef SKYEFFECTWRAPPER_H
#define SKYEFFECTWRAPPER_H

#include "skyresourcebean.h"

#include <skymedia/SkyEffect.h>

#include <QMap>
#include <QObject>
SKYMEDIA_USING_NAMESPACE

#define VENUS_FRONT_EFFECT_PRIORITY 100
#define DEFAULT_EFFECT_PRIORITY 0
#define PIC_IN_PIC_BG_EFFECT_PRIORITY -100
class SkyEffectWrapper : public QObject {
    Q_OBJECT
public:
    SkyEffectWrapper(QObject *ownerClip, SkyResourceBean resource, SkyEffect *effect = nullptr);
    SkyEffectWrapper(QObject *ownerClip, SkyEffect *effect);

    //    SkyEffectWrapper(const SkyEffectWrapper& wraper);

    ~SkyEffectWrapper();

    SkyEffect *addEffect();

    SkyEffect *skyEffect();

    void updateParams(SkyVariant params);

    SkyVariant getParams();

    SkyVariant saveConfig();

    SkyResourceBean &skyResource();

    void removeEffect();

    void onOfMessage(QString msg);

    void saveUserData();

    void restoreUserData();

    QString getOfEffectPath();

    QString toString();

    bool isVenusEffect();

    bool isAudioEffect();

    bool isVideoEffect();

    SkyVariant getKeyFrameParams();


    ResourceType resourceType() {
        return m_resBean.resouceType();
    }

    QMap<QString, SkyVariant> getOfParams() const;

    SkyVariant getOfParamValue(QString key) const;

    SkyVariant getOfParamValue(QString key,const SkyVariant& defVal) const;


    void enableKeyFrame(QString key, double curTime);

    bool isCurKeyFrame(QString key, double curTime);

    bool tryAddKeyFrame(QString key, double curTime, SkyVariant value);

    bool removeKeyFrame(QString key, double time);

    QString keyFrameTweenType(QString key, double curTime);

    QString updateKeyFrameTweenType(QString key, QString tween, double curTime);

    QList<double> getKeyFrameTimes() const;

    QList<double> getKeyFrameTimes(QString key) const;

    QMap<QString, SkyVariant> updateOfParamValues(QMap<QString, SkyVariant> &paramMap,
                                                  QMap<QString, SkyVariant> &historyParams);


    void moveIndex(int targetIndex);

    void trySetGroupInfo(QJsonObject& uiconfig);

    QString getFashionLineGroupInfo();

    void setUserData(QString key,QString value);

    QString getUserData(QString key);

    void onEffectCreated(int contextId,int effectId);

    void saveOfEffectParamFile();


public slots:
    void onTimelinePlayProgressChanged(double progress);

private:
    void registerOfMsgCallback();
    void tryAddVenusProcessor();
    void collectKeyFrameList();
    void tryFixSystemFontPath();

    void sendOfParamChangedEvent(QList<QString> keys);

    QMap<QString, SkyVariant> getOfParamsForce() const;

    mutable QMap<QString, SkyVariant> m_ofParams;
    QList<QString> m_keyFrameKeyList;
    mutable QMap<QString, QVariant> m_lastKeyFrameStates;

    bool m_isVenusEffect = false;
    SkyEffect *m_effect = nullptr;
    QObject *m_ownerClip = nullptr;
    SkyResourceBean m_resBean;
    SkyOFMessageCallback ofCallback;

    int m_ofContextID = -1;
    int m_ofEffectID = -1;
};

#endif // SKYEFFECTWRAPPER_H
