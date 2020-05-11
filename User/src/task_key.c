#include "adf.h"
#include "sysparams.h"
#include "board.h"
#include "task_key.h"
#include "key.h"
#include "menu.h"
#include "task_disp.h"
#include "task_report.h"
#include "cj188.h"
#include "task_adc.h"
#include "delay.h"
#include "disp.h"


//u16 t = 250;
//u8 p = 1;

/* key status */
KeyHandler s_key_handler; //定义了一个结构体变量

/*! \brief
*  按键进程
*/
void task_key_proc(void)
{ 
    
    //printf("g_sys_params.temp_set2222222=%d\r\n",g_sys_params.temp_set);
    switch(s_key_handler.sts)
    {
    case KEY_STS_IDLE:
        break;
    case KEY_STS_PUSH:
        // 判断是否为合法按键
        if (TRUE == key_check_valid(s_key_handler.key_val))
        {
            g_run_params.key_long = 1;  //有按键按下此标志为1，按键释放此标志为0
            s_key_handler.sts = KEY_STS_VALID;
            g_run_params.ds18b20_keytrigger = 0;//把adc采集由慢变快的标志清零，这样只要有按键按下，他都要6s之后刷新屏幕
            g_run_params.adc_keytrigger = 0;
            task_adc_timereset();    
        }
        else
        {
            s_key_handler.sts = KEY_STS_IDLE;
        }
        break;
    case KEY_STS_VALID:
        // 检测按键释放
        delay_ms(10);
        if (TRUE == key_release(s_key_handler.key_val))
        {
            s_key_handler.timer_long = 0;
            g_run_params.key_long = 0;
            s_key_handler.sts = KEY_STS_DEAL;  
        }
        
        if(g_run_params.key_long == 1)  // 如果按键按下（长按也能进入下一个状态）
        {
            if (FALSE == key_release(s_key_handler.key_val))    //按键没有释放
            {
                s_key_handler.timer_long++;
                if(s_key_handler.timer_long >= TWO_SECOND)      // 长按2s才能切换下一个屏幕
                {
                    s_key_handler.timer_long = 0;
                    s_key_handler.sts = KEY_STS_DEAL;
                }
            }
        }
        break;
    case KEY_STS_DEAL:
        if(g_run_params.set_id == 1)    //当为设置ID时，当有按键按下第一次时，返回主界面温度界面
        {
            g_run_params.set_id = 0;
            //g_run_params.ds18b20_keytrigger = 1;
            s_key_handler.timer = 1;
            s_key_handler.sts = KEY_STC_FINISH;
            break;
        }
        g_run_params.set_id = 0;    //当不是设置温度模式下时也要把标志清零，和正常一样能返回到主界面
        //printf("s_key_handler.key_val:%d\r\n",s_key_handler.key_val);
        //printf("key_code:%d\r\n",key_code);
        s_key_handler.timer = THREE_SECOND;
        /* '1'为加; '2'为菜单; '3'为减; 'a'为加和菜单同时按下 */
        switch(s_key_handler.key_val)
        {
        case '1':
            key_action_add();
            break;
        case '2':
            key_action_view();
            break;
        case '3':
            key_action_sub();
            break;
        case 'b':
            key_action_setid();
            break;
        default:
            break;
        }
        //        if(s_key_handler.key_val == 49)
        //        { 
        //            key_action_view();
        //        }
        //        
        //        if(s_key_handler.key_val == 50)
        //        {
        //            key_action_sub();   
        //        }
        //        
        //        if(s_key_handler.key_val == 51)
        //        {
        //            key_action_add();
        //        }
        
        
        task_lcd_triggered(g_run_params.menu_id);       // 触发液晶屏显示，debug
        
        // 以下有没有都可以，功能就是长按连环触发
        if(g_run_params.key_long == 1)  //当按键还是没有释放时，按键处理完再次回到按键释放进程进行检测，直到按键释放清除标志
        {
            s_key_handler.sts = KEY_STS_VALID;
            break;
        }

        eeprom_write_sys(14, &g_sys_params.temp_set, 2);        //只写了设定温度，别的没写
        s_key_handler.sts = KEY_STC_FINISH;
        break;
    case KEY_STC_FINISH:
        //printf("s_key_handler.timer = %lu\r\n", s_key_handler.timer);
        if(g_run_params.set_id != 1)    // 在设定id的情况下不用6s回到主界面
        {
            s_key_handler.timer--;
        }
        if(s_key_handler.timer <= 0)//为了让没有按键后过几秒自动切换成主屏幕并刷新室温，而不是一直等待adc采集的时间到了才刷新成主屏幕
        {
            //printf("11111111111111\r\n");
            g_run_params.view_idx = MENU_VIEW_IDX_TEMP_SET-1;
            g_run_params.ds18b20_keytrigger = 1;
            g_run_params.adc_keytrigger = 1;
            task_report_triggered(CTRL_CODE_REPORT);//没有按键按下后6S触发上报，adc采集
            
            //task_adc_triggered();
            
            
            //            g_run_params.menu_id = MENU_MAIN;
            //            task_lcd_triggered(g_run_params.menu_id);
            
            //            printf("write[1]\r\n");
            //            eeprom_init();
            //            eeprom_write(24, (u8*)&(g_sys_params.temp_set), sizeof(g_sys_params.temp_set));//只写了设定温度，别的没写
            //            //printf("write sucess\r\n");
            //            eeprom_close();
            //            printf("write sucess\r\n");
            
            
//            if(g_run_params.key_long == 0)
//            {
                s_key_handler.sts = KEY_STS_IDLE;
//            }
//            else
//            {
//                s_key_handler.sts = KEY_STS_VALID;
//            }
        }
        
        break;
    default:
        break;  
    }
}

