#ifndef __SKYAPI_C99_H__
#define __SKYAPI_C99_H__

#include <stdint.h>

#ifdef __wasm__
#include <emscripten/em_macros.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#ifdef __cplusplus
#define SKYMEDIA_C_EXTEN extern "C"
static_assert(sizeof(bool) == sizeof(int8_t), "");
#else
#define SKYMEDIA_C_EXTEN
#endif

typedef int8_t SkyBool;
#define SkyTrue 1
#define SkyFalse 0

#if defined(_MSC_VER)
#define SKY_C_DEPRECATED __declspec(deprecated)
#define SKY_C_FORMAT_PRINTF(a, b)
#ifdef SKYMEDIA_EXPORTS
#define SKYMEDIA_C_API SKYMEDIA_C_EXTEN __declspec(dllexport)
#else
#define SKYMEDIA_C_API SKYMEDIA_C_EXTEN __declspec(dllimport)
#endif
#else
#define SKY_C_DEPRECATED __attribute__((deprecated))
#define SKY_C_FORMAT_PRINTF(a, b) __attribute__((format(printf, a, b)));
#define SKYMEDIA_C_API SKYMEDIA_C_EXTEN __attribute__((visibility("default"))) EMSCRIPTEN_KEEPALIVE
#endif

////////////      SkyDefs.h      //////////////////////
typedef struct SkyString SkyString;
SKYMEDIA_C_API SkyString *SkyString_create(const char *str);
SKYMEDIA_C_API SkyString *SkyString_retain(SkyString *ref);
SKYMEDIA_C_API void SkyString_release(SkyString *ref);
SKYMEDIA_C_API const char *SkyString_c_str(SkyString *ref);
SKYMEDIA_C_API int SkyString_size(SkyString *ref);

typedef struct SkyRect {
    double x;
    double y;
    double width;
    double height;
} SkyRect;

SKYMEDIA_C_API SkyRect SkyRect_make(double x, double y, double width, double height);

typedef struct SkyVec2D {
    double x;
    double y;
} SkyVec2D;

SKYMEDIA_C_API SkyVec2D SkyVec2D_make(double x, double y);

typedef struct SkyColor {
    double r;
    double g;
    double b;
    double a;
} SkyColor;

SKYMEDIA_C_API SkyColor SkyColor_make(double r, double g, double b, double a);

typedef struct SkyTransform2D {
    SkyVec2D scale;
    double rotation; // 旋转角度，弧度制
    SkyVec2D translation;
} SkyTransform2D;

SKYMEDIA_C_API SkyTransform2D SkyTransform2D_makeDefault(void);

typedef struct SkyTrackBox {
    SkyRect rect;    // 没有旋转之前的矩形框, 0 - 1 之间的相对坐标系
    double rotation; // 旋转角度，弧度制
} SkyTrackBox;

SKYMEDIA_C_API SkyTrackBox SkyTrackBox_makeDefault(void);
// 使用 videoSize 为参考，获取旋转后矩形的四个顶点，顺序见注释
SKYMEDIA_C_API void SkyTrackBox_getPoints(SkyTrackBox box, SkyVec2D points[4], int videoWidth, int videoHeight);

///////////////////////////////////////////////////////
typedef enum {
    SkySampleFormat_None = -1,
    SkySampleFormat_U8,     ///< unsigned 8 bits
    SkySampleFormat_S16,    ///< signed 16 bits
    SkySampleFormat_S32,    ///< signed 32 bits
    SkySampleFormat_S64,    ///< signed 64 bits
    SkySampleFormat_Float,  ///< float
    SkySampleFormat_Double, ///< double
} SkySampleFormat;

typedef enum {
    SkyScaleMode_Parent = -1,
    SkyScaleMode_AspectFill = 0,
    SkyScaleMode_AspectFit = 1,
    SkyScaleMode_ScaleToFill = 2,
} SkyScaleMode;

typedef enum {
    SkyDurationMode_Max = 0,   // 取最长的时长作为总时长
    SkyDurationMode_Video = 1, // 以视频时长作为总时长
} SkyDurationMode;

typedef struct SkyAudioParams {
    SkySampleFormat format;
    int channelCount;
    int sampleRate;
} SkyAudioParams;

SKYMEDIA_C_API SkyAudioParams SkyAudioParams_makeDefault(void);

typedef struct SkyVideoParams {
    int width;
    int height;
    double frameRate;
    SkyScaleMode scaleMode;
} SkyVideoParams;

SKYMEDIA_C_API SkyVideoParams SkyVideoParams_makeDefault(void);

typedef struct SkyTrackLayout {
    SkyVec2D size;
    SkyVec2D position;
    SkyScaleMode contentMode;
    SkyBool clipsToBounds;
} SkyTrackLayout;
SKYMEDIA_C_API SkyTrackLayout SkyTrackLayout_makeDefault(void);

typedef struct SkyEncodingParams {
    // 视频参数
    SkyBool isVideoEnabled;
    SkyBool isVideoHardencode;
    int64_t videoBitRate;
    int crf;
    SkyVideoParams videoParams;
    // 音频参数
    SkyBool isAudioEnabled;
    int64_t audioBitRate;
    SkyAudioParams audioParams;

    SkyDurationMode durationMode;

    // 编码时候隔多少帧调用一次进度回调
    int notifyProgressInterval;
} SkyEncodingParams;

SKYMEDIA_C_API SkyEncodingParams SkyEncodingParams_makeDefault(void);

