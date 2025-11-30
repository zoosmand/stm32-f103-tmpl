/* Includes ------------------------------------------------------------------*/
#include "ow.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t lastfork;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE ErrorStatus OneWire_ErrorHandler(OneWireBus_TypeDef*);

__STATIC_INLINE void OneWire_WriteBit(OneWireBus_TypeDef*, uint8_t);

__STATIC_INLINE ErrorStatus OneWire_Enumerate(OneWireDevice_t*);





// -------------------------------------------------------------  

ErrorStatus OneWireBus_Init(OneWireBus_TypeDef* busDev) {
  /* Init GPIO */
  if (busDev->Port > 7) {
    MODIFY_REG(
      busDev->Port->CRH, 
      (0x0f << ((busDev->Pin - 8) * 4)), 
      ((GPIO_GPO_OD | GPIO_IOS_10) << ((busDev->Pin - 8) * 4))
    );
  } else {
    MODIFY_REG(
      busDev->Port->CRL, 
      (0x0f << ((busDev->Pin - 8) * 4)), 
      ((GPIO_GPO_OD | GPIO_IOS_10) << (busDev->Pin * 4))
    );
  }
  
  PIN_H(busDev->Port, busDev->Pin);

  return OneWire_Search(busDev);
}



// -------------------------------------------------------------  

ErrorStatus OneWire_Reset(OneWireBus_TypeDef* busDev) {

  OneWire_High;
  // PIN_L(busDev->Port, busDev->Pin);
  _delay_us(580);
  OneWire_Low;
  // PIN_H(busDev->Port, busDev->Pin);
  _delay_us(15);
  
  int i = 0;
  ErrorStatus status = ERROR;

  while (i++ < 240) {
    // if (!(PIN_LEVEL(busDev->Port, busDev->Pin))) {
    if (!OneWire_Level) {
      status = SUCCESS;
      break;
    }
    _delay_us(1);
  }

  /* to prevent non pulled-up pin to response */
  if (i == 1) {
    status = ERROR;
  } else {
    _delay_us(580 - i);
  }

  return status;
}


// -------------------------------------------------------------  

__STATIC_INLINE void OneWire_WriteBit(OneWireBus_TypeDef* busDev, uint8_t bit) {
  OneWire_High;
  if (bit) {
    _delay_us(6);
    OneWire_Low;
    _delay_us(64);
  } else {
    _delay_us(60);
    OneWire_Low;
    _delay_us(10);
  }
}


// ------------------------------------------------------------- 

void OneWire_WriteByte(OneWireBus_TypeDef* busDev, uint8_t byte) {
  for (int i = 0; i < 8; i++) {
    OneWire_WriteBit(busDev, byte & 0x01);
    byte >>= 1;
  }
}


// -------------------------------------------------------------  

uint8_t OneWire_ReadBit(OneWireBus_TypeDef* busDev) {
  OneWire_High;
  _delay_us(6);
  OneWire_Low;
  _delay_us(9);
  uint8_t level = OneWire_Level;
  _delay_us(55);
  
  return level;
}


// -------------------------------------------------------------  

void OneWire_ReadByte(OneWireBus_TypeDef* busDev, uint8_t* data) {
  for (int i = 0; i < 8; i++) {
    *data >>= 1;
    *data |= (OneWire_ReadBit(busDev)) ? 0x80 : 0;
  }
}


// -------------------------------------------------------------  

uint8_t OneWire_CRC8(uint8_t crc, uint8_t byte) {
  // 0x8c - it is a bit reverse of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
		crc = ((crc ^ (byte >> i)) & 0x01) ? ((crc >> 1) ^ 0x8c) : (crc >> 1);
	}

  return crc;
}


// -------------------------------------------------------------  

__STATIC_INLINE ErrorStatus OneWire_ErrorHandler(OneWireBus_TypeDef*) {
  return (ERROR);
}






// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------







__STATIC_INLINE ErrorStatus OneWire_Enumerate(OneWireDevice_t* dev) {
  if (!lastfork) return (1);
  
	if (OneWire_Reset((OneWireBus_TypeDef*)dev->ParentBusPtr)) return (ERROR);
  uint8_t* addr = dev->Addr;
  
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
	OneWire_WriteByte((OneWireBus_TypeDef*)dev->ParentBusPtr, SearchROM);
  
	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OneWire_ReadBit((OneWireBus_TypeDef*)dev->ParentBusPtr);
    bit1 = OneWire_ReadBit((OneWireBus_TypeDef*)dev->ParentBusPtr);
    
		if (!bit0) {
      if (!bit1) {
        if (i < lastfork) {
          if (prev & 1) {
            curr |= 0x80;
					} else {
            fork = i;
					}
				} else if (i == lastfork) {
          curr |= 0x80;
        } else {
          fork = i;
        }
			}
		} else {
      if (!bit1) {
        curr |= 0x80;
			} else {
        return (ERROR);
			}
		}
    
		OneWire_WriteBit((OneWireBus_TypeDef*)dev->ParentBusPtr, (curr & 0x80));
    
		if (!bp) {
      *addr = curr;
			curr = 0;
			addr++;
			prev = *addr;
			bp = 8;
		} else {
      prev >>= 1;
			curr >>= 1;
		}
    bp--;
	}
	lastfork = fork;
  return (SUCCESS);  
}


// -------------------------------------------------------------

ErrorStatus OneWire_Search(OneWireBus_TypeDef* busDev) {

  if (OneWire_Reset(busDev)) return (ERROR);

  lastfork = 65;
  for (uint8_t i = 0; i < busDev->Count; i++) {
    if (OneWire_Enumerate(&busDev->Devs[i])) {
      break;
    } else {
      busDev->Devs[i].Family = busDev->Devs[i].Addr[0];
      if (busDev->Devs[i].Family == DS18B20_Family_Code) {
        busDev->Devs[i].Type = 't';
      }
    }
  }

  return (SUCCESS);
}


// -------------------------------------------------------------

uint8_t OneWire_ReadPowerSupply(OneWireDevice_t* dev) {
  OneWire_MatchROM(dev);
  OneWire_WriteByte((OneWireBus_TypeDef*)dev->ParentBusPtr, ReadPowerSupply);
  
  return !OneWire_ReadBit((OneWireBus_TypeDef*)dev->ParentBusPtr);
}


/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
ErrorStatus OneWire_MatchROM(OneWireDevice_t* dev) {
  if (OneWire_Reset((OneWireBus_TypeDef*)dev->ParentBusPtr)) return (ERROR);
  
  OneWire_WriteByte((OneWireBus_TypeDef*)dev->ParentBusPtr, MatchROM);
  for (uint8_t i = 0; i < 8; i++) {
    OneWire_WriteByte((OneWireBus_TypeDef*)dev->ParentBusPtr, dev->Addr[i]);
  }

  return (SUCCESS);
}


