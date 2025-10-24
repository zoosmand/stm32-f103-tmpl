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

/* Private function prototypes -----------------------------------------------*/
static int mAX7219_PrintBuf(Max7219_TypeDef* dev, uint16_t);
static void mAX7219_CompressBuf(Max7219_TypeDef* dev, uint16_t, uint8_t);

/**
 * @brief Adjusts SPI bus according to MAC7219 parameters.
 * @param   dev: pointer to the flash device struct
 * @retval  none
 */
__STATIC_INLINE void SPI_Adjust(Max7219_TypeDef*);






////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Transfesr SPI data via DMA
  * @param  cnt: Count of bytes to transfer
  * @param  dir: Direction - READ or WRITE EEPROM
  * @param  offset: Offset of a particular address of EEPROM
  * @retval None
  */
int MAX7219_Init(Max7219_TypeDef* dev) {
  if ((dev->SPIx == NULL) || (dev->DMAx == NULL) || (dev->DMAxRx == NULL) || (dev->DMAxTx == NULL)) return (1); 

  dev->Lock = 1;

  SPI_Adjust(dev);
  if (SPI_Enable(dev->SPIx)) return (1);
  
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
    segs = dev->SegCnt;
    NSS_1_L;
    while (segs-- > 0) {
      if (SPI_Write_16b(dev->SPIx, &maxInit[i], 1)) return (1);
    }
    NSS_1_H;
  }
  dev->Lock = 0;
  
  return (0);
}




__STATIC_INLINE void SPI_Adjust(Max7219_TypeDef* dev) {
  /* adjust frequency divider, 0b010 = 8, (PCLK)72/8 = 9MHz */
  /* set 16-bit data buffer length */ 
  MODIFY_REG(dev->SPIx->CR1, (SPI_CR1_BR_Msk, SPI_CR1_DFF_Msk), (SPI_CR1_BR_1 | SPI_CR1_DFF));
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_SSOE_Pos);
  dev->DMAxTx->CCR = 0UL;
  dev->DMAxRx->CCR = 0UL;
  /* Clear correspondents DMA flags */
  dev->DMAx->IFCR |= 0x00000ff0;
}





/*
 *
 */
static int mAX7219_PrintBuf(Max7219_TypeDef* dev, uint16_t len) {

  SPI_Adjust(dev);
  if (SPI_Enable(dev->SPIx)) return (1);

  for (uint8_t i = 0; i < 8; i++) {
    NSS_1_L;
    
    for (uint8_t k = 0; k < dev->SegCnt; k++) {
      if (SPI_Write_16b(dev->SPIx, &(dev->TmpBufPtr)[k + (len * i)], 1)) return (1);
    }
    
    NSS_1_H;
  }

  if (SPI_Disable(dev->SPIx)) return (1);
  return (0);
}




/*
 *
 */
void MAX7219_Print(Max7219_TypeDef* dev, const char* buf) {

  uint16_t len = 0;
  const char *buf_ = buf;

  while (*buf_++) len++;
  
  len = (len > dev->MaxSegCnt) ? dev->MaxSegCnt : len;

  for (uint16_t k = 0; k < len; k++) {
    
    uint8_t pos = buf[k];
    if ((pos > 126) || (pos < 32)) {
      if (pos == 176) pos = 95;
      else return;
    } else pos -= 32;
    
    for (uint8_t i = 0; i < 8; i++) {
      dev->TmpBufPtr[(k + (len * i))] = ((i + 1) << 8) | (uint8_t)font_dot_5x7_max[pos][i];
    }
  }

  /* TODO fix buffer overflow bug */
  // mAX7219_CompressBuf(dev, len, 2);
  if (mAX7219_PrintBuf(dev, len)) return;
}









/*
 *
 */
static void mAX7219_CompressBuf(Max7219_TypeDef* dev, uint16_t len, uint8_t step) {

  if (step > 3) return;
  uint16_t cur_val, next_val, cur_ptr, next_ptr;
  uint8_t step_tmp, step_cnt, step_offset;
  
  for (uint8_t i = 0; i < (len * 8); i++) {
    cur_val     = 0;
    next_val    = 0;
    step_tmp    = 0;
    step_cnt    = 0;
    step_offset = 0;
    cur_ptr     = 0;
    next_ptr    = 0;
    
    for (uint8_t k = 0; k < len; k++) {

      if ((k + 2) >= len) {
        dev->TmpBufPtr[k + 1] = (dev->TmpBufPtr[k + 1] & 0xff00);
        break;
      }

      if (step_tmp >= 8) {
        step_offset++;
        k--;
      } 

      step_tmp  = ((step_cnt++) & 0x3) * step + step;

      cur_ptr   = len * i;
      next_ptr  = (len * i) + 1 + step_offset;
      
      cur_val   = dev->TmpBufPtr[k + cur_ptr];
      next_val  = dev->TmpBufPtr[k + next_ptr];

      dev->TmpBufPtr[k + cur_ptr]                  = (cur_val & 0x00ff) | (((next_val & 0x00ff) << step_tmp) >> 8) | (cur_val & 0xff00);
      dev->TmpBufPtr[k + next_ptr - step_offset]   = ((next_val << step_tmp) & 0x00ff) | (next_val & 0xff00);

    }
  }
}