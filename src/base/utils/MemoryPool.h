#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <QMap>
#include <QMutex>

class MemoryPool {
public:
    static MemoryPool *instance();

    unsigned char *obtain(int size);

    void destroy(int size, unsigned char *p);

    void clear();

private:
    MemoryPool();
    ~MemoryPool();

    QMap<int, QList<unsigned char *> *> m_caches;

    QMutex m_mutex;
    int m_total_malloc_memory = 0;
    int m_cached_memory = 0;
};

#endif // MEMORYPOOL_H
