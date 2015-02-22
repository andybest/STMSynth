/**
******************************************************************************
* @file    usbd_midi.c
* @author  MCD Application Team
* @version V2.3.0
* @date    04-November-2014
* @brief   This file provides the HID core functions.
*
* @verbatim
*      
*          ===================================================================      
*                                MIDI Class  Description
*          ===================================================================
*          
*
*
*
*           
*      
* @note     In HS mode and when the DMA is used, all variables and data structures
*           dealing with the DMA during the transaction process should be 32-bit aligned.
*           
*      
*  @endverbatim
*
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

#include "stm32f4_discovery.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
* @{
*/


/** @defgroup USBD_MIDI 
* @brief usbd core module
* @{
*/ 

/** @defgroup USBD_MIDI_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBD_MIDI_Private_Defines
* @{
*/ 

/**
* @}
*/ 


/** @defgroup USBD_MIDI_Private_Macros
* @{
*/ 
                                         
/**
* @}
*/ 




/** @defgroup USBD_MIDI_Private_FunctionPrototypes
* @{
*/


static uint8_t  USBD_MIDI_Init (USBD_HandleTypeDef *pdev, 
uint8_t cfgidx);

static uint8_t  USBD_MIDI_DeInit (USBD_HandleTypeDef *pdev, 
uint8_t cfgidx);

static uint8_t  USBD_MIDI_Setup (USBD_HandleTypeDef *pdev, 
USBD_SetupReqTypedef *req);

