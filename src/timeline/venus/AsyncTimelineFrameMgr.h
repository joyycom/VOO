#pragma once

#include <base/utils/Channel.h>
#include <base/utils/StdThreadPools.h>
#include <base/utils/ThreadWroker.h>

#include <skymedia/SkyTimeline.h>

#include <list>

#include <ui/preview/SkyTimelineView.h>

#include <timeline/models/baseclip.h>

SKYMEDIA_USING_NAMESPACE

class AsyncTimelineFrameMgr {
private:
    AsyncTimelineFrameMgr();
    ~AsyncTimelineFrameMgr();

public:
    static AsyncTimelineFrameMgr *getIns();

    void setTimeline(SkyTimeline *timline);

    void addClipProcessTask(BaseClip *clip);

    void removeClipProcessTask(SkyClip *clip);

    void updateClipProcessTask(BaseClip *clip);

    ThreadWroker *workThread() {
        return m_thread;
    }

    void destroyTask();

    void initOfflineGLEnvironment();

    void destroyGLEnvironment();

private:
    bool isClipNeedProcess(BaseClip *clip);

    void tryProcessClip();

    void preloadClipFrameData(BaseClip *clip);

    void snapFrameData(SkyClip *clip);

    SkyClip *copyClipToNewTimeline(SkyClip *clip);

    void tryClearMemoryPools();

    SkyTimeline *m_orignalTimeline = nullptr;
    SkyListenerID m_listenerId;

    std::list<BaseClip *> m_waitingClips;
    BaseClip *m_processingClip = nullptr;
    mutable std::mutex m_lockMutex;
    OffscreenGLContext *m_renderContext = nullptr;

    ThreadWroker *m_thread = nullptr;
    bool isInterruptSnap = false;
};
