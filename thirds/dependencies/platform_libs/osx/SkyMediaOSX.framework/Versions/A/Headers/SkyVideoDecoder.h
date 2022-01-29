#pragma once

#include "SkyFrame.h"
#include "SkyResource.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyRetrieveMode_Accurate = 0, // 精确到时间
    SkyRetrieveMode_KeyFrame = 1, // 关键帧
} SkyRetrieveMode;

class SKYMEDIA_API SkyVideoDecoder final {
public:
    SkyVideoDecoder();
    SkyVideoDecoder(const SkyVideoDecoder &rhs);
    SkyVideoDecoder &operator=(const SkyVideoDecoder &rhs);
    ~SkyVideoDecoder();

    bool isAvailable() const;

    SkyVideoStream getVideoStream() const;

    SkyVideoFrame retrieveVideo(double t, SkyRetrieveMode retrieveMode) const;
    SkyVideoFrame retrieveVideo(double t, SkyRetrieveMode retrieveMode, SkyVideoParams videoParams) const;

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
