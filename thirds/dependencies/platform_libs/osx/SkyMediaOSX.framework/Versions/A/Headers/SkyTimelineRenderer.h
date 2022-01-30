#pragma once

#include "SkyDefs.h"
#include "SkyTimeline.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyPlayState_Pause = 0,
    SkyPlayState_Playback = 1,
} SkyPlayState;

struct SKYMEDIA_API SkyAudioOutputClass final {
    void *(*create)() = nullptr;
    void (*destory)(void *raw) = nullptr;

    void (*open)(void *raw, const SkyAudioParams &audioParams) = nullptr;
    void (*cleanup)(void *raw) = nullptr;
    void (*writeAudioData)(void *raw, const SkyAudioFrame &audioFrame) = nullptr;
    void (*setState)(void *raw, SkyPlayState state) = nullptr;
};

typedef enum {
    SkySamplerType_Linear,       // 默认算法
    SkySamplerType_MipmapLinear, // 打开 Mipmap
    SkySamplerType_Area,         // 打开 Area 重采样
    SkySamplerType_Bicubic,
} SkySamplerType;

struct SkyRenderParams final {
    int surfaceWidth = 0;
    int surfaceHeight = 0;
    SkyScaleMode scaleMode = SkyScaleMode_AspectFit;
    SkyColor clearColor = {0, 0, 0, 1};
    bool verticalFlip = false;
    SkySamplerType minSamplerType = SkySamplerType_Linear; // viewport 比纹理小的插值方式
    SkySamplerType magSamplerType = SkySamplerType_Linear; // viewport 比纹理大的插值方式
};

class SKYMEDIA_API SkyTimelineRenderer final {
public:
    SkyTimelineRenderer(const SkyTimelineRenderer &) = delete;
    SkyTimelineRenderer &operator=(const SkyTimelineRenderer &) = delete;

    SkyTimelineRenderer(SkyTimeline *timeline,
                        double frameRate,
                        SkyPlayState state,
                        SkyAudioOutputClass audioOutputClass);
    ~SkyTimelineRenderer();

    void renderFrame(SkyRenderParams params, double deltaTime);
    void renderFrame(SkyRenderParams params, SkyVideoFrame frame);

    void setState(SkyPlayState state);
    void setMute(bool isMute);
    bool isMute();
    void *getAudioOutput() const;

private:
    void *m_impl;
};

/////////////////////////////////////////////////////
template <typename T>
struct SkyDefaultAudioOutputClass final {
    static void *create() {
        return new T();
    }

    static void destory(void *raw) {
        delete static_cast<T *>(raw);
    }

    static void open(void *raw, const SkyAudioParams &audioParams) {
        static_cast<T *>(raw)->open(audioParams);
    }

    static void cleanup(void *raw) {
        return static_cast<T *>(raw)->cleanup();
    }

    static void writeAudioData(void *raw, const SkyAudioFrame &audioFrame) {
        return static_cast<T *>(raw)->writeAudioData(audioFrame);
    }

    static SkyAudioOutputClass getClass() {
        SkyAudioOutputClass result;
        result.create = create;
        result.destory = destory;
        result.open = open;
        result.cleanup = cleanup;
        result.writeAudioData = writeAudioData;
        return result;
    }
};

SKYMEDIA_NAMESPACE_END
