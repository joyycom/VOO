#include "MemoryPool.h"
#include <QDebug>
#include <mutex>

MemoryPool::MemoryPool() {
}

MemoryPool::~MemoryPool() {
    clear();
}

MemoryPool *MemoryPool::instance() {
    static MemoryPool s_instance;
    return &s_instance;
}

unsigned char *MemoryPool::obtain(int size) {
    std::lock_guard<QMutex> locker(m_mutex);
    QList<unsigned char *> *list = m_caches[size];
    if (list == nullptr) {
        list = new QList<unsigned char *>();
        m_caches.insert(size, list);
    }
    unsigned char *p = nullptr;
    if (list->size() > 0) {
        p = list->first();
        list->removeFirst();
        m_cached_memory -= size;
    }
    if (p == nullptr) {
        p = (unsigned char *)malloc(size);
        qDebug() << "MemoryPool obtain malloc memory!! " << size << " P:" << p;
        if (p == nullptr) {
            qWarning() << "MemoryPool obtain Failed！ clear memory!! " << m_cached_memory;
            clear();
            p = (unsigned char *)malloc(size);
        }
        if (p == nullptr) {
            qWarning() << "MemoryPool obtain Failed！ malloc retry Failed!! " << size;
        } else {
            m_total_malloc_memory += size;
        }
    }
    memset(p, 0, size);
    return p;
}

void MemoryPool::destroy(int size, unsigned char *p) {
    if (p == nullptr) {
        return;
    }
    std::lock_guard<QMutex> locker(m_mutex);
    qDebug() << "MemoryPool destroy memory!! " << size << " P:" << p;

    QList<unsigned char *> *list = m_caches[size];
    if (list == nullptr) {
        list = new QList<unsigned char *>();
        m_caches.insert(size, list);
    }
    if (list->size() >= 10 || size <= 0) {
        free(p);
    } else {
        m_cached_memory += size;
        list->append(p);
    }
}

void MemoryPool::clear() {
    std::lock_guard<QMutex> locker(m_mutex);
    int freeSize = 0;
    for (auto key : m_caches.keys()) {
        QList<unsigned char *> *list = m_caches[key];
        if (list != nullptr) {
            for (int i = 0; i < list->size(); ++i) {
                free(list->at(i));
                freeSize += key;
            }
            list->clear();
            delete list;
        }
    }
    qDebug() << "Memory Pool clear Free Size:" << freeSize;
    m_caches.clear();
}
