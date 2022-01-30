#pragma once

#include "SkyAsyncTask.h"
#include "SkyCaches.h"
#include "SkyCurve.h"
#include "SkyEffect.h"
#include "SkyEffectClass.h"
#include "SkyError.h"
#include "SkyFrame.h"
#include "SkyHandleMap.h"
#include "SkyLog.h"
#include "SkyResource.h"
#include "SkyTimeline.h"
#include "SkyVariant.h"
#include <stdio.h>

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkySeekType_Set = SEEK_SET,
    SkySeekType_Cur = SEEK_CUR,
    SkySeekType_End = SEEK_END,
} SkySeekType;

struct SKYMEDIA_API SkyIOContextClass final {
    void *userData = nullptr;
    void *(*openIOContext)(void *userData, const char *keyPath) = nullptr;
    void (*closeIOContext)(void *ioCtx) = nullptr;

    int (*read)(void *ioCtx, uint8_t *data, int size) = nullptr;
    int (*seek)(void *ioCtx, int64_t offset, int whence) = nullptr;
    int64_t (*getSize)(void *ioCtx) = nullptr;
    int (*isEOF)(void *ioCtx) = nullptr;
};

struct SKYMEDIA_API SkyFFmpegCommandCallback final {
    void *userData = nullptr;
    bool (*shouldBeCancelled)(void *userData) = nullptr;
    void (*onProgress)(void *userData, double currentTime) = nullptr;
    void (*onFFmpegLog)(void *userData, int level, const char *msg, int msgsize);
};

struct SKYMEDIA_API SkyApi final {
    static void initSDK();

    static SkyError getLastError();
    static const char *stringFromError(SkyError error);
    static const char *getSDKVersion();
    static double getCurrentSystemTimestamp();

    static bool registerTimelineCallback(void (*onCreate)(SkyTimeline *), void (*onDestory)(SkyTimeline *));
    static void registerLogCallback(void (*onLogCallback)(int level, const char *msg, int msgsize));

    static bool registerEffectClass(const char *className, const SkyEffectClass &effectClass);
    static bool registerIOContext(const char *className, const SkyIOContextClass &ioClass);
    static bool registerFrameDataClass(const char *className, const SkyFrameDataClass &frameDataClass);
    static bool registerTweenFormula(const char *tweenName, double (*formula)(double t));

    static void setMaxNumberOfHardwareDecoder(int maxNumber);
    static void setDebugMode(bool flag);

    static void setVenusSegmentModelPaths(const char **modelPaths, int size);
    static void setVenusFaceModelPaths(const char **modelPaths, int size);
    static void setVenusSkyModelPaths(const char **modelPaths, int size);
    static void setVenusHairModelPaths(const char **modelPaths, int size);
    static void setVenusClothesModelPaths(const char **modelPaths, int size);

    static int runFFmpegCommand(const char *cmd, SkyFFmpegCommandCallback callback);
    static int runFFmpegCommand(int argc, const char **argv, SkyFFmpegCommandCallback callback);
};

SKYMEDIA_NAMESPACE_END
