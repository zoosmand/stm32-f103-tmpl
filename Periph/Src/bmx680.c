/**
  ******************************************************************************
  * @file           : bmx680.c
  * @brief          : This file contains code for BOSCH BMx680 temperature, 
  *                   humidity, ans atmospheric pressue sensor.
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

/**
 * @brief Adjusts SPI bus according to device requirements.
 * @param   dev: pointer to the device struct
 * @retval  none
 */
__STATIC_INLINE void SPI_Adjust(BMxX80_TypeDef*);

////////////////////////////////////////////////////////////////////////////////





// ----------------------------------------------------------------------------
__STATIC_INLINE void SPI_Adjust(BMxX80_TypeDef* dev) {

  /* adjust frequency divider, 0b001 = 4, (PCLK)72/4 = 18MHz */
  /* set 8-bit data buffer length */ 
  MODIFY_REG(dev->SPIx->CR1, (SPI_CR1_BR_Msk | SPI_CR1_DFF_Msk), (SPI_CR1_BR_1 | SPI_CR1_DFF));
  PREG_SET(dev->SPIx->CR2, SPI_CR2_SSOE_Pos);


  // uint8_t pump = 0;
  // /* configure DMA, Channel2 - RX */
  // /* set priority high*/
  // /* set memory to increment */
  // MODIFY_REG(dev->DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC));
  
  // /* set buffer size to 0 */
  // dev->DMAxRx->CNDTR = 0UL;
  // /* set peripheral address */
  // dev->DMAxRx->CPAR = (uint32_t)&dev->SPIx->DR;
  // /* set memory address */
  // dev->DMAxRx->CMAR = (uint32_t)&pump;
  
  // /* configure DMA, Channel3 - TX */
  // /* set priority high*/
  // /* set memory to increment */
  // /* set direction from memory to peripheral */
  // MODIFY_REG(dev->DMAxTx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR));

  // /* set buffer size to 0 */
  // dev->DMAxTx->CNDTR = 0UL;
  // /* set peripheral address */
  // dev->DMAxTx->CPAR = (uint32_t)&dev->SPIx->DR;
  // /* set memory address */
  // dev->DMAxTx->CMAR = (uint32_t)&pump;

  dev->DMAxTx->CCR = 0UL;
  dev->DMAxRx->CCR = 0UL;
  /* Clear correspondents DMA flags */
  dev->DMAx->IFCR |= 0x00000ff0;

}






// ----------------------------------------------------------------------------

ErrorStatus BMx680_Init(BMxX80_TypeDef* dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  
  
  /* Initialize NSS Pin */
  if (dev->SPINssPin > 7) {
    MODIFY_REG(dev->SPINssPort->CRL, (0xf << ((dev->SPINssPin - 8) * 4)), ((GPIO_GPO_PP | GPIO_IOS_2) << ((dev->SPINssPin -8) * 4)));
  } else {
    MODIFY_REG(dev->SPINssPort->CRL, (0xf << (dev->SPINssPin * 4)), ((GPIO_GPO_PP | GPIO_IOS_2) << (dev->SPINssPin * 4)));
  }
  PIN_H(dev->SPINssPort, dev->SPINssPin);
  
  _delay_us(10);
  
  SPI_Adjust(dev);
  if (SPI_Enable(dev->SPIx)) return (ERROR);
  
  PIN_L(dev->SPINssPort, dev->SPINssPin);
  while (PREG_CHECK(dev->SPINssPort->IDR, dev->SPINssPin));
  
  /* write a command, a dummy byte has to be read further */
  *(__IO uint8_t*)&dev->SPIx->DR = 0x50 | 0x80;
  

  uint32_t tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_TXE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (1);
    }
  }
  
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (1);
    }
  }
  
  uint8_t regNum = dev->SPIx->DR;

  PIN_H(dev->SPINssPort, dev->SPINssPin);
  if (SPI_Disable(dev->SPIx)) return (ERROR);

  if (regNum != BME680_ID) return (ERROR);

  dev->Lock = DISABLE;
  return (SUCCESS);
}




/**
  * @brief  Runs a measurement proccess
  * @param  None
  * @return Error status
  */
ErrorStatus BMx680_Measurment(BMxX80_TypeDef *dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);


  dev->Lock = DISABLE;
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
  if (dev->I2Cx != NULL) {
    if (I2C_Read(dev->I2Cx, BMX680_I2C_ADDR, reg, dev->RawBufPtr, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {

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
  if (dev->I2Cx != NULL) {
    if (I2C_Write(dev->I2Cx, BMX680_I2C_ADDR, dev->RawBufPtr, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {

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
