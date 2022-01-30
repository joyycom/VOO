#pragma once

#include "SkyDefs.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkySampleFormat_None = -1,
    SkySampleFormat_U8 = 0,     ///< unsigned 8 bits
    SkySampleFormat_S16 = 1,    ///< signed 16 bits
    SkySampleFormat_S32 = 2,    ///< signed 32 bits
    SkySampleFormat_S64 = 3,    ///< signed 64 bits
    SkySampleFormat_Float = 4,  ///< float
    SkySampleFormat_Double = 5, ///< double
} SkySampleFormat;

typedef enum {
    SkyScaleMode_Parent = -1,
    SkyScaleMode_AspectFill = 0,
    SkyScaleMode_AspectFit = 1,
    SkyScaleMode_ScaleToFill = 2,
} SkyScaleMode;

struct SKYMEDIA_API SkyVideoParams final {
    int width = 544;
    int height = 960;
    double frameRate = 30;
    SkyScaleMode scaleMode = SkyScaleMode_AspectFit;
};

struct SKYMEDIA_API SkyAudioParams final {
    SkySampleFormat format = SkySampleFormat_S16;
    int channelCount = 2;
    int sampleRate = 44100;

    // bytes <-> samples
    int bytesFromSamples(int samples) const;
    int samplesFromBytes(int bytes) const;

    // bytes <-> time
    int bytesFromTime(double t) const;
    double timeFromBytes(int bytes) const;

    // sample <-> time
    int samplesFromTime(double t) const;
    double timeFromSamples(int samples) const;

    int bytesPerSamplePerChannel() const;
};

SKYMEDIA_NAMESPACE_END
