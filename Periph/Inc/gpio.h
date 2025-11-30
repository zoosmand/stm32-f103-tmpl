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
#define LED_GREEN_Pin         GPIO_PIN_13
#define LED_GREEN_Pin_Pos     GPIO_PIN_13_Pos
#define LED_GREEN_Pin_Mask    GPIO_PIN_13_Mask
#define LED_GREEN_Port        GPIOB

#define LED_BLUE_Pin          GPIO_PIN_15
#define LED_BLUE_Pin_Pos      GPIO_PIN_15_Pos
#define LED_BLUE_Pin_Mask     GPIO_PIN_15_Mask
#define LED_BLUE_Port         GPIOB

#define LED_RED_Pin           GPIO_PIN_8
#define LED_RED_Pin_Pos       GPIO_PIN_8_Pos
#define LED_RED_Pin_Mask      GPIO_PIN_8_Mask
#define LED_RED_Port          GPIOA


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
#define OneWire_PIN           GPIO_PIN_9_Pos
#define OneWire_PIN_Pos       GPIO_PIN_9_Pos


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initialized GPIO pins for the LED indication.
 * @param   none
 * @retval  status of operation
 */
ErrorStatus GPIO_LED_Init(void);

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