static uint8_t  *USBD_MIDI_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_MIDI_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_MIDI_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_MIDI_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_MIDI_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_MIDI_EP0_TxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_MIDI_SOF (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_MIDI_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_MIDI_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

/**
* @}
*/ 

/** @defgroup USBD_MIDI_Private_Variables
* @{
*/ 

USBD_ClassTypeDef  USBD_MIDI_ClassDriver = 
{
    USBD_MIDI_Init,
    USBD_MIDI_DeInit,
    USBD_MIDI_Setup,
    USBD_MIDI_EP0_TxReady,  
    USBD_MIDI_EP0_RxReady,
    USBD_MIDI_DataIn,
    USBD_MIDI_DataOut,
    USBD_MIDI_SOF,
    USBD_MIDI_IsoINIncomplete,
    USBD_MIDI_IsoOutIncomplete,      
    USBD_MIDI_GetCfgDesc,
    USBD_MIDI_GetCfgDesc, 
    USBD_MIDI_GetCfgDesc,
    USBD_MIDI_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
/* USB MIDI device Configuration Descriptor */
static uint8_t USBD_MIDI_CfgDesc[USB_MIDI_CONFIG_DESC_SIZ] =
{
    // CONFIGURATION DESCRIPTOR (9)
    0x09,                                   // bLength: Configuation Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,// bDescriptorType: Configuration */
    USB_MIDI_CONFIG_DESC_SIZ,               // wTotalLength: Bytes returned 
    0x00,                                   //          Second byte of above
    0x02,                                   // bNumInterfaces: 2 interfaces (Standard AC Int., MIDI Streaming Int.)
    0x01,                                   // bConfigurationValue: Configuration value
    0x00,                                   // iConfiguration: Unused
    0x80,                                   // bmAttributes: bus powered and does not support remote wakeup
    0x32,                                   // MaxPower 100 mA: this current is used for detecting Vbus
    /* 09 */
  
    // AUDIO CONTROL INTERFACE DESCRIPTOR (9)
    0x09,                       // bLength: length of descriptor
    USB_DESC_TYPE_INTERFACE,    // bDescriptorType
    0x00,                       // bInterfaceNumber: index of interface
    0x00,                       // bAlternateSetting: index of this setting
    0x00,                       // bNumEndpoints: 0 endpoints
    0x01,                       // bInterfaceClass: Audio
    0x01,                       // bInterfaceSubclass: Audio Control
    0x00,                       // bInterfaceProtocol: unused
    0x00,                       // iInterface: unused
    
    // STANDARD MIDI STREAMING INTERFACE DESCRIPTOR (9)
    0x09,                       // bLength
    0x04,                       // bDescriptorType
    0x01,                       // bInterfaceNumber: Index of this interface
    0x00,                       // bAlternateSetting: Index of this alt. setting
    0x02,                       // bNumEndpoints: 2 endpoints (bulk in, bulk out)
    0x01,                       // bInterfaceClass: Audio Class
    0x03,                       // bInterfaceSubclass: MIDI Streaming
    0x00,                       // bInterfaceProtocol: Unused
    0x00,                       // iInterface: Unused
    
    // CLASS SPECIFIC AUDIO CONTROL INTERFACE DESCRIPTOR (9)
    0x09,                       // bLength
    0x24,                       // bDescriptorType: CS_INTERFACE
    0x01,                       // bDescriptorSubtype: Header subtype
    0x00,                       // bcdADC: Revision of class spec.- 1.0
    0x01,                       //          second byte of above
    0x09,                       // wTotalLength: Total size of class specific desc.
    0x00,                       //          second byte of above
    0x01,                       // bInCollection: No. of streaming interfaces
    0x01,                       // baInterfaceNr(1): MIDI Streaming interface belongs
                                // to this control interface.
    
    
    // CLASS SPECIFIC MIDI STREAMING INTERFACE DESCRIPTOR (7)
    0x07,                       // bLength
    0x24,                       // bDescriptorType
    0x01,                       // bDescriptorSubType: MS_HEADER
    0x00,                       // BcdADC: Revision of class spec.
    0x10,                       //          second byte of above
    0x41,                       // wTotalLength: Total length of class-specific descriptors
    0x00,                       //          second byte of above
    
    // MIDI IN JACK DESCRIPTOR (6)
    0x06,                           // bLength
    0x24,                           // bDescriptorType
    0x02,                           // bDescriptorSubtype: Midi in jack
    0x01,                           // bJackType: Embedded
    0x01,                           // bJackID: ID of this jack
    0x00,                           // iJack: Unused
    
    // MIDI IN JACK DESCRIPTOR (6)
    0x06,                           // bLength
    0x24,                           // bDescriptorType
    0x02,                           // bDescriptorSubtype: Midi in jack
    0x02,                           // bJackType: External
    0x02,                           // bJackID: ID of this jack
    0x00,                           // iJack: Unused
    
    // MIDI OUT JACK DESCRIPTOR (9)
    0x09,                           // bLength
    0x24,                           // bDescriptorType
    0x03,                           // bDescriptorSubtype: Midi out
    0x01,                           // bJackType: Embedded
    0x03,                           // bJackID: ID of this jack
    0x01,                           // bNrInputPins
    0x02,                           // BaSourceID
    0x01,                           // BaSourcePin
    0x00,                           // iJack: Unused
    
    // MIDI OUT JACK DESCRIPTOR (9)
    0x09,                           // bLength
    0x24,                           // bDescriptorType
    0x03,                           // bDescriptorSubtype: Midi out
    0x02,                           // bJackType: External
    0x04,                           // bJackID: ID of this jack
    0x01,                           // bNrInputPins
    0x01,                           // BaSourceID
    0x01,                           // BaSourcePin
    0x00,                           // iJack: Unused
    
    // STANDARD BULK OUT ENDPOINT DESCRIPTOR (9)
    0x09,                           // bLength
    USB_DESC_TYPE_ENDPOINT,         // bDescriptorType
    0x01,                           // bEndpointAddress
    0x02,                           // bmAttributes: Bulk, not shared
    0x40,                           // wMaxPacketSize: 64 bytes per packet
    0x00,                           //          second byte of above
    0x00,                           // bInterval: ignored
    0x00,                           // bRefresh: unused
    0x00,                           // bSynchAddress: unused
    
    // CLASS SPECIFIC MIDI STREAMING BULK OUT ENDPOINT DESCRIPTOR (5)
    0x05,                           // bLength
    0x25,                           // bDescriptorType
    MS_GENERAL_DESCRIPTOR_SUBTYPE,  // bDescriptorSubtype
    0x01,                           // bNumEmbMIDIJack: Num embedded jacks
    0x01,                           // BaAssocJackID: ID of embedded jack
    
    // STANDARD BULK IN ENDPOINT DESCRIPTOR (9)
    0x09,                           // bLength
    USB_DESC_TYPE_ENDPOINT,         // bDescriptorType
    0x81,                           // bEndpointAddress
    0x02,                           // bmAttributes: Bulk, not shared
    0x40,                           // wMaxPacketSize: 64 bytes per packet
    0x00,                           //          second byte of above
    0x00,                           // bInterval: ignored
    0x00,                           // bRefresh: unused
    0x00,                           // bSynchAddress: unused
    
    // CLASS SPECIFIC MIDI STREAMING BULK IN ENDPOINT DESCRIPTOR (5)
    0x05,                           // bLength
    0x25,                           // bDescriptorType
    MS_GENERAL_DESCRIPTOR_SUBTYPE,  // bDescriptorSubtype
    0x01,                           // bNumEmbMIDIJack: Num embedded jacks
    0x03,                           // BaAssocJackID: ID of embedded jack
 
};
  
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
/* USB Standard Device Descriptor */
static uint8_t USBD_MIDI_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

/**
* @}
*/ 

/** @defgroup USBD_MIDI_Private_Functions
* @{
*/ 

/**
* @brief  USBD_MIDI_Init
*         Initialize the MIDI interface
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/

uint8_t midiOutBuf[64];

static uint8_t  USBD_MIDI_Init (USBD_HandleTypeDef *pdev, 
uint8_t cfgidx)
{
    uint8_t ret = 0;
    printf("USBD_MIDI_Init\r\n");

    // Open in and out endpoints
    USBD_LL_OpenEP(pdev, MIDI_DATA_OUT_EP, USBD_EP_TYPE_BULK, MIDI_DATA_OUT_SIZE);
    USBD_LL_OpenEP(pdev, MIDI_DATA_IN_EP, USBD_EP_TYPE_BULK, MIDI_DATA_IN_SIZE);
    
    /* Prepare Out endpoint to receive 1st packet */ 
    USBD_LL_PrepareReceive(pdev,
                           MIDI_DATA_OUT_EP,
                           &midiOutBuf[0],                        
                           MIDI_DATA_OUT_SIZE);
    return ret;
}

/**
* @brief  USBD_MIDI_Init
*         DeInitialize the MIDI layer
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/
static uint8_t  USBD_MIDI_DeInit (USBD_HandleTypeDef *pdev, 
uint8_t cfgidx)
{
    printf("USBD_MIDI_DeInit\r\n");

    // Close endpoints
    USBD_LL_CloseEP(pdev, MIDI_DATA_OUT_EP);
    USBD_LL_CloseEP(pdev, MIDI_DATA_IN_EP);
    return USBD_OK;
}

/**
* @brief  USBD_MIDI_Setup
*         Handle the MIDI specific requests
* @param  pdev: instance
* @param  req: usb requests
* @retval status
*/
static uint8_t  USBD_MIDI_Setup (USBD_HandleTypeDef *pdev, 
USBD_SetupReqTypedef *req)
{
     printf("USBD_MIDI_Setup\r\n");
    return USBD_OK;
}


/**
* @brief  USBD_MIDI_GetCfgDesc 
*         return configuration descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_MIDI_GetCfgDesc (uint16_t *length)
{
    *length = sizeof (USBD_MIDI_CfgDesc);
    return USBD_MIDI_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_MIDI_DeviceQualifierDescriptor (uint16_t *length)
{
    *length = sizeof (USBD_MIDI_DeviceQualifierDesc);
    return USBD_MIDI_DeviceQualifierDesc;
}


/**
* @brief  USBD_MIDI_DataIn
*         handle data IN Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t  USBD_MIDI_DataIn (USBD_HandleTypeDef *pdev, 
uint8_t epnum)
{
    BSP_LED_Toggle(LED5);

    return USBD_OK;
}

/**
* @brief  USBD_MIDI_EP0_RxReady
*         handle EP0 Rx Ready event
* @param  pdev: device instance
* @retval status
*/
static uint8_t  USBD_MIDI_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
    // Don't need anything here
    return USBD_OK;
}
/**
* @brief  USBD_MIDI_EP0_TxReady
*         handle EP0 TRx Ready event
* @param  pdev: device instance
* @retval status
*/
static uint8_t  USBD_MIDI_EP0_TxReady (USBD_HandleTypeDef *pdev)
{
    return USBD_OK;
}
/**
* @brief  USBD_MIDI_SOF
*         handle SOF event
* @param  pdev: device instance
* @retval status
*/
static uint8_t  USBD_MIDI_SOF (USBD_HandleTypeDef *pdev)
{
    return USBD_OK;
}
/**
* @brief  USBD_MIDI_IsoINIncomplete
*         handle data ISO IN Incomplete event
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t  USBD_MIDI_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    return USBD_OK;
}
/**
* @brief  USBD_MIDI_IsoOutIncomplete
*         handle data ISO OUT Incomplete event
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t  USBD_MIDI_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    return USBD_OK;
}
/**
* @brief  USBD_MIDI_DataOut
*         handle data OUT Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t  USBD_MIDI_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    BSP_LED_Toggle(LED5);
    
    uint32_t dataLength = USBD_LL_GetRxDataSize (pdev, epnum);

    if(dataLength % 4 != 0) {
        printf("MIDI Data is not a multiple of 4 bytes!!!");
    } else {
        USBD_MIDI_GotMessageCallback((USB_MIDI_Event_Packet_TypeDef *)&midiOutBuf[0], dataLength / 4);
    }

    /* Prepare Out endpoint to receive next packet */ 
    USBD_LL_PrepareReceive(pdev,
                           MIDI_DATA_OUT_EP,
                           &midiOutBuf[0],                        
                           MIDI_DATA_OUT_SIZE);
                           
    return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_MIDI_GetDeviceQualifierDesc (uint16_t *length)
{
    *length = sizeof (USBD_MIDI_DeviceQualifierDesc);
    return USBD_MIDI_DeviceQualifierDesc;
}

void USB_MIDI_DecodeMidiMessage(uint8_t *data, uint32_t len)
{


}

/**
* @}
*/ 


/**
* @}
*/ 


/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
