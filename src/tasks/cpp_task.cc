#include "cpp_task.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Task::Task() : _taskHandle(NULL)
{
}

Task::~Task()
{
}

void Task::start(osPriority priority)
{
    // Copy name into a temporary var on the heap so that FreeRTOS can
    // access it.
    std::string taskName = getName();
    char *name = new char[taskName.length()];
    strcpy(name, taskName.c_str());
    
    xTaskCreate(_taskRunner, name, configMINIMAL_STACK_SIZE, static_cast<void *>(this), tskIDLE_PRIORITY + 1, &_taskHandle);
    
    delete[] name;
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

    printf("Task %s failed!\r\n", getName().c_str());

}

TaskHandle_t Task::getTaskHandle()
{
    return _taskHandle;
}
