/**
  ******************************************************************************
  * @file           : spi.h
  * @brief          : This file contains header definitions for SPI peripherals 
  *                   code
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
#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private defines -----------------------------------------------------------*/
#define SPI_BUS_TMOUT   10000 /* cycles timeout on SPI bus operations */
#define NSS_0_Pin       GPIO_PIN_4
#define NSS_0_Pin_Pos   GPIO_PIN_4_Pos
#define NSS_0_Pin_Mask  GPIO_PIN_4_Mask
#define NSS_1_Pin       GPIO_PIN_3
#define NSS_1_Pin_Pos   GPIO_PIN_3_Pos
#define NSS_1_Pin_Mask  GPIO_PIN_3_Mask
#define SCK_Pin         GPIO_PIN_5
#define SCK_Pin_Pos     GPIO_PIN_5_Pos
#define SCK_Pin_Mask    GPIO_PIN_5_Mask
#define MISO_Pin        GPIO_PIN_6
#define MISO_Pin_Pos    GPIO_PIN_6_Pos
#define MISO_Pin_Mask   GPIO_PIN_6_Mask
#define MOSI_Pin        GPIO_PIN_7
#define MOSI_Pin_Pos    GPIO_PIN_7_Pos
#define MOSI_Pin_Mask   GPIO_PIN_7_Mask
#define SPI_Port        GPIOA

#define NSS_0_H         PIN_H(SPI_Port, NSS_0_Pin_Pos)
#define NSS_0_L         PIN_L(SPI_Port, NSS_0_Pin_Pos)
#define NSS_1_H         PIN_H(SPI_Port, NSS_1_Pin_Pos)
#define NSS_1_L         PIN_L(SPI_Port, NSS_1_Pin_Pos)



/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  Initializes the given SPI bus.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval (int) status of operation
 */
int SPI_Init(SPI_TypeDef*);

/**
 * @brief  Enables the given SPI peripherals.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval (int) status of operation
 */
int SPI_Enable(SPI_TypeDef*);

/**
 * @brief  Disables the given SPI peripherals.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval (int) status of operation
 */
int SPI_Disable(SPI_TypeDef*);

int SPI_Read_8b(SPI_TypeDef*, uint8_t*, uint8_t);
int SPI_Write_8b(SPI_TypeDef*, uint8_t*, uint8_t);
int SPI_Read_16b(SPI_TypeDef*, uint16_t*, uint8_t);
int SPI_Write_16b(SPI_TypeDef*, uint16_t*, uint8_t);


#ifdef __cplusplus
}
#endif
#endif // __SPI_H
