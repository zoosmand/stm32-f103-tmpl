/**
  ******************************************************************************
  * @file           : monitor.c
  * @brief          : This file contains the code dedicated to the configuration
  *                   of devices of peripherals that take place in the monioring
  *                   procedures.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Askug Ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "monitor.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Local variables -----------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/





////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus Heartbeat_Init(HearbeatDevice_TypeDev* dev) {
  
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);
  
  /* Init GPIO */
  if (dev->Pin > 7) {
    MODIFY_REG(
      dev->Port->CRH, 
      (0x0f << ((dev->Pin - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_2) << ((dev->Pin - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->Port->CRL, 
      (0x0f << ((dev->Pin - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_2) << (dev->Pin * 4))
    );
  }
  
  PIN_H(dev->Port, dev->Pin);
  
  dev->Lock == DISABLE;
  return (SUCCESS);
}




/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
