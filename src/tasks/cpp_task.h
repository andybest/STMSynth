#pragma once

#include "cmsis_os.h"

class Task
{
public:
    Task(const char *name);
    ~Task();

    void start(osPriority priority);
    char *getName();
    static void _taskRunner(void *args);
    void _taskHandler();

    virtual void init() = 0;
    virtual bool run() = 0;
    
    TaskHandle_t _taskHandle;

    char *_taskName;
    
};


