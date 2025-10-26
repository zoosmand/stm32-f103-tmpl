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

#define MAX72XX_SEG_CNT       4
#define MAX72XX_MAX_SEG_CNT   (MAX72XX_SEG_CNT * 8)




/**
 * @brief   Bosch BMx280 device type definition struct.
 */
typedef struct {
  uint8_t       ID;
  uint16_t      T1;
  int16_t       T2;
  int16_t       T3;
  uint16_t      P1;
  int16_t       P2;
  int16_t       P3;
  int16_t       P4;
  int16_t       P5;
  int16_t       P6;
  int16_t       P7;
  int16_t       P8;
  int16_t       P9;
  uint8_t       H1;
  int16_t       H2;
  uint8_t       H3;
  uint8_t       Lock;
  I2C_TypeDef*  I2Cx;
  uint8_t*      BufPtr;
} BMx280_TypeDef;


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


/* Types demanded by BMx280 datasheet */
typedef int32_t               BMP280_S32_t;
typedef uint32_t              BMP280_U32_t;



/**
 * @brief   Initializes the given Bosch BMx280 device.
 * @param   dev: pointer to the MAX72xx device struct
 * @retval  (int) status of operation
 */
// int BMx280_Init(BMx280_TypeDef*);



BMP280_S32_t* BMP280_ReadT(void);
BMP280_U32_t* BMP280_ReadP(void);
double* BMP280_ReadTP(void);
double* BMP280_ReadPP(void);









/* Global variables ----------------------------------------------------------*/
extern uint32_t _BMX280REG_;
extern int32_t temperature;
extern uint32_t pressure;
extern uint32_t humidity;



/* Private defines -----------------------------------------------------------*/
typedef enum {
  BMP280  = 0x58,
  BME280  = 0x60
} BMx280_SensorTypeDef;

typedef enum {
  BMx280_SPI  = 0,
  BMx280_I2C  = !BMx280_SPI
} BMx280_BusTypeDef;

typedef struct {  
  BMx280_SensorTypeDef sensorType;
  BMx280_BusTypeDef busType;
  I2C_TypeDef *bus;
} BMx280_ItemTypeDef;

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

/* Types demanded by BMx280 datasheet */
typedef int32_t           BMx280_S32_t;
typedef uint32_t          BMx280_U32_t;

/* BMX280 Registry Flags */


extern ErrorStatus bmx280Status;

/* Exported functions prototypes ---------------------------------------------*/
ErrorStatus BMx280_Init(BMx280_ItemTypeDef*);
ErrorStatus BMx280_Measurment(BMx280_ItemTypeDef*, int32_t*);


#ifdef __cplusplus
}
#endif
#endif /* __BMX280_H */

