#ifndef  _LCD_DRIVER_H_
#define  _LCD_DRIVER_H_

#include "stm8l15x.h"
//#include "TIMER.h"
#include "bsp.h"

#ifdef _LCD_DRIVER_C_
#define LCD_DRIVER_EXT
#else 
#define LCD_DRIVER_EXT extern
#endif

/*

              1A                    2A                   3A                 
            -----             ------             -----
          |         |           |          |          |          |   
        1|         |1B   2F|          |2B  3F|          |2B    
          |  1G  |           |          |          |          |
          ------              -----             ------
          |         |           |          |          |          |
        1|         |1C   2E|          |2C  3E|          |3C
          |         |           |          |          |          |
           -----              ------            ------
               1D                    2D                     3D


      COM0 COM1 COM2 COM3
SEG0 { A  , F  , E  , 1  }  
SEG1 { B  , G  , C  , D  }

如显示‘0 ’，则
 { 1 , 1 , 1 , 0 }
 { 1 , 0 , 1 , 1 }
-------------------
=  3   1   3   3  hex
=> '0' = 0x3132
*/



#define 	LCD_YEAR_H_POSITION   				1
#define 	LCD_YEAR_L_POSITION    				2  
#define 	LCD_MONTH_L_POSITION  				3
#define 	LCD_DAY_H_POSITION    				4
#define 	LCD_DAY_L_POSITION    				5
#define 	LCD_WEEK_POSITION    				6    
#define 	LCD_HOUR_H_POSITION      			12
#define 	LCD_HOUR_L_POSITION      			13
#define 	LCD_MINUTE_H_POSITION    			14
#define 	LCD_MINUTE_L_POSITION    			15
#define 	LCD_TEMP_D_POSITION   				7
#define 	LCD_TEMP_U_POSITION   				8
#define 	LCD_TEMP_P_POSITION   				9

#define  	Heat_Value_HT_POSITION    		10        	//	热量		十万位     hundred   thoUsand
#define  	Heat_Value_TT_POSITION    		11    	 	//   热量		万位 TEN  THOUSAND
#define  	Heat_Value_U_POSITION    		12		 	//   热量		千位   thoUsand
#define  	Heat_Value_H_POSITION    		13		 	//	热量		百位  hundred
#define  	Heat_Value_T_POSITION    		14     		//	热量		十位 TEN
#define  	Heat_Value_O_POSITION    		15		    //	热量		个位  ONE

#define  	Heat_Time_U_POSITION    		12			// 热量时间	千位   thoUsand
#define  	Heat_Time_H_POSITION    		13			// 热量时间	百位  hundred
#define  	Heat_Time_T_POSITION				14      		// 热量时间	十位 TEN
#define  	Heat_Time_O_POSITION    		15			// 热量时间	个位  ONE

#define  	LCD_COM_K_POSITION      		12   		//   通讯、设备组号千位
#define  	LCD_COM_H_POSITION      		13   		//   通讯、设备组号百位
#define  	LCD_COM_T_POSITION      		14   		//   通讯、设备组号十位
#define  	LCD_COM_O_POSITION      		15   		//   通讯、设备组号个位

#ifdef  _LCD_DRIVER_C_          					//  对应的c文件要用到的局部变量

/*Local Variables -------------------------------------------------------------*/
#define   PUTS1_S2(x)    		((x) << 1)    
#define   PUTS3_S4(x)    		((x) << 3)   
#define   PUTS5_S6(x)    		((x) << 5)  
#define   PUTS7(x)       		((x) << 7)
#define   PUTS0(x)       		((x) >> 1)              //  放在下一个的  RAM中 
 
#define   PUTS0_0(x)       	((x) << 0)              //  正常的 0 到7 ， 不用移位
#define   PUTS1_0(x)       	((x) << 1) 
#define   PUTS2_0(x)       	((x) << 2) 
#define   PUTS3_0(x)       	((x) << 3) 
#define   PUTS4_0(x)       	((x) << 4) 
#define   PUTS5_0(x)       	((x) << 5) 
#define   PUTS6_0(x)       	((x) << 6)  
#define   PUTS7_0(x)       	((x) << 7)

