
/*

	DS18B20 服务功能模块

*/

#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "stm8l15x.h"
#include "bsp.h"

#ifdef _DS18B20_C_
#define DS18B20_EXT
#else
#define DS18B20_EXT extern
#endif



//#define DQ_BIT_W1				GPIO_SetBits(DS18B20_DQ_PORT,DS18B20_DQ_PIN)
//
//#define DQ_BIT_W0				GPIO_ResetBits(DS18B20_DQ_PORT,DS18B20_DQ_PIN)
//
//#define DQ_BIT_OutPut			GPIO_Init(DS18B20_DQ_PORT,DS18B20_DQ_PIN,GPIO_Mode_Out_PP_High_Fast);  // direction   output
//
//
//#define DQ_BIT_InPut			GPIO_Init(DS18B20_DQ_PORT,DS18B20_DQ_PIN,GPIO_Mode_In_PU_No_IT);    	 // direction  input
//
//#define DQ_BIT_Read			(GPIO_ReadInputDataBit(DS18B20_DQ_PORT,DS18B20_DQ_PIN))           	//  input mode,read the  DQ data
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DQ_BIT_W1				GPIO_SetBits(DS18B20_DQ_PORT,DS18B20_DQ_PIN)

#define DQ_BIT_W0				GPIO_ResetBits(DS18B20_DQ_PORT,DS18B20_DQ_PIN)

#define DQ_BIT_OutPut			GPIO_Init(DS18B20_DQ_PORT,DS18B20_DQ_PIN,GPIO_Mode_Out_PP_High_Fast);  // direction   output


#define DQ_BIT_InPut			GPIO_Init(DS18B20_DQ_PORT,DS18B20_DQ_PIN,GPIO_Mode_In_PU_No_IT);    	 // direction  input

#define DQ_BIT_Read			(BSP_GPIO_ReadState(PORTF, PIN1))           	//  input mode,read the  DQ data


#define AVEtemp_BufferSize 		100
typedef struct
{

	u8	DS18b20Flag;			//Ds18b20Flag=1表示存在，=0表示不存在
	s16	Sys_Temp;				//系统实际温度
	s16	Sys_Temp1;				//系统实际温度(平均温度使用)
	u8	ScanCtrl_Flag;			//扫描控制标志 0温度获取 其他 扫描定时
}Ds18B20GetTemp_Str;

typedef struct
{
	u8	Count;				//计数器
	u8	StackFull;				//堆栈满标志
	s32	Temp_Add;				//总加和
	s16	Buffer[AVEtemp_BufferSize];	//缓冲序列
}AveTempCal_Stru;

#ifdef _DS18B20_C_

#endif

DS18B20_EXT	 	Ds18B20GetTemp_Str 	DS_18B20Controler;
DS18B20_EXT		AveTempCal_Stru 		AVE_TempData;
DS18B20_EXT		u32 Ds18b20_TimeCount;
DS18B20_EXT		void AVETemp_PointInput(s16* Temp);
DS18B20_EXT		s16 AVETemp_Calculate(s16* Temp);

DS18B20_EXT		u8 DS18B20_Init(void);
DS18B20_EXT		void DS18B20_PowerON_Init(void);

DS18B20_EXT   	void Write_One_Byte(u8 byte);
DS18B20_EXT   	u8 Read_One_Byte(void);


DS18B20_EXT		void DS18B20_GetTemp_Server(void);
DS18B20_EXT		void DS18B20_Start(void);
DS18B20_EXT		s16 DS18B20_ReadTemp(void);

void ds18b20_close(void);

void sample_temp(void);

#endif




