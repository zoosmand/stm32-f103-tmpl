/**
  ******************************************************************************
  * @file           : bmx280.c
  * @brief          : This file contains header definitions BOSCH BMx280 
  *                   temperature, humidity, ans atmospheric pressue sensor.
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

/**
 * @brief  Writes/sends data to a BMx680 device.
 * @param  dev: a Bocsh BMxX80 device structure pointer
 * @param  len: number of bytes to send
 * @return status of operation
 */
static ErrorStatus bmx280_send(BMxX80_TypeDef*, uint8_t);

/**
  * @brief  Reads/receives data from a BMx680 device.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  reg: a register address or command to send to the device
  * @param  len: number of bytes to receive
  * @return status of operation
  */
static ErrorStatus bmx280_receive(BMxX80_TypeDef*, uint8_t, uint8_t);

/**
 * @brief  Writes/sends data via I2C.
 * @param  dev: a Bocsh BMxX80 device structure pointer
 * @param  len: number of bytes to send
 * @return status of operation
 */
static ErrorStatus i2c_send(BMxX80_TypeDef*, uint16_t);

/**
 * @brief  Reads/receives data via I2C.
 * @param  dev: a Bocsh BMxX80 device structure pointer
 * @param  reg: a register address or command to send to the device
 * @param  len: number of bytes to receive
 * @return status of operation
 */
static ErrorStatus i2c_receive(BMxX80_TypeDef*, uint8_t, uint16_t);

/**
  * @brief  Calculates temperature in DegC, resolution is 0.01 DegC.
  *         Output value of “5123” equals 51.23 DegC.
  * @param  adc_T: Raw temperature data
  * @return Measured temprerature
  */
static BMx280_S32_t bmx280_compensate_t_int32(BMx280_S32_t);

/**
  * @brief  Calcutales pressure in Pa as unsigned 32 bit integer.
  *         Output value of “96386” equals 96386 Pa = 963.86 hPa
  * @param  adc_P: Raw pressure data
  * @return Measured pressure
  */
static BMx280_U32_t bmx280_compensate_p_int32(BMx280_S32_t);

/**
  * @brief  Calculates humidity in %RH as unsigned 32 bit integer in Q22.10 format 
  *         (22 integer and 10 fractional bits).
  *         Output value of “47445” represents 47445/1024 = 46.333 %RH
  * @param  adc_H: Raw humidity data
  * @return Measured humidity
  */
static BMx280_U32_t bmx280_compensate_h_int32(BMx280_S32_t);

/**
  * @brief   Adjusts I2C bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE void i2c_dma_configure(BMxX80_TypeDef*);

/**
  * @brief   Adjusts I2C bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE ErrorStatus i2c_dma_unconfigure(BMxX80_TypeDef*);





////////////////////////////////////////////////////////////////////////////////





// ----------------------------------------------------------------------------

__STATIC_INLINE void i2c_dma_configure(BMxX80_TypeDef* dev) {

  /* configure DMA, Channel7 - RX */
  /* set priority high*/
  /* set memory to increment */
  /* clear dir bit, means reading from the bus */
  MODIFY_REG(dev->DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC));
  
  /* set peripheral address */
  dev->DMAxRx->CPAR = (uint32_t)&dev->I2Cx->DR;
  /* set memory address */
  dev->DMAxRx->CMAR = (uint32_t)dev->RawBufPtr;
  
}


// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus i2c_dma_unconfigure(BMxX80_TypeDef* dev) {

  /* Disable DMA transfer */
  PREG_CLR(dev->I2Cx->CR2, I2C_CR2_DMAEN);

  /* Disable transfer from peripheral to memory */
  PREG_CLR(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);

  /* Receive the last byte and send NACK */
  PREG_CLR(dev->I2Cx->CR1, I2C_CR1_ACK_Pos);
  I2C_ReadByte(dev->I2Cx); 

  I2C_Stop(dev->I2Cx);

  /* Clear DMA configuration */
  MODIFY_REG(dev->DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), 0UL);
  
  /* Clear peripheral address */
  dev->DMAxRx->CCR = 0UL;
  /* Clear correspondents DMA interrupt flags */
  dev->DMAx->IFCR = (DMA_IFCR_CHTIF7_Msk | DMA_IFCR_CGIF7_Msk | DMA_IFCR_CTCIF7_Msk);

  return (ERROR);
}









