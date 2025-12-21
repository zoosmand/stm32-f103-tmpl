/**
  ******************************************************************************
  * @file           : tm1803.h
  * @brief          : This file contains header definitions for tm1803.c file,
  *                   3-channel LED controller.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 Askug Ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#ifndef __TM1803_H
#define __TM1803_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/
#define TM1803_BUS_TMOUT   5000 /* cycles timeout on TM bus operations */

#define TM_DATA_Low            PIN_L(TM_DIO_Port, TM_DIO_Pin)
#define TM_DATA_High           PIN_H(TM_DIO_Port, TM_DIO_Pin)

#define TM_DATA_Level          (PIN_LEVEL(TM_DIO_Port, TM_DIO_Pin))



/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initializes the given TM1803 device.
 * @param   dev pointer to the TM163x device struct
 * @retval  status of operation
 */
ErrorStatus TM1803x_Init(TM1803_TypeDef*);



#ifdef __cplusplus
}
#endif
#endif /* __TM1803_H */

