/**
  ******************************************************************************
  * @file           : bmx280.c
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


/* Includes ------------------------------------------------------------------*/
#include "bmx280.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Locaal variables ---------------------------------------------------------*/
static uint16_t dig_T1  = 0;
static int16_t dig_T2   = 0;
static int16_t dig_T3   = 0;
static uint16_t dig_P1  = 0;
static int16_t dig_P2   = 0;
static int16_t dig_P3   = 0;
static int16_t dig_P4   = 0;
static int16_t dig_P5   = 0;
static int16_t dig_P6   = 0;
static int16_t dig_P7   = 0;
static int16_t dig_P8   = 0;
static int16_t dig_P9   = 0;
static uint8_t dig_H1   = 0;
static int16_t dig_H2   = 0;
static uint8_t dig_H3   = 0;
static int16_t dig_H4   = 0;
static int16_t dig_H5   = 0;
static int8_t dig_H6    = 0;
static BMx280_S32_t t_fine = 0;


/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/
static ErrorStatus BMx280_Read(BMxX80_TypeDef*, uint8_t, uint8_t);
static ErrorStatus BMx280_Write(BMxX80_TypeDef*, uint8_t);

static BMx280_S32_t bmx280_compensate_T_int32(BMx280_S32_t);
static BMx280_U32_t bmx280_compensate_P_int32(BMx280_S32_t);
static BMx280_U32_t bmx280_compensate_H_int32(BMx280_S32_t);


/**
  * @brief  Writes 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @param  data: a byte to send
  * @return status of operation
  */
static int I2C_Write(I2C_TypeDef*, uint8_t, uint8_t*, uint16_t);

/**
  * @brief  Reads 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @return status of operation
  */
static int I2C_Read(I2C_TypeDef*, uint8_t, uint8_t, uint8_t*, uint16_t);








////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus BMx280_Init(BMxX80_TypeDef* dev) {
  // uint8_t buf[32];

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);

  /* Read Device ID and if it isn't equal to the current, exit. */
  dev->RawBufPtr[0] = 0;
  if (BMx280_Read(dev, BMX280_DEV_ID, 1)) return (ERROR);
  
  dev->DevID = dev->RawBufPtr[0];

  if (!((dev->DevID == BMP280_ID) || (dev->DevID == BME280_ID))) return (ERROR);

  /* Read calibration data. This is common for both sensor's types */
  if (BMx280_Read(dev, BMX280_CALIB1, 26)) return (ERROR);
  
  dig_T1 = (uint16_t)((dev->RawBufPtr[1] << 8) | dev->RawBufPtr[0]);
  dig_T2 = (int16_t)((dev->RawBufPtr[3] << 8) | dev->RawBufPtr[2]);
  dig_T3 = (int16_t)((dev->RawBufPtr[5] << 8) | dev->RawBufPtr[4]);
  dig_P1 = (uint16_t)((dev->RawBufPtr[7] << 8) | dev->RawBufPtr[6]);
  dig_P2 = (int16_t)((dev->RawBufPtr[9] << 8) | dev->RawBufPtr[8]);
  dig_P3 = (int16_t)((dev->RawBufPtr[1] << 8) | dev->RawBufPtr[0]);
  dig_P4 = (int16_t)((dev->RawBufPtr[3] << 8) | dev->RawBufPtr[2]);
  dig_P5 = (int16_t)((dev->RawBufPtr[5] << 8) | dev->RawBufPtr[4]);
  dig_P6 = (int16_t)((dev->RawBufPtr[7] << 8) | dev->RawBufPtr[6]);
  dig_P7 = (int16_t)((dev->RawBufPtr[9] << 8) | dev->RawBufPtr[8]);
  dig_P8 = (int16_t)((dev->RawBufPtr[21] << 8) | dev->RawBufPtr[20]);
  dig_P9 = (int16_t)((dev->RawBufPtr[23] << 8) | dev->RawBufPtr[22]);

  /* If the sensor is BME280, read additianal block of calibration data */
  if (dev->DevID == BME280_ID) {
    dig_H1 = (uint8_t)dev->RawBufPtr[25];

    if (BMx280_Read(dev, BMX280_CALIB2, 16))return (ERROR);
    
    dig_H2 = (int16_t)((dev->RawBufPtr[1] << 8) | dev->RawBufPtr[0]);
    dig_H3 = (uint8_t)dev->RawBufPtr[2];
    dig_H4 = (int16_t)((dev->RawBufPtr[3] << 4) | (dev->RawBufPtr[4] & 0x0f));
    dig_H5 = (int16_t)((dev->RawBufPtr[5] << 4) | ((dev->RawBufPtr[4] >> 4) & 0x0f));
    dig_H6 = (int8_t)dev->RawBufPtr[6];
  }

  /* Set filter coefficient and inactive duration in normal mode */
  dev->RawBufPtr[0] = BMX280_SETTINGS;
  dev->RawBufPtr[1] = (BMX280_ConfigInactive_250 << BMX280_ConfigInactive_Pos)
                    | (BMX280_ConfigFilter_4 << BMX280_ConfigFilter_Pos);
  if (BMx280_Write(dev, 2)) return (ERROR);

  /* Set the humidity oversampling precision */
  dev->RawBufPtr[0] = BMX280_CTRL_HUM;
  dev->RawBufPtr[1] = (BMX280_HumidityOvs_x4 << BMX280_HumidityOvs_Pos);
  if (BMx280_Write(dev, 2)) return (ERROR);

  /* Set the temperature and pressure oversampling precision */
  dev->RawBufPtr[0] = BMX280_CTRL_MEAS;
  dev->RawBufPtr[1] = (BMX280_TemperatureOvs_x4 << BMX280_TemperatureOvs_Pos) 
                    | (BMX280_PressureOvs_x4 << BMX280_PressureOvs_Pos)
                    | (BMX280_NormalMode << BMX280_Mode_Pos);
  if (BMx280_Write(dev, 2)) return (ERROR);

  dev->Lock = ENABLE;
  return (SUCCESS);
}




