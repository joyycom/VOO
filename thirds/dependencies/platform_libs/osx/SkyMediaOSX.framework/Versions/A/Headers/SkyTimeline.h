#pragma once

#include "SkyAsyncTask.h"
#include "SkyAudioDecoder.h"
#include "SkyCaches.h"
#include "SkyDefs.h"
#include "SkyEffect.h"
#include "SkyError.h"
#include "SkyObject.h"
#include "SkyResource.h"
#include "SkyVideoDecoder.h"
#include "SkyWav.h"
#include <cstring>

SKYMEDIA_NAMESPACE_BEGIN

extern SKYMEDIA_API const char *SkyOrangeEffectClassName;
extern SKYMEDIA_API const char *SkyOrangeTransitionClassName;
extern SKYMEDIA_API const char *SkyVolumeEffectClassName;

typedef enum {
    SkyEffectCategory_Video = 0,
    SkyEffectCategory_Audio = 1,
} SkyEffectCategory;

struct SKYMEDIA_API SkyTimingFunction final {
    SkyTimingFunction(const char *str = "linear") {
        setTweenName(str);
    }
    void setTweenName(const char *str) {
        strncpy(tweenName, str, sizeof(tweenName) - 1);
    }
    char tweenName[32];
};

struct SKYMEDIA_API SkyEffectDescriptor final {
    SkyEffectDescriptor(const char *name = "") : className(name), timeRange(0, -1) {
    }
    SkyString className;
    SkyVariant params;
    SkyTimeRange timeRange;
};

struct SKYMEDIA_API SkyTransitionDescriptor final {
    SkyTransitionDescriptor(const char *name = "") : className(name) {
    }
    SkyString className;
    SkyVariant params;
    double duration = 1;
    double beforeDurationScale = 1;
    double afterDurationScale = 1;
    SkyTimingFunction beforeTimingFunction;
    SkyTimingFunction afterTimingFunction;
};

typedef enum {
    SkyDurationMode_Max = 0,   // 取最长的时长作为总时长
    SkyDurationMode_Video = 1, // 以视频时长作为总时长
} SkyDurationMode;

struct SKYMEDIA_API SkyEncodingParams final {
    // 视频参数
    bool isVideoEnabled = true;
    bool isVideoHardencode = false;
    int64_t videoBitRate = 1650000;
    int crf = -1;
    SkyVideoParams videoParams;

    // 音频参数
    bool isAudioEnabled = true;
    int64_t audioBitRate = 64000;
    SkyAudioParams audioParams;

    SkyDurationMode durationMode = SkyDurationMode_Video;

    // 编码时候隔多少帧调用一次进度回调
    int notifyProgressInterval = 30;
};

struct SKYMEDIA_API SkyEncodingCallback final {
    void *userData = nullptr;
    bool (*shouldBeCancelled)(void *userData) = nullptr;
    void (*onProgress)(void *userData, double currentTime, double totalTime) = nullptr;
    void (*onFinish)(void *userData) = nullptr;
    void (*onError)(void *userData, SkyError error) = nullptr;
};

typedef enum {
    SkyFrameOrigin_FromCache = 0,
    SkyFrameOrigin_FromDecoder = 1,
} SkyFrameOrigin;

struct SKYMEDIA_API SkyDecodeCallback final {
    void *userData = nullptr;
    SkyVideoFrame (*onFetchFromCaches)(void *userData) = nullptr;
    void (*onFinish)(void *userData, SkyVideoFrame videoFrame, SkyFrameOrigin frameOrigin) = nullptr;
    void (*onError)(void *userData, SkyError error) = nullptr;
};

// 状态通知
struct SkyTimelinListener final {
    void *userData = nullptr;

    // 时间线内部状态被改变，需要刷新界面
    void (*onTimelineNeedsDisplay)(void *userData) = nullptr;

    // 时间线上的对象被删除。
    // 比如某个 trackA 上有 clipA、clipB，clipA 上有 effectA、effectB。
    // 这样删除 trackA 之后，也会删除一系列的对象。effectA、effectB、clipA、clipB、trackA 都被删除了。
    void (*onDidRemoveObjects)(void *userData, const SkyObject **objects, int objSize) = nullptr;
};

struct SKYMEDIA_API SkyListenerID {
    bool isAvailable() const {
        return value != 0;
    }
    int64_t value = 0;
};

typedef enum {
    SkyMipmapFlags_StaticImage = 0x001, // 对静态图片（非视频）打开 mipmap
} SkyMipmapFlags;

class SkyAudioTrack;
class SkyVideoTrack;
class SkyTrack;
class SkyClip;
class SKYMEDIA_API SkyTimeline final : public SkyObject {
public:
    static SkyTimeline *create(const SkyCaches &caches,
                               const SkyAudioParams &audioParams,
                               const SkyVideoParams &videoParams);
    static SkyTimeline *create(const SkyCaches &caches) {
        return SkyTimeline::create(caches, SkyAudioParams(), SkyVideoParams());
    }
    static void destory(SkyTimeline *timeline);

