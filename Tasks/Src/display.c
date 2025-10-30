/**
  ******************************************************************************
  * @file           : eeprom.c
  * @brief          : This file contains code EEPROM devices.
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
#include "display.h"


/* Private variables ----------------------------------------------------------*/
/* MAX display related data */
__attribute__((section(".cron"))) static uint32_t maxDisplayTaskCnt    = 0;
__attribute__((section(".cron"))) static uint32_t maxDisplayTaskReg    = 0;

static task_scheduler_t maxDisplayScheduler = {
  .counter        = &maxDisplayTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 8,
  .counterReg     = &maxDisplayTaskReg,
  .entranceFlag   = 31,
};

static uint16_t maxDisplay_0_data[(MAX72XX_MAX_SEG_CNT * 8)];
static Max72xx_TypeDef maxDisplay_0 = {
  .SegCnt     = MAX72XX_SEG_CNT,
  .MaxSegCnt  = MAX72XX_MAX_SEG_CNT,
  .BufPtr     = maxDisplay_0_data,
  .SPIx       = SPI1,
  .DMAx       = DMA1,
  .DMAxTx     = DMA1_Channel3,
  .DMAxRx     = DMA1_Channel2,
  .Lock       = ENABLE,
};



/* Private function prototypes ----------------------------------------------*/

static ErrorStatus maxDisplay_Task(Max72xx_TypeDef*);




////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------

void MaxDisplay_CronHandler(void) {

  Scheduler_Handler(&maxDisplayScheduler);

  if (FLAG_CHECK(maxDisplayScheduler.counterReg, maxDisplayScheduler.entranceFlag)) {

    FLAG_CLR(maxDisplayScheduler.counterReg, maxDisplayScheduler.entranceFlag);
    
    if (maxDisplay_Task(&maxDisplay_0)) {
      /* TODO reinitialize, overwise clear rediness flag */
      printf("Cannot run display device\n");
    }

    /* TODO handle EEPROM data usage */
    __NOP();
  }
}



// ----------------------------------------------------------------------------

static ErrorStatus maxDisplay_Task(Max72xx_TypeDef* dev) {

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
    
  MAX72xx_Print(dev, "1234567890");

  __NOP();
  
  dev->Lock = ENABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

Max72xx_TypeDef* Get_MaxDiplayDevice(void) {
  return &maxDisplay_0;
}

