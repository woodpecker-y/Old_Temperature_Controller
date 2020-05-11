#ifdef HAVE_RF_BSP

#include "stm8l15x.h"
#include "bsp_si4432.h"
#include "delay.h"
//#include "dbg.h"

//u8 rf_buf[80] = {0};
//u8 rf_len  = 0;
//u8 rf_flag = 0;
//u8 rf_sts  = 0;

typedef struct _RFHanddler{
    u8*     dat;
    u16     len;
    u8      flag;
    u8      mode;
}RFHandler;

/*! \brief rfid configure handler pointer */
static RFConfig  *s_rf_cfg_handler;
static RFHandler  s_rf_handler;

/*! \brief 
*      RF Configuartion Init
* 
* \param rf_cfg[IN]           - rf config pointer.
* \note  
* 
*/
void BSP_RF_LoadConfig(RFConfig *rf_cfg)
{
    u16 i = 0;
    
    s_rf_cfg_handler = rf_cfg;
    s_rf_handler.dat = rf_cfg->rcv_ptr;
    s_rf_handler.len = 0;
    
    for (i=0; i<rf_cfg->max_rcv_size; ++i)
    {
        s_rf_handler.dat[i] = 0;
    }
}

void BSP_RF_MOSI_H()
{
    
}

void BSP_RF_SCK_SetHigh(void)
{
    BSP_GPIO_SetHigh(s_rf_cfg_handler->sck.port, s_rf_cfg_handler->sck.pin);
}

void BSP_RF_SCK_SetLow(void)
{
    BSP_GPIO_SetLow(s_rf_cfg_handler->sck.port, s_rf_cfg_handler->sck.pin);
}

void BSP_RF_MOSI_SetHigh(void)
{
    BSP_GPIO_SetHigh(s_rf_cfg_handler->mosi.port, s_rf_cfg_handler->mosi.pin);
}

void BSP_RF_MOSI_SetLow(void)
{
    BSP_GPIO_SetLow(s_rf_cfg_handler->mosi.port, s_rf_cfg_handler->mosi.pin);
}

void BSP_RF_NSS_SetHigh(void)
{
    BSP_GPIO_SetHigh(s_rf_cfg_handler->nss.port, s_rf_cfg_handler->nss.pin);
}

void BSP_RF_NSS_SetLow(void)
{
    BSP_GPIO_SetLow(s_rf_cfg_handler->nss.port, s_rf_cfg_handler->nss.pin);
}

u8  BSP_RF_MISO_ReadState(void)
{
    return BSP_GPIO_ReadState(s_rf_cfg_handler->miso.port, s_rf_cfg_handler->miso.pin);
}

/*! \brief 
*      RF管脚初始化
* 
* \note  
* 
*/
void BSP_RF_PortInit(void)
{
    // SDN输出为高
    BSP_GPIO_Init(s_rf_cfg_handler->sdn.port, s_rf_cfg_handler->sdn.pin, GPIO_Mode_Out_PP_High_Fast);
    
    // MOSI、SCK、NSS输出为低
    BSP_GPIO_Init(s_rf_cfg_handler->mosi.port, s_rf_cfg_handler->mosi.pin, GPIO_Mode_Out_PP_Low_Fast);
    BSP_GPIO_Init(s_rf_cfg_handler->sck.port, s_rf_cfg_handler->sck.pin, GPIO_Mode_Out_PP_Low_Fast);
    BSP_GPIO_Init(s_rf_cfg_handler->nss.port, s_rf_cfg_handler->nss.pin, GPIO_Mode_Out_PP_Low_Fast);
    
    // MISO浮空输入
    BSP_GPIO_Init(s_rf_cfg_handler->miso.port, s_rf_cfg_handler->miso.pin, GPIO_Mode_In_PU_No_IT);
    
    // NIRQ带中断的浮空输入
    BSP_GPIO_Init(s_rf_cfg_handler->nirq.port, s_rf_cfg_handler->nirq.pin, GPIO_Mode_In_FL_IT );
    EXTI_SetPinSensitivity(BSP_GPIO_GetEXTIPinNum(s_rf_cfg_handler->nirq.pin), EXTI_Trigger_Falling);
}

/*! \brief 
*      上电
* 
* \note  
* 
*/
void BSP_RF_PowerOn(void)
{
    BSP_GPIO_Init(s_rf_cfg_handler->pwr.port, s_rf_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_Low_Fast);
    //RF_PWR_L();
}

