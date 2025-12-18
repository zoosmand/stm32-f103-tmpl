/**
  ******************************************************************************
  * @file           : hchk.c
  * @brief          : This file contains health check and monitoring procedures
  *                   code.
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

/* Includes ------------------------------------------------------------------*/
#include "hchk.h"


/* Private includes ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

__attribute__((section(".cron"))) static uint32_t hbCounter       = 0;
__attribute__((section(".cron"))) static uint32_t hbRegister      = 0;
__attribute__((section(".cron"))) static uint32_t blinkThreshold  = 0;

static task_scheduler_t heartbeatScheduler = {
  .counter        = &hbCounter,
  .counterSrc     = &sysCnt,
  .period         = 1500,
  .counterReg     = &hbRegister,
  .entranceFlag   = 31,
};

static HearbeatDevice_TypeDev hearbeatLedDevice = {
  .Port           = HEARTBEAT_LED_Port,
  .Pin            = HEARTBEAT_LED_Pin,
  .Lock           = DISABLE,
  .BlinkCount     = 2,
  .BlinkDuration  = 5,
  .BlinkPause     = 200,
  .BlinkThreshold = &blinkThreshold,
  .CounterSrc     = &sysCnt,
};




////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

void Heartbeat_CronHandler(void) {

  if (hearbeatLedDevice.Lock == DISABLE) {
    /* TODO Reinitialize heartbeat device */
    return;
  }

  Scheduler_Handler(&heartbeatScheduler);

  if (FLAG_CHECK(heartbeatScheduler.counterReg, heartbeatScheduler.entranceFlag)) {


    for (uint8_t i = 0; i < hearbeatLedDevice.BlinkCount; i++) {
      // Turn on the LED
      if (!FLAG_CHECK(heartbeatScheduler.counterReg, (0 + (i * 4)))) {
        FLAG_SET(heartbeatScheduler.counterReg, (0 + (i * 4)));
        PIN_L(hearbeatLedDevice.Port, hearbeatLedDevice.Pin);
        // Set up the first pause
        *hearbeatLedDevice.BlinkThreshold = *hearbeatLedDevice.CounterSrc + hearbeatLedDevice.BlinkDuration;
        return;
      }
  
      // Handle the first pause
      if (!FLAG_CHECK(heartbeatScheduler.counterReg, (1 + (i * 4)))) {
        if (*hearbeatLedDevice.BlinkThreshold > *hearbeatLedDevice.CounterSrc) return;
        FLAG_SET(heartbeatScheduler.counterReg, (1 + (i * 4)));
        return;
      }
  
      // Turn off the LED
      if (!FLAG_CHECK(heartbeatScheduler.counterReg, (2 + (i * 4)))) {
        FLAG_SET(heartbeatScheduler.counterReg, (2 + (i * 4)));
        PIN_H(hearbeatLedDevice.Port, hearbeatLedDevice.Pin);
        // Set up the second pause
        *hearbeatLedDevice.BlinkThreshold = *hearbeatLedDevice.CounterSrc + hearbeatLedDevice.BlinkPause;
        return;
      }

      // Handle the second pause
      if (!FLAG_CHECK(heartbeatScheduler.counterReg, (3 + (i * 4)))) {
        if (*hearbeatLedDevice.BlinkThreshold > *hearbeatLedDevice.CounterSrc) return;
        FLAG_SET(heartbeatScheduler.counterReg, (3 + (i * 4)));
        return;
      }
    }

    // Clear the dedicated registry
    *heartbeatScheduler.counterReg = 0;
  }
}




// ----------------------------------------------------------------------------

HearbeatDevice_TypeDev* Get_HeartbeatDevice(void) {
  return &hearbeatLedDevice;
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
