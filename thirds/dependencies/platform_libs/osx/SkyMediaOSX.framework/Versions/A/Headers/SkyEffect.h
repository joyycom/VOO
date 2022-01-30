#pragma once

#include "SkyCaches.h"
#include "SkyCurve.h"
#include "SkyFrame.h"
#include "SkyInputList.h"
#include "SkyOFWrapper.h"
#include "SkyObject.h"
#include "SkyResource.h"
#include "SkyVariant.h"
#include "SkyVideoDecoder.h"

SKYMEDIA_NAMESPACE_BEGIN

extern SKYMEDIA_API const char *SkyInputKey_OriginVideoFrame; // 从解码器解码出来的，最原始的帧
extern SKYMEDIA_API const char *SkyInputKey_VideoFrame;       // 经过特效的那一帧，

// 三种可能的触发 Callback 的方式
typedef enum {
    SkyTriggerMode_Snap = 0,   // 调用 Timeline 的 snapCurrentVideo、snapVideoAtTime 时触发。
    SkyTriggerMode_Play = 1,   // Timeline 在播放过程中触发。
    SkyTriggerMode_Export = 2, // Timeline 在导出视频中触发。
} SkyTriggerMode;

struct SKYMEDIA_API SkyFrameDataInputContext final {
    SkyFrameDataInputContext *operator=(const SkyFrameDataInputContext &rhs) = delete;
    SkyFrameDataInputContext(const SkyFrameDataInputContext &rhs) = delete;

    void *getExtentPointer(const char *key) const;
    SkyTriggerMode getTriggerMode() const;
    SkyCaches getCaches() const;
    SkyHash getInputHash() const;
    SkyVariant getInputByKey(const char *inputKey) const;
};

struct SKYMEDIA_API SkyFrameDataClass final {
    void *classData = nullptr;
    void *(*createInstance)(void *classData, SkyVariant params) = nullptr;
    void (*destoryInstance)(void *instance) = nullptr;
    SkyVariant (*onFrameData)(void *instance, const SkyFrameDataInputContext &inputCtx) = nullptr;
};

struct SKYMEDIA_API SkyFrameDataDescriptor final {
    SkyString className;
    SkyVariant params;
    SkyString name;
    SkyStringList inputKeys;
    SkyCachesStrategy cachesStrategy = SkyCachesStrategy_None;
};

class SkyEffect;
class SKYMEDIA_API SkyFrameDataCallback final {
public:
    SkyFrameDataCallback *operator=(const SkyFrameDataCallback &rhs) = delete;
    SkyFrameDataCallback(const SkyFrameDataCallback &rhs) = delete;

    SkyFrameDataDescriptor getDescriptor() const;
    SkyEffect *getOwner() const;
    void removeFromOwner();

private:
    SkyFrameDataCallback();
    ~SkyFrameDataCallback();
};

typedef enum {
    SkyEffectFlags_Render = 0x001,   // 是否应用特效的渲染
    SkyEffectFlags_Callback = 0x002, // 是否触发 Callback
    SkyEffectFlags_All = SkyEffectFlags_Render | SkyEffectFlags_Callback,
} SkyEffectFlags;

class SkyTimeline;
class SKYMEDIA_API SkyEffect : public SkyObject {
public:
    SkyTimeline *getTimeline() const;

    bool isAudioEffect() const;
    bool isVideoEffect() const;

    int getIndex() const;
    bool moveToIndex(int destIndex);

    // 假如在 Clip(或Track) 上，添加多个特效，会按 index 顺序依次执行特效。
    // 有时我们想强制某个特效无论 index 顺序如何，都能在最前（或最后）执行，这时可以修改特效执行的优先级。
    // 优先级默认为 0，数值越大越先执行，数值越小越后执行。假如我们想让特效 A 在最后执行，可以将优先级设置为一个负数。
    // B(priority:0) -> C(priority:0) -> A(priority:-100)
    void setPriority(int priority);
    int getPriority() const;

    void reload();
    void removeFromOwner();
    SkyObject *getOwner() const;
    void updateParams(const SkyVariant &params);

    void setParamCurve(const char *keyPath, const SkyCurve &curve);
    SkyCurve getParamCurve(const char *keyPath) const;
    SkyStringList getParamCurveKeys() const;

    bool isEnabled(SkyEffectFlags flags) const;
    void disable(SkyEffectFlags flags);
    void enable(SkyEffectFlags flags);

    SkyVariant getParams() const;
    SkyVariant getParam(const char *keyPath) const;

    SkyTimeRange getTimeRange() const;
    void setTimeRange(const SkyTimeRange &range);

    SkyInputList *getInputList() const;
    SkyOFWrapper *getOFWrapper() const;

    SkyVariant saveConfig() const;

    SkyFrameDataCallback *addFrameDataCallback(const SkyFrameDataDescriptor &descriptor);
    SkyFrameDataCallback *findCallbackByName(const char *name) const;
    SkyFrameDataCallback *findCallbackByClassName(const char *name) const;

    void *getExtWrapper(const char *wrapperKey) const;
    void callAsync(const char *fnName, SkyVariant args);
    void timedCall(double t, const char *fnName, SkyVariant args);
    void removeAllCalls();

private:
    SkyEffect();
    ~SkyEffect();
};

class SKYMEDIA_API SkyTransition final : public SkyEffect {
public:
    double getDuration() const;
    void setDuration(double duration) const;

private:
    SkyTransition();
    ~SkyTransition();
};

SKYMEDIA_NAMESPACE_END
