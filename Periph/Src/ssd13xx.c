/*
 *
 *
 *
 *
 */


 /* Includes ------------------------------------------------------------------*/
#include "ssd13xx.h"




/* Private variables ---------------------------------------------------------*/

const static uint8_t ssd13xxInitParams[24] = {
  0xae,       // set display off
  0xd5, 0x80, // set display clock divide; oscillator frequency in [7:4]; ratio in [3:0]
  0xa8, 0x3c, // set MUX ratio; ratio in [5:0]
  0xd3, 0x00, // set display offset; vertical shift in [5:0]
  0x40,       // set start line address; address in [5:0]
  0x00,       // set low column address; address in [3:0]
  0x8d, 0x14, // enable charge pump; enable in [2]; [7]=0 and [0]=0 set 7.5V charge pumpmode
  0xa1,       // set segment re-map; remap in [0]; [0]=1 then 127->0; [0]=0 then 0->127 
  0xc0,       // set COM csan direction; direction in [3]; [3]=0 then COM[0]->COM[-1]; [3]=1 then COM[n-1]->COM[0]
  0xda, 0x12, // set com pins hardware configuration; configuratino in [5:4]
  0x81, 0x7f, // set contrast control register
  0xd9, 0x88, // set pre-charge period; phase 1 in [7:4]; phase 2 in [3:0] 
  0xdb, 0x20, // set vcomh, vcomh level in [5:4]
  0xa4,       // entire display on; [0]=1 then ignore RAM [0]=0 then follow RAM
  0xa6,       // set normal/inverse display; [0]=1 then inverse; [0]=0 then normal
  0xaf        // set display on
};

const static uint8_t ssd13xxClrDspl[8] = {
  0x20, 0x00,       // set horizontal addressing mode 
  0x21, 0x00, 0x7f, // set column address from 0 to 127
  0x22, 0x00, 0x07  // set page address from 0 to 7
};

const static uint8_t ssd13xxInitCurPosParams_5x7[8] = {
  0x20, 0x00, 
  0x21, 0x00, 0x05, 
  0x22, 0x06, 0x06
};
const static uint8_t ssd13xxInitCurPosParams_10x14[8] = {
  0x20, 0x01, 
  0x21, 0x00, 0x0b, 
  0x22, 0x05, 0x06
};

// const static uint8_t ssd13xxInitCurPosParams[8] = {
//   0x20, 0x00, 
//   0x21, 0x00, 0x05, 
//   0x22, 0x06, 0x06
// };

static uint8_t ssd13xxCurrentCurPosParams[8];


/* Private function prototypes -----------------------------------------------*/

/**
 * @brief  Writes/Sends a command to SSD13xx display.
 * @param  cmd: ssd13xx command
 * @retval status of operation
 */
static ErrorStatus ssd13xx_send_command(SSD13xx_TypeDef*, uint8_t);

/**
 * @brief  Writes/Sends a text buffer to SSD13xx display.
 * @param  buf: pointer to the character/text buffer
 * @param  len: buffer length
 * @param  pos: pointer to the cursor position
 * @retval status of operation
 */
static ErrorStatus ssd13xx_send_buffer(SSD13xx_TypeDef*, const uint8_t*, uint16_t, uint8_t*);

/**
  * @brief   Adjusts I2C bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE void i2c_dma_configure(SSD13xx_TypeDef*);

/**
  * @brief   Adjusts I2C bus according to device requirements.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE ErrorStatus i2c_dma_unconfigure(SSD13xx_TypeDef*);

/**
  * @brief  Transmits in master mode an amount of data.
  * @param  dev Pointer to the target device structure
  * @param  buf Pointer to data buffer
  * @param  len Amount of data to be sent
  * @return transmit status
  */
__STATIC_INLINE ErrorStatus i2c_master_send(SSD13xx_TypeDef*);

/**
  * @brief  Clears the diven device buffer.
  * @param  dev Pointer to the target device structure
  * @return transmit status
  */
__STATIC_INLINE void ssd13xx_clear_buffer(SSD13xx_TypeDef*);





