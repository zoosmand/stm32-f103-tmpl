#ifndef __DS18B20_H
#define __DS18B20_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
//#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "ow.h"

/* Private typedef -----------------------------------------------------------*/
//typedef struct {
//  struct OneWireDevTypeDef  *ow_common;
//  uint16_t                  ts_temp_data;
//  uint16_t                  ts_ub_data;
//  uint8_t                   ts_conf_data;
//  uint8_t                   ts_crc_data;
//} TempDevTypeDef;


/* Exported types ------------------------------------------------------------*/
extern uint32_t _OWREG_;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

int DS18B20_GetTemperatureMeasurment(void);

/* Private defines -----------------------------------------------------------*/
#define AlarmSearch     0xec
#define ConvertT        0x44
#define WriteScratchpad 0x4e
#define ReadScratchpad  0xbe
#define CopyScratchpad  0x48
#define RecallE         0xb8
#define ReadPowerSupply 0xb4

/* temperature.c -------------------------------------------------------------*/
void TempCollect(void);

#ifdef __cplusplus
}
#endif

#endif /* __DS18B20_H */
