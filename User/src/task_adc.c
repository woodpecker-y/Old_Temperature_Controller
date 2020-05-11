#include <stdio.h>
#include <math.h>
#include "adf.h"
#include "sysparams.h"
#include "board.h"
#include "task_adc.h"
#include "delay.h"
#include "disp.h"
#include "menu.h"
#include "task_disp.h"
#include "task_report.h"
#include "cj188.h"
#include "gpio.h"


static TASK_ADC_HANDLER s_task_adc_handler;
u32 adc_ext = 0;

/*! \brief
*  adc主进程
*/
void task_adc_proc(void)
{
    //    s_task_adc_handler.adc_timer++;
    switch(s_task_adc_handler.sts)
    {
    case E_TASK_ADC_IDLE:      // 空闲状态
        task_adc_schedule();
        //        if (s_task_adc_handler.adc_timer >= QUARTER_HOUR)
        //        {
        //            s_task_adc_handler.sts = E_TASK_ADC_READY;
        //            s_task_adc_handler.adc_timer = 0;  
        //        }
        //s_task_adc_handler.sts = E_TASK_ADC_READY;
        break;
    case E_TASK_ADC_READY:    //准备采集阶段
//        adc_init_vrefint1();
        adc_power_on1(ADC_BATT);
        adc_init1(ADC_BATT);
        s_task_adc_handler.sts = E_TASK_ADC_COLLECTION_BATTERY;
        g_run_params.batt_collect_flag = 1;
        break;  
    case E_TASK_ADC_COLLECTION_BATTERY:   //采集阶段
//        g_run_params.batt_lvl = adc_sample_vrefint1();
////        printf("batt_lvl = %d\r\n", g_run_params.batt_lvl);
//        g_run_params.batt_lvl = (int)(4997120 / g_run_params.batt_lvl);         // 4997120 = 1220*4096
////        printf("batt_lvl = %d\r\n", g_run_params.batt_lvl);
      
        adc_ext = adc_sample1(ADC_BATT);
        g_run_params.batt_lvl = (int)(3300 * adc_ext / 4096);
        s_task_adc_handler.sts = E_TASK_ADC_FINISH;
        g_run_params.batt_collect_flag = 0;
        break;
    case E_TASK_ADC_FINISH:     //完成阶段
//        adc_close_vrefint1();
        adc_close1(ADC_BATT);
        adc_power_off1(ADC_BATT);
        GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT );       // 必须加，不加功耗会高
        //GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow );
        s_task_adc_handler.sts = E_TASK_ADC_IDLE; 
        break;
    default:
        break;
    }
    
}         

/*! \brief
*  adc采集初始化，上电采集温度和电池电量     
*/
void task_adc_init(void)
{
    s_task_adc_handler.adc_singleslow_timer = HALF_HOUR-10;     // 过2s后再采集
}

/*! \brief
*  adc触发函数     
*/
void task_adc_triggered(void)
{
    s_task_adc_handler.sts = E_TASK_ADC_READY;  
}

/*! \brief
*  adc采样周期转换函数     
*/
void task_adc_schedule(void)
{
    //printf("g_run_params.adc_keytrigger =%d\r\n", g_run_params.adc_keytrigger);
    if(g_run_params.adc_keytrigger == 1)
    {
        s_task_adc_handler.adc_keytrigger_timer++;
        if(s_task_adc_handler.adc_keytrigger_timer <= TWO_MIN)
        {
            s_task_adc_handler.adc_singlefast_timer++;
            if(s_task_adc_handler.adc_singlefast_timer >= THREE_SECOND)
            {
                s_task_adc_handler.sts = E_TASK_ADC_READY;
                s_task_adc_handler.adc_singlefast_timer = 0;
            }
        }
        else
        {
            g_run_params.adc_keytrigger = 0;
            s_task_adc_handler.adc_keytrigger_timer = 0;
        }
    }
    else
    {
        s_task_adc_handler.adc_singleslow_timer++;
        if(s_task_adc_handler.adc_singleslow_timer >= HALF_HOUR)
        {
            s_task_adc_handler.sts = E_TASK_ADC_READY;
            s_task_adc_handler.adc_singleslow_timer = 0;
        }
    }
}

/*! \brief
*  按键adc采样时间清零函数     
*/
void task_adc_timereset(void)
{
    s_task_adc_handler.adc_singleslow_timer = 0;
    s_task_adc_handler.adc_keytrigger_timer = 0;
    s_task_adc_handler.adc_singlefast_timer = THREE_SECOND-1;
}

/*! \brief
*  温度补偿函数     
*/
void Temperature_Compensation(void)
{
    printf("g_run_params.temp3 = %d\r\n", g_run_params.temp);
//    if(g_run_params.temp <= 225)
//    {
//        g_run_params.temp = g_run_params.temp-13;    
//    }
//    else if(g_run_params.temp>225 && g_run_params.temp<=230)
//    {
//        g_run_params.temp = g_run_params.temp-10;
//    }
//    else if(g_run_params.temp>230 && g_run_params.temp<=235)
//    {
//        g_run_params.temp = g_run_params.temp-8;
//    }
//    else if(g_run_params.temp>235 && g_run_params.temp<=245)
//    {
//        g_run_params.temp = g_run_params.temp-5;   
//    }  
//    else if(g_run_params.temp > 245)
//    {
//        g_run_params.temp = g_run_params.temp-3;  
//    }
    
    
    if(g_run_params.temp <= 214)
    {
        g_run_params.temp = g_run_params.temp-23;    
    }
    else if(g_run_params.temp>214 && g_run_params.temp<=235)
    {
        g_run_params.temp = g_run_params.temp-20;
    }
    else if(g_run_params.temp>235 && g_run_params.temp<=245)
    {
        g_run_params.temp = g_run_params.temp-16;
    }
    else if(g_run_params.temp>245 && g_run_params.temp<=275)
    {
        g_run_params.temp = g_run_params.temp-12;
    }
    else if(g_run_params.temp>275 && g_run_params.temp<=300)
    {
        g_run_params.temp = g_run_params.temp-9;   
    }
    else if(g_run_params.temp > 300)
    {
        g_run_params.temp = g_run_params.temp-5;  
    }
    printf("g_run_params.temp4 = %d\r\n", g_run_params.temp);
}