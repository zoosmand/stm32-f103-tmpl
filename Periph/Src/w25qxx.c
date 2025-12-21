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

/**
 * @brief w25qxx EEPROM capacities in blocks.
 */
static const uint16_t w25q[] = {
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





/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Transfers SPI data to/from EEPROM via DMA.
 * @param   dev: pointer to the flash device struct
 * @param   cnt: number of bytes to transfer
 * @param   dir: direction of data transfer: READ or WRITE
 * @param   offset: offset of a particular EEPROM address
 * @param   buf: pointer to a buffer to write/read the data
 * @retval  status of operation
 */
__STATIC_INLINE ErrorStatus spi_transfer_dma(W25qxx_TypeDef*, const uint16_t, const DataTransmitDirection_TypeDef, const uint32_t, uint8_t*);

/**
 * @brief Transfers SPI commands and service data to/from EEPROM via SPI bus.
 * @param   dev: pointer to the flash device struct
 * @param   cmd: EEPROM specific command
 * @param   cnt: number of bytes to transfer
 * @param   dir: direction of data transfer: READ or WRITE
 * @param   offset: offset of a particular EEPROM address
 * @param   buf: pointer to a buffer to write/read the data
 * @retval  status of operation
 */
__STATIC_INLINE ErrorStatus spi_transfer(W25qxx_TypeDef*, const uint8_t, int32_t, uint16_t, const DataTransmitDirection_TypeDef, const uint32_t, uint8_t*);

/**
 * @brief Adjusts SPI bus according to EEPROM parameters.
 * @param   dev: pointer to the flash device struct
 * @retval  none
 */
__STATIC_INLINE void spi_dma_configure(W25qxx_TypeDef*);

/**
  * @brief   Unconfigure SPI bus.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE ErrorStatus spi_dma_unconfigure(W25qxx_TypeDef*);

/**
 * @brief   Controls the busy status of the given EEPROM.
 * @param   dev: pointer to the flash device struct
 * @retval  status of operation
 */
__STATIC_INLINE ErrorStatus w25qxx_busy(W25qxx_TypeDef*);




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



// ----------------------------------------------------------------------------

__STATIC_INLINE void spi_dma_configure(W25qxx_TypeDef* dev) {
  /* adjust frequency divider, 0b001 = 4, (PCLK)72/4 = 18MHz */
  /* set 8-bit data buffer length */ 
  MODIFY_REG(dev->SPIx->CR1, (SPI_CR1_BR_Msk | SPI_CR1_DFF_Msk), SPI_CR1_BR_0);
  PREG_SET(dev->SPIx->CR2, SPI_CR2_SSOE_Pos);


  uint8_t pump = 0;
  /* configure DMA, Channel2 - RX */
  /* set priority high*/
  /* set memory to increment */
  MODIFY_REG(dev->DMAxRx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC));
  
  /* set buffer size to 0 */
  dev->DMAxRx->CNDTR = 0UL;
  /* set peripheral address */
  dev->DMAxRx->CPAR = (uint32_t)&dev->SPIx->DR;
  /* set memory address */
  dev->DMAxRx->CMAR = (uint32_t)&pump;
  
  /* configure DMA, Channel3 - TX */
  /* set priority high*/
  /* set memory to increment */
  /* set direction from memory to peripheral */
  MODIFY_REG(dev->DMAxTx->CCR, (DMA_CCR_PL_Msk | DMA_CCR_MINC_Msk | DMA_CCR_DIR_Msk), (DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR));

  /* set buffer size to 0 */
  dev->DMAxTx->CNDTR = 0UL;
  /* set peripheral address */
  dev->DMAxTx->CPAR = (uint32_t)&dev->SPIx->DR;
  /* set memory address */
  dev->DMAxTx->CMAR = (uint32_t)&pump;

  SPI_Enable(dev->SPIx);
}




// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus spi_dma_unconfigure(W25qxx_TypeDef* dev) {

  /* Clear correspondents DMA flags */
  dev->DMAx->IFCR |= (
      DMA_IFCR_CGIF2_Msk
    | DMA_IFCR_CGIF3_Msk
    | DMA_IFCR_CHTIF2_Msk
    | DMA_IFCR_CHTIF3_Msk
    | DMA_IFCR_CTCIF2_Msk
    | DMA_IFCR_CTCIF3_Msk
  );
  
  /* Disable from memory to peripheral DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
  /* Disable from peripheral to memory DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);

  PIN_H(dev->SPINssPort, dev->SPINssPin);
  SPI_Disable(dev->SPIx);

  /* Clear DMA registers */
  dev->DMAxTx->CCR    = 0UL;
  dev->DMAxTx->CMAR   = 0UL;
  dev->DMAxTx->CPAR   = 0UL;
  dev->DMAxTx->CNDTR  = 0UL;

  dev->DMAxRx->CCR    = 0UL;
  dev->DMAxRx->CMAR   = 0UL;
  dev->DMAxRx->CPAR   = 0UL;
  dev->DMAxRx->CNDTR  = 0UL;

  return (ERROR);
}




// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus spi_transfer_dma(W25qxx_TypeDef* dev, const uint16_t cnt, const DataTransmitDirection_TypeDef dir, const uint32_t offset, uint8_t *buf) {
  uint8_t pump = 0;
  uint32_t tmout = 0;

  if (dir == TX) {
    /* Enable from memory to peripheral DMA transfer */
    PREG_SET(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Enable incremental and wait until it ready */
    PREG_SET(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos);

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos))) {
      if (!(--tmout)) { return spi_dma_unconfigure(dev); }
    }

    /* Set buffer address */
    dev->DMAxTx->CMAR = ((uint32_t)buf) + (offset * W25Qxx_PAGE_SIZE);
    /* Set counter */
    dev->DMAxTx->CNDTR = cnt;
    __NOP();
    /* Enable transfer from memory to peripheral */
    PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);

  } else {
    /* Enable from memory to peripheral DMA transfer */
    PREG_SET(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Enable from peripheral to memory DMA transfer */
    PREG_SET(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);
    /* Disable incremental and wait until it clears */
    PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos);
    
    tmout = SPI_BUS_TMOUT;
    while((PREG_CHECK(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos))) {
      if (!(--tmout)) { return spi_dma_unconfigure(dev); }
    }

    /* Set buffer address */
    dev->DMAxRx->CMAR = (uint32_t)buf;
    dev->DMAxTx->CMAR = (uint32_t)&pump;
    /* Set counter */
    dev->DMAxRx->CNDTR = cnt;
    dev->DMAxTx->CNDTR = cnt;
    __NOP();
    /* Enable transfer from memory to peripheral */
    PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
    /* Enable transfer from peripheral to memory */
    PREG_SET(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);
  }

  /* Wait for transfer is compete */
  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF3_Pos))) {
     if (!(--tmout)) { return spi_dma_unconfigure(dev); }
   }

  /* Clear complete transger flag */
  SET_BIT(dev->DMAx->IFCR, DMA_IFCR_CTCIF3);
  
  if (dir == TX) {
    /* Wait white SPI is busy */
    tmout = SPI_BUS_TMOUT;
    while((PREG_CHECK(dev->SPIx->SR, SPI_SR_BSY_Pos))) {
      if (!(--tmout)) { return spi_dma_unconfigure(dev); }
    }

    /* Disable from memory to peripheral DMA transfer */
    PREG_CLR(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Disable transfer from memory to peripheral */
    PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  } else {
    /* Wait white SPI is busy */
    tmout = SPI_BUS_TMOUT;
    while((PREG_CHECK(dev->SPIx->SR, SPI_SR_BSY_Pos))) {
      if (!(--tmout)) { return spi_dma_unconfigure(dev); }
    }

    /* Disable from memory to peripheral DMA transfer */
    PREG_CLR(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
    /* Disable from peripheral to memory DMA transfer */
    PREG_CLR(dev->SPIx->CR2, SPI_CR2_RXDMAEN_Pos);
    /* Disable transfer from peripheral to memory */
    PREG_CLR(dev->DMAxRx->CCR, DMA_CCR_EN_Pos);
    /* Disable transfer from memory to peripheral */
    PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  }
  /* Clear correspondents DMA flags */
  dev->DMAx->IFCR |= 0x00000ff0;

  return (SUCCESS);
}




// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus spi_transfer(W25qxx_TypeDef* dev, const uint8_t cmd, int32_t addr, const uint16_t cnt, const DataTransmitDirection_TypeDef dir, const uint32_t offset, uint8_t *buf) {

  uint32_t tmout = 0;

  /* Activate slave and wait the level is low */
  PIN_L(dev->SPINssPort, dev->SPINssPin);

  /* write a command, a dummy byte has to be read further */
  dev->SPIx->DR = cmd;

  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_TXE_Pos))) {
    if (!(--tmout)) { return spi_dma_unconfigure(dev); }
  }

  tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) {
    if (!(--tmout)) { return spi_dma_unconfigure(dev); }
  }

  dev->SPIx->DR;

  /* write 24-bit address into the bus and wait each byte to be sent,
     a dummy byte has to be read further */
  if (addr >= 0) {
    uint8_t i = 4;
    while (--i) {
      dev->SPIx->DR = (uint8_t)((addr >> (8 * i)) & 0xff);

      /* read a dummy byte from the bus to clear receive buffer */
      tmout = SPI_BUS_TMOUT;
      while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_TXE_Pos))) {
        if (!(--tmout)) { return spi_dma_unconfigure(dev); }
      }

      tmout = SPI_BUS_TMOUT;
      while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) {
        if (!(--tmout)) { return spi_dma_unconfigure(dev); }
      }

      dev->SPIx->DR;
    }
  }

  /* in case of FastRead command, fillfull an address with a zero 
     and wait it to be sent, a dummy byte has to be read further */
  if (cmd == W25Qxx_FastRead) {
    dev->SPIx->DR = 0;

    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_TXE_Pos))) {
      if (!(--tmout)) { return spi_dma_unconfigure(dev); }
    }


    tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) {
      if (!(--tmout)) { return spi_dma_unconfigure(dev); }
    }

    dev->SPIx->DR;
  }

  /* put the date to DMA bus */
  if (cnt) {
    if (spi_transfer_dma(dev, cnt, dir, offset, buf)) return (ERROR);
  }

  /* Deactivate slave */
  PIN_H(dev->SPINssPort, dev->SPINssPin);

  return (SUCCESS);
}





