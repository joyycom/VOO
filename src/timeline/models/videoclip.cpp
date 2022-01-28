#include "videoclip.h"

#include <base/utils/JsonUtils.h>
#include <base/utils/SkyVariantUtil.h>

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QRunnable>
#include <QStandardPaths>
#include <QThread>
#include <QTimer>

#include "TimelineDef.h"
#include "baseclip.h"
#include "src/project/SEProject.h"

VideoClip::VideoClip(const SkyResourceBean &bean, SkyClip *clip) : BaseClip(bean, clip) {
}

void VideoClip::onSaveInstanceState(QMap<QString, SkyVariant> &bundle) {
    BaseClip::onSaveInstanceState(bundle);
    if (m_transitionClip != nullptr) {
        m_transitionClip->resourceBean().saveSkyVariant(m_transitionClip->skyTransition());
        qDebug() << "onSaveInstanceState " << toString() << " skyTransition ";
    }
}

void VideoClip::onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) {
    BaseClip::onRestoreInstanceState(bundle);
    SkyTransition *skyTransition = skyClip()->getTransitionBefore();
    if (skyTransition != nullptr) {
        SkyResourceBean bean = SkyResourceBean::restoreSkyVariant(skyTransition);
        if (!bean.isValidResource()) {
            auto effectPath = QString::fromUtf8(skyTransition->getParams().getString("effectPath", ""));
            bean = effectPath;
        }
        m_transitionClip = new TransitionClip(bean, skyTransition);

        qDebug() << "onRestoreInstanceState " << toString() << " skyTransition " << bean.toString();
    }
}

bool VideoClip::addTransition(SkyResourceBean &resource, double during) {
    QString effectPath = resource.entryPath();
    if (!QFile(effectPath).exists()) {
        qDebug() << "Add Filter Failed: File NOT EXISTS! " << effectPath;
        return false;
    }
    SkyTransitionDescriptor descriptor(resource.name().toUtf8().data());
    SkyVariant params = SkyVariant::makeObject();
    params.insert("effectPath", SkyVariant::makePath(effectPath.toUtf8().data()));

    //    descriptor.beforeDurationScale = 0.5;
    //    descriptor.afterDurationScale = 0.5;
    descriptor.className = SkyOrangeTransitionClassName;
    descriptor.params = params;
    SkyTransition *skyTransition = skyClip()->setTransitionBefore(descriptor);
    qDebug() << "addTransition Clip:" << this << " skyTransition:" << skyTransition << " SkyResourceBean "
             << resource.toString();

    if (skyTransition != nullptr) {
        if (during >= 0) {
            skyTransition->setDuration(during);
        }
        m_transitionClip = new TransitionClip(resource, skyTransition);
        return true;
    }
    return false;
}

bool VideoClip::removeTransition() {
    if (m_transitionClip != nullptr) {
        qDebug() << "removeTransition Clip:" << this << " skyTransition:" << m_transitionClip->skyTransition();
        delete m_transitionClip;
        m_transitionClip = nullptr;
        return true;
    }
    return false;
}

TransitionClip *VideoClip::getTransition() {
    return m_transitionClip;
}

double VideoClip::transitionDuring() const {
    if (m_transitionClip) {
        return m_transitionClip->skyTransition()->getDuration();
    }
    return -1;
}

void VideoClip::setTransitionDuring(double during) {
    if (m_transitionClip != nullptr) {
        m_transitionClip->skyTransition()->setDuration(during);
    }
}

double VideoClip::getMinTransition() const {
    if (m_transitionClip) {
        return m_transitionClip->getMinTransition();
    }
    return 0.1;
}

double VideoClip::getMaxTransition() const {
    if (m_transitionClip) {
        return m_transitionClip->getMaxTransition();
    }
    return 2;
}

