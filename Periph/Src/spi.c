/**
  ******************************************************************************
  * @file           : spi.c
  * @brief          : This file contains SPI peripherals code
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
#include "spi.h"





/**
 * @brief  Initialize the given SPI bus.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval (int) status of operation
 */
int SPI_Init(SPI_TypeDef* SPIx) {
  // uint8_t pump = 0;


  /* Enable GPIO SCK, MISO, MOSI alternative on high speed */

  if (SPIx == SPI1) {
    MODIFY_REG(SPI_Port->CRL,
      (NSS_0_Pin_Mask | SCK_Pin_Mask | MISO_Pin_Mask | MOSI_Pin_Mask | NSS_1_Pin_Mask), (
        ((GPIO_AF_PP | GPIO_IOS_10) << (SCK_Pin_Pos * 4U))
      | ((GPIO_AF_PP | GPIO_IOS_10) << (MISO_Pin_Pos * 4U))
      | ((GPIO_AF_PP | GPIO_IOS_10) << (MOSI_Pin_Pos * 4U))
      | ((GPIO_GPO_PP | GPIO_IOS_2) << (NSS_0_Pin_Pos * 4U))
      | ((GPIO_GPO_PP | GPIO_IOS_2) << (NSS_1_Pin_Pos * 4U))
    ));
  }

  /* set ready NSS pins for the multimaster mode */
  NSS_0_H;
  NSS_1_H;

  /* Enbale SPI master mode */
  SET_BIT(SPIx->CR1, SPI_CR1_MSTR);

  /* Configure DMA, Channel2 - RX, Channel3 - TX */
  /* Set priority high*/
  /* Set memory to increment */
  // DMA1_Channel2->CCR = (DMA_CCR_PL_1 | DMA_CCR_MINC);
  // /* Set buffer size to 0 */
  // DMA1_Channel2->CNDTR = 0UL;
  // /* Set peripheral address */
  // DMA1_Channel2->CPAR = (uint32_t)&SPIx->DR;
  // /* Set memory address */
  // DMA1_Channel2->CMAR = (uint32_t)&pump;
  
  // /* Set priority high*/
  // /* Set memory to increment */
  // /* Set direction from memory to peripheral */
  // DMA1_Channel3->CCR = (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR);
  // /* Set buffer size to 0 */
  // DMA1_Channel3->CNDTR = 0UL;
  // /* Set peripheral address */
  // DMA1_Channel3->CPAR = (uint32_t)&SPIx->DR;
  // /* Set memory address */
  // DMA1_Channel3->CMAR = (uint32_t)&pump;
  
  if (SPIx == SPI1) {
    // NVIC_SetPriority(SPI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
    // NVIC_EnableIRQ(SPI1_IRQn);
    return (0);
  }

  return (1);
}






int SPI_Enable(SPI_TypeDef* SPIx) {

  uint32_t tmout = SPI_BUS_TMOUT;

  while(PREG_CHECK(SPIx->SR, SPI_SR_BSY_Pos)) {
    if (!(--tmout)) {
      SPI_Disable(SPIx);
      return (1);
    }
  }

  // if (dataBufLen == SPIBufLen_16bit) {
  //   PREG_SET(SPIx->CR1, SPI_CR1_DFF_Pos); // set 16-bit buffer length, otherwise 8-bit
  // } else {
  //   PREG_CLR(SPIx->CR1, SPI_CR1_DFF_Pos); // otherwise set buffer to 8-bit
  // }
  
  PREG_SET(SPIx->CR1, SPI_CR1_SPE_Pos);
  return (0);
}





int SPI_Disable(SPI_TypeDef* SPIx) {
  uint32_t tmout = SPI_BUS_TMOUT;
  
  while(PREG_CHECK(SPIx->SR, SPI_SR_BSY_Pos)) {
    if (!(--tmout)) {
      return (1);
    }
  }

  PREG_CLR(SPIx->CR1, SPI_CR1_SPE_Pos);
  return (0);
}





/**
  * @brief  Reads data from SPI bus with 8-bit data buffer length
  * @param  buf: pointer to buffer to read, the first item of buffer could contain
  *              a command data. Beginning iteration reads a dummy byte.
  *         cnt: count of bytes to read.
  * @retval none
  */
int SPI_Read_8b(SPI_TypeDef* SPIx, uint8_t *buf, uint8_t cnt) {
  uint32_t tmout = 0;

  while (cnt--) {
    *(__IO uint8_t*)&SPIx->DR = 0;


    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    *buf++ = (uint8_t)SPIx->DR;
  }

  return (0);
}






/**
  * @brief  Writes data into SPI bus with 8-bit data buffer length
  * @param  buf: pointer to buffer to write.
  *         cnt: count of bytes to write.
  * @retval none
  */
int SPI_Write_8b(SPI_TypeDef* SPIx, const uint8_t *buf, uint8_t cnt) {
  uint32_t tmout = 0;

  while (cnt--) {
    *(__IO uint8_t*)&SPIx->DR = *buf++;

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    SPIx->DR;
  }

  return (0);
}




/**
  * @brief  Reads data from SPI bus with 16-bit data buffer length
  * @param  buf: pointer to buffer to read, the first item of buffer could contain
  *              a command data. Beginning iteration reads a dummy byte.
  *         cnt: count of bytes to read.
  * @retval none
  */
int SPI_Read_16b(SPI_TypeDef* SPIx, uint16_t *buf, uint8_t cnt) {
  uint32_t tmout = 0;

  while (cnt--) {
    *(__IO uint16_t*)&SPIx->DR = 0;


    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    *buf++ = (uint16_t)SPIx->DR;
  }

  return (0);
}






/**
  * @brief  Writes data into SPI bus with 8-bit data buffer length
  * @param  buf: pointer to buffer to write.
  *         cnt: count of bytes to write.
  * @retval none
  */
int SPI_Write_16b(SPI_TypeDef* SPIx, const uint16_t *buf, uint8_t cnt) {
  uint32_t tmout = 0;

  while (cnt--) {
    *(__IO uint16_t*)&SPIx->DR = *buf++;

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
      if (!(--tmout)) {
        return (1);
      }
    }

    SPIx->DR;
  }

  return (0);
}
