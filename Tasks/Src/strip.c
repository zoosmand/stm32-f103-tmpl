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
  .period         = 137,
  .counterReg     = &stripRegister,
  .entranceFlag   = 31,
};

#define LEDS  22
#define STEPS ((LEDS * 24) + 50)
static uint8_t stripDeviceBuf_2812_01[STEPS];
static StripDevice_TypeDev stripDevice_2812_01 = {
  .Timer          = TIM1,
  .BufPtr         = stripDeviceBuf_2812_01,
  .BufSize        = STEPS,
  .LedCount       = LEDS,
  .Lock           = DISABLE,
};

static uint32_t index = 0;




////////////////////////////////////////////////////////////////////////////////



__STATIC_INLINE void setPixelColor(StripDevice_TypeDev* dev, uint8_t j, uint8_t r, uint8_t g, uint8_t b) {
  
  uint32_t color = ((j << 24) | (g << 16) | (r << 8) | b);
  for (int8_t i = 23; i >= 0; i--) {
    dev->BufPtr[index++] = (color & (1 << i)) ? 60 : 30;
  }
}





static uint32_t effStep = 50;
  
void stripRainbow(StripDevice_TypeDev* dev) {
  // Strip ID: 0 - Effect: Rainbow - LEDS: 16
  // Steps: steps - Delay: 92
  // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
  // Options: rainbowlen=75, toLeft=false, 
  // if(millis() - strip_0.effStart < 92 * (strip_0.effStep)) return 0x00;
  uint8_t steps = 88;
  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j = 0; j < dev->LedCount; j++) {
    ind = steps - (uint16_t)(effStep - j * 0.96) % steps;
    switch((int)((ind % steps) / 24)) {
      case 0: factor1 = 1.0 - ((float)(ind % steps - 0 * 24) / 24);
              factor2 = (float)((int)(ind - 0) % steps) / 24;
              setPixelColor(dev, 0, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              break;
      case 1: factor1 = 1.0 - ((float)(ind % steps - 1 * 24) / 24);
              factor2 = (float)((int)(ind - 24) % steps) / 24;
              setPixelColor(dev, 0, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              break;
      case 2: factor1 = 1.0 - ((float)(ind % steps - 2 * 24) / 24);
              factor2 = (float)((int)(ind - 48) % steps) / 24;
              setPixelColor(dev, 0, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              break;
    }
  }
  if(effStep >= steps) effStep = 0;
  else effStep++;
}






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

    // Random colors
    // uint32_t color = 0;
    // for (uint16_t k = 0; k < dev->LedCount; k++) {
    //   color = rand();
    //   setPixelColor(dev, 0, (uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color);
    // }





    stripRainbow(dev);


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
    uint16_t start = dev->BufSize - 50;
    for (uint16_t i = start; i < dev->BufSize; i++) {
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
