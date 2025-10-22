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
static int SPI_Transfer_DMA(SPI_TypeDef*, const uint16_t, const SPIDir_TypeDef, const uint32_t, uint8_t*);
static int SPI_Transfer(SPI_TypeDef*, const uint8_t, int32_t, uint16_t, const SPIDir_TypeDef, const uint32_t, uint8_t*);
__STATIC_INLINE void SPI_Adjust(SPI_TypeDef*, DMA_Channel_TypeDef*, DMA_Channel_TypeDef*);






////////////////////////////////////////////////////////////////////////////////
__STATIC_INLINE void SPI_Adjust(SPI_TypeDef* SPIx, DMA_Channel_TypeDef* DMAxTx, DMA_Channel_TypeDef* DMAxRx) {
  /* adjust frequency divider, 0b001 = 4, (PCLK)72/4 = 18MHz */
  /* set 8-bit data buffer length */ 
  MODIFY_REG(SPIx->CR1, (SPI_CR1_BR_Msk | SPI_CR1_DFF_Msk), SPI_CR1_BR_0);
  PREG_SET(SPIx->CR2, SPI_CR2_SSOE_Pos);


  uint8_t pump = 0;
  /* configure DMA, Channel2 - RX, Channel3 - TX */
  /* set priority high*/
  /* set memory to increment */
  MODIFY_REG(DMAxTx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC));

  /* set buffer size to 0 */
  DMAxTx->CNDTR = 0UL;
  /* set peripheral address */
  DMAxTx->CPAR = (uint32_t)&SPIx->DR;
  /* set memory address */
  DMAxTx->CMAR = (uint32_t)&pump;
  
  /* set priority high*/
  /* set memory to increment */
  /* set direction from memory to peripheral */
  MODIFY_REG(DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR));

  /* set buffer size to 0 */
  DMAxRx->CNDTR = 0UL;
  /* set peripheral address */
  DMAxRx->CPAR = (uint32_t)&SPIx->DR;
  /* set memory address */
  DMAxRx->CMAR = (uint32_t)&pump;
}



/**
  * @brief  Transfesr SPI data via DMA
  * @param  cnt: Count of bytes to transfer
  * @param  dir: Direction - READ or WRITE EEPROM
  * @param  offset: Offset of a particular address of EEPROM
  * @retval None
  */
static int SPI_Transfer_DMA(SPI_TypeDef* SPIx, const uint16_t cnt, const SPIDir_TypeDef dir, const uint32_t offset, uint8_t *buf) {
  uint8_t pump = 0;
  uint32_t tmout = 0;

  if (dir == WRITE) {
    /* Enable from memory to peripheral DMA transfer */
    PREG_SET(SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Enable incremental and wait until it ready */
    PREG_SET(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos);

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos))) {
      if (!(--tmout)) {
        SPI_Disable(SPIx);
        return (1);
      }
    }

    /* Set buffer address */
    DMA1_Channel3->CMAR = ((uint32_t)buf) + (offset * W25Qxx_PAGE_SIZE);
    /* Set counter */
    DMA1_Channel3->CNDTR = cnt;
    __NOP();
    /* Enable transfer from memory to peripheral */
    PREG_SET(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);

  } else {
    /* Enable from memory to peripheral DMA transfer */
    PREG_SET(SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Enable from peripheral to memory DMA transfer */
    PREG_SET(SPIx->CR2, SPI_CR2_RXDMAEN_Pos);
    /* Disable incremental and wait until it clears */
    PREG_CLR(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos);
    
    tmout = SPI_BUS_TMOUT;
    while((PREG_CHECK(DMA1_Channel3->CCR, DMA_CCR_MINC_Pos))) {
      if (!(--tmout)) {
        SPI_Disable(SPIx);
        return (1);
      }
    }

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
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(DMA1->ISR, DMA_ISR_TCIF3_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(SPIx);
      return (1);
    }
  }

  /* Clear complete transger flag */
  SET_BIT(DMA1->IFCR, DMA_IFCR_CTCIF3);
  
  if (dir == WRITE) {
    /* Wait white SPI is busy */
    tmout = SPI_BUS_TMOUT;
    while((PREG_CHECK(SPIx->SR, SPI_SR_BSY_Pos))) {
      if (!(--tmout)) {
        SPI_Disable(SPIx);
        return (1);
      }
    }

    /* Disable from memory to peripheral DMA transfer */
    PREG_CLR(SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Disable transfer from memory to peripheral */
    PREG_CLR(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);
  } else {
    /* Wait white SPI is busy */
    tmout = SPI_BUS_TMOUT;
    while((PREG_CHECK(SPIx->SR, SPI_SR_BSY_Pos))) {
      if (!(--tmout)) {
        SPI_Disable(SPIx);
        return (1);
      }
    }

    /* Disable from memory to peripheral DMA transfer */
    PREG_CLR(SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Disable from peripheral to memory DMA transfer */
    PREG_CLR(SPIx->CR2, SPI_CR2_RXDMAEN_Pos);
    /* Disable transfer from peripheral to memory */
    PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
    /* Disable transfer from memory to peripheral */
    PREG_CLR(DMA1_Channel3->CCR, DMA_CCR_EN_Pos);
  }
  /* Clear correspondents DMA flags */
  DMA1->IFCR |= 0x00000ff0;

  return (0);
}





static int SPI_Transfer(SPI_TypeDef* SPIx, const uint8_t cmd, int32_t addr, const uint16_t cnt, const SPIDir_TypeDef dir, const uint32_t offset, uint8_t *buf) {

  uint32_t tmout = 0;
  /* Activate slave and wait the level is low */
  NSS_0_L;
  while (PREG_CHECK(SPI_Port->IDR, NSS_0_Pin_Pos));

  /* write a command, a dummy byte has to be read further */
  SPIx->DR = cmd;

  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(SPIx);
      return (1);
    }
  }

  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(SPIx);
      return (1);
    }
  }

  SPIx->DR;

  /* write 24-bit address into the bus and wait each byte to be sent,
     a dummy byte has to be read further */
  if (addr >= 0) {
    uint8_t i = 4;
    while (--i) {
      SPIx->DR = (uint8_t)((addr >> (8 * i)) & 0xff);

      /* read a dummy byte from the bus to clear receive buffer */
      tmout = SPI_BUS_TMOUT;
      while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
        if (!(--tmout)) {
          SPI_Disable(SPIx);
          return (1);
        }
      }

      tmout = SPI_BUS_TMOUT;
      while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
        if (!(--tmout)) {
          SPI_Disable(SPIx);
          return (1);
        }
      }

      SPIx->DR;
    }
  }

  /* in case of FastRead command, fillfull an address with a zero 
     and wait it to be sent, a dummy byte has to be read further */
  if (cmd == W25Qxx_FastRead) {
    SPIx->DR = 0;

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_TXE_Pos))) {
      if (!(--tmout)) {
        SPI_Disable(SPIx);
        return (1);
      }
    }


    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
      if (!(--tmout)) {
        SPI_Disable(SPIx);
        return (1);
      }
    }

    SPIx->DR;
  }

  /* put the date to DMA bus */
  if (cnt) {
    SPI_Transfer_DMA(SPIx, cnt, dir, offset, buf);
  }

  /* Deactivate slave */
  NSS_0_H;

  return (0);
}







