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

#define BMX280_I2C_ADDR             0x76
#define BMP280_ID                   0x58
#define BME280_ID                   0x60

/* BMx280 General Registers */
#define BMX280_DEV_ID               0xd0
#define BMX280_CALIB1               0x88
#define BMX280_CALIB2               0xe1
#define BMX280_CTRL_HUM             0xf2
#define BMX280_CTRL_MEAS            0xf4
#define BMX280_STATUS               0xf3
#define BMX280_DATA                 0xf7
#define BMX280_SETTINGS             0xf5
#define BMX280_RESET                0xe0
#define BMX280_RESET_VAL            0xb6

/* BMx280 Status Register */
#define BMX280_Measuring            0x08
#define BMX280_Measuring_Pos        3
#define BMX280_ImUpdate             0x00
#define BMX280_ImUpdate_Pos         0

/* BMx280 Control Measuring Register */
#define BMX280_TemperatureOvsMask   0xe0
#define BMX280_TemperatureOvs_Pos   5
#define BMX280_TemperatureOvs_x0    0b000
#define BMX280_TemperatureOvs_x1    0b001
#define BMX280_TemperatureOvs_x2    0b010
#define BMX280_TemperatureOvs_x4    0b011
#define BMX280_TemperatureOvs_x8    0b100
#define BMX280_TemperatureOvs_x16   0b101

#define BMX280_PressureOvsMask      0x1c
#define BMX280_PressureOvs_Pos      2
#define BMX280_PressureOvs_x0       0b000
#define BMX280_PressureOvs_x1       0b001
#define BMX280_PressureOvs_x2       0b010
#define BMX280_PressureOvs_x4       0b011
#define BMX280_PressureOvs_x8       0b100
#define BMX280_PressureOvs_x16      0b101

#define BMX280_HumidityOvsMask      0x03
#define BMX280_HumidityOvs_Pos      0
#define BMX280_HumidityOvs_x0       0b000
#define BMX280_HumidityOvs_x1       0b001
#define BMX280_HumidityOvs_x2       0b010
#define BMX280_HumidityOvs_x4       0b011
#define BMX280_HumidityOvs_x8       0b100
#define BMX280_HumidityOvs_x16      0b101

#define BMX280_ConfigInactiveMask   0xe0
#define BMX280_ConfigInactive_Pos   5
#define BMX280_ConfigInactive_1     0b000
#define BMX280_ConfigInactive_10    0b110
#define BMX280_ConfigInactive_20    0b111
#define BMX280_ConfigInactive_62    0b001
#define BMX280_ConfigInactive_125   0b010
#define BMX280_ConfigInactive_250   0b011
#define BMX280_ConfigInactive_500   0b100
#define BMX280_ConfigInactive_1000  0b101

#define BMX280_ConfigFilterMask     0x1c
#define BMX280_ConfigFilter_Pos     2
#define BMX280_ConfigFilter_0       0b000
#define BMX280_ConfigFilter_2       0b001
#define BMX280_ConfigFilter_4       0b010
#define BMX280_ConfigFilter_8       0b011
#define BMX280_ConfigFilter_16      0b100

#define BMX280_ModeMask             0x03
#define BMX280_Mode_Pos             0
#define BMX280_SleepMode            0b00
#define BMX280_ForceMode            0b01
#define BMX280_NormalMode           0b11

/* Some other definitions */
#define BMX280_WriteMask            0x7f



/* The redefined by Bosch types of integers from the dedicated datasheet */
typedef int32_t                     BMx280_S32_t;
typedef uint32_t                    BMx280_U32_t;



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

