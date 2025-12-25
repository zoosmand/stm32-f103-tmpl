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
#define STRIP_BUS_TMOUT   5000 /* cycles timeout on TM bus operations */

#define STRIP_DATA_Low(dev)       PIN_L(dev->PortData, dev->PinData)
#define STRIP_DATA_High(dev)      PIN_H(dev->PortData, dev->PinData)


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initializes the given TM1803 device.
 * @param   dev pointer to the TM163x device struct
 * @retval  status of operation
 */
ErrorStatus TM1803_Init(StripDevice_TypeDev*);


ErrorStatus TM1803_RunStrip(StripDevice_TypeDev*);




#ifdef __cplusplus
}
#endif
#endif /* __TM1803_H */

