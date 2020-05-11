#include <stdio.h>
#include "adf.h"
#include "board.h"
#include "sysparams.h"
#include "verify.h"
#include "awu.h"
#include "motor.h"
#include "BSP_SysTimer.h"
#include "stm8l15x_tim1.h"
#include "flash_ext.h"
#include "disp.h"
#include "menu.h"
#include "task_report.h"
#include "task_key.h"
#include "task_config.h"
#include "task_adc.h"
#include "task_disp.h"
#include "task_disp.h"
#include "task_ds18b20.h"
#include "task_main.h"

#define TEST_WDG 0
#define TEST_COM 1
#define TEST_SOFT_TIMER 0
#define TEST_DELAY_MS 0
#define TEST_DELAY_US 0

void hardware_init(void)
{
//    int adc = 0;
//    u32 adc_ext = 0;
//    int voltage = 0;
//    uint8_t test_data[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
//    uint8_t test_data[11] = "start-test";
//    u8 rv = 0;
//    u8 *rcv_dat = NULL;
//    u16 rcv_len = 0;
//    u8 rcv_rflen = 0;
//
//    u16 set_temp = 2800;

    //int rv;
    /* PORT初始化，有500uA的功耗 */
    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOG, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );

//    GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT );
//    GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow );

    /* ----------------关闭ds18b20---------------- */
    ds18b20_close();  // 如果注释会增加20uA的功耗

    /* 配置串口引脚 */
//    gpio_init(PORTA, PIN2, GPIO_Mode_Out_PP_High_Fast);//TX
//    gpio_init(PORTA, PIN3, GPIO_Mode_In_FL_No_IT);//RX
//    gpio_init(PORTA, PIN2, GPIO_Mode_Out_PP_Low_Slow);//TX
//    gpio_init(PORTA, PIN3, GPIO_Mode_Out_PP_High_Fast);//TX

    /* ----------------system clock init---------------- */
    sysclk_init_hsi(CLK_SYSCLKDiv_2);   //二分频

    /* ----------------实时时钟初始化,使用内部的38MHz----------------  */
    /* 暂时使用，后期使用外部的32.768KHz晶振，时钟分频要更改 */
    STM8RTC_Initialiazation();

    /* ----------------debug,暂时关闭因为没有外部晶振---------------- */
//    rtc_init();

    /* ----------------自动唤醒时钟初始化---------------- */
    awu_init();
    awu_enable(CYCLE_AWU);

#if 1
    struct tm t;

    t.tm_year = 19;
    t.tm_mon  = 8;
    t.tm_mday = 19;
    t.tm_week = 1;
    t.tm_hour = 8;
    t.tm_min  = 0;
    t.tm_sec  = 0;

    rtc_write(t);   //把时钟写进去
#endif

      /* ----------------和无线引脚暂时冲突---------------- */
      /* com load configuration & init */
////////    com_load_config(com_cfg, sizeof(com_cfg)/sizeof(com_cfg[0]));
////////    com_init(COM1, 2400, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (uint8_t)0x0C, ENABLE, 0);
//////////    com_init(COM3, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_All, ENABLE, 0);
////////    com_recv_init(COM1);
////////    //com_recv_init(COM2);

    /* ----------------LCD初始化---------------- */
    LCD_Config();
    LCD_Whole_Clear();   //清除整个屏幕信息

    /* 显示软件版本号0x56 */
    LCD_NUM_WriteChar(0x06, LCD_TEMP_D_POSITION, WRITE);    // LCD_TEMP_D_POSITION   温度十位  用于显示版本号
    LCD_NUM_WriteChar(0x00, LCD_TEMP_U_POSITION, WRITE);	// LCD_TEMP_U_POSITION   温度个位  用于显示版本号

    delay_ms(2000);
    LCD_Whole_Clear();   //清除整个屏幕信息
//    LCD_Whole_Display();
//    delay_ms(10000);

    /* 厂家给的采集温度程序测试，未成功 */
//    while (1)
//    {
////    sample_temp();
//      DS18B20_GetTemp_Server();
//      delay_ms(2000);
//    }
    /* 采集温度后屏幕显示测试 */
//    while(1)
//    {
//      /* 显示温度 */
//      DS18B20_GetTemp_Server();
//      /* 显示时间 */
////      rtc_read(&t);
////      printf("t.tm_year = %d\r\n", t.tm_year);
////      LCD_Date_Display(t.tm_year, t.tm_mon, t.tm_mday, t.tm_week, WRITE);	//	Display DATE
////      LCD_Time_Display( t.tm_hour, t.tm_min,WRITE);			//	Display  TIME
//
//      lcd_temp_data_disp(g_run_params.temp);
//      delay_ms(2000);
//      lcd_clr_region();
////      lcd_temp_data_disp();
//
//      delay_ms(2000);
//
//    }

    /* 外部的adc测试 */
    BSP_ADC_LoadConfig1(adc_cfg, sizeof(adc_cfg)/sizeof(adc_cfg[0]));
    adc_close1(ADC_BATT);
    GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT );       // 必须加，不加功耗会高
