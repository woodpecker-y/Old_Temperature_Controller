#ifndef _DISP_H
#define _DISP_H

#include "sysparams.h"
#include "bsp.h"
#include "lcd.h"

#define DISPLAY_ON          1       /*!< \brief 显示开 */
#define DISPLAY_OFF         0       /*!< \brief 显示关 */


/*! \brief 段位编码 */
typedef enum _LcdSeg{
    /* COM1 */
    SEG_BH = 1,     //编号
    SEG_L = 2,      //竖线
    SEG_KL = 4,     //警告
    SEG_T1 = 5,     //无线信号第一格（小圆点）
    SEG_6D = 6,     //数码管6D
    SEG_S6 = 7,     //剩余热量
    SEG_5D = 8,     //数码管5D
    SEG_S5 = 9,     //累计热量
    SEG_4D = 10,    //数码管4D
    SEG_S4 = 11,    //用热时间
    SEG_3D = 12,    //数码管3D
    SEG_S3 = 13,    //点号S3
    SEG_2D = 14,    //数码管2D
    SEG_S2 = 15,    //点号S2
    SEG_1D = 16,    //数码管1D
    SEG_S1 = 17,    //单位：时
    /* COM2 */
    SEG_QF = 28,    //欠费
    SEG_GJ = 29,    //单位：GJ
    SEG_SJ = 30,    //时间
    SEG_B4 = 31,    //电池电量第三格
    SEG_XH = 32,    //雷达标识
    SEG_T2 = 33,    //无线信号第二格
    SEG_6E = 34,    //数码管6E
    SEG_6C = 35,    //数码管6C
    SEG_5E = 36,    //数码管5E
    SEG_5C = 37,    //数码管5C
    SEG_4E = 38,    //数码管4E
    SEG_4C = 39,    //数码管4C
    SEG_3E = 40,    //数码管3E
    SEG_3C = 41,    //数码管3C
    SEG_2E = 42,    //数码管2E
    SEG_2C = 43,    //数码管2C
    SEG_1E = 44,    //数码管1E
    SEG_1C = 45,    //数码管1C
    /* COM3 */
    SEG_FJ = 56,    //房间号
    SEG_KWH = 57,   //单位;KWh
    SEG_GZ = 58,    //故障
    SEG_B3 = 59,    //电池电量第二格
    SEG_FM = 60,    //阀门标识
    SEG_T3 = 61,    //无线信号第三格
    SEG_6F = 62,    //数码管6F
    SEG_6G = 63,    //数码管6G
    SEG_5F = 64,    //数码管5F
    SEG_5G = 65,    //数码管5G
    SEG_4F = 66,    //数码管4F
    SEG_4G = 67,    //数码管4G
    SEG_3F = 68,    //数码管3F
    SEG_3G = 69,    //数码管3G
    SEG_2F = 70,    //数码管2F
    SEG_2G = 71,    //数码管2G
    SEG_1F = 72,    //数码管1F
    SEG_1G = 73,    //数码管1G
    /* COM4 */
    SEG_SD = 84,    //设定
    SEG_DF = 85,    //单位：℃
    SEG_SW = 86,    //室温
    SEG_B2 = 87,    //电池电量第一格
    SEG_B1 = 88,    //电池电量边框
    SEG_T4 = 89,    //无线信号第四格
    SEG_6A = 90,    //数码管6A
    SEG_6B = 91,    //数码管6B 
    SEG_5A = 92,    //数码管5A
    SEG_5B = 93,    //数码管5B 
    SEG_4A = 94,    //数码管4A
    SEG_4B = 95,    //数码管4B 
    SEG_3A = 96,    //数码管3A
    SEG_3B = 97,    //数码管3B 
    SEG_2A = 98,    //数码管2A
    SEG_2B = 99,    //数码管2B 
    SEG_1A = 100,   //数码管1A
    SEG_1B = 101,   //数码管1B 
}LcdSeg;

/*! \brief digit display position */
typedef enum _DigitPos{
    DIGIT6 = 0, //数码管6索引
    DIGIT5,     //数码管5索引
    DIGIT4,     //数码管4索引
    DIGIT3,     //数码管3索引
    DIGIT2,     //数码管2索引
    DIGIT1,     //数码管1索引
}DigitPos;

