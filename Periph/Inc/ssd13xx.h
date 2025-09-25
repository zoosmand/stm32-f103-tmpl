/*
 *
 *
 *
 *
 */
#ifndef __SSD13XX_H
#define __SSD13XX_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

//////////////////////////////////////////////////////////////////////////
// -------- Landscape --------
#define SSD13xx_LCDHEIGHT           64
#define SSD13xx_LCDWIDTH            128
#define SSD13xx_LCDCOLUMNS          128
#define SSD13xx_LCDPAGES            8

   

//////////////////////////////////////////////////////////////////////////
// // ---- SSD13xx Commands
// #define SSD13xx_ADDR      0x3d // SSD13XX I2C Address
// #define SSD13xx_CMD       0x00
// #define SSD13xx_DATA      0x40
// // ---- SSD13xx Commands
// #define SSD13xx_MEM_ADDR  0x20 // followed by the memory mode value
// #define SSD13xx_CASET     0x21 // followed by column start address value, then by column end address value
// #define SSD13xx_PASET     0x22 // followed by page start address value, theb by page end address value
// #define SSD13xx_CONTRAST  0x81 // followed by the byte contrast value
// #define SSD13xx_PUMP      0x8d // followed by the charge pump value
// #define SSD13xx_SEG0      0xa0 // remap address 0 to SEG0
// #define SSD13xx_SEG127    0xa1 // remap address 127 to SEG0
// #define SSD13xx_ON_RAM    0xa4 // displays RAM contents
// #define SSD13xx_ON_RESET  0xa5 // ignore RAM contents
// #define SSD13xx_NORMAL    0xa6
// #define SSD13xx_INVERSE   0xa7
// #define SSD13xx_MUX       0xa8 // followed by the multiplex ratio value
// #define SSD13xx_OLED_OFF  0xae
// #define SSD13xx_OLED_ON   0xaf
// #define SSD13xx_COM_NOR   0xc0 // Normal mode from COM0 to COM[x]
// #define SSD13xx_COM_REM   0xc8 // Remapped mode from COM[x] to COM0
// #define SSD13xx_OFFSET    0xd3 // followed by the offset value
// #define SSD13xx_DIV       0xd5 // followed by the clock divide value
// #define SSD13xx_PCHARGE   0xd9 // followed by the pre-charge value
// #define SSD13xx_HW_CONF   0xda // followed by the hardware configuration pins value
// #define SSD13xx_VLEVEL    0xdb // followed by the vertical deselect level value
// #define SSD13xx_NOP       0xe3

// #define SSD13xx_Black     0x00
// #define SSD13xx_White     0x01

// // Memory mode value definition
// #define _SSD_1306_HAM     0x00 // Horizontal addressing mode
// #define _SSD_1306_VAM     0x01 // Vertical addressing mode
// #define _SSD_1306_PAM     0x10 // Page addressing mode


#define SSD1306_I2C_ADDR    0x78 // SSD1306 I2C Address
#define SSD1315_I2C_ADDR    0x3c // SSD1315 I2C Address
#define SSD13xx_Co_BIT      7 // Co bit
#define SSD13xx_DC_BIT      6 // DC bit (1 - data, 0 - command)


//////////////////////////////////////////////////////////////////////////
int SSD13xx_Init(I2C_TypeDef* i2c);
// void SSD13xx_FillRectangle(const uint8_t c0, const uint8_t c1, const uint8_t p0, const uint8_t p1, const uint8_t patch);
// void SSD13xx_FillRectangle2(const uint8_t x, const uint8_t y, uint8_t w, uint8_t h, const uint8_t patch);
// void SSD13xx_DrawSymbol(uint8_t *c, uint8_t *p, const font_t *font, font_size_t *size, uint8_t pos);
// void SSD13xx_PrintString(uint8_t *c, uint8_t *p, const font_t *font, font_size_t *size, const char *buf, uint8_t wrap);
// void SSD13xx_PrintDate(const char* buf);
// void SSD13xx_PrintTime(const char* buf);


#ifdef __cplusplus
}
#endif
#endif // __SSD13XX_H