//    adc_power_on1(ADC_BATT);
//    adc_init1(ADC_BATT);
//    delay_ms(50);
//    while (1) {
//      adc_ext = adc_sample1(ADC_BATT);
////      printf("adc_ext = %d\r\n", adc_ext);
//      voltage = (int)(3300 * adc_ext / 4096);
//      LCD_HEAT_Display((u32)voltage, WRITE);                //  用于显示
//      LCD_Temp_Display((s16)voltage, INPUT, WRITE);
//      delay_ms(2000);
//    }

    /* 内部的adc测试 */
//    adc_close_vrefint1();
//    adc_init_vrefint1();
//    delay_ms(2000);
//    adc = adc_sample_vrefint1();
//    while (1) {
//      adc = adc_sample_vrefint1();
//      adc = (int)(4997120 / adc);         // 4997120 = 1220*4096
//      LCD_HEAT_Display((u32)adc,WRITE);                //  用于显示
////        printf("adc = %d\r\n", adc);
//        delay_ms(2000);
//    }

    /* ----------------load key configure & init---------------- */
    key_load_config(key_config, sizeof(key_config)/sizeof(key_config[0]));
    key_init();

    /* ----------------rf无线需要使用测试暂时使用---------------- */
//    enableInterrupts();           //始能终端函数
    /* 使用无线一定要打开中断并且而且要配置中断函数 */
    /* ----------------rf config & init---------------- */
    rf_load_config(&rf_cfg);
    rf_close();
    rf_power_off();

    GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow );  // 由外围电路决定



      /* si4432无线进行测试 */
//    rf_power_on();
//    rv = rf_init(RF_FRE_470_5M, RF_DATA_RATE_1200, TXPOW_20dBm, 0);
//    if (rv != E_RF_OK)
//    {
//      delay_ms(1);
//    }
//
//    rf_rcv_init();
//
//    while(1)
//    {
//      /* 接收 */
//      rv = rf_rcv_data(&rcv_dat, &rcv_rflen);
//      if(rv == 0)
//      {
////        lcd_temp_data_disp(g_run_params.temp);
////        MYLOG_DEBUG_HEXDUMP("RF Config Recv:", rcv_dat, rcv_len);
////        disp_temp_room(330);
////        disp_update();
//          printf("rcv_dat = %s\r\n", rcv_dat);
//          rv = memcmp(rcv_dat, "start-test", 10);
//          if (rv == 0)
//          {
//            /* 发送 */
//            rf_snd_data(test_data, 6);
//          }
//
//
//          rf_rcv_init();
//      }
//
//
////      /* 发送 */
////      rf_snd_data(test_data, 11);
////      delay_ms(3000);
//    }

    /* ----------------暂时没用到---------------- */
    /* lcd config & init */
    /*lcd_load_config(&lcd_config);
    lcd_power_on();
    lcd_init();
    disp_temp_room(250);
    disp_update();
    exti_load_config(pwr_check_cfg, 1);
    exti_init();

    th_sht2x_load_config(&sht20_cfg);
    th_sht2x_power_on();
    th_sht2x_init();
    SHT2x_Init();

    flash_ext_load_config(&flash_config);
    flash_ext_init();

    // RFID load config
    rfid_load_config(&rfid_config);
    rv = rfid_init(&ctrl_reg, &ver_reg);
    if (rv != 0)
    {
      printf("rfid init error\r\n");
    }
    //printf("ctrl_reg:%02X, ver_reg:%02X\r\n", ctrl_reg, ver_reg);
    rfid_close();
    rfid_power_off(); */

    return;
}

