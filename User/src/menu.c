#include "stm8l15x.h"
#include "menu.h"
#include "disp.h"
#include "sysparams.h"
#include "lcd.h"
#include "utili.h"
#include "error.h"
#include "adf.h"

/*! \brief 
*      显示选择
* \param menu_id[IN]      - 菜单选择
*
* \note  
* 
*/
void menu_disp(u8 menu_id)
{
    switch(menu_id)
    {
    case MENU_MAIN:
        menu_main();
        break;
    case MENU_SET_TEMP:
        menu_set_temp();
        break;
    case MENU_VIEW:
        menu_view();
        break;
    case MENU_ID:
        menu_ID();
        break;
    default:
        break;
    }
//////    disp_update();
}


/*! \brief
*  设定温度功能
*/
void menu_set_temp(void)
{
    menu_common();
//    // 设定温度
//    disp_temp_set(g_sys_params.temp_set); 
    lcd_clr_region();
//    printf("g_sys_params.temp_set = %d\r\n", g_sys_params.temp_set);
    LCD_Temp_Set(g_sys_params.temp_set, T_SET, WRITE);
}

/*! \brief
*  一直检测显示的内容
*/
void menu_common(void)
{       
    LCD_MARK_WriteChar("W", WRITE);     // 信号塔
//    /* 阀门状态 */
//    if(g_run_params.valve_position == Valve_Open)      	//  阀开  
//    {
//        LCD_MARK_WriteChar("O", WRITE);   				//   O 代表 OPEN  ，MARK:阀开 ,写
//        LCD_MARK_WriteChar("C", CLEAR);
//    } else if (g_run_params.valve_position == Valve_Close)  //  阀关 
//    {
//        LCD_MARK_WriteChar("C", WRITE);    				//  C 代表 close  ，MARK:阀关 , 写
//        LCD_MARK_WriteChar("O", CLEAR);
//    } else
//    {
//        LCD_MARK_WriteChar("C", CLEAR);
//        LCD_MARK_WriteChar("O", CLEAR);
//    }

//    if((DevPara_DATA.ValvePosition == Valve_Middle)||(DevPara_DATA.ValvePosition == Valve_Execute))//正在开阀或关阀 
//    {
//            
//    }
    /* 欠费标志 */
    if(g_run_params.arrears_state == 1)         			//  欠费   
    {
         LCD_MARK_WriteChar("R", WRITE);         		//   R 代表 aRrearage ，MARK: 欠费 ,     写
    } 
    else
    {
         LCD_MARK_WriteChar("R", CLEAR);        		// R 代表 aRrearage ，MARK: 欠费 ， 清
    } 
    
    //printf("g_run_params.batt_lvl = %d\r\n" ,g_run_params.batt_lvl);
    /* 电池电量 */
    batter_value();
    
////////    /* 警告状态 */
////////    /*st占用两个字节16位，主要看前面的14位为多少：0—表示正常，1—表示异常   不够的话前面补零*/
////////    //printf("g_run_params.st = %04x\r\n" ,g_run_params.st);
////////    if (((g_run_params.st&0xFFFC)!=0) || ((g_run_params.st&0x0003)==0x0003) || ((g_run_params.st&0x0002)==0x0002))
////////    {
////////        disp_warning(DISPLAY_ON);
////////    }
////////    else
////////    {
////////        disp_warning(DISPLAY_OFF);
////////        
////////        // 阀门状态
////////        if ((g_run_params.st&0x03) == 0x00)//最后两位为00H时那么为开阀
////////        {
////////            disp_valve_sts(DISPLAY_ON);
////////            //disp_valve_sign(DISPLAY_OFF);
////////        }
////////        else if ((g_run_params.st&0x03) == 0x01)//最后两位为01H时那么为关阀
////////        {
////////            disp_valve_sts(DISPLAY_OFF);
////////            //disp_valve_sign(DISPLAY_OFF);
////////        }
////////    }
////////    
////////    /* 欠费 */
////////    //printf("g_sys_params.balance_alarm = %d\r\n" ,g_sys_params.balance_alarm);
////////    //printf("g_sys_params.balance = %ld\r\n" ,g_sys_params.balance);
////////    if (g_sys_params.balance < g_sys_params.balance_alarm && (g_sys_params.disp_ctl&DISP_CTL_OWE))
////////    {
////////        disp_owed(DISPLAY_ON);
////////    }
////////    else
////////    {
////////        disp_owed(DISPLAY_OFF);
////////    }
////////    disp_line(1);
    //disp_rf_state(1);
}

