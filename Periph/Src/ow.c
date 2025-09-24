/* Includes ------------------------------------------------------------------*/
#include "ow.h"

/* Private includes ----------------------------------------------------------*/
//OneWireDevTypeDef OneWireSet[3];

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// __attribute__((section(".cron"))) uint32_t _OWREG_ = 0;
static uint8_t lastfork;
// static uint8_t addr_buf[8];
static ow_device_t ow_devices[1];


/* Private function prototypes -----------------------------------------------*/

/**
 * @brief   Write a bit into OneWire bus.
 * @param   bit a bit to write
 * @retval  none
 */
static void OW_WriteBit(uint8_t);

/**
 * @brief   Enumerates OneWire device addresses. Realizes BTREE traversal method.
 * @param   addr OneWire device address
 * @retval  (int) status of operation
 */
static int OW_Enumerate(uint8_t* addr);




// -------------------------------------------------------------  
int OW_Reset(void) {
  // FLAG_CLR(&_OWREG_, _OLF_);

  PIN_High;
  _delay_us(580);
  PIN_Low;
  _delay_us(15);
  
  int i = 0;
  int k = 1;
  while (i++ < 240) {
    if (!PIN_Level) {
      // FLAG_SET(&_OWREG_, _OLF_);
      k = 0;
      break;
    }
    _delay_us(1);
  }

  _delay_us(580 - i);
  return k; // no error on the bus
}


// -------------------------------------------------------------  
static void OW_WriteBit(uint8_t bit) {
  PIN_High;
  if (bit) {
    _delay_us(6);
    PIN_Low;
    _delay_us(64);
  } else {
    _delay_us(60);
    PIN_Low;
    _delay_us(10);
  }
}


// -------------------------------------------------------------  
void OW_Write(uint8_t data) {
  uint8_t byte = data;
  for (int i = 0; i < 8; i++) {
    OW_WriteBit(byte & 0x01);
    byte >>= 1;
  }
}


// -------------------------------------------------------------  
uint8_t OW_ReadBit(void) {
  PIN_High;
  _delay_us(6);
  PIN_Low;
  _delay_us(9);
  uint8_t level = PIN_Level;
  _delay_us(55);
  
  return level;
}


// -------------------------------------------------------------  
uint8_t OW_Read(void) {
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++) {
    byte >>= 1;
    byte |= (OW_ReadBit()) ? 0x80 : 0;
  }
  return byte;
}


// -------------------------------------------------------------  
uint8_t OW_CRC8(uint8_t crc, uint8_t byte) {
  // 0x8c - it is a bit reverse of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
		crc = ((crc ^ (byte >> i)) & 0x01) ? ((crc >> 1) ^ 0x8c) : (crc >> 1);
	}
  return crc;
}


// -------------------------------------------------------------  
int OW_Error_Handler(void) {
  return (-1);
}










// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  






static int OW_Enumerate(uint8_t* addr) {
	if (!lastfork) return 1;
  
	if (OW_Reset()) return 1;
  
  //addr += 7;
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
	OW_Write(SearchROM);

	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OW_ReadBit();
    bit1 = OW_ReadBit();

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
				return 1;
			}
		}
      
		OW_WriteBit(curr & 0x80);
    
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
  return 0;  
}




uint8_t OW_ReadPowerSupply(uint8_t* addr) {
  OW_MatchROM(addr);
  OW_Write(ReadPowerSupply);
  
  return !OW_ReadBit();
}


int OW_MatchROM(uint8_t* addr) {
  if (OW_Reset()) return 1;
  
  OW_Write(MatchROM);
  for (uint8_t i = 0; i < 8; i++) {
    OW_Write(addr[i]);
  }

  return 0;
}


int OW_Search(void) {
  lastfork = 65;
  for (uint8_t i = 0; i < 1; i++) {
    uint8_t p = OW_Enumerate(ow_devices[i].addr);
    if (p) break;
  }
  return 0;
}


ow_device_t* Get_OwDevices(void) {
  return ow_devices;
}



