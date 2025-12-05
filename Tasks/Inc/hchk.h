/**
  ******************************************************************************
  * @file           : led.h
  * @brief          : Header for led.c file.
  *                   This file contains the common defines of the LED functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private structures -------------------------------------------------------------*/


void Heartbeat_CronHandler(void);
// void LedToggle_Task(uint32_t*);


/**
 * @brief   Returns OneWire device struct pointer.
 * @param   devNum: the number of the device in the set of the bus device
 * @return  the OneWire devices struct pointer
 */
HearbeatDevice_TypeDev* Get_HeartbeatDevice(void);



#ifdef __cplusplus
}
#endif

#endif /* __LED_H */