/*! \brief 
*      上电
* 
* \note  
* 
*/
void BSP_RF_PowerOn_OpenDrain(void)
{
    BSP_GPIO_Init(s_rf_cfg_handler->pwr.port, s_rf_cfg_handler->pwr.pin, GPIO_Mode_Out_OD_Low_Fast);
    //RF_PWR_L();
}

/*! \brief 
*      断电
* 
* \note  
* 
*/
void BSP_RF_PowerOff(void)
{
    BSP_GPIO_Init(s_rf_cfg_handler->pwr.port, s_rf_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_High_Fast);
    //RF_PWR_H();
}

/*! \brief 
*      断电
* 
* \note  
* 
*/
void BSP_RF_PowerOff_OpenDrain(void)
{
    BSP_GPIO_Init(s_rf_cfg_handler->pwr.port, s_rf_cfg_handler->pwr.pin, GPIO_Mode_Out_OD_HiZ_Fast);
    //RF_PWR_H();
}

void BSP_RF_WriteRegister(u8 u8Reg, u8 u8Value)
{
    u8	u8Index;
    
    u8Reg |= 0x80;
    
    //片选拉低
    //Si4432_CS_L;
    BSP_RF_NSS_SetLow();
    
    //写命令和起始地址
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        if((u8Reg << u8Index) & 0x80)
        {
            BSP_RF_MOSI_SetHigh();
            //Si4432_MOSI_H;
        }
        else
        {
            BSP_RF_MOSI_SetLow();
            //Si4432_MOSI_L;	
        }
        //Si4432_SCK_L;
        //Si4432_SCK_H;	
        BSP_RF_SCK_SetLow();
        BSP_RF_SCK_SetHigh();
    }
    //写命令和起始地址
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        if((u8Value << u8Index) & 0x80)
        {
            //Si4432_MOSI_H;
            BSP_RF_MOSI_SetHigh();
        }
        else
        {
            //Si4432_MOSI_L;	
            BSP_RF_MOSI_SetLow();
        }
        //Si4432_SCK_L;
        //Si4432_SCK_H;	
        BSP_RF_SCK_SetLow();
        BSP_RF_SCK_SetHigh();
    }
    //Si4432_CS_H;                             
    //Si4432_SCK_L;  
    BSP_RF_NSS_SetHigh();
    BSP_RF_SCK_SetLow();
}

/*******************************************************
* Name: BSP_RF_ReadRegister
* Description: 读寄存器
********************************************************/
u8 BSP_RF_ReadRegister(u8 u8Reg)
{
    u8	u8Index;
    u8 	u8Value;
    
    u8Reg &= 0x7F;
    //片选拉低
    //Si4432_CS_L;
    BSP_RF_NSS_SetLow();
    
    //写命令和起始地址
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        if((u8Reg << u8Index) & 0x80)
        {
            //Si4432_MOSI_H;
            BSP_RF_MOSI_SetHigh();
        }
        else
        {
            //Si4432_MOSI_L;	
            BSP_RF_MOSI_SetLow();
        }
        //Si4432_SCK_L;
        //Si4432_SCK_H;		
        BSP_RF_SCK_SetLow();
        BSP_RF_SCK_SetHigh();	
    }
    
    u8Value = 0;
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        //Si4432_SCK_L;
        //Si4432_SCK_H;	
        BSP_RF_SCK_SetLow();
        BSP_RF_SCK_SetHigh();        
        
        u8Value = u8Value << 1;
        //if(Si4432_MISO_READ)
        if (1 == BSP_RF_MISO_ReadState())
        {
            u8Value |= 0x01;		
        }
    }
    
    //Si4432_CS_H;                             
    //Si4432_SCK_L;
    BSP_RF_NSS_SetHigh();        
    BSP_RF_SCK_SetLow();        
    
    return u8Value;   
}

