#include "SkyTimelineView.h"
#include <QTimer>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <src/project/SEProject.h>
#include <QDebug>
#include <QOffscreenSurface>
#include <ui/export/ExportDialogDataSource.h>
#include "SkyQtAudioOutput.h"
#include "src/base/utils/ScopeGuard.h"
#include <QImage>
#include "src/base/utils/AppPath.h"

#ifdef Q_OS_WIN
#include "src/glhook/GLewHook.h"
#endif

inline static double getCurrentTime() {
    return QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

void onTimelineNeedsDisplay(void *userData) {
    // TODO:
    SEProject::current()->dom()->previewController()->refreshPreview();
}

SkyTimelineFramebufferRenderer::SkyTimelineFramebufferRenderer(double frameRate) {
    m_renderThread = new ThreadWroker(100);
    m_renderThread->start();

    doInitialize(m_renderThread, frameRate);
}

SkyTimelineFramebufferRenderer::~SkyTimelineFramebufferRenderer() {
    delete m_vao;
    m_vao = nullptr;

    delete m_renderThread;
    m_renderThread = nullptr;

    delete m_renderder;
    m_renderder = nullptr;

    delete m_renderContext;
    m_renderContext = nullptr;

    m_lastVideoFrame = SkyVideoFrame();
    if (mSkyListener.isAvailable() && m_timeline != nullptr) {
        m_timeline->removeListener(mSkyListener);
    }
    
    SEProject::current()->dom()->onTimelineFramebufferRendererDestory();
    m_timeline = nullptr;
}

void SkyTimelineFramebufferRenderer::render() {
    auto skytimelineview = SEProject::current()->dom()->previewController()->getSkyTimelineView();
    if (!skytimelineview || !skytimelineview->isVisible()) {
        return;
    }
    SkyVideoFrame frame;
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        frame = m_lastVideoFrame;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //(0.08f, 0.09f, 0.1f, 1.0f); //#15171C
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto frameBuffer = framebufferObject();
    if (m_renderder && frame.isAvailable()) {
        SkyRenderParams renderParams;
        renderParams.surfaceWidth = frameBuffer->width();
        renderParams.surfaceHeight = frameBuffer->height();
        renderParams.minSamplerType = SkySamplerType_Area;
        m_renderder->renderFrame(renderParams, frame);
    }
    glFinish();
}

void SkyTimelineFramebufferRenderer::setPlayState(SkyPlayState playState) {
    std::lock_guard<std::mutex> locker(m_mutex);
    if (m_playLoopInfo.playState != playState) {
        m_playLoopInfo.playState = playState;
        if (m_renderder) {
            m_renderder->setState(playState);
        }
        m_lastTimerTime = getCurrentTime();
    }
}

QImage SkyTimelineFramebufferRenderer::snapFrame(int type, double time) {
    return m_renderThread->sync([this, type, time] {
        m_renderContext->makeCurrent();
        glGetError();
        SkyVideoFrame frame;
        double currentTime = m_timeline->getCurrentTime();
        if (type == 0) {
            frame = getVideoFrame();
        } else if (type == 1) {
            m_timeline->seekTo(0);
            frame = m_timeline->snapCurrentVideo();
        } else if (type == 2) {
            m_timeline->seekTo(m_timeline->getVideoDuration() - 0.0000001);
            frame = m_timeline->snapCurrentVideo();
        } else if (type == 3) {
            m_timeline->seekTo(time);
            frame = m_timeline->snapCurrentVideo();
        }
        if (type > 0) {
            m_timeline->seekTo(currentTime);
        }
        SkyVideoFrame rgbaFrame = m_timeline->transToRGBA8Frame(frame);
        glFinish();
        QImage image(rgbaFrame.getRGBA8Data(), rgbaFrame.getWidth(), rgbaFrame.getHeight(), QImage::Format_RGBA8888);
        m_renderContext->doneCurrent();
        return image.copy();
    });
}

void SkyTimelineFramebufferRenderer::outputVideo(int64_t videoBitRate,
                                                 QString videoPath,
                                                 SkyEncodingCallback callback) {
    m_renderThread->sync([this, videoBitRate, videoPath, callback] {
        m_renderContext->makeCurrent();
        glGetError();
        SkyEncodingParams encodingParams;
        encodingParams.audioParams = m_timeline->getAudioParams();
        encodingParams.videoParams = m_timeline->getVideoParams();
        if (videoBitRate > 0) {
            encodingParams.videoBitRate = videoBitRate;
        }
        double currentTime = m_timeline->getCurrentTime();
        m_timeline->exportVideo(videoPath.toUtf8().data(), encodingParams, callback);
        m_timeline->seekTo(currentTime);
        m_renderContext->doneCurrent();
    });
}

void SkyTimelineFramebufferRenderer::destoryTimeline(SkyTimeline *timeline) {
    m_renderThread->sync([=] {
        assert(timeline != m_timeline);
        m_renderContext->makeCurrent();
        SkyTimeline::destory(timeline);
        m_renderContext->doneCurrent();
    });
}

SkyVideoFrame SkyTimelineFramebufferRenderer::doRenderLoopOffscreen() {
    m_renderContext->makeCurrent();
    glGetError();
    ON_SCOPE_EXIT {
        glFinish();
        m_renderContext->doneCurrent();
    };

    PlayLoopInfo loopInfo;
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        loopInfo = m_playLoopInfo;
    }

    if (loopInfo.playState == SkyPlayState_Pause) {
        m_lastTimerTime = getCurrentTime();
        return m_timeline->snapCurrentVideo();
    }

    SkyTimeline *timeline = m_timeline;
    double duration = timeline->getDuration();
    double endTime = std::min(loopInfo.playRange.end, duration);
    if (endTime < 0) {
        endTime = duration;
    }

    if (timeline->getCurrentTime() + 1e-6 >= endTime) {
        if (loopInfo.numberOfLoops <= 0) {
            timeline->seekTo(loopInfo.playRange.begin);
        } else if (m_loopIndex < loopInfo.numberOfLoops - 1) {
            m_loopIndex++;
            timeline->seekTo(loopInfo.playRange.begin);
        } else {
            setPlayState(SkyPlayState_Pause);
            return timeline->snapCurrentVideo();
        }
    }
    double currentTime = getCurrentTime();
    double timeStep = (currentTime - m_lastTimerTime);
    m_lastTimerTime = currentTime;
    return m_timeline->playVideoFrame(timeStep).video;
}

