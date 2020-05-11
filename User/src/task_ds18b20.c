#include <stdio.h>
#include "task_ds18b20.h"
#include "adf.h"
#include "sysparams.h"
#include "menu.h"
#include "task_disp.h"

#define TEMP_COLLECT_TIME 10    // 温度采集时间为10分钟
#define TEMP_AVE_NUM 6    // 计算平均温度采集的次数

static TASK_18B20_HANDLER s_task_18b20_handler;
static s16 temp_history_data[6] = {0};
static u8 temp_offset = 0;
static u8 temp_num_flag = 0;
static u8 i = 0;
static u16 temp_total = 0;
/*! \brief
*  ds18b20进程
*/
void task_18b20_proc(void)
{
    //    s_task_18b20_handler.adc_timer++;
    switch(s_task_18b20_handler.sts)
    {
    case E_TASK_18B20_IDLE:      // 空闲状态
        task_18b20_schedule();
        break;
    case E_TASK_18B20_READY:    //准备采集阶段
//      printf("E_TASK_18B20_READY\r\n");
          DS18B20_Start(); 
//        adc_init(ADC_BATT);   //adc电源初始化
//        adc_init(ADC_TEMP);   //adc温度初始化
        s_task_18b20_handler.sts = E_TASK_18B20_COLLECTION_TEMP;
        break;     
    case E_TASK_18B20_COLLECTION_TEMP:   //采集阶段
//      printf("E_TASK_18B20_COLLECTION_TEMP\r\n");
//        temp_sample();
        g_run_params.temp = DS18B20_ReadTemp();	//娓╁害璇诲彇鍛戒护
        if (g_run_params.temp > 5000)
        {
          s_task_18b20_handler.date_timer = UNIT_MIN - 1;
          s_task_18b20_handler.ds18b20_singleslow_timer = TEMP_COLLECT_TIME - 1;
        }
//        g_run_params.temp = DS18B20_ReadTemp();	//娓╁害璇诲彇鍛戒护
//        printf("temp = %d\r\n", g_run_params.temp);
        temp_history_data[temp_offset] = g_run_params.temp;
        temp_offset++;
        if (temp_offset >= TEMP_AVE_NUM)
        {
          temp_num_flag = 1;
          temp_offset = 0;
        }
        if (temp_num_flag == 1)         // 如果采集的温度次数不超过6次，用当前的温度当做平均温度，如果超过6次，那么就计算平均值
        {
          temp_total = 0;       // 总数清零
          for (i = 0; i < TEMP_AVE_NUM; i++)
          {
            temp_total += temp_history_data[i];
          }
          g_run_params.ave_temp_100 = (temp_total / TEMP_AVE_NUM);
        }
        else
        {
          g_run_params.ave_temp_100 = g_run_params.temp;
        }
          
//////        printf("g_run_params.temp = %d\r\n", g_run_params.temp);        // 暂时注释
        s_task_18b20_handler.sts = E_TASK_18B20_FINISH;
        break;
    case E_TASK_18B20_FINISH:     //完成阶段
//        printf("E_TASK_18B20_FINISH\r\n");
//        LCD_Temp_Display(g_run_params.temp, INPUT, WRITE);
        task_lcd_triggered(MENU_MAIN);//为了让屏幕实时刷新，多次触发LCD刷新的函数
//        adc_close(ADC_BATT);
//        adc_close(ADC_TEMP);
        ds18b20_close();
        s_task_18b20_handler.sts = E_TASK_18B20_IDLE; 
        break;
    default:
        break;
    }
    
}        

/*! \brief
*  ds18b20触发函数     
*/
void task_18b20_triggered(void)
{
    s_task_18b20_handler.sts = E_TASK_18B20_READY;  
}

/*! \brief
*  ds18b20采样周期转换函数     
*/
void task_18b20_schedule(void)
{
//    printf("s_task_18b20_handler.ds18b20_singleslow_timer = %u\r\n", s_task_18b20_handler.ds18b20_singleslow_timer);
    if(g_run_params.ds18b20_keytrigger == 1)
    {
        s_task_18b20_handler.ds18b20_keytrigger_timer++;
        if(s_task_18b20_handler.ds18b20_keytrigger_timer <= TWO_MIN)
        {
            s_task_18b20_handler.ds18b20_singlefast_timer++;
            if(s_task_18b20_handler.ds18b20_singlefast_timer >= THREE_SECOND)
            {
                s_task_18b20_handler.sts = E_TASK_18B20_READY;
                s_task_18b20_handler.ds18b20_singlefast_timer = 0;
            }
        }
        else
        {
            g_run_params.ds18b20_keytrigger = 0;
            s_task_18b20_handler.ds18b20_keytrigger_timer = 0;
        }
    }
    else
    {
        s_task_18b20_handler.date_timer++;   
        if(s_task_18b20_handler.date_timer >= UNIT_MIN) // 一分钟刷新下时间
        {
            s_task_18b20_handler.sts = E_TASK_18B20_FINISH;     // 只刷新屏幕
            s_task_18b20_handler.date_timer = 0;
            
            s_task_18b20_handler.ds18b20_singleslow_timer++;
            if(s_task_18b20_handler.ds18b20_singleslow_timer >= TEMP_COLLECT_TIME)
            {
                s_task_18b20_handler.sts = E_TASK_18B20_READY;
                s_task_18b20_handler.ds18b20_singleslow_timer = 0;
            }
        }
    }
}

/*! \brief
*  按键ds18b20采样时间清零函数     
*/
void task_18b20_timereset(void)
{
    s_task_18b20_handler.date_timer = 0;
    s_task_18b20_handler.ds18b20_singleslow_timer = 0;
    s_task_18b20_handler.ds18b20_keytrigger_timer = 0;
    s_task_18b20_handler.ds18b20_singlefast_timer = THREE_SECOND-1;
}

/*! \brief
*  ds18b20采集初始化，上电采集温度 
*/
void task_18b20_init(void)
{
    s_task_18b20_handler.date_timer = UNIT_MIN - 1;
    s_task_18b20_handler.ds18b20_singleslow_timer = TEMP_COLLECT_TIME - 1;
}