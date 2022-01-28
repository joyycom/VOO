#pragma once

#include <cstdint>
#include <map>
#include <mutex>

class HandleMap {
public:
    int64_t allocHandle(void *value);
    void freeHandle(void *value);

    int64_t getHandle(void *value) const;
    void *lockHandle(int64_t handle) const;

    static HandleMap &getInstance() {
        static HandleMap s_handleMap;
        return s_handleMap;
    }

private:
    HandleMap() = default;

private:
    int64_t m_nextHandle = 0;
    mutable std::mutex m_mutex;
    std::map<void *, int64_t> m_map0;
    std::map<int64_t, void *> m_map1;
};

template <typename DerivedT>
class HandleMapItem {
public:
    HandleMapItem() {
        // C++ 多重继承下，指针值可能发生偏移，这里不能直接使用 this
        DerivedT *value = static_cast<DerivedT *>(this);
        HandleMap::getInstance().allocHandle(value);
    }

    ~HandleMapItem() {
        DerivedT *value = static_cast<DerivedT *>(this);
        HandleMap::getInstance().freeHandle(value);
    }
};
