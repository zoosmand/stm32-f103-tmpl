/**
  ******************************************************************************
  * @file           : monitor.h
  * @brief          : This file contains header definitions for the file
  *                   monitor.c that realises the monitoring procedures.
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

#ifndef __MONITOR_H
#define __MONITOR_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initialized Heartbeat monitoring.
 * @param   none
 * @retval  status of operation
 */
ErrorStatus Heartbeat_Init(HearbeatDevice_TypeDev*);



#ifdef __cplusplus
}
#endif
#endif /* __MONITOR_H */
