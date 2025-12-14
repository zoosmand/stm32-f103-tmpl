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

#define SPI1_NSS_0_H         PIN_H(SPI1_Port, SPI1_NSS_0_Pin)
#define SPI1_NSS_0_L         PIN_L(SPI1_Port, SPI1_NSS_0_Pin)



/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  Initializes the given SPI bus.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval status of operation
 */
ErrorStatus SPI_Init(SPI_TypeDef*);

/**
 * @brief  Enables the given SPI peripherals.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval status of operation
 */
ErrorStatus SPI_Enable(SPI_TypeDef*);

/**
 * @brief  Disables the given SPI peripherals.
 * @param  SPIx: pointer to the given SPI peripherals
 * @retval status of operation
 */
ErrorStatus SPI_Disable(SPI_TypeDef*);

/**
  * @brief  Reads data from SPI bus with 8-bit data buffer size.
  * @param  SPIx: pointer to the dedicatred SPI bus structure
  * @param  buf: pointer to the buffer to store the read data
  * @param  cnt: count of bytes to read from the bus
  * @retval status of aperation
  */
ErrorStatus SPI_Read_8b(SPI_TypeDef*, uint8_t*, uint16_t);

/**
  * @brief  Writes data to the SPI bus with 8-bit data buffer size.
  * @param  SPIx: pointer to the dedicatred SPI bus structure
  * @param  buf: pointer to the buffer of transmitting data
  * @param  cnt: count of bytes to write to the bus
  * @retval status of aperation
  */
ErrorStatus SPI_Write_8b(SPI_TypeDef*, uint8_t*, uint16_t);


/**
  * @brief  Reads data from SPI bus with 16-bit data buffer size.
  * @param  SPIx: pointer to the dedicatred SPI bus structure
  * @param  buf: pointer to the buffer to store the read data
  * @param  cnt: count of halfwords to read from the bus
  * @retval none
  */
ErrorStatus SPI_Read_16b(SPI_TypeDef*, uint16_t*, uint16_t);

/**
  * @brief  Writes data to the SPI bus with 16-bit data buffer size.
  * @param  SPIx: pointer to the dedicatred SPI bus structure
  * @param  buf: pointer to the buffer of transmitting data
  * @param  cnt: count of halfwords to write to the bus
  * @retval status of aperation
  */
ErrorStatus SPI_Write_16b(SPI_TypeDef*, uint16_t*, uint16_t);


#ifdef __cplusplus
}
#endif
#endif // __SPI_H
