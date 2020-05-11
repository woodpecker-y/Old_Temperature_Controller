/*    调用流程:
*       |-->BSP_ADC_LoadConfig()
*           |-->BSP_ADC_PowerOn() or BSP_ADC_PowerOn_OpenDrain()
*               |-->BSP_ADC_Init() or BSP_ADC_Init_Vrefint()
*                   |-->BSP_ADC_SampleOnce() or BSP_ADC_SampleOnceVrefint
*               |-->BSP_ADC_Close() or BSP_ADC_CloseVrefint()
*           |-->BSP_ADC_PowerOff() or BSP_ADC_PowerOff_OpenDrain
******************************************************************************/
#include "bsp.h"
#include "adc_bsp.h"
#include "bsp_adc_bsp.h"

#ifdef HAVE_ADC_BSP1

/*! \brief
 *       Load ADC Sample port configure
 * \param handler[IN]        - adc sample configure handler
 */
//void adc_load_config1(ADCConfig1 *handler, u8 num)
//{
//    BSP_ADC_LoadConfig1(handler, num);
//}

/*! \brief
 *       adc sample all pin initilization
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void adc_power_on1(u8 idx)
{
    BSP_ADC_PowerOn1(idx);
}

/*! \brief
 *       adc sample all pin initilization
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void adc_power_on_opendrain1(u8 idx)
{
    BSP_ADC_PowerOn_OpenDrain1(idx);
}

/*! \brief
 *       adc sample all pin close
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void adc_power_off1(u8 idx)
{
    BSP_ADC_PowerOff1(idx);
}

/*! \brief
 *       adc sample all pin close
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void adc_power_off_opendrain1(u8 idx)
{
    BSP_ADC_PowerOff_OpenDrain1(idx);
}

/*! \brief
 *       adc sample initilization
 * \param adc_channel[IN]        - adc samle channel
 * \return
 *       none
 */
int adc_init1(u8 idx)
{
    return BSP_ADC_Init1(idx);
}

/*! \brief
 *       adc sample once
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample_once1(u8 idx)
{
    return BSP_ADC_SampleOnce1(idx);
}

/*! \brief
 *       adc sample 8 cycle,get average
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample1(u8 idx)
{
    int i = 0;
    u32 result = 0;

    for (i=0; i<8; ++i)
    {
        result += BSP_ADC_SampleOnce1(idx);
    }
    return result/8;
}

/*! \brief
 *       adc sample finish
 * \param adc_channel[IN]        采样通道
 */
void adc_close1(u8 idx)
{
    BSP_ADC_Close1(idx);
}

/*! \brief
 *       adc sample initilization with VREFINT Channel
 * \return
 *       0  - successful
 */
int adc_init_vrefint1(void)
{
    return BSP_ADC_Init_Vrefint1();
}

/*! \brief
 *       对VREFINT通道进行一次AD采样
 * \return
 *       AD采样值
 * \note
 *       1、必须调用BSP_ADC_Init_Vrefint之后才可以使用；
 *       2、BSP_ADC_Init_Vrefint初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample_once_vrefint1(void)
{
    return BSP_ADC_SampleOnceVrefint1();
}

/*! \brief
 *       adc sample 8 cycle,get average
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample_vrefint1(void)
{
    int i = 0;
    u32 result = 0;

    for (i=0; i<8; ++i)
    {
        result += adc_sample_once_vrefint1();
    }

    return result/8;
}

/*! \brief
 *       AD采样结束
 * \param adc_channel[IN]        采样通道
 */
void adc_close_vrefint1(void)
{
    BSP_ADC_CloseVrefint1();
}

/*! \brief
 *       adc sample initilization single
 * \param adc_idx[IN]           - adc idx:E_ADC1
 * \param port[IN]              - adc idx:ADC Sample Port
 * \param pin[IN]               - adc idx:ADC Sample Pin
 * \return
 *       none
 */
int adc_init_single1(u8 adc_idx, u8 port, u8 pin)
{
    return BSP_ADC_Init_Single1(adc_idx, port, pin);
}

/*! \brief
 *       adc sample once single
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 adc_sample_once_single1(u8 adc_idx)
{
    return BSP_ADC_SampleOnce_Single1(adc_idx);
}

/*! \brief
 *       adc sample finish single
 * \param adc_channel[IN]        - adc sample channel
 */
void adc_close_single1(u8 adc_idx, u8 port, u8 pin)
{
    BSP_ADC_Close_Single1(adc_idx, port, pin);
}

int  		AdcRefVoltage;    						//参考电压值
#define  	VoltageRefInt   	1224L  				//内部系统电压值

/******************************************
function :ADC 初始化  
          引脚端口 等

******************************************/
void  ADC_BatteryInit(void)
{ 

//内部参数方式 管脚不使用
  GPIO_Init(ADC_BatteryPort,ADC_BatteryPin, GPIO_Mode_In_FL_No_IT);
  
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
  /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  

  /* Enable End of conversion ADC1 Interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);

  /* Start ADC1 Conversion using Software trigger*/
  ADC_SoftwareStartConv(ADC1);
//  ADC_BAT_Ctrler.Ticks =&SystemTick_ms;
  
}

/******************************************
function:  关闭adc  ADC反向初始化
******************************************/
void  ADC_BatteryUninit(void)
{
	ADC_Cmd(ADC1,DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
}

/************************************************
function : 得到电池电量

************************************************/
void  GetBatteryValue(void)
{
	int  ADC_REF_INT=0;
	char   i=10; 
	int ADCdata=0;
	
	/////////转换ref////////       
	/* Enable ADC1 Channel ADC_Channel_Vrefint*/   
	ADC_VrefintCmd(ENABLE);//TSON  
	delay_us(5);
	ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);   
	ADC_SoftwareStartConv(ADC1);  

	int Loops=0XFFF;
	/* Wait until End-Of-Convertion */  
	while ( 0 ==ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))  
	{
		Loops-=1;
		if(0 ==Loops)
		{
			break;
		}
	}
	/* Get ADC convertion value  */  
	ADC_REF_INT= ADC_GetConversionValue(ADC1); //得到内部参考电压参考值
	
	//TSON复位 
	ADC_VrefintCmd(DISABLE);
	ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint,DISABLE);//关闭通道

	
	///////转换电池电压////////  
	ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE);
	
	while(i--)
	{
		/* Enable ADC1 Channel 21*/
		ADC_SoftwareStartConv(ADC1);
		/* Wait until End-Of-Convertion */ 
		Loops=0XFFF;
		while (RESET ==ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))  
		{
			Loops-=1;
			if( 0==Loops)
			{
				break;
			}
		}
		/* Get ADC convertion value */  
		ADCdata+= ADC_GetConversionValue(ADC1); 
		
	}
	ADCdata/=10;//求平均值
        printf("ADCdata_befor = %u\r\n", ADCdata);
	//关闭通道
	ADC_ChannelCmd(ADC1, ADC_Channel_24,DISABLE);
    ADCdata=(VoltageRefInt*ADCdata)/(int)ADC_REF_INT; 	//电池电压
    printf("ADCdata = %u\r\n", ADCdata);
}

#endif
