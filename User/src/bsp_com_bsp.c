#ifdef HAVE_COM_BSP

#include <stdio.h>
#include "bsp.h"
#include "bsp_com_bsp.h"
#include "stm8l15x_gpio.h"

///*! \brief UART max recv buffer size */
//#define UART_BUF_MAX_SIZE  80

#define MAX_COM_NUM 3

/*! \brief UART Recv FLags Definition */
typedef enum _UartRxFlags{
    UART_RX_FLAG_IDLE = 0,
    UART_RX_FLAG_RECVING = 1,
    UART_RX_FLAG_RECV_COMPLETE = 2,
    UART_RX_FLAG_PROCESSING = 3,
}UartRxFlags;

/*! \brief UART Recv Handler Definition */
typedef struct _ComRxHandler{
    u8*	data;
    u16	size;
    u16 max_size;
    UartRxFlags	flag;
}ComRxHandler;

/*! \brief UART Pointer List */
//USART_TypeDef* com_list[] = {USART1, USART2, USART3};
/*! \brief UART Recv Handler Declare */
static ComRxHandler  s_com_handler[MAX_COM_NUM];
/*! \brief UART Port Configure Handler */
static COMConfig*    s_com_cfg_handler;
static uint8_t       s_com_cfg_count = 0;
static uint8_t       s_com_rx_mode = GPIO_Mode_Out_PP_High_Fast;

static uint8_t BSP_COM_GetComIdx(COM com)
{
    uint8_t i = 0;

    for (i=0; i<s_com_cfg_count; ++i)
    {
        if (com == s_com_cfg_handler[i].com)
            break;
    }

    return i;
}

/*! \brief
*       com confirure handler
* \param com[IN]        uart com no.
*/
void BSP_COM_LoadConfig(COMConfig *com_cfg, uint8_t cnt)
{
    s_com_cfg_handler = com_cfg;
    s_com_cfg_count   = cnt;
}

/*! \brief
*       uart power on
* \param com[IN]        uart com no.
*/
uint8_t BSP_COM_PowerOn(COM com)
{
    //    if (&(s_com_cfg_handler[com_idx]->pwr) != NULL)
    //    {
    //        BSP_GPIO_Init(s_com_cfg_handler[com_idx]->pwr.port, s_com_cfg_handler[com_idx]->pwr.pin, GPIO_Mode_Out_PP_Low_Fast);
    //    }

    return 0;
}

/*! \brief
*       uart power off
* \param com[IN]        uart com no.
*/
uint8_t BSP_COM_PowerOff(COM com)
{
    //    if (&(s_com_cfg_handler[com_idx]->pwr) != NULL)
    //    {
    //        BSP_GPIO_Init(s_com_cfg_handler[com_idx]->pwr.port, s_com_cfg_handler[com_idx]->pwr.pin, GPIO_Mode_Out_PP_High_Slow);
    //    }

    return 0;
}

USART_TypeDef* BSP_COM_GetComPtr(COM com)
{
    uint8_t com_idx = 0;
    USART_TypeDef *p = NULL;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return NULL;
    }

    switch(com)
    {
    case COM1:
        p = USART1;
        break;
    case COM2:
        p = USART2;
        break;
    case COM3:
        p = USART3;
        break;
    }

    return p;
}

/*! \brief
*       uart initilization
* \param com[IN]                uart com no.
* \param baudrate[IN]           uart baudrate
* \param wordlength[IN]         databits
* \param stopbits[IN]           stopbits
* \param mode[IN]               USART_Mode_Tx or USART_Mode_Rx
* \param rx_interrupt_state[IN] uart recv interrupt enable state
* \return
*       none
*/
void BSP_COM_SetRxPinMode(u8 mode)
{
    s_com_rx_mode = mode;
}