/////////     SkyFrame.h begin    //////////
typedef struct SkyVideoFrame SkyVideoFrame;
SKYMEDIA_C_API SkyVideoFrame *SkyVideoFrame_loadRGBAFromFile(const char *path);
SKYMEDIA_C_API SkyVideoFrame *SkyVideoFrame_makeRGBA(const uint8_t *data, int width, int height);
SKYMEDIA_C_API SkyVideoFrame *
SkyVideoFrame_makeRGBANoCopy(const uint8_t *data, int width, int height, void (*onFree)(void *data));
SKYMEDIA_C_API SkyVideoFrame *SkyVideoFrame_create(void);
SKYMEDIA_C_API SkyVideoFrame *SkyVideoFrame_retain(SkyVideoFrame *ref);
SKYMEDIA_C_API void SkyVideoFrame_release(SkyVideoFrame *ref);
SKYMEDIA_C_API SkyBool SkyVideoFrame_isAvailable(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API int SkyVideoFrame_getWidth(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API int SkyVideoFrame_getHeight(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API double SkyVideoFrame_getDisplayRotation(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API SkyBool SkyVideoFrame_isGPUTexture(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API int SkyVideoFrame_getGPUTextureID(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API SkyBool SkyVideoFrame_isRGBA8(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API const uint8_t *SkyVideoFrame_getRGBA8Data(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API int SkyVideoFrame_getRGBA8DatSize(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API SkyBool SkyVideoFrame_isGapPlaceholder(SkyVideoFrame *skyVideoFrame);
SKYMEDIA_C_API SkyBool SkyVideoFrame_savePngToFile(SkyVideoFrame *ref, const char *path);
SKYMEDIA_C_API SkyBool SkyVideoFrame_saveJpgToFile(SkyVideoFrame *ref, const char *path, int quality);

typedef struct SkyAudioFrame SkyAudioFrame;
SKYMEDIA_C_API SkyAudioFrame *SkyAudioFrame_create(void);
SKYMEDIA_C_API SkyAudioFrame *SkyAudioFrame_retain(SkyAudioFrame *ref);
SKYMEDIA_C_API void SkyAudioFrame_release(SkyAudioFrame *ref);
SKYMEDIA_C_API SkyBool SkyAudioFrame_isAvailable(SkyAudioFrame *skyAudioFrame);
SKYMEDIA_C_API SkyAudioParams SkyAudioFrame_getParams(SkyAudioFrame *skyAudioFrame);
SKYMEDIA_C_API int SkyAudioFrame_getSampleCount(SkyAudioFrame *skyAudioFrame);
SKYMEDIA_C_API int SkyAudioFrame_getDataSize(SkyAudioFrame *skyAudioFrame);
SKYMEDIA_C_API const uint8_t *SkyAudioFrame_getData(SkyAudioFrame *skyAudioFrame);

/////////     SkyFrame.h end    ////////////

/////////     SkyVariant.h begin    //////////

typedef enum {
    SkyVariantType_Null = 0,
    SkyVariantType_Bool = 1,
    SkyVariantType_Int64 = 2,
    SkyVariantType_Double = 3,
    SkyVariantType_String = 4,
    SkyVariantType_Array = 5,
    SkyVariantType_Object = 6,
    SkyVariantType_Binary = 7,
} SkyVariantType;

typedef struct {
    uint8_t *data;
    int64_t size;
} SkyBinary;

typedef enum {
    SkyReturnMode_Retained = 0,    // 返回值内部调用了 retain, 外面需要 release。
    SkyReturnMode_NotRetained = 1, // 返回值内部没有调用 retain，外面不需要 release
} SkyReturnMode;

typedef struct SkyVariant SkyVariant;
SKYMEDIA_C_API SkyVariant *SkyVariant_retain(SkyVariant *ref);
SKYMEDIA_C_API void SkyVariant_release(SkyVariant *ref);

SKYMEDIA_C_API SkyVariant *SkyVariant_createNull(void);
SKYMEDIA_C_API SkyVariant *SkyVariant_createArray(void);
SKYMEDIA_C_API SkyVariant *SkyVariant_createObject(void);
SKYMEDIA_C_API SkyVariant *SkyVariant_createPath(const char *path);

SKYMEDIA_C_API SkyVariant *SkyVariant_createDouble(double value);
SKYMEDIA_C_API SkyVariant *SkyVariant_createInt64(int64_t val);
SKYMEDIA_C_API SkyVariant *SkyVariant_createBool(SkyBool val);
SKYMEDIA_C_API SkyVariant *SkyVariant_createString(const char *val);
SKYMEDIA_C_API SkyVariant *SkyVariant_createBinary(SkyBinary val, void *ctx, void (*onFree)(void *ctx, uint8_t *data));

SKYMEDIA_C_API SkyBool SkyVariant_hasPathTag(SkyVariant *ref);

SKYMEDIA_C_API const char *SkyVariant_toString(SkyVariant *ref);
SKYMEDIA_C_API double SkyVariant_toDouble(SkyVariant *ref);
SKYMEDIA_C_API SkyBool SkyVariant_toBool(SkyVariant *ref);
SKYMEDIA_C_API int64_t SkyVariant_toInt64(SkyVariant *ref);
SKYMEDIA_C_API SkyBinary SkyVariant_toBinary(SkyVariant *ref);

SKYMEDIA_C_API SkyBool SkyVariant_empty(SkyVariant *ref);
SKYMEDIA_C_API int SkyVariant_getSize(SkyVariant *ref);

SKYMEDIA_C_API SkyVariant *SkyVariant_at(SkyVariant *ref, int index, SkyReturnMode mode);
SKYMEDIA_C_API void SkyVariant_append(SkyVariant *ref, SkyVariant *variant);
SKYMEDIA_C_API void SkyVariant_replace(SkyVariant *ref, int index, SkyVariant *variant);

SKYMEDIA_C_API SkyVariant *SkyVariant_find(SkyVariant *ref, const char *key, SkyReturnMode mode);
SKYMEDIA_C_API void SkyVariant_getAllKeys(SkyVariant *ref, SkyString **holder, int size);
SKYMEDIA_C_API void SkyVariant_insert(SkyVariant *ref, const char *key, SkyVariant *val);
SKYMEDIA_C_API void SkyVariant_replace_byKey(SkyVariant *ref, const char *key, SkyVariant *variant);

SKYMEDIA_C_API SkyString *SkyVariant_transToJsonString(SkyVariant *skyVariant, SkyBool isPretty);
SKYMEDIA_C_API SkyVariant *SkyVariant_parseFromJsonString(const char *json);

SKYMEDIA_C_API SkyVariantType SkyVariant_getType(SkyVariant *skyVariant);

typedef enum {
    SkyError_Success = 0,
    SkyError_Cancel = 1,
    SkyError_NativeIsNull = 2,

    SkyError_ExportCancel = 1,
    SkyError_ExportNativeIsNull = 2,
    SkyError_ExportAllocOutputContext = 100,
    SkyError_ExportOpenVideoStream = 101,
    SkyError_ExportOpenAudioStream = 102,
    SkyError_ExportAvioOpen = 103,
    SkyError_ExportWriteHeader = 104,
} SkyError;

typedef enum {
    SKY_COLOR_ATTACHMENT0 = 0x8CE0,
    SKY_DEPTH_ATTACHMENT = 0x8D00,
    SKY_STENCIL_ATTACHMENT = 0x8D20,
} SkyAttachment;

typedef struct SkyTimeRange {
    double begin;
    double end;
} SkyTimeRange;

SKYMEDIA_C_API SkyTimeRange SkyTimeRange_makeDefault(void);

typedef enum {
    SkyEffectCategory_Video,
    SkyEffectCategory_Audio,
} SkyEffectCategory;

typedef struct SkyEffectDescriptor {
    SkyString *className;
    SkyVariant *params;
    SkyTimeRange timeRange;
} SkyEffectDescriptor;

typedef struct SkyEffect SkyEffect;
typedef struct SkyCurve SkyCurve;
typedef struct SkyTimeline SkyTimeline;
typedef struct SkyAudioTrack SkyAudioTrack;
typedef struct SkyVideoTrack SkyVideoTrack;
typedef struct SkyTrack SkyTrack;
typedef struct SkyClip SkyClip;
typedef struct SkyVideoClip SkyVideoClip;
typedef struct SkyInputList SkyInputList;
typedef struct SkyOFWrapper SkyOFWrapper;
typedef struct SkyTransition SkyTransition;
typedef struct SkyOFWrapper SkyOFWrapper;
typedef struct SkyResource SkyResource;
typedef struct SkyWavInput SkyWavInput;
typedef struct SkyCaches SkyCaches;
typedef struct SkyVideoDecoder SkyVideoDecoder;
typedef struct SkyAudioDecoder SkyAudioDecoder;

/////////     SkyVariant.h end    //////////

/////////     SkyTimeline.h     ////////
static const char *SkyOrangeEffectClassName = "SkyOrangeEffect";
static const char *SkyOrangeTransitionClassName = "SkyOrangeTransition";

typedef struct SkyObject SkyObject;
typedef struct SkyTimelinListener {
    void *userData;

    void (*onTimelineNeedsDisplay)(void *userData);

    void (*onDidRemoveObjects)(void *userData, const SkyObject **objects, int objSize);
} SkyTimelinListener;

typedef struct SkyListenerID {
    int64_t value;
} SkyListenerID;

typedef struct SkyEncodingCallback {
    void *userData;
    SkyBool (*shouldBeCancelled)(void *userData);
    void (*onProgress)(void *userData, double currentTime, double totalTime);
    void (*onFinish)(void *userData);
    void (*onError)(void *userData, SkyError error);
} SkyEncodingCallback;

typedef enum {
    SkyPlaceClipStrategy_Strict = 0,      // 出现重叠时，会失败。
    SkyPlaceClipStrategy_AdjustAfter = 1, // 出现重叠时，自动调整后面 clip 的位置。
} SkyPlaceClipStrategy;

typedef struct SkyStringList SkyStringList;
SKYMEDIA_C_API SkyStringList *SkyStringList_create(void);
SKYMEDIA_C_API SkyStringList *SkyStringList_retain(SkyStringList *ref);
SKYMEDIA_C_API void SkyStringList_release(SkyStringList *ref);
SKYMEDIA_C_API void SkyStringList_append(SkyStringList *stringList, const char *str);
SKYMEDIA_C_API int SkyStringList_size(SkyStringList *stringList);
SKYMEDIA_C_API const char *SkyStringList_c_strAt(SkyStringList *stringList, int index);

typedef enum {
    SkyCachesStrategy_None = 0,          // 不缓存
    SkyCachesStrategy_Memory = 1,        // 只缓存在内存（内存不足时，会自动清除）
    SkyCachesStrategy_MemoryAndDisk = 2, // 内存和磁盘（内存不足时，会写到磁盘）
} SkyCachesStrategy;

typedef struct SkyFrameDataDescriptor {
    SkyString *className;
    SkyVariant *params;
    SkyString *name;
    SkyStringList *inputKeys;
    SkyCachesStrategy cachesStrategy;
} SkyFrameDataDescriptor;

// SKYMEDIA_C_API SkyTimeline *SkyTimeline_create(const char *cacheDirectory,
//                                               SkyAudioParams audioParams,
//                                               SkyVideoParams videoParams);
SKYMEDIA_C_API void SkyTimeline_destroy(SkyTimeline *timeline);

SKYMEDIA_C_API SkyListenerID SkyTimeline_addListener(SkyTimeline *timeline, SkyTimelinListener listener);
SKYMEDIA_C_API SkyBool SkyTimeline_removeListener(SkyTimeline *timeline, SkyListenerID ID);
SKYMEDIA_C_API void SkyTimeline_removeAllListeners(SkyTimeline *timeline);

SKYMEDIA_C_API SkyAudioTrack *SkyTimeline_appendAudioTrack(SkyTimeline *timeline);
SKYMEDIA_C_API SkyVideoTrack *SkyTimeline_appendVideoTrack(SkyTimeline *timeline);

SKYMEDIA_C_API SkyBool SkyTimeline_removeTrack(SkyTimeline *timeline, SkyTrack *track);
SKYMEDIA_C_API void SkyTimeline_removeAllTracks(SkyTimeline *timeline);

SKYMEDIA_C_API int SkyTimeline_numberOfTracks(SkyTimeline *timeline);
SKYMEDIA_C_API SkyTrack *SkyTimeline_getTrackAt(SkyTimeline *timeline, int index);
SKYMEDIA_C_API void SkyTimeline_getTracks(SkyTimeline *timeline, SkyTrack **tracks, int size);
SKYMEDIA_C_API SkyBool SkyTimeline_moveTrack(SkyTimeline *timeline, SkyTrack *track, int destIndex);
SKYMEDIA_C_API SkyTrack *SkyTimeline_findTrackByName(SkyTimeline *timeline, const char *name);
SKYMEDIA_C_API SkyClip *SkyTimeline_findClipByName(SkyTimeline *timeline, const char *name);
SKYMEDIA_C_API SkyEffect *SkyTimeline_findEffectByName(SkyTimeline *timeline, const char *name);
SKYMEDIA_C_API SkyTransition *SkyTimeline_findTransitionByName(SkyTimeline *timeline, const char *name);
SKYMEDIA_C_API SkyObject *SkyTimeline_findObjectByName(SkyTimeline *timeline, const char *name);
SKYMEDIA_C_API double SkyTimeline_getCurrentTime(SkyTimeline *timeline);
SKYMEDIA_C_API double SkyTimeline_getDuration(SkyTimeline *timeline);
SKYMEDIA_C_API double SkyTimeline_getVideoDuration(SkyTimeline *timeline);
SKYMEDIA_C_API double SkyTimeline_getAudioDuration(SkyTimeline *timeline);
SKYMEDIA_C_API SkyAudioParams SkyTimeline_getAudioParams(SkyTimeline *timeline);
SKYMEDIA_C_API void SkyTimeline_setVideoParams(SkyTimeline *timeline, const SkyVideoParams videoParams);
SKYMEDIA_C_API SkyVideoParams SkyTimeline_getVideoParams(SkyTimeline *timeline);
SKYMEDIA_C_API void SkyTimeline_setCaches(SkyTimeline *ref, SkyCaches *caches);
SKYMEDIA_C_API SkyCaches *SkyTimeline_getCaches(SkyTimeline *ref);
SKYMEDIA_C_API void SkyTimeline_seekTo(SkyTimeline *timeline, double timestamp);
SKYMEDIA_C_API int SkyTimeline_numberOfEffects(SkyTimeline *timeline);
SKYMEDIA_C_API void SkyTimeline_getEffects(SkyTimeline *timeline, SkyEffect **effects, int size);
SKYMEDIA_C_API SkyEffect *SkyTimeline_addEffect(SkyTimeline *timeline, const SkyEffectDescriptor desc);
// 将指针跟时间线的绑定起来，可扩展 Timeline 的功能, 比如接入 OF 等。Timeline 保存的时候，不会保存这个指针。
SKYMEDIA_C_API void
SkyTimeline_setExtentPointer(SkyTimeline *timeline, const char *key, void *ptr, void (*onExtentPtrDestory)(void *));
SKYMEDIA_C_API void *SkyTimeline_getExtentPointer(SkyTimeline *timeline, const char *key);
SKYMEDIA_C_API SkyVariant *SkyTimeline_saveConfig(SkyTimeline *timeline, const char **baseDirectories, int size);
SKYMEDIA_C_API SkyBool SkyTimeline_loadConfig(SkyTimeline *timeline,
                                              const SkyVariant *v,
                                              const char **baseDirectories,
                                              int size);

SKYMEDIA_C_API SkyVideoDecoder *SkyTimeline_openVideoDecoder(SkyTimeline *timeline, SkyResource *res);
SKYMEDIA_C_API SkyAudioDecoder *SkyTimeline_openAudioDecoder(SkyTimeline *timeline, SkyResource *res);

SKYMEDIA_C_API int SkyTimeline_getMaxGPUTextureSize(SkyTimeline *timeline);

// 下面几个函数，用到 OpenGL 的函数，需要在 OpenGL 的环境中执行
SKYMEDIA_C_API SkyBool SkyTimeline_exportVideo(SkyTimeline *timeline,
                                               const char *filePath,
                                               SkyEncodingParams params,
                                               SkyEncodingCallback callback);
SKYMEDIA_C_API SkyVideoFrame *SkyTimeline_snapCurrentVideo(SkyTimeline *timeline);
SKYMEDIA_C_API SkyVideoFrame *SkyTimeline_transToGPUFrame(SkyTimeline *timeline, SkyVideoFrame *frame);
SKYMEDIA_C_API SkyVideoFrame *SkyTimeline_transToRGBA8Frame(SkyTimeline *timeline, SkyVideoFrame *frame);

////////////////////////

SKYMEDIA_C_API SkyTimeline *SkyTrack_getTimeline(SkyTrack *track);
SKYMEDIA_C_API double SkyTrack_getDuration(SkyTrack *track);
SKYMEDIA_C_API SkyBool SkyTrack_isAudioTrack(SkyTrack *track);
SKYMEDIA_C_API SkyBool SkyTrack_isVideoTrack(SkyTrack *track);
SKYMEDIA_C_API int SkyTrack_getIndex(SkyTrack *track);
SKYMEDIA_C_API SkyBool SkyTrack_isLocked(SkyTrack *track);
SKYMEDIA_C_API void SkyTrack_setLocked(SkyTrack *track, SkyBool flag);
SKYMEDIA_C_API SkyBool SkyTrack_isHidden(SkyTrack *track);
SKYMEDIA_C_API void SkyTrack_setHidden(SkyTrack *track, SkyBool flag);
SKYMEDIA_C_API SkyBool SkyTrack_isMute(SkyTrack *track);
SKYMEDIA_C_API void SkyTrack_setMute(SkyTrack *track, SkyBool flag);
SKYMEDIA_C_API void SkyTrack_setVolume(SkyTrack *track, double volume);
SKYMEDIA_C_API double SkyTrack_getVolume(SkyTrack *track);
// 删除片段。keepGap 用于指定片段移除后，是否保留该片段在轨道上的空间。
SKYMEDIA_C_API SkyBool SkyTrack_removeClip(SkyTrack *track, SkyClip *clip, SkyBool keepGap);
SKYMEDIA_C_API SkyClip *SkyTrack_appendClip(SkyTrack *track, SkyResource *res, SkyTimeRange trimRange);
// 分割片段。splitPoint 为分割点，是 Track 上的时间。假如分割成功，返回分割后的片段。
// 比如 Track 上有片段 A B C。将 B 分割，假如成功，就会变成 B1 B2, 这时会返回 B1，原来的 B 数据会被原地修改，变成
// B2。 于是分割之后，Track 上的片段为 A B1 B2 C。
// 假如不成功，返回空指针。
SKYMEDIA_C_API SkyClip *SkyTrack_splitClip(SkyTrack *track, SkyClip *clip, double splitPoint);
// 将两个片段合并成一个片段，是 splitClip 的逆操作。需要满足一些条件
// 1. clipA 在 clipB 的前面。
// 2. 片段之间没有任何空隙。
// 2. 片段是打开相同的资源，并且资源之间也是连续的。
// 成功合并, 返回 true，clipB 的数据会被更新，而 clipA 会被销毁。合并失败，会返回 false
SKYMEDIA_C_API SkyBool SkyTrack_mergeClips(SkyTrack *track, SkyClip *clipA, SkyClip *clipB);
// 调整 clip 的顺序，移动到索引为 destIndex 的位置，索引为 0 表示在最前面。
// 其中特殊的值，索引为 -1 表示在最后面。
SKYMEDIA_C_API SkyBool SkyTrack_moveClip(SkyTrack *track, SkyClip *clip, int destIndex);
SKYMEDIA_C_API SkyBool SkyTrack_reorderClips1(SkyTrack *track, const SkyClip **clips, int size);
SKYMEDIA_C_API SkyBool SkyTrack_reorderClips(SkyTrack *track, const SkyClip **clips, const int *indics, int size);
// 自由摆放 clip, 其他 clip 尽可能让其不动。strategy 是摆放时，出现重叠的处理行为, 具体见注释。
SKYMEDIA_C_API SkyBool SkyTrack_placeClip(SkyTrack *track,
                                          SkyClip *clip,
                                          const SkyTimeRange trimRange,
                                          double trackBeginTime,
                                          SkyPlaceClipStrategy strategy);
SKYMEDIA_C_API int SkyTrack_numberOfClips(SkyTrack *track);
SKYMEDIA_C_API SkyClip *SkyTrack_getClipAt(SkyTrack *track, int index);
SKYMEDIA_C_API void SkyTrack_getClips(SkyTrack *track, SkyClip **clips, int size);
SKYMEDIA_C_API SkyClip *SkyTrack_hitTest(SkyTrack *track, double timestamp);
SKYMEDIA_C_API SkyClip *SkyTrack_findClipByName(SkyTrack *track, const char *name);
SKYMEDIA_C_API void SkyTrack_removeAllGaps(SkyTrack *track);
SKYMEDIA_C_API int SkyTrack_numberOfEffects(SkyTrack *track);
SKYMEDIA_C_API void SkyTrack_getEffects(SkyTrack *track, SkyEffect **effects, int size);
SKYMEDIA_C_API SkyEffect *SkyTrack_addEffect(SkyTrack *track, SkyEffectDescriptor desc);
// 下面接口用于修改 Track 自身的位置, 和内容区域的位置
SKYMEDIA_C_API void SkyTrack_setLayout(SkyTrack *track, const SkyTrackLayout layout);
SKYMEDIA_C_API SkyTrackLayout SkyTrack_getLayout(SkyTrack *track);
SKYMEDIA_C_API void SkyTrack_setTransform(SkyTrack *track, const SkyTransform2D transform);
SKYMEDIA_C_API SkyTransform2D SkyTrack_getTransform(SkyTrack *track);
// 获取 track 的包围框，同时结合了 layout 和 transform 数据
SKYMEDIA_C_API SkyTrackBox SkyTrack_getFinalBox(SkyTrack *track);
SKYMEDIA_C_API void SkyTrack_setContentTransform(SkyTrack *track, const SkyTransform2D transform);
SKYMEDIA_C_API SkyTransform2D SkyTrack_getContentTransform(SkyTrack *track);

/////////     SkyResource.h     ////////
// 表示媒体素材的视频信息
typedef struct SkyVideoStream {
    int streamIndex;
    int width;
    int height;
    int numberOfFrames; // 流中包含的帧数，未知为 0
    double frameRate;
    double duration;
    int64_t bitRate;
    double displayRotation; // 视频的旋转信息，角度制（已被SDK转为 0-360 的范围)
} SkyVideoStream;

// 表示媒体素材的音频信息
typedef struct SkyAudioStream {
    int streamIndex;
    int channelCount;
    int sampleRate;
    double duration;
    int64_t bitRate;
    SkySampleFormat format;
} SkyAudioStream;

typedef struct SkyResource SkyResource;

SKYMEDIA_C_API SkyResource *SkyResource_create(const char *path);
SKYMEDIA_C_API SkyResource *SkyResource_retain(SkyResource *ref);
SKYMEDIA_C_API void SkyResource_release(SkyResource *ref);

SKYMEDIA_C_API SkyResource *SkyResource_makeSolidColor(const SkyColor color, int width, int height);
SKYMEDIA_C_API SkyResource *SkyResource_makeGap(void);
SKYMEDIA_C_API SkyBool SkyResource_isStatic(SkyResource *resource);
SKYMEDIA_C_API SkyBool SkyResource_isAvailable(SkyResource *resource);
SKYMEDIA_C_API SkyString *SkyResource_getPath(SkyResource *resource);
SKYMEDIA_C_API double SkyResource_getDuration(SkyResource *resource);
SKYMEDIA_C_API double SkyResource_getVideoDuration(SkyResource *resource);
SKYMEDIA_C_API double SkyResource_getAudioDuration(SkyResource *resource);
SKYMEDIA_C_API SkyBool SkyResource_hasVideoStream(SkyResource *resource);
SKYMEDIA_C_API SkyBool SkyResource_hasAudioStream(SkyResource *resource);
SKYMEDIA_C_API SkyVideoStream SkyResource_firstVideoStream(SkyResource *resource);
SKYMEDIA_C_API SkyAudioStream SkyResource_firstAudioStream(SkyResource *resource);

/////////     SkyResource.h end    //////////

/////////     SkyEffect.h     ////////

/////////     SkyEffectInputContext     ////////

typedef struct SkyEffectInputContext SkyEffectInputContext;

SKYMEDIA_C_API int SkyEffectInputContext_getVideoCount(SkyEffectInputContext *context);
SKYMEDIA_C_API SkyVideoFrame *SkyEffectInputContext_getVideoAt(SkyEffectInputContext *context, int idx);

SKYMEDIA_C_API int SkyEffectInputContext_getAudioCount(SkyEffectInputContext *context);
SKYMEDIA_C_API SkyAudioFrame *SkyEffectInputContext_getAudioAt(SkyEffectInputContext *context, int idx);

SKYMEDIA_C_API double SkyEffectInputContext_getTimestamp(SkyEffectInputContext *context);
SKYMEDIA_C_API SkyTimeRange SkyEffectInputContext_getTimeRange(SkyEffectInputContext *context);

SKYMEDIA_C_API SkyBool SkyEffectInputContext_useBackgroundAsInput(SkyEffectInputContext *context);
SKYMEDIA_C_API SkyVideoFrame *SkyEffectInputContext_getBackground(SkyEffectInputContext *context);

SKYMEDIA_C_API SkyVideoParams SkyEffectInputContext_getVideoParams(SkyEffectInputContext *context);
SKYMEDIA_C_API void *SkyEffectInputContext_getExtentPointer(SkyEffectInputContext *context, const char *key);

SKYMEDIA_C_API void SkyEffectInputContext_bindSharedFramebuffer(SkyEffectInputContext *context,
                                                                SkyVideoFrame *gpuSample,
                                                                SkyAttachment attachment);
SKYMEDIA_C_API void SkyEffectInputContext_bindSharedFramebuffer2(SkyEffectInputContext *context,
                                                                 int textID,
                                                                 SkyAttachment attachment);

SKYMEDIA_C_API SkyVideoFrame *SkyEffectInputContext_resizeFrame(SkyEffectInputContext *context,
                                                                SkyVideoFrame *frame,
                                                                int width,
                                                                int height,
                                                                SkyScaleMode scaleMode);
SKYMEDIA_C_API SkyVideoFrame *SkyEffectInputContext_cropFrame(SkyEffectInputContext *context,
                                                              SkyVideoFrame *frame,
                                                              SkyRect cropRect);

SKYMEDIA_C_API SkyVideoFrame *
SkyEffectInputContext_allocGPUFrame(SkyEffectInputContext *context, int width, int height, void *data);
SKYMEDIA_C_API SkyVideoFrame *
SkyEffectInputContext_allocGPUFrameRef(SkyEffectInputContext *context, int width, int height, int textID);

SKYMEDIA_C_API SkyVideoFrame *SkyEffectInputContext_transToGPUFrame(SkyEffectInputContext *context,
                                                                    SkyVideoFrame *frame);
SKYMEDIA_C_API SkyVideoFrame *SkyEffectInputContext_transToRGBA8Frame(SkyEffectInputContext *context,
                                                                      SkyVideoFrame *frame);

/////////     SkyEffectInputContext end   ////////

/////////     SkyEffect     ////////

typedef enum {
    SkyEffectFlags_Render = 0x001,   // 是否应用特效的渲染
    SkyEffectFlags_Callback = 0x002, // 是否触发 Callback
    SkyEffectFlags_All = SkyEffectFlags_Render | SkyEffectFlags_Callback,
} SkyEffectFlags;

typedef struct SkyFrameDataCallback SkyFrameDataCallback;
SKYMEDIA_C_API SkyEffect *SkyFrameDataCallback_getOwner(SkyFrameDataCallback *ref);
SKYMEDIA_C_API void SkyFrameDataCallback_removeFromOwner(SkyFrameDataCallback *ref);

SKYMEDIA_C_API SkyTimeline *SkyEffect_getTimeline(SkyEffect *effect);
SKYMEDIA_C_API int SkyEffect_getIndex(SkyEffect *effect);
SKYMEDIA_C_API SkyBool SkyEffect_moveToIndex(SkyEffect *effect, int destIndex);
SKYMEDIA_C_API void SkyEffect_reload(SkyEffect *effect);
SKYMEDIA_C_API void SkyEffect_removeFromOwner(SkyEffect *effect);
SKYMEDIA_C_API void SkyEffect_updateParams(SkyEffect *effect, SkyVariant *params);
SKYMEDIA_C_API void SkyEffect_setParamCurve(SkyEffect *effect, const char *keyPath, SkyCurve *curve);
SKYMEDIA_C_API SkyCurve *SkyEffect_getParamCurve(SkyEffect *effect, const char *keyPath);
SKYMEDIA_C_API SkyStringList *SkyEffect_getParamCurveKeys(SkyEffect *effect);
SKYMEDIA_C_API SkyVariant *SkyEffect_getParams(SkyEffect *effect);
SKYMEDIA_C_API SkyTimeRange SkyEffect_getTimeRange(SkyEffect *effect);
SKYMEDIA_C_API void SkyEffect_setTimeRange(SkyEffect *effect, SkyTimeRange range);
SKYMEDIA_C_API SkyInputList *SkyEffect_getInputList(SkyEffect *effect);
SKYMEDIA_C_API SkyOFWrapper *SkyEffect_getOFWrapper(SkyEffect *effect);
SKYMEDIA_C_API void *SkyEffect_getExtWrapper(SkyEffect *effect, const char *wrapperKey);
SKYMEDIA_C_API SkyVariant *SkyEffect_saveConfig(SkyEffect *effect);
SKYMEDIA_C_API SkyFrameDataCallback *SkyEffect_addFrameDataCallback(SkyEffect *effect,
                                                                    const SkyFrameDataDescriptor *descriptor);
SKYMEDIA_C_API SkyFrameDataCallback *SkyEffect_findCallbackByName(SkyEffect *effect, const char *name);
SKYMEDIA_C_API SkyFrameDataCallback *SkyEffect_findCallbackByClassName(SkyEffect *effect, const char *name);
SKYMEDIA_C_API void SkyEffect_isCacheExistAt(SkyEffect *effect,
                                             const double timestamp,
                                             const SkyFrameDataDescriptor *descriptor);
SKYMEDIA_C_API void SkyEffect_callAsync(SkyEffect *effect, const char *fnName, SkyVariant *args);
SKYMEDIA_C_API void SkyEffect_timedCall(SkyEffect *effect, double t, const char *fnName, SkyVariant *args);
SKYMEDIA_C_API void SkyEffect_removeAllCalls(SkyEffect *effect);
SKYMEDIA_C_API void SkyEffect_setName(SkyEffect *effect, const char *name);
SKYMEDIA_C_API SkyString *SkyEffect_getName(SkyEffect *effect);

SKYMEDIA_C_API void SkyEffect_disable(SkyEffect *effect, SkyEffectFlags flags);
SKYMEDIA_C_API void SkyEffect_enable(SkyEffect *effect, SkyEffectFlags flags);
SKYMEDIA_C_API void SkyEffect_setPriority(SkyEffect *effect, int priority);
SKYMEDIA_C_API int SkyEffect_getPriority(SkyEffect *effect);

SKYMEDIA_C_API double SkyTransition_getDuration(SkyTransition *transition);
SKYMEDIA_C_API void SkyTransition_setDuration(SkyTransition *transition, double duration);

/////////     SkyEffect end    ////////

/////////     SkyEffect.h end    //////////
// 三种可能的触发 Callback 的方式
typedef enum {
    SkyTriggerMode_Snap = 0,   // 调用 Timeline 的 snapCurrentVideo、snapVideoAtTime 时触发。
    SkyTriggerMode_Play = 1,   // Timeline 在播放过程中触发。
    SkyTriggerMode_Export = 2, // Timeline 在导出视频中触发。
} SkyTriggerMode;

typedef struct {
    uint8_t data[16];
} SkyHash;

SKYMEDIA_C_API SkyCaches *SkyCaches_create(void);
SKYMEDIA_C_API SkyCaches *SkyCaches_create_dir(const char *cacheDirectory);
SKYMEDIA_C_API SkyCaches *SkyCaches_retain(SkyCaches *ref);
SKYMEDIA_C_API void SkyCaches_release(SkyCaches *ref);
SKYMEDIA_C_API SkyBool SkyCaches_isAvailable(SkyCaches *ref);
SKYMEDIA_C_API SkyVariant *SkyCaches_get(SkyCaches *ref, SkyHash hash);
SKYMEDIA_C_API void SkyCaches_put(SkyCaches *ref, SkyHash hash, SkyVariant *value, SkyCachesStrategy strategy);

// 内存的最大字节数，默认为 256 MB。超过限制，会从内存中自动清除旧的缓存（有可能会触发写磁盘）
SKYMEDIA_C_API void SkyCaches_setMaxMemoryByteSize(SkyCaches *ref, int64_t maxByteSize);
SKYMEDIA_C_API int64_t SkyCaches_getMaxMemoryByteSize(SkyCaches *ref);

// 磁盘的最大字节数，默认为 2 GB。超过限制，会从磁盘自动清除旧的缓存。
SKYMEDIA_C_API void SkyCaches_setMaxDiskByteSize(SkyCaches *ref, int64_t maxByteSize);
SKYMEDIA_C_API int64_t SkyCaches_getMaxDiskByteSize(SkyCaches *ref);

static const char *SkyInputKey_OriginVideoFrame = "originVideoFrame"; // 从解码器解码出来的，最原始的帧
static const char *SkyInputKey_VideoFrame = "videoFrame";             // 经过特效的那一帧，

typedef struct SkyFrameDataInputContext SkyFrameDataInputContext;
SKYMEDIA_C_API SkyTriggerMode SkyFrameDataInputContext_getTriggerMode(SkyFrameDataInputContext *inputContext);
SKYMEDIA_C_API SkyCaches *SkyFrameDataInputContext_getCaches(SkyFrameDataInputContext *inputContext);
SKYMEDIA_C_API SkyHash SkyFrameDataInputContext_getInputHash(SkyFrameDataInputContext *inputContext);
SKYMEDIA_C_API SkyVariant *SkyFrameDataInputContext_getInputByKey(SkyFrameDataInputContext *inputContext,
                                                                  const char *inputKey);

typedef struct SkyFrameDataClass {
    void *classData;
    void *(*createInstance)(void *classData, SkyVariant *params);
    void (*destoryInstance)(void *instance);
    SkyVariant *(*onFrameData)(void *instance, SkyFrameDataInputContext *inputCtx);
} SkyFrameDataClass;

typedef struct SkyFFmpegCommandCallback {
    void *userData;
    SkyBool (*shouldBeCancelled)(void *userData);
    void (*onProgress)(void *userData, double currentTime);
    void (*onFFmpegLog)(void *userData, int level, const char *msg, int msgsize);
} SkyFFmpegCommandCallback;

///////////////     SkyApi.h begin    //////////////
SKYMEDIA_C_API SkyError SkyApi_getLastError(void);
SKYMEDIA_C_API const char *SkyApi_stringFromError(SkyError error);
SKYMEDIA_C_API const char *SkyApi_getSDKVersion(void);
SKYMEDIA_C_API void SkyApi_initSDK(void);
SKYMEDIA_C_API SkyBool SkyApi_registerTimelineCallback(void (*onCreate)(SkyTimeline *),
                                                       void (*onDestory)(SkyTimeline *));
SKYMEDIA_C_API void SkyApi_registerLogCallback(void (*onLogCallback)(int level, const char *msg, int msgsize));
SKYMEDIA_C_API SkyBool SkyApi_registerFrameDataClass(const char *className, SkyFrameDataClass frameDataClass);
SKYMEDIA_C_API void SkyApi_setMaxNumberOfHardwareDecoder(int maxNumber);
SKYMEDIA_C_API void SkyApi_setDebugMode(SkyBool flag);
SKYMEDIA_C_API void SkyApi_setVenusSegmentModelPaths(const char **modelPaths, int size);
SKYMEDIA_C_API void SkyApi_setVenusFaceModelPaths(const char **modelPaths, int size);
SKYMEDIA_C_API void SkyApi_setVenusSkyModelPaths(const char **modelPaths, int size);
SKYMEDIA_C_API void SkyApi_setVenusHairModelPaths(const char **modelPaths, int size);
SKYMEDIA_C_API void SkyApi_setVenusClothesModelPaths(const char **modelPaths, int size);

SKYMEDIA_C_API int SkyApi_runFFmpegCommand(const char *cmd, SkyFFmpegCommandCallback callback);
SKYMEDIA_C_API int SkyApi_runFFmpegCommand_args(int argc, const char **argv, SkyFFmpegCommandCallback callback);

///////////////     SkyApi.h end      //////////////

/////////     SkyCurve.h    //////////
typedef struct SkyCurveItem {
    double time;
    SkyVariant *value;
    char tweenName[32];
} SkyCurveItem;

typedef struct SkyCurve SkyCurve;

SKYMEDIA_C_API SkyCurve *SkyCurve_create(void);
SKYMEDIA_C_API SkyCurve *SkyCurve_retain(SkyCurve *ref);
SKYMEDIA_C_API void SkyCurve_release(SkyCurve *ref);
SKYMEDIA_C_API SkyBool SkyCurve_isEmpty(SkyCurve *curve);
SKYMEDIA_C_API int SkyCurve_numberOfItems(SkyCurve *curve);
SKYMEDIA_C_API SkyCurveItem SkyCurve_getItemAt(SkyCurve *curve, int index);
SKYMEDIA_C_API void SkyCurve_removeItemAt(SkyCurve *curve, int index);
SKYMEDIA_C_API void SkyCurve_removeItemAtTime(SkyCurve *curve, double time);
SKYMEDIA_C_API int SkyCurve_setItem(SkyCurve *curve, double time, SkyVariant *value, const char *tweenName);
SKYMEDIA_C_API int SkyCurve_setItem2(SkyCurve *curve, SkyCurveItem item);

SKYMEDIA_C_API SkyVariant *SkyCurve_saveConfig(SkyCurve *curve);

SKYMEDIA_C_API void SkyCurve_loadConfig(SkyCurve *curve, SkyVariant *v);

/////////     SkyCurve.h end    //////////

/////////     SkyOFWrapper.h    //////////

typedef struct SkyOFMessageCallback {
    void *userData;
    SkyString *(*onMessageBack)(void *userData, const char *msg);
    void (*onCreateEffect)(void *userData, int contextID, int effectID);
    void (*onDestory)(void *userData);
} SkyOFMessageCallback;

typedef struct SkyOFWrapper SkyOFWrapper;

SKYMEDIA_C_API SkyEffect *SkyOFWrapper_getOwner(SkyOFWrapper *ofWrapper);

SKYMEDIA_C_API void SkyOFWrapper_setMessageCallback(SkyOFWrapper *ofWrapper, SkyOFMessageCallback callback);
SKYMEDIA_C_API void SkyOFWrapper_sendMessage(SkyOFWrapper *ofWrapper, const char *msg);
SKYMEDIA_C_API void SkyOFWrapper_sendTimedMessage(SkyOFWrapper *ofWrapper, double t, const char *msg);
SKYMEDIA_C_API void SkyOFWrapper_removeAllMessages(SkyOFWrapper *ofWrapper);
SKYMEDIA_C_API void SkyOFWrapper_setFaceFrameDataArray(SkyOFWrapper *ofWrapper, const SkyVariant *data);
SKYMEDIA_C_API void SkyOFWrapper_setAvatarInfo(SkyOFWrapper *ofWrapper, const SkyVariant *info);
SKYMEDIA_C_API void SkyOFWrapper_setNeedsUpdateDuration(SkyOFWrapper *ofWrapper, SkyBool needsUpdate);
SKYMEDIA_C_API void SkyOFWrapper_setOFParam(SkyOFWrapper *ofWrapper, const char *key, SkyVariant *value);
SKYMEDIA_C_API SkyVariant *SkyOFWrapper_getOFParam(SkyOFWrapper *ofWrapper, const char *key);

/////////     SkyOFWrapper.h end    //////////

/////////     SkyClip   //////////

typedef struct SkyTimingFunction {
    char tweenName[32]; // 设置默认为 "linear"
} SkyTimingFunction;

typedef struct SkyTransitionDescriptor {
    SkyString *className;
    SkyVariant *params;
    double duration;
    double beforeDurationScale;
    double afterDurationScale;
    SkyTimingFunction beforeTimingFunction;
    SkyTimingFunction afterTimingFunction;
} SkyTransitionDescriptor;

SKYMEDIA_C_API SkyTimeline *SkyClip_getTimeline(SkyClip *clip);
SKYMEDIA_C_API SkyTrack *SkyClip_getTrack(SkyClip *clip);

SKYMEDIA_C_API SkyBool SkyClip_isAudioClip(SkyClip *clip);
SKYMEDIA_C_API SkyBool SkyClip_isVideoClip(SkyClip *clip);

SKYMEDIA_C_API int SkyClip_getIndex(SkyClip *clip);

SKYMEDIA_C_API SkyBool SkyClip_updateResource(SkyClip *clip, SkyResource *res);
SKYMEDIA_C_API SkyBool SkyClip_updateResource2(SkyClip *clip, SkyResource *res, const SkyTimeRange trimRange);

SKYMEDIA_C_API SkyResource *SkyClip_getResource(SkyClip *clip);

SKYMEDIA_C_API double SkyClip_getSpeed(SkyClip *clip);
SKYMEDIA_C_API void SkyClip_setSpeed(SkyClip *clip, double speed);

SKYMEDIA_C_API SkyCurve *SkyClip_getSpeedCurve(SkyClip *clip);
SKYMEDIA_C_API void SkyClip_setSpeedCurve(SkyClip *clip, SkyCurve *curve);

SKYMEDIA_C_API void SkyClip_setVolume(SkyClip *clip, double volume);
SKYMEDIA_C_API double SkyClip_getVolume(SkyClip *clip);

SKYMEDIA_C_API SkyTimeRange SkyClip_getTrackRange(SkyClip *clip);
SKYMEDIA_C_API SkyTimeRange SkyClip_getTrimRange(SkyClip *clip);

SKYMEDIA_C_API double SkyClip_trackTSFromTrimTS(SkyClip *clip, double trimTS);
SKYMEDIA_C_API double SkyClip_trimTSFromTrackTS(SkyClip *clip, double trackTS);

SKYMEDIA_C_API SkyEffect *SkyClip_addEffect(SkyClip *clip, SkyEffectDescriptor desc);
SKYMEDIA_C_API SkyEffect *SkyClip_addEffectFromConfig(SkyClip *clip, SkyVariant *config);
SKYMEDIA_C_API int SkyClip_numberOfEffects(SkyClip *clip);
SKYMEDIA_C_API void SkyClip_getEffects(SkyClip *clip, SkyEffect **effects, int size);

SKYMEDIA_C_API SkyTransition *SkyClip_setTransitionAfter(SkyClip *clip, SkyTransitionDescriptor desc);
SKYMEDIA_C_API SkyTransition *SkyClip_getTransitionAfter(SkyClip *clip);

SKYMEDIA_C_API SkyTransition *SkyClip_setTransitionBefore(SkyClip *clip, SkyTransitionDescriptor desc);
SKYMEDIA_C_API SkyTransition *SkyClip_getTransitionBefore(SkyClip *clip);

SKYMEDIA_C_API SkyBool SkyClip_setGapBefore(SkyClip *clip, double gap);
SKYMEDIA_C_API SkyBool SkyClip_setGapAfter(SkyClip *clip, double gap);
SKYMEDIA_C_API double SkyClip_getGapBefore(SkyClip *clip);
SKYMEDIA_C_API double SkyClip_getGapAfter(SkyClip *clip);

SKYMEDIA_C_API SkyClip *SkyClip_getNext(SkyClip *clip);
SKYMEDIA_C_API SkyClip *SkyClip_getPrev(SkyClip *clip);

SKYMEDIA_C_API void SkyClip_setName(SkyClip *clip, const char *name);
SKYMEDIA_C_API SkyString *SkyClip_getName(SkyClip *clip);

SKYMEDIA_C_API void SkyVideoClip_setCropRect(SkyVideoClip *clip, const SkyRect region);
SKYMEDIA_C_API SkyRect SkyVideoClip_getCropRect(SkyVideoClip *clip);

SKYMEDIA_C_API void SkyVideoClip_setCropTransform(SkyVideoClip *clip, const SkyTransform2D transform);
SKYMEDIA_C_API SkyTransform2D SkyVideoClip_getCropTransform(SkyVideoClip *clip);

// 缩放视频尺寸。cropRect 和 resize 都被设置时，会先裁剪再拉伸。
SKYMEDIA_C_API void SkyVideoClip_resize(SkyVideoClip *clip, int width, int height, SkyScaleMode scaleMode);

SKYMEDIA_C_API SkyVariant *SkyClip_saveConfig(SkyClip *clip);
SKYMEDIA_C_API SkyBool SkyClip_loadConfig(SkyClip *clip, const SkyVariant *v, SkyBool updateResource);

/////////     SkyClip end    //////////

/////////     SkyInputItem    //////////

typedef struct SkyInputList SkyInputList;
typedef struct SkyInputItem SkyInputItem;

SKYMEDIA_C_API SkyInputList *SkyInputItem_getOwner(SkyInputItem *inputItem);

SKYMEDIA_C_API void SkyInputItem_setAsUser(SkyInputItem *inputItem);
SKYMEDIA_C_API void SkyInputItem_setAsUser2(SkyInputItem *inputItem, int userIdx);
SKYMEDIA_C_API void SkyInputItem_setAsBackground(SkyInputItem *inputItem);
SKYMEDIA_C_API void SkyInputItem_setAsResource(SkyInputItem *inputItem, SkyResource *res);
SKYMEDIA_C_API void SkyInputItem_setAsResource2(SkyInputItem *inputItem,
                                                SkyResource *res,
                                                const SkyTimeRange trimRange);

SKYMEDIA_C_API SkyResource *SkyInputItem_getResource(SkyInputItem *inputItem);

SKYMEDIA_C_API SkyTimeRange SkyInputItem_getTrimRange(SkyInputItem *inputItem);

SKYMEDIA_C_API void SkyInputItem_setName(SkyInputItem *inputItem, const char *name);
SKYMEDIA_C_API SkyString *SkyInputItem_getName(SkyInputItem *inputItem);

SKYMEDIA_C_API void SkyInputItem_setCropRect(SkyInputItem *inputItem, const SkyRect region);
SKYMEDIA_C_API SkyRect SkyInputItem_getCropRect(SkyInputItem *inputItem);

SKYMEDIA_C_API void SkyInputItem_setCropTransform(SkyInputItem *inputItem, SkyTransform2D transform);
SKYMEDIA_C_API SkyTransform2D SkyInputItem_getCropTransform(SkyInputItem *inputItem);

// 缩放视频尺寸。cropRect 和 resize 都被设置时，会先裁剪再拉伸。
SKYMEDIA_C_API void SkyInputItem_resize(SkyInputItem *inputItem, int width, int height, SkyScaleMode scaleMode);

/////////     SkyInputItem end    //////////

/////////     SkyInputList    //////////

SKYMEDIA_C_API SkyEffect *SkyInputList_getOwner(SkyInputList *inputIList);

SKYMEDIA_C_API SkyInputItem *SkyInputList_getItemAt(SkyInputList *inputIList, int index);
SKYMEDIA_C_API SkyInputItem *SkyInputList_findItemByName(SkyInputList *inputIList, const char *name);

SKYMEDIA_C_API void SkyInputList_ensureNumber(SkyInputList *inputIList, int number);
SKYMEDIA_C_API int SkyInputList_numberOfItems(SkyInputList *inputIList);

SKYMEDIA_C_API void SkyInputList_commit(SkyInputList *inputIList);

/////////     SkyInputList end    //////////

/////////     SkyAudioParams    //////////

// bytes <-> samples
SKYMEDIA_C_API int SkyAudioParams_bytesFromSamples(SkyAudioParams audio, int samples);
SKYMEDIA_C_API int SkyAudioParams_samplesFromBytes(SkyAudioParams audio, int bytes);

// bytes <-> time
SKYMEDIA_C_API int SkyAudioParams_bytesFromTime(SkyAudioParams audio, double t);
SKYMEDIA_C_API double SkyAudioParams_timeFromBytes(SkyAudioParams audio, int bytes);

// sample <-> time
SKYMEDIA_C_API int SkyAudioParams_samplesFromTime(SkyAudioParams audio, double t);
SKYMEDIA_C_API double SkyAudioParams_timeFromSamples(SkyAudioParams audio, int samples);

SKYMEDIA_C_API int SkyAudioParams_bytesPerSamplePerChannel(SkyAudioParams audio);

/////////     SkyAudioParams    //////////

/////////     SkyVideoDecoder    //////////

typedef enum {
    SkyRetrieveMode_Accurate, // 精确到时间
    SkyRetrieveMode_KeyFrame, // 关键帧
} SkyRetrieveMode;

SKYMEDIA_C_API SkyVideoDecoder *SkyVideoDecoder_create(void);
SKYMEDIA_C_API SkyVideoDecoder *SkyVideoDecoder_retain(SkyVideoDecoder *ref);
SKYMEDIA_C_API void SkyVideoDecoder_release(SkyVideoDecoder *ref);

SKYMEDIA_C_API SkyBool SkyVideoDecoder_isAvailable(SkyVideoDecoder *decoder);

SKYMEDIA_C_API SkyVideoStream SkyVideoDecoder_getVideoStream(SkyVideoDecoder *decoder);

SKYMEDIA_C_API SkyVideoFrame *
SkyVideoDecoder_retrieveVideo(SkyVideoDecoder *decoder, double t, SkyRetrieveMode retrieveMode, SkyBool putInCache);
SKYMEDIA_C_API SkyVideoFrame *SkyVideoDecoder_retrieveVideo2(SkyVideoDecoder *decoder,
                                                             double t,
                                                             SkyRetrieveMode retrieveMode,
                                                             SkyVideoParams videoParams,
                                                             SkyBool putInCache);

/////////     SkyVideoDecoder    //////////

/////////     SkyAudioDecoder    //////////
SKYMEDIA_C_API SkyAudioDecoder *SkyAudioDecoder_create(void);
SKYMEDIA_C_API SkyAudioDecoder *SkyAudioDecoder_retain(SkyAudioDecoder *ref);
SKYMEDIA_C_API void SkyAudioDecoder_release(SkyAudioDecoder *ref);

SKYMEDIA_C_API SkyBool SkyAudioDecoder_isAvailable(SkyAudioDecoder *decoder);

SKYMEDIA_C_API SkyAudioStream SkyAudioDecoder_getAudioStream(SkyAudioDecoder *decoder);

SKYMEDIA_C_API SkyAudioFrame *
SkyAudioDecoder_retrieveAudio(SkyAudioDecoder *decoder, int startSample, int endSample, SkyAudioParams params);
/////////     SkyAudioDecoder    //////////

/////////     SkyWavInput    //////////

SKYMEDIA_C_API SkyWavInput *SkyWavInput_create(const char *wavPath);
SKYMEDIA_C_API SkyWavInput *SkyWavInput_retain(SkyWavInput *ref);
SKYMEDIA_C_API void SkyWavInput_release(SkyWavInput *ref);

SKYMEDIA_C_API SkyBool SkyWavInput_isAvailable(SkyWavInput *wavInput);
SKYMEDIA_C_API SkyString *SkyWavInput_getPath(SkyWavInput *wavInput);
SKYMEDIA_C_API double SkyWavInput_getDuration(SkyWavInput *wavInput);
SKYMEDIA_C_API int getSampleCount(SkyWavInput *wavInput);
SKYMEDIA_C_API SkyAudioParams SkyWavInput_getAudioParams(SkyWavInput *wavInput);

SKYMEDIA_C_API SkyBool SkyWavInput_seekPcm(SkyWavInput *wavInput, int bytePos);
SKYMEDIA_C_API int SkyWavInput_readPcm(SkyWavInput *wavInput, void *bytes, int length);

/////////     SkyWavInput    //////////
SKYMEDIA_C_API void SkyObject_setName(void *obj, const char *name);
SKYMEDIA_C_API SkyString *SkyObject_getName(void *obj);

SKYMEDIA_C_API void SkyObject_setUserData(void *obj, const char *key, SkyVariant *value);
SKYMEDIA_C_API SkyVariant *SkyObject_getUserData(void *obj, const char *key);

#endif
