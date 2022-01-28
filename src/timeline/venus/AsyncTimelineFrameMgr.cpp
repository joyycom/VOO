#include "AsyncTimelineFrameMgr.h"

#include <base/utils/MemoryPool.h>
#include <base/utils/StdThreadPools.h>
#include <base/utils/threadpools.h>
#include <assert.h>
#include <timeline/models/videoclip.h>

#include <project/SEProject.h>

AsyncTimelineFrameMgr::AsyncTimelineFrameMgr() {
    m_thread = new ThreadWroker();
    m_thread->start();
    qInfo() << "AsyncTimelineFrameMgr start:" << m_thread;
}

AsyncTimelineFrameMgr::~AsyncTimelineFrameMgr() {
    qInfo() << "AsyncTimelineFrameMgr ~ begin";
    destroyTask();

    if (m_thread != nullptr) {
        delete m_thread;
    }
    m_thread = nullptr;

    qInfo() << "AsyncTimelineFrameMgr Done~";
}

void AsyncTimelineFrameMgr::initOfflineGLEnvironment() {
    qInfo() << "initOfflineGLEnvironment m_renderContext:" << m_renderContext << " Thread:" << m_thread;
    if (m_renderContext == nullptr) {
        m_thread->sync([this] {
            if (m_renderContext != nullptr) {
                return;
            }
            QSurfaceFormat format;
            m_renderContext = new OffscreenGLContext(format, m_thread);
            m_renderContext->makeCurrent();

            qDebug() << "AsyncTimelineFrameMgr async OPENGL";
            m_renderContext->doneCurrent();
        });
    }
}

void AsyncTimelineFrameMgr::destroyGLEnvironment() {
    m_thread->sync([this] {
        qDebug() << "AsyncTimelineFrameMgr::destroyGLEnvironment m_renderContext:" << m_renderContext;
        delete m_renderContext;
        m_renderContext = nullptr;
    });
}

AsyncTimelineFrameMgr *AsyncTimelineFrameMgr::getIns() {
    static AsyncTimelineFrameMgr mgr;
    return &mgr;
}

void AsyncTimelineFrameMgr::destroyTask() {
    qDebug() << "AsyncTimelineFrameMgr::destroyTask begin!" << m_processingClip;
    {
        std::unique_lock<std::mutex> lock(m_lockMutex);
        m_waitingClips.clear();
        isInterruptSnap = true;
    }
    // wait for exiting success of venus task
    destroyGLEnvironment();
}

void onDidRemoveObjects(void *userData, const SkyObject **objects, int objSize) {
    AsyncTimelineFrameMgr *mgr = (AsyncTimelineFrameMgr *)userData;
    if (mgr) {
        for (int i = 0; i < objSize; ++i) {
            if (objects[i]->getObjectType() == SkyObjectType_Clip) {
                mgr->removeClipProcessTask((SkyClip *)objects[i]);
            }
        }
    }
}

void AsyncTimelineFrameMgr::updateClipProcessTask(BaseClip *clip) {
    removeClipProcessTask(clip->skyClip());
    addClipProcessTask(clip);
}

void AsyncTimelineFrameMgr::setTimeline(SkyTimeline *timeline) {
    if (m_orignalTimeline != nullptr) {
        if (m_listenerId.isAvailable()) {
            m_orignalTimeline->removeListener(m_listenerId);
        }
        std::unique_lock<std::mutex> lock(m_lockMutex);
        isInterruptSnap = true;
        qDebug() << "setTimeline Interruption Thread";

        m_waitingClips.clear();
    }

    m_orignalTimeline = timeline;
    if (timeline) {
        SkyTimelinListener listener;
        listener.userData = this;
        listener.onDidRemoveObjects = onDidRemoveObjects;
        timeline->addListener(listener);
    }
}

void AsyncTimelineFrameMgr::addClipProcessTask(BaseClip *clip) {
    if (clip == nullptr || clip->skyClip() == nullptr ||
        (clip->clipType() != ImageClipType && clip->clipType() != VideoClipType) || !isClipNeedProcess(clip)) {
        return;
    }

    assert(clip->skyClip()->getTimeline() == m_orignalTimeline);

    clip->updateAsyncProgress(0);

    qInfo() << "addClipProcessTask: Clip: " << clip->toString() << " SkyClip:" << clip->skyClip();
    {
        std::unique_lock<std::mutex> lock(m_lockMutex);
        for (auto c : m_waitingClips) {
            if (c == clip) {
                qInfo() << "Exit Clip in Waiting List, Skip";
                return;
            }
        }
        m_waitingClips.push_back(clip);
    }

    tryProcessClip();
}

void AsyncTimelineFrameMgr::removeClipProcessTask(SkyClip *clip) {
    if (clip == nullptr) {
        return;
    }
    qInfo() << "removeClipProcessTask: SkyClip: " << clip;
    std::unique_lock<std::mutex> lock(m_lockMutex);
    for (auto c : m_waitingClips) {
        if (c->skyClip() == clip) {
            c->updateAsyncProgress(c->during());
            m_waitingClips.remove(c);
            break;
        }
    }
    if (m_processingClip != nullptr && m_processingClip->skyClip() == clip) {
        m_processingClip->updateAsyncProgress(m_processingClip->during());
        isInterruptSnap = true;
        qInfo() << "setTimeline Interruption Thread";
    }
}

