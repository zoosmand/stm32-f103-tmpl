#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint8_t   addr[8];
  uint8_t   spad[9];
} OneWireDevice_t;


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
extern __attribute__((section(".cron"))) uint32_t _OWREG_;

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

int OneWire_Reset(void);

void OneWire_WriteByte(uint8_t*);

uint8_t OneWire_ReadBit(void);

void OneWire_ReadByte(uint8_t*);

void OneWire_CRC8(uint8_t*, uint8_t);

void OneWire_Search(void);


OneWireDevice_t* Get_OwDevices(void);


/* Exported defines -----------------------------------------------------------*/
#define SearchROM       0xf0
#define ReadROM         0x33
#define MatchROM        0x55
#define SkipROM         0xcc


/* Private defines -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __ONEWIRE_H */
