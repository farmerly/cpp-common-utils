#ifndef THREAD_TASK_H_H_H
#define THREAD_TASK_H_H_H

#include <functional>

class ThreadTask
{
public:
    ThreadTask();
    ~ThreadTask();

public:
    void runTask();
};

#endif