/*! \brief
*  主屏幕显示温度
*/
void menu_main(void)
{
    menu_common();
////////    disp_temp_room(g_run_params.temp);
    // 还差状态更新显示
    lcd_clr_region();
    lcd_temp_data_disp(g_run_params.temp);
  
}

/*! \brief
*  设置ID显示
*/
void menu_ID(void)
{
    menu_common();
    lcd_clr_region();
    disp_set_id();
}



/*! \brief
*  菜单查询
*/
void menu_view(void)
{
//////    struct tm t;
    
    menu_common();
    lcd_temp_data_disp(g_run_params.temp);
////    disp_temp_room(g_run_params.temp);
    lcd_clr_region();
    
    switch(g_run_params.view_idx)
    {   
    case MENU_VIEW_IDX_TEMP:    // 室内温度和时间日期
        //printf("0\r\n");
//        disp_temp_room(g_run_params.temp);
        lcd_temp_data_disp(g_run_params.temp);
        break;
    case MENU_VIEW_IDX_TEMP_SET:        // 设定温度
//        disp_temp_set(g_sys_params.temp_set);
        lcd_set_temp_disp();
        break;
//////    case MENU_VIEW_IDX_BALANCE: //剩余热量
//////        disp_surplus_heat(g_sys_params.balance, g_sys_params.balance_unit);
//////        //printf("1\r\n");
//////        break;
    case MENU_VIEW_IDX_HEATING_QUANTITY_TOTAL: //累计热量
        //printf("Heat Quantity Total:%lu\r\n", gp_sys_params->heating_quantity_total);
//        disp_heat_quantity(g_sys_params.heating_quantity, g_sys_params.heating_quantity_unit);
        lcd_heating_quantity_disp();
        //printf("2\r\n");
        break;      
    case MENU_VIEW_IDX_HEATING_TIME_TOTAL:  //用热时间
        //printf("Heat Time Total:%lu\r\n", gp_sys_params->heating_time_total);
////////        disp_heat_time(g_sys_params.heating_time);
        lcd_heating_time_disp();
        //printf("3\r\n");
        break;
    case MENU_VIEW_IDX_FAULT:          // 通讯
        //printf("g_run_params.st:%04x\r\n", g_run_params.st);
//////        if (((g_run_params.st&0xFFFC)!=0) || ((g_run_params.st&0x0003)==0x0003) || ((g_run_params.st&0x0002)==0x0002) || ((g_run_params.st&0x0004)==0x0004) || ((g_run_params.st&0x0008)==0x0008))
//////        {
//////            disp_fault(g_run_params.st);
//////        }
//////        else
//////        {
//////            disp_fault(0);
//////        }
      lcd_device_gn_disp();
        //printf("4\r\n");
        break;
//////    case MENU_VIEW_IDX_ROOM_ID:
//////        //        u8 room_name[6] = {'1','2','3','4','5','6'};//定义一个数组
//////        //        //把新的数组给系统参数，这也是数组赋值的一种方式
//////        //        memcpy(g_sys_params.room_name, room_name, sizeof(g_sys_params.room_name)/sizeof(g_sys_params.room_name[0]));
//////        disp_room_id((char*)g_sys_params.room_name, sizeof(g_sys_params.room_name)/sizeof(g_sys_params.room_name[0])); //(char*)为强制类型转换，g_sys_params.room_name本身就是地址
//////        //printf("strlen(id)=%d\r\n" ,strlen(id));
//////        //printf("5\r\n");
//////        break;  
//////    case MENU_VIEW_IDX_ID_LEFT:
//////        disp_id(g_sys_params.addr, 3);  //在主函数做了处理
//////        //printf("6\r\n");
//////        break;
//////    case MENU_VIEW_IDX_ID_RIGHT:
//////        disp_id(g_sys_params.addr+3, 2);//g_sys_params.addr+3是地址
//////        //printf("7\r\n");
//////        break;
//////    case MENU_VIEW_IDX_TIME:
//////        rtc_read(&t);
//////        g_run_params.date[0] = t.tm_year-100;
//////        g_run_params.date[1] = t.tm_mon+1;
//////        g_run_params.date[2] = t.tm_mday;
//////        g_run_params.date[3] = t.tm_hour;
//////        g_run_params.date[4] = t.tm_min;
//////#ifdef TASK_PRINTF
//////        printf("g_run_params.date[0]=%d\r\n" ,g_run_params.date[0]);
//////        printf("g_run_params.date[1]=%d\r\n" ,g_run_params.date[1]);
//////        printf("g_run_params.date[2]=%d\r\n" ,g_run_params.date[2]);
//////        printf("g_run_params.date[3]=%d\r\n" ,g_run_params.date[3]);
//////        printf("g_run_params.date[4]=%d\r\n" ,g_run_params.date[4]);
//////#endif
//////        disp_sys_time(g_run_params.date[3], g_run_params.date[4]);
//////        //disp_sys_time(1, 2);
//////        //disp_sys_time(t.tm_hour, t.tm_min);
//////        //printf("8\r\n");
//////        break;
    
    default:
        break;
    }
}

