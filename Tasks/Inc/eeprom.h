/**
  ******************************************************************************
  * @file           : eeprom.h
  * @brief          : This file contains header definitions for EEPROM devices
  *                   code in located in eeprom.c file.
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
#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private structures -------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

void EepromHealthCheck_CronHandler(void);

/**
 * @brief Return BMx Bosch device struct pointer.
 * @return device struct pointer
 */
W25qxx_TypeDef* Get_EepromDevice(void);


#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H */