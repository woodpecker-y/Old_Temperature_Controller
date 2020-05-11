
#ifndef _BSP_BEEP_H
#define _BSP_BEEP_H

#ifdef HAVE_BEEP

#include "bsp_gpio.h"

/*! \brief
*       beep load configure
* \param handler[IN]                - beep handler
*/
void BSP_BEEP_LoadConfig(PortTypeDef *handler);

/*! \brief
*       beep initilization
* \param handler[IN]                - beep handler
*/
void BSP_BEEP_Init(void);

/*! \brief
*       beep set on
*/
void BSP_BEEP_On(void);

/*! \brief
*       beep set off
*/
void BSP_BEEP_Off(void);

#endif

#endif