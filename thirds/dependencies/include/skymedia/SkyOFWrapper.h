#pragma once

#include "SkyDefs.h"
#include "SkyVariant.h"

SKYMEDIA_NAMESPACE_BEGIN

struct SKYMEDIA_API SkyOFMessageCallback final {
    void *userData = nullptr;
    SkyString (*onMessageBack)(void *userData, const char *msg) = nullptr;
    void (*onCreateEffect)(void *userData, int contextID, int effectID) = nullptr;
    void (*onDestory)(void *userData) = nullptr;
};

class SkyEffect;
class SKYMEDIA_API SkyOFWrapper final {
public:
    SkyOFWrapper *operator=(const SkyOFWrapper &rhs) = delete;
    SkyOFWrapper(const SkyOFWrapper &rhs) = delete;

    SkyEffect *getOwner() const;

    void setMessageCallback(SkyOFMessageCallback callback);
    void sendMessage(const char *msg);
    void sendTimedMessage(double t, const char *msg);

    void removeAllMessages();

    void setFaceFrameDataArray(const SkyVariant &data);
    void setAvatarInfo(const SkyVariant &info);

    void setNeedsUpdateDuration(bool needsUpdate);

    void setOFParam(const char *key, SkyVariant value);
    SkyVariant getOFParam(const char *key) const;

private:
    SkyOFWrapper();
    ~SkyOFWrapper();
};

SKYMEDIA_NAMESPACE_END
