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
// typedef struct {
//   task_scheduler_t  *scheduler;
//   GPIO_TypeDef      *port;
//   uint16_t          pin;
//   void              (*callback)(uint32_t*);
//   uint32_t          *pauseCnt_1;
//   uint32_t          *srcPauseCnt_1;
//   uint32_t          pauseValue_1;
//   uint32_t          *pauseCnt_2;
//   uint32_t          *srcPauseCnt_2;
//   uint32_t          pauseValue_2;
// } task_led_toggle_t;


void Heartbeat_CronHandler(void);
// void LedToggle_Task(uint32_t*);

ErrorStatus Heartbeat_Init(HearbeatDevice_TypeDev*);

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