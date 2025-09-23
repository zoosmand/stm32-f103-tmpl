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
#define OW_PORT   GPIOB
#define OW_PIN    GPIO_PIN_12


#define PIN_Low  PIN_H(OW_PORT, OW_PIN)
#define PIN_High PIN_L(OW_PORT, OW_PIN)
#define PIN_Level (PIN_LEVEL(OW_PORT, OW_PIN))


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Run the reset condition on the OneWire bus.
 * @retval  (int) status of operation
 */
int OW_Reset(void);

/**
 * @brief   Write a byte into OneWire bus.
 * @param   data a byte to write
 * @retval  none
 */
void OW_Write(uint8_t);

/**
 * @brief   Reads a bit from OneWire bus.
 * @retval  (uint8_t) bit to be read
 */
uint8_t OW_ReadBit(void);

/**
 * @brief   Reads a byte from OneWire bus.
 * @retval  (uint8_t) byte to be read
 */
uint8_t OW_Read(void);

/**
 * @brief   Calculates CRC of a OneWire device.
 * @param   crc current CRC value
 * @param   data source value
 * @retval  (uint8_t) calculated CRC value
 */
uint8_t OW_CRC8(uint8_t, uint8_t);

/**
 * @brief   Defines parasitic powered devices on OnWire bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of power supply
 */
uint8_t OW_ReadPowerSupply(uint8_t*);

/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
int OW_MatchROM(uint8_t* addr);


int OW_Error_Handler(void);

int OW_Search(void);

ow_device_t* Get_OwDevices(void);



/* Private defines -----------------------------------------------------------*/
#define SearchROM       0xf0
#define ReadROM         0x33
#define MatchROM        0x55
#define SkipROM         0xcc

// #define _OLF_     0 // On-Line Flag, i.e. an OW device answered on reset 
// #define _CRCERF_  1 // CRC Error Flag 


#ifdef __cplusplus
}
#endif

#endif /* __ONEWIRE_H */
