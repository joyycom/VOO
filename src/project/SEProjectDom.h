#ifndef SEPROJECTDOM_H
#define SEPROJECTDOM_H

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <timeline/controller/propertyeditcontroller.h>
#include <timeline/controller/timelinecontroller.h>
#include <ui/editor/EditorPageDataSource.h>
#include <ui/preview/PreviewController.h>
#include <ui/store/StoreResourcePreviewController.h>
#include "skymedia/SkyApi.h"
SKYMEDIA_USING_NAMESPACE

class SEProjectDom {
public:
    SEProjectDom();
    ~SEProjectDom();

    void save(QString root_dir);
    void restore(QString root_dir);

    bool isMute();
    void setMute(bool b);

    // timeline
    SkyTimeline *onTimelineFramebufferRendererInitialize();
    void onTimelineFramebufferRendererDestory();
    
    SkyTimeline *timeline();
    // ui
    void setPreviewController(PreviewController *controller);
    PreviewController *previewController();

    void setTimelineController(TimelineController *controller);
    TimelineController *timelineController();

    void setPropertyEditController(PropertyEditController *controller);
    PropertyEditController *propertyEditController();

    void setResourcePreviewController(StoreResourcePreviewController *controller);
    StoreResourcePreviewController *resourcePreviewController();

    void setEditPageSource(EditorPageDataSource *controller);
    EditorPageDataSource *editPageSource();

    void setExportSource(void *controller);
    void *exportSource();

private:
    void tryPerformRestoreState();

private:
    bool m_mute = false;
    SkyTimeline *m_timeline = nullptr;
    PreviewController *m_previewController = nullptr;
    TimelineController *m_timelineController = nullptr;
    PropertyEditController *m_propertyEditController = nullptr;
    StoreResourcePreviewController *m_resourcePreviewController = nullptr;
    EditorPageDataSource *m_EditPageSource = nullptr;
    void *m_exportSource = nullptr;
};
#endif // SEPROJECTDOM_H
