#ifndef STDTHREADWORK_H
#define STDTHREADWORK_H
#include "Channel.h"
#include <thread>

template <int numberOfThreads>
class ThreadPool final {
public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

    ThreadPool(int taskCapacity) : m_tasks(taskCapacity, numberOfThreads > 1) {
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

    ~ThreadPool() {
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
    Channel<std::function<void()>> m_tasks;
};

#endif // STDTHREADWORK_H
