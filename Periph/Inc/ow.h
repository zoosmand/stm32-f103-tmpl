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
ErrorStatus OneWireBus_Init(OneWireBus_TypeDef*);

ErrorStatus OneWire_Reset(OneWireBus_TypeDef*);

void OneWire_WriteByte(OneWireBus_TypeDef*, uint8_t);

uint8_t OneWire_ReadBit(OneWireBus_TypeDef*);

void OneWire_ReadByte(OneWireBus_TypeDef*, uint8_t*);

uint8_t OneWire_CRC8(uint8_t, uint8_t);

ErrorStatus OneWire_Search(OneWireBus_TypeDef*);

/**
 * @brief   Defines parasitic powered devices on OnWire bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of power supply
 */
uint8_t OneWire_ReadPowerSupply(OneWireDevice_t*);

/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
ErrorStatus OneWire_MatchROM(OneWireDevice_t*);


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
