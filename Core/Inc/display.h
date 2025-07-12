/**
  ******************************************************************************
  * @file           : display.h
  * @brief          : This file contains header definitions for display code
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
#ifndef _DISPLAY_H
#define _DISPLAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
/* Private typedef -----------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void WH1602_I2C_Init(I2C_TypeDef*);
void WH1602_WriteChar(I2C_TypeDef*, uint8_t);
void WH1602_WriteCommand(I2C_TypeDef*, uint8_t, uint16_t);
void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t, const char*);
   
/* Private defines -----------------------------------------------------------*/
#define _ADDR_        0x27 // I2C Address of WH1602
#define _4BITINIT_    0x03 // 4bit initialization
#define _CURUPLEFT_   0x02 // ������ � ������� ����� �������
#define	_4BITBUS_     0x28 // ���� 4 ���, LCD - 2 ������
#define _READYNOCUR_	0x0c // ��������� ����� �����������, ������ �� �����
#define _CLEARDISP_   0x01 // ������� �������
#define _CLEARDISP2_  0x80 // ������� �������

/***************************** WH0802A Control Bits *****************************/
#define _Bl   0x08
#define _E    0x04
#define _Rw 	0x02
#define _Rs 	0x01



/* Extern functions prototypes ----------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif // _DISPLAY_H
