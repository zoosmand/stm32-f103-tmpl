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
static ErrorStatus bmx680_send(BMxX80_TypeDef*, uint8_t);

/**
  * @brief  Reads/receives data from a BMx680 device.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  reg: a register address or command to send to the device
  * @param  len: number of bytes to receive
  * @return status of operation
  */
static ErrorStatus bmx680_receive(BMxX80_TypeDef*, uint8_t, uint8_t);

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
  * @brief  Writes/sends data via SPI.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  len: number of bytes to send
  * @return status of operation
  */
static ErrorStatus spi_send(BMxX80_TypeDef*, uint16_t);

/**
  * @brief  Reads/receives data via SPI.
  * @param  dev: a Bocsh BMxX80 device structure pointer
  * @param  reg: a register address or command to send to the device
  * @param  len: number of bytes to receive
  * @return status of operation
  */
static ErrorStatus spi_receive(BMxX80_TypeDef*, uint8_t, uint16_t);

/**
  * @brief   Adjusts SPI bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE void spi_dma_configure(BMxX80_TypeDef*);

/**
  * @brief   Unconfigure SPI bus.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE ErrorStatus spi_dma_unconfigure(BMxX80_TypeDef*);





////////////////////////////////////////////////////////////////////////////////





// ----------------------------------------------------------------------------

ErrorStatus BMx680_Init(BMxX80_TypeDef* dev) {
  /* Lock the device */
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
  if (bmx680_receive(dev, BMx680_dev_id, 1)) return (ERROR);
  
  if (dev->RawBufPtr[0] != BME680_ID) return (ERROR);
  
  /* Get par_t1 calibration value */
  if (bmx680_receive(dev, BMx680_calib1, 20)) return (ERROR);
  // par_t1 = *(int16_t*)dev->RawBufPtr;

  // /* Get par_t2 calibration value */
  // if (bmx680_receive(dev, BMx680_par_t2, 2)) return (ERROR);
  // par_t2 = *(int16_t*)dev->RawBufPtr;

  // /* Get par_t3 calibration value */
  // if (bmx680_receive(dev, BMx680_par_t3, 1)) return (ERROR);
  // par_t3 = *(int8_t*)dev->RawBufPtr;


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

static ErrorStatus bmx680_receive(BMxX80_TypeDef *dev, uint8_t reg, uint8_t len) {

  if (dev->I2Cx != NULL) {
    if (i2c_receive(dev, reg, len)) return (ERROR);
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {
    if (spi_receive(dev, reg, len)) return(spi_dma_unconfigure(dev));
    return (SUCCESS);
  } 
  return (ERROR);
}



// ----------------------------------------------------------------------------

static ErrorStatus bmx680_send(BMxX80_TypeDef *dev, uint8_t len) {
  
  if (dev->I2Cx != NULL) {
    if (i2c_send(dev, len)) return (spi_dma_unconfigure(dev));
    return (SUCCESS);
  } 

  if (dev->SPIx != NULL) {
    return (SUCCESS);
  } 
  return (ERROR);
}





// ----------------------------------------------------------------------------

static ErrorStatus i2c_send(BMxX80_TypeDef* dev, uint16_t len) {
  
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, dev->RawBufPtr, len)) return (ERROR);
  
  return (SUCCESS);
}





// ----------------------------------------------------------------------------

static ErrorStatus i2c_receive(BMxX80_TypeDef* dev, uint8_t reg, uint16_t len) {
  
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, &reg, 1)) return (ERROR);
  
  I2C_Master_Receive(dev->I2Cx, dev->I2C_Address, dev->RawBufPtr, len);
  
  return (SUCCESS);
}




// ----------------------------------------------------------------------------

static ErrorStatus spi_send(BMxX80_TypeDef *dev, uint16_t len) {

  return (SUCCESS);
}




// ----------------------------------------------------------------------------

