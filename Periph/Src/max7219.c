/**
  ******************************************************************************
  * @file           : max7219.c
  * @brief          : This file contains MAX 7219 display
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
#include "max7219.h"

/* Private variables ---------------------------------------------------------*/
__IO static uint16_t maxBuf[(MAX7219_SEG_CNT * 8)];
__IO static uint16_t tmpBuf[(MAX7219_MAX_SEG_CNT * 8)];
static SPI_TypeDef* SPI_Instance;

/* Private function prototypes -----------------------------------------------*/
// static int mAX7219_WriteByte(const uint8_t, const uint8_t);
static int mAX7219_PrintBuf(const uint16_t*, uint16_t);
static void mAX7219_CompressBuf(uint16_t*, uint16_t, uint8_t);
__STATIC_INLINE void SPI_Adjust(SPI_TypeDef*, DMA_Channel_TypeDef*, DMA_Channel_TypeDef*);







////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Transfesr SPI data via DMA
  * @param  cnt: Count of bytes to transfer
  * @param  dir: Direction - READ or WRITE EEPROM
  * @param  offset: Offset of a particular address of EEPROM
  * @retval None
  */
int MAX7219_Init(SPI_TypeDef* SPIx) {

  SPI_Instance = SPIx;
  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);

  if (SPI_Enable(SPIx)) return (1);
  NSS_1_H;
  
  _delay_ms(5);
  
  const uint16_t maxInit[15] = {
    0x0c00, // 0x0c - Shutdown,     0x00 - Shutdown
    0x0f00, // 0x0f - DisplayTest,  0x00 - Off, 0x01 - On
    0x0900, // 0x09 - Decode Mode,  0x00 - No decode for digits 7�0, 0xff - Code B decode for digits 7�0
    0x0a04, // 0x0a - Intensity,    0x00 - lowest, 0x0f - highest
    0x0b07, // 0x0b - Scan-Limit,   0x07 - Display digits 0 1 2 3 4 5 6 7
    0x0100, // 0x01 - Line 1
    0x0200,
    0x0300,
    0x0400,
    0x0500,
    0x0600,
    0x0700,
    0x0800, // 0x08 - Line 8
    0x0c01, // 0x0c - Run,          0x01 - Normal operation 
    0x0000  // 0x00 - NOP
  };
  
  uint8_t segs = 0;
  
  for (uint8_t i = 0; i < sizeof(maxInit)/2; i++) {
    segs = MAX7219_SEG_CNT;
    NSS_1_L;
    while (segs-- > 0) {
      if (SPI_Write_16b(SPI_Instance, &maxInit[i], 1)) return (1);
      // if (mAX7219_WriteByte(SPI1, maxInit[i])) return (1);
    }
    NSS_1_H;
  }
  
  return (0);
}




__STATIC_INLINE void SPI_Adjust(SPI_TypeDef* SPIx, DMA_Channel_TypeDef* DMAxTx, DMA_Channel_TypeDef* DMAxRx) {
  /* adjust frequency divider, 0b010 = 8, (PCLK)72/8 = 9MHz */
  /* set 16-bit data buffer length */ 
  MODIFY_REG(SPIx->CR1, (SPI_CR1_BR_Msk, SPI_CR1_DFF_Msk), (SPI_CR1_BR_1 | SPI_CR1_DFF));
  DMAxTx->CCR = 0UL;
  DMAxRx->CCR = 0UL;
}



// /*
//  *
//  */
// static int mAX7219_WriteByte(const uint8_t line, const uint8_t byte) {

//   if (line > 8) return (1);

//   __IO uint16_t data = ((line << 8) | byte) & 0xffff;
//   if (SPI_Write_16b(SPI_Instance, &data, 1)) return (1);

//   return (0);
// }




/*
 *
 */
static int mAX7219_PrintBuf(const uint16_t* buf, uint16_t len) {

  SPI_Adjust(SPI_Instance, DMA1_Channel2, DMA1_Channel3);
  if (SPI_Enable(SPI_Instance)) return (1);

  for (uint8_t i = 0; i < 8; i++) {
    NSS_1_L;
    
    for (uint8_t k = 0; k < MAX7219_SEG_CNT; k++) {
      if (SPI_Write_16b(SPI_Instance, &buf[k + (len * i)], 1)) return (1);
    }
    
    NSS_1_H;
  }

  if (SPI_Disable(SPI_Instance)) return (1);
  return (0);
}




/*
 *
 */
void MAX7219_Print(const char* buf) {

  uint16_t len = 0;
  const char *buf_ = buf;

  while (*buf_++) len++;
  
  len = (len > MAX7219_MAX_SEG_CNT) ? MAX7219_MAX_SEG_CNT : len;

  for (uint16_t k = 0; k < len; k++) {
    
    uint8_t pos = buf[k];
    if ((pos > 126) || (pos < 32)) {
      if (pos == 176) pos = 95;
      else return;
    } else pos -= 32;
    
    for (uint8_t i = 0; i < 8; i++) {
      tmpBuf[(k + (len * i))] = ((i + 1) << 8) | (uint8_t)font_dot_5x7_max[pos][i];
    }
  }

  // mAX7219_CompressBuf(tmpBuf, len, 2);
  if (mAX7219_PrintBuf(tmpBuf, len)) return;
}









/*
 *
 */

static void mAX7219_CompressBuf(uint16_t* buf, uint16_t len, uint8_t step) {

  uint16_t curr = 0, next = 0, val = 0, val2 = 0;
  uint8_t _step = 0;
  
  for (uint8_t i = 0; i < (len * 8); i++) {
    
    for (uint8_t k = 0; k < len; k++) {
      if ((k + 2) >= len) break;
      
      curr = buf[k + (len * i)];
      next = buf[k + (len * i) + 1];

      _step = k * step + step;
      
      val = (curr & 0x00ff) | (((next & 0x00ff) << _step) >> 8) | (curr & 0xff00);
      val2 = ((next << _step) & 0x00ff) | (next & 0xff00);
      buf[k + (len * i)] = val;
      buf[k + (len * i) + 1] = val2;
    }
  }
}