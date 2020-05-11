
#ifndef _REMAP_H
#define _REMAP_H

#ifdef HAVE_STM8S

// 函数名称重定义
#define GPIO_SetBits			GPIO_WriteHigh
#define GPIO_ResetBits		        GPIO_WriteLow
#define GPIO_ReadInputDataBit	        GPIO_ReadInputPin

// 类型名称重定义
#define EXTI_Pin_TypeDef                
#endif

#endif