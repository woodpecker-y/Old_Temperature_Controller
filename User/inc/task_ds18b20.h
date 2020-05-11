#ifndef _TASK_DS18B20_H
#define _TASK_DS18B20_H

#include "sysparams.h"

typedef enum {
    E_TASK_18B20_IDLE = 0,    //空闲状态
    E_TASK_18B20_READY,       //准备状态
    E_TASK_18B20_COLLECTION_TEMP,  //采集状态
    E_TASK_18B20_FINISH,      //完成状态
}TASK_18B20_STATE;

typedef struct _TASK_18B20_HANDLER{
    u8  sts;        //状态
    u32 date_timer;   //时间刷新时间
    u32 ds18b20_singleslow_timer;   //采集时间慢
    u32 ds18b20_singlefast_timer;   //采集时间快
    u32 ds18b20_keytrigger_timer;   //按键触发快速刷新的时间
}TASK_18B20_HANDLER;

/* ds18b20进程 */
void task_18b20_proc(void);
/* ds18b20触发函数 */
void task_18b20_triggered(void);
/* ds18b20采样周期转换函数 */    
void task_18b20_schedule(void);
/* 按键ds18b20采样时间清零函数 */  
void task_18b20_timereset(void);
/* ds18b20采集初始化，上电采集温度 */
void task_18b20_init(void);


#endif