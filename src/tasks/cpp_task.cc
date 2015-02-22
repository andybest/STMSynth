#include "cpp_task.h"

#include <stdio.h>
#include <string.h>

Task::Task(const char *name)
{
    _taskName = strcpy(_taskName, name);
}

Task::~Task()
{

}

void Task::start(osPriority priority)
{
    osThreadDef_t threadDef;

    threadDef.name = _taskName;
    threadDef.pthread = (os_pthread)&_taskRunner;
    threadDef.tpriority = priority;
    threadDef.instances = 1;
    threadDef.stacksize = 0;

    _taskArgs.task = this;
    osThreadCreate(&threadDef, (void *)&_taskArgs);
}

void Task::_taskRunner(const void *args)
{
    const TaskArgs_t *tArgs = (const TaskArgs_t*)args;
    tArgs->task->_taskHandler();
}

void Task::_taskHandler()
{
    init();
    while(run()) {
    }

    printf("Task %s failed!\r\n", getName());

}

char *Task::getName()
{
    return _taskName;
}