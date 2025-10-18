/**
  ******************************************************************************
  * @file           : w25qxx.c
  * @brief          : This file contains Windond W25Q SPI and QSPI seroal NOR 
  *                   flash code
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
#include "w25qxx.h"

/* Private variables ---------------------------------------------------------*/
/* -- w25q capacities in blocks -- */
const uint16_t w25q[] = {
    0x0002
  , 0x0004
  , 0x0008
  , 0x0010
  , 0x0020
  , 0x0040
  , 0x0080
  , 0x0100
  , 0x0200
  , 0x0400
};

w25qxx_t w25qxx;

/* Private function prototypes -----------------------------------------------*/
static void SPI_Transfer_DMA(const uint16_t cnt, const SPIDir_TypeDef dir, const uint32_t offset, uint8_t *buf);
static void SPI_Transfer(const uint8_t cmd, int32_t addr, uint16_t cnt, const SPIDir_TypeDef dir, const uint32_t offset, uint8_t *buf);







////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Transfesr SPI data via DMA
  * @param  cnt: Count of bytes to transfer
  * @param  dir: Direction - READ or WRITE EEPROM
  * @param  offset: Offset of a particular address of EEPROM
  * @retval None
  */
static void SPI_Transfer_DMA(const uint16_t cnt, const SPIDir_TypeDef dir, const uint32_t offset, uint8_t *buf) {
  uint8_t pump = 0;
  if (dir == WRITE) {
    /* Enable from memory to peripheral DMA transfer */
    PREG_SET(SPI1->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Enable incremental and wait until it ready */
    PREG_SET(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos);
    while (!(PREG_CHECK(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos)));
    /* Set buffer address */
    DMA1_Channel3->CMAR = ((uint32_t)buf) + (offset * 256);
    /* Set counter */
    DMA1_Channel3->CNDTR = cnt;
    __NOP();
    /* Enable transfer from memory to peripheral */
    PREG_SET(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);
  } else {
    /* Enable from memory to peripheral DMA transfer */
    PREG_SET(SPI1->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Enable from peripheral to memory DMA transfer */
    PREG_SET(SPI1->CR2, SPI_CR2_RXDMAEN_Pos);
    /* Disable incremental and wait until it clears */
    PREG_CLR(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos);
    while (PREG_CHECK(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos));
    /* Set buffer address */
    DMA1_Channel2->CMAR = (uint32_t)buf;
    DMA1_Channel3->CMAR = (uint32_t)&pump;
    /* Set counter */
    DMA1_Channel2->CNDTR = cnt;
    DMA1_Channel3->CNDTR = cnt;
    __NOP();
    /* Enable transfer from memory to peripheral */
    PREG_SET(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);
    /* Enable transfer from peripheral to memory */
    PREG_SET(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
  }

  /* Wait for transfer is compete */ 
  while (!(PREG_CHECK(DMA1->ISR, DMA_ISR_TCIF3_Pos)));
  /* Clear complete transger flag */
  SET_BIT(DMA1->IFCR, DMA_IFCR_CTCIF3);
  
  if (dir == WRITE) {
    /* Wait white SPI is busy */
    while (PREG_CHECK(SPI1->SR, SPI_SR_BSY_Pos));
    /* Disable from memory to peripheral DMA transfer */
    PREG_CLR(SPI1->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Disable transfer from memory to peripheral */
    PREG_CLR(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);
  } else {
    /* Wait white SPI is busy */
    while (PREG_CHECK(SPI1->SR, SPI_SR_BSY_Pos));
    /* Disable from memory to peripheral DMA transfer */
    PREG_CLR(SPI1->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Disable from peripheral to memory DMA transfer */
    PREG_CLR(SPI1->CR2, SPI_CR2_RXDMAEN_Pos);
    /* Disable transfer from peripheral to memory */
    PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
    /* Disable transfer from memory to peripheral */
    PREG_CLR(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);
  }
  /* Clear correspondents DMA flags */
  DMA1->IFCR |= 0x00000ff0;
}





static void SPI_Transfer(const uint8_t cmd, int32_t addr, const uint16_t cnt, const SPIDir_TypeDef dir, const uint32_t offset, uint8_t *buf) {
  /* Activate slave and wait the level is low */
  NSS_0_L;
  while (PREG_CHECK(SPI_Port->IDR, NSS_0_Pin_Pos));

  /* write a command, a dummy byte has to be read further */
  SPI1->DR = cmd;
  while (!(PREG_CHECK(SPI1->SR, SPI_SR_TXE_Pos)));
  while (!(PREG_CHECK(SPI1->SR, SPI_SR_RXNE_Pos)));
  SPI1->DR;

  /* write 24-bit address into the bus and wait each byte to be sent,
     a dummy byte has to be read further */
  if (addr >= 0) {
    uint8_t i = 4;
    while (--i) {
      SPI1->DR = (uint8_t)((addr >> (8 * i)) & 0xff);
      while (!(PREG_CHECK(SPI1->SR, SPI_SR_TXE_Pos)));
      /* read a dummy byte from the bus to clear receive buffer */
      while (!(PREG_CHECK(SPI1->SR, SPI_SR_RXNE_Pos)));
      SPI1->DR;
    }
  }

  /* in case of FastRead command, fillfull an address with a zero 
     and wait it to be sent, a dummy byte has to be read further */
  if (cmd == W25Qxx_FastRead) {
    SPI1->DR = 0;
    while (!(PREG_CHECK(SPI1->SR, SPI_SR_TXE_Pos)));
    while (!(PREG_CHECK(SPI1->SR, SPI_SR_RXNE_Pos)));
    SPI1->DR;
  }

  /* put the date to DMA bus */
  if (cnt) {
    SPI_Transfer_DMA(cnt, dir, offset, buf);
  }

  /* Deactivate slave */
  NSS_0_H;
}







int W25qxx_Init(void) {
  /* Prepare NSS pin */
  NSS_0_H;
  _delay_ms(1);


  uint8_t buf[12];

  w25qxx.Lock = 1;

  SPI_Transfer(W25Qxx_Read_JedecID, -1, 4, READ, 0, buf);
  w25qxx.ManID = buf[0];
  w25qxx.Type = buf[1];
  w25qxx.BlockCount = w25q[((buf[2] - 1) & 0x0f)];  
  w25qxx.Capacity = w25qxx.BlockCount * W25Qxx_BLOCK_SIZE;

  SPI_Transfer(W25Qxx_Read_UniqueID, -1, 12, READ, 0, buf);
  w25qxx.UniqID = *(uint64_t*)(&buf[4]);

  SPI_Transfer(W25Qxx_Read_DeviceID, -1, 4, READ, 0, buf);
  w25qxx.ID = buf[3];

  w25qxx.Lock = 0;

  if ((w25qxx.ManID == 0xef) && (w25qxx.Type = 0x40)) return (0);

  /* protect first 32K */
  // uint8_t stub;
  // stub = W25qxx_WriteStatusRegister(0, (W25Qxx_SEC_ | W25Qxx_TB_ | W25Qxx_BP2_));
  // SPI_Transfer(W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &stub);
  // printf("%x\n", ddd);

  return (1);
}







// -------------------------------------------------------------  
void W25qxx_Read(const uint32_t addr, const uint16_t cnt, uint8_t *buf) {
  uint32_t phy_addr = 0;
  W25qxx_IsBusy();
  
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
  phy_addr += W25Qxx_PAGE_SIZE * (addr & 0xf);
  
  SPI_Transfer(W25Qxx_FastRead, (phy_addr << 8), cnt, READ, 0, buf);  
}





// -------------------------------------------------------------  
void W25qxx_Write(uint32_t addr, uint16_t cnt, uint8_t *buf) {
  uint8_t pump = 0;
  uint32_t phy_addr = 0;
  W25qxx_IsBusy();
  
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
  phy_addr += W25Qxx_PAGE_SIZE * (addr & 0xf);
  uint32_t i = 0;
  for (i = 0; i < (cnt / 256); i++) {
    SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
    SPI_Transfer(W25Qxx_PageProgram, (phy_addr << 8), 256, WRITE, i, buf);  
    phy_addr += W25Qxx_PAGE_SIZE;
    W25qxx_IsBusy();
  }
  
  if (cnt % 256) {
    SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
    SPI_Transfer(W25Qxx_PageProgram, (phy_addr << 8), (cnt % 256), WRITE, i, buf);  
    W25qxx_IsBusy();
  }
}






// -------------------------------------------------------------  
void W25qxx_Erase(uint32_t addr, uint16_t sectors) {
  uint8_t pump = 0;
  uint32_t phy_addr = 0;
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  
  if (sectors > 8) {
    if (sectors > 16) {
      // *******************************  
      uint32_t blocks = (sectors / 16);
      for (int i = 0; i < blocks; i++) {
        SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
        SPI_Transfer(W25Qxx_Erase_Block_64, (phy_addr << 8), 0, NEUTRAL, 0, &pump);
        phy_addr += W25Qxx_BLOCK_SIZE;
        W25qxx_IsBusy();
      }
      uint32_t remain_addr = (addr & 0xffffff00) + (blocks << 8);
      if (sectors % 16) {
        SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
        W25qxx_Erase(remain_addr, (sectors % 16));
        W25qxx_IsBusy();
      }
      // *******************************  
    } else {
      // ******************************* 
      if (((addr >> 4) & 0xf) > 8) {
        phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
      }
      SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
      SPI_Transfer(W25Qxx_Erase_Block_32, (phy_addr << 8), 0, NEUTRAL, 0, &pump);
      uint32_t remain_addr = (addr & 0xffffff80) + (8 << 4);
      W25qxx_IsBusy();
      
      if (sectors - 8) {
        SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
        W25qxx_Erase(remain_addr, sectors - 8);
        W25qxx_IsBusy();
      }
    // *******************************  
    }
  } else {
    // *******************************  
    phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
    for (int i = 0; i < sectors; i++) {
      SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
      SPI_Transfer(W25Qxx_Erase_Sector, (phy_addr << 8), 0, NEUTRAL, 0, &pump);
      phy_addr += W25Qxx_SECTOR_SIZE;
      W25qxx_IsBusy();
    }
    // *******************************  
  }
}





void W25qxx_IsBusy() {
  uint8_t pump = 0;
  pump = W25Qxx_BUSY_;
  while (pump & W25Qxx_BUSY_) {
    SPI_Transfer(W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &pump);
  }
}






void W25qxx_Reset() {
  uint8_t pump = 0;
  SPI_Transfer(W25Qxx_EnableReset, -1, 0, NEUTRAL, 0, &pump);
  SPI_Transfer(W25Qxx_ResetProccess, -1, 0, NEUTRAL, 0, &pump);
  _delay_ms(10);
  SPI_Transfer(W25Qxx_ReleasePowerDown, -1, 0, NEUTRAL, 0, &pump);
}



/* type: 0 - non-volatile bits, 1 - volatile bits*/
uint8_t W25qxx_WriteStatusRegister(uint8_t type, uint8_t status) {
  uint8_t pump = 0;
  if (type) {
    SPI_Transfer(W25Qxx_Write_StatusNVRegEnable, -1, 0, NEUTRAL, 0, &pump);
  } else {
    SPI_Transfer(W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump);
  }
  
  SPI_Transfer(W25Qxx_Write_StatusRegister_1, -1, 1, WRITE, 0, &status);
  /* Skip one trash bytes */
  while (!(READ_BIT(SPI1->SR, SPI_SR_RXNE)));
  SPI1->DR;

  W25qxx_IsBusy();
  SPI_Transfer(W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &pump);
  return (pump);
}