QOpenGLFramebufferObject *SkyTimelineFramebufferRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void SkyTimelineFramebufferRenderer::synchronize(QQuickFramebufferObject *owner) {
    SkyTimelineView *view = dynamic_cast<SkyTimelineView *>(owner);
    if (view) {
        view->setRenderder(this);
        setPlayState(view->getPlayState());
    }
}

void SkyTimelineFramebufferRenderer::setPlayRange(SkyTimeRange &range, int loopNums) {
    m_playLoopInfo.playRange = range;
    m_playLoopInfo.numberOfLoops = loopNums;
    m_loopIndex = 0;
}

SkyTimeRange SkyTimelineFramebufferRenderer::getPlayRange() {
    return m_playLoopInfo.playRange;
}

void SkyTimelineFramebufferRenderer::doInitialize(ThreadWroker *renderThread, double frameRate) {
    initializeOpenGLFunctions();

#ifdef Q_OS_WIN
    QOpenGLFunctions_3_3_Core *core = new QOpenGLFunctions_3_3_Core();
    core->initializeOpenGLFunctions();
    GLEW_setupOpenGLFunctions(core);
    GLEW_Hook();

    m_vao = new QOpenGLVertexArrayObject();
    m_vao->create();
    m_vao->bind();
#endif

    QOpenGLContext *mainContext = QOpenGLContext::currentContext();
    QSurface *mainSurface = mainContext->surface();
    m_renderContext = new OffscreenGLContext(mainContext, renderThread);
    mainContext->makeCurrent(mainSurface);

    m_timeline = SEProject::current()->dom()->onTimelineFramebufferRendererInitialize();
    qInfo() << "create timeline  success!";

    QString baseDir = SEProject::current()->rootPath();
    QString jsonPath = SEProject::current()->proj()->entryFile();

    SkyTimelinListener m_skyTimelineListener;
    m_skyTimelineListener.userData = this;
    m_skyTimelineListener.onTimelineNeedsDisplay = onTimelineNeedsDisplay;
    mSkyListener = m_timeline->addListener(m_skyTimelineListener);

    SkyPlayState playeSate = m_playLoopInfo.playState;
    m_renderder = new SkyTimelineRenderer(m_timeline, frameRate, playeSate, SkyQtAudioOutput::getClass());

#ifdef Q_OS_MAC
    m_lastVideoFrame = m_timeline->snapCurrentVideo();
#endif
}