/*! \brief
*       uart initilization
* \param com[IN]                uart com no.
* \param baudrate[IN]           uart baudrate
* \param wordlength[IN]         databits
* \param stopbits[IN]           stopbits
* \param mode[IN]               USART_Mode_Tx or USART_Mode_Rx
* \param rx_interrupt_state[IN] uart recv interrupt enable state
* \return
*       none
*/
uint8_t BSP_COM_Init(COM com, u32 baudrate, u8 wordlength, u8 stopbits, u8 parity, u8 mode, u8 rx_interrupt_state, uint8_t  rx_interrupt_prority)
{
    u16 i = 0;
    uint8_t com_idx = 0;
    USART_TypeDef *uart = NULL;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    uart = BSP_COM_GetComPtr(com);
    if (NULL == uart)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    /* Enable USART clock */
    switch(com)
    {
    case COM1:
        CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
        break;
    case COM2:
        CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
        break;
    case COM3:
        CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
        break;
    default:
        return E_COM_ERR_NOT_DEFINED_COM_ID;
        break;
    }

    //#ifdef USART1_REMAP
    switch(com)
    {
    case COM1:
        if (PORTA == s_com_cfg_handler[com_idx].rx.port && PIN3 == s_com_cfg_handler[com_idx].rx.pin)
        {
            // UART1 Remap
            SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);
        }
        break;
    case COM2:
        break;
    case COM3:
        if (PORTF == s_com_cfg_handler[com_idx].rx.port && PIN1 == s_com_cfg_handler[com_idx].rx.pin)
        {
            // UART3 Remap
            SYSCFG_REMAPPinConfig(REMAP_Pin_USART3TxRxPortF, ENABLE);
        }
        break;
    default:
        return E_COM_ERR_NOT_DEFINED_COM_ID;
        break;
    }

    /* Init Uart Port */
    BSP_GPIO_Init(s_com_cfg_handler[com_idx].rx.port, s_com_cfg_handler[com_idx].rx.pin, GPIO_Mode_In_PU_No_IT);
    //BSP_GPIO_Init(s_com_cfg_handler[com_idx].tx.port, s_com_cfg_handler[com_idx].tx.pin, GPIO_Mode_Out_PP_High_Fast);
    BSP_GPIO_Init(s_com_cfg_handler[com_idx].tx.port, s_com_cfg_handler[com_idx].tx.pin, s_com_rx_mode);
    //BSP_GPIO_Init(s_com_cfg_handler[com_idx].tx.port, s_com_cfg_handler[com_idx].tx.pin, GPIO_Mode_Out_PP_Low_Fast);
    //#endif
    //SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC, ENABLE);

    /* USART configuration */
    USART_Init(uart, baudrate, (USART_WordLength_TypeDef)wordlength, (USART_StopBits_TypeDef)stopbits, (USART_Parity_TypeDef)parity, (USART_Mode_TypeDef)mode);

    if (ENABLE == rx_interrupt_state)
    {
        // ENABLE RECV INTERRUPT
        USART_ITConfig(uart, USART_IT_RXNE, ENABLE);
        // ENABLE IDLE INTERRUPT
        USART_ITConfig(uart, USART_IT_IDLE, ENABLE);

        //  Init UART1 Recving Handler
        s_com_handler[com].size = 0;
        s_com_handler[com].max_size = s_com_cfg_handler[com_idx].max_rcv_size;
        s_com_handler[com].data = s_com_cfg_handler[com_idx].rcv_ptr;
        s_com_handler[com].flag = UART_RX_FLAG_IDLE;
        for (i=0; i<s_com_handler[com].max_size; ++i)
        {
            s_com_handler[com].data[i] = 0;
        }
    }

    // Enable USART
    USART_Cmd(uart, ENABLE);

    //printf("com[%d] max len:%d\r\n", com, s_com_handler[com].max_size);

    return 0;
}

/*! \brief
*       uart close
* \param com[IN]            uart com no.
* \return
*       none
*/
uint8_t BSP_COM_Close(COM com)
{
    uint8_t com_idx = 0;
    USART_TypeDef *uart = NULL;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    uart = BSP_COM_GetComPtr(com);
    if (NULL == uart)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    // Make pin of UART to LowPower Mode
    BSP_GPIO_Init(s_com_cfg_handler[com_idx].rx.port, s_com_cfg_handler[com_idx].rx.pin, GPIO_Mode_Out_PP_Low_Slow);
    BSP_GPIO_Init(s_com_cfg_handler[com_idx].tx.port, s_com_cfg_handler[com_idx].tx.pin, GPIO_Mode_Out_PP_Low_Slow);

    // DISABLE RECV INTERRUPT
    USART_ITConfig(uart, USART_IT_RXNE, DISABLE);
    // DISABLE IDLE INTERRUPT
    USART_ITConfig(uart, USART_IT_IDLE, DISABLE);
    // DISABLE USART
    USART_Cmd(uart, DISABLE);

    return 0;
}

/*! \brief
*       uart send one char
* \param com[IN]            uart com no.
* \param ch[IN]             one char
* \return
*       none
*/
uint8_t BSP_COM_SendChar(COM com, u8 ch)
{
    uint8_t com_idx = 0;
    USART_TypeDef *uart = NULL;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    uart = BSP_COM_GetComPtr(com);
    if (NULL == uart)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    USART_SendData8(uart, ch);

    do{
        /* Wait Send Complete*/
    }while (USART_GetFlagStatus(uart, USART_FLAG_TXE) == RESET);

    return 0;
}

