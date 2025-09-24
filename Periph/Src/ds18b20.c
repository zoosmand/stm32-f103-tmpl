/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// static uint8_t spad[9];



// // -------------------------------------------------------------  
// void DS18B20_Command(uint8_t cmd) {
//   OW_Write(&cmd);
// }


// // -------------------------------------------------------------  
// void DS18B20_Write(uint8_t num_bytes, uint8_t* data) {
//   for (int i = 0; i < num_bytes; i++){
//     OW_Write(&data[i]);
//   }
// }


// // -------------------------------------------------------------  
// void DS18B20_Read(uint8_t num_bytes, uint8_t* data, uint8_t reverse) {
//   uint8_t crc = 0;
//   // FLAG_CLR(&_OWREG_, _CRCERF_);

//   if (reverse){
//     for (int i = (num_bytes - 1); i >= 0; i--){
//       OW_Read(&data[i]);
//       OW_CRC8(&crc, data[i]);
//     }
//   } else {
//     for (int i = 0; i < num_bytes; i++){
//       OW_Read(&data[i]);
//       OW_CRC8(&crc, data[i]);
//     }
//   }
  
//   if (crc) FLAG_SET(&_OWREG_, _CRCERF_);
// }


// -------------------------------------------------------------  
int DS18B20_ReadScratchpad(uint8_t* addr, uint8_t* buf) {
  // OW_Reset();
  // if (!(FLAG_CHECK(&_OWREG_, _OLF_))) return (0);
  
  // DS18B20_Command(MatchROM);
  // DS18B20_Write(8, address);
  // DS18B20_Command(ReadScratchpad);

  
  // DS18B20_Read(9, scratchpad, 0);
  
  // return (1);
  if (OW_MatchROM(addr)) return 1;
  DS18B20_ConvertT(addr);
  OW_Write(ReadScratchpad);

  uint32_t __delay = sysCnt + 2;
  while (__delay >= sysCnt) {__asm volatile("nop");};
  
  uint8_t crc = 0;
  for (int8_t i = 0; i < 9; i++) {
    buf[i] = OW_Read();
    crc = OW_CRC8(crc, buf[i]);
  }
  if (crc) return 1;
  
  return 0;
}



uint8_t DS18B20_WriteScratchpad(uint8_t* addr, uint8_t* buf) {
  if (OW_MatchROM(addr)) return 1;
  OW_Write(WriteScratchpad);

  for (uint8_t i = 0; i < 3; i++) {
    OW_Write(buf[i]);
  }
  
  return 0;
}



// -------------------------------------------------------------  
int DS18B20_ConvertT(uint8_t* addr) {
  

  // OW_Reset();
  // if (!(FLAG_CHECK(&_OWREG_, _OLF_))) return (0);
  
  // // uint8_t* addr = Get_AddrBuf();
  // OW_Write(SkipROM);
  // OW_Write(ConvertT);

  // uint32_t __delay = sysCnt + 750;
  // while (__delay >= sysCnt) {
  //   __asm volatile("nop");
  // };
  
  // while(!OW_ReadBit());

  // ow_device_t* ow_devices = Get_OwDevices();

  // for (uint8_t i = 0; i < 2; i++) {
  //   DS18B20_ReadScratchpad(ow_devices[i].spad, ow_devices[i].addr);
  //   while(!OW_ReadBit());
  // }
  // uint32_t* t1 = (int32_t*)&ow_devices[0].spad;
  // uint32_t* t2 = (int32_t*)&ow_devices[1].spad;
  // printf("%d.%02d %d.%02d\n", 
  //   (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4),
  //   (int8_t)((*t2 & 0x0000fff0) >> 4), (uint8_t)(((*t2 & 0x0000000f) * 100) >> 4)
  // );
  // return (1);


  if (OW_MatchROM(addr)) return 1;
  uint8_t pps = OW_ReadPowerSupply(addr);

  if (OW_MatchROM(addr)) return 1;
  OW_Write(ConvertT);
  
  if (pps) {
    PIN_Low;
    uint32_t __delay = sysCnt + 750;
    while (__delay >= sysCnt) {__asm volatile("nop");};
    PIN_High;
  } else {
    /* TDOD implement timeout, handle independently */
    while(!OW_ReadBit()) {__asm volatile("nop");};
  }
  
  return 0;
}


int DS18B20_ShowTemperatureMeasurment() {

  ow_device_t* ow_devices = Get_OwDevices();

  for (uint8_t i = 0; i < 1; i++) {
    DS18B20_ReadScratchpad(ow_devices[i].addr, ow_devices[i].spad);
  }
  int32_t* t1 = (int32_t*)&ow_devices[0].spad;
  printf("%d.%02d\n", (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4));
  return 0;

}