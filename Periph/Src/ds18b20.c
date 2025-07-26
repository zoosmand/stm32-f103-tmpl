/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



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
  SimpleDelay(2000);  
  DS18B20_Read(9, scratchpad, 0);
  
  return (1);
}


// -------------------------------------------------------------  
int8_t DS18B20_ConvertT(void) {

  uint8_t scratchpad[9];
  OW_Reset();
  if (!(FLAG_CHECK(&_OWREG_, _OLF_))) return (0);
  
  DS18B20_Command(SkipROM);
  DS18B20_Command(ConvertT);
  SimpleDelay(850000);

  DS18B20_Command(ReadScratchpad);
  SimpleDelay(4000);  
  DS18B20_Read(9, scratchpad, 0);

  printf("T:%s\n", scratchpad);
  
  return (1);
}

// void TempCollect(void) {
//   uint8_t scratchpad[8];
//   int32_t* tmp;
//   int32_t temper = 0;
  
//   if (DS18B20_ConvertT()) {
//     for (int i = 0; i < owDevices.size; i++) {
//       printf("address: %llx\n", *(uint64_t*)arrayElement(&owDevices, i));
//       if (DS18B20_ReadScratchpad(scratchpad, (uint8_t*)arrayElement(&owDevices, i))) {
//         tmp = (int32_t*)&scratchpad;
//         temper = (((*tmp & 0x0000fff0) >> 4) * 10000) + (((*tmp & 0x0000000f) * 10000) >> 4);
//         printf("T: %f\n", (temper * 0.0001));
//       } else {
//         printf("No data!\n");
//       }
//     }
//   }
// //   printf("------------------\n");
// }