    SkyTimeline &operator=(const SkyTimeline &rhs) = delete;
    SkyTimeline(const SkyTimeline &rhs) = delete;

    void setMipmapFlags(SkyMipmapFlags flags);

    SkyListenerID addListener(SkyTimelinListener listener);
    bool removeListener(SkyListenerID ID);
    void removeAllListeners();

    SkyAudioTrack *appendAudioTrack() const;
    SkyVideoTrack *appendVideoTrack() const;

    bool removeTrack(SkyTrack *track);
    void removeAllTracks();

    int numberOfTracks() const;
    SkyTrack *getTrackAt(int index) const;
    void getTracks(SkyTrack **tracks, int size) const;

    // 调整 track 的顺序，移动到索引为 destIndex 的位置，索引为 0 表示在最前面。
    // 其中特殊的值，索引为 -1 表示在最后面。
    bool moveTrack(SkyTrack *track, int destIndex);

    SkyTrack *findTrackByName(const char *name) const;
    SkyClip *findClipByName(const char *name) const;
    SkyEffect *findEffectByName(const char *name) const;
    SkyTransition *findTransitionByName(const char *name) const;
    SkyObject *findObjectByName(const char *name) const;
    SkyObjectList getAllSubObjects(bool includeSelf = false) const;

    double getCurrentTime() const;
    double getDuration() const;
    double getVideoDuration() const;
    double getAudioDuration() const;

    SkyAudioParams getAudioParams() const;

    void setVideoParams(const SkyVideoParams &videoParams);
    SkyVideoParams getVideoParams() const;

    void setCaches(const SkyCaches &caches);
    SkyCaches getCaches() const;

    void seekTo(double timestamp);

    int numberOfEffects(SkyEffectCategory category = SkyEffectCategory_Video) const;
    void getEffects(SkyEffect **effects, int size, SkyEffectCategory category = SkyEffectCategory_Video) const;
    SkyEffect *addEffect(const SkyEffectDescriptor &desc, SkyEffectCategory category = SkyEffectCategory_Video);
    SkyEffect *addEffectFromConfig(const SkyVariant &config, SkyEffectCategory category = SkyEffectCategory_Video);

    // 将指针跟时间线的绑定起来，可扩展 Timeline 的功能, 比如接入 OF 等。Timeline 保存的时候，不会保存这个指针。
    void setExtentPointer(const char *key, void *ptr, void (*onExtentPtrDestory)(void *));
    void *getExtentPointer(const char *key) const;

    SkyVariant saveConfig() const;
    SkyVariant saveConfig(const char *baseDirectory) const;
    SkyVariant saveConfig(const char **baseDirectories, int size) const;

    bool loadConfig(const SkyVariant &v);
    bool loadConfig(const SkyVariant &v, const char *baseDirectory);
    bool loadConfig(const SkyVariant &v, const char **baseDirectories, int size);

    void reloadFilesNotFound();
    void reloadFilesAtTime(double timestamp);

    SkyVideoDecoder openVideoDecoder(const SkyResource &res) const;
    SkyAudioDecoder openAudioDecoder(const SkyResource &res) const;

    SkyAsyncTask
    retrieveVideoAsync(const SkyResource &res, double time, SkyRetrieveMode retrieveMode, SkyDecodeCallback callback);

    int getMaxGPUTextureSize() const;

    // 下面几个函数，用到 OpenGL 的函数，需要在 OpenGL 的环境中执行
    bool exportVideo(const char *filePath, const SkyEncodingParams &params);
    bool exportVideo(const char *filePath, const SkyEncodingParams &params, const SkyEncodingCallback &callback);

    SkyVideoFrame snapCurrentVideo() const;
    SkyVideoFrame snapVideoAtTime(double timestamp) const;

    struct PlayResult {
        SkyVideoFrame video;
        SkyAudioFrame audio;
        double videoTimestamp = 0;
        double audioTimestamp = 0;
        bool isFinish = true;
        bool isVideoFinish = true;
        bool isAudioFinish = true;
    };
    PlayResult playFrames(double deltaTime);
    PlayResult playVideoFrame(double deltaTime);
    PlayResult playAudioFrame(double deltaTime);

    SkyVideoFrame transToGPUFrame(const SkyVideoFrame &frame) const;
    SkyVideoFrame transToRGBA8Frame(const SkyVideoFrame &frame) const;

private:
    SkyTimeline();
    ~SkyTimeline();
};

