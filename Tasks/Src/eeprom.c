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
#include "eeprom.h"


/* Private variables ----------------------------------------------------------*/
/* W25Qxx related data */
__attribute__((section(".cron"))) static uint32_t eepromTaskCnt    = 0;
__attribute__((section(".cron"))) static uint32_t eepromTaskReg    = 0;

static task_scheduler_t eepromScheduler = {
  .counter        = &eepromTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 60,
  .counterReg     = &eepromTaskReg,
  .entranceFlag   = 31,
};

static W25qxx_TypeDef eeprom_0 = {
  .SPIx   = SPI1,
  .DMAx   = DMA1,
  .DMAxTx = DMA1_Channel3,
  .DMAxRx = DMA1_Channel2,
  .Lock   = ENABLE,
};


/* Private function prototypes ----------------------------------------------*/

static ErrorStatus eepromHealthCheck_Task(W25qxx_TypeDef*);




////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------

void EepromHealthCheck_CronHandler(void) {

  Scheduler_Handler(&eepromScheduler);

  if (FLAG_CHECK(eepromScheduler.counterReg, eepromScheduler.entranceFlag)) {

    FLAG_CLR(eepromScheduler.counterReg, eepromScheduler.entranceFlag);
    
    if (eepromHealthCheck_Task(&eeprom_0)) {
      /* TODO reinitialize, overwise clear rediness flag */
      printf("Cannot provide a health check for EEPROM device\n");
    }

    /* TODO handle EEPROM data usage */
    __NOP();
  }
}



// ----------------------------------------------------------------------------

static ErrorStatus eepromHealthCheck_Task(W25qxx_TypeDef* dev) {

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);

  /* TODO realize a propert health check */
  static uint8_t dataBuf[16];
  static uint8_t dataBuf2[256];
  dataBuf[0] = 200;
  dataBuf[1] = 201;
  dataBuf[2] = 202;
  dataBuf[3] = 203;
  dataBuf[4] = 204;
  dataBuf[5] = 205;
  dataBuf[6] = 206;
  dataBuf[7] = 207;
  dataBuf[8] = 208;
  dataBuf[9] = 209;
  dataBuf[10] = 210;
  dataBuf[11] = 211;
  dataBuf[12] = 212;
  dataBuf[13] = 213;
  dataBuf[14] = 214;
  dataBuf[15] = 215;


  if (W25qxx_Erase(dev, 0, 32)) { printf("Cannot erase the flash\n"); return (ERROR); }
  if (W25qxx_Read(dev, 0x00000000, 256, dataBuf2)) { printf("Cannot read the flash\n"); return (ERROR); }

  if (W25qxx_Write(dev, 0x00000000, 16, dataBuf)) { printf("Cannot write to the flash\n"); return (ERROR); }
  if (W25qxx_Read(dev, 0x00000000, 256, dataBuf2)) { printf("Cannot read the flash\n"); return (ERROR); }

  __NOP();

  dev->Lock = ENABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

W25qxx_TypeDef* Get_EepromDevice(void) {
  return &eeprom_0;
}

