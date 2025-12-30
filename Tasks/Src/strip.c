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

#define STEPS ((8 * 24) + 50)
static uint8_t stripDeviceBuf_2812_01[STEPS];
static StripDevice_TypeDev stripDevice_2812_01 = {
  .Timer          = TIM1,
  .BufPtr         = stripDeviceBuf_2812_01,
  .Count          = STEPS,
  .Lock           = DISABLE,
};

uint32_t index = 0;



////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------

__STATIC_INLINE void start_bus(StripDevice_TypeDev* dev) {

  TIM_TypeDef* TIMx = dev->Timer;

  PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
  DMA1->IFCR = DMA_IFCR_CGIF2;        // clear all flags for CH2

  DMA1_Channel2->CMAR = (uint32_t)dev->BufPtr;
  DMA1_Channel2->CPAR = (uint32_t)&TIM1->CCR1;
  DMA1_Channel2->CNDTR = dev->Count;


  PREG_SET(TIMx->DIER, TIM_DIER_CC1DE_Pos);
  PREG_SET(TIMx->DIER, TIM_DIER_UDE_Pos);
  PREG_SET(TIMx->EGR, TIM_EGR_UG_Pos);
  PREG_SET(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
  PREG_SET(TIMx->CCER, TIM_CCER_CC1E_Pos);
  PREG_SET(TIMx->BDTR, TIM_BDTR_MOE_Pos);
  PREG_SET(TIMx->CR1, TIM_CR1_CEN_Pos);

}



// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus stop_bus(StripDevice_TypeDev* dev) {
  uint32_t tmout = 10000;
  ErrorStatus status = SUCCESS;
  TIM_TypeDef* TIMx = dev->Timer;

  while(!(PREG_CHECK(DMA1->ISR, DMA_ISR_TCIF2_Pos))) {
    if (!(--tmout)) { status = ERROR; }
  }

  PREG_CLR(TIMx->CR1, TIM_CR1_CEN_Pos);
  PREG_CLR(TIMx->BDTR, TIM_BDTR_MOE_Pos);
  PREG_CLR(TIMx->CCER, TIM_CCER_CC1E_Pos);
  PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);

  PREG_CLR(TIMx->DIER, TIM_DIER_CC1DE_Pos);
  PREG_CLR(TIMx->DIER, TIM_DIER_UDE_Pos);
  PREG_CLR(TIMx->EGR, TIM_EGR_UG_Pos);

  DMA1->IFCR = DMA_IFCR_CGIF2;

  return (status);
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


    uint32_t color = 0;


    for (uint8_t i = 0; i < 8; i++) {

      color = rand();

      for (int8_t i = 23; i >= 0; i--) {
        dev->BufPtr[index++] = (color & (1 << i)) ? 60 : 30;
      }

    }

    for (uint8_t i = 0; i < 50; i++) {
      dev->BufPtr[index++] = 0;
    }

    start_bus(dev);
    stop_bus(dev);

    index = 0;





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
