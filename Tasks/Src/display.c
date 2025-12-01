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
  .counter      = &displayHealthCheckTaskCnt,
  .counterSrc   = &secCnt,
  .period       = 8,
  .counterReg   = &displayHealthCheckTaskReg,
  .entranceFlag = 31,
};

static uint16_t maxDisplay_0_data[(MAX72XX_MAX_SEG_CNT * 8)];
static Max72xx_TypeDef maxDisplay_0 = {
  .SegCnt       = MAX72XX_SEG_CNT,
  .MaxSegCnt    = MAX72XX_MAX_SEG_CNT,
  .BufPtr       = maxDisplay_0_data,
  .SPINssPort   = SPI1_NSS_1_Port,
  .SPINssPin    = SPI1_NSS_1_Pin,
  .SPIx         = SPI1,
  .DMAx         = DMA1,
  .DMAxTx       = DMA1_Channel3,
  .Lock         = DISABLE,
};


static TM163x_TypeDef tmDisplay_0 = {
  .PortSck      = TM_SCK_Port,
  .PortDio      = TM_DIO_Port,
  .PinSck       = TM_SCK_Pin_Pos,
  .PinDio       = TM_DIO_Pin_Pos,
  .Dig1         = 0,
  .Dig1         = 0,
  .Dig2         = 0,
  .Dig3         = 0,
  .Lock         = DISABLE,
};


static WHxxxx_TypeDef whDisplay_1602 = {
  .Lock         = DISABLE,
  .I2Cx         = I2C1,
  .I2C_Address  = WHxxxx_I2C_ADDR,
  .DMAx         = DMA1,
  .DMAxTx       = DMA1_Channel6,
};


static WHxxxx_TypeDef whDisplay_2004 = {
  .Lock         = DISABLE,
  .I2Cx         = I2C1,
  .I2C_Address  = WHxxxx_I2C_ADDR,
  .DMAx         = DMA1,
  .DMAxTx       = DMA1_Channel6,
};


/* Private function prototypes ----------------------------------------------*/

static ErrorStatus maxDisplayHealthCheck_Task(Max72xx_TypeDef*);
static ErrorStatus tmDisplayHealthCheck_Task(TM163x_TypeDef*);
static ErrorStatus whDisplayHealthCheck_Task(WHxxxx_TypeDef*);




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

    if (whDisplay_1602.Lock == DISABLE) {
      if (whDisplayHealthCheck_Task(&whDisplay_1602)) {
        /* TODO reinitialize, overwise clear rediness flag */
        printf("Cannot run WHxxxx display device\n");
        whDisplay_1602.Lock = ENABLE;
      }
    }

    if (whDisplay_2004.Lock == DISABLE) {
      if (whDisplayHealthCheck_Task(&whDisplay_2004)) {
        /* TODO reinitialize, overwise clear rediness flag */
        printf("Cannot run WHxxxx display device\n");
        whDisplay_2004.Lock = ENABLE;
      }
    }
  }
}



/* ------------ MAX -------------- */
// ----------------------------------------------------------------------------

static ErrorStatus maxDisplayHealthCheck_Task(Max72xx_TypeDef* dev) {

  if (dev->Lock) return (ERROR);
  
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

Max72xx_TypeDef* Get_MaxDiplayDevice(void) {
  return &maxDisplay_0;
}



/* ------------ TM -------------- */
// ----------------------------------------------------------------------------

static ErrorStatus tmDisplayHealthCheck_Task(TM163x_TypeDef* dev) {
  
  if (dev->Lock) return (ERROR);

  return (SUCCESS);
}


// ----------------------------------------------------------------------------

TM163x_TypeDef* Get_TmDiplayDevice(void) {
  return &tmDisplay_0;
}




/* ------------ WH -------------- */
// ----------------------------------------------------------------------------

WHxxxx_TypeDef* Get_WhDiplayDevice(uint16_t model) {
  if (model == 1602) return &whDisplay_1602;
  if (model == 2004) return &whDisplay_2004;
  return NULL;
}


// ----------------------------------------------------------------------------

static ErrorStatus whDisplayHealthCheck_Task(WHxxxx_TypeDef* dev) {
  
  if (dev->Lock) return (ERROR);

  return (SUCCESS);
}