typedef enum {
    SkyPlaceClipStrategy_Strict = 0,      // 出现重叠时，会失败。
    SkyPlaceClipStrategy_AdjustAfter = 1, // 出现重叠时，自动调整后面 clip 的位置。
} SkyPlaceClipStrategy;

struct SKYMEDIA_API SkyTrackLayout final {
    SkyVec2D size = {1.0, 1.0};
    SkyVec2D position = {0.5, 0.5};
    SkyScaleMode contentMode = SkyScaleMode_Parent;
    bool clipsToBounds = false;
};

/*
    0 +----------+ 3
      |          |
      |          |
      |          |
    1 +----------+ 2
*/
struct SKYMEDIA_API SkyTrackBox final {
    SkyRect rect = {0, 0, 1, 1}; // 没有旋转之前的矩形框, 0 - 1 之间的相对坐标系
    double rotation = 0.0;       // 旋转角度，弧度制

    // 使用 videoSize 为参考，获取旋转后矩形的四个顶点，顺序见注释
    void getPoints(SkyVec2D points[4], int videoWidth, int videoHeight) const;
};

class SKYMEDIA_API SkyClip : public SkyObject {
public:
    SkyClip &operator=(const SkyClip &rhs) = delete;
    SkyClip(const SkyClip &rhs) = delete;

    SkyTimeline *getTimeline() const;
    SkyTrack *getTrack() const;

    bool isAudioClip() const;
    bool isVideoClip() const;

    bool isFileNotFound() const;

    int getIndex() const;
    SkyClip *getNext() const;
    SkyClip *getPrev() const;

    bool updateResource(const SkyResource &res);
    bool updateResource(const SkyResource &res, const SkyTimeRange &trimRange);

    SkyResource getResource() const;

    // 当设置了速度曲线时，getSpeed() 返回是平均速度。
    double getSpeed() const;

    // 设置匀速的速度（假如已有速度曲线，曲线会被清空掉）
    void setSpeed(double speed);

    void setSpeedCurve(const SkyCurve &curve);
    SkyCurve getSpeedCurve() const;

    void setVolume(double volume);
    double getVolume() const;

    SkyTimeRange getTrackRange() const;
    SkyTimeRange getTrimRange() const;

    double trackTSFromTrimTS(double trimTS) const;
    double trimTSFromTrackTS(double trackTS) const;

    SkyEffect *addEffect(const SkyEffectDescriptor &desc, SkyEffectCategory category = SkyEffectCategory_Video);
    SkyEffect *addEffectFromConfig(const SkyVariant &config, SkyEffectCategory category = SkyEffectCategory_Video);
    int numberOfEffects(SkyEffectCategory category = SkyEffectCategory_Video) const;
    void getEffects(SkyEffect **effects, int size, SkyEffectCategory category = SkyEffectCategory_Video) const;

    SkyTransition *setTransitionAfter(const SkyTransitionDescriptor &desc);
    SkyTransition *getTransitionAfter() const;

    SkyTransition *setTransitionBefore(const SkyTransitionDescriptor &desc);
    SkyTransition *getTransitionBefore() const;

    bool setGapBefore(double gap);
    bool setGapAfter(double gap);

    double getGapBefore() const;
    double getGapAfter() const;

    SkyVariant saveConfig() const;
    bool loadConfig(const SkyVariant &v, bool updateResource = true);

protected:
    SkyClip() = default;
    ~SkyClip() = default;
};

class SKYMEDIA_API SkyTrack : public SkyObject {
public:
    SkyTrack &operator=(const SkyTrack &rhs) = delete;
    SkyTrack(const SkyTrack &rhs) = delete;

    SkyTimeline *getTimeline() const;

    double getDuration() const;

    bool isAudioTrack() const;
    bool isVideoTrack() const;

    int getIndex() const;

    bool isLocked() const;
    void setLocked(bool flag);

    bool isHidden() const;
    void setHidden(bool flag);

    bool isMute() const;
    void setMute(bool flag);

    void setVolume(double volume);
    double getVolume() const;

    // 删除片段。keepGap 用于指定片段移除后，是否保留该片段在轨道上的空间。
    bool removeClip(SkyClip *clip, bool keepGap = false);

    SkyClip *appendClip(const SkyResource &res);
    SkyClip *appendClip(const SkyResource &res, const SkyTimeRange &trimRange);

    // 分割片段。splitPoint 为分割点，是 Track 上的时间。假如分割成功，返回分割后的片段。
    // 比如 Track 上有片段 A B C。将 B 分割，假如成功，就会变成 B1 B2, 这时会返回 B1，原来的 B 数据会被原地修改，变成
    // B2。 于是分割之后，Track 上的片段为 A B1 B2 C。
    // 假如不成功，返回空指针。
    SkyClip *splitClip(SkyClip *clip, double splitPoint);

