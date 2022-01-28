#include "threadpools.h"

#include <QDebug>
#include <QThreadPool>
#include <QTimer>

#include "TaskWrapper.h"

ThreadPools *ThreadPools::s_instance = nullptr;

ThreadPools::ThreadPools() {
}

void ThreadPools::initMainThread() {
    m_mainThread = QThread::currentThread();
}

void ThreadPools::exec(QRunnable *task) {
    QThreadPool::globalInstance()->start(task);
}

void ThreadPools::exec(std::function<void()> task) {
    QThreadPool::globalInstance()->start(new RunnableFuncWrapper(task));
}

void ThreadPools::mainThread(QRunnable *task, int delay) {
    exec([this, task, delay] {
        TaskWrapper *mainTask = new TaskWrapper(task);
        mainTask->moveToThread(m_mainThread);
        QTimer::singleShot(delay, mainTask, &TaskWrapper::run);
        QObject::connect(mainTask, &TaskWrapper::finish, [=, mainTask = mainTask] {
            delete mainTask;
        });
    });
}

void ThreadPools::mainThread(std::function<void()> task, int delay) {
    exec([this, task, delay] {
        TaskWrapper *mainTask = new TaskWrapper(task);
        mainTask->moveToThread(m_mainThread);
        QTimer::singleShot(delay, mainTask, &TaskWrapper::run);
        QObject::connect(mainTask, &TaskWrapper::finish, [=, mainTask = mainTask] {
            delete mainTask;
        });
    });
}

bool ThreadPools::isMainThread() {
    return QThread::currentThread() == m_mainThread;
}

ThreadPools *ThreadPools::instance() {
    if (s_instance == nullptr) {
        s_instance = new ThreadPools();
    }
    return s_instance;
}
