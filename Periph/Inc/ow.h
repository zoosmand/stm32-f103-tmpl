#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
extern __attribute__((section(".cron"))) uint32_t _OWREG_;

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Inializes OneWire Bus pulling reset.
 * @return  status of operation
 */
int OneWireBus_Init(void);

int OneWire_Reset(void);

void OneWire_WriteByte(uint8_t);

uint8_t OneWire_ReadBit(void);

void OneWire_ReadByte(uint8_t*);

uint8_t OneWire_CRC8(uint8_t, uint8_t);

int OneWire_Search(void);

/**
 * @brief   Defines parasitic powered devices on OnWire bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of power supply
 */
uint8_t OneWire_ReadPowerSupply(uint8_t*);

/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
int OneWire_MatchROM(uint8_t*);


OneWireDevice_t* Get_OwDevices(void);


/* Exported defines -----------------------------------------------------------*/
#define SearchROM       0xf0
#define ReadROM         0x33
#define MatchROM        0x55
#define SkipROM         0xcc


#define OneWire_Low     PIN_H(OneWire_PORT, OneWire_PIN)
#define OneWire_High    PIN_L(OneWire_PORT, OneWire_PIN)
#define OneWire_Level   (PIN_LEVEL(OneWire_PORT, OneWire_PIN))


/* Private defines -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __ONEWIRE_H */
