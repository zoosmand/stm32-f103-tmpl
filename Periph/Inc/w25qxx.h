/**
  ******************************************************************************
  * @file           : w25qxx.h
  * @brief          : This file contains header definitions for Windond W25Q
  *                   SPI and QSPI flash code
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


#ifndef __W25QXX_H
#define __W25QXX_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define W25Qxx_WriteEnable              0x06
#define W25Qxx_WriteDisable             0x04
#define W25Qxx_Read_JedecID             0x9f
#define W25Qxx_Read_UniqueID            0x4b
#define W25Qxx_Read_DeviceID            0xab
#define W25Qxx_ReleasePowerDown         0xab
#define W25Qxx_Read_StatusRegister_1    0x05
#define W25Qxx_Read_StatusRegister_2    0x35
#define W25Qxx_Read_StatusRegister_3    0x15
#define W25Qxx_Write_StatusNVRegEnable  0x50
#define W25Qxx_Write_StatusRegister     0x01
#define W25Qxx_Write_StatusRegister_1   0x01
#define W25Qxx_Write_StatusRegister_2   0x31
#define W25Qxx_Write_StatusRegister_3   0x11
#define W25Qxx_PageProgram              0x02
#define W25Qxx_ReadData                 0x03
#define W25Qxx_FastRead                 0x0b
#define W25Qxx_Erase_Chip               0xc7
#define W25Qxx_Erase_Block_64           0xd8
#define W25Qxx_Erase_Block_32           0x52
#define W25Qxx_Erase_Sector             0x20
#define W25Qxx_Erase_Suspend            0x75
#define W25Qxx_Erase_Resume             0x7a
#define W25Qxx_PowerDown                0xb9
#define W25Qxx_PowerDown_Release        0xab
#define W25Qxx_EnableReset              0x66
#define W25Qxx_ResetProccess            0x99

#define W25Qxx_BUSY_                    0x01 // Erase or write in progress
#define W25Qxx_WEL_                     0x02 // Write enable Latch
#define W25Qxx_BP0_                     0x04 // Block protect bit 0 (non-volatile)
#define W25Qxx_BP1_                     0x08 // Block protect bit 1 (non-volatile)
#define W25Qxx_BP2_                     0x10 // Block protect bit 2 (non-volatile)
#define W25Qxx_TB_                      0x20 // Top/Bottom write protect (non-volatile)
#define W25Qxx_SEC_                     0x40 // Sector protect (non-volatile)

#define W25Qxx_SRP0_                    0x80 // Status register protect bit 0 (non-volatile)
#define W25Qxx_SRP1_                    0x01 // Status register protect bit 1 (non-volatile)
#define W25Qxx_SREQ_                    0x02 // Quad enable

#define W25Qxx_PAGE_SIZE                0x0100 // 256
#define W25Qxx_SECTOR_SIZE              0x1000 // 4096
#define W25Qxx_BLOCK_32K_SIZE           0x8000 // 32768
#define W25Qxx_BLOCK_SIZE               0x00010000 // 64536

typedef struct {
  uint8_t               ID;
  uint8_t               ManID;
  uint8_t               Type;
  uint64_t              UniqID;
  uint16_t              BlockCount;
  uint32_t              Capacity;
  uint8_t               Lock;
  SPI_TypeDef*          SPIx;
  DMA_Channel_TypeDef*  DMAxRx;
  DMA_Channel_TypeDef*  DMAxTx;
} w25qxx_t;


// Address Pattern
// 0x00BBBBSP
// bits 3..0 - number of a page - P
// bits 7..4 - number of a sector - S
// bits 23..8 - number of a block - B

extern w25qxx_t	w25qxx;




int W25qxx_Init(SPI_TypeDef*);
int W25qxx_Reset(w25qxx_t*);
int W25qxx_IsBusy(w25qxx_t*);
int W25qxx_Read(w25qxx_t*, const uint32_t, const uint16_t, uint8_t*);
int W25qxx_Write(w25qxx_t*, const uint32_t, const uint16_t, uint8_t*);
int W25qxx_Erase(w25qxx_t*, uint32_t, uint16_t);
uint8_t W25qxx_WriteStatusRegister(w25qxx_t*, uint8_t, uint8_t);


#ifdef __cplusplus
}
#endif
#endif /* __W25QXX_H */

