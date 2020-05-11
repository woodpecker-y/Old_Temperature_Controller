
/*    调用流程:
*       |-->rtc_init()
*           |-->rtc_read()
*           |-->rtc_write()
*           |-->rtc_read_bcd()
*           |-->rtc_write_bcd()
******************************************************************************/
#ifndef _RTC_H
#define _RTC_H

#ifdef HAVE_RTC

#include "bsp.h"

#define FMT_DATETIME    0x00
#define FMT_DATE        0x01
#define FMT_TIME        0x02

/*! \brief
*       rtc initilization
* \return
*       none
*/
void rtc_init(void);

/*! \brief
*       rtc read time of the format of struct tm
* \param t[IN]        - time of the format of struct tm
* \return
*       none
*/
void rtc_read(struct tm *t);

/*! \brief
*       rtc write time of the format of struct tm
* \param t[IN]        - time of the format of struct tm
* \return
*       none
*/
void rtc_write(struct tm t);

/*! \brief
*       rtc read time of the format of bcd bytes
* \param bcd_datetime[OUT]        - time of the format of struct tm
* \param len[IN]                 - the length of bcd bytes array 
* \return
*       none
*/
void rtc_read_bcd(unsigned char *bcd_datetime, unsigned char len);

/*! \brief
*       rtc write time of the format of bcd bytes
* \param bcd_datetime[IN]        - time of the format of struct tm
* \param len[IN]                 - the length of bcd bytes array 
* \return
*       none
*/
void rtc_write_bcd(unsigned char *bcd_datetime, unsigned char len);

#endif

#endif
