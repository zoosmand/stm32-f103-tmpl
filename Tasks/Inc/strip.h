/**
  ******************************************************************************
  * @file           : strip.h
  * @brief          : Header for strip.c file.
  *                   This file contains the common defines the strip
  *                   application.
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
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STRIP_H
#define __STRIP_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private structures --------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Provides heartbeat CRON handler procedure.
 * @return  none
 */
void Strip_CronHandler(void);

/**
 * @brief   Returns the heartbeat device struct pointer.
 * @return  the heartbeat devices struct pointer
 */
StripDevice_TypeDev* Get_StripDevice(void);


#ifdef __cplusplus
}
#endif

#endif /* __STRIP_H */