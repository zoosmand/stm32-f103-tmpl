/**
  ******************************************************************************
  * @file           : bmx680.h
  * @brief          : This file contains header definitions BOSCH BMx680 
  *                   temperature, humidity, and atmospheric pressue sensor.
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


#ifndef __BMX680_H
#define __BMX680_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/

#define BMX680_I2C_ADDR             0x76
#define BME680_ID                   0x61

#define BMx680_RW_BIT               0x80

#define BMx680_SPI_DEV_ID_REG       0x50



/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initializes the given Bosch BMx680 device.
 * @param   dev: pointer to the Bosch BMx680 device struct
 * @retval  status of operation
 */
ErrorStatus BMx680_Init(BMxX80_TypeDef*);

/**
 * @brief   Runs the measurment procedures on the given Bosch BMx680 device.
 * @param   dev: pointer to the Bosch BMx680 device struct
 * @retval  status of operation
 */
ErrorStatus BMx680_Measurment(BMxX80_TypeDef*);


#ifdef __cplusplus
}
#endif
#endif /* __BMX680_H */