// ----------------------------------------------------------------------------

ErrorStatus W25qxx_Init(W25qxx_TypeDef* dev) {

  if ((dev->SPIx == NULL) || (dev->DMAx == NULL) || (dev->DMAxRx == NULL) || (dev->DMAxTx == NULL)) return (ERROR); 

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  /* Initialize NSS Pin */
  if (dev->SPIx != NULL) {
    if (dev->SPINssPin > 7) {
      MODIFY_REG(dev->SPINssPort->CRL, (0xf << ((dev->SPINssPin - 8) * 4)), ((GPIO_GPO_PP | GPIO_IOS_2) << ((dev->SPINssPin -8) * 4)));
    } else {
      MODIFY_REG(dev->SPINssPort->CRL, (0xf << (dev->SPINssPin * 4)), ((GPIO_GPO_PP | GPIO_IOS_2) << (dev->SPINssPin * 4)));
    }
    PIN_H(dev->SPINssPort, dev->SPINssPin);
    
    _delay_us(10);
  }

  spi_dma_configure(dev);

  uint8_t buf[12];
  int ret = SUCCESS;


  if (spi_transfer(dev, W25Qxx_Read_JedecID, -1, 4, RX, 0, buf)) return (ERROR);
  dev->ManID = buf[0];
  dev->Type = buf[1];
  dev->BlockCount = w25q[((buf[2] - 1) & 0x0f)];  
  dev->Capacity = dev->BlockCount * W25Qxx_BLOCK_SIZE;

  if (spi_transfer(dev, W25Qxx_Read_UniqueID, -1, 12, RX, 0, buf)) return (ERROR);
  dev->UniqID = *(uint64_t*)(&buf[4]);

  if (spi_transfer(dev, W25Qxx_Read_DeviceID, -1, 4, RX, 0, buf)) return (ERROR);
  dev->ID = buf[3];

  
  if (!(dev->ManID == 0xef) && !(dev->Type == 0x40)) ret = ERROR;
  
  /* protect first 32K */
  // uint8_t stub;
  // stub = W25qxx_WriteStatusRegister(dev, 0, (W25Qxx_SEC | W25Qxx_TB | W25Qxx_BP2));
  // spi_transfer(dev, W25Qxx_Read_StatusRegister_1, -1, 1, READ, 0, &stub);
  // printf("%x\n", stub);
  
  spi_dma_unconfigure(dev);
  
  dev->Lock = DISABLE;
  return ret;
}







