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

/* Locaal variables ----------------------------------------------------------*/
static int16_t par_t1 = 0;
static int16_t par_t2 = 0;
static int8_t par_t3 = 0;

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

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

  /* adjust frequency divider, 0b010 = 8, (PCLK)72/8 = 9MHz */
  /* set 8-bit data buffer length */ 
  MODIFY_REG(dev->SPIx->CR1, (SPI_CR1_BR_Msk | SPI_CR1_DFF_Msk), (SPI_CR1_BR_1 | SPI_CR1_DFF));
  PREG_SET(dev->SPIx->CR2, SPI_CR2_SSOE_Pos);


  uint8_t pump = 0;
  /* configure DMA, Channel2 - RX */
  /* set priority high*/
  /* set memory to increment */
  MODIFY_REG(dev->DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC));
  
  /* set buffer size to 0 */
  dev->DMAxRx->CNDTR = 0UL;
  /* set peripheral address */
  dev->DMAxRx->CPAR = (uint32_t)&dev->SPIx->DR;
  /* set memory address */
  dev->DMAxRx->CMAR = (uint32_t)&pump;
  
  /* configure DMA, Channel3 - TX */
  /* set priority high*/
  /* set memory to increment */
  /* set direction from memory to peripheral */
  MODIFY_REG(dev->DMAxTx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR));

  /* set buffer size to 0 */
  dev->DMAxTx->CNDTR = 0UL;
  /* set peripheral address */
  dev->DMAxTx->CPAR = (uint32_t)&dev->SPIx->DR;
  /* set memory address */
  dev->DMAxTx->CMAR = (uint32_t)&pump;

  // dev->DMAxTx->CCR = 0UL;
  // dev->DMAxRx->CCR = 0UL;
  // /* Clear correspondents DMA flags */
  // dev->DMAx->IFCR |= 0x00000ff0;

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
  BMx680_Read(dev, BMx680_dev_id, 1);
  
  if (dev->RawBufPtr[0] != BME680_ID) return (ERROR);
  
  /* Get par_t1 calibration value */
  BMx680_Read(dev, BMx680_par_t1, 3);
  par_t1 = *(int16_t*)dev->RawBufPtr;

  /* Get par_t2 calibration value */
  BMx680_Read(dev, BMx680_par_t2, 2);
  par_t2 = *(int16_t*)dev->RawBufPtr;

  /* Get par_t3 calibration value */
  BMx680_Read(dev, BMx680_par_t2, 1);
  par_t3 = *(int8_t*)dev->RawBufPtr;


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
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {
    if (SPI_Read(dev, reg, len)) return(ERROR);
    return (SUCCESS);
  } 
  return (ERROR);
}



// ----------------------------------------------------------------------------

static ErrorStatus BMx680_Write(BMxX80_TypeDef *dev, uint8_t len) {
  if (dev->I2Cx != NULL) {
    if (I2C_Write(dev, len)) return (ERROR);
    return (SUCCESS);
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
  uint8_t pump = 0;

  
  SPI_Adjust(dev);
  if (SPI_Enable(dev->SPIx)) return (ERROR);
  
  /* run the bus */
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
  
  /* Read data from the bus */
  // SPI_Read_8b(dev->SPIx, dev->RawBufPtr, len);


    
  /* --- Read data from the bus via DMA --- */

  /* Enable from memory to peripheral DMA transfer */
  PREG_SET(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
  /* Enable from peripheral to memory DMA transfer */
  PREG_SET(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);
  /* Disable incremental and wait until it clears */
  PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos);
  
  tmout = SPI_BUS_TMOUT;
  while((PREG_CHECK(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (1);
    }
  }

  /* Set buffer address */
  dev->DMAxRx->CMAR = (uint32_t)dev->RawBufPtr;
  dev->DMAxTx->CMAR = (uint32_t)&pump;
  /* Set counter */
  dev->DMAxRx->CNDTR = len;
  dev->DMAxTx->CNDTR = len;
  __NOP();
  /* Enable transfer from memory to peripheral */
  PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  /* Enable transfer from peripheral to memory */
  PREG_SET(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);

  /* Wait for transfer is compete */
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF3_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (1);
    }
  }

  /* Clear complete transger flag */
  SET_BIT(dev->DMAx->IFCR, DMA_IFCR_CTCIF3);

  tmout = SPI_BUS_TMOUT;
  while((PREG_CHECK(dev->SPIx->SR, SPI_SR_BSY_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (1);
    }
  }

  /* Disable from memory to peripheral DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
  /* Disable from peripheral to memory DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);
  /* Disable transfer from peripheral to memory */
  PREG_CLR(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);
  /* Disable transfer from memory to peripheral */
  PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);

  /* Clear correspondent DMA flags */
  dev->DMAx->IFCR |= 0x00000ff0;

  /* stop the bus */
  PIN_H(dev->SPINssPort, dev->SPINssPin);
  if (SPI_Disable(dev->SPIx)) return (ERROR);

  return (SUCCESS);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
