#pragma once

#include "SkyDefs.h"

SKYMEDIA_NAMESPACE_BEGIN

class SKYMEDIA_API SkyAsyncTask final {
public:
    SkyAsyncTask();
    SkyAsyncTask(const SkyAsyncTask &rhs);
    SkyAsyncTask &operator=(const SkyAsyncTask &rhs);
    ~SkyAsyncTask();

    void cancel();
    bool isCanceled() const;
    bool isAvailable() const;

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
