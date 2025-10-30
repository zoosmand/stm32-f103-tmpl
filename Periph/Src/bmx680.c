/**
  ******************************************************************************
  * @file           : bmx680.c
  * @brief          : This file contains header definitions BOSCH BMx680 
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
#include "bmx680.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Locaal variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

static ErrorStatus BMx680_Read(BMxX80_TypeDef*, uint8_t, uint8_t);
static ErrorStatus BMx680_Write(BMxX80_TypeDef*, uint8_t);

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

ErrorStatus BMx680_Init(BMxX80_TypeDef* dev) {
  // uint8_t buf[32];

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);


  dev->Lock = ENABLE;
  return (SUCCESS);
}




/**
  * @brief  Runs a measurement proccess
  * @param  None
  * @return Error status
  */
ErrorStatus BMx680_Measurment(BMxX80_TypeDef *dev) {

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);


  dev->Lock = ENABLE;
  return (SUCCESS);
}




/**
  * @brief  Reads BMx680 data
  * @param  item: BMx280 item to exploit for reading
  * @param  reg: a register to read from BMx280
  * @param  buf: a pointer of a buffer which is for store data
  * @param  len: length of the buffer
  * @return Error status
  */
static ErrorStatus BMx680_Read(BMxX80_TypeDef *dev, uint8_t reg, uint8_t len) {
  if (dev->I2CBus != NULL) {
    if (I2C_Read(dev->I2CBus, BMX680_I2C_ADDR, reg, dev->RawBufPtr, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIBus != NULL) {

    /* TODO realize SPI bus, probably with BME680 */
    return (SUCCESS);
  } 

  return (ERROR);
}




/**
  * @brief  Writess BMx680 data
  * @param  item: BMx280 item to exploit for writing
  * @param  buf: a pointer of a buffer when sending data is stored 
  *         (the first item should contain a BMx280 register address)
  * @param  len: length of the buffer
  * @return Error status
  */
static ErrorStatus BMx680_Write(BMxX80_TypeDef *dev, uint8_t len) {
  if (dev->I2CBus != NULL) {
    if (I2C_Write(dev->I2CBus, BMX680_I2C_ADDR, dev->RawBufPtr, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIBus != NULL) {

    /* TODO realize SPI bus, probably with BME680 */
    return (SUCCESS);
  } 

  return (ERROR);
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
