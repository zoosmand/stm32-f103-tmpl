/**
  ******************************************************************************
  * @file           : tmpr.c
  * @brief          : This file contains code for measurment of tempetature,
  *                   humidity, and atmospheric pressure.
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
#include "tmpr.h"


/* Private includes ----------------------------------------------------------*/
__attribute__((section(".cron"))) static uint32_t boschTaskCnt           = 0;
__attribute__((section(".cron"))) static uint32_t boschTaskReg           = 0;

static task_scheduler_t boschScheduler = {
  .counter        = &boschTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 7,
  .counterReg     = &boschTaskReg,
  .entranceFlag   = 31,
};




static ErrorStatus BoschMeasurment_Handler(void) {

  return (SUCCESS);
}


ErrorStatus BoschMeasurment_Task(BMx280_ItemTypeDef* dev) {


  return (SUCCESS);
}