/**
  * @brief  Runs a measurement proccess
  * @param  None
  * @return Error status
  */
ErrorStatus BMx280_Measurment(BMxX80_TypeDef *dev) {

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);

  /* Run conversion in force mode, keep oversampling */
  dev->RawBufPtr[0] = BMX280_CTRL_MEAS;
  dev->RawBufPtr[1] = (BMX280_TemperatureOvs_x4 << BMX280_TemperatureOvs_Pos) 
                    | (BMX280_PressureOvs_x4 << BMX280_PressureOvs_Pos)
                    | (BMX280_ForceMode << BMX280_Mode_Pos);
  if (BMx280_Write(dev, 2)) return (ERROR);

  /* Read the status register and check measuring busy flag, wait for conversion */
  uint8_t wait_status_threshold = 250;
  
  if (BMx280_Read(dev, BMX280_STATUS, 1)) return (ERROR);
  dev->RawBufPtr[0] = BMX280_Measuring;
  while (dev->RawBufPtr[0] & BMX280_Measuring) {
    _delay_ms(1);
    if (BMx280_Read(dev, BMX280_STATUS, 1)) return (ERROR);
    if (!(wait_status_threshold--)) return (ERROR);
  }

  /* Read raw data */
  if (BMx280_Read(dev, BMX280_DATA, 8)) return (ERROR);

  BMx280_S32_t adc_P = (((dev->RawBufPtr[0] << 8) | dev->RawBufPtr[1]) << 4) | (dev->RawBufPtr[2] >> 4);
  BMx280_S32_t adc_T = (((dev->RawBufPtr[3] << 8) | dev->RawBufPtr[4]) << 4) | (dev->RawBufPtr[5] >> 4);
  BMx280_S32_t adc_H = (dev->RawBufPtr[6] << 8) | dev->RawBufPtr[7];

  dev->ResBufPtr[0] = bmx280_compensate_T_int32(adc_T);
  dev->ResBufPtr[1] = bmx280_compensate_P_int32(adc_P);
  dev->ResBufPtr[2] = bmx280_compensate_H_int32(adc_H);

  dev->Lock = ENABLE;
  return (SUCCESS);
}




/**
  * @brief  Reads BMx280 data
  * @param  item: BMx280 item to exploit for reading
  * @param  reg: a register to read from BMx280
  * @param  buf: a pointer of a buffer which is for store data
  * @param  len: length of the buffer
  * @return Error status
  */
