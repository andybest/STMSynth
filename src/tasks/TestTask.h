#pragma once

#include "cpp_task.h"

class TestTask : public Task
{
public:
    TestTask();
    ~TestTask();
    
    std::string getName();
    void init();
    bool run();
};