/**
  ******************************************************************************
  * @file           : tm1803.c
  * @brief          : This file contains code of TM1803 3-channel LED
  *                   controller. 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 Askug Ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "tm1803.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Locaal variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

__STATIC_INLINE void send_color(StripDevice_TypeDev*, uint32_t);
__STATIC_INLINE void send_zero(StripDevice_TypeDev*);
__STATIC_INLINE void send_one(StripDevice_TypeDev*);




////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus TM1803_Init(StripDevice_TypeDev* dev) {
  
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);
  
  #ifdef STRIP_DEV
  /* Init GPIO */
  if (dev->PinData > 7) {
    MODIFY_REG(
      dev->PortData->CRH, 
      (0x0f << ((dev->PinData - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_50) << ((dev->PinData - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->PortData->CRL, 
      (0x0f << (dev->PinData * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_50) << (dev->PinData * 4))
    );
  }

  dev->Lock = DISABLE;
  return (SUCCESS);

  #else
    dev->Lock = ENABLE;
    return (ERROR);
  #endif /* ifdef STRIP_DEV */
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void delay_cycles(uint32_t c) {
  while (c--) {
    __asm volatile("nop");
  }
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void send_bit(StripDevice_TypeDev* dev, uint8_t bit) {
  STRIP_DATA_High(dev);
  delay_cycles(26);
  delay_cycles(bit ? 26 : 0);
  STRIP_DATA_Low(dev);
  delay_cycles(bit ? 12 : 38);
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void send_color(StripDevice_TypeDev* dev, uint32_t color) {
  
  for (int8_t i = 23; i >= 0; i--) {
    send_bit(dev, (color & (1UL << i)));
  }
}



// ----------------------------------------------------------------------------

ErrorStatus TM1803_RunStrip(StripDevice_TypeDev* dev) {

  __disable_irq();
  for (uint16_t i = 0; i < dev->Count; i++) {
    send_color(dev, dev->BufPtr[i]);
  }
  _delay_us(500);
  __enable_irq();
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
