/*    调用流程:
*       |-->BSP_ADC_Init()
*           |-->BSP_ADC_SampleInit()
*               |-->BSP_ADC_SampleOnce()
*               |-->...
*               |-->BSP_ADC_SampleOnce()
*           |-->BSP_ADC_SampleFinish()
*       |-->BSP_ADC_Close()
******************************************************************************/
#ifndef _ADC_BSP_H
#define _ADC_BSP_H

#include "bsp.h"
#include "bsp_adc_bsp.h"

#ifdef HAVE_ADC_BSP1

/*! \brief
*       Load ADC Sample port configure
* \param handler[IN]        - adc sample configure handler
*/
//void adc_load_config1(ADCConfig1 *handler, u8 num);

/*! \brief
*       adc sample all pin initilization
* \param idx[IN]        - adc sample configure index which is in the handler array.
* \note
*       ADC电源关闭后立即上电可能会出现无法驱动的问题，导致ADC采样异常，已出现此情况，需要延时50ms
*/
void adc_power_on1(u8 idx);
void adc_power_on_opendrain1(u8 idx);

/*! \brief
*       adc sample all pin close
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void adc_power_off1(u8 idx);
void adc_power_off_opendrain1(u8 idx);

/*! \brief
*       adc sample initilization
* \param adc_channel[IN]        - adc samle channel
* \return
*       none
*/
int adc_init1(u8 idx);
/*! \brief
 *       adc sample initilization with VREFINT Channel
 * \return
 *       0  - successful
 */
int adc_init_vrefint1(void);

/*! \brief
*       adc sample once
* \return
*       adc sample value
* \note
*       1、必须调用BSP_ADC_SampleInit之后才可以使用；
*       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
*/
u32 adc_sample_once1(u8 idx);

/*! \brief
 *       adc sample 8 cycle,get average
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample1(u8 idx);

/*! \brief
*       对VREFINT通道进行一次AD采样
* \return
*       AD采样值
* \note
*       1、必须调用BSP_ADC_Init_Vrefint之后才可以使用；
*       2、BSP_ADC_Init_Vrefint初始化完毕后必须延时至少25us才能获取正确值；
*/
u32 adc_sample_once_vrefint1(void);

/*! \brief
 *       adc sample 8 cycle,get average
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample_vrefint1(void);

/*! \brief
*       adc sample finish
* \param adc_channel[IN]        - adc sample channel
*/
void adc_close1(u8 idx);

/*! \brief
*       AD采样结束
* \param adc_channel[IN]        采样通道
*/
void adc_close_vrefint1(void);

/*! \brief
 *       adc sample initilization single
 * \param adc_idx[IN]           - adc idx:E_ADC1
 * \param port[IN]              - adc idx:ADC Sample Port
 * \param pin[IN]               - adc idx:ADC Sample Pin
 * \return
 *       none
 */
int adc_init_single1(u8 adc_idx, u8 port, u8 pin);

/*! \brief
 *       adc sample once single
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample_once_single1(u8 adc_idx);

/*! \brief
 *       adc sample finish single
 * \param adc_channel[IN]        - adc sample channel
 */
void adc_close_single1(u8 adc_idx, u8 port, u8 pin);
   
#define	ADC_BatteryPort		GPIOF
#define	ADC_BatteryPin		GPIO_Pin_0

#define	AD_Valve_VOL		2600			//电池极限电压设置值 单位毫伏



/**
  * @}
  */

/*-------------------------------------外部调用------------------------------------*/
 
#ifdef _ADC_C_

#endif

typedef struct
{
	int 	ADCdata;				//转换值
	int* 	Ticks;					//时钟记计数器
}ADC_BAT_CtrlStru;



extern ADC_BAT_CtrlStru ADC_BAT_Ctrler;

extern void ADC_BatteryInit(void);
extern void GetBatteryValue(void);
extern void ADC_BatteryUninit(void); 
extern void AD1_Valve_Det_INT(void);  

#endif

#endif