void VideoClip::setVideoOverlayEffectScaleParam(SkyEffectWrapper *wrapper) {
    QSize previewSize(SEProject::current()->dom()->timeline()->getVideoParams().width,
                      SEProject::current()->dom()->timeline()->getVideoParams().height);
    QSize videoSize(resourceBean().skyResouce().firstVideoStream().width,
                    resourceBean().skyResouce().firstVideoStream().height);
    QSize newSize;
    double time = startTime();
    //    bool fillScale = !SEProject::current()->dom()->timelineController()->timelineModel()->hasClipInTime(this,
    //    time);
    float ratio = 1;
    float sizeScale = 1;
    if ((float)videoSize.width() / videoSize.height() > (float)previewSize.width() / previewSize.height()) {
        newSize.setWidth(previewSize.width() * ratio);
        newSize.setHeight(newSize.width() * (videoSize.height() / videoSize.width()));
        sizeScale = (double)newSize.width() / (double)videoSize.width();
    } else {
        newSize.setHeight(previewSize.height() * ratio);
        newSize.setWidth(newSize.height() * (videoSize.width() / videoSize.height()));
        sizeScale = (double)newSize.height() / (double)videoSize.height();
    }
    QMap<QString, SkyVariant> ofParam;
    QMap<QString, SkyVariant> historyParam;
    ofParam.insert("0:Scale", sizeScale);
    wrapper->updateOfParamValues(ofParam, historyParam);
}

QJsonDocument VideoClip::getUIConfig() {
    auto wrapper = skyEffectWrapper();
    if (wrapper != nullptr) {
        return wrapper->skyResource().getUIConfig();
    }
    return QJsonDocument();
}

QVariant VideoClip::get(int role) const {
    switch (role) {
        case RoleMinTransitionDuring:
            return getMinTransition();
        case RoleMaxTransitionDuring:
            return getMaxTransition();
        case RoleTransitionDuring:
            return transitionDuring();
        default:
            return BaseClip::get(role);
    }
}

SkyEffect *VideoClip::addDefaultEffect() {
    auto effect = skyEffect();
    if (effect != nullptr) {
        qDebug() << " Exit Default Video Effect Skip!";
        return effect;
    }
    QString beautyEffectPath = QCoreApplication::applicationDirPath().append("/effects/video/video.ofeffect");
    SkyResourceBean resBean(beautyEffectPath, "Video", OverlayResource);
    SkyEffectWrapper *wraper = addEffect(resBean);
    if (wraper != nullptr) {
        setVideoOverlayEffectScaleParam(wraper);
        return wraper->skyEffect();
    }
    return nullptr;
}

SkyEffectWrapper *VideoClip::skyEffectWrapper() const {
    for (auto wraper : m_effectList) {
        if (wraper != nullptr && wraper->resourceType() == OverlayResource) {
            return wraper;
        }
    }
    if (m_effectList.size() > 0) {
        return m_effectList[0];
    }
    return nullptr;
}

bool VideoClip::rewindVideoClip(QString rewindPath) {
    SkyResourceBean resBean = resourceBean();
    //交换资源路径
    SkyResourceBean rewindResBean = resBean.rewindVideoResource(rewindPath);

    SkyResource skyResource = rewindResBean.skyResouce();
    if (!skyResource.isAvailable() || skyResource.getVideoDuration() == 0) {
        return false;
    }

    double cropStart = skyResource.getVideoDuration() - cropEndTime();
    double cropEnd = skyResource.getVideoDuration() - cropStartTime();

    if (skyClip()->updateResource(skyResource, SkyTimeRange(cropStart, cropEnd))) {
        setResourceBean(rewindResBean);
        qDebug() << "rewindVideoClip VideoPath :" << resBean.entryPath() << " ->" << rewindPath;
        qDebug() << "rewindVideoClip Crop : [" << cropStartTime() << " ," << cropEndTime() << "] -> [" << cropStart
                 << "," << cropEnd << "]";
        return true;
    } else {
        qWarning() << "rewindVideoClip updateResource Failed!";
        return false;
    }
}

void VideoClip::updateVideoOverlayEffectScaleParam() {
    auto wrapper = skyEffectWrapper();
    if (wrapper != nullptr) {
        setVideoOverlayEffectScaleParam(wrapper);
    }
}

VideoClip::~VideoClip() {
    //    removeEffect();
    removeTransition();
}
