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

#define SSD1306_I2C_ADDR    0x78 // SSD1306 I2C Address
#define SSD1315_I2C_ADDR    0x3c // SSD1315 I2C Address
#define SSD13xx_Co_BIT      7 // Co bit
#define SSD13xx_DC_BIT      6 // DC bit (1 - data, 0 - command)



//////////////////////////////////////////////////////////////////////////
int SSD13xx_Init(I2C_TypeDef*);
int __attribute__((weak)) putc_dspl_5x7(char);
int __attribute__((weak)) putc_dspl_10x14(char);


#ifdef __cplusplus
}
#endif
#endif // __SSD13XX_H
