

#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#ifdef HAVE_EXTI

/*! \brief exti confirgure Definition */
typedef struct _EXTIConfig{
    PortTypeDef gpio;       /*!< \brief exti port */
    u8          trigger;    /*!< \brief exti trigger mode */
}EXTIConfig;

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \param cnt[IN]            - extern interrupt count
* \note
*      1.exti port confirure array must be ended with "NULL".
*      ex.
*           __far PortTypeDef exti_handler[4] = {
*               {PORTA, PIN0},      // EXTI 1
*               {PORTA, PIN1},      // EXTI 2
*               {PORTA, PIN2},      // EXTI 3
*            };
*/
void BSP_EXTI_LoadConfig(const EXTIConfig *handler, u8 cnt);

/*! \brief
*      exti initilization
*/
void BSP_EXTI_Init(void);

/*! \brief
*      extern interrupt pin state
* \param idx[IN]        extern interrupt pin array index
* \return
*      0    - low level
*      1    - high level
*/
u8  BSP_EXTI_ReadState(u8 idx);

#endif

#endif