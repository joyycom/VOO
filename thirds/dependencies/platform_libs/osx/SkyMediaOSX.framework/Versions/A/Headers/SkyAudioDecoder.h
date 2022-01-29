#pragma once

#include "SkyFrame.h"
#include "SkyResource.h"

SKYMEDIA_NAMESPACE_BEGIN

class SKYMEDIA_API SkyAudioDecoder final {
public:
    SkyAudioDecoder();
    SkyAudioDecoder(const SkyAudioDecoder &rhs);
    SkyAudioDecoder &operator=(const SkyAudioDecoder &rhs);
    ~SkyAudioDecoder();

    bool isAvailable() const;

    SkyAudioStream getAudioStream() const;

    SkyAudioFrame retrieveAudio(int startSample, int endSample, const SkyAudioParams &params) const;

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
