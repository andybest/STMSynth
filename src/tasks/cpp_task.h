#pragma once

#include "cmsis_os.h"
#include <string>

class Task
{
public:
    Task();
    ~Task();

    void start(osPriority priority);
    static void _taskRunner(void *args);
    void _taskHandler();
    
    virtual std::string getName() = 0;
    virtual void init() = 0;
    virtual bool run() = 0;
    
    TaskHandle_t _taskHandle;    
};


