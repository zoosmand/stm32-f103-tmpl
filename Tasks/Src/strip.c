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
  .period         = 157,
  .counterReg     = &stripRegister,
  .entranceFlag   = 31,
};

#define LEDS  12
#define STEPS ((LEDS * 24) + 50)
static uint8_t stripDeviceBuf_2812_01[STEPS];
static StripDevice_TypeDev stripDevice_2812_01 = {
  .Timer          = TIM1,
  .BufPtr         = stripDeviceBuf_2812_01,
  .Count          = STEPS,
  .LedCount       = LEDS,
  .Lock           = DISABLE,
};

static uint32_t index = 0;




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

    dev->Lock = ENABLE;
    uint32_t color = 0;

    for (uint16_t k = 0; k < dev->LedCount; k++) {
      color = rand();
      for (int8_t i = 23; i >= 0; i--) {
        dev->BufPtr[index++] = (color & (1 << i)) ? 60 : 30;
      }
    }

    LedStrip_RunBus(dev);

    index = 0;
    dev->Lock = DISABLE;

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
    uint16_t start = dev->Count - 50;
    for (uint16_t i = start; i < dev->Count; i++) {
      dev->BufPtr[i] = 0;
    }
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
