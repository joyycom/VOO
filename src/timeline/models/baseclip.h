#pragma once

#include <QList>
#include <QObject>
#include <QPointF>

#include "SkyEffectWrapper.h"
#include "TimelineDef.h"
#include "asyncprocessmodel.h"
#include "inputDevice.h"
#include "skymedia/SkyTimeline.h"
#include "skyresourcebean.h"
SKYMEDIA_USING_NAMESPACE

class BaseComponent;
class BaseClip : public QObject {
    Q_OBJECT
public:
    explicit BaseClip(const SkyResourceBean &bean, SkyClip *clip = nullptr);
    ~BaseClip();

    virtual QVariant get(int role) const;

    virtual double during() const;
    virtual double sourceDuring() const;

    double startTime() const;
    double endTime() const;
    double cropStartTime() const;
    double cropEndTime() const;
    virtual double resouceTime() const;

    SkyResourceBean resourceBean() const;
    QString entryPath() const;

    virtual ClipType clipType() const {
        return VideoClipType;
    };

    QString wordContent() const;

    void setResourceBean(const SkyResourceBean bean);

    virtual QString toString() const;

    virtual QJsonDocument getUIConfig();

    SkyClip *skyClip() const {
        return m_skyClip;
    }

    SkyEffect *skyEffect();

    virtual SkyEffectWrapper *skyEffectWrapper() const;

    virtual SkyEffectWrapper *addEffect(SkyResourceBean &resource);

    SkyEffectWrapper *addEffectFromConfig(SkyResourceBean &resource, SkyVariant& config);


    SkyEffectWrapper *findEffect(const QString effectPath);

    virtual SkyEffect *addDefaultEffect();

    virtual bool removeEffect(QString effectPath);

    QVariantList getKeyFrameTimes() const;

    QStringList getGroupInfos() const;

    QList<SkyEffectWrapper *> effectList();


    QList<SkyEffectWrapper *> findEffectListByType(ResourceType type);

    SkyVariant backupClipConfig() const;

    void recoverClipConfig(const SkyVariant& config);

    void saveInstanceState();
    void restoreInstanceState();

    void setCurvedPointsList(const QList<QPointF> &points);

    QList<QPointF> getCurvedPoints() const;

    virtual double getPlayPTS(double sourcePts);

    virtual double getSourcePTS(double playPts);

    void setSpeed(double speed);

    double getSpeed() const;

    double getCurvedSpeed() const;

    double getAvgSpeed() const;

    void attachAsyncProcessModel(AsyncProcessModel *model);

    void updateAsyncProgress(double progress);

    bool hasVenusEffect() {
        return m_hasVenusEffect;
    }

    qint64 getInputId() const;

    void setInputId(qint64 id);

    void removeInputDevice();

    bool equalsInputDeviceId(qint64 id);

    QString getSelectCurvedSpeedName();

    void setSelectCurvedSpeedName(QString name);

    void loadSkyEffect();

    bool moveEffectPos(int fromIndex,int toIndex);

    void updateCurFocusEffect(SkyEffectWrapper* effect);


    double getVolumeFadeInDuring(){
        return mVolumeFadeInDuring;
    }

    double getVolumeFadeOutDuring(){
        return mVolumeFadeOutDuring;
    }

    void setVolumeFadeInDuring(double during);

    void setVolumeFadeOutDuring(double during);

    void updateVolumeFadeEffectParam();

    QString updateResource(QString resourcePath);

public slots:
    void onWordContentChange(QString wordContent);

protected:
    SkyResourceBean m_resBean;
    QList<SkyEffectWrapper *> m_effectList;
    SkyEffectWrapper* m_curFocusEffect = nullptr;
    SkyEffect* m_curAudioEffect = nullptr;

    virtual void onSaveInstanceState(QMap<QString, SkyVariant> &bundle);
    virtual void onRestoreInstanceState(QMap<QString, SkyVariant> &bundle);

public:
signals:
    void clipDataChanged(BaseClip *clip, QVector<int> roles);

    void ofParamChanged(BaseClip *clip, QString key, SkyVariant value, SkyEffectWrapper* effect = nullptr);

    void inputDeviceChanged();

public:
    //用于文本动态宽高
    int ofWidth = -1;
    int ofHeight = -1;

private:

    void addAudioEffect();

    bool m_hasVenusEffect = false;
    SkyClip *m_skyClip = nullptr;
    AsyncProcessModel *m_asyncProcessModel = nullptr;
    qint64 m_inputDeviceId = 0;
    QString m_wordContent = nullptr;
    double mVolumeFadeInDuring = 0;
    double mVolumeFadeOutDuring = 0;
};
