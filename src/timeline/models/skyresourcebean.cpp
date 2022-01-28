#include "skyresourcebean.h"

#include <base/utils/JsonUtils.h>
#include <skymedia/SkyResource.h>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "../../project/SEProject.h"
#include "src/project/SEProjectProj.h"

SKYMEDIA_USING_NAMESPACE

SkyResourceBean::SkyResourceBean( QString path, QString name, ResourceType type, QString thumb, QString rewindPath, QString resId, bool gltf)
    : m_resourceId(resId),
      m_name(name),
      m_resourceType(type),
      m_thumb(thumb),
      m_rewindPath(rewindPath),
      m_isGLTF(gltf) {
    QFileInfo info(path);
    if (info.isDir()) {
        SEProjectProj proj(SEProjectProj::findProjectFile(path));
        if (proj.id().length() > 0) {
            m_entryPath = proj.entryFile();
        }
        m_resourceDir = path;
    } else {
        m_entryPath = path;
        m_resourceDir = info.absoluteDir().path();
    }
    QFileInfo file(m_entryPath);
    if (!file.exists()) {
        QFileInfo file2(SEProject::current()->rootPath().append("/").append(m_entryPath));
        if (file2.exists()) {
            m_baseDirEntryPath = file2.fileName();
            m_resourceDir = file2.absoluteDir().path();
        }
    }
    qDebug() << "SkyResourceBean entryPath:" << m_entryPath << " resourceDir:" << m_resourceDir << " path:" << path <<" isAbsolute:"<<info.isAbsolute();

    if (isEffectResource()) {
        m_skyResouce = SkyResource::makeGap();
    } else {
        QString resourcePath = m_entryPath;
        if (!m_baseDirEntryPath.isEmpty()) {
            resourcePath = m_resourceDir+"/"+m_baseDirEntryPath;
        }
        SkyResource resource(resourcePath.toUtf8().data());
        qDebug() << " SkyResouce: " << resource.isAvailable() << " HasVideo:" << resource.hasVideoStream()
                 << " HasAudio: " << resource.hasAudioStream() << " VideoDuration:" << resource.getVideoDuration()
                 << " AudioDuration:" << resource.getAudioDuration();
        m_skyResouce = resource;
    }
    m_isGLTF = isGLTF();
}

SkyResourceBean::SkyResourceBean(const SkyResourceBean &rhs)
    : m_resourceId(rhs.m_resourceId),
      m_resourceDir(rhs.m_resourceDir),
      m_entryPath(rhs.m_entryPath),
      m_name(rhs.m_name),
      m_resourceType(rhs.m_resourceType),
      m_thumb(rhs.m_thumb),
      m_skyResouce(rhs.m_skyResouce),
      m_rewindPath(rhs.m_rewindPath),
      m_isGLTF(rhs.m_isGLTF){
    QFile file(m_entryPath);
    if (!file.exists()) {
        QFile file2(SEProject::current()->rootPath().append("/").append(m_entryPath));
        if (file2.exists()) {
            m_baseDirEntryPath = file2.fileName();
        }
    }

    //qDebug() << " Copy SkyResouce: " << toString() << " From:"<<rhs.toString() ;
}

SkyResourceBean::SkyResourceBean(const SkyResource &resource) : m_skyResouce(resource) {
    QString path = QString::fromUtf8(resource.getPath().c_str());
    QFileInfo info(path);
    if (resource.isStatic()) {
        if (path.compare("skymedia://type=gap", Qt::CaseInsensitive) == 0) {
            return;
        } else {
            m_resourceType = ImageResource;
        }

    } else {
        if (resource.hasVideoStream()) {
            m_resourceType = VideoResource;
        } else {
            m_resourceType = AudioResource;
        }
    }
    m_entryPath = path;
    m_resourceDir = info.absoluteDir().path();
    m_name = info.fileName();

    qDebug() << " SkyResouce: " << resource.isAvailable() << " HasVideo:" << resource.hasVideoStream()
             << " HasAudio: " << resource.hasAudioStream() << " VideoDuration:" << resource.getVideoDuration()
             << " AudioDuration:" << resource.getAudioDuration();
}

