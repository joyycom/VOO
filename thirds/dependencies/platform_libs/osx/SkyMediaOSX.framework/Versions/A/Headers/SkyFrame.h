#pragma once

#include "SkyDefs.h"
#include "SkyParams.h"
#include <cstdint>

SKYMEDIA_NAMESPACE_BEGIN

class SKYMEDIA_API SkyVideoFrame final {
public:
    SkyVideoFrame();
    SkyVideoFrame(const SkyVideoFrame &rhs);
    SkyVideoFrame &operator=(const SkyVideoFrame &rhs);
    ~SkyVideoFrame();

    static SkyVideoFrame makeRGBA(const uint8_t *data, int width, int height);
    static SkyVideoFrame makeRGBANoCopy(const uint8_t *data, int width, int height, void (*onFree)(void *data));
    static SkyVideoFrame loadRGBAFromFile(const char *path);

    bool isAvailable() const;

    int getWidth() const;
    int getHeight() const;
    double getDisplayRotation() const;

    bool isGPUTexture() const;
    int getGPUTextureID() const;

    bool isRGBA8() const;
    const uint8_t *getRGBA8Data() const;
    int getRGBA8DatSize() const;

    bool isGapPlaceholder() const;

    // 注意，需要 RGBA 数据类型，才能保存到文件。假如是纹理，保存失败。
    bool savePngToFile(const char *path) const;
    bool saveJpgToFile(const char *path, int quality) const;

private:
    void *m_impl;
};

class SKYMEDIA_API SkyAudioFrame final {
public:
    SkyAudioFrame();
    SkyAudioFrame(const SkyAudioFrame &rhs);
    SkyAudioFrame &operator=(const SkyAudioFrame &rhs);
    ~SkyAudioFrame();

    bool isAvailable() const;

    SkyAudioParams getParams() const;
    int getSampleCount() const;
    int getDataSize() const;
    const uint8_t *getData() const;

private:
    void *m_impl;
};

// 音频重采样
class SKYMEDIA_API SkyAudioConverter final {
public:
    SkyAudioConverter();
    ~SkyAudioConverter();
    SkyAudioConverter(const SkyAudioConverter &rhs);
    SkyAudioConverter &operator=(const SkyAudioConverter &rhs);

    SkyAudioFrame resample(const SkyAudioFrame &inputFrame, const SkyAudioParams &outputParams);

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
