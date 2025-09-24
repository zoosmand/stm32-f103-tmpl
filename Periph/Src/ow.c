/* Includes ------------------------------------------------------------------*/
#include "ow.h"

/* Private includes ----------------------------------------------------------*/
//OneWireDevTypeDef OneWireSet[3];

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
static uint8_t lastfork;
static OneWireDevice_t oneWireDevices[2];


/* Private variables ---------------------------------------------------------*/





// -------------------------------------------------------------  
int OneWire_Reset(void) {

  OneWire_High;
  _delay_us(580);
  OneWire_Low;
  _delay_us(15);
  
  int i = 0;
  int status = 1;
  while (i++ < 240) {
    if (!OneWire_Level) {
      status = 0;
      break;
    }
    _delay_us(1);
  }

  _delay_us(580 - i);
  return status;
}


// -------------------------------------------------------------  
void OneWire_WriteBit(uint8_t bit) {
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
void OneWire_WriteByte(uint8_t* data) {
  uint8_t _byte_ = *data;
  for (int i = 0; i < 8; i++) {
    OneWire_WriteBit(_byte_ & 0x01);
    _byte_ >>= 1;
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
void OneWire_CRC8(uint8_t* __crc, uint8_t __byte) {
  // 0x8c - it is a bit reverse of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
		*__crc = ((*__crc ^ (__byte >> i)) & 0x01) ? ((*__crc >> 1) ^ 0x8c) : (*__crc >> 1);
	}
}


// -------------------------------------------------------------  
int8_t OneWire_ErrorHandler(void) {
  return (-1);
}










// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  







uint8_t OneWire_Enumerate(uint8_t* addr) {
	if (!lastfork) return (0);
  
	if (OneWire_Reset()) return 0;

  //addr += 7;
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
  uint8_t cmd = SearchROM;
	OneWire_WriteByte(&cmd);

	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OneWire_ReadBit();
    bit1 = OneWire_ReadBit();

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
  return (1);  
}


void OneWire_Search(void) {
  lastfork = 65;
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t p = OneWire_Enumerate(oneWireDevices[i].addr);
    if (!p) break;
  }
  
}


OneWireDevice_t* Get_OwDevices(void) {
  return oneWireDevices;
}