/*! \brief 电量的枚举 */
typedef enum _BattLel{
    BATT_LVL_0 = 0, //0格电量
    BATT_LVL_1,     //1格电量
    BATT_LVL_2,     //2格电量
    BATT_LVL_3,     //3格电量
}BattLel;

/*! \brief 阀门开关状态 */
typedef enum _Valve_Sts{
    VALVE_STS_CLOSE = 0x00, //阀门关闭状态
    VALVE_STS_OPEN  = 0x01, //阀门打开状态
}_Valve_Sts;


/*! \brief
*       display a digit 显示一个数字
* \param pos[IN]    - segment digit display index
*       DIGIT1      - the left digist position
*       DIGIT2      - the second digit position
*       DIGIT3      - the third digit position
*       DIGIT4      - the fourth digit position
*       DIGIT5      - the fifth digit position
* \param val[IN]    - display value index:0~9,A~F,S,T,-,P,r,' '
* \param sts[IN]    - display state
*       DISPLAY_ON  - display on
*       DISPLAY_OFF - display off
* \note
*       1)stm8l152?????????????????????????????????????????????????????????????
*               SEG1    SEG2    SEG3    SEG4
*       COM1    *       *       *       *
*       COM2    -       -       -       -
*       COM3    -       -       -       -
*       COM4    -       -       -       -
*           *       ?????????????
*           -       ?????????????
*       stm8l152??跽??????????COM1????????COM2\COM3\COM4??????????????????LCD????о
*   ????????????????λ????????????????????о??????????SEG??????????STM8L152?????COM??????????
*       2)?????????????????????λ?????????bitλ???????????????????λ????
*   ?????????????????????????????λ????????λ???и????????
*/
void disp_digit(u8 pos, u8 val, u8 sts);

/*! \brief
*       display a digit
* \param pos[IN]    - segment char display index
*       DIGIT1      - the left digist position
*       DIGIT2      - the second digit position
*       DIGIT3      - the third digit position
*       DIGIT4      - the fourth digit position
*       DIGIT5      - the fifth digit position
* \param ch[IN]     - display character
*/
void disp_char(u8 start, u8 ch);
/*! \brief 显示字符串 */
void disp_str(u8 start, u8 end, const char *str);
/*! \brief 显示整串数字 */
void disp_number(u8 start, u8 end, u32 val);
/*! \brief 液晶初始化 */
void disp_init(void);
/*! \brief 液晶关闭 */
void disp_close(void);
/*! \brief 显示液晶 */
void disp_update(void);
/*! \brief 液晶全显 */
void disp_all(void);
/*! \brief 液晶清屏 */
void disp_clr(void);
/*! \brief 显示分割线 */
void disp_line(u8 disp_sts);
/*! \brief 显示电池电量 */
void disp_batt(u8 disp_sts, u8 lv);
/*! \brief 显示无线信号 */
void disp_ant(u8 disp_sts, u8 sts);
/*! \brief 显示信号（雷达信号） */
void disp_rf_state(u8 disp_sts);
/*! \brief 显示警告 */
void disp_warning(u8 disp_sts);
/*! \brief 显示欠费 */
void disp_owed(u8 disp_sts);
/*! \brief 显示用热时间 */
void disp_heat_time(u32 heat_time);
/*! \brief 显示系统时间年月日 */
void disp_sys_time_ymd(u8 year, u8 month, u8 day);
/*! \brief 显示系统时间时分秒 */
void disp_sys_time_hms(u8 hour, u8 min, u8 second);
/*! \brief 显示室内温度 */
void disp_temp_room(s16 temp_room);
/*! \brief 显示设定温度 */
void disp_temp_set(u16 temp_set);
/*! \brief 显示阀门状态 */
void disp_valve_sts(u8 sts);
/*! \brief 显示房间号 */
void disp_room_id(const char *id, u8 len);
/*! \brief 显示ID（备注：ID不能一次显示完全，需要调用两次，分别显示高4位和低6位） */
void disp_id(const unsigned char *id, u8 len);
/*! \brief 显示故障 */
void disp_fault(u16 code);
/*! \brief 显示累计热量 */
void disp_heat_quantity(u32 heat_quantity, u8 heat_quantity_uint);
/*! \brief 显示剩余热量 */
void disp_surplus_heat(long balance, u8 unit);
/*! \brief 清除不必要的显示 */
void disp_clr_region(void);
/*! \brief 显示小时和分钟 */
void disp_sys_time(u8 hour, u8 min);

void disp_set_id(void);

#endif