#define  bit(x)  ((uint8_t)(1 << x))

uint8_t Clear0 = (~(uint8_t)(0x01));
uint8_t Clear1 = (~(uint8_t)(0x02));
uint8_t Clear2 = (~(uint8_t)(0x04));  
uint8_t Clear3 = (~(uint8_t)(0x08));
uint8_t Clear4 = (~(uint8_t)(0x10));  
uint8_t Clear5 = (~(uint8_t)(0x20));
uint8_t Clear6 = (~(uint8_t)(0x40));  
//uint8_t Clear7 = (~(uint8_t)(0x80));
uint8_t Clear7 = 0x7F;


uint8_t 	digit[4];     /* Digit frame buffer */


__CONST 		uint16_t 	mask[4] = {0xF000, 0x0F00, 0x00F0, 0x000F};
__CONST 		uint8_t 	shift[4] = {12 , 8, 4 , 0};
			uint8_t 	LCD_DisBuffer[4];
                                 /* 0            1              2              3             4              5             6             7             8              9   */
__CONST 		uint16_t	NumberMap[10] = {0x3132, 0x2020, 0x3212, 0x3222, 0x2320, 0x1322, 0x1332, 0x3020, 0x3332, 0x3322};             


#endif     //  _LCD_DRIVER_C_ 






typedef enum 
{
	WRITE = 0, 
	CLEAR = !WRITE
}LCD_CLEAR_FLAG;

typedef enum 
{
	INPUT = 0,             			//  Input  ，MARK:  室内温度
	T_SET = 1,                		//  Set  ，MARK:设定温度
	EXPORT = 2           	 		//  export  ，MARK:室外温度
}LCD_PT_TEMP_TYPE;

#define 	LCD_CLEAR_MSB		(uint8_t)0x0F
#define 	LCD_CLEAR_LSB		(uint8_t)0xF0


typedef struct
{
	u32	KEY1_PRE_SYSTEMTICK;			//按键记录原SYSTEMTICK的值 
	u8	KEY1_BACKUP_RA;			//备份RA
	u32	KEY3_PRE_SYSTEMTICK;			//按键记录原SYSTEMTICK的值 
	u8	KEY3_BACKUP_RA;			//备份RA
	u8	KEY_FIRST_PRESS_FLAG;
}LCD_KEY_DELAY_Str;


typedef struct
{
	u8		BlinkFlg;		     	// LCD  MARK 闪烁标志     0 不闪烁  ，0xAA 闪烁
	u8		BlinkNum;          	// 闪烁次数
	u32		BlinkTimeRcrd;      		// 闪烁时间记录
	u8		BlinkTimeRcrdFlg;     	// 闪烁时间记录标志，   0 没有记录 ，0xAA已经记录
}LCD_Blink_Str;
LCD_DRIVER_EXT		LCD_Blink_Str			LCD_Blink;




/******************************************************/
LCD_DRIVER_EXT		LCD_KEY_DELAY_Str 		lcd_key;


