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

/**
 * @brief  Writes/sends data to a BMx680 device.
 * @param  dev: a Bocsh BMxX80 device structure pointer
 * @param  len: number of bytes to send
 * @return status of operation
 */
static ErrorStatus BMx680_Write(BMxX80_TypeDef*, uint8_t);

/**
  * @brief  Reads/receives data from a BMx680 device.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  reg: a register address or command to send to the device
  * @param  len: number of bytes to receive
  * @return status of operation
  */
static ErrorStatus BMx680_Read(BMxX80_TypeDef*, uint8_t, uint8_t);

/**
  * @brief  Writes/sends data via I2C.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  len: number of bytes to send
  * @return status of operation
  */
static ErrorStatus I2C_Write(BMxX80_TypeDef*, uint16_t);

/**
  * @brief  Reads/receives data via I2C.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  reg: a register address or command to send to the device
  * @param  len: number of bytes to receive
  * @return status of operation
  */
static ErrorStatus I2C_Read(BMxX80_TypeDef*, uint8_t, uint16_t);

/**
  * @brief  Writes/sends data via SPI.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  len: number of bytes to send
  * @return status of operation
  */
static ErrorStatus SPI_Write(BMxX80_TypeDef*, uint16_t);

/**
  * @brief  Reads/receives data via SPI.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  reg: a register address or command to send to the device
  * @param  len: number of bytes to receive
  * @return status of operation
  */
static ErrorStatus SPI_Read(BMxX80_TypeDef*, uint8_t, uint16_t);

/**
  * @brief   Adjusts SPI bus according to device requirements.
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
  
  /* Get device ID */
  SPI_Read(dev, BMx680_SPI_DEV_ID_REG, 1);

  if (dev->RawBufPtr[0] != BME680_ID) return (ERROR);

  dev->Lock = DISABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

ErrorStatus BMx680_Measurment(BMxX80_TypeDef *dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);


  dev->Lock = DISABLE;
  return (SUCCESS);
}




// ----------------------------------------------------------------------------

static ErrorStatus BMx680_Read(BMxX80_TypeDef *dev, uint8_t reg, uint8_t len) {
  if (dev->I2Cx != NULL) {
    if (I2C_Read(dev, reg, len)) return (ERROR);
  } 

  if (dev->SPIx != NULL) {
    if (SPI_Read(dev, reg, len)) return(ERROR);
  } 
  return (ERROR);
}



// ----------------------------------------------------------------------------

static ErrorStatus BMx680_Write(BMxX80_TypeDef *dev, uint8_t len) {
  if (dev->I2Cx != NULL) {
    if (I2C_Write(dev, len)) return (ERROR);
  } 

  if (dev->SPIx != NULL) {
    return (SUCCESS);
  } 
  return (ERROR);
}





// ----------------------------------------------------------------------------

static ErrorStatus I2C_Write(BMxX80_TypeDef* dev, uint16_t len) {
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, dev->RawBufPtr, len)) return (ERROR);
  return (SUCCESS);
}





// ----------------------------------------------------------------------------

static ErrorStatus I2C_Read(BMxX80_TypeDef* dev, uint8_t reg, uint16_t len) {
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, &reg, 1)) return (ERROR);
  I2C_Master_Receive(dev->I2Cx, dev->I2C_Address, dev->RawBufPtr, len);
  return (SUCCESS);
}




// ----------------------------------------------------------------------------

static ErrorStatus SPI_Write(BMxX80_TypeDef *dev, uint16_t len) {

  return (SUCCESS);
}



  // ----------------------------------------------------------------------------

static ErrorStatus SPI_Read(BMxX80_TypeDef *dev, uint8_t reg, uint16_t len) {
  uint32_t tmout;
  
  SPI_Adjust(dev);
  if (SPI_Enable(dev->SPIx)) return (ERROR);
  
  PIN_L(dev->SPINssPort, dev->SPINssPin);
  while (PREG_CHECK(dev->SPINssPort->IDR, dev->SPINssPin));
  
  /* send a command (register address) to the device */
  *(__IO uint8_t*)&dev->SPIx->DR = reg | BMx680_RW_BIT;
  
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_TXE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (ERROR);
    }
  }
  
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (ERROR);
    }
  }
  
  uint8_t* buf = dev->RawBufPtr;
  for (uint16_t i = 0; i < len; i++) {
    *buf++ = (uint8_t)dev->SPIx->DR;
  }

  PIN_H(dev->SPINssPort, dev->SPINssPin);
  if (SPI_Disable(dev->SPIx)) return (ERROR);

  return (SUCCESS);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
