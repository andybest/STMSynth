#pragma once

#include "cpp_task.h"

class TestTask : public Task
{
public:
    TestTask(const char *name);
    ~TestTask();

    void init();
    bool run();
};