#pragma once

#include "Channel.h"
#include <QThread>

class ThreadWroker : public QThread {
public:
    ThreadWroker(const ThreadWroker &) = delete;
    ThreadWroker &operator=(const ThreadWroker &) = delete;

    ThreadWroker(int taskCapacity = -1) : m_tasks(taskCapacity, false) {
    }

    ~ThreadWroker() {
        m_tasks.close();
        this->wait();
    }

    template <typename F>
    void async(F &&f) {
        m_tasks.send(std::forward<F>(f));
    }

    template <class F>
    auto sync(F &&f) -> typename std::result_of<F()>::type {
        using return_type = typename std::result_of<F()>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
        std::future<return_type> res = task->get_future();
        m_tasks.send([task]() {
            (*task)();
        });
        return res.get();
    }

    template <typename F>
    void asyncOnce(F &&f) {
        m_tasks.send(std::forward<F>(f));
        m_tasks.close();
    }

    virtual void run() override {
        for (;;) {
            std::function<void()> task;
            bool isClosed = false;
            m_tasks.receive(task, isClosed);
            if (isClosed) {
                break;
            }
            task();
        }
    }

    bool isEmpty() const {
        return m_tasks.size() == 0;
    }

private:
    skymedia::Channel<std::function<void()>> m_tasks;
};
