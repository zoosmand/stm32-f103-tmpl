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


static uint32_t stripDeviceBuf_01[5];
static StripDevice_TypeDev stripDevice_01 = {
  .PortData       = STRIP_DATA_Port,
  .PinData        = STRIP_DATA_Pin,
  .BufPtr         = stripDeviceBuf_01,
  .Count          = 5,
  .Lock           = DISABLE,
};




////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

void Strip_CronHandler(void) {

  if (stripDevice_01.Lock == ENABLE) {
    /* TODO Reinitialize heartbeat device */
    return;
  }

  Scheduler_Handler(&stripScheduler);

  if (FLAG_CHECK(stripScheduler.counterReg, stripScheduler.entranceFlag)) {

    stripDeviceBuf_01[0] = 0x00000000;
    stripDeviceBuf_01[1] = 0x00000000;
    stripDeviceBuf_01[2] = 0x00000000;
    stripDeviceBuf_01[3] = 0x00000000;
    stripDeviceBuf_01[4] = 0x00000000;
    TM1803_RunStrip(&stripDevice_01);

    // Clear the dedicated registry
    *stripScheduler.counterReg = 0;
  }
}




// ----------------------------------------------------------------------------

StripDevice_TypeDev* Get_StripDevice(void) {
  return &stripDevice_01;
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
