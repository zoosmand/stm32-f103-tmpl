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
__attribute__((section(".cron"))) static uint32_t displayHealthCheckTaskCnt    = 0;
__attribute__((section(".cron"))) static uint32_t displayHealthCheckTaskReg    = 0;

static task_scheduler_t displayHealthCheckScheduler = {
  .counter        = &displayHealthCheckTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 8,
  .counterReg     = &displayHealthCheckTaskReg,
  .entranceFlag   = 31,
};

static uint16_t maxDisplay_0_data[(MAX72XX_MAX_SEG_CNT * 8)];
static Max72xx_TypeDef maxDisplay_0 = {
  .SegCnt     = MAX72XX_SEG_CNT,
  .MaxSegCnt  = MAX72XX_MAX_SEG_CNT,
  .BufPtr     = maxDisplay_0_data,
  .SPINssPort = GPIOA,
  .SPINssPin  = NSS_1_Pin_Pos,
  .SPIx       = SPI1,
  .DMAx       = DMA1,
  .DMAxTx     = DMA1_Channel3,
  .DMAxRx     = DMA1_Channel2,
  .Lock       = DISABLE,
};


static TM163x_TypeDef tmDisplay_0 = {
  .PortSck    = TM_SCK_Port,
  .PortDio    = TM_DIO_Port,
  .PinSck     = TM_SCK_Pin_Pos,
  .PinDio     = TM_DIO_Pin_Pos,
  .Dig1       = 0,
  .Dig1       = 0,
  .Dig2       = 0,
  .Dig3       = 0,
  .Lock       = DISABLE,
};


/* Private function prototypes ----------------------------------------------*/

static ErrorStatus maxDisplayHealthCheck_Task(Max72xx_TypeDef*);
static ErrorStatus tmDisplayHealthCheck_Task(TM163x_TypeDef*);




////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------

void DisplayHealthCheck_CronHandler(void) {

  Scheduler_Handler(&displayHealthCheckScheduler);

  if (FLAG_CHECK(displayHealthCheckScheduler.counterReg, displayHealthCheckScheduler.entranceFlag)) {

    FLAG_CLR(displayHealthCheckScheduler.counterReg, displayHealthCheckScheduler.entranceFlag);
    
    if (maxDisplay_0.Lock == DISABLE) {
      if (maxDisplayHealthCheck_Task(&maxDisplay_0)) {
        /* TODO reinitialize, overwise clear rediness flag */
        printf("Cannot run MAX72xx display device\n");
        maxDisplay_0.Lock = ENABLE;
      }
    }

    if (tmDisplay_0.Lock == DISABLE) {
      if (tmDisplayHealthCheck_Task(&tmDisplay_0)) {
        /* TODO reinitialize, overwise clear rediness flag */
        printf("Cannot run TM163x display device\n");
        tmDisplay_0.Lock = ENABLE;
      }
    }
  }
}



// ----------------------------------------------------------------------------

static ErrorStatus maxDisplayHealthCheck_Task(Max72xx_TypeDef* dev) {

  if (dev->Lock) return (ERROR);
  
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

Max72xx_TypeDef* Get_MaxDiplayDevice(void) {
  return &maxDisplay_0;
}



// ----------------------------------------------------------------------------

static ErrorStatus tmDisplayHealthCheck_Task(TM163x_TypeDef* dev) {
  
  if (dev->Lock) return (ERROR);

  return (SUCCESS);
}



// ----------------------------------------------------------------------------

TM163x_TypeDef* Get_TmDiplayDevice(void) {
  return &tmDisplay_0;
}