/*! \brief
*  按键触发函数
*/
void task_key_triggered(u8 key_code)
{
    s_key_handler.key_val = key_code;
    s_key_handler.sts = KEY_STS_PUSH;
#if TASK_PRINTF
    printf("key_code:%c\r\n", key_code);
#endif
}

/*! \brief
*  向上键动作
*/
void key_action_add(void)
{
    //printf("g_sys_params.temp_set1111111=%d\r\n",g_sys_params.temp_set);
    //eeprom_read(0, (u8*)&g_sys_params, sizeof(g_sys_params));
    //printf("g_sys_params.temp_set=%d\r\n",g_sys_params.temp_set);
    if(g_sys_params.temp_set < (g_sys_params.temp_room_uplimits * 100))
    {
        g_sys_params.temp_set = g_sys_params.temp_set+50;    
    }
    else
    {
        g_sys_params.temp_set = (g_sys_params.temp_room_uplimits * 100);
    }
    g_run_params.temp_return = g_sys_params.temp_set;
    //    eeprom_init();
    //    eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
    //    //printf("write sucess\r\n");
    //    eeprom_close();
    g_run_params.menu_id = MENU_SET_TEMP;
    g_sys_params.sys_set_temp_flg = 0x00;
    eeprom_write_sys(13, &g_sys_params.sys_set_temp_flg, 1);
}

/*! \brief
*  向下键动作
*/
void key_action_sub(void)
{
    //eeprom_read(0, (u8*)&g_sys_params, sizeof(g_sys_params));
    
    if(g_sys_params.temp_set > (g_sys_params.temp_room_lowlimits*100))
    {
        g_sys_params.temp_set = g_sys_params.temp_set-50;
    }
    else
    {
        g_sys_params.temp_set = (g_sys_params.temp_room_lowlimits*100);
    }
    g_run_params.temp_return = g_sys_params.temp_set;
    //    eeprom_init();
    //    eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
    //    //printf("write sucess\r\n");
    //    eeprom_close();
    
    g_run_params.menu_id = MENU_SET_TEMP; 
    g_sys_params.sys_set_temp_flg = 0x00;
    eeprom_write_sys(13, &g_sys_params.sys_set_temp_flg, 1);
}

/*! \brief
*  查询建动作
*/
void key_action_view(void)
{
//    printf("g_run_params.view_idx = %d\r\n" ,g_run_params.view_idx);
    g_run_params.view_idx ++;
//    printf("g_run_params.view_idx = %d\r\n" ,g_run_params.view_idx);
    if(g_run_params.view_idx >= 5)
    {
        g_run_params.view_idx = MENU_VIEW_IDX_TEMP_SET;//循环到设定温度
    }
    
    /* 下面的代码除非有显示控制位，否则不会用到 */
    //printf("g_sys_params.disp_ctl = %x\r\n" ,g_sys_params.disp_ctl);
    /* 如果现在是剩余热量，但是不让显示，跳过进入累计热量 控制码的8位，哪个为1显示哪项，全是FF都显示，00都不显示*/
    if (g_run_params.view_idx==MENU_VIEW_IDX_TEMP_SET && !(g_sys_params.disp_ctl&DISP_CTL_BALANCE))
    {
        g_run_params.view_idx = MENU_VIEW_IDX_HEATING_QUANTITY_TOTAL;
    } 
    
    /* 如果现在是累计热量，但是不让显示，跳过进入用热时间 */
    if (g_run_params.view_idx==MENU_VIEW_IDX_HEATING_QUANTITY_TOTAL && !(g_sys_params.disp_ctl&DISP_CTL_HEAT_QUANTITY_TOTAL))
    {
        g_run_params.view_idx = MENU_VIEW_IDX_HEATING_TIME_TOTAL;
    }
    /* 如果现在是用热时间，但是不让显示，跳过进入故障 */
    if (g_run_params.view_idx==MENU_VIEW_IDX_HEATING_TIME_TOTAL && !(g_sys_params.disp_ctl&DISP_CTL_HEAT_TIME_TOTAL))
    {
        g_run_params.view_idx = MENU_VIEW_IDX_FAULT;
    }
    /* 如果现在是故障，但是不让显示，跳过进入房间号 */
    if (g_run_params.view_idx==MENU_VIEW_IDX_FAULT && !(g_sys_params.disp_ctl&DISP_CTL_FAULT))
    {
        g_run_params.view_idx = MENU_VIEW_IDX_ID_LEFT;
    }
//    /* 如果现在是房间号，但是不让显示，跳过进入编号左 */
//    if (g_run_params.view_idx==MENU_VIEW_IDX_ROOM_ID && !(g_sys_params.disp_ctl&DISP_CTL_ROOM_NAME))
//    {
//        g_run_params.view_idx = MENU_VIEW_IDX_ID_LEFT;
//    }
    
    
    g_run_params.menu_id  = MENU_VIEW;
    
}

void key_action_setid(void)
{
    int rv;
    if (g_sys_params.factory_flag == 0)
    {
        rf_power_on();
        rv = rf_init(RF_FRE_420_5M, RF_DATA_RATE_1200, TXPOW_20dBm, DOWNLOAD_ID_CHANNEL);
        if(rv == 0)
        {
            g_run_params.menu_id  = MENU_ID;
            //printf("set id\r\n");
            g_run_params.set_id = 1;
            rf_rcv_init();
        }
        
    } 
}