__STATIC_INLINE void spi_dma_configure(BMxX80_TypeDef* dev) {
  
  // uint8_t pump = 0;
  
  /* adjust frequency divider, 0b010 = 8, (PCLK)72/8 = 9MHz */
  /* set 8-bit data buffer length */ 
  MODIFY_REG(dev->SPIx->CR1, (SPI_CR1_BR_Msk | SPI_CR1_DFF_Msk), (SPI_CR1_BR_1 | SPI_CR1_DFF));
  PREG_SET(dev->SPIx->CR2, SPI_CR2_SSOE_Pos);
  
  /* configure DMA, Channel2 - RX */
  /* set priority high*/
  /* set memory to increment */
  MODIFY_REG(dev->DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC));
  
  /* set peripheral address */
  dev->DMAxRx->CPAR = (uint32_t)&dev->SPIx->DR;
  /* set memory address */
  dev->DMAxRx->CMAR = (uint32_t)dev->RawBufPtr;
  
  /* configure DMA, Channel3 - TX */
  /* set priority high*/
  /* set memory to increment */
  /* set direction from memory to peripheral */
  MODIFY_REG(dev->DMAxTx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR));
  
  /* set peripheral address */
  dev->DMAxTx->CPAR = (uint32_t)&dev->SPIx->DR;
  /* set memory address */
  dev->DMAxTx->CMAR = (uint32_t)dev->RawBufPtr;
  
}




// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus spi_dma_unconfigure(BMxX80_TypeDef* dev) {

  /* Clear correspondents DMA flags */
  dev->DMAx->IFCR |= (
      DMA_IFCR_CGIF2_Msk
    | DMA_IFCR_CGIF3_Msk
    | DMA_IFCR_CHTIF2_Msk
    | DMA_IFCR_CHTIF3_Msk
    | DMA_IFCR_CTCIF2_Msk
    | DMA_IFCR_CTCIF3_Msk
  );
  
  /* Disable from memory to peripheral DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
  /* Disable from peripheral to memory DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);

  PIN_H(dev->SPINssPort, dev->SPINssPin);
  SPI_Disable(dev->SPIx);

  /* Clear DMA registers */
  dev->DMAxTx->CCR    = 0UL;
  dev->DMAxTx->CMAR   = 0UL;
  dev->DMAxTx->CPAR   = 0UL;
  dev->DMAxTx->CNDTR  = 0UL;

  dev->DMAxRx->CCR    = 0UL;
  dev->DMAxRx->CMAR   = 0UL;
  dev->DMAxRx->CPAR   = 0UL;
  dev->DMAxRx->CNDTR  = 0UL;

  return (ERROR);
}




// ----------------------------------------------------------------------------

static ErrorStatus spi_receive(BMxX80_TypeDef *dev, uint8_t reg, uint16_t len) {
  uint32_t tmout;
  uint8_t pump = 0;   /* The pump variable to complete DMA configuration */
  
  spi_dma_configure(dev);
  if (SPI_Enable(dev->SPIx)) return (spi_dma_unconfigure(dev));
  
  /* run the bus */
  PIN_L(dev->SPINssPort, dev->SPINssPin);
  
  /* send a command (register address) to the device */
  *(__IO uint8_t*)&dev->SPIx->DR = reg | BMx680_RW_BIT;
  
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_TXE_Pos))) { if (!(--tmout)) return (spi_dma_unconfigure(dev)); }
  
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) { if (!(--tmout)) return (spi_dma_unconfigure(dev)); }
  
  /* AN alternative way to read/receive data from the bus */
  // SPI_Read_8b(dev->SPIx, dev->RawBufPtr, len);
  // PIN_H(dev->SPINssPort, dev->SPINssPin);


  /* --- Read/receive data from the bus via DMA --- */

  /* Enable from memory to peripheral DMA transfer */
  PREG_SET(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
  /* Enable from peripheral to memory DMA transfer */
  PREG_SET(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);

  /* Disable incremental and wait until it clears, due to here only nulls to be sent to the bus */
  PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos);
  tmout = SPI_BUS_TMOUT;
  while((PREG_CHECK(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos))) { if (!(--tmout)) return (spi_dma_unconfigure(dev)); }

  /* Set buffer address to the null pump */
  dev->DMAxTx->CMAR = (uint32_t)&pump;

  /* Set counter */
  dev->DMAxRx->CNDTR = len;
  dev->DMAxTx->CNDTR = len;
  
  /* Enable transfer from memory to peripheral */
  PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  /* Enable transfer from peripheral to memory */
  PREG_SET(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);

  /* Wait for transfer TX (memory to peripheral) to be competed */
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF3_Pos))) { if (!(--tmout)) return (spi_dma_unconfigure(dev)); }

  /* Clear complete transger flag */
  SET_BIT(dev->DMAx->IFCR, DMA_IFCR_CTCIF3);

  tmout = SPI_BUS_TMOUT;
  while((PREG_CHECK(dev->SPIx->SR, SPI_SR_BSY_Pos))) { if (!(--tmout)) return (spi_dma_unconfigure(dev)); }

  /* stop the bus */
  spi_dma_unconfigure(dev);

  return (SUCCESS);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
