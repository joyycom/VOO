#pragma once

#include "SkyDefs.h"
#include "SkyParams.h"

SKYMEDIA_NAMESPACE_BEGIN

class SKYMEDIA_API SkyWavInput final {
public:
    SkyWavInput();
    SkyWavInput(const char *wavPath);
    SkyWavInput(const SkyWavInput &rhs);
    SkyWavInput &operator=(const SkyWavInput &rhs);
    ~SkyWavInput();

    bool isAvailable() const;

    SkyString getPath() const;

    double getDuration() const;
    int getSampleCount() const;
    SkyAudioParams getAudioParams() const;

    bool seekPcm(int bytePos);
    int readPcm(void *bytes, int length);

private:
    void *m_impl;
};

class SKYMEDIA_API SkyWavOuput final {
public:
    SkyWavOuput();
    SkyWavOuput(const char *wavPath, const SkyAudioParams &audioParams);
    SkyWavOuput(const SkyWavOuput &rhs);
    SkyWavOuput &operator=(const SkyWavOuput &rhs);
    ~SkyWavOuput();

    bool isAvailable() const;
    SkyString getPath() const;
    SkyAudioParams getAudioParams() const;

    void writePcm(const void *bytes, int length);

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
