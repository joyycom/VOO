#include "StoreResourcePreviewController.h"
#include "timeline/models/videoclip.h"
#include <timeline/models/effectclip.h>
#include <QDebug>
#include <project/SEProject.h>
#include <base/utils/AppPath.h>
#include "src/base/utils/OrangeFilterUtils.h"

StoreResourcePreviewController::StoreResourcePreviewController(QObject *parent) : QObject(parent) {
    SEProject::current()->dom()->setResourcePreviewController(this);
}

void StoreResourcePreviewController::attachModel() {
    qDebug() << "attachModel:StoreResourcePreviewController";
}

void StoreResourcePreviewController::startPreviewResource(SkyResourceBean &bean) {
    m_previewResource = bean;
    SEProject::current()->dom()->previewController()->stop();
    if (lastPreviewTime < 0) {
        lastPreviewTime = SEProject::current()->dom()->timelineController()->cursorTime();
    } else {
        SEProject::current()->dom()->previewController()->seekVideo(lastPreviewTime);
    }

    if (bean.resouceType() == TransitionResource) {
        startPreviewTransition(bean);
    } else if (bean.isEffectResource()) {
        startPreviewEffect(bean);
    } else if (bean.resouceType() == VideoResource || bean.resouceType() == AudioResource ||
               bean.resouceType() == ImageResource) {
        qDebug() << "StoreResourcePreviewController:startPreviewResource " << bean.toString();
        SEProject::current()->dom()->previewController()->startPreviewVideoResource(bean);
    }
}

void StoreResourcePreviewController::stopPreviewResource(SkyResourceBean &bean) {
    qDebug() << "StoreResourcePreviewController:stopPreviewResource" << bean.toString();

    m_previewResource = SkyResourceBean();
    if (bean.resouceType() == TransitionResource) {
        stopPreviewTransition();
    } else if (bean.isEffectResource()) {
        stopPreviewEffect();
    } else if (bean.resouceType() == VideoResource || bean.resouceType() == AudioResource ||
               bean.resouceType() == ImageResource) {
        SEProject::current()->dom()->previewController()->stopPreview();
    }

    if (lastPreviewTime >= 0) {
        qDebug() << "seekVideo::: m_timeline->getCurrentTime(): " << lastPreviewTime;

        SEProject::current()->dom()->previewController()->seekVideo(lastPreviewTime);
        lastPreviewTime = -1;
    }
}

void StoreResourcePreviewController::repreviewResource() {
    if (m_previewResource.isValidResource()) {
        startPreviewResource(m_previewResource);
    }
}

void StoreResourcePreviewController::startPreviewEffect(SkyResourceBean &bean) {
    QJsonObject jsonObject;
    SkyVariant params = SkyVariant::makeObject();

    QString effectPath = bean.entryPath();
    if (!QFile(effectPath).exists()) {
        qDebug() << "Add Filter Failed: File NOT EXISTS! " << effectPath;
        return;
    }
    SkyTimeRange range = bean.defaultTimeRange();
    double during = range.end - range.begin;
    double currentTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    SkyTimeRange effectRange = SkyTimeRange(currentTime, currentTime + during);
    params.insert("effectPath", SkyVariant::makePath(effectPath.toUtf8().data()));
    if (bean.resouceType() == TextResource) {
        SkyVariant ofParam = SkyVariant::makeObject();
        ofParam.insert("0:SystemFontDir", SkyVariant::makePath(AppPath::systemFontPath().toUtf8().data()));
        ofParam.insert("0:SystemFontNames", OrangeFilterUtils::getLocalTTF().toStdString().c_str());
        params.insert("ofParam", ofParam);
    }

    SkyEffectDescriptor descroptor;
    descroptor.className = SkyOrangeEffectClassName;
    descroptor.params = params;
    descroptor.timeRange = effectRange;

    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (m_skyTrack != nullptr) {
        timeline->removeTrack(m_skyTrack);
    }

    m_skyTrack = timeline->appendVideoTrack();
    SkyClip *clip =
        m_skyTrack->appendClip(SkyResource::makeGap(), SkyTimeRange(0, effectRange.end - effectRange.begin));
    m_skyTrack->placeClip(clip, currentTime, SkyPlaceClipStrategy_Strict);

    auto skyEffect = clip->addEffect(descroptor);
    SkyInputList *inputList = skyEffect->getInputList();
    inputList->ensureNumber(1);
    inputList->getItemAt(0)->setAsBackground();
    inputList->commit();

    SEProject::current()->dom()->previewController()->startPreviewResource(effectRange);

    qDebug() << "add Preview Effect:" << skyEffect << " Path:" << effectPath << " Time:" << currentTime;
}

void StoreResourcePreviewController::startPreviewTransition(SkyResourceBean &bean) {
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    TimelineModel *model = timeline->timelineModel();
    double currentTime = SEProject::current()->dom()->timeline()->getCurrentTime();
    QModelIndex index;
    auto data = model->addTransitionResource(bean, currentTime, index);
    m_preTransitionResource = data.first;
    m_preTransitionResourceIndex = data.second;

    BaseClip *clip = model->data(m_preTransitionResourceIndex);
    if (clip != nullptr && ((VideoClip *)clip)->getTransition() != nullptr) {
        double startTime = clip->startTime();
        double during = ((VideoClip *)clip)->getTransition()->during();
        SkyTimeRange effectRange = SkyTimeRange(startTime, startTime + during);
        SEProject::current()->dom()->previewController()->startPreviewResource(effectRange);
        qDebug() << "startPreviewTransition Success! Resource:" << bean.toString();
    } else {
        qDebug() << "startPreviewTransition Failed! Resource:" << bean.toString();
    }
}

void StoreResourcePreviewController::stopPreviewTransition() {
    if (m_preTransitionResourceIndex.isValid()) {
        TimelineController *timeline = SEProject::current()->dom()->timelineController();
        TimelineModel *model = timeline->timelineModel();
        if (m_preTransitionResource.isValidResource()) {
            model->addTransitionResource(m_preTransitionResource, 0, m_preTransitionResourceIndex);
        } else {
            model->removeTransitionResource(m_preTransitionResource, m_preTransitionResourceIndex);
        }
        m_preTransitionResourceIndex = QModelIndex();
    }
    SEProject::current()->dom()->previewController()->stopPreview();
}

void StoreResourcePreviewController::stopPreviewEffect() {
    if (m_skyTrack != nullptr) {
        SkyTimeline *timeline = SEProject::current()->dom()->timeline();
        m_skyTrack->removeClip(m_skyTrack->getClipAt(0));
        timeline->removeTrack(m_skyTrack);
        m_skyTrack = nullptr;
    }
    SEProject::current()->dom()->previewController()->stopPreview();
}
