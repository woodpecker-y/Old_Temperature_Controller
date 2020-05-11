/*    调用流程:
*       |-->rtc_init()
*           |-->rtc_read()
*           |-->rtc_write()
*           |-->rtc_read_bcd()
*           |-->rtc_write_bcd()
******************************************************************************/
#include "rtc_ext.h"
#include "bsp.h"
#include "utili.h"

#ifdef HAVE_RTC_EXT

/*! \brief
*       rtc initilization
* \return
*       none
*/
void rtc_init(void)
{
    BSP_RTC_Init();
}

/*! \brief
*       rtc read time of the format of struct tm
* \param t[IN]        - time of the format of struct tm
* \return
*       none
*/
void rtc_read(struct tm *t)
{
    BSP_RTC_Read(t);
}

/*! \brief
*       rtc write time of the format of struct tm
* \param t[IN]        - time of the format of struct tm
* \return
*       none
*/
void rtc_write(struct tm t)
{
    BSP_RTC_Write(t);
}

/*! \brief
*       rtc read time of the format of bcd bytes
* \param bcd_datetime[OUT]        - time of the format of struct tm
* \param len[IN]                 - the length of bcd bytes array 
* \return
*       none
*/
void rtc_read_bcd(unsigned char *bcd_datetime, unsigned char len)
{
    struct tm t;

    rtc_read(&t);

    dec_2_bcd_type(bcd_datetime+0, 2, t.tm_year+1900, ORD_MOTOR);        
    dec_2_bcd_type(bcd_datetime+2, 1, t.tm_mon+1, ORD_MOTOR);        
    dec_2_bcd_type(bcd_datetime+3, 1, t.tm_mday , ORD_MOTOR);        
    dec_2_bcd_type(bcd_datetime+4, 1, t.tm_hour , ORD_MOTOR);        
    dec_2_bcd_type(bcd_datetime+5, 1, t.tm_min  , ORD_MOTOR);        
    dec_2_bcd_type(bcd_datetime+6, 1, t.tm_sec  , ORD_MOTOR);  
  
    return;
}

/*! \brief
*       rtc write time of the format of bcd bytes
* \param bcd_datetime[IN]        - time of the format of struct tm
* \param len[IN]                 - the length of bcd bytes array 
* \return
*       none
*/
void rtc_write_bcd(unsigned char *bcd_datetime, unsigned char len)
{
    struct tm t;

    t.tm_year = bcd_2_dec_type(bcd_datetime+0, 1, ORD_MOTOR)*100;
    t.tm_year = bcd_2_dec_type(bcd_datetime+1, 1, ORD_MOTOR)+t.tm_year-1900;
    // 2013-05-14 14:44修复struct tm格式时间与常规时间月份差1的BUG
    t.tm_mon  = bcd_2_dec_type(bcd_datetime+2, 1, ORD_MOTOR)-1;
    t.tm_mday = bcd_2_dec_type(bcd_datetime+3, 1, ORD_MOTOR);
    t.tm_hour = bcd_2_dec_type(bcd_datetime+4, 1, ORD_MOTOR);
    t.tm_min  = bcd_2_dec_type(bcd_datetime+5, 1, ORD_MOTOR);
    t.tm_sec  = bcd_2_dec_type(bcd_datetime+6, 1, ORD_MOTOR);

    rtc_write(t);
}   

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

#endif
