#pragma once

#include "SkyEffect.h"
#include "SkyVariant.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SKY_COLOR_ATTACHMENT0 = 0x8CE0,
    SKY_DEPTH_ATTACHMENT = 0x8D00,
    SKY_STENCIL_ATTACHMENT = 0x8D20,
} SkyAttachment;

struct SKYMEDIA_API SkyEffectInputContext final {
    SkyEffectInputContext *operator=(const SkyEffectInputContext &rhs) = delete;
    SkyEffectInputContext(const SkyEffectInputContext &rhs) = delete;

    int getVideoCount() const;
    SkyVideoFrame getVideoAt(int idx) const;

    int getAudioCount() const;
    SkyAudioFrame getAudioAt(int idx) const;

    double getTimestamp() const;
    SkyTimeRange getTimeRange() const;

    bool useBackgroundAsInput() const;
    SkyVideoFrame getBackground() const;

    SkyVideoParams getVideoParams() const;

    void *getExtentPointer(const char *key) const;

    SkyVariant evaluateParamAtTime(const char *key, double time) const;

    SkyVariant getExtFrameData() const;

    SkyVideoDecoder openVideoDecoder(const SkyResource &res) const;

    // render video 相关的一些辅助函数
    void bindSharedFramebuffer(const SkyVideoFrame &gpuSample, SkyAttachment attachment = SKY_COLOR_ATTACHMENT0) const;
    void bindSharedFramebuffer(int textID, SkyAttachment attachment = SKY_COLOR_ATTACHMENT0) const;

    SkyVideoFrame resizeFrame(const SkyVideoFrame &frame, int width, int height, SkyScaleMode scaleMode) const;
    SkyVideoFrame cropFrame(const SkyVideoFrame &frame, const SkyRect &cropRect) const;

    SkyVideoFrame allocGPUFrame(int width, int height, void *data = nullptr) const;
    SkyVideoFrame allocGPUFrameRef(int width, int height, int textID) const;

    SkyVideoFrame transToGPUFrame(const SkyVideoFrame &frame) const;
    SkyVideoFrame transToRGBA8Frame(const SkyVideoFrame &frame) const;

    // render audio 相关辅助函数
    void scaleVolume(SkyAudioFrame *frame, double volume) const;
};

class SkyExtWrapperClass;
struct SKYMEDIA_API SkyEffectClass final {
    // 路径转换函数
    SkyVariant (*pathConversion)(const SkyVariant &params) = nullptr;

    // 创建销毁
    void *(*createEffect)() = nullptr;
    void (*destoryEffect)(void *effect) = nullptr;

    // 特效参数
    bool (*initWithParams)(void *effect, const SkyEffectInputContext &inputCtx, const SkyVariant &params) = nullptr;
    bool (*reload)(void *effect, const SkyEffectInputContext &inputCtx, const SkyVariant &params) = nullptr;

    void (*updateParams)(void *effect, const SkyVariant &params) = nullptr;
    SkyVariant (*getParam)(void *effect, const char *keyPath) = nullptr;

    // 应用特效
    SkyVideoFrame (*renderVideo)(void *effect, const SkyEffectInputContext &inputCtx) = nullptr;
    SkyAudioFrame (*renderAudio)(void *effect, const SkyEffectInputContext &inputCtx) = nullptr;

    // 进一步扩展特效的功能，调用特效的扩展函数
    SkyExtWrapperClass (*getExtWrapperClass)(const char *wrapperKey) = nullptr;
    SkyVariant (*extCall)(void *effect, const char *key, const SkyVariant &args) = nullptr;
};

struct SkyExtWrapperClass final {
    void *(*createWrapper)() = nullptr;
    void (*destoryWrapper)(void *wrapper) = nullptr;

    void (*initWithParams)(void *wrapper, SkyEffect *effect, const SkyVariant &params) = nullptr;
    SkyVariant (*saveParams)(void *wrapper) = nullptr;
    void (*reload)(void *wrapper) = nullptr;
};

template <typename EffectT>
struct SkyDefaultEffectClass final {
    static void *createEffect() {
        return new EffectT();
    }

    static void destoryEffect(void *effect) {
        delete static_cast<EffectT *>(effect);
    }

    static bool initWithParams(void *effect, const SkyEffectInputContext &inputCtx, const SkyVariant &params) {
        return static_cast<EffectT *>(effect)->initWithParams(inputCtx, params);
    }

    static bool reload(void *effect, const SkyEffectInputContext &inputCtx, const SkyVariant &params) {
        return static_cast<EffectT *>(effect)->reload(inputCtx, params);
    }

    static void updateParams(void *effect, const SkyVariant &params) {
        static_cast<EffectT *>(effect)->updateParams(params);
    }

    static SkyVideoFrame renderVideo(void *effect, const SkyEffectInputContext &inputCtx) {
        return static_cast<EffectT *>(effect)->renderVideo(inputCtx);
    }

    static SkyAudioFrame renderAudio(void *effect, const SkyEffectInputContext &inputCtx) {
        return static_cast<EffectT *>(effect)->renderAudio(inputCtx);
    }

    static SkyVariant getParam(void *effect, const char *keyPath) {
        return static_cast<EffectT *>(effect)->getParam(keyPath);
    }

    static SkyEffectClass getVideoClass() {
        SkyEffectClass result;
        result.createEffect = createEffect;
        result.destoryEffect = destoryEffect;
        result.initWithParams = initWithParams;
        result.updateParams = updateParams;
        result.renderVideo = renderVideo;
        result.reload = reload;
        result.getParam = getParam;
        return result;
    }

    static SkyEffectClass getAudioClass() {
        SkyEffectClass result;
        result.createEffect = createEffect;
        result.destoryEffect = destoryEffect;
        result.initWithParams = initWithParams;
        result.updateParams = updateParams;
        result.renderAudio = renderAudio;
        result.reload = reload;
        result.getParam = getParam;
        return result;
    }
};

SKYMEDIA_NAMESPACE_END
