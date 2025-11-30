/**
  ******************************************************************************
  * @file           : tmpr.h
  * @brief          : This file contains header definitions for temperature
  *                   atmospheric pressure, and humidity measurements code in
  *                   tmpr.c file.
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
#ifndef __TMPR_H
#define __TMPR_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
// #include "bmx280.h"

/* Private structures -------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Provides Bosch BMx CRON handler procedure.
 * @return  none
 */
void BoschMeasurment_CronHandler(void);

/**
 * @brief   Provides Bosch DS CRON handler procedure.
 * @return  none
 */
void DsMeasurment_CronHandler(void);

/**
 * @brief   Returns BMx Bosch device struct pointer.
 * @return  device struct pointer
 */
BMxX80_TypeDef* Get_BoschDevice(uint8_t);

/**
 * @brief   Returns OneWire bus struct pointer.
 * @return  the set of OneWire devices struct pointer
 */
OneWireBus_TypeDef* Get_OneWireBusDevice(void);


#ifdef __cplusplus
}
#endif

#endif /* __TMPR_H */