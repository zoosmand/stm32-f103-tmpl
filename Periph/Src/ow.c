/* Includes ------------------------------------------------------------------*/
#include "ow.h"

/* Private includes ----------------------------------------------------------*/
//OneWireDevTypeDef OneWireSet[3];

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define OW_Low  PIN_H(OW_PORT, OW_PIN)
#define OW_High PIN_L(OW_PORT, OW_PIN)
#define OW_Level (PIN_LEVEL(OW_PORT, OW_PIN))

/* Global variables ----------------------------------------------------------*/
__attribute__((section(".cron"))) uint32_t _OWREG_ = 0;
static uint8_t lastfork;
// static uint8_t addr_buf[8];
static ow_device_t ow_devices[2];


/* Private variables ---------------------------------------------------------*/





// -------------------------------------------------------------  
void OW_Reset(void) {
  FLAG_CLR(&_OWREG_, _OLF_);

  OW_High;
  _delay_us(580);
  OW_Low;
  _delay_us(15);
  
  int i = 0;
  while (i++ < 240) {
    if (!OW_Level) {
      FLAG_SET(&_OWREG_, _OLF_);
      break;
    }
    _delay_us(1);
  }

  _delay_us(580 - i);
}


// -------------------------------------------------------------  
void OW_WriteBit(uint8_t bit) {
  OW_High;
  if (bit) {
    _delay_us(6);
    OW_Low;
    _delay_us(64);
  } else {
    _delay_us(60);
    OW_Low;
    _delay_us(10);
  }
}


// -------------------------------------------------------------  
void OW_Write(uint8_t* data) {
  uint8_t _byte_ = *data;
  for (int i = 0; i < 8; i++) {
    OW_WriteBit(_byte_ & 0x01);
    _byte_ >>= 1;
  }
}


// -------------------------------------------------------------  
uint8_t OW_ReadBit(void) {
  OW_High;
  _delay_us(6);
  OW_Low;
  _delay_us(9);
  uint8_t level = OW_Level;
  _delay_us(55);
  
  return level;
}


// -------------------------------------------------------------  
void OW_Read(uint8_t* data) {
  for (int i = 0; i < 8; i++) {
    *data >>= 1;
    *data |= (OW_ReadBit()) ? 0x80 : 0;
  }
}


// -------------------------------------------------------------  
void OW_CRC8(uint8_t* __crc, uint8_t __byte) {
  // 0x8c - it is a bit reverse of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
		*__crc = ((*__crc ^ (__byte >> i)) & 0x01) ? ((*__crc >> 1) ^ 0x8c) : (*__crc >> 1);
	}
}


// -------------------------------------------------------------  
int8_t OW_Error_Handler(void) {
  return (-1);
}










// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  







uint8_t OW_Enumerate(uint8_t* addr) {
	if (!lastfork) return (0);
  
	OW_Reset();
  // _delay_us(100000);  

  if (!FLAG_CHECK(&_OWREG_, _OLF_)) return (0);
  
  //addr += 7;
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
  uint8_t cmd = SearchROM;
	OW_Write(&cmd);

	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OW_ReadBit();
    bit1 = OW_ReadBit();

		if (!bit0) { // ���� ������������ � ������� ��� ����
			if (!bit1) { // �� ����� ������������� ��� 1 (�����)
				if (i < lastfork) { // ���� �� ����� �������� ������� ������������ ����,
					if (prev & 1) {
						curr |= 0x80; // �� �������� �������� ���� �� �������� �������
					} else {
						fork = i; // ���� ����, �� �������� ����������� �����
					}
				} else if (i == lastfork) {
            curr |= 0x80; // ���� �� ���� ����� � ������� ��� ��� ������ �������� � ����, ������� 1
					} else {
            fork = i; // ������ - ������� ���� � ���������� ����������� �����
          }
			} // � ��������� ������ ���, ������� ���� � ������
		} else {
			if (!bit1) { // ������������ �������
				curr |= 0x80;
			} else { // ��� �� ����� �� ������ - ��������� ��������
				return 0;
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
  return (1);  
}


void OW_Search(void) {
  lastfork = 65;
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t p = OW_Enumerate(ow_devices[i].addr);
    if (!p) break;
  }
  
}


ow_device_t* Get_OwDevices(void) {
  return ow_devices;
}