/*! \brief
*       uart send string
* \param com[IN]            uart com no.
* \param str[IN]            string
* \return
*       none
*/
uint8_t BSP_COM_SendStr(COM com, const char *str)
{
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    while(*str != '\0')
    {
        BSP_COM_SendChar(com, (u8)*str);
        str++;
    }

    return 0;
}

/*! \brief
*       uart send data
* \param com[IN]            uart com no.
* \param val[IN]            bytes array to be sended
* \param len[IN]            size of bytes array
* \return
*       none
*/
uint8_t BSP_COM_SendData(COM com, const u8 *val, u16 len)
{
    u16 i = 0;
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    while (i < len)
    {
        BSP_COM_SendChar(com, val[i]);
        i++;
    }

    return 0;
}

/*! \brief
*       uart get recv flag
* \param com[IN]            uart com no.
* \return
*       RESET       - False
*       Others      - True
*/
FlagStatus BSP_COM_GetRxFlag(COM com)
{
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return 0xFF;
    }

    return USART_GetFlagStatus(BSP_COM_GetComPtr(com), USART_FLAG_RXNE);
}

/*! \brief
*       uart get recv one char
* \param com[IN]            uart com no.
* \return
*       value       - one char
*/
eCOMRet BSP_COM_RecvChar(COM com, uint16_t *dat)
{
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return 0xFF;
    }

    if (RESET != USART_GetFlagStatus(BSP_COM_GetComPtr(com), USART_FLAG_RXNE))
    {
        *dat = USART_ReceiveData8(BSP_COM_GetComPtr(com));
        USART_ClearFlag(BSP_COM_GetComPtr(com), USART_FLAG_RXNE);

        return E_COM_OK;
    }
    else
    {
        return E_COM_ERR_NO_DATA;
    }
}

//static void BSP_COM_Delay(void)
//{
//    int i = 20000;
//
//    for (; i>0; i--);
//}

/*! \brief
*       uart recv initilization
* \param com[IN]            uart com no.
* \note
*       1.clear data buffer;
*       2.make size of data already recved to 0
*       3.make flag to be UART_RX_FLAG_RECVING.
* \note
*       you must call BSP_COM_RecvInit function before recving data.
*/
uint8_t BSP_COM_RecvInit(COM com)
{
    u16 i = 0;
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    //delay_ms(10);

    s_com_handler[com].size = 0;
    s_com_handler[com].data = s_com_cfg_handler[com_idx].rcv_ptr;
    s_com_handler[com].flag = UART_RX_FLAG_RECVING;
    for (i=0; i<s_com_cfg_handler[com_idx].max_rcv_size; ++i)
    {
        s_com_handler[com].data[i] = 0;
    }

    return 0;
}

/*! \brief
*       uart read data from recv data buffer.
* \param com[IN]            uart com no.
* \param pval[OUT]          data ptr point to recv data buffer.
* \param len[OUT]           length of recved data.
* \return
*       >0          read data successfully.
*       =0          no data
*/
u8   BSP_COM_RecvData(COM com, u8 **pval, u16 *len)
{
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    if (UART_RX_FLAG_RECV_COMPLETE == s_com_handler[com].flag)
    {
        *pval = s_com_handler[com].data;
        *len  = s_com_handler[com].size;
        s_com_handler[com].flag = UART_RX_FLAG_PROCESSING;

        return E_COM_OK;
    }

    return E_COM_ERR_NO_DATA;
}

/*! \brief
*       uart read data from recv data buffer.
* \param com[IN]            uart com no.
* \param pval[OUT]          data ptr point to recv data buffer.
* \param len[OUT]           length of recved data.
* \return
*       >0          read data successfully.
*       =0          no data
*/
u8   BSP_COM_RecvDataStream(COM com, u8 **pval, u16 *len)
{
    uint8_t com_idx = 0;

    com_idx = BSP_COM_GetComIdx(com);
    if (com_idx == s_com_cfg_count)
    {
        return E_COM_ERR_NOT_DEFINED_COM_ID;
    }

    if (UART_RX_FLAG_RECV_COMPLETE == s_com_handler[com].flag)
    {
        *pval = s_com_handler[com].data;
        *len  = s_com_handler[com].size;

        return E_COM_OK;
    }

    return E_COM_ERR_NO_DATA;
}

