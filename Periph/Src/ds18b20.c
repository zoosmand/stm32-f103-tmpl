/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t spad[9];



// -------------------------------------------------------------  
void DS18B20_Command(uint8_t cmd) {
  OneWire_WriteByte(&cmd);
}


// -------------------------------------------------------------  
void DS18B20_Write(uint8_t num_bytes, uint8_t* data) {
  for (int i = 0; i < num_bytes; i++){
    OneWire_WriteByte(&data[i]);
  }
}


// -------------------------------------------------------------  
void DS18B20_Read(uint8_t num_bytes, uint8_t* data, uint8_t reverse) {
  uint8_t crc = 0;

  if (reverse){
    for (int i = (num_bytes - 1); i >= 0; i--){
      OneWire_ReadByte(&data[i]);
      OneWire_CRC8(&crc, data[i]);
    }
  } else {
    for (int i = 0; i < num_bytes; i++){
      OneWire_ReadByte(&data[i]);
      OneWire_CRC8(&crc, data[i]);
    }
  }
  
}


// -------------------------------------------------------------  
int8_t DS18B20_ReadScratchpad(uint8_t* scratchpad, uint8_t* address) {
  if (OneWire_Reset()) return 0;
  // if (!(FLAG_CHECK(&_OWREG_, _OLF_))) return (0);
  
  DS18B20_Command(MatchROM);
  DS18B20_Write(8, address);
  DS18B20_Command(ReadScratchpad);

  uint32_t __delay = sysCnt + 2;
  while (__delay >= sysCnt) {__asm volatile("nop");};
  
  DS18B20_Read(9, scratchpad, 0);
  
  return (1);
}


// -------------------------------------------------------------  
int8_t DS18B20_ConvertT(void) {

  if (OneWire_Reset()) return 0;
  
    DS18B20_Command(SkipROM);
    DS18B20_Command(ConvertT);
    // uint32_t __delay = sysCnt + 750;
    // while (__delay >= sysCnt) {__asm volatile("nop");};
    while(!OneWire_ReadBit()) {__asm volatile("nop");};

  OneWireDevice_t* oneWireDevices = Get_OwDevices();

  for (uint8_t i = 0; i < 2; i++) {

    
    DS18B20_ReadScratchpad(oneWireDevices[i].spad, oneWireDevices[i].addr);
    while(!OneWire_ReadBit());
  }
  uint32_t* t1 = (int32_t*)&oneWireDevices[0].spad;
  uint32_t* t2 = (int32_t*)&oneWireDevices[1].spad;
  printf("%d.%02d %d.%02d\n", 
    (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4),
    (int8_t)((*t2 & 0x0000fff0) >> 4), (uint8_t)(((*t2 & 0x0000000f) * 100) >> 4)
  );
  return (1);
}
