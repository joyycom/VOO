#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace skymedia {
template <typename T>
class Channel final {
public:
    Channel(const Channel &) = delete;
    Channel &operator=(const Channel &) = delete;

    Channel(int capacity, bool multithreadingReceive = true) {
        m_capacity = capacity;
        m_close = false;
        m_multithreadingReceive = multithreadingReceive;
    }

    ~Channel() {
    }

    template <typename F>
    bool send(F &&data) {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        bool need_notify_one = false;
        {
            if (m_capacity > 0) {
                m_sendCondition.wait(lock, [&] {
                    return m_close || m_buffer.size() <= m_capacity;
                });
            }
            if (m_close) {
                return false;
            }

            // m_multithreadingReceive 用于表示是否有多个线程会调用 receive
            // 分析代码知道，当在 !m_multithreadingReceive && !m_buffer.empty() 时，receive 函数中肯定没有在 wait。
            // 此时，并不需要发送 notify_one 唤醒。
            // 上述条件的反面，才需要发送 notify_one 唤醒通知。
            need_notify_one = m_multithreadingReceive || m_buffer.empty();
            m_buffer.emplace(std::forward<F>(data));
        }
        if (need_notify_one) {
            m_receiveCondition.notify_one();
        }
        return true;
    }

    bool isClosed() {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        return m_close;
    }

    void close() {
        {
            std::unique_lock<std::mutex> lock(m_bufferMutex);
            m_close = true;
        }
        // don't use notify_one
        m_receiveCondition.notify_all();
        m_sendCondition.notify_all();
    }

    void receive(T &data, bool &isClosed) {
        {
            std::unique_lock<std::mutex> lock(m_bufferMutex);
            m_receiveCondition.wait(lock, [this] {
                return m_close || !m_buffer.empty();
            });
            if (m_close && m_buffer.empty()) {
                isClosed = true;
                return;
            }
            isClosed = false;
            data = std::move(m_buffer.front());
            m_buffer.pop();
        }

        if (m_capacity > 0) {
            m_sendCondition.notify_one();
        }
    }

    size_t size() const {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        return m_buffer.size();
    }

private:
    std::queue<T> m_buffer;
    // synchronization
    mutable std::mutex m_bufferMutex;
    std::condition_variable m_receiveCondition;
    std::condition_variable m_sendCondition;
    int m_capacity;
    bool m_close;
    bool m_multithreadingReceive;
};

}; // namespace skymedia
