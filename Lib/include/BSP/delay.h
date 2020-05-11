
#ifndef _DELAY_H
#define _DELAY_H

#include "bsp.h"

/*! \brief
*       delay config
* \param pre[IN]        - sysclk div
*       CLK_SYSCLKDiv_1
*       CLK_SYSCLKDiv_2
*       CLK_SYSCLKDiv_4
*       CLK_SYSCLKDiv_8
*       CLK_SYSCLKDiv_16
*/
void delay_config(u8 pre);

/*! \brief
*       millisecond delay
* \param _1ms[IN]        - delay time
*/
void delay_ms(u16 _1ms);

/*! \brief
*       microseconds delay
* \param us[IN]        - delay time
*/
void delay_us(u16 us);

#endif