/*******************************************************
* Name: BSP_RF_Init
* Description: 无线初始
********************************************************/ 
u8 BSP_RF_Init(RF_FRE rf_fre, RF_DATA_RATE date_rate, RF_TXPOW rf_pow, u8 ch)
{      
    u8 dev_code = 0, dev_ver = 0;
    
    BSP_RF_PortInit(); 
    
    //Si4432_SDN_H;
    BSP_GPIO_SetHigh(s_rf_cfg_handler->sdn.port, s_rf_cfg_handler->sdn.pin);
    delay_ms(10);	// RF 模块复位
    
    // debug
    //Si4432_SDN_L;
    BSP_GPIO_SetLow(s_rf_cfg_handler->sdn.port, s_rf_cfg_handler->sdn.pin);
    delay_ms(200);	// 延时150ms RF 模块进入工作状态
    
    // 清中断
    BSP_RF_ReadRegister(0x03);		
    BSP_RF_ReadRegister(0x04);
    
    // 关闭中断
    BSP_RF_WriteRegister(0x06, 0x00);
    
    // 进入Ready  
    BSP_RF_WriteRegister(0x07, 0x01);
    
    // 设置负载电容
    BSP_RF_WriteRegister(0x09, 0x7F);	
    
    /*set the GPIO's according the testcard type*/
    BSP_RF_WriteRegister(0x0A, 0x05);															//Set GPIO0 output
    BSP_RF_WriteRegister(0x0B, 0xEA);															//Set GPIO0 output
    BSP_RF_WriteRegister(0x0C, 0xEA);															//Set GPIO1 output
    BSP_RF_WriteRegister(0x0D, 0xEA);															//Set GPIO2 output Rx Data	
    
    // 设置频率
    BSP_RF_SetFre(rf_fre);
    
    // 设置速率
    BSP_RF_SetDataRate(date_rate);
    
    // 设置功率
    BSP_RF_WriteRegister(0x6D, rf_pow);															
    
    // 设置跳频
    BSP_RF_WriteRegister(0x79, ch);     // 跳频通道1
    BSP_RF_WriteRegister(0x7a, 0x05);   // 跳频宽度50KHz 5*10KHz
    //BSP_RF_WriteRegister(0x7a, 0x0A);   // 跳频宽度100KHz 10*10KHz
    
    /*set the packet structure and the modulation type*/
    //set the preamble length to 10bytes if the antenna diversity is used and set to 5bytes if not 
    BSP_RF_WriteRegister(0x34, 0x0C);															//write data to the Preamble Length u8Register
    //set preamble detection threshold to 20bits
    BSP_RF_WriteRegister(0x35, 0x2A); 															//write data to the Preamble Detection Control  u8Register
    
    //Disable header bytes; set variable packet length (the length of the payload is defined by the
    //received packet length field of the packet); set the synch word to two bytes long
    BSP_RF_WriteRegister(0x33, 0x02);															//write data to the Header Control2 u8Register    
    
    //Set the sync word pattern to 0x2DD4
    BSP_RF_WriteRegister(0x36, 0x2D);															//write data to the Sync Word 3 u8Register
    BSP_RF_WriteRegister(0x37, 0xD4);															//write data to the Sync Word 2 u8Register
    
    //enable the TX & RX packet handler and CRC-16 (IBM) check
    BSP_RF_WriteRegister(0x30, 0x8C);															//write data to the Data Access Control u8Register
    //Disable the receive header filters
    BSP_RF_WriteRegister(0x32, 0x00 );												  			//write data to the Header Control1 u8Register            
    
    //enable FIFO mode and GFSK modulation
    //BSP_RF_WriteRegister(0x71, 0x63);															//write data to the Modulation Mode Control 2 u8Register
    BSP_RF_WriteRegister(0x71, 0x22);
    
    //设置RSSI门限150
    BSP_RF_WriteRegister(0x27, 0x96);
    
    BSP_RF_WriteRegister(0x0E, 0x00);
    
    dev_code = BSP_RF_ReadRegister(0x00);
    dev_ver  = BSP_RF_ReadRegister(0x01);
    
////////    printf("RF DevCode:%x\r\n", dev_code);      // 暂时关闭
////////    printf("RF DevVer :%x\r\n", dev_ver);   
    
    BSP_RF_SetRxMode();
    if (dev_code != 0x08 || dev_ver != 0x06)
        return E_RF_ERR_INIT;
    
    BSP_RF_RecvInit();
    
    return E_RF_OK;    
}

void BSP_RF_Close(void)
{
    BSP_RF_WriteRegister(0x07, 0x01);
    BSP_RF_WriteRegister(0x07, 0x00);    
    BSP_RF_WriteRegister(0x0A, 0x00);        
    BSP_RF_WriteRegister(0x05, 0x00);                   
    BSP_RF_WriteRegister(0x06, 0x00);                       
    BSP_RF_ReadRegister(0x03);      
    
    s_rf_handler.mode = E_RF_MODE_IDLE;
}