void software_init(void)
{
//    u16 gn = 0x4002;    // debug
//    u32 sn = 0x19014002;        //debug
//    u8 rf_fre = 0;
//    u8 rf_channel = 0;

//    printf("sizeof(g_sys_params) = %d\r\n", sizeof(g_sys_params));
    memset(&g_sys_params, 0, sizeof(g_sys_params));
    memset(&g_run_params, 0, sizeof(g_run_params));
    eeprom_read(0, (u8*)&g_sys_params, sizeof(g_sys_params));

    // 测试用 debug
//    eeprom_write_sys(2, &gn, 2);
//    eeprom_write_sys(4, &sn, 4);


    /* 一般在没有ID情况下使用，出厂后为25℃ */
    if (g_sys_params.temp_set == 0) //设定一个温度
    {
        g_sys_params.temp_set = 1800;
    }

    /* 配置无线 */
    g_sys_params.rf_fre = (g_sys_params.Device_SN & 0x0000FFFF)>>12;
//    printf("g_sys_params.rf_fre = %d\r\n", g_sys_params.rf_fre);
    if ((g_sys_params.rf_fre != RF_FRE_470_5M) && (g_sys_params.rf_fre != RF_FRE_480_5M) && (g_sys_params.rf_fre != RF_FRE_490_5M))        // 如果无线不是规定的频率，那么就默认配置410_5MHz
        g_sys_params.rf_fre = RF_FRE_410_5M;    // 默认为410.5M

    // RF通道号
    g_sys_params.rf_channel = (((g_sys_params.Device_SN & 0x00000FFF)>>8)*100) + (((g_sys_params.Device_SN & 0x000000FF)>>4)*10) + (g_sys_params.Device_SN & 0x0000000F);
//    printf("g_sys_params.rf_channel = %d\r\n", g_sys_params.rf_channel);
    if(g_sys_params.rf_channel > 200)
        g_sys_params.rf_channel = 200;  // 默认通道号为200

    // RF功率
    g_sys_params.rf_power = TXPOW_20dBm;

    /* 参数初始化 */
//    g_sys_params.heating_quantity = 12345678;   // 最高为99999999，其中后面两位直接去掉，应该是不要小数部分     // debug
//    g_sys_params.heating_time = 4442400;   // 按秒计算的，4442400/60/60 = 1234小时                              // debug
    /* 设置温度的上限初始化 */
    if (g_sys_params.temp_room_uplimits == 0)
    {
      g_sys_params.temp_room_uplimits = 32;
    }
    /* 设置温度的上限初始化 */
    g_sys_params.temp_room_lowlimits = 0;

    g_sys_params.disp_ctl = 255;        // 显示控制位
//    g_sys_params.workflg = 0xAA;        // 默认在供暖期     // debug
    g_run_params.valve_position = Valve_Open;       // 默认为开阀
    g_run_params.arrears_state = 1;      // 默认欠费
    g_run_params.batt_lvl = 3300;       // 默认电池电量低
    g_run_params.face_state = 0x00;       // 默认没有问题
    g_run_params.sts_login = 1;

    task_18b20_init();
    task_adc_init();    // 上电采集电池电量
    task_report_init();
}

void system_print(void)
{
    u8 gn_arry[2] = {0};
    u8 sn_arry[4] = {0};

    memcpy(gn_arry, &g_sys_params.Device_GN, 2);
    memcpy(sn_arry, &g_sys_params.Device_SN, 4);
    printf("\f");
    printf("productmodel:%s\r\n", product_model);
    printf("Hardware Ver:%s\r\n", hw_ver);
    printf("Software Ver:%s\r\n", sw_ver);
    printf("Product GN :%02X%02X\r\n", gn_arry[0], gn_arry[1]);
    printf("Product SN :%02X%02X %02X%02X\r\n", sn_arry[0], sn_arry[1], sn_arry[2], sn_arry[3]);

    printf("Factory Mode:%d\r\n", g_sys_params.factory_flag);
    printf("Heating Period: %02X\r\n" ,g_sys_params.workflg);
    printf("Temp Set: %d\r\n" ,g_sys_params.temp_set);
    printf("Rf Fre: %u\r\n", g_sys_params.rf_fre);
    printf("Rf Channel: %u\r\n",g_sys_params.rf_channel);
    printf("Com Ok: %lu\r\n", g_sys_params.com_ok);
    printf("Com Total: %lu\r\n", g_sys_params.com_total);
    printf("Temp Uplimits: %u\r\n", g_sys_params.temp_room_uplimits);
}

void main(void)
{
    hardware_init();
    software_init();
    factory_check();
//////    system_print();

    wdg_init(1500);//看门狗函数（如果程序死机1.5s，那么程序重启）
    /* 使能中断 */
    enableInterrupts();
    PWR_UltraLowPowerCmd(ENABLE);   //外部调压
    if (g_sys_params.factory_flag == 1)
    {
//      com_close(COM1);
    }
    LCD_MARK_WriteChar("W", WRITE);     // 信号塔
    while (1)
    {
      /* 阀门状态 */
      if(g_run_params.valve_position == Valve_Open)      	//  阀开
      {
          LCD_MARK_WriteChar("O", WRITE);   				//   O 代表 OPEN  ，MARK:阀开 ,写
          LCD_MARK_WriteChar("C", CLEAR);
      } else if (g_run_params.valve_position == Valve_Close)  //  阀关
      {
          LCD_MARK_WriteChar("C", WRITE);    				//  C 代表 close  ，MARK:阀关 , 写
          LCD_MARK_WriteChar("O", CLEAR);
      } else
      {
          LCD_MARK_WriteChar("C", CLEAR);
          LCD_MARK_WriteChar("O", CLEAR);
      }
        if ((g_sys_params.factory_flag == 0) || (g_run_params.batt_collect_flag == 1)/*&& g_run_params.set_id==1*/)     // 注：内部采集adc的时候必须是wfi模式
        {
//          halt();
            wfi();
        }
        else
        {
            halt();
        }
    }
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    //printf("Wrong parameters value: file %s on line %lu\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif