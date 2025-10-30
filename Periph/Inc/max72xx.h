/**
  ******************************************************************************
  * @file           : max72xx.h
  * @brief          : This file contains header definitions MAX 72xx display
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


#ifndef __MAX72XX_H
#define __MAX72XX_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define MAX72XX_SEG_CNT       4
#define MAX72XX_MAX_SEG_CNT   (MAX72XX_SEG_CNT * 8)

// /**
//  * @brief   MAX7219 device type definition struct.
//  */
// typedef struct {
//   uint8_t               SegCnt;
//   uint8_t               MaxSegCnt;
//   uint16_t*             BufPtr;
//   uint8_t               Lock;
//   SPI_TypeDef*          SPIx;
//   DMA_TypeDef*          DMAx;
//   DMA_Channel_TypeDef*  DMAxTx;
//   DMA_Channel_TypeDef*  DMAxRx;
// } Max72xx_TypeDef;


/**
 * @brief   Initializes the given MAX72xx device.
 * @param   dev: pointer to the MAX72xx device struct
 * @retval  (int) status of operation
 */
int MAX72xx_Init(Max72xx_TypeDef*);

/**
 * @brief   Prints the buffer from the given MAX72xx dev.
 * @param   dev: pointer to the MAX72xx device struct
 * @param   buf: chars buffer to print
 * @retval  none
 */
void MAX72xx_Print(Max72xx_TypeDef*, const char*);


#ifdef __cplusplus
}
#endif
#endif /* __MAX72XX_H */

