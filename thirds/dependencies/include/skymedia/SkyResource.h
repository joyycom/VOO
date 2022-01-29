#pragma once

#include "SkyDefs.h"
#include "SkyParams.h"
#include <cstdint>

SKYMEDIA_NAMESPACE_BEGIN

// 表示媒体素材的视频信息
struct SKYMEDIA_API SkyVideoStream final {
    int streamIndex = -1;
    int width = 0;
    int height = 0;
    int numberOfFrames = 1; // 流中包含的帧数，未知为 0
    double frameRate = 0;
    double duration = 0;
    int64_t bitRate = 0;
    double displayRotation = 0; // 视频的旋转信息，角度制（已被SDK转为 0-360 的范围)
};

// 表示媒体素材的音频信息
struct SKYMEDIA_API SkyAudioStream final {
    int streamIndex = -1;
    int channelCount = 0;
    int sampleRate = 0;
    double duration = 0;
    int64_t bitRate = 0;
    SkySampleFormat format = SkySampleFormat_None;
};

class SKYMEDIA_API SkyResource final {
public:
    SkyResource();
    explicit SkyResource(const char *path);
    SkyResource(const SkyResource &rhs);
    SkyResource &operator=(const SkyResource &rhs);
    ~SkyResource();

    static SkyResource makeSolidColor(const SkyColor &color, int width, int height);
    static SkyResource makeGap();
    static SkyResource makeIO(const char *className, const char *keyPath);

    bool isStatic() const;

    bool isAvailable() const;

    SkyString getPath() const;
    double getDuration() const;
    double getVideoDuration() const;
    double getAudioDuration() const;

    // 判断是否有音频流和视频流
    bool hasVideoStream() const;
    bool hasAudioStream() const;

    // 找不到的话，相应的流索引为 -1
    SkyVideoStream firstVideoStream() const;
    SkyAudioStream firstAudioStream() const;

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