// -------------------------------------------------------------

ErrorStatus W25qxx_Read(W25qxx_TypeDef* dev, const uint32_t addr, const uint16_t cnt, uint8_t *buf) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  spi_dma_configure(dev);

  uint32_t phy_addr = 0;
  if (w25qxx_busy(dev)) return (ERROR);
  
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
  phy_addr += W25Qxx_PAGE_SIZE * (addr & 0xf);
  
  if (spi_transfer(dev, W25Qxx_FastRead, (phy_addr << 8), cnt, RX, 0, buf)) return (ERROR);

  spi_dma_unconfigure(dev);

  dev->Lock = DISABLE;
  return (SUCCESS);
}





// -------------------------------------------------------------

ErrorStatus W25qxx_Write(W25qxx_TypeDef* dev, uint32_t addr, uint16_t cnt, uint8_t *buf) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  uint8_t pump = 0;
  uint32_t phy_addr = 0;

  spi_dma_configure(dev);

  if (w25qxx_busy(dev)) return (ERROR);
  
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
  phy_addr += W25Qxx_PAGE_SIZE * (addr & 0xf);

  uint32_t i = 0;
  
  for (i = 0; i < (cnt / 256); i++) {
    if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
    if (spi_transfer(dev, W25Qxx_PageProgram, (phy_addr << 8), 256, TX, i, buf)) return (ERROR);
    phy_addr += W25Qxx_PAGE_SIZE;
    if (w25qxx_busy(dev)) return (ERROR);
  }
  
  if (cnt % 256) {
    if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
    if (spi_transfer(dev, W25Qxx_PageProgram, (phy_addr << 8), (cnt % 256), TX, i, buf)) return (ERROR);
    if (w25qxx_busy(dev)) return (ERROR);
  }

  spi_dma_unconfigure(dev);

  dev->Lock = DISABLE;
  return (SUCCESS);
}






// -------------------------------------------------------------

