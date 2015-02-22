#include "cpp_task.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Task::Task(const char *name) : _taskHandle(NULL)
{
    _taskName = (char *)malloc(sizeof(strlen(name)) + 1);
    _taskName = strcpy(_taskName, name);
}

Task::~Task()
{
    if(_taskName != NULL){
        free(_taskName);
    }
}

void Task::start(osPriority priority)
{
    xTaskCreate(_taskRunner, "Test", configMINIMAL_STACK_SIZE, static_cast<void *>(this), tskIDLE_PRIORITY, &_taskHandle);
}

void Task::_taskRunner(void *args)
{
    Task *t = static_cast<Task *>(args);
    t->_taskHandler();
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