QString SkyResourceBean::entryPath() const {
    if (m_baseDirEntryPath.isEmpty()) {
        return m_entryPath;
    } else {
        return m_baseDirEntryPath;
    }
}

QString SkyResourceBean::resourceDir() const {
    return m_resourceDir;
}

SkyResource SkyResourceBean::skyResouce() const {
    return m_skyResouce;
}

ResourceType SkyResourceBean::resouceType() const {
    return m_resourceType;
}

SkyVariant SkyResourceBean::toSkyVariant() const {
    SkyVariant data = SkyVariant::makeObject();
    // QString path = "", QString name = "", ResourceType type = EffectResource, QString thumb = ""
    data.insert("path", SkyVariant::makePath(m_entryPath.toUtf8().data()));
    data.insert("name", SkyVariant(m_name.toUtf8().data()));
    data.insert("type", SkyVariant(m_resourceType));
    data.insert("thumb", SkyVariant(m_thumb.toUtf8().data()));
    data.insert("rewind", SkyVariant::makePath(m_rewindPath.toUtf8().data()));
    return data;
}

SkyResourceBean SkyResourceBean::parseSkyVariant(SkyVariant data) {
    QString path = QString::fromUtf8(data.getString("path", ""));
    QString name = QString::fromUtf8(data.getString("name", ""));
    ResourceType type = (ResourceType)(data.getInt("type", 1));
    QString thumb = QString::fromUtf8(data.getString("thumb", ""));
    QString rewindPath = QString::fromUtf8(data.getString("rewind", ""));
    return SkyResourceBean(path, name, type, thumb, rewindPath,"");
}

void SkyResourceBean::saveSkyVariant(SkyObject* obj){
    obj->setUserData("ResId", m_resourceId.toUtf8().data());
    obj->setUserData("ResPath", SkyVariant::makePath(m_entryPath.toUtf8().data()));
    obj->setUserData("ResName", SkyVariant(m_name.toUtf8().data()));
    obj->setUserData("ResType", nameFromResourceType(m_resourceType));
    obj->setUserData("ResThumb", SkyVariant(m_thumb.toUtf8().data()));
    obj->setUserData("ResRewind", SkyVariant::makePath(m_rewindPath.toUtf8().data()));
    obj->setUserData("ResIsGLTF", SkyVariant(isGLTF()));
}

SkyResourceBean SkyResourceBean::restoreSkyVariant(SkyObject* obj){
    QString id = QString::fromUtf8(obj->getUserData("ResId").toString());
    QString path = QString::fromUtf8(obj->getUserData("ResPath").toString());
    QString name = QString::fromUtf8(obj->getUserData("ResName").toString());
    ResourceType type = resourceTypeFromName(obj->getUserData("ResType").toString());
    QString thumb = QString::fromUtf8(obj->getUserData("ResThumb").toString());
    QString rewindPath = QString::fromUtf8(obj->getUserData("ResRewind").toString());
    bool isgltf = obj->getUserData("ResIsGLTF").toBool();
    return SkyResourceBean(path, name, type, thumb, rewindPath, id, isgltf);

}

TrackType SkyResourceBean::trackType() const {
    if (m_resourceType == ImageResource || m_resourceType == VideoResource || m_resourceType == TransitionResource) {
        return VideoTrack;
    } else if (m_resourceType == AudioResource) {
        return AudioTrack;
    } else {
        return EffectTrack;
    }
}

