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

#ifndef SSD_DSPL_MODEL
#define SSD_DSPL_MODEL 1315
#endif /* #ifndef SSD_DSPL_MODEL */

#ifndef SSD_DSPL_FONT
#define SSD_DSPL_FONT 1014
#endif /* #ifndef SSD_DSPL_FONT */


#define SSD1306_I2C_ADDR    0x78 // SSD1306 I2C Address
#define SSD1315_I2C_ADDR    0x3c // SSD1315 I2C Address
#define SSD13xx_Co_BIT      7 // Co bit
#define SSD13xx_DC_BIT      6 // DC bit (1 - data, 0 - command)

#define SSD13xx_DATA_CTRL   (uint8_t)((1 << SSD13xx_DC_BIT) & ~(1 << SSD13xx_Co_BIT))
#define SSD13xx_CMD_CTRL    (uint8_t)(~(1 << SSD13xx_Co_BIT) & ~(1 << SSD13xx_DC_BIT))


//////////////////////////////////////////////////////////////////////////
ErrorStatus SSD13xx_Init(SSD13xx_TypeDef*);
int __attribute__((weak)) putc_dspl_ssd(char);


#ifdef __cplusplus
}
#endif
#endif // __SSD13XX_H