/*! \brief
*  电池电量的选择
*/
void batter_value(void)
{
//    if(g_run_params.batt_lvl >= 3000)
//    {
//        disp_batt(DISPLAY_ON, BATT_LVL_3);
//    }
//    else if(g_run_params.batt_lvl >= 2700 && g_run_params.batt_lvl < 3000)
//    {
//        disp_batt(DISPLAY_ON, BATT_LVL_2);
//    }
//    else if(g_run_params.batt_lvl >= 2400 && g_run_params.batt_lvl < 2700)
//    {
//        error_clr(FAULT_BATT);
//        disp_batt(DISPLAY_ON, BATT_LVL_1);
//    }
//    else if(g_run_params.batt_lvl < 2400)
//    {
//        error_set(FAULT_BATT);
//        disp_batt(DISPLAY_ON, BATT_LVL_0);
//    }
    
    if(g_run_params.batt_lvl >= 3000)
    {
      error_clr_u8(FAULT_LOW_BATT);
//        disp_batt(DISPLAY_ON, BATT_LVL_3);
        LCD_MARK_WriteChar("B", CLEAR); 
    }
    else if(g_run_params.batt_lvl >= 2700 && g_run_params.batt_lvl < 3000)
    {
      error_clr_u8(FAULT_LOW_BATT);
//        disp_batt(DISPLAY_ON, BATT_LVL_2);
        LCD_MARK_WriteChar("B", CLEAR); 
    }
    else if(g_run_params.batt_lvl >= 2500 && g_run_params.batt_lvl < 2700)
    {
////        error_clr(FAULT_BATT);
        error_clr_u8(FAULT_LOW_BATT);
//        disp_batt(DISPLAY_ON, BATT_LVL_1);
        LCD_MARK_WriteChar("B", CLEAR); 
    }
    else if(g_run_params.batt_lvl < 2500)
    {
////        error_set(FAULT_BATT);
        error_set_u8(FAULT_LOW_BATT);
//        disp_batt(DISPLAY_ON, BATT_LVL_0);
        LCD_MARK_WriteChar("B", WRITE); 
    }
}
