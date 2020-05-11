#ifdef HAVE_RTC_EXT

#include "bsp.h"
#include "bsp_rtc_ext.h"

/*! \brief
*       rtc initilization
*/
void BSP_RTC_Init(void)
{
    RTC_InitTypeDef   RTC_InitStr;
    RTC_TimeTypeDef   RTC_TimeStr;
    RTC_DateTypeDef   RTC_DateStr;

    /* Enable LSE */
    CLK_LSEConfig(CLK_LSE_ON);
    /* Wait for LSE clock to be ready */
    while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
    /* wait for 1 second for the LSE Stabilisation */
    delay_ms(1000);

    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

    RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStr.RTC_AsynchPrediv = 0x7F;
    RTC_InitStr.RTC_SynchPrediv = 0x00FF;
    RTC_Init(&RTC_InitStr);

    RTC_DateStructInit(&RTC_DateStr);
    RTC_DateStr.RTC_WeekDay = RTC_Weekday_Friday;
    RTC_DateStr.RTC_Date  = 0x01;
    RTC_DateStr.RTC_Month = 0x01;
    RTC_DateStr.RTC_Year  = 0x14;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStr);

    RTC_TimeStructInit(&RTC_TimeStr);
    RTC_TimeStr.RTC_Hours   = 0x08;
    RTC_TimeStr.RTC_Minutes = 0x00;
    RTC_TimeStr.RTC_Seconds = 0x00;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStr);
}

/*! \brief
*       rtc read time with tm format
* \param t[OUT]         - time format named "tm"
*/
void BSP_RTC_Read(struct tm *t)
{
    RTC_TimeTypeDef   RTC_TimeStr;
    RTC_DateTypeDef   RTC_DateStr;

    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);

//    t->tm_year = RTC_DateStr.RTC_Year+100;
//    t->tm_mon  = RTC_DateStr.RTC_Month-1;
    t->tm_year = RTC_DateStr.RTC_Year;
    t->tm_mon  = RTC_DateStr.RTC_Month;
    t->tm_mday = RTC_DateStr.RTC_Date;
    t->tm_week = RTC_DateStr.RTC_WeekDay;
    
    t->tm_hour = RTC_TimeStr.RTC_Hours;
    t->tm_min  = RTC_TimeStr.RTC_Minutes;
    t->tm_sec  = RTC_TimeStr.RTC_Seconds;

    return;
}

/*! \brief
*       rtc write time with tm format
* \param t[IN]         - time format named "tm"
*/
void BSP_RTC_Write(struct tm t)
{
    RTC_TimeTypeDef   RTC_TimeStr;
    RTC_DateTypeDef   RTC_DateStr;

    RTC_DateStructInit(&RTC_DateStr);
//    RTC_DateStr.RTC_Year    = t.tm_year-100;
//    RTC_DateStr.RTC_Month   = t.tm_mon+1;
    RTC_DateStr.RTC_Year    = t.tm_year;
    RTC_DateStr.RTC_Month   = t.tm_mon;
    RTC_DateStr.RTC_Date    = t.tm_mday;
    RTC_DateStr.RTC_WeekDay =(RTC_Weekday_TypeDef)((t.tm_week==0)?(RTC_Weekday_Sunday):t.tm_week);

    RTC_TimeStructInit(&RTC_TimeStr);
    RTC_TimeStr.RTC_Hours   = t.tm_hour;
    RTC_TimeStr.RTC_Minutes = t.tm_min;
    RTC_TimeStr.RTC_Seconds = t.tm_sec;
    

    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);
}

#endif





