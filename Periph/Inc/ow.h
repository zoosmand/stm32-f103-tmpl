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
} ow_device_t;


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
extern __attribute__((section(".cron"))) uint32_t _OWREG_;

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void OW_Reset(void);
void OW_WriteBit(uint8_t);
void OW_Write(uint8_t*);
uint8_t OW_ReadBit(void);
void OW_Read(uint8_t*);
void OW_CRC8(uint8_t*, uint8_t);
void OW_Search(void);
int8_t OW_Error_Handler(void);
uint8_t* Get_AddrBuf(void);
ow_device_t* Get_OwDevices(void);

/* Private defines -----------------------------------------------------------*/
#define SearchROM       0xf0
#define ReadROM         0x33
#define MatchROM        0x55
#define SkipROM         0xcc

#define OW_PORT   GPIOB
#define OW_PIN    GPIO_PIN_12_Pos

#define _OLF_     0 // On-Line Flag, i.e. an OW device answered on reset 
#define _CRCERF_  1 // CRC Error Flag 


#ifdef __cplusplus
}
#endif

#endif /* __ONEWIRE_H */
