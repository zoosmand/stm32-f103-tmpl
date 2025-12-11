/**
  ******************************************************************************
  * @file           : bmx680.h
  * @brief          : This file contains header definitions BOSCH BMx680 
  *                   temperature, humidity, and atmospheric pressue sensor.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Askug Ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#ifndef __BMX680_H
#define __BMX680_H

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif


#ifndef BMX680_MODEL
#define BMX680_MODEL 680
#endif /* #ifndef BMX680_MODEL */


/* Private defines -----------------------------------------------------------*/

#define BMX680_I2C_ADDR             0x77
#define BME680_ID                   0x61

#define BMx680_RW_BIT               0x80


/* BMx680 Registers */
#define BMx680_i2c_dev_id           0xd0
#define BMx680_i2c_reset            0xe0
#define BMx680_spi_dev_id           0x50
#define BMx680_spi_page             0x73
#define BMx680_spi_reset            0x60

#define BMx680_par_t1               0xe9
#define BMx680_par_t2               0x8a
#define BMx680_par_t3               0x8c

#define BMx680_calib1               0x8a // 21
#define BMx680_calib2               0xe1 // 16

#define BMx680_ctrl_meas                0x74
#define BMx680_ctrl_meas_sleep_mode     (0b00 << 0)
#define BMx680_ctrl_meas_forced_mode    (0b01 << 0)
#define BMx680_ctrl_meas_tmpr_overs_0   (0b000 << 5)
#define BMx680_ctrl_meas_tmpr_overs_1   (0b001 << 5)
#define BMx680_ctrl_meas_tmpr_overs_2   (0b010 << 5)
#define BMx680_ctrl_meas_tmpr_overs_4   (0b011 << 5)
#define BMx680_ctrl_meas_tmpr_overs_8   (0b100 << 5)
#define BMx680_ctrl_meas_tmpr_overs_16  (0b101 << 5)
#define BMx680_ctrl_meas_pres_overs_0   (0b000 << 2)
#define BMx680_ctrl_meas_pres_overs_1   (0b001 << 2)
#define BMx680_ctrl_meas_pres_overs_2   (0b010 << 2)
#define BMx680_ctrl_meas_pres_overs_4   (0b011 << 2)
#define BMx680_ctrl_meas_pres_overs_8   (0b100 << 2)
#define BMx680_ctrl_meas_pres_overs_16  (0b101 << 2)

#define BMx680_ctrl_hym                 0x72
#define BMx680_ctrl_meas_hum_overs_0   (0b000 << 0)
#define BMx680_ctrl_meas_hum_overs_1   (0b001 << 0)
#define BMx680_ctrl_meas_hum_overs_2   (0b010 << 0)
#define BMx680_ctrl_meas_hum_overs_4   (0b011 << 0)
#define BMx680_ctrl_meas_hum_overs_8   (0b100 << 0)
#define BMx680_ctrl_meas_hum_overs_16  (0b101 << 0)

#define BMx680_iir_filter           0x75
#define BMx680_iir_filter_0         (0b000 << 2)
#define BMx680_iir_filter_1         (0b001 << 2)
#define BMx680_iir_filter_3         (0b010 << 2)
#define BMx680_iir_filter_7         (0b011 << 2)
#define BMx680_iir_filter_15        (0b100 << 2)
#define BMx680_iir_filter_31        (0b101 << 2)
#define BMx680_iir_filter_63        (0b110 << 2)
#define BMx680_iir_filter_127       (0b111 << 2)

#define BMx680_ctrl_gas_0           0x71
#define BMx680_ctrl_gas_1           0x70
#define BMx680_gas_wait_x           0x64 // by 0x6d
#define BMx680_res_heat_x           0x5a // by 0x63
#define BMx680_ldac_heat_x          0x50 // by 0x59


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief   Initializes the given Bosch BMx680 device.
 * @param   dev: pointer to the Bosch BMx680 device struct
 * @retval  status of operation
 */
ErrorStatus BMx680_Init(BMxX80_TypeDef*);

/**
 * @brief   Runs the measurment procedures on the given Bosch BMx680 device.
 * @param   dev: pointer to the Bosch BMx680 device struct
 * @retval  status of operation
 */
ErrorStatus BMx680_Measurement(BMxX80_TypeDef*);


#ifdef __cplusplus
}
#endif
#endif /* __BMX680_H */

