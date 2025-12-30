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
#include "mled.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Locaal variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

__STATIC_INLINE void send_color(StripDevice_TypeDev*, uint32_t);

__STATIC_INLINE void send_bit(StripDevice_TypeDev*, uint8_t);





////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus WS281x_Init(StripDevice_TypeDev* dev) {
  
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

__STATIC_INLINE void send_bit(StripDevice_TypeDev* dev, uint8_t bit) {
  STRIP_DATA_High;
  __asm volatile(" \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
  ");
  if (bit) {
    __asm volatile(" \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
    ");
  }
  STRIP_DATA_Low;
  if (!bit) {
    __asm volatile(" \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
    ");
  }
  __asm volatile(" \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
  ");
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void send_color(StripDevice_TypeDev* dev, uint32_t color) {
  
  for (int8_t i = 23; i >= 0; i--) {
    send_bit(dev, (uint8_t)((color >> i) & 1));
  }
}



// ----------------------------------------------------------------------------

ErrorStatus RunStrip(StripDevice_TypeDev* dev) {

  __disable_irq();
  for (uint16_t i = 0; i < dev->Count; i++) {
    send_color(dev, dev->BufPtr[i]);
  }
  _delay_us(500);
  __enable_irq();

  return (SUCCESS);
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
