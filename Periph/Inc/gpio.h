/**
  ******************************************************************************
  * @file           : gpio.h
  * @brief          : This file contains header definitions for the code that
  *                   uses standard GPIO pin functions.
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


#ifndef __GPIO_INIT_H
#define __GPIO_INIT_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/




/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initialized GPIO pins for the LED indication.
 * @param   none
 * @retval  status of operation
 */
ErrorStatus GPIO_LED_Init(void);

/**
 * @brief   Initialized GPIO pins for TM163x devices.
 * @param   none
 * @return  status of operation
 */
ErrorStatus GPIO_TM163x_Init(void);


#ifdef __cplusplus
}
#endif
#endif /* __GPIO_INIT_H */

