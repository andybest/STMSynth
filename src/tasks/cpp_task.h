#pragma once

#include "cmsis_os.h"
#include "FreeRTOS.h"

class Task;

typedef struct
{
    Task *task;
} TaskArgs_t;

class Task
{
public:
    Task(const char *name);
    ~Task();

    void start(osPriority priority);

    virtual void init() = 0;
    virtual bool run() = 0;

    char *getName();

    protected:
    static void _taskRunner(const void *args);
    void _taskHandler();
    osThreadDef_t threadDef;


private:
    char *_taskName;
    TaskArgs_t _taskArgs;
    xTaskHandle _taskHandle;
};


