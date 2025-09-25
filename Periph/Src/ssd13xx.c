/*
 *
 *
 *
 *
 */


 /* Includes ------------------------------------------------------------------*/
#include "ssd13xx.h"



/* Private variables ---------------------------------------------------------*/
static I2C_TypeDef* I2C_Instance;

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
  0xa4,       // entire display on; [0]=1 then ignore RAMl [0]=0 then follow RAM
  0xa6,       // set normal/inverse display; [0]=1 then inverse; [0]=0 then normal
  0xaf        // set display on
};

const static uint8_t ssd13xxClrDspl[8] = {
  0x20, 0x00,       // set horizontal addressing mode 
  0x21, 0x00, 0x7f, // set column address from 0 to 127
  0x22, 0x00, 0x07  // set page address from 0 to 7
};

const static uint8_t ssd13xxInitCurPosParams[8] = {
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
 * @brief  Writes/Sends a command to SSD13xx display
 * @param  cmd: ssd13xx command
 * @retval (int) status of operation
 */
static int sSD13xx_WriteCommand(uint8_t cmd);

/**
 * @brief  Writes/Sends a data to SSD13xx display
 * @param  data: ssd13xx data byte
 * @retval (int) status of operation
 */
static int sSD13xx_WriteDataByte(uint8_t data);

/**
 * @brief  Writes/Sends a text buffer to SSD13xx display
 * @param  buf: pointer to the character/text buffer
 * @param  len: buffer length
 * @param  pos: pointer to the cursor position
 * @retval (int) status of operation
 */
static int sSD13xx_WriteBuf(const uint8_t* buf, uint16_t len, uint8_t* pos);






/******************************************************************************/

// ----------------------------------------------------------------------------
int SSD13xx_Init(I2C_TypeDef* i2c) {

  I2C_Instance = i2c;
  
  /* Initial delay according ssd1315 documentation */
  _delay_ms(15);

  /* --- Initialization commands --- */
  for (uint8_t i = 0; i < sizeof(ssd13xxInitParams); i++) {
    if (sSD13xx_WriteCommand(ssd13xxInitParams[i])) return (1);
  }

    /* --- Clear display --- */
  for (uint8_t i = 0; i < sizeof(ssd13xxClrDspl); i++) {
    if (sSD13xx_WriteCommand(ssd13xxClrDspl[i])) return (1);
  }

  if (I2C_Start(I2C_Instance)) return (1);
  _delay_us(1);
  /* --- Control ACK on sending address --- */
  if (I2C_SendAddress(I2C_Instance, SSD1315_I2C_ADDR)) return (1);
  /* --- Send control byte --- */
  if (sSD13xx_WriteDataByte((uint8_t)((1 << SSD13xx_DC_BIT) & ~(1 << SSD13xx_Co_BIT)))) return (1);

  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t y = 0; y < 128; y++) {
      if (sSD13xx_WriteDataByte(0x00)) return (1);
    }
  }
  I2C_Stop(I2C_Instance);

  // _delay_ms(10);

  // for (uint8_t i = 0; i < sizeof(ssd13xxInitCurPosParams); i++) {
  //   ssd13xxCurrentCurPosParams[i] = ssd13xxInitCurPosParams[i];
  // }

  // char ch = 'D';
  // sSD13xx_WriteBuf(font_dot_10x14[(((uint8_t)ch) - 32)], sizeof(font_dot_10x14_t), ssd13xxCurrentCurPosParams);
 
  return (0);
}




// ----------------------------------------------------------------------------
static int sSD13xx_WriteCommand(uint8_t cmd) {

  if (I2C_Start(I2C_Instance)) return (1);
  _delay_us(1);

  /* --- Control ACK on sending address --- */
  if (I2C_SendAddress(I2C_Instance, SSD1315_I2C_ADDR)) return (1);
  
  /* --- Send control byte --- */
  if (I2C_WriteByte(I2C_Instance, (uint8_t)(~(1 << SSD13xx_Co_BIT) & ~(1 << SSD13xx_DC_BIT)))) return (1);
  
  /* --- Send data byte --- */
  if (I2C_WriteByte(I2C_Instance, cmd)) return (1);

  I2C_Stop(I2C_Instance);

  return (0);
}




// ----------------------------------------------------------------------------
static int sSD13xx_WriteDataByte(uint8_t data) {
  /* --- Send data byte --- */
  if (I2C_WriteByte(I2C_Instance, data)) return (1);
  
  return (0);
}




// ----------------------------------------------------------------------------
static int sSD13xx_WriteBuf(const uint8_t* buf, uint16_t len, uint8_t* pos) {

  /* --- Set cursor position --- */
  for (uint8_t i = 0; i < 8; i++) {
    if (sSD13xx_WriteCommand(pos[i])) return (1);
  }
  
  if (((pos[4] + 12) & 0x7f) < pos[3]) {
    pos[3] = 0x00;
    pos[4] = 0x0b;
    pos[6] = (pos[6] - 2) & 0x07;
    pos[7] = (pos[7] - 2) & 0x07;
  } else {
    pos[3] = pos[4] + 1;
    pos[4] = pos[4] + 12;
  }

  /* --- Write the buffer --- */
  if (I2C_Start(I2C_Instance)) return (1);
  _delay_us(1);

  /* --- Control ACK on sending address --- */
  if (I2C_SendAddress(I2C_Instance, SSD1315_I2C_ADDR)) return (1);

  /* --- Send control byte --- */
  if (sSD13xx_WriteDataByte((uint8_t)((1 << SSD13xx_DC_BIT) & ~(1 << SSD13xx_Co_BIT)))) return (1);

  /* --- Send buffer data --- */
  for (uint8_t i = 0; i < len; i++) {
    if (sSD13xx_WriteDataByte(buf[i])) return (1);
  }
  I2C_Stop(I2C_Instance);

  return (0);
}

