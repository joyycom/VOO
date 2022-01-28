#ifndef STDTHREADPOOLS_H
#define STDTHREADPOOLS_H

#include "Channel.h"
#include <thread>
#include <functional>

template <int numberOfThreads>
class StdThreadPools final {
public:
    StdThreadPools(const StdThreadPools &) = delete;
    StdThreadPools &operator=(const StdThreadPools &) = delete;

    StdThreadPools(int taskCapacity) : m_tasks(taskCapacity, numberOfThreads > 1) {
        for (int i = 0; i < numberOfThreads; i++) {
            m_threads[i] = std::thread([this] {
                for (;;) {
                    std::function<void()> task;
                    bool isClosed = false;
                    m_tasks.receive(task, isClosed);
                    if (isClosed) {
                        break;
                    }
                    task();
                }
            });
        }
    }

    ~StdThreadPools() {
        closeThreads();
    }

    template <typename F>
    void async(F &&f) {
        m_tasks.send(std::forward<F>(f));
    }

    void closeThreads() {
        if (!m_tasks.isClosed()) {
            m_tasks.close();
            for (auto &thread : m_threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }
    }

private:
    std::thread m_threads[numberOfThreads];
    skymedia::Channel<std::function<void()>> m_tasks;
};

#endif // STDTHREADPOOLS_H
