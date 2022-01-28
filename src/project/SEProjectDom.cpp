#include "SEProjectDom.h"

#include <base/utils/threadpools.h>

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include "SEProject.h"
#include "skymedia/SkyDefs.h"
#include <timeline/venus/AsyncTimelineFrameMgr.h>
#include "src/base/utils/AppPath.h"

static SkyTimeline *createDefaultTimeline() {
    SkyAudioParams audioParams;
    SkyVideoParams videoParams;
    videoParams.width = 1920;
    videoParams.height = 1080;
    QString cacheDirectory = AppPath::skymedia();
    SkyCaches cache(cacheDirectory.toUtf8().data());
    int64_t diskSize = ((int64_t)10) * 1024 * 1024 * 1024;
    cache.setMaxDiskByteSize(diskSize);
    cache.setMaxMemoryByteSize(0);
    return SkyTimeline::create(cache, audioParams, videoParams);
}

SEProjectDom::SEProjectDom() {
    qDebug() << "SEProjectDom::SEProjectDom " << this;

    // 将 timeline 创建时机放到前面，避免一些多线程问题。timeline 创建时候，还没有用到任何 GL 资源，可以在非 GL
    // 环境中创建。
    m_timeline = createDefaultTimeline();
}

SEProjectDom::~SEProjectDom() {
    if (m_timeline != nullptr) {
        SkyTimeline::destory(m_timeline);
        m_timeline = nullptr;
    }
    m_previewController = nullptr;
    m_timelineController = nullptr;
    m_propertyEditController = nullptr;
    m_resourcePreviewController = nullptr;
    qDebug() << "SEProjectDom::~SEProjectDom" << this;
}

SkyTimeline *SEProjectDom::onTimelineFramebufferRendererInitialize() {
    assert(m_timeline);
    AsyncTimelineFrameMgr::getIns()->setTimeline(m_timeline);
    ThreadPools::instance()->mainThread([=] {
        if (SEProject::isQuit()) {
            qDebug() << "SEProject had quit???" << this;
            return;
        }
        if (m_previewController) {
            m_previewController->notifyFrameConfig();
        }
        if (m_timelineController) {
            m_timelineController->fpsChanged();
        }
    });
    return m_timeline;
}

// 原则上说，对象应该是自己管理自己，哪里创建就哪里释放。
// 只是 Timeline 中有可能用到一些 GL 相关的资源，需要在 GL 环境中释放。因而这里的 timeline 由外部触发来销毁。
void SEProjectDom::onTimelineFramebufferRendererDestory() {
    SkyTimeline::destory(m_timeline);
    m_timeline = nullptr;
    AsyncTimelineFrameMgr::getIns()->setTimeline(nullptr);
}

SkyTimeline *SEProjectDom::timeline() {
    return m_timeline;
}

void SEProjectDom::save(QString root_dir) {
    SkyString jsonHolder = m_timeline->saveConfig(root_dir.toUtf8().data()).transToJsonString(true);
    QString json = QString::fromUtf8(jsonHolder.c_str());
    QFile file(SEProject::current()->proj()->entryFile());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(json.toUtf8());
        file.flush();
        file.close();
        qDebug() << "Save File success Path:" << file.fileName() << " Size:" << file.size();
    } else {
        qInfo("SEProjectDom save timeline fail!");
    }
}

void SEProjectDom::restore(QString root_dir) {
    tryPerformRestoreState();
}

bool SEProjectDom::isMute() {
    return m_mute;
}

void SEProjectDom::setMute(bool b) {
    m_mute = b;
}

void SEProjectDom::tryPerformRestoreState() {
    if (m_timeline == nullptr || m_timelineController == nullptr) {
        qDebug() << "Restore Config Wait Timeline INIT:";
        return;
    }
    QString root_dir = SEProject::current()->rootPath();
    QFile file(SEProject::current()->proj()->entryFile());
    if (file.open(QIODevice::ReadOnly)) {
        QString json = QString::fromUtf8(file.readAll());
        file.close();

        SkyVariant variant = SkyVariant::parseFromJsonString(json.toUtf8().data());
        if (variant.isNull()) {
            qDebug() << "Restore Is NULL";
            return;
        }

        qDebug() << "Restore Timeline:" << m_timeline << " Path:" << root_dir;
        bool result = m_timeline->loadConfig(variant, root_dir.toUtf8().data());
        qDebug() << "Restore m_timelineController:" << m_timelineController
                 << " Tracks:" << m_timeline->numberOfTracks() << " Result:" << result;
        m_timelineController->restoreInstanceState();
        qDebug() << "Restore m_timelineController DONE! "
                 << " Tracks:" << m_timeline->numberOfTracks();
    }
}

void SEProjectDom::setPreviewController(PreviewController *controller) {
    m_previewController = controller;
    qDebug() << "DOM:" << this << " setPreviewController:" << controller << " m_previewController"
             << m_previewController;
}

PreviewController *SEProjectDom::previewController() {
    return m_previewController;
}

void SEProjectDom::setTimelineController(TimelineController *controller) {
    m_timelineController = controller;
}
TimelineController *SEProjectDom::timelineController() {
    return m_timelineController;
}

void SEProjectDom::setPropertyEditController(PropertyEditController *controller) {
    m_propertyEditController = controller;
}
PropertyEditController *SEProjectDom::propertyEditController() {
    return m_propertyEditController;
}

void SEProjectDom::setResourcePreviewController(StoreResourcePreviewController *controller) {
    m_resourcePreviewController = controller;
}
StoreResourcePreviewController *SEProjectDom::resourcePreviewController() {
    return m_resourcePreviewController;
}

void SEProjectDom::setEditPageSource(EditorPageDataSource *controller) {
    m_EditPageSource = controller;
}
EditorPageDataSource *SEProjectDom::editPageSource() {
    return m_EditPageSource;
}

void SEProjectDom::setExportSource(void *controller) {
    m_exportSource = controller;
}
void *SEProjectDom::exportSource() {
    return m_exportSource;
}
