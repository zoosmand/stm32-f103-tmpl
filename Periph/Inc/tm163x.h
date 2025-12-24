/**
  ******************************************************************************
  * @file           : tm163x.h
  * @brief          : This file contains header definitions of TM163x Two Wire
  *                   shift register for a digital display code.
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


#ifndef __TM163X_H
#define __TM163X_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



/* Private defines -----------------------------------------------------------*/
#define TM_BUS_TMOUT   5000 /* cycles timeout on TM bus operations */

#define TM_DIO_Low(dev)       PIN_L(dev->PortDio, dev->PinDio)
#define TM_DIO_High(dev)      PIN_H(dev->PortDio, dev->PinDio)
#define TM_SCK_Low(dev)       PIN_L(dev->PortSck, dev->PinSck)
#define TM_SCK_High(dev)      PIN_H(dev->PortSck, dev->PinSck)

#define TM_DIO_Level(dev)    (PIN_LEVEL(dev->PortDio, dev->PinDio))



/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initializes the given TM163x device.
 * @param   dev pointer to the TM163x device struct
 * @retval  status of operation
 */
ErrorStatus TM163x_Init(TM163x_TypeDef*);

/**
 * @brief   Prints digits in the given display.
 * @param   dev pointer to the TM163x device struct
 * @return  status of operation
 */
ErrorStatus TM163x_Print(TM163x_TypeDef*);


#ifdef __cplusplus
}
#endif
#endif /* __TM163X_H */

