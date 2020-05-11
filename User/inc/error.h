#ifndef _ERROR_H
#define _ERROR_H

#include "stm8l15x.h"

typedef enum _ErrorSet{
    FAULT_VALVE				= 0x0003,  // 阀门故障
    FAULT_COMM				= 0x0004,  // 通讯失连
    FAULT_BATT			        = 0x0008,  // 电池电压过低
    FAULT_RF_OUTDOOR			= 0x0010,  // 室外机RF故障
    FAULT_RF_INDOOR		        = 0x0020,  // 室内机RF故障
    FAULT_TEMP_SENSOR_IN		= 0x0040,  // 入水温度传感器故障
    FAULT_TEMP_SENSOR_OUT		= 0x0080,  // 回水温度传感器故障
    FAULT_TEMP_SENSOR_INDOOR	        = 0x0100,  // 室内机温度传感器故障
    FAULT_BATT_SENDOR_INDOOR	        = 0x0200,  // 电池传感器故障
    //FAULT_VALVE_IS_ACTION               = 0x0400,  // 阀门动作中
    FAULT_RF                            = 0x80,  // 无线故障
    FAULT_LOW_BATT                      = 0x01,  // 电池电压过低
}ErrorSet;

void error_set(u16 code);

void error_clr(u16 code);

void error_set_u8(u8 code);
void error_clr_u8(u8 code);

#endif