#include "TaskWrapper.h"

TaskWrapper::TaskWrapper(QRunnable *task, QObject *parent)
    : QObject(parent),
      m_task(task){

      };

TaskWrapper::TaskWrapper(std::function<void()> task, QObject *parent) : QObject(parent), m_funTask(task) {
}

TaskWrapper::~TaskWrapper() {
    if (m_task) {
        delete m_task;
        m_task = nullptr;
    }
    if (m_funTask) {
        m_funTask = nullptr;
    }
};

void TaskWrapper::run() {
    if (m_task != nullptr) {
        m_task->run();
    } else if (m_funTask != nullptr) {
        m_funTask();
    }
    emit finish(this);
}

RunnableFuncWrapper::RunnableFuncWrapper(std::function<void()> task) : m_funTask(task) {
}

void RunnableFuncWrapper::run() {
    if (m_funTask != nullptr) {
        m_funTask();
    }
}
