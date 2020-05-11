
#ifndef _BSP_WDG_H
#define _BSP_WDG_H

#ifdef HAVE_WDG

#include "stm8l15x.h"

/*! \brief
*       watch dog initilization
* \param init[IN]        watch dog reset time.
*/
void BSP_WDG_Init(u32 init);

/*! \brief
*       watch dog feed
*/
void BSP_WDG_Reset(void);

#endif

#endif