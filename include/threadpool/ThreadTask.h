#pragma once

#include <functional>

class ThreadTask
{
public:
    ThreadTask(std::function<void(void *)> func, void *args, std::function<void(void *)> release = nullptr);
    virtual ~ThreadTask();

public:
    void runTask();

private:
    std::function<void(void *)> m_callback;
    void                       *m_args;
    std::function<void(void *)> m_release;
};
