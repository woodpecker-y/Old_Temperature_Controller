#include "bsp.h"
#include "bsp_gpio.h"

/*! \brief
*       gpio port remapping
* \param port[IN]                gpio port index.
* \return
*       gpio port pointer
*/
GPIO_TypeDef* BSP_GPIO_Port_Remap(Port port)
{
    assert_param(port==PORTA || port==PORTB || port==PORTC || port==PORTD || port==PORTE || port==PORTF || port==PORTG);

    switch(port)
    {
    case PORTA:
        return GPIOA;
        break;
    case PORTB:
        return GPIOB;
        break;
    case PORTC:
        return GPIOC;
        break;
    case PORTD:
        return GPIOD;
        break;
    case PORTE:
        return GPIOE;
        break;
    case PORTF:
        return GPIOF;
        break;
    case PORTG:
        return GPIOG;
        break;        
    default:
        return NULL;
        break;
    }
}

/*! \brief
*       gpio pin remapping
* \param pin[IN]                gpio pin index.
* \return
*       stm8l15x chip gpio pin index
*/
GPIO_Pin_TypeDef BSP_GPIO_Pin_Remap(Pin pin)
{
    assert_param(pin==PIN0 || pin==PIN1 || pin==PIN2 || pin==PIN3 || pin==PIN4 || pin==PIN5 || pin==PIN6 || pin==PIN7);

    switch(pin)
    {
    case PIN0:
        return GPIO_Pin_0;
        break;
    case PIN1:
        return GPIO_Pin_1;
        break;
    case PIN2:
        return GPIO_Pin_2;
        break;
    case PIN3:
        return GPIO_Pin_3;
        break;
    case PIN4:
        return GPIO_Pin_4;
        break;
    case PIN5:
        return GPIO_Pin_5;
        break;
    case PIN6:
        return GPIO_Pin_6;
        break;
    case PIN7:
        return GPIO_Pin_7;
        break;
    default:
        return GPIO_Pin_All;
        break;
    }
}

/*! \brief
*       Get EXTI Num by Pin Number
* \param pin[IN]        - Pin Number of Port
* \return EXTI Number
*/
EXTI_Pin_TypeDef BSP_GPIO_GetEXTIPinNum(Pin pin)
{
    assert_param(pin==PIN0 || pin==PIN1 || pin==PIN2 || pin==PIN3 || pin==PIN4 || pin==PIN5 || pin==PIN6 || pin==PIN7);
    //assert(pin!=PIN0 && pin!=PIN1 && pin!=PIN2 && pin!=PIN3 && pin!=PIN4 && pin!=PIN5 && pin!=PIN6 && pin!=PIN7);

    switch(pin)
    {
    case PIN0:
        return EXTI_Pin_0;
        break;
    case PIN1:
        return EXTI_Pin_1;
        break;
    case PIN2:
        return EXTI_Pin_2;
        break;
    case PIN3:
        return EXTI_Pin_3;
        break;
    case PIN4:
        return EXTI_Pin_4;
        break;
    case PIN5:
        return EXTI_Pin_5;
        break;
    case PIN6:
        return EXTI_Pin_6;
        break;
    case PIN7:
        return EXTI_Pin_7;
        break;
    default:
        return EXTI_Pin_0;
        break;
    }
}

/*! \brief
*       gpio pin initilization
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
* \param mode[IN]           - gpio pin init mode.
*/
u8 BSP_GPIO_Init(Port port, Pin pin, u8 mode)
{
    GPIO_Init(BSP_GPIO_Port_Remap(port), BSP_GPIO_Pin_Remap(pin), (GPIO_Mode_TypeDef)mode);
    
    return 0;
}

/*! \brief
*       set gpio pin to 1
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
*/
u8 BSP_GPIO_SetHigh(Port port, Pin pin)
{
    GPIO_SetBits(BSP_GPIO_Port_Remap(port), BSP_GPIO_Pin_Remap(pin));
    
    return 0;
}

/*! \brief
*       set gpio pin to 0
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
*/
u8 BSP_GPIO_SetLow(Port port, Pin pin)
{
    GPIO_ResetBits(BSP_GPIO_Port_Remap(port), BSP_GPIO_Pin_Remap(pin));
    
    return 0;
}

/*! \brief
*       read gpio pin state
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
* \return
*       pin state:0-low level;1-high level
*/
u8  BSP_GPIO_ReadState(Port port, Pin pin)
{
    return GPIO_ReadInputDataBit(BSP_GPIO_Port_Remap(port), BSP_GPIO_Pin_Remap(pin)) == RESET ? 0 : 1;
}


