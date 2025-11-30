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
  
  /* ----------------------------------*/
  /* ----------------------------------*/
  /* ----------------------------------*/
  if (CRON_SEC_EVENT) {
    
    // printf("The long test message, that might stuck the program but now it does not at all...\n");
    printf("sec:%li\n", secCnt);
    

    if (FLAG_CHECK(&_ASREG_, OW_BUS_RF)) { DsMeasurment_CronHandler(); }
    
    if (FLAG_CHECK(&_ASREG_, BMX280_RF)) { BoschMeasurment_CronHandler(); }

    if (FLAG_CHECK(&_ASREG_, EEPROM_RF)) { EepromHealthCheck_CronHandler(); }
    
    if (
         FLAG_CHECK(&_ASREG_, MAX_DSPL_RF) 
      || FLAG_CHECK(&_ASREG_, TM_DSPL_RF)
      || FLAG_CHECK(&_ASREG_, WH_DSPL_RF)
    ) { DisplayHealthCheck_CronHandler(); }

  }

  /* ----------------------------------*/
  /* ----------------------------------*/
  /* ----------------------------------*/
  if (CRON_SYSTICK_EVENT) {

    if (FLAG_CHECK(&_ASREG_, GPIO_LED_RF)) { Led_CronHandler(); }

  }

}





/**
 * @brief  The application cron service.
 * @retval none
 */
void Cron_Handler(void) {

  __NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  SET_BIT(CoreDebug->DEMCR, CoreDebug_DEMCR_TRCENA_Msk);

  /* Initialize GPIOs and buses */
  if (!GPIO_LED_Init())         FLAG_SET(&_ASREG_, GPIO_LED_RF);
  if (!GPIO_TM163x_Init())      FLAG_SET(&_ASREG_, GPIO_TM_RF);
  if (!GPIO_OneWire_Init())     FLAG_SET(&_ASREG_, GPIO_OW_RF);
  if (!SPI_Init(SPI1))          FLAG_SET(&_ASREG_, SPI1_RF);
  if (!I2C_Init(I2C1))          FLAG_SET(&_ASREG_, I2C1_RF);
  

  /* Initialize devices based on their own bus */
  if (FLAG_CHECK(&_ASREG_, GPIO_OW_RF)) {
    if (!OneWireBus_Init(Get_OneWireBusDevice()))     FLAG_SET(&_ASREG_, OW_BUS_RF);
  }
  if (FLAG_CHECK(&_ASREG_, GPIO_TM_RF)) {
    if (!TM163x_Init(Get_TmDiplayDevice()))           FLAG_SET(&_ASREG_, TM_DSPL_RF);
  }

  /* Initialize SPI1 bus devices */
  if (FLAG_CHECK(&_ASREG_, SPI1_RF)) {
    if (!W25qxx_Init(Get_EepromDevice()))             FLAG_SET(&_ASREG_, EEPROM_RF);
    if (!MAX72xx_Init(Get_MaxDiplayDevice()))         FLAG_SET(&_ASREG_, MAX_DSPL_RF);
    if (!BMx680_Init(Get_BoschDevice(1)))             FLAG_SET(&_ASREG_, BMX680_RF);
  }
  
  /* Initialize I2C1 bus devices */
  if (FLAG_CHECK(&_ASREG_, I2C1_RF)) {
    if (!BMx280_Init(Get_BoschDevice(0)))             FLAG_SET(&_ASREG_, BMX280_RF);
    if (!SSD13xx_Init(I2C1))                          FLAG_SET(&_ASREG_, SSD_DSPL_RF);
    if (!WHxxxx_Init(Get_WhDiplayDevice(WH_MODEL)))   FLAG_SET(&_ASREG_, WH_DSPL_RF);
  }

  /* Display calibration */
  printf("\n");

  
  while (1) {
    __disable_irq();
    
    if (FLAG_CHECK(&_GEREG_, _SYSTICKF_)) {
      FLAG_CLR(&_GEREG_, _SYSTICKF_);
    }

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

