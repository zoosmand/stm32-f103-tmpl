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
__STATIC_INLINE ErrorStatus OneWire_ErrorHandler(void);

__STATIC_INLINE void OneWire_WriteBit(uint8_t);




// -------------------------------------------------------------  

ErrorStatus OneWireBus_Init(OneWireBus_TypeDef* busDev) {
  return OneWire_Search(busDev);
}



// -------------------------------------------------------------  

ErrorStatus OneWire_Reset(void) {

  OneWire_High;
  _delay_us(580);
  OneWire_Low;
  _delay_us(15);
  
  int i = 0;
  ErrorStatus status = ERROR;

  while (i++ < 240) {
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

__STATIC_INLINE void OneWire_WriteBit(uint8_t bit) {
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

void OneWire_WriteByte(uint8_t byte) {
  for (int i = 0; i < 8; i++) {
    OneWire_WriteBit(byte & 0x01);
    byte >>= 1;
  }
}


// -------------------------------------------------------------  

uint8_t OneWire_ReadBit(void) {
  OneWire_High;
  _delay_us(6);
  OneWire_Low;
  _delay_us(9);
  uint8_t level = OneWire_Level;
  _delay_us(55);
  
  return level;
}


// -------------------------------------------------------------  

void OneWire_ReadByte(uint8_t* data) {
  for (int i = 0; i < 8; i++) {
    *data >>= 1;
    *data |= (OneWire_ReadBit()) ? 0x80 : 0;
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

__STATIC_INLINE ErrorStatus OneWire_ErrorHandler(void) {
  return (ERROR);
}






// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------







__STATIC_INLINE int OneWire_Enumerate(uint8_t* addr) {
  if (!lastfork) return (1);
  
	if (OneWire_Reset()) return (1);
  
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
	OneWire_WriteByte(SearchROM);
  
	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OneWire_ReadBit();
    bit1 = OneWire_ReadBit();
    
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
        return (1);
			}
		}
    
		OneWire_WriteBit(curr & 0x80);
    
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
  return (0);  
}


// -------------------------------------------------------------

ErrorStatus OneWire_Search(OneWireBus_TypeDef* busDev) {

  if (OneWire_Reset()) return (ERROR);

  lastfork = 65;
  for (uint8_t i = 0; i < busDev->Count; i++) {
    if (OneWire_Enumerate(busDev->Devs[i].Addr)) {
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
  OneWire_WriteByte(ReadPowerSupply);
  
  return !OneWire_ReadBit();
}


/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
ErrorStatus OneWire_MatchROM(OneWireDevice_t* dev) {
  if (OneWire_Reset()) return (ERROR);
  
  OneWire_WriteByte(MatchROM);
  for (uint8_t i = 0; i < 8; i++) {
    OneWire_WriteByte(dev->Addr[i]);
  }

  return (SUCCESS);
}