int W25qxx_Init(SPI_TypeDef *SPIx) {
  /* Prepare NSS pin */
  // NSS_0_H;
  // _delay_ms(1);

  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);

  uint8_t buf[12];
  int ret = 0;

  w25qxx.Lock = 1;

  if (SPI_Transfer(SPIx, W25Qxx_Read_JedecID, -1, 4, READ, 0, buf)) return (1);
  w25qxx.ManID = buf[0];
  w25qxx.Type = buf[1];
  w25qxx.BlockCount = w25q[((buf[2] - 1) & 0x0f)];  
  w25qxx.Capacity = w25qxx.BlockCount * W25Qxx_BLOCK_SIZE;

  if (SPI_Transfer(SPIx, W25Qxx_Read_UniqueID, -1, 12, READ, 0, buf)) return (1);
  w25qxx.UniqID = *(uint64_t*)(&buf[4]);

  if (SPI_Transfer(SPIx, W25Qxx_Read_DeviceID, -1, 4, READ, 0, buf)) return (1);
  w25qxx.ID = buf[3];

  w25qxx.Lock = 0;

  if (!(w25qxx.ManID == 0xef) && !(w25qxx.Type = 0x40)) ret = 1;

  /* protect first 32K */
  // uint8_t stub;
  // stub = W25qxx_WriteStatusRegister(0, (W25Qxx_SEC_ | W25Qxx_TB_ | W25Qxx_BP2_));
  // SPI_Transfer(W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &stub);
  // printf("%x\n", ddd);

  SPI_Disable(SPIx);
  return ret;
}







// -------------------------------------------------------------  
int W25qxx_Read(SPI_TypeDef *SPIx, const uint32_t addr, const uint16_t cnt, uint8_t *buf) {

  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);

  uint32_t phy_addr = 0;
  if (W25qxx_IsBusy(SPIx)) return (1);
  
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
  phy_addr += W25Qxx_PAGE_SIZE * (addr & 0xf);
  
  if (SPI_Transfer(SPIx, W25Qxx_FastRead, (phy_addr << 8), cnt, READ, 0, buf)) return (1);

  SPI_Disable(SPIx);
  return (0);
}





