#include "stm8l15x.h"
#include "menu.h"
#include "disp.h"
#include "sysparams.h"
#include "lcd.h"
#include "utili.h"
#include "error.h"
#include "adf.h"

/*! \brief 
*      ��ʾѡ��
* \param menu_id[IN]      - �˵�ѡ��
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
*  �趨�¶ȹ���
*/
void menu_set_temp(void)
{
    menu_common();
//    // �趨�¶�
//    disp_temp_set(g_sys_params.temp_set); 
    lcd_clr_region();
//    printf("g_sys_params.temp_set = %d\r\n", g_sys_params.temp_set);
    LCD_Temp_Set(g_sys_params.temp_set, T_SET, WRITE);
}

/*! \brief
*  һֱ�����ʾ������
*/
void menu_common(void)
{       
    LCD_MARK_WriteChar("W", WRITE);     // �ź���
//    /* ����״̬ */
//    if(g_run_params.valve_position == Valve_Open)      	//  ����  
//    {
//        LCD_MARK_WriteChar("O", WRITE);   				//   O ���� OPEN  ��MARK:���� ,д
//        LCD_MARK_WriteChar("C", CLEAR);
//    } else if (g_run_params.valve_position == Valve_Close)  //  ���� 
//    {
//        LCD_MARK_WriteChar("C", WRITE);    				//  C ���� close  ��MARK:���� , д
//        LCD_MARK_WriteChar("O", CLEAR);
//    } else
//    {
//        LCD_MARK_WriteChar("C", CLEAR);
//        LCD_MARK_WriteChar("O", CLEAR);
//    }

//    if((DevPara_DATA.ValvePosition == Valve_Middle)||(DevPara_DATA.ValvePosition == Valve_Execute))//���ڿ�����ط� 
//    {
//            
//    }
    /* Ƿ�ѱ�־ */
    if(g_run_params.arrears_state == 1)         			//  Ƿ��   
    {
         LCD_MARK_WriteChar("R", WRITE);         		//   R ���� aRrearage ��MARK: Ƿ�� ,     д
    } 
    else
    {
         LCD_MARK_WriteChar("R", CLEAR);        		// R ���� aRrearage ��MARK: Ƿ�� �� ��
    } 
    
    //printf("g_run_params.batt_lvl = %d\r\n" ,g_run_params.batt_lvl);
    /* ��ص��� */
    batter_value();
    
////////    /* ����״̬ */
////////    /*stռ�������ֽ�16λ����Ҫ��ǰ���14λΪ���٣�0����ʾ������1����ʾ�쳣   �����Ļ�ǰ�油��*/
////////    //printf("g_run_params.st = %04x\r\n" ,g_run_params.st);
////////    if (((g_run_params.st&0xFFFC)!=0) || ((g_run_params.st&0x0003)==0x0003) || ((g_run_params.st&0x0002)==0x0002))
////////    {
////////        disp_warning(DISPLAY_ON);
////////    }
////////    else
////////    {
////////        disp_warning(DISPLAY_OFF);
////////        
////////        // ����״̬
////////        if ((g_run_params.st&0x03) == 0x00)//�����λΪ00Hʱ��ôΪ����
////////        {
////////            disp_valve_sts(DISPLAY_ON);
////////            //disp_valve_sign(DISPLAY_OFF);
////////        }
////////        else if ((g_run_params.st&0x03) == 0x01)//�����λΪ01Hʱ��ôΪ�ط�
////////        {
////////            disp_valve_sts(DISPLAY_OFF);
////////            //disp_valve_sign(DISPLAY_OFF);
////////        }
////////    }
////////    
////////    /* Ƿ�� */
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
*  ����Ļ��ʾ�¶�
*/
void menu_main(void)
{
    menu_common();
////////    disp_temp_room(g_run_params.temp);
    // ����״̬������ʾ
    lcd_clr_region();
    lcd_temp_data_disp(g_run_params.temp);
  
}

/*! \brief
*  ����ID��ʾ
*/
void menu_ID(void)
{
    menu_common();
    lcd_clr_region();
    disp_set_id();
}



/*! \brief
*  �˵���ѯ
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
    case MENU_VIEW_IDX_TEMP:    // �����¶Ⱥ�ʱ������
        //printf("0\r\n");
//        disp_temp_room(g_run_params.temp);
        lcd_temp_data_disp(g_run_params.temp);
        break;
    case MENU_VIEW_IDX_TEMP_SET:        // �趨�¶�
//        disp_temp_set(g_sys_params.temp_set);
        lcd_set_temp_disp();
        break;
//////    case MENU_VIEW_IDX_BALANCE: //ʣ������
//////        disp_surplus_heat(g_sys_params.balance, g_sys_params.balance_unit);
//////        //printf("1\r\n");
//////        break;
    case MENU_VIEW_IDX_HEATING_QUANTITY_TOTAL: //�ۼ�����
        //printf("Heat Quantity Total:%lu\r\n", gp_sys_params->heating_quantity_total);
//        disp_heat_quantity(g_sys_params.heating_quantity, g_sys_params.heating_quantity_unit);
        lcd_heating_quantity_disp();
        //printf("2\r\n");
        break;      
    case MENU_VIEW_IDX_HEATING_TIME_TOTAL:  //����ʱ��
        //printf("Heat Time Total:%lu\r\n", gp_sys_params->heating_time_total);
////////        disp_heat_time(g_sys_params.heating_time);
        lcd_heating_time_disp();
        //printf("3\r\n");
        break;
    case MENU_VIEW_IDX_FAULT:          // ͨѶ
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
//////        //        u8 room_name[6] = {'1','2','3','4','5','6'};//����һ������
//////        //        //���µ������ϵͳ��������Ҳ�����鸳ֵ��һ�ַ�ʽ
//////        //        memcpy(g_sys_params.room_name, room_name, sizeof(g_sys_params.room_name)/sizeof(g_sys_params.room_name[0]));
//////        disp_room_id((char*)g_sys_params.room_name, sizeof(g_sys_params.room_name)/sizeof(g_sys_params.room_name[0])); //(char*)Ϊǿ������ת����g_sys_params.room_name�������ǵ�ַ
//////        //printf("strlen(id)=%d\r\n" ,strlen(id));
//////        //printf("5\r\n");
//////        break;  
//////    case MENU_VIEW_IDX_ID_LEFT:
//////        disp_id(g_sys_params.addr, 3);  //�����������˴���
//////        //printf("6\r\n");
//////        break;
//////    case MENU_VIEW_IDX_ID_RIGHT:
//////        disp_id(g_sys_params.addr+3, 2);//g_sys_params.addr+3�ǵ�ַ
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
*  ��ص�����ѡ��
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