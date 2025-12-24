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






////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus TM1803_Init(StripDevice_TypeDev* dev) {
  
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  /* Init GPIO */
  if (dev->PinData > 7) {
    MODIFY_REG(
      dev->PortData->CRH, 
      (0x0f << ((dev->PinData - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << ((dev->PinData - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->PortData->CRL, 
      (0x0f << (dev->PinData * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << (dev->PinData * 4))
    );
  }


  dev->Lock = DISABLE;
  return (SUCCESS);
}




/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
