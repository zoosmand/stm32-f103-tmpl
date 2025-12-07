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


/* BMx680 Registers */
#define BMx680_dev_id               0x50
#define BMx680_spi_page             0x73
#define BMx680_spi_reset            0x60

#define BMx680_par_t1               0xe9
#define BMx680_par_t2               0x8a
#define BMx680_par_t3               0x8c

#define BMx680_calib1               0x8a // 20
#define BMx680_calib2               0xe1 // 16

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

