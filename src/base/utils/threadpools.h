#ifndef THREADPOOLS_H
#define THREADPOOLS_H

#include <QObject>
#include <QRunnable>
#include <functional>

class ThreadPools {
public:
    static ThreadPools *instance();

    void exec(QRunnable *task);
    void exec(std::function<void()> task);

    void initMainThread();

    void mainThread(QRunnable *task, int delay = 0);
    void mainThread(std::function<void()> task, int delay = 0);

    bool isMainThread();

private:
    ThreadPools();

private:
    static ThreadPools *s_instance;

    QThread *m_mainThread;
};

#endif // THREADPOOLS_H