void AsyncTimelineFrameMgr::tryProcessClip() {
    std::unique_lock<std::mutex> lock(m_lockMutex);
    if (m_processingClip != nullptr) {
        return;
    }

    if (m_waitingClips.empty()) {
        return;
    }

    auto clip = m_waitingClips.front();
    if (clip == nullptr) {
        return;
    }
    m_waitingClips.remove(clip);
    preloadClipFrameData(clip);
}

bool AsyncTimelineFrameMgr::isClipNeedProcess(BaseClip *clip) {
    int nums = clip->skyClip()->numberOfEffects();
    std::vector<SkyEffect *> effects(nums);
    clip->skyClip()->getEffects(effects.data(), nums);
    bool hasCallbackParams = false;
    for (int i = 0; i < nums; ++i) {
        if (!effects[i]->getUserData("FrameCallbackParams").isNull()) {
            hasCallbackParams = true;
            break;
        }
    }
    return hasCallbackParams;
}

void AsyncTimelineFrameMgr::tryClearMemoryPools() {
    ThreadPools::instance()->exec([=] {
        bool isAsyncTaskFinished = false;
        {
            std::unique_lock<std::mutex> lock(m_lockMutex);
            isAsyncTaskFinished = !m_thread->isRunning() && m_waitingClips.size() == 0 && m_processingClip == nullptr;
        }
        if (isAsyncTaskFinished) {
            MemoryPool::instance()->clear();
        }
    });
}

void AsyncTimelineFrameMgr::preloadClipFrameData(BaseClip *clip) {
    initOfflineGLEnvironment();
    m_thread->async([=] {
        {
            std::unique_lock<std::mutex> lock(m_lockMutex);
            isInterruptSnap = false;
            m_processingClip = clip;
            qInfo() << "preloadClipFrameData begin: Clip:" << clip->toString() << " SkyClip:" << clip->skyClip();
        }

        m_renderContext->makeCurrent();
        SkyClip *processClip = copyClipToNewTimeline(clip->skyClip());
        snapFrameData(processClip);

        if (!isInterruptSnap) {
            ThreadPools::instance()->mainThread(
                [=] {
                    clip->updateAsyncProgress(clip->during());
                },
                0);
        }

        SkyTimeline::destory(processClip->getTimeline());
        {
            std::unique_lock<std::mutex> lock(m_lockMutex);
            m_processingClip = nullptr;
            isInterruptSnap = true;
        }
        m_renderContext->doneCurrent();

        qInfo() << "preloadClipFrameData Done";
        tryProcessClip();
        tryClearMemoryPools();
    });
}

void AsyncTimelineFrameMgr::snapFrameData(SkyClip *clip) {
    auto timeline = clip->getTimeline();
    auto frameRate = timeline->getVideoParams().frameRate;
    auto trackRange = clip->getTrackRange();
    double curTime = trackRange.begin;
    qInfo() << "snapFrameData BeginTime:" << curTime << " EndTime:" << trackRange.end;
    while (curTime <= trackRange.end) {
        if (isInterruptSnap) {
            qInfo() << "Interrupt SnapFrameThread! CurTime:" << curTime << " EndTime:" << trackRange.end;
            break;
        }
        QTime begin = QTime::currentTime();
        timeline->snapVideoAtTime(curTime);
        if (isInterruptSnap) {
            qInfo() << "Interrupt SnapFrameThread! CurTime:" << curTime << " EndTime:" << trackRange.end;
            break;
        }
        double progress = (curTime - trackRange.begin);
        BaseClip *processClip = m_processingClip;
        ThreadPools::instance()->mainThread(
            [=] {
                if (!isInterruptSnap && processClip == m_processingClip) {
                    processClip->updateAsyncProgress(progress);
                }
            },
            0);
        qDebug() << "snapVideoAtTime:" << curTime << " Spend:" << QTime::currentTime().msecsTo(begin);
        curTime += 1 / frameRate;
    }
}

SkyClip *AsyncTimelineFrameMgr::copyClipToNewTimeline(SkyClip *clip) {
    auto orignalTimeline = clip->getTimeline();
    auto audioParams = orignalTimeline->getAudioParams();
    auto videoParams = orignalTimeline->getVideoParams();
    auto caches = orignalTimeline->getCaches();
    auto timeline = SkyTimeline::create(caches, audioParams, videoParams);
    timeline->setExtentPointer("AsyncVNTimeline", timeline, nullptr);

    auto videoTrack = timeline->appendVideoTrack();
    auto resource = clip->getResource();

    SkyClip *processClip = videoTrack->appendClip(resource, clip->getTrimRange());
    auto config = clip->saveConfig();
    processClip->loadConfig(config, true);
    qInfo() << "copyClipToNewTimeline  Timeline:" << timeline << " Clip:" << clip;
    return processClip;
}
