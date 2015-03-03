 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "portmacro.h"

#include "Audio.h"
#include "TestTask.h"
#include "MidiTask.h"

#include "synthesizer.h"
#include "midi.h"
#include "synthesizer/Synthesizer.h"

using namespace Synthia;

static void SystemClock_Config(void);
void initHardware(void);
void initUART(void);
void initUSB(void);
void initTasks(void);

void initSynth();
void audioCallback(uint16_t *buf , uint16_t length);
uint16_t audiobuff[BUFF_LEN];

TestTask testTask;
MidiTask midiTask;

Synthesizer synth(AUDIO_SAMPLE_RATE);

/* Variables used for USB */
USBD_HandleTypeDef  hUSBDDevice;
UART_HandleTypeDef UartHandle;

// MIDI message queue
QueueHandle_t midiQueueHandle;
QueueHandle_t synthMidiQueueHandle;

xTaskHandle xIdleHandle = NULL;

TIM_HandleTypeDef htim10;

GPIO_InitTypeDef gpioPerf;


extern "C" void vLEDFlashTask(void *pvParameters)
{
    portTickType xLastWakeTime;
    const portTickType xFrequency = 1000;
    xLastWakeTime=xTaskGetTickCount();
    for( ;; )
    {
        BSP_LED_Toggle(LED3);
        vTaskDelayUntil(&xLastWakeTime,xFrequency);
    }
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    initHardware();
    initTasks();

    printf("Starting OS Kernel \r\n");
    osKernelStart();
    
    while (1)
    {
        HAL_Delay(500);
    }
}

void initHardware(void)
{
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    BSP_LED_Init(LED5);
    BSP_LED_Init(LED6);

    initUART();
    initUSB();
    BSP_ACCELERO_Init();

    initAudio();
    
    htim10.Instance = TIM10;
    htim10.Init.Prescaler = 83;
    htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim10.Init.Period = 999;
    htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim10);
    
    //__HAL_TIM_ENABLE(htim10);
    HAL_TIM_Base_Start(&htim10);
    
    gpioPerf.Pin = GPIO_PIN_15;
    gpioPerf.Mode = GPIO_MODE_OUTPUT_PP;
    gpioPerf.Speed = GPIO_SPEED_HIGH;
    gpioPerf.Pull = GPIO_NOPULL;
    
    HAL_GPIO_Init(GPIOE, &gpioPerf);
}

void initTasks(void)
{
    midiQueueHandle = xQueueCreate(32, sizeof(USB_MIDI_Event_Packet_TypeDef));
    synthMidiQueueHandle = xQueueCreate(32, sizeof(MidiMessage_t));
    
    printf("Starting tasks\r\n");
    testTask.start(osPriorityIdle);
    xIdleHandle = (xTaskHandle)testTask.getTaskHandle();
    
    //xTaskCreate(vLEDFlashTask, "Blink", 255, NULL, tskIDLE_PRIORITY, NULL);
    
    // Midi Task
    midiTask.start(osPriorityIdle);
}

void initUSB(void)
{
    USBD_Init(&hUSBDDevice, &MIDI_Desc, 0);
    USBD_RegisterClass(&hUSBDDevice, USBD_MIDI_CLASS);
    USBD_Start(&hUSBDDevice);
}

void initUART(void)
{
    UartHandle.Instance          = USARTx;
    UartHandle.Init.BaudRate     = 115200;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }
}

void initAudio(void)
{
    BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, AUDIO_SAMPLE_RATE);
    BSP_AUDIO_OUT_SetVolume(60);
    BSP_AUDIO_OUT_Play((uint16_t*)&audiobuff[0], 2*BUFF_LEN);
}

void setHeadphoneVolume(int vol)
{
    BSP_AUDIO_OUT_SetVolume(vol);
}

void make_sound(uint16_t *buf , uint16_t length)
{
    MidiMessage_t msg;
    BaseType_t xTaskWokenByReceive = pdFALSE;
    
    while(xQueueReceiveFromISR(synthMidiQueueHandle, &msg, &xTaskWokenByReceive)) {
        synth.processMidiMessage(&msg);
    }
    
    uint16_t 	pos;
    uint16_t 	*outp;
    float	 	yL, yR ;
    uint16_t 	valueL, valueR;

    outp = buf;

    for (pos = 0; pos < length; pos++)
    {
        float samp = synth.tick();
        yL = samp;
        yR = samp;

        valueL = (uint16_t)((int16_t)((32767.0f) * yL)); // conversion float -> int
        valueR = (uint16_t)((int16_t)((32767.0f) * yR));

        *outp++ = valueL; // left channel sample
        *outp++ = valueR; // right channel sample
    }

}

extern "C" void USBD_MIDI_GotMessageCallback(USB_MIDI_Event_Packet_TypeDef *packets, uint32_t len)
{
    uint32_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    for(i = 0; i < len; i++) {
        // Put the MIDI messages on the queue to be picked up by the MIDI
        // task.
        //printf("Got message from USB\r\n");
        xQueueSendToBackFromISR(midiQueueHandle, (const void*)&packets[i], &xHigherPriorityTaskWoken);
    }
    
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR(1);
    }
}

uint32_t lastPeriod = 0;
uint32_t ticksThisSecond = 0;
uint32_t genTime = 0;

extern "C" void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    //HAL_GPIO_WritePin(GPIOE, 15, GPIO_PIN_SET);
    
    BSP_LED_On(LED4);
    make_sound((uint16_t *)(audiobuff + BUFF_LEN_DIV2), BUFF_LEN_DIV4);
    BSP_LED_Off(LED4);
    
    //HAL_GPIO_WritePin(GPIOE, 15, GPIO_PIN_RESET);
}

extern "C" void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    BSP_LED_On(LED4);
    make_sound((uint16_t *)audiobuff, BUFF_LEN_DIV4);
    BSP_LED_Off(LED4);
}

extern "C" void BSP_AUDIO_OUT_Error_Callback(void)
{
    //while(1) {
    BSP_LED_On(LED6);
    //}
}

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
        /* Enable the Flash prefetch */
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    /* Turn LED3 on: Transfer error in reception/transmission process */
    BSP_LED_On(LED3);
}

void Error_Handler(void)
{
    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while(1)
    {
    }
}



extern "C" void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
    ( void ) pxTask;
    ( void ) pcTaskName;
    
    BSP_LED_On(LED6);
    for( ;; );
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif
