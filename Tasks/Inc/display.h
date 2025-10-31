/**
  ******************************************************************************
  * @file           : display.h
  * @brief          : This file contains header definitions for display devices
  *                   code in located in display.c file.
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
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private structures -------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

void DisplayHealthCheck_CronHandler(void);

/**
 * @brief Return MAX display device struct pointer.
 * @return device struct pointer
 */
Max72xx_TypeDef* Get_MaxDiplayDevice(void);

/**
 * @brief Return TM display device struct pointer.
 * @return device struct pointer
 */
TM163x_TypeDef* Get_TmDiplayDevice(void);


#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_H */