////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus BMx280_Init(BMxX80_TypeDef* dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);


  /* Read Device ID and if it isn't equal to the current, exit. */
  dev->RawBufPtr[0] = 0;
  if (bmx280_receive(dev, BMX280_DEV_ID, 1)) return (ERROR);
  
  dev->DevID = dev->RawBufPtr[0];

  if (!((dev->DevID == BMP280_ID) || (dev->DevID == BME280_ID))) return (ERROR);

  /* Read calibration data. This is common for both sensor's types */
  if (bmx280_receive(dev, BMX280_CALIB1, 26)) return (ERROR);
  
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

    if (bmx280_receive(dev, BMX280_CALIB2, 16))return (ERROR);
    
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
  if (bmx280_send(dev, 2)) return (ERROR);

  /* Set the humidity oversampling precision */
  dev->RawBufPtr[0] = BMX280_CTRL_HUM;
  dev->RawBufPtr[1] = (BMX280_HumidityOvs_x4 << BMX280_HumidityOvs_Pos);
  if (bmx280_send(dev, 2)) return (ERROR);

  /* Set the temperature and pressure oversampling precision */
  dev->RawBufPtr[0] = BMX280_CTRL_MEAS;
  dev->RawBufPtr[1] = (BMX280_TemperatureOvs_x4 << BMX280_TemperatureOvs_Pos) 
                    | (BMX280_PressureOvs_x4 << BMX280_PressureOvs_Pos)
                    | (BMX280_NormalMode << BMX280_Mode_Pos);
  if (bmx280_send(dev, 2)) return (ERROR);

  dev->Lock = DISABLE;
  return (SUCCESS);
}




/**
  * @brief  Runs a measurement proccess
  * @param  None
  * @return Error status
  */
