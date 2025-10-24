/**
  ******************************************************************************
  * @file           : max7219.h
  * @brief          : This file contains header definitions MAX 7219 display
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


#ifndef __MAX7219_H
#define __MAX7219_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define MAX7219_SEG_CNT       4
#define MAX7219_MAX_SEG_CNT   32

/**
 * @brief   MAX7219 device type definition struct.
 */
typedef struct {
  uint8_t               SegCnt;
  uint8_t               MaxSegCnt;
  uint16_t*             BufPtr;
  uint16_t*             TmpBufPtr;
  uint8_t               Lock;
  SPI_TypeDef*          SPIx;
  DMA_TypeDef*          DMAx;
  DMA_Channel_TypeDef*  DMAxTx;
  DMA_Channel_TypeDef*  DMAxRx;
} Max7219_TypeDef;



int MAX7219_Init(Max7219_TypeDef*);
void MAX7219_Print(Max7219_TypeDef*, const char*);


#ifdef __cplusplus
}
#endif
#endif /* __MAX7219_H */

