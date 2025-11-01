/**
  ******************************************************************************
  * @file           : gpio.c
  * @brief          : This file contains code services that use standart GPIO
  *                   pin functions;
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
#include "gpio.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Locaal variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/






////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus GPIO_LED_Init(void) {

  MODIFY_REG(
    LED_GREEN_Port->CRH, 
    LED_GREEN_Pin_Mask, 
    ((GPIO_GPO_PP | GPIO_IOS_2) << ((LED_GREEN_Pin_Pos - 8) * 4))
  );

  MODIFY_REG(
    LED_BLUE_Port->CRH, 
    LED_BLUE_Pin_Mask, 
    ((GPIO_GPO_PP | GPIO_IOS_2) << ((LED_BLUE_Pin_Pos - 8) * 4))
  );

  MODIFY_REG(
    LED_RED_Port->CRH, 
    LED_RED_Pin_Mask, 
    ((GPIO_GPO_PP | GPIO_IOS_2) << ((LED_RED_Pin_Pos - 8) * 4))
  );

  return (SUCCESS);
}





// ----------------------------------------------------------------------------
ErrorStatus GPIO_TM163x_Init(void) {

  MODIFY_REG(
    TM_SCK_Port->CRL, 
    TM_SCK_Pin_Mask, 
    ((GPIO_GPO_PP | GPIO_IOS_10) << (TM_SCK_Pin_Pos * 4))
  );

  MODIFY_REG(
    TM_DIO_Port->CRL, 
    TM_DIO_Pin_Mask, 
    ((GPIO_GPO_PP | GPIO_IOS_10) << (TM_DIO_Pin_Pos * 4))
  );

  PIN_H(TM_DIO_Port, TM_DIO_Pin);
  PIN_H(TM_SCK_Port, TM_SCK_Pin);

  return (SUCCESS);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
