/**
  ******************************************************************************
  * @file           : display.c
  * @brief          : This file contains display code
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
#include "whxxxx.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define _WR1NCHAR(ch) ((ch & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_E)|(1<<WHxxxx_Bl)|(1<<WHxxxx_Rs))
#define _WR2NCHAR(ch) ((ch & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_Bl)|(1<<WHxxxx_Rs))
#define _WR1NCMD(cmd) ((cmd & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_E)|(1<<WHxxxx_Bl))
#define _WR2NCMD(cmd) ((cmd & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_Bl))


/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static __attribute__((section(".cron"))) uint32_t _DSPLREG_ = 0;
static uint16_t diplPrintPos = 0;
static const uint16_t initParams[16] = {
  WHxxxx_8BIT_BUS,      4100,
  WHxxxx_8BIT_BUS,      100,
  WHxxxx_8BIT_BUS,      WHxxxx_SD,
  WHxxxx_CUR_UPLEFT,    WHxxxx_LD,
  WHxxxx_4BIT_BUS_1L,   WHxxxx_SD,
  WHxxxx_DSPL_SW,       WHxxxx_SD,
  WHxxxx_CLR_DSLP,      WHxxxx_LD,
  WHxxxx_POS_1LS,       WHxxxx_SD
}; 


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief   Adjusts I2C bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE void i2c_dma_configure(WHxxxx_TypeDef*);

/**
  * @brief   Adjusts I2C bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE ErrorStatus i2c_dma_unconfigure(WHxxxx_TypeDef*);

/**
  * @brief  Transmits in master mode an amount of data.
  * @param  dev Pointer to the target device structure
  * @param  buf Pointer to data buffer
  * @param  len Amount of data to be sent
  * @return transmit status
  */
__STATIC_INLINE ErrorStatus i2c_master_send(WHxxxx_TypeDef*, uint8_t*, uint16_t);

/**
 * @brief  Writes/Sends a charachter symbol to the WHxxxx display.
 * @param  dev: pointer to the particular device structure
 * @param  ch: ACSII charachter
 * @retval status of operation
 */
static ErrorStatus whxxxx_write_char(WHxxxx_TypeDef*, uint8_t);

/**
 * @brief  Writes/Sends a command to the WHxxxx display.
 * @param  dev: pointer to the particular device structure
 * @param  cmd: WH display command
 * @param  delay: command delay according documentation
 * @retval status of operation
 */
static ErrorStatus whxxxx_write_command(WHxxxx_TypeDef*, uint8_t, uint32_t);




////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

__STATIC_INLINE void i2c_dma_configure(WHxxxx_TypeDef* dev) {

  /* Disable DMA transfer */
  PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  /* Clear ACK bnit */
  PREG_CLR(dev->I2Cx->CR1, I2C_CR1_ACK_Pos);
  /* configure DMA, Channel6 - TX */
  /* set priority high*/
  /* set memory to increment */
  /* set dir bit, means reading from the bus */
  MODIFY_REG(dev->DMAxTx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR));
  
  /* set peripheral address */
  dev->DMAxTx->CPAR = (uint32_t)&dev->I2Cx->DR;
  
}


// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus i2c_dma_unconfigure(WHxxxx_TypeDef* dev) {
  
  /* Clear correspondent DMA interrupt flags */
  dev->DMAx->IFCR = (DMA_IFCR_CHTIF6_Msk | DMA_IFCR_CGIF6_Msk | DMA_IFCR_CTCIF6_Msk);
  /* Clear DMA configuration */
  dev->DMAxTx->CPAR   = 0UL;
  dev->DMAxTx->CMAR   = 0UL;
  dev->DMAxTx->CNDTR  = 0UL;
  dev->DMAxTx->CCR    = 0UL;
  
  /* Disable DMA transfer on the bus */
  PREG_CLR(dev->I2Cx->CR2, I2C_CR2_DMAEN);
  
  /* Disable I2C bus */
  I2C_Stop(dev->I2Cx);
  
  return (ERROR);
}


// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus i2c_master_send(WHxxxx_TypeDef* dev, uint8_t *buf, uint16_t len) {
  
  uint32_t tmout;

  /* Adjust I2C bus with DMA transfer */
  i2c_dma_configure(dev);
  
  /* Start bus transmission */
  if (I2C_Start(dev->I2Cx)) { return (i2c_dma_unconfigure(dev)); }
  if (I2C_SendAddress(dev->I2Cx, dev->I2C_Address, TX)) { return (i2c_dma_unconfigure(dev)); }

  /* Set counter */
  dev->DMAxTx->CNDTR = len;
  /* Set buffer pointer address */
  dev->DMAxTx->CMAR = (uint32_t)buf;
  /* Enable receiving DMA transfer */
  PREG_SET(dev->I2Cx->CR2, I2C_CR2_DMAEN_Pos);
  /* Enable DMS transfer*/
  PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  
  /* Wait for transfer is complete */
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF6_Pos))) {
    if (!(--tmout)) return (i2c_dma_unconfigure(dev));
  }

  /* Verify after transferring if transmition is finished */
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(dev->I2Cx->SR1, I2C_SR1_BTF_Pos))) {
    if (!(--tmout)) { return (i2c_dma_unconfigure(dev)); }
  }

  /* Unconfigure DMA and stop the I2C bus */
  i2c_dma_unconfigure(dev);
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

ErrorStatus WHxxxx_Init(WHxxxx_TypeDef* dev) {
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  /* Initial delay according WHxxxx documentation */
  _delay_ms(40);
 
  /* Initialize the device wuth parameter/delay pairs */
  uint8_t bytes[4];
  for(uint8_t i = 0; i < (sizeof(initParams) / 2); (i += 2)) {

    bytes[0] = _WR1NCMD((uint8_t)initParams[i]);
    bytes[1] = _WR2NCMD((uint8_t)initParams[i]);
    bytes[2] = _WR1NCMD(((uint8_t)initParams[i] << 4));
    bytes[3] = _WR2NCMD(((uint8_t)initParams[i] << 4));
    
    // if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, bytes, 4)) return (ERROR);
    if (i2c_master_send(dev, bytes, 4)) return (ERROR);
    _delay_us(initParams[i + 1]);
  }
  dev->Lock = DISABLE;  
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

static ErrorStatus whxxxx_write_char(WHxxxx_TypeDef* dev, uint8_t ch) {
  
  uint8_t bytes[4];
  
  bytes[0] = _WR1NCHAR(ch);
  bytes[1] = _WR2NCHAR(ch);
  bytes[2] = _WR1NCHAR(ch << 4);
  bytes[3] = _WR2NCHAR(ch << 4);
  
  // if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, bytes, 4)) return (ERROR);
  if (i2c_master_send(dev, bytes, 4)) return (ERROR);
  
  _delay_us(WHxxxx_SD);
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

static ErrorStatus whxxxx_write_command(WHxxxx_TypeDef* dev, uint8_t cmd, uint32_t delay) {  
  
  uint8_t bytes[4];
  
  bytes[0] = _WR1NCMD(cmd);
  bytes[1] = _WR2NCMD(cmd);
  bytes[2] = _WR1NCMD(cmd << 4);
  bytes[3] = _WR2NCMD(cmd << 4);
  
  // if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, bytes, 4)) return (ERROR);
  if (i2c_master_send(dev, bytes, 4)) return (ERROR);
  
  _delay_us(delay);
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

ErrorStatus WHxxxx_Print(WHxxxx_TypeDef* dev, uint8_t* buf, uint8_t len) {
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  if (whxxxx_write_command(dev, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (ERROR);
  for (uint8_t i = 0; i < len; i++) {
    if (whxxxx_write_char(dev, buf[i])) return (ERROR);
  }

  dev->Lock = DISABLE;
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

int __attribute__((weak)) putc_dspl_wh(char ch) {
  WHxxxx_TypeDef* dev = Get_WhDiplayDevice(WH_DSPL_MODEL);
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    if (whxxxx_write_command(dev, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (ERROR);
    if (whxxxx_write_command(dev, WHxxxx_POS_1LS, WHxxxx_SD)) return (ERROR);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    if (diplPrintPos > 15) {
      if (whxxxx_write_command(dev, WHxxxx_POS_2LS, WHxxxx_SD)) return (1);
      diplPrintPos = 0;
    }
    if (whxxxx_write_char(dev, ch)) return (ERROR);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);

  dev->Lock = DISABLE;
  return (SUCCESS);
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