void BSP_RF_PortClose(void)
{
    // SDN输出为高
    //BSP_GPIO_Init(s_rf_cfg_handler->sdn.port, s_rf_cfg_handler->sdn.pin,      GPIO_Mode_Out_PP_High_Slow);
    BSP_GPIO_Init(s_rf_cfg_handler->sdn.port, s_rf_cfg_handler->sdn.pin,   GPIO_Mode_Out_PP_Low_Slow);
    // MOSI、SCK、NSS输出为低
    BSP_GPIO_Init(s_rf_cfg_handler->mosi.port, s_rf_cfg_handler->mosi.pin,   GPIO_Mode_Out_PP_Low_Slow);
    BSP_GPIO_Init(s_rf_cfg_handler->sck.port, s_rf_cfg_handler->sck.pin,    GPIO_Mode_Out_PP_Low_Slow);
    BSP_GPIO_Init(s_rf_cfg_handler->nss.port, s_rf_cfg_handler->nss.pin,    GPIO_Mode_Out_PP_Low_Slow);
    
    // MISO浮空输入
    BSP_GPIO_Init(s_rf_cfg_handler->miso.port, s_rf_cfg_handler->miso.pin,   GPIO_Mode_Out_PP_Low_Slow);
    
    // NIRQ带中断的浮空输入
    BSP_GPIO_Init(s_rf_cfg_handler->nirq.port, s_rf_cfg_handler->nirq.pin,   GPIO_Mode_Out_PP_Low_Slow);
    //BSP_GPIO_Init(s_rf_cfg_handler->nirq.port, s_rf_cfg_handler->nirq.pin,       GPIO_Mode_In_PU_IT);
    //EXTI_SetPinSensitivity(BSP_GPIO_GetEXTIPinNum(s_rf_cfg_handler->nirq.pin), EXTI_Trigger_Falling);
    
    // GPIO2未使用，保持低功耗状态 
    //BSP_GPIO_Init(RF_PORT_GPIO2, RF_PIN_GPIO2,      GPIO_Mode_Out_PP_Low_Slow);    
}

u8 BSP_RF_ReInit(RF_FRE rf_fre, RF_DATA_RATE date_rate, RF_TXPOW rf_pow, u8 ch)
{
    u8 rv;
    
    // 关闭端口
    BSP_RF_PortClose();
    
    // 断电
    BSP_RF_PowerOff();
    
    // 上电
    BSP_RF_PowerOn();       
    
    // RF初始化
    rv = BSP_RF_Init(rf_fre, date_rate, rf_pow, ch);
    if (rv != 0)
    {   
        // 关闭无线电源
        BSP_RF_PowerOff();
        
        return E_RF_ERR_INIT;
    }  
    
    return E_RF_OK;
}

void RF_SetFreChannel(u8 ch)
{  
    BSP_RF_WriteRegister(0x79, ch);   
}

/*******************************************************
* Name: RF_SetTxpow
* Description: 设置输出功率
********************************************************/
void RF_SetTxpow(RF_TXPOW txpow)
{
    BSP_RF_WriteRegister(0x6D, txpow);
}

/*******************************************************
* Name: RF_SetFre
* Description: 设置频率
********************************************************/
/*******************************************************
* Name: RF_SetFre
* Description: 设置频率
********************************************************/
void BSP_RF_SetFre(RF_FRE fre)
{
    switch (fre)
    {
    case RF_FRE_410_5M:
        BSP_RF_WriteRegister(0x75, 0x51);
        BSP_RF_WriteRegister(0x76, 0x0C);
        BSP_RF_WriteRegister(0x77, 0x80);
        break;
    case RF_FRE_420_5M:
        BSP_RF_WriteRegister(0x75, 0x52);
        BSP_RF_WriteRegister(0x76, 0x0c);
        BSP_RF_WriteRegister(0x77, 0x80);
        break;
    case RF_FRE_430_5M:
        BSP_RF_WriteRegister(0x75, 0x53);
        BSP_RF_WriteRegister(0x76, 0x0C);
        BSP_RF_WriteRegister(0x77, 0x80);
        break;
    case RF_FRE_440_5M:
        BSP_RF_WriteRegister(0x75, 0x54);
        BSP_RF_WriteRegister(0x76, 0x0c);
        BSP_RF_WriteRegister(0x77, 0x80);
        break;
    case RF_FRE_470_5M:
        BSP_RF_WriteRegister(0x75, 0x57);
        BSP_RF_WriteRegister(0x76, 0x0C);
        BSP_RF_WriteRegister(0x77, 0x80);
        break;
    case RF_FRE_480_5M:
        BSP_RF_WriteRegister(0x75, 0x60);
        BSP_RF_WriteRegister(0x76, 0x06);
        BSP_RF_WriteRegister(0x77, 0x40);
        break;
    case RF_FRE_490_5M:
        BSP_RF_WriteRegister(0x75, 0x60);
        BSP_RF_WriteRegister(0x76, 0x83);
        BSP_RF_WriteRegister(0x77, 0x40);
        break;
    case RF_FRE_500_5M:
        BSP_RF_WriteRegister(0x75, 0x61);
        BSP_RF_WriteRegister(0x76, 0x06);
        BSP_RF_WriteRegister(0x77, 0x40);
        break;
    case RF_FRE_510_5M:
        BSP_RF_WriteRegister(0x75, 0x61);
        BSP_RF_WriteRegister(0x76, 0x83);
        BSP_RF_WriteRegister(0x77, 0x40);
        break;
    case RF_FRE_520_5M:
        BSP_RF_WriteRegister(0x75, 0x62);
        BSP_RF_WriteRegister(0x76, 0x06);
        BSP_RF_WriteRegister(0x77, 0x40);        
        break;
    default:
        return;	
    }
}


