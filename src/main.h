
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio.h"
#include "stm32f4_discovery_accelerometer.h"
#include <stdio.h>
#include "stm32f4xx_it.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_midi.h"
#include "usbd_hid.h"

typedef enum
{
  APPLICATION_IDLE = 0,  
  APPLICATION_START,    
  APPLICATION_RUNNING,
}
MSC_ApplicationTypeDef;

/* Defines for LEDs lighting */
#define LED3_TOGGLE      0x03  /* Toggle LED3 */
#define LED4_TOGGLE      0x04  /* Toggle LED4 */
#define LED6_TOGGLE      0x06  /* Toggle LED6 */
#define LEDS_OFF         0x07  /* Turn OFF all LEDs */
#define STOP_TOGGLE      0x00  /* Stop LED Toggling */

/* Defines for MEMS Acclerometer ID Types */
#define MEMS_LIS3DSH     0x3F /* LIS3DSH MEMS Acclerometer ID */
#define MEMS_LIS302DL    0x3B /* LIS302DL MEMS Acclerometer ID */
                                                                                    
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);


#define BUFF_LEN_DIV4           512 // number of samples in the audiobuffer for each channel <==> XX ms latency at 48kHz
#define BUFF_LEN_DIV2           (2*BUFF_LEN_DIV4)
#define BUFF_LEN                (4*BUFF_LEN_DIV4)  // Audio buffer length :


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