// -------------------------------------------------------------  
int W25qxx_Write(SPI_TypeDef *SPIx, uint32_t addr, uint16_t cnt, uint8_t *buf) {
  uint8_t pump = 0;
  uint32_t phy_addr = 0;

  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);

  if (W25qxx_IsBusy(SPIx)) return (1);
  
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
  phy_addr += W25Qxx_PAGE_SIZE * (addr & 0xf);

  uint32_t i = 0;
  
  for (i = 0; i < (cnt / 256); i++) {
    if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
    if (SPI_Transfer(SPIx, W25Qxx_PageProgram, (phy_addr << 8), 256, WRITE, i, buf)) return (1);
    phy_addr += W25Qxx_PAGE_SIZE;
    if (W25qxx_IsBusy(SPIx)) return (1);
  }
  
  if (cnt % 256) {
    if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
    if (SPI_Transfer(SPIx, W25Qxx_PageProgram, (phy_addr << 8), (cnt % 256), WRITE, i, buf)) return (1);
    if (W25qxx_IsBusy(SPIx)) return (1);
  }

  SPI_Disable(SPIx);
  return (0);
}






// -------------------------------------------------------------  
int W25qxx_Erase(SPI_TypeDef *SPIx, uint32_t addr, uint16_t sectors) {
  uint8_t pump = 0;
  uint32_t phy_addr = 0;
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  
  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);
  
  if (sectors > 8) {
    if (sectors > 16) {
      // *******************************  
      uint32_t blocks = (sectors / 16);
      for (int i = 0; i < blocks; i++) {
        if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
        if (SPI_Transfer(SPIx, W25Qxx_Erase_Block_64, (phy_addr << 8), 0, NEUTRAL, 0, &pump)) return (1);
        phy_addr += W25Qxx_BLOCK_SIZE;
        if (W25qxx_IsBusy(SPIx)) return (1);
      }
      uint32_t remain_addr = (addr & 0xffffff00) + (blocks << 8);
      if (sectors % 16) {
        if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
        if (W25qxx_Erase(SPIx, remain_addr, (sectors % 16))) return (1);
        if (W25qxx_IsBusy(SPIx)) return (1);
      }
      // *******************************  
    } else {
      // ******************************* 
      if (((addr >> 4) & 0xf) > 8) {
        phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
      }

      if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
      if (SPI_Transfer(SPIx, W25Qxx_Erase_Block_32, (phy_addr << 8), 0, NEUTRAL, 0, &pump)) return (1);
      uint32_t remain_addr = (addr & 0xffffff80) + (8 << 4);
      if (W25qxx_IsBusy(SPIx)) return (1);
      
      if (sectors - 8) {
        if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
        if (W25qxx_Erase(SPIx, remain_addr, sectors - 8)) return (1);
        if (W25qxx_IsBusy(SPIx)) return (1);
      }
    // *******************************  
    }
  } else {
    // *******************************  
    phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
    for (int i = 0; i < sectors; i++) {
      if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
      if (SPI_Transfer(SPIx, W25Qxx_Erase_Sector, (phy_addr << 8), 0, NEUTRAL, 0, &pump)) return (1);
      phy_addr += W25Qxx_SECTOR_SIZE;
      if (W25qxx_IsBusy(SPIx)) return (1);
    }
    // *******************************  
  }

  SPI_Disable(SPIx);
  return (0);
}





int W25qxx_IsBusy(SPI_TypeDef *SPIx) {
  uint8_t pump = 0;

  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);

  pump = W25Qxx_BUSY_;
  while (pump & W25Qxx_BUSY_) {
    if (SPI_Transfer(SPIx, W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &pump)) return (1);
  }

  SPI_Disable(SPIx);
  return (0);
}






int W25qxx_Reset(SPI_TypeDef *SPIx) {
  uint8_t pump = 0;

  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);

  if (SPI_Transfer(SPIx, W25Qxx_EnableReset, -1, 0, NEUTRAL, 0, &pump)) return (1);
  if (SPI_Transfer(SPIx, W25Qxx_ResetProccess, -1, 0, NEUTRAL, 0, &pump)) return (1);
  _delay_ms(10);
  if (SPI_Transfer(SPIx, W25Qxx_ReleasePowerDown, -1, 0, NEUTRAL, 0, &pump)) return (1);

  SPI_Disable(SPIx);
  return (0);
}



/* type: 0 - non-volatile bits, 1 - volatile bits*/
uint8_t W25qxx_WriteStatusRegister(SPI_TypeDef *SPIx, uint8_t type, uint8_t status) {
  uint8_t pump = 0;

  SPI_Adjust(SPIx, DMA1_Channel2, DMA1_Channel3);
  SPI_Enable(SPIx);

  if (type) {
    if (SPI_Transfer(SPIx, W25Qxx_Write_StatusNVRegEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
  } else {
    if (SPI_Transfer(SPIx, W25Qxx_WriteEnable, -1, 0, NEUTRAL, 0, &pump)) return (1);
  }
  
  if (SPI_Transfer(SPIx, W25Qxx_Write_StatusRegister_1, -1, 1, WRITE, 0, &status)) return (1);

  /* Skip one trash bytes */
  uint32_t tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(SPIx->SR, SPI_SR_RXNE_Pos))) {
    if (!(--tmout)) {
      SPI_Disable(SPIx);
      return (1);
    }
  }


  SPIx->DR;

  if (W25qxx_IsBusy(SPIx)) return (1);
  if (SPI_Transfer(SPIx, W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &pump)) return (1);


  SPI_Disable(SPIx);
  return (pump);
}

