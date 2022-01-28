#ifndef SKYRESOURCEBEAN_H
#define SKYRESOURCEBEAN_H

#include <skymedia/SkyResource.h>
#include <skymedia/SkyTimeline.h>

#include <QJsonDocument>
#include <QString>

#include "TimelineDef.h"

SKYMEDIA_USING_NAMESPACE

class SkyResourceBean {
public:
    SkyResourceBean(QString path = "",
                    QString name = "",
                    ResourceType type = EffectResource,
                    QString thumb = "",
                    QString rewindPath = "",
                    QString resId = "",
                    bool gltf = false);

    SkyResourceBean(const SkyResourceBean &rhs);

    SkyResourceBean(const SkyResource &skyResource);

    QString entryPath() const;
    QString resourceDir() const;

    QString name() const;

    QString thumb() const;

    SkyResource skyResouce() const;

    TrackType trackType() const;

    // BaseClip *makeClipBean(SkyClip *skyClip) const;

    ResourceType resouceType() const;

    QString resourceTypeStr() const{
        return nameFromResourceType(resouceType());
    }

    SkyTimeRange defaultTimeRange() const;

    QString toString() const ;

    bool isEffectResource() const;

    //为了兼容旧的协议 先保留
    SkyVariant toSkyVariant() const;

    //为了兼容旧的协议 先保留
    static SkyResourceBean parseSkyVariant(const SkyVariant variant);

    void saveSkyVariant(SkyObject* obj);

    static SkyResourceBean restoreSkyVariant(SkyObject* obj);

    bool isValidResource() const;



    QJsonDocument getUIConfig() const;

    // QString animationConfigPath() const;

    QString rewindVideoPath() const;

    bool isVenusEffect();

    SkyResourceBean rewindVideoResource(QString videoPath);

    bool isFreezeDuration();
    bool isGLTF();

private:
    QJsonDocument m_uiConfig;
    QString m_resourceId = "";
    QString m_resourceDir = "";
    QString m_entryPath = "";
    QString m_baseDirEntryPath = "";
    QString m_name = "";
    ResourceType m_resourceType = EffectResource;
    QString m_thumb = "";
    SkyResource m_skyResouce;
    QString m_rewindPath = "";
    bool m_isGLTF = false;
};

#endif // SKYRESOURCEBEAN_H
