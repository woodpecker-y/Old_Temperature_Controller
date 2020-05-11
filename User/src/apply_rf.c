/*    调用流程:
*       |-->rf_load_config()
*           |-->rf_power_on()
*               |-->rf_init()
*               |-->rf_reinit()
*                   |-->rf_snd_data()
*                   |-->rf_rcv_init()
*                   |-->rf_rcv_data()
*               |-->rf_close()
*           |-->rf_power_off()
*       此处为外部中断，具体选择哪一个外部中断函数与管脚有关，一般PIN5，外部中断即为EXTI5；PIN4，外部中断即为EXTI4
*       |-->INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
*           |-->rf_isr()
******************************************************************************/

#ifdef HAVE_RF_BSP

#include "apply_rf.h"
#include "bsp.h"

/*! \brief
*       rf module load config
* \param rf_cfg[IN]        - rf config poninter
* \return
*       none
*/
void rf_load_config(RFConfig *rf_cfg)
{
    BSP_RF_LoadConfig(rf_cfg);
}

/*! \brief
*       rf module power on
* \return
*       none
*/
void rf_power_on(void)
{
    BSP_RF_PowerOn();
}

/*! \brief
*       rf module power on
* \return
*       none
*/
void rf_power_on_opendrain(void)
{
    BSP_RF_PowerOn_OpenDrain();
}


/*! \brief
*       rf module power off
* \return
*       none
*/
void rf_power_off(void)
{
    // 关闭端口
    BSP_RF_PortClose();    
    BSP_RF_PowerOff();
}

/*! \brief
*       rf module power off
* \return
*       none
*/
void rf_power_off_opendrain(void)
{
    // 关闭端口
    BSP_RF_PortClose();    
    BSP_RF_PowerOff_OpenDrain();
}

/*! \brief
*       rf module init
* \param fre[IN]           - rf communication frequency
* \param rate[IN]          - rf communication rate
* \param rf_pow[IN]        - rf power
* \param ch[IN]            - rf communication channel
* \return
*       E_RF_OK         - successful
*       others          - failed
*/
u8  rf_init(RF_FRE fre, RF_DATA_RATE rate, RF_TXPOW rf_pow, u8 ch)
{
    return BSP_RF_Init(fre, rate, rf_pow, ch);
}

/*! \brief
*       rf module close
* \return
*       none
*/
void rf_close(void)
{
    BSP_RF_Close();
}

/*! \brief
*       rf module reinit
* \param fre[IN]           - rf communication frequency
* \param rate[IN]          - rf communication rate
* \param rf_pow[IN]        - rf power
* \param ch[IN]            - rf communication channel
* \return
*       E_RF_OK         - successful
*       others          - failed
*/
u8  rf_reinit(RF_FRE fre, RF_DATA_RATE rate, RF_TXPOW rf_pow, u8 ch)
{
    return BSP_RF_ReInit(fre, rate, rf_pow, ch);
}

/*! \brief
*       rf get rssi
* \return
*       E_RF_OK         - successful
*       others          - failed
* \note
*       not use.
*/
u8 rf_get_rssi(void)
{
    return 0;
}

/*! \brief
*       rf module send data
* \param dat[IN]           - rf send data 
* \param len[IN]           - the length of rf send data
* \return
*       E_RF_OK         - successful
*/
u8 rf_snd_data(u8 *dat, u8  len)
{
    BSP_RF_SendData(dat, len);
    
    return 0;
}

/*! \brief
*       rf module recv init
* \return
*       none
*/
void rf_rcv_init(void)
{
    BSP_RF_RecvInit();
}

/*! \brief
*       rf module recv data
* \param dat[OUT]           - rf recv data
* \param len[OUT]           - the length of rf recv data
* \return
*       E_RF_OK         - successful
* \note
*       此处dat为二级指针，不需要分配内存，典型用法：
*           uint8_t *rcv_ptr = NULL;
*	    uint16_t rcv_len = 0;
*			
*	    rf_rcv_data(&rcv_ptr, &rcv_len);         
*/
u8 rf_rcv_data(u8 **dat, u8 *len)
{
    return BSP_RF_RecvData(dat, len);
}

/*! \brief
*       rf module set rf channel
* \param ch[IN]           - rf recv data
* \return
*       none         
*/
void rf_set_channel(u8 ch)
{
    BSP_RF_SetFreChannel(ch);
}

/*! \brief
*       rf module set rf frequency
* \param fre[IN]           - rf communication frequency
* \return
*       none         
*/
void rf_set_fre(u8 fre)
{
    BSP_RF_SetFre((RF_FRE)fre);
}

/*! \brief
*       rf module enter into rx mode
* \return
*       none         
*/
void rf_set_rxmode(void)
{
    BSP_RF_SetIdleMode();
    BSP_RF_ResetRxFIFO();  
    BSP_RF_SetRxMode();
}

/*! \brief
*       rf module get state
* \return
*       E_RF_MODE_IDLE  - 空闲状态
*       E_RF_MODE_SEND  - 发送状态
*       E_RF_MODE_RECV  - 接收状态
*/
u8 rf_get_state(void)
{
    return BSP_RF_GetState();
}

/*! \brief
*       rf module interrupt service routines
* \return
*       E_RF_MODE_IDLE  - 空闲状态
*       E_RF_MODE_SEND  - 发送状态
*       E_RF_MODE_RECV  - 接收状态
*/
void rf_isr(void)
{
    BSP_RF_ISR();
}

/*! \brief
*       rf module query recv state
* \return
*       E_RF_RX_FLAG_IDLE           - 空闲状态
*       E_RF_RX_FLAG_RECVING        - 正在接收
*       E_RF_RX_FLAG_RECV_COMPLETE  - 接收完成
*       E_RF_RX_FLAG_PROCESSING     - 正在处理    
*/
u8 rf_get_recv_flag(void)
{
    return BSP_RF_GetRecvFlag();
}

#endif