ErrorStatus BMx280_Measurement(BMxX80_TypeDef *dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  /* Run conversion in force mode, keep oversampling */
  dev->RawBufPtr[0] = BMX280_CTRL_MEAS;
  dev->RawBufPtr[1] = (BMX280_TemperatureOvs_x4 << BMX280_TemperatureOvs_Pos) 
                    | (BMX280_PressureOvs_x4 << BMX280_PressureOvs_Pos)
                    | (BMX280_ForceMode << BMX280_Mode_Pos);
  if (bmx280_send(dev, 2)) return (ERROR);

  /* Read the status register and check measuring busy flag, wait for conversion */
  uint8_t wait_status_threshold = 250;
  
  if (bmx280_receive(dev, BMX280_STATUS, 1)) return (ERROR);
  dev->RawBufPtr[0] = BMX280_Measuring;
  while (dev->RawBufPtr[0] & BMX280_Measuring) {
    _delay_ms(1);
    if (bmx280_receive(dev, BMX280_STATUS, 1)) return (ERROR);
    if (!(wait_status_threshold--)) return (ERROR);
  }

  /* Read raw data */
  if (bmx280_receive(dev, BMX280_DATA, 8)) return (ERROR);

  BMx280_S32_t adc_P = (((dev->RawBufPtr[0] << 8) | dev->RawBufPtr[1]) << 4) | (dev->RawBufPtr[2] >> 4);
  BMx280_S32_t adc_T = (((dev->RawBufPtr[3] << 8) | dev->RawBufPtr[4]) << 4) | (dev->RawBufPtr[5] >> 4);
  BMx280_S32_t adc_H = (dev->RawBufPtr[6] << 8) | dev->RawBufPtr[7];

  dev->Results.temperature  = bmx280_compensate_t_int32(adc_T);
  dev->Results.pressure     = bmx280_compensate_p_int32(adc_P);
  dev->Results.humidity     = bmx280_compensate_h_int32(adc_H);

  dev->Lock = DISABLE;
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
static ErrorStatus bmx280_receive(BMxX80_TypeDef *dev, uint8_t reg, uint8_t len) {
  if (dev->I2Cx != NULL) {
    if (i2c_receive(dev, reg, len)) {
      /* Stop I2C bus */
      return (i2c_dma_unconfigure(dev));
    }
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {

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
static ErrorStatus bmx280_send(BMxX80_TypeDef *dev, uint8_t len) {
  if (dev->I2Cx != NULL) {
    if (i2c_send(dev, len)) {
      /* Stop I2C bus */
      return (i2c_dma_unconfigure(dev));
    }
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {

    /* TODO realize SPI bus, probably with BME680 */
    return (SUCCESS);
  } 

  return (ERROR);
}




// ----------------------------------------------------------------------------
/*
  The following is a set of functions provided by Bosch sensor engineering 
  and proposed by the BMx280 datasheet. They were set here with minor changes. 
  The logic has been kept.
  
  t_fine carries fine temperature as global value
*/

// ----------------------------------------------------------------------------
// ----------------------- Device specifiv function ---------------------------
// ----------------------------------------------------------------------------
static BMx280_S32_t bmx280_compensate_t_int32(BMx280_S32_t adc_T) {
  BMx280_S32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((BMx280_S32_t)dig_T1 << 1))) * (BMx280_S32_t)dig_T2) >> 11;
  var2 = (((((adc_T >> 4) - (BMx280_S32_t)dig_T1) * ((adc_T >> 4) - (BMx280_S32_t)dig_T1)) >> 12) * (BMx280_S32_t)dig_T3) >> 14;
  t_fine = var1 + var2;

  T = (t_fine * 5 + 128) >> 8;
  return (T);
}



// ----------------------------------------------------------------------------
// ----------------------- Device specifiv function ---------------------------
// ----------------------------------------------------------------------------

static BMx280_U32_t bmx280_compensate_p_int32(BMx280_S32_t adc_P) {
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




// ----------------------------------------------------------------------------
// ----------------------- Device specifiv function ---------------------------
// ----------------------------------------------------------------------------

static BMx280_U32_t bmx280_compensate_h_int32(BMx280_S32_t adc_H) {
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





// ----------------------------------------------------------------------------

static ErrorStatus i2c_send(BMxX80_TypeDef* dev, uint16_t len) {
  
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, dev->RawBufPtr, len)) return (ERROR);
  
  return (SUCCESS);
}




// ----------------------------------------------------------------------------

static ErrorStatus i2c_receive(BMxX80_TypeDef *dev, uint8_t reg, uint16_t len) {

  uint32_t tmout;

  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, &reg, 1)) return (i2c_dma_unconfigure(dev));
  i2c_dma_configure(dev);

  /* Set counter */
  dev->DMAxRx->CNDTR = len;

  /* Enable transfer from peripheral to memory */
  PREG_SET(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);

  /* Enable receiving DMA transfer */
  PREG_SET(dev->I2Cx->CR2, I2C_CR2_DMAEN_Pos);
  PREG_SET(dev->I2Cx->CR1, I2C_CR1_ACK_Pos);

  /* Start bus transmission */
  if (I2C_Start(dev->I2Cx)) return (i2c_dma_unconfigure(dev));  
  if (I2C_SendAddress(dev->I2Cx, dev->I2C_Address, RX)) return (ERROR);
  
  /* --- Read/receive data from the bus via DMA --- */
  /* Wait for transfer is compete */
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF7_Pos))) {
    if (!(--tmout)) return (ERROR);
  }

  /* Stop I2C bus transmission and unvonfigure DMA */
  i2c_dma_unconfigure(dev);

  return (SUCCESS);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
