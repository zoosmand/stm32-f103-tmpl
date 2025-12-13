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

#define BMX680_I2C_ADDR                 0x77
#define BME680_ID                       0x61

#define BMx680_RW_BIT                   0x80


/* BMx680 Registers */
#define BMx680_i2c_dev_id               0xd0
#define BMx680_i2c_reset                0xe0
#define BMx680_spi_dev_id               0x50
#define BMx680_spi_page                 0x73
#define BMx680_spi_page_pos             4
#define BMx680_spi_reset                0x60

#define BMx680_par_t1                   0xe9
#define BMx680_par_t2                   0x8a
#define BMx680_par_t3                   0x8c

#define BMx680_calib1                   0x8a // 21 bytes
#define BMx680_calib2                   0xe1 // 16 bytes

#define BMx680_config                   0x75
#define BMx680_config_spi_3w_en_pos     0
#define BMx680_config_iir_filter_pos    2
#define BMx680_config_iir_filter_0      (0b000 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_1      (0b001 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_3      (0b010 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_7      (0b011 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_15     (0b100 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_31     (0b101 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_63     (0b110 << BMx680_config_iir_filter_pos)
#define BMx680_config_iir_filter_127    (0b111 << BMx680_config_iir_filter_pos)

#define BMx680_ctrl_meas                0x74
#define BMx680_ctrl_meas_mode_pos       0
#define BMx680_ctrl_meas_sleep_mode     (0b00 << BMx680_ctrl_meas_mode_pos)
#define BMx680_ctrl_meas_forced_mode    (0b01 << BMx680_ctrl_meas_mode_pos)
#define BMx680_ctrl_meas_pres_ovrs_pos  2
#define BMx680_ctrl_meas_pres_ovrs_0    (0b000 << BMx680_ctrl_meas_pres_ovrs_pos)
#define BMx680_ctrl_meas_pres_ovrs_1    (0b001 << BMx680_ctrl_meas_pres_ovrs_pos)
#define BMx680_ctrl_meas_pres_ovrs_2    (0b010 << BMx680_ctrl_meas_pres_ovrs_pos)
#define BMx680_ctrl_meas_pres_ovrs_4    (0b011 << BMx680_ctrl_meas_pres_ovrs_pos)
#define BMx680_ctrl_meas_pres_ovrs_8    (0b100 << BMx680_ctrl_meas_pres_ovrs_pos)
#define BMx680_ctrl_meas_pres_ovrs_16   (0b101 << BMx680_ctrl_meas_pres_ovrs_pos)
#define BMx680_ctrl_meas_tmpr_ovrs_pos  5
#define BMx680_ctrl_meas_tmpr_ovrs_0    (0b000 << BMx680_ctrl_meas_tmpr_ovrs_pos)
#define BMx680_ctrl_meas_tmpr_ovrs_1    (0b001 << BMx680_ctrl_meas_tmpr_ovrs_pos)
#define BMx680_ctrl_meas_tmpr_ovrs_2    (0b010 << BMx680_ctrl_meas_tmpr_ovrs_pos)
#define BMx680_ctrl_meas_tmpr_ovrs_4    (0b011 << BMx680_ctrl_meas_tmpr_ovrs_pos)
#define BMx680_ctrl_meas_tmpr_ovrs_8    (0b100 << BMx680_ctrl_meas_tmpr_ovrs_pos)
#define BMx680_ctrl_meas_tmpr_ovrs_16   (0b101 << BMx680_ctrl_meas_tmpr_ovrs_pos)

#define BMx680_ctrl_hum                 0x72
#define BMx680_ctrl_meas_hum_ovrs_pos   0
#define BMx680_ctrl_spi_3w_int_en_pos   6
#define BMx680_ctrl_meas_hum_ovrs_0     (0b000 << BMx680_ctrl_meas_hum_ovrs_pos)
#define BMx680_ctrl_meas_hum_ovrs_1     (0b001 << BMx680_ctrl_meas_hum_ovrs_pos)
#define BMx680_ctrl_meas_hum_ovrs_2     (0b010 << BMx680_ctrl_meas_hum_ovrs_pos)
#define BMx680_ctrl_meas_hum_ovrs_4     (0b011 << BMx680_ctrl_meas_hum_ovrs_pos)
#define BMx680_ctrl_meas_hum_ovrs_8     (0b100 << BMx680_ctrl_meas_hum_ovrs_pos)
#define BMx680_ctrl_meas_hum_ovrs_16    (0b101 << BMx680_ctrl_meas_hum_ovrs_pos)

#define BMx680_ctrl_gas_1               0x71
#define BMx680_ctrl_gas_run_gas_pos     4
#define BMx680_ctrl_gas_nb_conv_pos     0 // [3:0]

#define BMx680_ctrl_gas_0               0x70
#define BMx680_ctrl_gas_heat_pff_pos    3

#define BMx680_gas_wait_x               0x64 // by 0x6d
#define BMx680_res_heat_x               0x5a // by 0x63
#define BMx680_ldac_heat_x              0x50 // by 0x59
#define BMx680_gas_r_msb                0x2a
#define BMx680_gas_r_lsb                0x2b // [7:6]
#define BMx680_gas_valid_r_pos          5
#define BMx680_heat_stab_r_pos          4
#define BMx680_gas_range_r_pos          0 // [3:0]

#define BMx680_hum_msb                  0x25
#define BMx680_hum_lsb                  0x26

#define BMx680_temp_msb                 0x22
#define BMx680_temp_lsb                 0x23
#define BMx680_temp_xlsb                0x24 // [7:4]
#define BMx680_temp_xlsb_pos            4

#define BMx680_pres_msb                 0x1f
#define BMx680_pres_lsb                 0x20
#define BMx680_pres_xlsb                0x21 // [7:4]
#define BMx680_pres_xlsb_pos            4

#define BMx680_eas_status_0             0x1d
#define BMx680_new_data_0_pos           7
#define BMx680_gas_measuring_pos        6
#define BMx680_measuring_pos            5
#define BMx680_gas_meas_index_pos       0 // [3:0]

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

