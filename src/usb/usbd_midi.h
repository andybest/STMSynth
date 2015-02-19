/**
  ******************************************************************************
  * @file    usbd_template_core.h
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    04-November-2014
  * @brief   Header file for the usbd_template_core.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/ 
#ifndef __USB_MIDI_CORE_H
#define __USB_MIDI_CORE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_MIDI
  * @brief This file is the header file for usbd_midi.c
  * @{
  */ 


/** @defgroup USBD_MIDI_Exported_Defines
  * @{
  */ 
#define MIDI_EPIN_ADDR                 0x81
#define MIDI_EPIN_SIZE                 0x10

#define USB_MIDI_CONFIG_DESC_SIZ       101

// Descriptor types
#define MS_DESCRIPTOR_UNDEFINED_DESCRIPTOR_TYPE     0x00
#define MS_HEADER_DESCRIPTOR_TYPE                   0x01
#define MIDI_IN_JACK_DESCRIPTOR_TYPE                0x02
#define MIDI_OUT_JACK_DESCRIPTOR_TYPE               0x03
#define MIDI_ELEMENT_DESCRIPTOR_TYPE                0x04
   
// Endpoint descriptor subtypes  
#define MS_DESCRIPTOR_UNDEFINED_DESCRIPTOR_SUBTYPE  0x00
#define MS_GENERAL_DESCRIPTOR_SUBTYPE               0x01
     
// MIDI in and out jack subtypes
#define MIDI_JACK_TYPE_UNDEFINED                    0x00
#define MIDI_JACK_TYPE_EMBEDDED                     0x01
#define MIDI_JACK_TYPE_EXTERNAL                     0x02

     // Control selectors
#define MIDI_EP_CONTROL_UNDEFINED                   0x00
#define MIDI_ASSOCIATION_CONTROL                    0x01

/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_ClassTypeDef  USBD_MIDI_ClassDriver;
#define USBD_MIDI_CLASS &USBD_MIDI_ClassDriver
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif  /* __USB_MIDI_CORE_H */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
