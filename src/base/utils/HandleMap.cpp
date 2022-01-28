#include "HandleMap.h"
#include <cassert>

int64_t HandleMap::allocHandle(void *value) {
    assert(value != nullptr);

    std::lock_guard<std::mutex> locker(m_mutex);
    m_nextHandle++;
    int64_t handle = m_nextHandle;
    assert(handle != 0);
    m_map0[value] = handle;
    m_map1[handle] = value;
    return handle;
}

void HandleMap::freeHandle(void *value) {
    assert(value != nullptr);

    std::lock_guard<std::mutex> locker(m_mutex);
    auto iter = m_map0.find(value);
    assert(iter != m_map0.end());
    if (iter != m_map0.end()) {
        int64_t handle = iter->second;
        m_map0.erase(iter);
        m_map1.erase(m_map1.find(handle));
    }
}

int64_t HandleMap::getHandle(void *value) const {
    if (value == nullptr) {
        return 0;
    }

    std::lock_guard<std::mutex> locker(m_mutex);
    auto iter = m_map0.find(value);
    assert(iter != m_map0.end());
    if (iter != m_map0.end()) {
        return iter->second;
    }
    return 0;
}

void *HandleMap::lockHandle(int64_t handle) const {
    if (handle == 0) {
        return nullptr;
    }

    std::lock_guard<std::mutex> locker(m_mutex);
    auto iter = m_map1.find(handle);
    //    assert(iter != m_map1.end());
    if (iter != m_map1.end()) {
        return iter->second;
    }
    return nullptr;
}