    // 将两个片段合并成一个片段，是 splitClip 的逆操作。需要满足一些条件
    // 1. clipA 在 clipB 的前面。
    // 2. 片段之间没有任何空隙。
    // 2. 片段是打开相同的资源，并且资源之间也是连续的。
    // 成功合并, 返回 true，clipB 的数据会被更新，而 clipA 会被销毁。合并失败，会返回 false
    bool mergeClips(SkyClip *clipA, SkyClip *clipB);

    // 调整 clip 的顺序，移动到索引为 destIndex 的位置，索引为 0 表示在最前面。
    // 其中特殊的值，索引为 -1 表示在最后面。
    bool moveClip(SkyClip *clip, int destIndex);

    bool reorderClips(const SkyClip **clips, const int *indics, int size);
    bool reorderClips(const SkyClip **clips, int size);

    // 自由摆放 clip, 其他 clip 尽可能让其不动。strategy 是摆放时，出现重叠的处理行为, 具体见注释。
    bool placeClip(SkyClip *clip, double trackBeginTime, SkyPlaceClipStrategy strategy);
    bool placeClip(SkyClip *clip, const SkyTimeRange &trimRange, double trackBeginTime, SkyPlaceClipStrategy strategy);

    int numberOfClips() const;
    SkyClip *getClipAt(int index) const;
    void getClips(SkyClip **clips, int size) const;

    SkyClip *hitTest(double timestamp) const;
    SkyClip *findClipByName(const char *name) const;

    void removeAllGaps();

    int numberOfEffects(SkyEffectCategory category = SkyEffectCategory_Video) const;
    void getEffects(SkyEffect **effects, int size, SkyEffectCategory category = SkyEffectCategory_Video) const;
    SkyEffect *addEffect(const SkyEffectDescriptor &desc, SkyEffectCategory category = SkyEffectCategory_Video);
    SkyEffect *addEffectFromConfig(const SkyVariant &config, SkyEffectCategory category = SkyEffectCategory_Video);

    // 下面接口用于修改 Track 自身的位置, 和内容区域的位置
    void setLayout(const SkyTrackLayout &layout);
    SkyTrackLayout getLayout() const;

    void setTransform(const SkyTransform2D &transform);
    SkyTransform2D getTransform() const;

    // 获取 track 的包围框，同时结合了 layout 和 transform 数据
    SkyTrackBox getFinalBox() const;

    void setContentTransform(const SkyTransform2D &transform);
    SkyTransform2D getContentTransform() const;

protected:
    SkyTrack() = default;
    ~SkyTrack() = default;
};

#define SKY_CAST_CLIP_TYPE(ClipType)                                              \
    ClipType *appendClip(const SkyResource &res) {                                \
        return (ClipType *)SkyTrack::appendClip(res);                             \
    }                                                                             \
                                                                                  \
    ClipType *appendClip(const SkyResource &res, const SkyTimeRange &trimRange) { \
        return (ClipType *)SkyTrack::appendClip(res, trimRange);                  \
    }

class SkyAudioClip;
class SKYMEDIA_API SkyAudioTrack final : public SkyTrack {
public:
    SkyAudioTrack *operator=(const SkyAudioTrack &rhs) = delete;
    SkyAudioTrack(const SkyAudioTrack &rhs) = delete;

    SKY_CAST_CLIP_TYPE(SkyAudioClip)

private:
    SkyAudioTrack();
    ~SkyAudioTrack();
};

class SkyVideoClip;
class SKYMEDIA_API SkyVideoTrack final : public SkyTrack {
public:
    SkyVideoTrack *operator=(const SkyVideoTrack &rhs) = delete;
    SkyVideoTrack(const SkyVideoTrack &rhs) = delete;

    SKY_CAST_CLIP_TYPE(SkyVideoClip)

private:
    SkyVideoTrack();
    ~SkyVideoTrack();
};

class SKYMEDIA_API SkyAudioClip final : public SkyClip {
public:
    SkyAudioClip &operator=(const SkyAudioClip &rhs) = delete;
    SkyAudioClip(const SkyAudioClip &rhs) = delete;
};

class SKYMEDIA_API SkyVideoClip final : public SkyClip {
public:
    SkyVideoClip &operator=(const SkyVideoClip &rhs) = delete;
    SkyVideoClip(const SkyVideoClip &rhs) = delete;

    void setCropRect(const SkyRect &region);
    SkyRect getCropRect() const;

    void setCropTransform(const SkyTransform2D &transform);
    SkyTransform2D getCropTransform() const;

    // 缩放视频尺寸。cropRect 和 resize 都被设置时，会先裁剪再拉伸。
    void resize(int width, int height, SkyScaleMode scaleMode);
    SkyVideoParams getResizeParams() const;
};

SKYMEDIA_NAMESPACE_END
