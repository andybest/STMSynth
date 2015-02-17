/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Src/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    26-December-2014
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


static void SystemClock_Config(void);
void make_sound(uint16_t *buf , uint16_t length);

uint16_t 			audiobuff[BUFF_LEN];

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
     - Configure the Flash prefetch, instruction and Data caches
     - Configure the Systick to generate an interrupt each 1 msec
     - Set NVIC Group Priority to 4
     - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  /* System interrupt init*/
  /* Sets the priority grouping field */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  
  /* Turn ON LED4: start of application */
  BSP_LED_On(LED3);
  //BSP_LED_On(LED4);
  //BSP_LED_On(LED5);
  //BSP_LED_On(LED6);
  
  /* Configure USER Button */
  //BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, 48000);
  BSP_AUDIO_OUT_SetVolume(40);
  
  BSP_AUDIO_OUT_Play((uint16_t*)&audiobuff[0], 2*BUFF_LEN);
  
  
  while (1)
  {
      //printf("Led Run! %u\r\n", HAL_GetTick());
      BSP_LED_Toggle(LED3);
      HAL_Delay(200);
      //BSP_LED_Toggle(LED5);
      //HAL_Delay(200);
      //BSP_LED_Toggle(LED6);
      //HAL_Delay(200);
      //BSP_LED_Toggle(LED4);
      //HAL_Delay(200);
  }
}

void make_sound(uint16_t *buf , uint16_t length) // To be used with the Sequencer
{

	uint16_t 	pos;
	uint16_t 	*outp;
	float	 	yL, yR ;
	uint16_t 	valueL, valueR;

	outp = buf;
    
    int accum = 0;
    int phase = 0;
    
	for (pos = 0; pos < length; pos++)
	{
        if(accum > 50) {
            accum = 0;
            if(phase)
                phase = 0;
            else
                phase = 1;
        }
        accum += 1;
        
        if(phase) {
            yL = 1.0f;
            yR = 1.0f;
        } else {
            yL = -1.0f;
            yR = -1.0f;
        }

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
	//BSP_LED_On(LED4);
    //BSP_AUDIO_OUT_Play((uint16_t*)&audiobuff[0], 2*BUFF_LEN);
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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/