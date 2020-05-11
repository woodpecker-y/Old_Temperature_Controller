/*    调用流程:
*       |-->wdg_init()
*           |-->wdg_reset()
*           |-->wdg_reset()
*           |-->wdg_reset()
******************************************************************************/

#ifndef _WDG_H
#define _WDG_H

#ifdef HAVE_WDG

/*! \brief
*       内部看门狗初始化
* \param _1s[IN]			- 看门狗溢出时间
*/
void wdg_init(u32 _1ms);

/*! \brief
*       内部看门狗喂狗
*/
void wdg_reset(void);

#endif

#endif
