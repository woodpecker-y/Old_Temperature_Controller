#ifndef _SHT20_H
#define _SHT20_H

#define SHT2x_Delay(a)          delay_us(a)

#define GPIOSCL_init()          gpio_init(PORTF,PIN4, GPIO_Mode_Out_OD_Low_Fast)
#define GPIOSDA_init()          gpio_init(PORTF,PIN1, GPIO_Mode_Out_OD_Low_Fast)

#define SHT2x_SCL_OUTPUT()      gpio_init(PORTF,PIN4, GPIO_Mode_Out_OD_Low_Fast)     //设置为输出
#define SHT2x_SCL_INPUT()       gpio_init(PORTF,PIN4, GPIO_Mode_In_FL_No_IT)         //设置为浮空输入
#define SHT2x_SDA_OUTPUT()      gpio_init(PORTF,PIN1, GPIO_Mode_Out_OD_Low_Fast)     //设置为输出
#define SHT2x_SDA_INPUT()       gpio_init(PORTF,PIN1, GPIO_Mode_In_FL_No_IT)         //设置为浮空输入

#define SHT2x_SCL_HIGH()        gpio_set_high(PORTF,PIN4)   //PB4置高      GPIO_SetBits(SHT2x_I2C_PORT, SHT2x_SCL_PIN)
#define SHT2x_SCL_LOW()         gpio_set_low(PORTF,PIN4)    //PB4置低      GPIO_ResetBits(SHT2x_I2C_PORT, SHT2x_SCL_PIN)

#define SHT2x_SDA_HIGH() 	    gpio_set_high(PORTF,PIN1)   //PB5置高      GPIO_SetBits(SHT2x_I2C_PORT, SHT2x_SDA_PIN)
#define SHT2x_SDA_LOW()         gpio_set_low(PORTF,PIN1)    //PB5置低      GPIO_ResetBits(SHT2x_I2C_PORT, SHT2x_SDA_PIN)

#define SHT2x_SCL_STATE()       gpio_read_state(PORTF,PIN4)  //读IO的状态  GPIO_ReadInputDataBit(SHT2x_I2C_PORT, SHT2x_SCL_PIN)
#define SHT2x_SDA_STATE()       gpio_read_state(PORTF,PIN1)  //读IO的状态  GPIO_ReadInputDataBit(SHT2x_I2C_PORT, SHT2x_SDA_PIN)

#define Bit_RESET               0
#define Bit_SET                 1

typedef enum {
    TRIG_TEMP_MEASUREMENT_HM   = 0xE3, //command trig. temp meas. hold master
    TRIG_HUMI_MEASUREMENT_HM   = 0xE5, //command trig. humidity meas. hold master
    TRIG_TEMP_MEASUREMENT_POLL = 0xF3, //command trig. temp meas. no hold master
    TRIG_HUMI_MEASUREMENT_POLL = 0xF5, //command trig. humidity meas. no hold master
    USER_REG_W                 = 0xE6, //command writing user register
    USER_REG_R                 = 0xE7, //command reading user register
    SOFT_RESET                 = 0xFE  //command soft reset
} SHT2xCommand;

typedef enum {
    SHT2x_RES_12_14BIT         = 0x00, //RH=12bit, T=14bit
    SHT2x_RES_8_12BIT          = 0x01, //RH= 8bit, T=12bit
    SHT2x_RES_10_13BIT         = 0x80, //RH=10bit, T=13bit
    SHT2x_RES_11_11BIT         = 0x81, //RH=11bit, T=11bit
    SHT2x_RES_MASK             = 0x81  //Mask for res. bits (7,0) in user reg.
} SHT2xResolution;

typedef enum {
    SHT2x_EOB_ON               = 0x40, //end of battery
    SHT2x_EOB_MASK             = 0x40  //Mask for EOB bit(6) in user reg.
} SHT2xEob;

typedef enum {
    SHT2x_HEATER_ON            = 0x04, //heater on
    SHT2x_HEATER_OFF           = 0x00, //heater off
    SHT2x_HEATER_MASK          = 0x04  //Mask for Heater bit(2) in user reg.
} SHT2xHeater;

typedef enum {
    TEMP,  
    HUMI 
} SHT2xMeasureType;

typedef enum {
    I2C_ADR_W                  = 0x80, //sensor I2C address + write bit
    I2C_ADR_R                  = 0x81  //sensor I2C address + read bit
} SHT2xI2cHeader;

typedef enum {
    ACK                        = 0,
    NO_ACK                     = 1
} SHT2xI2cAck;

typedef enum {
    ACK_OK                     = 0x00,  
    ACK_ERROR                  = 0x01
} SHT2xI2cAckState;

void SHT2x_Init(void);
s16 SHT2x_MeasureTempPoll(void);
s16 SHT2x_MeasureHumiPoll(void);
u8 SHT2x_ReadUserReg(void);
u8 SHT2x_WriteUserReg(u8 reg);
void SHT2x_SoftReset(void);
void SHT2x_Test(s16 *,s16 *);

#endif

