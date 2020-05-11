#ifdef HAVE_RTC_INTER

#define _RTC_C_
#include "RTC_inter.h"
//#include "main.h"

#include "bsp.h"
#include "sysparams.h"

void STM8RTC_Initialiazation(void)
{
    RTC_InitTypeDef   RTC_InitStr;
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);				//启动 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_4);			//RTC时钟配置  38K  /   4 = 9500 Hz

    RTC_WakeUpCmd(DISABLE);   
	RTC_ClearITPendingBit(RTC_IT_WUT); 
    RTC_ITConfig(RTC_IT_WUT, DISABLE); 
    RTC_ITConfig(RTC_IT_ALRA, DISABLE);
    RTC_ITConfig(RTC_IT_ALRA, DISABLE);     

    RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;  
    RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
    RTC_InitStr.RTC_SynchPrediv = 0x4B;          //    9500/(124(7c)+1)/(75(4b)+1) = 1 hz
  

    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR2 &=~(1<<(u8)0);//关闭ALRAE
    RTC->CR2 &=~(1<<(u8)4);//关闭ALRAIE
    RTC->WPR = 0xFF;

    RTC_Init(&RTC_InitStr);
    RTC_WakeUpCmd(ENABLE);
}

/*

  UpdateRTC_TO_SoftTime(RealClock_Str * SOFTReal)
  @bewrite  update SoftRealTime TO  Device calendar
  @parameters   SOFTReal   The system SoftReal Time point (Soft Calendar)
  @returnValue  1:NO Any Error  0X0A update time occurred error 0XA0:update data occdurred error

*/
u8 UpdateSoftTime_TO_RTC(RealClock_Str * SOFTReal)
{

	RTC_TimeTypeDef   RTC_TimeStr;
	RTC_DateTypeDef   RTC_DateStr;
	u8 ErrorFlg =State_OK;
	RTC_TimeStr.RTC_Seconds =  SOFTReal->Second ;
	RTC_TimeStr.RTC_Minutes =  SOFTReal->Minute ;
	RTC_TimeStr.RTC_Hours   =  SOFTReal->Hour ;
  	ErrorFlg =RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);	//set the Device RTC -time
 	if(ErrorFlg ==ERROR)
  	{
		ErrorFlg =State_Error;
		return ErrorFlg;
  	}
	RTC_DateStr.RTC_WeekDay =(RTC_Weekday_TypeDef)((SOFTReal->Week==0)?(RTC_Weekday_Sunday):SOFTReal->Week);
	RTC_DateStr.RTC_Date = (uint8_t)SOFTReal->Day ;
	RTC_DateStr.RTC_Month= (RTC_Month_TypeDef)SOFTReal->Month ;
	RTC_DateStr.RTC_Year = (u8)(SOFTReal->Year-2000);
	ErrorFlg =RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);  //set the RTC-data
	if(ErrorFlg ==ERROR)
  	{
		ErrorFlg =State_Error;

  	}
	return ErrorFlg;
}

/*

  UpdateRTC_TO_SoftTime(RealClock_Str * SOFTReal)
  @bewrite update Device calendar TO SoftRealTime
  @parameters   SOFTReal   The system SoftReal Time point (Soft Calendar)


*/
u8  UpdateRTC_TO_SoftTime(RealClock_Str * SOFTReal)
{

	u8 ErrFlg =SUCCESS;
	RTC_TimeTypeDef   RTC_TimeStr;
	RTC_DateTypeDef   RTC_DateStr;

	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStr);
	SOFTReal->Second = RTC_TimeStr.RTC_Seconds;
	SOFTReal->Minute = RTC_TimeStr.RTC_Minutes;
	SOFTReal->Hour = RTC_TimeStr.RTC_Hours;


	RTC_GetDate(RTC_Format_BIN,&RTC_DateStr);
	SOFTReal->Week =((RTC_DateStr.RTC_WeekDay ==7)?0:RTC_DateStr.RTC_WeekDay) ;   // week 0~6
	SOFTReal->Day = RTC_DateStr.RTC_Date;
	SOFTReal->Month =RTC_DateStr.RTC_Month ;
	SOFTReal->Year =(u16)(RTC_DateStr.RTC_Year+2000) ;

	if ((RTC_DateStr.RTC_WeekDay ==0)||(RTC_DateStr.RTC_Year==0 )||(RTC_DateStr.RTC_Date ==0)||(RTC_DateStr.RTC_Month ==0))
	{
		ErrFlg =ERROR;
	}

	
//	LowPower_Ctrler.LowPow_RTC_Flg =Flag_True;	//实时时钟读取完成标志置位
	
	return ErrFlg;
}

/**
  * @brief  Second change to Hour
  * @param  Second:   want to change Second  
  * 
  * @retval changed Hour
  */
u32 SectoHour(u32 Second)
{
    u32 Hour = 0;
    Hour = Second / (u32)(3600);
    return Hour;
}

#endif














