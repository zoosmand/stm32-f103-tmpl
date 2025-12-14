/**
  ******************************************************************************
  * @file           : hchk.h
  * @brief          : Header for hchk.c file.
  *                   This file contains the common defines for health check and
  *                   monitoring code.
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
#ifndef __HCHK_H
#define __HCHK_H

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
void Heartbeat_CronHandler(void);

/**
 * @brief   Returns the heartbeat device struct pointer.
 * @return  the heartbeat devices struct pointer
 */
HearbeatDevice_TypeDev* Get_HeartbeatDevice(void);


#ifdef __cplusplus
}
#endif

#endif /* __HCHK_H */