/**
  ******************************************************************************
  * @file           : strip.c
  * @brief          : This file contains LED strip application code.
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

/* Includes ------------------------------------------------------------------*/
#include "strip.h"


/* Private includes ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

__attribute__((section(".cron"))) static uint32_t stripCounter       = 0;
__attribute__((section(".cron"))) static uint32_t stripRegister      = 0;

static task_scheduler_t stripScheduler = {
  .counter        = &stripCounter,
  .counterSrc     = &sysCnt,
  .period         = 331,
  .counterReg     = &stripRegister,
  .entranceFlag   = 31,
};

#define STEPS 40
static uint16_t stripDeviceBuf_2812_01[STEPS + 10] = {
    0, 5, 10, 15, 20, 25, 30, 35, 40, 45,
    50, 55, 60, 65, 70, 75, 80, 85, 90, 95,
    90, 85, 80, 75, 70, 65, 60, 55, 50, 45,
    40, 35, 30, 25, 20, 15, 10, 5
  };
static StripDevice_TypeDev stripDevice_2812_01 = {
  .Timer          = TIM1,
  .BufPtr         = stripDeviceBuf_2812_01,
  .Count          = STEPS,
  .Lock           = DISABLE,
};




////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

void Strip_CronHandler(void) {

  StripDevice_TypeDev* dev = Get_StripDevice(2812);

  if (dev->Lock == ENABLE) {
    /* TODO Reinitialize heartbeat device */
    return;
  }

  Scheduler_Handler(&stripScheduler);

  if (FLAG_CHECK(stripScheduler.counterReg, stripScheduler.entranceFlag)) {

    dev->BufPtr[0] = 0x00550000;
    dev->BufPtr[1] = 0x00005500;
    dev->BufPtr[2] = 0x00000055;
    dev->BufPtr[3] = 0x00555500;
    dev->BufPtr[4] = 0x00005555;
    // RunStrip(dev);

    // Clear the dedicated registry
    *stripScheduler.counterReg = 0;
  }
}




// ----------------------------------------------------------------------------

StripDevice_TypeDev* Get_StripDevice(uint16_t model) {

  StripDevice_TypeDev* dev;

  switch (model) {
  case 2812:
    dev = &stripDevice_2812_01;
    break;
  
  default:
    dev = NULL;
    break;
  }
  return dev;
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
