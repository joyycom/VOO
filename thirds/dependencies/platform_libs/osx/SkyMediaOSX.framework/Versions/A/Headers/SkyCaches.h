#pragma once

#include "SkyDefs.h"
#include "SkyVariant.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyCachesStrategy_None = 0,          // 不缓存
    SkyCachesStrategy_Memory = 1,        // 只缓存在内存（内存不足时，会自动清除）
    SkyCachesStrategy_MemoryAndDisk = 2, // 内存和磁盘（内存不足时，会写到磁盘）
} SkyCachesStrategy;

struct SKYMEDIA_API SkyHash final {
    uint8_t data[16] = {0}; // 初始清 0
};

class SKYMEDIA_API SkyCaches final {
public:
    SkyCaches();
    explicit SkyCaches(const char *cacheDirectory);
    SkyCaches(const SkyCaches &rhs);
    SkyCaches &operator=(const SkyCaches &rhs);
    ~SkyCaches();

    bool isAvailable() const;

    // 内存的最大字节数，默认为 256 MB。超过限制，会从内存中自动清除旧的缓存（有可能会触发写磁盘）
    void setMaxMemoryByteSize(int64_t maxByteSize);
    int64_t getMaxMemoryByteSize() const;

    // 磁盘的最大字节数，默认为 2 GB。超过限制，会从磁盘自动清除旧的缓存。
    void setMaxDiskByteSize(int64_t maxByteSize);
    int64_t getMaxDiskByteSize() const;

    SkyVariant get(const SkyHash &hash) const;
    void put(const SkyHash &hash, SkyVariant value, SkyCachesStrategy strategy) const;

    // 将必要的缓存信息强制写到磁盘，防止软件中途崩溃。丢失磁盘缓存。
    void flushToDisk();

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