SkyTimelineView::SkyTimelineView(QQuickItem *parent) : QQuickFramebufferObject(parent) {
    setMirrorVertically(true);
    connect(
        this,
        &SkyTimelineView::imageReady,
        this,
        [this]() {
            update();
        },
        Qt::QueuedConnection);
}

SkyTimelineView::~SkyTimelineView() {
    qDebug() << "SkyTimelineView destoryed";
    stopTimer();
    m_renderer = nullptr;
}

void SkyTimelineView::onTimerFired() {
    if (SEProject::isQuit() || m_renderer == nullptr) {
        return;
    }
    if (isPlayEnd() && m_playState == SkyPlayState_Playback) {
        emit playEnd();
    }
    emit progressUpdated();
    refreshView();
}

void SkyTimelineView::refreshView() {
    this->update();
    //用于独立窗口
    if (!isVisible()) {
        m_renderer->setPlayState(getPlayState());
    }
    if (m_renderer->getRenderThread()->isEmpty() && !ExportDialogDataSource::isPacking()) {
        m_renderer->getRenderThread()->async([this] {
            if (SEProject::isQuit()) {
                return;
            }
            SkyVideoFrame frame = m_renderer->doRenderLoopOffscreen();
            if (frame.isAvailable() || m_renderer->timeline()->getDuration() == 0) {
                m_renderer->setVideoFrame(frame);
                auto skytimelineview2 = SEProject::current()->dom()->previewController()->getSkyTimelineView2();
                auto miniPlayerView = SEProject::current()->dom()->previewController()->getMiniPlayerView();
                bool showSkytimelineview2 = skytimelineview2 && skytimelineview2->isVisible();
                bool showMiniPlayerView = miniPlayerView;
                if (showSkytimelineview2 || showMiniPlayerView) {
                    m_renderer->m_renderContext->makeCurrent();
                    SkyVideoFrame rgbaFrame = m_renderer->timeline()->transToRGBA8Frame(frame);
                    m_renderer->m_renderContext->doneCurrent();
                    QImage image(
                        rgbaFrame.getRGBA8Data(), rgbaFrame.getWidth(), rgbaFrame.getHeight(), QImage::Format_RGBA8888);
                    QImage imageCopy = image.copy();
                    if (showSkytimelineview2) {
                        emit skytimelineview2->previewChanged(imageCopy);
                    }
                    if (showMiniPlayerView) {
                        emit miniPlayerView->previewChanged(imageCopy);
                    }
                }
            }
        });
    }
}

QImage SkyTimelineView::snapFrame(int type, double time) {
    return m_renderer->snapFrame(type, time);
}

void SkyTimelineView::outputVideo(int64_t videoBitRate, QString videoPath, SkyEncodingCallback callback) {
    m_renderer->outputVideo(videoBitRate, videoPath, callback);
}

void SkyTimelineView::initTimer() {
    if (m_timer == nullptr) {
        m_timer = new QTimer(nullptr);
        m_timer->setInterval(1000 / m_frameRate);
        m_timer->setTimerType(Qt::PreciseTimer);
        QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerFired()));
        m_timer->start();
    }
}
void SkyTimelineView::play() {
    initTimer();
    m_playState = SkyPlayState_Playback;
    if (m_renderer != nullptr) {
        m_renderer->setPlayState(getPlayState());
    } else {
        update();
    }
}

void SkyTimelineView::playRange(SkyTimeRange &range, int loops) {
    m_renderer->setPlayRange(range, loops);
}

bool SkyTimelineView::isPlayEnd() {
    if (m_renderer == nullptr) {
        return false;
    }
    SkyTimeline *timeline = m_renderer->timeline();
    if (timeline == nullptr) {
        return false;
    }
    if (timeline->getDuration() <= 0) {
        return false;
    }
    double duration = timeline->getDuration();
    double endTime = std::min(m_renderer->getPlayRange().end, duration);
    if (endTime < 0) {
        endTime = duration;
    }
    return timeline->getCurrentTime() + 1e-6 >= endTime;
}

void SkyTimelineView::stopTimer() {
    if (m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
}

void SkyTimelineView::destoryTimeline(SkyTimeline *timeline) {
    m_renderer->destoryTimeline(timeline);
}

void SkyTimelineView::stop() {
    //    if (m_timer != nullptr) {
    //        m_timer->stop();
    //        m_timer = nullptr;
    //    }
    m_playState = SkyPlayState_Pause;
    if (m_renderer != nullptr) {
        m_renderer->setPlayState(getPlayState());
    } else {
        update();
    }
}

void SkyTimelineView::setMute(bool b) {
    SEProject::current()->dom()->setMute(b);
}