/******************************************************************************/
// ----------------------------------------------------------------------------

__STATIC_INLINE void i2c_dma_configure(SSD13xx_TypeDef* dev) {

  /* Disable DMA transfer */
  PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  /* Clear ACK bnit */
  PREG_CLR(dev->I2Cx->CR1, I2C_CR1_ACK_Pos);
  /* configure DMA, Channel6 - TX */
  /* set priority high*/
  /* set memory to increment */
  /* set dir bit, means reading from the bus */
  MODIFY_REG(dev->DMAxTx->CCR, (
        DMA_CCR_PL_Msk
      | DMA_CCR_MINC_Msk
      | DMA_CCR_DIR_Msk
    ), (
        DMA_CCR_PL_1 
      | DMA_CCR_MINC 
      | DMA_CCR_DIR
    )
  );
  
  /* set peripheral address */
  dev->DMAxTx->CPAR = (uint32_t)&dev->I2Cx->DR;
  
}


// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus i2c_dma_unconfigure(SSD13xx_TypeDef* dev) {
  
  /* Clear correspondent DMA interrupt flags */
  dev->DMAx->IFCR = (DMA_IFCR_CHTIF6_Msk | DMA_IFCR_CGIF6_Msk | DMA_IFCR_CTCIF6_Msk);
  /* Clear DMA configuration */
  dev->DMAxTx->CPAR   = 0UL;
  dev->DMAxTx->CMAR   = 0UL;
  dev->DMAxTx->CNDTR  = 0UL;
  dev->DMAxTx->CCR    = 0UL;
  
  /* Disable DMA transfer on the bus */
  PREG_CLR(dev->I2Cx->CR2, I2C_CR2_DMAEN);
  
  /* Disable I2C bus */
  I2C_Stop(dev->I2Cx);
  
  return (ERROR);
}


// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus i2c_master_send(SSD13xx_TypeDef* dev) {
  
  uint32_t tmout;
  
  /* Adjust I2C bus with DMA transfer */
  i2c_dma_configure(dev);
  
  /* Set counter */
  dev->DMAxTx->CNDTR = dev->BufSize;
  /* Set buffer pointer address */
  dev->DMAxTx->CMAR = (uint32_t)dev->BufPtr;
  /* Enable receiving DMA transfer */
  PREG_SET(dev->I2Cx->CR2, I2C_CR2_DMAEN_Pos);

  /* Start bus transmission */
  if (I2C_Start(dev->I2Cx)) { return (i2c_dma_unconfigure(dev)); }
  if (I2C_SendAddress(dev->I2Cx, dev->I2C_Address, TX)) { return (i2c_dma_unconfigure(dev)); }
  
  /* Enable DMS transfer*/
  PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  
  /* Wait for transfer is complete */
  tmout = I2C_BUS_TMOUT * 1000;
  while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF6_Pos))) {
    if (!(--tmout)) return (i2c_dma_unconfigure(dev));
  }
  /* Verify after transferring if transmit buffer is empty */
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(dev->I2Cx->SR1, I2C_SR1_BTF_Pos))) {
    if (!(--tmout)) { return (i2c_dma_unconfigure(dev)); }
  }
  
  /* Unconfigure DMA and stop the I2C bus */
  i2c_dma_unconfigure(dev);
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void ssd13xx_clear_buffer(SSD13xx_TypeDef* dev) {
  for (uint16_t i = 0; i < dev->BufSize; i++) {
    dev->BufPtr[i] = 0;
  }
}




// ----------------------------------------------------------------------------

ErrorStatus SSD13xx_Init(SSD13xx_TypeDef* dev) {

  /* Initial delay according ssd1315 documentation */
  _delay_ms(15);

  /* --- Initialization commands --- */
  for (uint8_t i = 0; i < sizeof(ssd13xxInitParams); i++) {
    if (ssd13xx_send_command(dev, ssd13xxInitParams[i])) return (ERROR);
  }
    
  /* --- Clear display --- */
  for (uint8_t i = 0; i < sizeof(ssd13xxClrDspl); i++) {
    if (ssd13xx_send_command(dev, ssd13xxClrDspl[i])) return (ERROR);
  }

  dev->BufSize = (8 * 128 + 1);
  ssd13xx_clear_buffer(dev);
  dev->BufPtr[0] = SSD13xx_DATA_CTRL;

  if (i2c_master_send(dev)) return (ERROR);
  
  /* --- Initialize the init cursor position --- */
  putc_dspl('\n');
 
  return (SUCCESS);
}




