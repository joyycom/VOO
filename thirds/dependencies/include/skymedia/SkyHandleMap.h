#pragma once

#include "SkyDefs.h"

SKYMEDIA_NAMESPACE_BEGIN

class SKYMEDIA_API SkyHandleMap final {
public:
    static int64_t allocHandle(void *value);
    static void freeHandle(void *value);

    static int64_t getHandle(void *value);
    static void *lockHandle(int64_t handle);
};

SKYMEDIA_NAMESPACE_END