LCD_DRIVER_EXT		void 	LCD_Config(void);                      													//    Configure LCD
LCD_DRIVER_EXT  	void 	Convert(uint8_t* c);          															//   Converts an ascii char to the a LCD digit (previous coding).
LCD_DRIVER_EXT  	void 	LCD_NUM_WriteChar(uint8_t ch, uint8_t position, LCD_CLEAR_FLAG flag);   						//  this function writes a NUM in the LCD frame buffer.
LCD_DRIVER_EXT		void 	LCD_MARK_WriteChar(uint8_t* ch, LCD_CLEAR_FLAG flag);          								// MARK
LCD_DRIVER_EXT		void 	LCD_Whole_Display(void);                													// Display all  
LCD_DRIVER_EXT		void 	LCD_Whole_Clear(void);                  													// clear  all 
LCD_DRIVER_EXT  	void 	LCD_Time_Display(uint8_t hour, uint8_t minute, LCD_CLEAR_FLAG flag);
LCD_DRIVER_EXT		void 	LCD_Date_Display(uint16_t year, uint8_t month, uint8_t day, uint8_t week, LCD_CLEAR_FLAG flag);   	//  This function Display DATE  on LCD
LCD_DRIVER_EXT		void 	LCD_RTC_Time_Display(void);
LCD_DRIVER_EXT  	void 	LCD_Temp_Set(uint16_t Set_Temp_100, LCD_PT_TEMP_TYPE PT_Temp_type, LCD_CLEAR_FLAG flag);  			//  This function set  TEMPERATURE on LCD
LCD_DRIVER_EXT  	void 	LCD_Temp_Display(uint16_t DS18B20_Temp_100, LCD_PT_TEMP_TYPE PT_Temp_type, LCD_CLEAR_FLAG flag);   	//  his function Display  TEMPERATURE on LCD
LCD_DRIVER_EXT  	void 	LCD_PowerOn_Display(void);     															//  This function display PowerOn  screen
LCD_DRIVER_EXT  	void 	LCD_MARK_Display(void);    																//  This function display the LCD MARK
LCD_DRIVER_EXT		void 	DevWorkState_DispCtrl(void);																	//LCD MARK  闪烁函数
LCD_DRIVER_EXT		void 	LCD_COM_Display(u16 Device_GN, LCD_CLEAR_FLAG flag);										//	Communication/ device group number
LCD_DRIVER_EXT  	void 	LCD_HEAT_Display(u32 Heat_Value, LCD_CLEAR_FLAG flag);   										//This function display the LCD HEAT value
LCD_DRIVER_EXT		void 	LCD_HEAT_Time_Display(u32 Total_OpTim, LCD_CLEAR_FLAG flag);									//	 This function display the LCD HEAT time
LCD_DRIVER_EXT		void 	LCD_1_Screen_Display(void);																//	This function display the LCD  1  screen
////LCD_DRIVER_EXT		void 	LCD_2_Screen_Display(void);																//	This function display the LCD  2  screen
//LCD_DRIVER_EXT		void 	LCD_3_Screen_Display(void);																//	This function display the LCD  3  screen
//LCD_DRIVER_EXT		void 	LCD_4_Screen_Display(void);																//	This function display the LCD  4  screen
//LCD_DRIVER_EXT		void 	LCD_5_Screen_Display(void);																//	This function display the LCD  5  screen
LCD_DRIVER_EXT		void 	LCD_1_Screen_Clear(void);
LCD_DRIVER_EXT		void 	LCD_2_Screen_Clear(void);
LCD_DRIVER_EXT		void 	LCD_3_Screen_Clear(void);
LCD_DRIVER_EXT		void 	LCD_4_Screen_Clear(void);
LCD_DRIVER_EXT		void 	LCD_5_Screen_Clear(void);
LCD_DRIVER_EXT		void 	LCD_Key1_Screen_Display(void);															//	Key1   screen
LCD_DRIVER_EXT		void 	LCD_Key3_Screen_Display(void);															//	Key2   screen

/* 显示设定温度 */
void lcd_set_temp_disp();
/* 显示室内温度和时间日期 */
void lcd_temp_data_disp(s16 temp);
/* 只显示室内温度 */
void lcd_temp_disp(void);
/* 清除公共区域，除了状态标志 */
void lcd_clr_region(void);
/* 显示累计热量 */
void lcd_heating_quantity_disp();
/* 显示累计开阀时间 */
void  lcd_heating_time_disp(void);
/* 显示网络组号 */
void lcd_device_gn_disp(void);



#endif     //_LCD_DRIVER_H_ 




