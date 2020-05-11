#ifndef _TASK_POWER_H
#define _TASK_POWER_H

#include "sysparams.h"
  
#define TEMP_MEAS_R         5090 

typedef enum {
    E_TASK_ADC_IDLE = 0,    //空闲状态
    E_TASK_ADC_READY,       //准备状态
    E_TASK_ADC_COLLECTION_BATTERY,
    E_TASK_ADC_COLLECTION_TEMP,  //采集状态
    E_TASK_ADC_FINISH,      //完成状态
}TASK_ADC_STATE;

typedef struct _TASK_ADC_HANDLER{
    u8  sts;        //状态
    u32 adc_singleslow_timer;   //采集时间慢
    u32 adc_singlefast_timer;   //采集时间快
    u32 adc_keytrigger_timer;   //按键触发快速刷新的时间
        
}TASK_ADC_HANDLER;

/* adc进程 */
void task_adc_proc(void);
/* 电阻计算温度的值 */
u16 task_sample_result_temp(u16 adc_temp);
/* 温度采集 */
void temp_sample(void);
/* 电量采集 */
void batter_sample(void);
/* adc上电采集 */
void task_adc_init(void);
/* adc触发函数 */
void task_adc_triggered(void);
/* adc采样周期转换函数 */
void task_adc_schedule(void);
/* 按键adc采样周期清零函数 */
void task_adc_timereset(void);
/* 温度补偿函数 */
void Temperature_Compensation(void);

#endif