ErrorStatus W25qxx_Erase(W25qxx_TypeDef* dev, uint32_t addr, uint16_t sectors) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  uint8_t pump = 0;
  uint32_t phy_addr = 0;
  phy_addr = W25Qxx_BLOCK_SIZE * ((addr >> 8) & 0xffff);
  
  spi_dma_configure(dev);
  
  if (sectors > 8) {
    if (sectors > 16) {
      // *******************************  
      uint32_t blocks = (sectors / 16);
      for (int i = 0; i < blocks; i++) {
        if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
        if (spi_transfer(dev, W25Qxx_Erase_Block_64, (phy_addr << 8), 0, NOTR, 0, &pump)) return (ERROR);
        phy_addr += W25Qxx_BLOCK_SIZE;
        if (w25qxx_busy(dev)) return (ERROR);
      }
      uint32_t remain_addr = (addr & 0xffffff00) + (blocks << 8);
      if (sectors % 16) {
        if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
        if (W25qxx_Erase(dev, remain_addr, (sectors % 16))) return (ERROR);
        if (w25qxx_busy(dev)) return (ERROR);
      }
      // *******************************  
    } else {
      // ******************************* 
      if (((addr >> 4) & 0xf) > 8) {
        phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
      }

      if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
      if (spi_transfer(dev, W25Qxx_Erase_Block_32, (phy_addr << 8), 0, NOTR, 0, &pump)) return (ERROR);
      uint32_t remain_addr = (addr & 0xffffff80) + (8 << 4);
      if (w25qxx_busy(dev)) return (ERROR);
      
      if (sectors - 8) {
        if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
        if (W25qxx_Erase(dev, remain_addr, sectors - 8)) return (ERROR);
        if (w25qxx_busy(dev)) return (ERROR);
      }
    // *******************************  
    }
  } else {
    // *******************************  
    phy_addr += W25Qxx_SECTOR_SIZE * ((addr >> 4) & 0xf);
    for (int i = 0; i < sectors; i++) {
      if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (ERROR);
      if (spi_transfer(dev, W25Qxx_Erase_Sector, (phy_addr << 8), 0, NOTR, 0, &pump)) return (ERROR);
      phy_addr += W25Qxx_SECTOR_SIZE;
      if (w25qxx_busy(dev)) return (ERROR);
    }
    // *******************************  
  }

  spi_dma_unconfigure(dev);

  dev->Lock = DISABLE;
  return (SUCCESS);
}




// -------------------------------------------------------------

__STATIC_INLINE ErrorStatus w25qxx_busy(W25qxx_TypeDef* dev) {
  uint8_t pump = W25Qxx_BUSY;
  uint32_t tmout = SPI_BUS_TMOUT * 5;

  while (pump & W25Qxx_BUSY) {
    if (spi_transfer(dev, W25Qxx_Read_StatusRegister_1, -1, 1, RX, 0, &pump)) return (ERROR);
    if (!(--tmout)) {
      SPI_Disable(dev->SPIx);
      return (ERROR);
    }
  }

  return (SUCCESS);
}





// -------------------------------------------------------------  

ErrorStatus W25qxx_Reset(W25qxx_TypeDef* dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  uint8_t pump = 0;

  spi_dma_configure(dev);

  if (spi_transfer(dev, W25Qxx_EnableReset, -1, 0, NOTR, 0, &pump)) return (ERROR);
  if (spi_transfer(dev, W25Qxx_ResetProccess, -1, 0, NOTR, 0, &pump)) return (ERROR);
  _delay_ms(10);
  if (spi_transfer(dev, W25Qxx_ReleasePowerDown, -1, 0, NOTR, 0, &pump)) return (ERROR);

  spi_dma_unconfigure(dev);

  dev->Lock = DISABLE;
  return (SUCCESS);
}




/* type: 0 - non-volatile bits, 1 - volatile bits*/
// -------------------------------------------------------------  

uint8_t W25qxx_WriteStatusRegister(W25qxx_TypeDef* dev, uint8_t type, uint8_t status) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (0xff);

  uint8_t pump = 0;

  spi_dma_configure(dev);

  if (type) {
    if (spi_transfer(dev, W25Qxx_Write_StatusNVRegEnable, -1, 0, NOTR, 0, &pump)) return (0xff);
  } else {
    if (spi_transfer(dev, W25Qxx_WriteEnable, -1, 0, NOTR, 0, &pump)) return (0xff);
  }
  
  if (spi_transfer(dev, W25Qxx_Write_StatusRegister_1, -1, 1, TX, 0, &status)) return (0xff);

  /* Skip one trash bytes */
  uint32_t tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->SPIx->SR, SPI_SR_RXNE_Pos))) {
    if (!(--tmout)) {
      spi_dma_unconfigure(dev);
      return (0xff);
    }
  }

  dev->SPIx->DR;

  if (w25qxx_busy(dev)) return (0xff);
  if (spi_transfer(dev, W25Qxx_Read_StatusRegister_1, -1, 1, RX, 0, &pump)) return (0xff);

  spi_dma_unconfigure(dev);

  dev->Lock = DISABLE;
  return (pump);
}




/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
