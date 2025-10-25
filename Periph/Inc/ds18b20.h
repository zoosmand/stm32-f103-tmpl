#ifndef __DS18B20_H
#define __DS18B20_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

int DS18B20_GetTemperatureMeasurment(OneWireDevice_t*);

/* Private defines -----------------------------------------------------------*/
#define AlarmSearch     0xec
#define ConvertT        0x44
#define WriteScratchpad 0x4e
#define ReadScratchpad  0xbe
#define CopyScratchpad  0x48
#define RecallE         0xb8
#define ReadPowerSupply 0xb4


#ifdef __cplusplus
}
#endif

#endif /* __DS18B20_H */
