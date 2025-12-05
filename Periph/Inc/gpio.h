/**
  ******************************************************************************
  * @file           : gpio.h
  * @brief          : This file contains header definitions for the code that
  *                   uses standard GPIO pin functions.
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


#ifndef __GPIO_INIT_H
#define __GPIO_INIT_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/
/* GPIO definitions for LED indications */
#define HEARTBEAT_LED_Pin     GPIO_PIN_13
#define HEARTBEAT_LED_Port    GPIOC

/* GPIO definitions for TM display */
#define TM_DIO_Pin            GPIO_PIN_12
#define TM_DIO_Pin_Pos        GPIO_PIN_12_Pos
#define TM_DIO_Pin_Mask       GPIO_PIN_12_Mask
#define TM_DIO_Port           GPIOA

#define TM_SCK_Pin            GPIO_PIN_11
#define TM_SCK_Pin_Pos        GPIO_PIN_11_Pos
#define TM_SCK_Pin_Mask       GPIO_PIN_11_Mask
#define TM_SCK_Port           GPIOA


/* GPIO definitions for OneWire bus */
#define OneWire_PORT          GPIOB
#define OneWire_PIN           GPIO_PIN_12


/* GPIO definition for I2C1 bus */
#define I2C1_SCL_Pin          GPIO_PIN_6
#define I2C1_SDA_Pin          GPIO_PIN_7
#define I2C1_Port             GPIOB


/* GPIO definition for SPI1 bus */
#define SPI1_NSS_0_Port       GPIOA
#define SPI1_NSS_0_Pin        GPIO_PIN_4
#define SPI1_NSS_1_Port       GPIOA
#define SPI1_NSS_1_Pin        GPIO_PIN_3
#define SPI1_NSS_2_Port       GPIOA
#define SPI1_NSS_2_Pin        GPIO_PIN_2

#define SPI1_SCK_Pin          GPIO_PIN_5
#define SPI1_MISO_Pin         GPIO_PIN_6
#define SPI1_MOSI_Pin         GPIO_PIN_7
#define SPI1_Port             GPIOA




/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initialized GPIO pins for the LED indication.
 * @param   none
 * @retval  status of operation
 */
ErrorStatus GPIO_Heartbeat_Init(void);

/**
 * @brief   Initialized GPIO pins for TM163x devices.
 * @param   none
 * @return  status of operation
 */
ErrorStatus GPIO_TM163x_Init(void);

/**
 * @brief   Initialized GPIO pins for OnwWire bus.
 * @param   none
 * @return  status of operation
 */
ErrorStatus GPIO_OneWire_Init(void);


#ifdef __cplusplus
}
#endif
#endif /* __GPIO_INIT_H */