static ErrorStatus BMx280_Read(BMxX80_TypeDef *dev, uint8_t reg, uint8_t len) {
  if (dev->I2CBus != NULL) {
    if (I2C_Read(dev->I2CBus, BMX280_I2C_ADDR, reg, dev->RawBufPtr, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIBus != NULL) {

    /* TODO realize SPI bus, probably with BME680 */
    return (SUCCESS);
  } 

  return (ERROR);
}




/**
  * @brief  Writess BMx280 data
  * @param  item: BMx280 item to exploit for writing
  * @param  buf: a pointer of a buffer when sending data is stored 
  *         (the first item should contain a BMx280 register address)
  * @param  len: length of the buffer
  * @return Error status
  */
static ErrorStatus BMx280_Write(BMxX80_TypeDef *dev, uint8_t len) {
  if (dev->I2CBus != NULL) {
    if (I2C_Write(dev->I2CBus, BMX280_I2C_ADDR, dev->RawBufPtr, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIBus != NULL) {

    /* TODO realize SPI bus, probably with BME680 */
    return (SUCCESS);
  } 

  return (ERROR);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/*
  Here is a bunch of functions provided by Bosch sensor engineering 
  and proposed by BMx280 datasheet. They were set here with minor
  changed. The logic was kept.
  
  t_fine carries fine temperature as global value
*/

/**
  * @brief  Returns temperature in DegC, resolution is 0.01 DegC.
  *         Output value of “5123” equals 51.23 DegC.
  * @param  adc_T: Raw temperature data
  * @return Measured temprerature
  */
static BMx280_S32_t bmx280_compensate_T_int32(BMx280_S32_t adc_T) {
  BMx280_S32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((BMx280_S32_t)dig_T1 << 1))) * (BMx280_S32_t)dig_T2) >> 11;
  var2 = (((((adc_T >> 4) - (BMx280_S32_t)dig_T1) * ((adc_T >> 4) - (BMx280_S32_t)dig_T1)) >> 12) * (BMx280_S32_t)dig_T3) >> 14;
  t_fine = var1 + var2;

  T = (t_fine * 5 + 128) >> 8;
  return (T);
}



/**
  * @brief  Returns pressure in Pa as unsigned 32 bit integer.
  *         Output value of “96386” equals 96386 Pa = 963.86 hPa
  * @param  adc_P: Raw pressure data
  * @return Measured pressure
  */
static BMx280_U32_t bmx280_compensate_P_int32(BMx280_S32_t adc_P) {
  BMx280_S32_t var1, var2;
  BMx280_U32_t P;
  var1 = (((BMx280_S32_t)t_fine) >> 1) - (BMx280_S32_t)64000;
  var2 = (((var1 >> 2) * (var1 >> 2)) >> 11 ) * ((BMx280_S32_t)dig_P6);
  var2 = var2 + ((var1 * ((BMx280_S32_t)dig_P5)) << 1);
  var2 = (var2 >> 2) + (((BMx280_S32_t)dig_P4) << 16);
  var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13 )) >> 3) + ((((BMx280_S32_t)dig_P2) * var1) >> 1)) >> 18;
  var1 = ((((32768 + var1)) * ((BMx280_S32_t)dig_P1)) >> 15);

  if (var1 == 0) return (0); // avoid excePtion caused by division by zero

  P = (((BMx280_U32_t)(((BMx280_S32_t)1048576) - adc_P) - (var2 >> 12))) * 3125;
  if (P < 0x80000000) {
    P = (P << 1) / ((BMx280_U32_t)var1);
  } else {
    P = (P / (BMx280_U32_t)var1) * 2;
  }

  var1 = (((BMx280_S32_t)dig_P9) * ((BMx280_S32_t)(((P >> 3) * (P >> 3)) >> 13))) >> 12;
  var2 = (((BMx280_S32_t)(P >> 2)) * ((BMx280_S32_t)dig_P8)) >> 13;

  P = (BMx280_U32_t)((BMx280_S32_t)P + ((var1 + var2 + dig_P7) >> 4));
  return (P);
}




/**
  * @brief  Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
  *         Output value of “47445” represents 47445/1024 = 46.333 %RH
  * @param  adc_H: Raw humidity data
  * @return Measured humidity
  */
static BMx280_U32_t bmx280_compensate_H_int32(BMx280_S32_t adc_H) {
  // varant 2
  BMx280_U32_t H;
  H = (BMx280_S32_t)t_fine - (BMx280_S32_t)76800;
  H = (((((adc_H << 14) - ((BMx280_S32_t)dig_H4 << 20) - ((BMx280_S32_t)dig_H5 * H)) + 16384) >> 15) * ((((((((BMx280_S32_t)H * dig_H6) >> 10) * ((((BMx280_S32_t)H * dig_H3) >> 11) + 32768)) >> 10) + 2097152) * (BMx280_S32_t)dig_H2 + 8192) >> 14));
  H = (H - (((((H >> 15) * (H >> 15)) >> 7) * (BMx280_S32_t)dig_H1) >> 4));

  if (H > 0x19000000) H = 0x19000000;
  if (H < 0) H = 0;

  H >>= 12;
  return (H);
}





/**
  * @brief  Writes 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @param  data: a byte to send
  * @return None
  */
static int I2C_Write(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t *buf, uint16_t len) {
  if (I2C_Master_Send(I2Cx, slaveAddr, buf, len)) return (ERROR);
  return (0);
}






/**
  * @brief  Reads 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @return  a received byte
  */
static int I2C_Read(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t reg, uint8_t *buf, uint16_t len) {
  if (I2C_Master_Send(I2Cx, slaveAddr, &reg, 1)) return (ERROR);

  I2C_Master_Receive(I2Cx, slaveAddr, buf, len);

  return (0);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
