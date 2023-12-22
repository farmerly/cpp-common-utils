#include "ThreadTask.h"

ThreadTask::ThreadTask(std::function<void(void *)> func, void *args, std::function<void(void *)> release)
{
    m_callback = func;
    m_args = args;
    m_release = release;
}

ThreadTask::~ThreadTask()
{
    if (m_release) {
        m_release(m_args);
    }
}

void ThreadTask::runTask()
{
    m_callback(m_args);
}