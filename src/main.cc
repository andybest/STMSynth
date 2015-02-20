 
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "SynthContext.h"
#include "PulseOscillator.h"
#include "Lowpass.h"

using namespace Synthia;

static void SystemClock_Config(void);
void initHardware(void);
void initUART(void);
void initUSB(void);
void initAudio(void);
void make_sound(uint16_t *buf , uint16_t length);

uint16_t audiobuff[BUFF_LEN];

SynthContext synthContext(48000);
PulseOscillator osc1;
PulseOscillator lfo1;
Lowpass filt1;

/* Variables used for USB */
USBD_HandleTypeDef  hUSBDDevice;

UART_HandleTypeDef UartHandle;


/*
 * Implement C++ new/delete operators using the heap
 */
 
void *operator new(size_t size) {
  return malloc(size);
}
 
void *operator new[](size_t size) {
  return malloc(size);
}
 
void operator delete(void *p) {
  free(p);
}
 
void operator delete[](void *p) {
  free(p);
}



int main(void)
{
  HAL_Init();
  
  SystemClock_Config();
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  
  initHardware();
  
  osKernelStart();
  
  osc1.init(&synthContext);
  osc1.setFrequency(440.0f);
  
  lfo1.init(&synthContext);
  lfo1.setFrequency(2.0f);
  
  filt1.init(&synthContext);
  filt1.setResonance(0.8f);
  filt1.setCutoff(22000.0f);
  
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
    UartHandle.Init.BaudRate     = 9600;
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
    BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, 48000);
    BSP_AUDIO_OUT_SetVolume(80);
    BSP_AUDIO_OUT_Play((uint16_t*)&audiobuff[0], 2*BUFF_LEN);
}

void make_sound(uint16_t *buf , uint16_t length)
{
	uint16_t 	pos;
	uint16_t 	*outp;
	float	 	yL, yR ;
	uint16_t 	valueL, valueR;

	outp = buf;
    
	for (pos = 0; pos < length; pos++)
	{
        //float lfoSamp = lfo1.tick(0);
        //osc1.setFrequency(220.0f + (220.0f * lfoSamp));
        
        float samp = filt1.tick(0, osc1.tick(0));
        yL = samp;
        yR = samp;

		valueL = (uint16_t)((int16_t)((32767.0f) * yL)); // conversion float -> int
		valueR = (uint16_t)((int16_t)((32767.0f) * yR));

		*outp++ = valueL; // left channel sample
		*outp++ = valueR; // right channel sample
	}

}


extern "C" void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
	BSP_LED_Off(LED4);
	make_sound((uint16_t *)(audiobuff + BUFF_LEN_DIV2), BUFF_LEN_DIV4);
}

extern "C" void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
	BSP_LED_On(LED4);
	make_sound((uint16_t *)audiobuff, BUFF_LEN_DIV4);
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


#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif
