#include "TestTask.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "stm32f4_discovery.h"

TestTask::TestTask(const char *name) : Task(name)
{

}

TestTask::~TestTask()
{

}

void TestTask::init()
{
    BSP_LED_On(LED3);
    printf("Thread '%s' started.\r\n", getName());
}

bool TestTask::run()
{
    BSP_LED_Toggle(LED4);
    printf("Thread '%s' running. %lu", getName(), osKernelSysTick());
    osDelay(1000);
    return true;
}