#include "TestTask.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim10;

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
    printf("Thread '%s' started.\r\n", getName().c_str());
}

extern uint32_t genTime;

bool TestTask::run()
{
    //printf("Time taken: %lu\r\n", genTime);
    HAL_GPIO_WritePin(GPIOE, 15, GPIO_PIN_SET);
    osDelay(1000);
    HAL_GPIO_WritePin(GPIOE, 15, GPIO_PIN_RESET);
    return true;
}