/*******************************************************
* Name: RF_SetFre
* Description: 波特率设置
********************************************************/
void BSP_RF_SetDataRate(RF_DATA_RATE datarate)
{
    switch(datarate)
    {
    case RF_DATA_RATE_1200:  	//1.2kbps
        /*set the modem parameters according to the exel calculator(parameters: 1.2 kbps, deviation: 20 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x16);															//write data to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x83);															//write data to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0xC0);															//write data to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x13);															//write data to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0xA9);															//write data to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x00);															//write data to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x04);															//write data to the Clock Recovery Timing Loop Gain 0 register	
        //set the Tx deviation register (+-20kHz)
        BSP_RF_WriteRegister(0x72, 0x20);															//write data to the Frequency Deviation register 
        //set the desired TX data rate (1.2kbps)
        BSP_RF_WriteRegister(0x6E, 0x09);															//write data to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0xD5);															//write data to the TXDataRate 0 register
        //Set bit5 to 1 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x2C);	
        
        //BSP_RF_WriteRegister(0x58, 0x80);
        BSP_RF_WriteRegister(0x1D, 0x40);															//write data to the AFC Loop Gearshift Override register		
        //BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x14);															//write data to the AFC Limiter register		
        //BSP_RF_WriteRegister(0x1F, 0x03);
        //BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_2400:  //2.4kbps
        //set the desired TX data rate (2.4kbps)
        BSP_RF_WriteRegister(0x6E, 0x13);															//write data to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0xA9);															//write data to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0x80);
        //set the Tx deviation register (+-20kHz)
        BSP_RF_WriteRegister(0x72, 0x20);															//write data to the Frequency Deviation register 
        
        //Set bit5 to 1 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x2C);															//write data to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 1.2 kbps, deviation: 20 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x2C);															//write data to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0xA1);															//write data to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x20);															//write data to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x4E);															//write data to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0xA5);															//write data to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x00);															//write data to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x0B);															//write data to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write data to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x0F);															//write data to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_4800:  //4.8kbps
        //set the desired TX data rate (4.8kbps)
        BSP_RF_WriteRegister(0x6E, 0x27);															//write data to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0x52);															//write data to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0x80);
        //set the Tx deviation register (+-20kHz)
        BSP_RF_WriteRegister(0x72, 0x20);
        
        //Set bit5 to 1 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x2C);															//write data to the Modulation Mode Control 1 register
        //write data to the Frequency Deviation register 
        
        /*set the modem parameters according to the exel calculator(parameters: 1.2 kbps, deviation: 20 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x2D);															//write data to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0xD0);															//write data to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x00);															//write data to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x9D);															//write data to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x49);															//write data to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x00);															//write data to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x28);															//write data to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write data to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x0F);															//write data to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_9600:  //9.6kbps
        //set the desired TX data rate (9.6kbps)
        BSP_RF_WriteRegister(0x6E, 0x4E);															//write 0x4E to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0xA5);															//write 0xA5 to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0x80);
        //set the Tx deviation register (+-20kHz)
        BSP_RF_WriteRegister(0x72, 0x20);															//write 0x20 to the Frequency Deviation register 
        
        //Set bit5 to 1 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x2C);															//write 0x2C to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 9.6 kbps, deviation: 20 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x2E);															//write 0x2E to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x68);															//write 0x68 to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x01);															//write 0x01 to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x3A);															//write 0x3A to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x93);															//write 0x93 to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x00);															//write 0x00 to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x99);															//write 0x99 to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x10);															//write 0x10 to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_19200: //19.2kbps
        //set the desired TX data rate (19.2kbps)
        BSP_RF_WriteRegister(0x6E, 0x9D);															//write 0x9D to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0x49);															//write 0x49 to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0x80);
        //set the Tx deviation register (+-20kHz)
        BSP_RF_WriteRegister(0x72, 0x20);															//write 0x20 to the Frequency Deviation register 
        
        //Set bit5 to 1 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x2C);															//write 0x2C to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 19.2 kbps, deviation: 20 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x16);															//write 0x16 to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x68);															//write 0x68 to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x01);															//write 0x01 to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x3A);															//write 0x3A to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x93);															//write 0x93 to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x02);															//write 0x02 to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x5F);															//write 0x5F to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x14);															//write 0x14 to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_38400: //38.4kbps
        //set the desired TX data rate (38.4kbps)
        BSP_RF_WriteRegister(0x6E, 0x09);															//write 0x09 to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0xD5);															//write 0xD5 to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0x80);
        //set the Tx deviation register (+-20kHz)
        BSP_RF_WriteRegister(0x72, 0x20);															//write 0x20 to the Frequency Deviation register 
        
        //Set bit5 to 0 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x0C);															//write 0x0C to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 38.4 kbps, deviation: 20 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x02);															//write 0x02 to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x68);															//write 0x68 to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x01);															//write 0x01 to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x3A);															//write 0x3A to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x93);															//write 0x93 to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x04);															//write 0x04 to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0xBC);															//write 0xBC to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x1E);															//write 0x1E to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_76800: //76.8kbps
        //set the desired TX data rate (76.8kbps)
        BSP_RF_WriteRegister(0x6E, 0x13);															//write 0x13 to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0xA9);															//write 0xA9 to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0x80);
        //set the Tx deviation register (+-40kHz)
        BSP_RF_WriteRegister(0x72, 0x40);															//write 0x40 to the Frequency Deviation register 
        
        //Set bit5 to 0 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x0C);															//write 0x0C to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 76.8 kbps, deviation: 40 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x95);															//write 0x95 to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x4E);															//write 0x4E to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x01);															//write 0x01 to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0xA3);															//write 0xA3 to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x6E);															//write 0x6E to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x06);															//write 0x06 to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x4F);															//write 0x4F to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x3C);															//write 0x3C to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_128000:  //128kbps
        //set the desired TX data rate (128kbps)
        BSP_RF_WriteRegister(0x6E, 0x20);															//write 0x20 to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0xC5);															//write 0xC5 to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0xC0);
        //set the Tx deviation register (+-64kHz)
        BSP_RF_WriteRegister(0x72, 0x66);															//write 0x66 to the Frequency Deviation register 
        
        //Set bit5 to 0 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x0C);															//write 0x0C to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 128 kbps, deviation: 64 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x83);															//write 0x83 to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x5E);															//write 0x5E to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x01);															//write 0x01 to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0x5D);															//write 0x5D to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x86);															//write 0x86 to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x05);															//write 0x05 to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0x74);															//write 0x74 to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x0A);
        BSP_RF_WriteRegister(0x2A, 0x50);															//write 0x0A to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    case RF_DATA_RATE_256000:  //256kbps
        //set the desired TX data rate (256kbps)
        BSP_RF_WriteRegister(0x6E, 0x41);															//write 0x41 to the TXDataRate 1 register
        BSP_RF_WriteRegister(0x6F, 0x89);															//write 0x89 to the TXDataRate 0 register
        BSP_RF_WriteRegister(0x58, 0xED);
        //set the Tx deviation register (+-128kHz)
        BSP_RF_WriteRegister(0x72, 0xCD);															//write 0xCD to the Frequency Deviation register 
        
        //Set bit5 to 0 because the datarate below 30kbps
        BSP_RF_WriteRegister(0x70, 0x0C);															//write 0x0C to the Modulation Mode Control 1 register
        
        /*set the modem parameters according to the exel calculator(parameters: 256 kbps, deviation: 128 kHz*/
        BSP_RF_WriteRegister(0x1C, 0x8C);															//write 0x8C to the IF Filter Bandwidth register		
        BSP_RF_WriteRegister(0x20, 0x2F);															//write 0x2F to the Clock Recovery Oversampling Ratio register		
        BSP_RF_WriteRegister(0x21, 0x02);															//write 0x02 to the Clock Recovery Offset 2 register		
        BSP_RF_WriteRegister(0x22, 0xBB);															//write 0xBB to the Clock Recovery Offset 1 register		
        BSP_RF_WriteRegister(0x23, 0x0D);															//write 0x0D to the Clock Recovery Offset 0 register		
        BSP_RF_WriteRegister(0x24, 0x07);															//write 0x07 to the Clock Recovery Timing Loop Gain 1 register		
        BSP_RF_WriteRegister(0x25, 0xFF);															//write 0xFF to the Clock Recovery Timing Loop Gain 0 register		
        BSP_RF_WriteRegister(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
        BSP_RF_WriteRegister(0x1E, 0x02);
        BSP_RF_WriteRegister(0x2A, 0x50);															//write 0x50 to the AFC Limiter register		
        BSP_RF_WriteRegister(0x1F, 0x03);
        BSP_RF_WriteRegister(0x69, 0x60);
        
        break;
    default:
        break;
    }
}

/*******************************************************
* Name: RFSetRxMode
* Description: 设置无线为接受模式
********************************************************/ 
void BSP_RF_SetRxMode(void)
{  
    //使能芯片内部的接收链路
    BSP_RF_WriteRegister(0x07, 0x05); 
    //设置接受中断
    BSP_RF_WriteRegister(0x05, 0x02);													//write 0x04 to the Interrupt Enable 1 register	
    //BSP_RF_WriteRegister(0x06, 0x80); 
    //读取中断寄存器，清除标示
    BSP_RF_ReadRegister(0x03);
    BSP_RF_ReadRegister(0x04);
    //设置GPIO1低，GPIO0高，关闭发射
    //BSP_RF_WriteRegister(0x0E, 0x02);    
    
    s_rf_handler.mode = E_RF_MODE_RECV;        
}

/*******************************************************
* Name: BSP_RF_SetTxMode
* Description: 设置无线为发射模式
********************************************************/
void BSP_RF_SetTxMode(void)
{
    //设置发送完成中断
    BSP_RF_WriteRegister(0x05, 0x04);													//write 0x04 to the Interrupt Enable 1 register	
    //BSP_RF_WriteRegister(0x06, 0x00);
    //读取中断寄存器，清除标示
    BSP_RF_ReadRegister(0x03);
    BSP_RF_ReadRegister(0x04); 
    //设置GPIO1高，GPIO0低，打开发射
    //BSP_RF_WriteRegister(0x0E, 0x01);    
    //使能芯片内部的发射链路
    BSP_RF_WriteRegister(0x07, 0x09);
}

/*******************************************************
* Name: BSP_RF_SetIdleMode
* Description: 设置无线为空闲模式
********************************************************/
void BSP_RF_SetIdleMode(void)
{
    //使能芯片休眠
    BSP_RF_WriteRegister(0x07, 0x01);
    ////设置GPIO1低，GPIO0高，关闭发射
    //   BSP_RF_WriteRegister(0x0E, 0x02);	
    //   //关闭所有中断
    //   BSP_RF_WriteRegister(0x05, 0x00);													//write 0x04 to the Interrupt Enable 1 register	
    //   BSP_RF_WriteRegister(0x06, 0x00);
    //   //读取中断寄存器，清除标示
    //   BSP_RF_ReadRegister(0x03);
    //   BSP_RF_ReadRegister(0x04); 
}

/*******************************************************
* Name: RF_GetRssi
* Description: 获得信号强度
********************************************************/
u8 BSP_RF_GetRssi(void)
{
    //获得信号强度
    return BSP_RF_ReadRegister(0x26);													//write 0x09 to the Operating Function Control 1 register
}

/*******************************************************
* Name: RF_ResetRxFIFO
* Description: 接受缓存复位
********************************************************/
void BSP_RF_ResetRxFIFO(void)
{
    // 设置天线为接收状态
    BSP_RF_WriteRegister(0x0E, 0x02);
    //delay_ms(5);
    
    BSP_RF_WriteRegister(0x08, 0x03);
    BSP_RF_WriteRegister(0x08, 0x00);
}

/*******************************************************
* Name: BSP_RF_ResetTxFIFO
* Description: 发送缓存复位
********************************************************/
void BSP_RF_ResetTxFIFO(void)
{
    // 设置天线为发送状态
    BSP_RF_WriteRegister(0x0E, 0x01);
    //delay_ms(5);
    
    BSP_RF_WriteRegister(0x08, 0x03);
    BSP_RF_WriteRegister(0x08, 0x00);
}

/*******************************************************
* Name: RF_GetDevStatus
* Description: 获取设备状态
********************************************************/
u8 BSP_RF_GetDevStatus(void)
{
    return BSP_RF_ReadRegister(0x02);
}

/*******************************************************
* Name: RF_GetITStatus1
* Description: 获取无线中断状态1
********************************************************/
u8 BSP_RF_GetITStatus1(void)
{
    return BSP_RF_ReadRegister(0x03);
}

/*******************************************************
* Name: RF_GetITStatus2
* Description: 获取无线中断状态2
********************************************************/
u8 BSP_RF_GetITStatus2(void)
{
    return BSP_RF_ReadRegister(0x04);
}

/*******************************************************
* Name: RF_SendData
* Description: 无线发送数据
********************************************************/
void BSP_RF_SendData(u8 *pu8DataBuff, u8 u16DataLen)
{           
    u8 u16Index;
    
    //设置为空闲
    BSP_RF_SetIdleMode();
    //复位缓存
    BSP_RF_ResetTxFIFO();
    
    //发送数据
    BSP_RF_WriteRegister(0x3E, u16DataLen); 
    for(u16Index = 0; u16Index < u16DataLen; u16Index++)
    {
        BSP_RF_WriteRegister(0x7F, pu8DataBuff[u16Index]);
    }
    
    //设置为发送模式
    BSP_RF_SetTxMode();	
    
    s_rf_handler.mode = E_RF_MODE_SEND;
}

void BSP_RF_ISR(void)
{
    u8 i = 0;
    u8 len = 0;
    u8 sts1=0, sts2=0;
    
    sts1 = BSP_RF_ReadRegister(0x03);										//read the Interrupt Status1 register
    sts2 = BSP_RF_ReadRegister(0x04);										//read the Interrupt Status2 register
    
    //MYLOG_DEBUG("RF sts[%02X], st1:%02X,st2:%02X\r\n", rf_sts, sts1, sts2);
    
    if (s_rf_handler.mode == E_RF_MODE_RECV)
    {
        if (sts1 & 0x02)
        {
            //printf("Recv OK!\r\n");
            
            len = BSP_RF_ReadRegister(0x4B);	
            if (len <= s_rf_cfg_handler->max_rcv_size)
            {
                for (i=0; i<len; ++i)
                {
                    s_rf_handler.dat[i] = BSP_RF_ReadRegister(0x7F);
                }
                
                s_rf_handler.len  = len;
                s_rf_handler.flag = E_RF_RX_FLAG_RECV_COMPLETE;
            }
            else
            {
                //printf("Length is more than buffer[%u]!\r\n", len);
            }
        }
        else if (sts2 & 0x80)
        {
            //printf("%u\r\n", BSP_RF_GetRssi());
            return;
        }
    }
    else if (s_rf_handler.mode == E_RF_MODE_SEND)
    {
        if (sts1 & 0x04)
        {
            //printf("Send OK!\r\n");
            
            s_rf_handler.mode = E_RF_MODE_RECV;
        }
    }
    else
    {
        //printf("YYYYY\r\n");
    }   
    
    BSP_RF_SetIdleMode();
    BSP_RF_ResetRxFIFO();  
    BSP_RF_SetRxMode();
    
    s_rf_handler.mode = E_RF_MODE_RECV;
}

void BSP_RF_RecvInit(void)
{
    u16 i = 0;
    
    s_rf_handler.flag = E_RF_RX_FLAG_RECVING;
    
    for (i=0; i<s_rf_cfg_handler->max_rcv_size; ++i)
    {
        s_rf_handler.dat[i] = 0;
    }    
}

u8  BSP_RF_RecvData(u8 **dat, u8 *len)
{
    //u8 i = 0;
    
    if (E_RF_RX_FLAG_RECV_COMPLETE == s_rf_handler.flag)
    {
        *dat = s_rf_handler.dat;
        *len = s_rf_handler.len;
        
//        char rev_data_201[41] = {0x3A, 0x70 , 0x98 , 0x10 , 0x05 , 0x18 , 0x00 , 0x00 , 0x98 , 0x10 , 0xC9 , 0x1B , 0xAA , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xE3 , 0x07 , 0x07 , 0x1F , 0x12 , 0x2F , 0x10 , 0x03 , 0x00 , 0x00 , 0xCD , 0x00 , 0x00 , 0x00 , 0xB8 , 0x0B , 0x99 , 0xAA};
//        memcpy(dat, rev_data_201, 41);
//        *len = 41;
        
        s_rf_handler.flag = E_RF_RX_FLAG_PROCESSING;
        
        //s_rf_handler.flag = 0;
        
        return 0;
    }
    
    return E_RF_ERR_NO_DATA;
}

u8 BSP_RF_GetRecvFlag(void)
{
    return s_rf_handler.flag;
}

u8 BSP_RF_GetState(void)
{
    return s_rf_handler.mode;
}

#endif