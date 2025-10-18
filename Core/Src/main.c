/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Global variables ---------------------------------------------------------*/
__attribute__((section(".cron"))) uint32_t _GEREG_              = 0;
__attribute__((section(".cron"))) uint32_t _ASREG_              = 0;
__attribute__((section(".cron"))) uint32_t sysCnt               = 0;
__attribute__((section(".cron"))) uint32_t secCnt               = 0;

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static __attribute__((section(".cron"))) uint32_t secCntCache   = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {


  static uint32_t tmpCnt = 0;
  
  if (CRON_SEC_EVENT) {
    
    // printf("The long test message, that might stuck the program but now it does not at all...\n");
    printf("sec:%li\n", secCnt);
    
    if (FLAG_CHECK(&_ASREG_, OneWireBus_RF)) {
      if (tmpCnt <= secCnt ) {

        OneWireDevice_t* devs = Get_OwDevices();

        for (uint8_t i = 0; i < 2; i++) {
          if (DS18B20_GetTemperatureMeasurment(&devs[i])) {
            /* --- on error, set up -128.00 C --- */
            devs[i].spad[0] = 0x00;
            devs[i].spad[1] = 0x08;
          }
        }

        uint32_t* t1 = (int32_t*)&devs[0].spad;
        uint32_t* t2 = (int32_t*)&devs[1].spad;
        printf("%d.%02d %d.%02d\n", 
          (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4),
          (int8_t)((*t2 & 0x0000fff0) >> 4), (uint8_t)(((*t2 & 0x0000000f) * 100) >> 4)
        );

        tmpCnt = secCnt + 4;
      }
    }
  }

  Led_Handler();

}


/**
 * @brief  The application cron service.
 * @retval none
 */
void Cron_Handler(void) {

  __NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  SET_BIT(CoreDebug->DEMCR, CoreDebug_DEMCR_TRCENA_Msk);

  if (!OneWire_Search())    FLAG_SET(&_ASREG_, OneWireBus_RF);
  if (!SSD13xx_Init(I2C1))  FLAG_SET(&_ASREG_, SSDDisplay_RF);
  if (!WHxxxx_Init(I2C1))   FLAG_SET(&_ASREG_, WHDisplay_RF);
  if (!SPI_Init(SPI1))      FLAG_SET(&_ASREG_, SPI1_RF);
  
  if (FLAG_CHECK(&_ASREG_, SPI1_RF)) {
    if (!W25qxx_Init(SPI1))      FLAG_SET(&_ASREG_, W25QXX_RF);
  }
  
  /* Display calibration */
  printf("\n");

  
  while (1) {
    __disable_irq();
    
    if (FLAG_CHECK(&_GEREG_, _SYSSECF_)) {
      FLAG_CLR(&_GEREG_, _SYSSECF_);
      IWDG->KR = IWDG_KEY_RELOAD;
    }

    if (secCntCache <= sysCnt) {
      secCntCache = sysCnt + 1000;
      secCnt++;
      FLAG_SET(&_GEREG_, _SYSSECF_);
    }

    __enable_irq();
    main();
  }
}



void Scheduler_Handler(task_scheduler_t *scheduler) {
  __disable_irq();

  if (*scheduler->counter <= *scheduler->counterSrc) {
    *scheduler->counter = *scheduler->counterSrc + scheduler->period;
    FLAG_SET(scheduler->counterReg, scheduler->entranceFlag);
  }

  __enable_irq();
}