SkyTimeRange SkyResourceBean::defaultTimeRange() const{
    if (isEffectResource()) {
        QJsonDocument doc = this->getUIConfig();
        double duration = doc.object().value("duration").toInt(DEF_EFFECT_CLIP_DURING) / 1000.0;
        if (duration <= 0) {
            duration = DEF_EFFECT_CLIP_DURING / 1000.0;
        }
        return SkyTimeRange(0, duration);
    } else if (m_resourceType == ImageResource) {
        return SkyTimeRange(0, DEF_IMAGE_CLIP_DURING);
    } else if (m_resourceType == AudioResource) {
        return SkyTimeRange(0, m_skyResouce.getAudioDuration());
    } else if (m_resourceType == VideoResource) {
        return SkyTimeRange(0, m_skyResouce.getVideoDuration());
    } else {
        return SkyTimeRange(0, DEF_IMAGE_CLIP_DURING);
    }
}

QString SkyResourceBean::name() const {
    return m_name;
}

QString SkyResourceBean::thumb() const {
    return m_thumb;
}

QString SkyResourceBean::rewindVideoPath() const {
    return m_rewindPath;
}

bool SkyResourceBean::isValidResource() const {
    return !m_entryPath.isEmpty();
}

bool SkyResourceBean::isEffectResource() const {
    return m_resourceType == EffectResource || m_resourceType == FilterResource || m_resourceType == StickerResource ||
           m_resourceType == TransitionResource || m_resourceType == TextResource || m_resourceType == MoreResource ||
            m_resourceType == OverlayResource;
}

QString SkyResourceBean::toString() const{
    return "SkyResourceBean:Path:" + m_entryPath + " Name:" + m_name + " Type:" + m_resourceType +" Dir:" + m_resourceDir +
           " During:" + defaultTimeRange().end;
}

QJsonDocument SkyResourceBean::getUIConfig()const {
    if (!m_uiConfig.isNull()) {
        return m_uiConfig;
    }
    QString uiinfoPath = resourceDir() + "/uiinfo.conf";
    if (resouceType() == FilterResource) {
        uiinfoPath = "";
    }
    qDebug() << "QJsonObject:" << uiinfoPath << " EntryPath:" << entryPath();
    if (!QFile::exists(uiinfoPath)) {
        QHash<int, QString> configFilePrefixs;
        configFilePrefixs[FilterResource] = "filter";
        configFilePrefixs[TransitionResource] = "transition";
        configFilePrefixs[EffectResource] = "effect";
        configFilePrefixs[AudioResource] = "audio";
        configFilePrefixs[VideoResource] = "video";
        configFilePrefixs[StickerResource] = "sticker";
        configFilePrefixs[TextResource] = "words";
        configFilePrefixs[ImageResource] = "video";
        configFilePrefixs[OverlayResource] = "video";
        QString prefix = configFilePrefixs[resouceType()];
        uiinfoPath =
            QCoreApplication::applicationDirPath().append("/Configs/default_").append(prefix).append("_uiinfo.conf");
    }
    QJsonDocument config = JsonUtils::jsonObjectFromFile(uiinfoPath);
    return config;
}

bool SkyResourceBean::isVenusEffect() {
    auto config = getUIConfig();
    if (config.isNull()) {
        return false;
    }
    auto venusObj = config.object().value("venus");
    return venusObj.isArray() && venusObj.toArray().size() > 0;
}

//改网络下载后不需要获取这个地址文件
// QString SkyResourceBean::animationConfigPath() const {
//    if (m_resourceType == ImageResource || m_resourceType == VideoResource) {
//        return QCoreApplication::applicationDirPath().append("/Configs/animations_video.conf");
//    } else if (m_resourceType == StickerResource) {
//        return QCoreApplication::applicationDirPath().append("/Configs/animations_sticker.conf");
//    } else if (m_resourceType == TextResource) {
//        return QCoreApplication::applicationDirPath().append("/Configs/animations_text.conf");
//    } else {
//        return "";
//    }
//}

SkyResourceBean SkyResourceBean::rewindVideoResource(QString videoPath) {
    return SkyResourceBean(videoPath, name(), VideoResource, thumb(), entryPath(), m_resourceId);
}

bool SkyResourceBean::isFreezeDuration() {
    return getUIConfig().object().value("freezeDuration").toInt(0);
}

bool SkyResourceBean::isGLTF() {
    return getUIConfig().object().value("isGLTF").toInt(0);
}
