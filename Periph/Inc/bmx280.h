/**
  ******************************************************************************
  * @file           : bmx280.h
  * @brief          : This file contains header definitions BOSCH BMx280 
  *                   temperature, humidity, ans atmospheric pressue
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


#ifndef __BMX280_H
#define __BMX280_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/
/* BMx280 registers */
#define SensorID              0xd0
#define Calib1                0x88
#define Calib2                0xe1
#define CollectData           0xf7
#define ConfigSensor          0xf5
#define CtrlMeasure           0xf4
#define StatusSensor          0xf3
#define CtrlHumidity          0xf2
#define ReserSensor           0xe0
/* Definitions for Status register */
#define Measuring             0x08
#define Measuring_Pos         3
#define ImUpdate              0x00
#define ImUpdate_Pos          0
/* Definitions for Control Measuring register */
#define TemperatureOvsMask    0xe0
#define TemperatureOvs_Pos    5
#define PressureOvsMask       0x1c 
#define PressureOvs_Pos       2 
#define ModeMask              0x03
#define Mode_Pos              0
#define SleepMode             0x00
#define ForceMode             0x01
#define NormalMode            0x03
/* Some other definitions */
#define WriteMask             0x7f
#define ResetValue            0xb6
#define BMP280_ID             0x58
#define BME280_ID             0x60
#define BME680_ID             0x62


/* Global variables ----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define BMX280_I2C_ADDR   0x76

#define BMX280_DEV_ID     0xd0
#define BMX280_CALIB1     0x88
#define BMX280_CALIB2     0xe1
#define BMX280_CTRL_HUM   0xf2
#define BMX280_CTRL_MEAS  0xf4
#define BMX280_STATUS     0xf3
#define BMX280_DATA       0xf7
#define BMX280_SETTINGS   0xf5
#define BMX280_RESET      0xe0
#define BMX280_RESET_VAL  0xb6

/* The redefined by Bosch types of integers from the dedicated datasheet */
typedef int32_t           BMx280_S32_t;
typedef uint32_t          BMx280_U32_t;



/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initializes the given Bosch BMx280 device.
 * @param   dev: pointer to the Bosch BMx280 device struct
 * @retval  status of operation
 */
ErrorStatus BMx280_Init(BMxX80_TypeDef*);

ErrorStatus BMx280_Measurment(BMxX80_TypeDef*);


#ifdef __cplusplus
}
#endif
#endif /* __BMX280_H */

