#pragma once

#include <QtQuick/QQuickFramebufferObject>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QOpenGLFunctions_2_1>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLContext>
#include <memory>
#include <QThread>
#include <QOffscreenSurface>
#include "skymedia/SkyApi.h"
#include "skymedia/SkyTimelineRenderer.h"
#include <QOpenGLVertexArrayObject>
#include "src/base/utils/ThreadWroker.h"
#include <mutex>
SKYMEDIA_USING_NAMESPACE

#ifdef Q_OS_MAC
#define Sky_OpenGLFunctions QOpenGLFunctions_2_1
#else
#define Sky_OpenGLFunctions QOpenGLFunctions_3_3_Core
#endif

class OffscreenGLContext final : public QObject {
public:
    OffscreenGLContext(QOpenGLContext *mainContext, QThread *thread, bool shareContext = true) {
        m_renderContext = new QOpenGLContext();
        m_renderContext->setFormat(mainContext->format());
        if (shareContext) {
            m_renderContext->setShareContext(mainContext);
        }
        m_renderContext->create();
        m_renderContext->moveToThread(thread);

        m_renderSurface = new QOffscreenSurface();
        m_renderSurface->setFormat(mainContext->format());
        m_renderSurface->create();

#ifdef Q_OS_WIN
        m_vao = new QOpenGLVertexArrayObject();
        m_vao->create();
        m_vao->bind();
#endif
    }

    OffscreenGLContext(QSurfaceFormat &format, QThread *thread) {
        m_renderContext = new QOpenGLContext();
        m_renderContext->setFormat(format);
        m_renderContext->create();
        m_renderContext->moveToThread(thread);

        m_renderSurface = new QOffscreenSurface();
        m_renderSurface->setFormat(format);
        m_renderSurface->create();

#ifdef Q_OS_WIN
        m_vao = new QOpenGLVertexArrayObject();
        m_vao->create();
        m_vao->bind();
#endif
    }

    ~OffscreenGLContext() {
        if (m_vao) {
            m_vao->destroy();
            delete m_vao;
        }

        if (m_renderSurface) {
            m_renderSurface->destroy();
            delete m_renderSurface;
        }

        if (m_renderContext) {
            delete m_renderContext;
        }
    }

    void makeCurrent() {
        m_renderContext->makeCurrent(m_renderSurface);
        if (m_vao) {
            m_vao->bind();
        }
    }

    void doneCurrent() {
        m_renderContext->doneCurrent();
    }

private:
    QOpenGLContext *m_renderContext = nullptr;
    QOffscreenSurface *m_renderSurface = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
};

class SkyTimelineView;
class SkyTimelineFramebufferRenderer : public QQuickFramebufferObject::Renderer, protected Sky_OpenGLFunctions {
public:
    SkyTimelineFramebufferRenderer(double frameRate);
    ~SkyTimelineFramebufferRenderer();

    virtual void render() override;
    virtual QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    virtual void synchronize(QQuickFramebufferObject *owner) override;

    SkyTimeline *timeline() const {
        return m_timeline;
    }

    void setVideoFrame(SkyVideoFrame frame) {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_lastVideoFrame = frame;
    }

    SkyVideoFrame getVideoFrame() {
        std::lock_guard<std::mutex> locker(m_mutex); // read write lock
        return m_lastVideoFrame;
    }

    SkyVideoFrame doRenderLoopOffscreen();

    ThreadWroker *getRenderThread() {
        return m_renderThread;
    }

    void setPlayRange(SkyTimeRange &range, int loops);
    SkyTimeRange getPlayRange();

    QImage snapFrame(int type = 0, double time = 0);
    void outputVideo(int64_t videoBitRate, QString videoPath, SkyEncodingCallback callback);
    void destoryTimeline(SkyTimeline *timeline);
    void setPlayState(SkyPlayState playState);

private:
    void doInitialize(ThreadWroker *renderThread, double frameRate);

public:
    SkyListenerID mSkyListener;
    SkyTimeline *m_timeline = nullptr;
    SkyTimelineRenderer *m_renderder = nullptr;

    struct PlayLoopInfo {
        int numberOfLoops = 1;
        SkyTimeRange playRange = {0, -1};
        SkyPlayState playState = SkyPlayState_Pause;
    } m_playLoopInfo;
    int m_loopIndex = 0;
    double m_lastTimerTime = 0;

    std::mutex m_mutex;
    SkyVideoFrame m_lastVideoFrame;
    OffscreenGLContext *m_renderContext = nullptr;
    ThreadWroker *m_renderThread = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
};

////////////////////////////////////////
class SkyTimelineView : public QQuickFramebufferObject {
    Q_OBJECT
public:
    SkyTimelineView(QQuickItem *parent = nullptr);
    ~SkyTimelineView();

    virtual QQuickFramebufferObject::Renderer *createRenderer() const override {
        return new SkyTimelineFramebufferRenderer(m_frameRate);
    }

    void initTimer();
    void play();
    QImage snapFrame(int type = 0, double time = 0);
    void outputVideo(int64_t videoBitRate, QString videoPath, SkyEncodingCallback callback);

    void playRange(SkyTimeRange &range, int loops);
    bool isPlayEnd();
    void stopTimer();
    void destoryTimeline(SkyTimeline *timeline);

    SkyPlayState getPlayState() const {
        return m_playState;
    }

    void setRenderder(SkyTimelineFramebufferRenderer *renderder) {
        m_renderer = renderder;
    }

    ThreadWroker *getRenderThread() {
        return m_renderer->getRenderThread();
    }

    void refreshView();

signals:
    void imageReady();
    void progressUpdated();
    void playEnd();

public slots:
    void stop();
    void setMute(bool b);
    void onTimerFired();

private:
    QTimer *m_timer = nullptr;
    double m_frameRate = 30;
    SkyPlayState m_playState = SkyPlayState_Pause;
    SkyTimelineFramebufferRenderer *m_renderer = nullptr;
};