// ----------------------------------------------------------------------------

static ErrorStatus ssd13xx_send_command(SSD13xx_TypeDef* dev, uint8_t cmd) {

  uint8_t cmdBuf[2] = { SSD13xx_CMD_CTRL, cmd };

  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, cmdBuf, 2)) return (ERROR);

  return (SUCCESS);
}




// ----------------------------------------------------------------------------

static ErrorStatus ssd13xx_send_buffer(SSD13xx_TypeDef* dev, const uint8_t* buf, uint16_t len, uint8_t* pos) {
  
  /* --- Set cursor position --- */
  for (uint8_t i = 0; i < 8; i++) {
    if (ssd13xx_send_command(dev, pos[i])) return (ERROR);
  }
  
  uint8_t step_left, step_up;
  
  switch (len) {
    case 24:
    step_left = 12;
    step_up = 2;
    break;
    
    default:
    step_left = 6;
    step_up = 1;
    break;
  }
  
  if (((pos[4] + 6) & 0x7f) < pos[3]) {
    pos[3] = 0x00;
    pos[4] = step_left;
    pos[6] = (pos[6] - step_up) & 0x07;
    pos[7] = (pos[7] - step_up) & 0x07;
  } else {
    pos[3] = pos[4] + 1;
    pos[4] = pos[4] + step_left;
  }
  // if (((pos[4] + 12) & 0x7f) < pos[3]) {
  //   pos[3] = 0x00;
  //   pos[4] = 0x0b;
  //   pos[6] = (pos[6] - 2) & 0x07;
  //   pos[7] = (pos[7] - 2) & 0x07;
  // } else {
  //   pos[3] = pos[4] + 1;
  //   pos[4] = pos[4] + 12;
  // }
  

  dev->BufSize = (len + 1); // data and one byte for a command
  // ssd13xx_clear_buffer(dev); // it is not necessary cleaning buffer here though it might help in some cases
  dev->BufPtr[0] = SSD13xx_DATA_CTRL;

  for (uint16_t i = 0; i < len; i++) { dev->BufPtr[i + 1] = buf[i]; }
  
  if (i2c_master_send(dev)) return (ERROR);
  
  return (SUCCESS);
}
    
    
    
    
// ----------------------------------------------------------------------------

int __attribute__((weak)) putc_dspl_ssd(char ch) {
  
  if ((ch != 0x0a) && (ch != 0x0d)) {
    SSD13xx_TypeDef* dsplDev = Get_SsdDiplayDevice(SSD_DSPL_MODEL);

    #if SSD_DSPL_FONT == 1014
      ssd13xx_send_buffer(dsplDev, font_dot_10x14[(((uint8_t)ch) - 32)], sizeof(font_dot_10x14_t), ssd13xxCurrentCurPosParams);
    #elif SSD_DSPL_FONT == 57
      ssd13xx_send_buffer(dsplDev, font_dot_5x7[(((uint8_t)ch) - 32)], sizeof(font_dot_5x7_t), ssd13xxCurrentCurPosParams);
    #endif
  } else {
    #if SSD_DSPL_FONT == 1014
      for (uint8_t i = 0; i < sizeof(ssd13xxInitCurPosParams_10x14); i++) {
        ssd13xxCurrentCurPosParams[i] = ssd13xxInitCurPosParams_10x14[i];
      }
    #elif SSD_DSPL_FONT == 57
      for (uint8_t i = 0; i < sizeof(ssd13xxInitCurPosParams_5x7); i++) {
        ssd13xxCurrentCurPosParams[i] = ssd13xxInitCurPosParams_5x7[i];
      }
    #endif
  }
  return (0);
}
