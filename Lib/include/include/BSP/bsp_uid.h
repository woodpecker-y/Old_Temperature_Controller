
#ifndef _BSP_UID_H
#define _BSP_UID_H

#ifdef HAVE_UID

#include "stm8l15x.h"

/*! \brief
*       watch dog initilization
* \param init[IN]        watch dog reset time.
*/
void BSP_UID_Read(u8 *uid);

#endif

#endif