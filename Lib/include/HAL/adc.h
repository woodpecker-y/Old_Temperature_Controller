/*    调用流程:
*       |-->BSP_ADC_Init()
*           |-->BSP_ADC_SampleInit()
*               |-->BSP_ADC_SampleOnce()
*               |-->...
*               |-->BSP_ADC_SampleOnce()
*           |-->BSP_ADC_SampleFinish()
*       |-->BSP_ADC_Close()
******************************************************************************/
#ifndef _ADC_H
#define _ADC_H

#ifdef HAVE_ADC

#include"stm8l15x.h"
#include "bsp_adc.h"

/*! \brief
*       Load ADC Sample port configure
* \param handler[IN]        - adc sample configure handler
*/
void adc_load_config(ADCConfig *handler, u8 num);

/*! \brief
*       adc sample all pin initilization
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void adc_init(u8 idx);

/*! \brief
*       adc sample all pin close
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void adc_close(u8 idx);

/*! \brief
*       adc sample initilization
* \param adc_channel[IN]        - adc samle channel
* \return
*       none
*/
void adc_sample_init(u8 idx);

/*! \brief
*       adc sample once
* \return
*       adc sample value
* \note
*       1、必须调用BSP_ADC_SampleInit之后才可以使用；
*       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
*/
u32 adc_sample_once(u8 idx);

/*! \brief
*       adc sample finish
* \param adc_channel[IN]        - adc sample channel
*/
void adc_sample_finish(u8 idx);

#endif

#endif

