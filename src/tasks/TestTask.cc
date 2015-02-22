#include "TestTask.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "stm32f4_discovery.h"

TestTask::TestTask() : Task()
{

}

TestTask::~TestTask()
{

}

std::string TestTask::getName()
{
    return std::string("Test Task");
}

void TestTask::init()
{
    BSP_LED_On(LED3);
    printf("Thread '%s' started.\r\n", getName().c_str());
}

bool TestTask::run()
{
    BSP_LED_Toggle(LED3);
    //printf("Thread '%s' running. %lu\r\n", getName().c_str(), osKernelSysTick());
    osDelay(1000);
    return true;
}
