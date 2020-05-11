/*
***********************************************************************************

***********************************************************************************
*/
#ifndef _RTC_INTER_H_
#define _RTC_INTER_H_

#ifdef HAVE_RTC_INTER


#include "stm8l15x.h"
//#include "TypeDef.h"
//#include "TIMER.h"

#include "bsp.h"
#include "sysparams.h"

#ifdef _RTC_C_
#define RTC_EXT
#else
#define RTC_EXT extern
#endif



#ifdef _RTC_C_


#endif
RTC_EXT void 	STM8RTC_Initialiazation(void);
RTC_EXT u8 	UpdateSoftTime_TO_RTC(RealClock_Str * SOFTReal);
RTC_EXT u8 	UpdateRTC_TO_SoftTime(RealClock_Str * SOFTReal);
RTC_EXT u32	SectoHour(u32 Second);

#endif

#endif