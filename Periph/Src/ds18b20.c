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
  OW_Write(&cmd);
}


// -------------------------------------------------------------  
void DS18B20_Write(uint8_t num_bytes, uint8_t* data) {
  for (int i = 0; i < num_bytes; i++){
    OW_Write(&data[i]);
  }
}


// -------------------------------------------------------------  
void DS18B20_Read(uint8_t num_bytes, uint8_t* data, uint8_t reverse) {
  uint8_t crc = 0;
  FLAG_CLR(&_OWREG_, _CRCERF_);

  if (reverse){
    for (int i = (num_bytes - 1); i >= 0; i--){
      OW_Read(&data[i]);
      OW_CRC8(&crc, data[i]);
    }
  } else {
    for (int i = 0; i < num_bytes; i++){
      OW_Read(&data[i]);
      OW_CRC8(&crc, data[i]);
    }
  }
  
  if (crc) FLAG_SET(&_OWREG_, _CRCERF_);
}


// -------------------------------------------------------------  
int8_t DS18B20_ReadScratchpad(uint8_t* scratchpad, uint8_t* address) {
  OW_Reset();
  if (!(FLAG_CHECK(&_OWREG_, _OLF_))) return (0);
  
  DS18B20_Command(MatchROM);
  DS18B20_Write(8, address);
  DS18B20_Command(ReadScratchpad);
  _delay_us(2000);  
  DS18B20_Read(9, scratchpad, 0);
  
  return (1);
}


// -------------------------------------------------------------  
int8_t DS18B20_ConvertT(void) {

  OW_Reset();
  if (!(FLAG_CHECK(&_OWREG_, _OLF_))) return (0);
  
  // uint8_t* addr = Get_AddrBuf();
    DS18B20_Command(SkipROM);
    DS18B20_Command(ConvertT);
    _delay_us(750000);
    while(!OW_ReadBit());

  ow_device_t* ow_devices = Get_OwDevices();

  for (uint8_t i = 0; i < 2; i++) {

    
    DS18B20_ReadScratchpad(ow_devices[i].spad, ow_devices[i].addr);
    while(!OW_ReadBit());
  }
  uint32_t* t1 = (int32_t*)&ow_devices[0].spad;
  uint32_t* t2 = (int32_t*)&ow_devices[1].spad;
  printf("%d.%02d %d.%02d\n", 
    (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4),
    (int8_t)((*t2 & 0x0000fff0) >> 4), (uint8_t)(((*t2 & 0x0000000f) * 100) >> 4)
  );
  return (1);
}
