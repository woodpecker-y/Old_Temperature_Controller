
#ifndef _BEEP_H
#define _BEEP_H

#ifdef HAVE_BEEP

/*! \brief
*       beep load configure
* \param handler[IN]        - beep port configure handler
*/
void beep_load_config(PortTypeDef *handler);

/*! \brief
*       beep initilization
*/
void beep_init(void);

/*! \brief
*       beep on
*/
void beep_on(void);

/*! \brief
*       beep off
*/
void beep_off(void);

#endif

#endif