/*! \brief
*       uart interrupt service routine.
* \param com[IN]            uart com no.
* \return
*      none
* \note
*       1.Uart recv interrupt service runtine is valid when rx_interrupt state is enabled
*       2.Uart starts to recv data When recv flag is UART_RX_FLAG_RECVING;
*       3.Uart makes flags to UART_RX_FLAG_RECV_COMPLETE when idle interrupt of uart is triggered.
*/
void BSP_COM1_ISR(COM com)
{
    //uint8_t com_idx = 0;

    // Recv Interrupt Processing
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        if ((s_com_handler[com].flag==UART_RX_FLAG_RECVING || s_com_handler[com].flag==UART_RX_FLAG_RECV_COMPLETE) && s_com_handler[com].size < s_com_handler[com].max_size)
        {
            s_com_handler[com].data[s_com_handler[com].size++] = USART_ReceiveData8(USART1);
        }

        // Clear Recv Interrupt
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    // Idle Interrupt Processing
    else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData8(USART1);

        s_com_handler[com].flag = UART_RX_FLAG_RECV_COMPLETE;
    }
}

void BSP_COM2_ISR(COM com)
{
    //uint8_t com_idx = 0;

    // Recv Interrupt Processing
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        if ((s_com_handler[com].flag==UART_RX_FLAG_RECVING || s_com_handler[com].flag==UART_RX_FLAG_RECV_COMPLETE) && s_com_handler[com].size < s_com_handler[com].max_size)
        {
            s_com_handler[com].data[s_com_handler[com].size++] = USART_ReceiveData8(USART2);
        }

        // Clear Recv Interrupt
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    // Idle Interrupt Processing
    else if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData8(USART2);

        s_com_handler[com].flag = UART_RX_FLAG_RECV_COMPLETE;
    }
}

void BSP_COM3_ISR(COM com)
{
    //uint8_t com_idx = 0;

    // Recv Interrupt Processing
//    if (USART_GetITStatus(USART3, USART_IT_OR) != RESET)
//    {
//        USART_ClearITPendingBit(USART3, USART_IT_OR);
//        if ((s_com_handler[com].flag==UART_RX_FLAG_RECVING || s_com_handler[com].flag==UART_RX_FLAG_RECV_COMPLETE) && s_com_handler[com].size < s_com_handler[com].max_size)
//        {
//            s_com_handler[com].data[s_com_handler[com].size++] = USART_ReceiveData8(USART3);
//        }
//    }
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        if ((s_com_handler[com].flag==UART_RX_FLAG_RECVING || s_com_handler[com].flag==UART_RX_FLAG_RECV_COMPLETE) && s_com_handler[com].size < s_com_handler[com].max_size)
        {
            s_com_handler[com].data[s_com_handler[com].size++] = USART_ReceiveData8(USART3);
        }

        // Clear Recv Interrupt
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
    // Idle Interrupt Processing
    else if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData8(USART3);

        s_com_handler[com].flag = UART_RX_FLAG_RECV_COMPLETE;
    }
}

void BSP_COM_ISR(COM com)
{
    switch(com)
    {
    case COM1:
        BSP_COM1_ISR(com);
        break;
    case COM2:
        BSP_COM2_ISR(com);
        break;
    case COM3:
        BSP_COM3_ISR(com);
        break;
    default:
        break;
    }
}

/**  @brief   com1�ⲿ�жϻ��ѽ��պ���
*  @param   rcv_buf [OUT]     - ���ջ���
*  @param   rcv_len [OUT]     - �������ݳ���
*
*  @retval  none
*/
u8 BSP_COM_LprRecvData(COM com, u8 **rcv_buf, u16 *rcv_len)
{
    u16 uart_timer = 4000;
    //u16 length = 0;

    s_com_handler[com].size = 0;

    while (uart_timer > 0)
    {
        if (RESET != BSP_COM_GetRxFlag(COM1))
        {
            BSP_COM_RecvChar(COM1, (uint16_t*)s_com_handler[com].data[s_com_handler[com].size]);
            s_com_handler[com].size++;
            //rcv_buf[length++] = BSP_COM_RecvChar(COM1);
            uart_timer = 4000;
        }
        else
        {
            uart_timer--;
            delay_us(10);
        }
    }

    //delay_ms(20);
    //MYLOG_DEBUG_HEXDUMP("Recv:%");
    if(s_com_handler[com].size > 0)
    {
        //*rcv_len = length;
        *rcv_buf = s_com_handler[com].data;
        *rcv_len = s_com_handler[com].size;
        // com_send_data(COM1, rcv_buf, *rcv_len);
        // delay_ms(5);
        return 0;
    }
    else
    {
        return E_COM_ERR_NO_DATA;
    }
}
#endif
