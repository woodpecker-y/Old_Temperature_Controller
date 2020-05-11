#ifndef _SYS_PARAMS_H
#define _SYS_PARAMS_H

#include "adf.h"
/* 时间 */
////#define CYCLE_AWU                       101UL         // 使用外部的32.768kHz晶振使用
//#define CYCLE_AWU                       29UL            // 使用内部的38kHz晶振使用，近似和上面时间，但是有误差
//#define UNIT_SECOND	                20UL            // 20个50ms是1s
//#define HALF_SECOND                     10UL

//#define CYCLE_AWU                       202UL         // 使用外部的32.768kHz晶振使用
#define CYCLE_AWU                       116UL            // 使用内部的38kHz晶振使用，近似和上面时间，但是有误差
#define UNIT_SECOND	                5UL            // 5个200ms是1s
#define HALF_SECOND                     2UL

#define FIVE_SECOND                     (5*UNIT_SECOND)
#define SIX_SECOND                      (6*UNIT_SECOND)
#define THREE_SECOND                    (3*UNIT_SECOND)
#define TWO_SECOND                      (2*UNIT_SECOND)
#define TEN_SECOND                      (10*UNIT_SECOND)
#define UNIT_MIN		        (60*UNIT_SECOND)
#define TWO_MIN                         (2*60*UNIT_SECOND)
#define FIVE_MIN                        (5*60*UNIT_SECOND)
#define FIFTEEN_SECOND		        (15*UNIT_SECOND)
#define THIRTY_SECOND		        (30*UNIT_SECOND)
#define QUARTER_HOUR                    (15*60*UNIT_SECOND)
#define HALF_HOUR                       (30*60*UNIT_SECOND)
#define UNIT_HOUR                       (60*60*UNIT_SECOND)
#define UNIT_DAY                        (24UL*60UL*60UL*UNIT_SECOND)

#define UNIT_KWH                        0x05    /*!< \brief KWH */
#define UNIT_GJ                         0x11    /*!< \brief GJ */

#define AWU_TIMER    15                 // ~50ms

/*      正式版根据生产下载ID流程分为三组，根据通道号不同区分。
    使用无线下载ID特殊频率 定为 420.5M -- 通道40  通道50   通道60*/
//#define DOWNLOAD_ID_CHANNEL    40
//#define DOWNLOAD_ID_CHANNEL    50
#define DOWNLOAD_ID_CHANNEL    60



//typedef enum _DevType{
//    E_DEV_TYPE_ELEC_VALVE = 0x01,
//    E_DEV_TYPE_IC_HP_LOCKING_VALVE,
//}DevType;
//
//typedef enum _eValveCtrl{
//    E_VALVE_MODE_OPEN  = 0x55,
//    E_VALVE_MODE_CLOSE = 0x99,
//    E_VALVE_MODE_AUTO  = 0x59,
//}eValveCtrl;
//
//typedef enum _ValveState{
//    E_VALVE_STATE_OPENED = 0x00,
//    E_VALVE_STATE_CLOSED = 0x01,
//    E_VALVE_STATE_ABNORMAL = 0x03,
//}ValveState;
//
//typedef enum _HPState{
//    E_HP_IN = 0,
//    E_HP_OUT,
//}HPState;
//
//typedef struct _RechargeInfoHP{
//    u16     recharge_sn;
//    u8      hp_st[4];
//    u8      hp_et[4];
//}RechargeInfoHP;
//
//typedef union _RechargeInfo{
//    RechargeInfoHP  hp_info;
//}RechargeInfo;

#define LENGTH_ADDR 8   //add的长度

// 显示控制位
typedef enum _DISP_CTL_BIT{
    DISP_CTL_HEAT_TIME_DAY = 0x01,
    DISP_CTL_HEAT_TIME_TOTAL = 0x02,
    DISP_CTL_HEAT_QUANTITY_DAY = 0x04,
    DISP_CTL_HEAT_QUANTITY_TOTAL = 0x08,
    DISP_CTL_BALANCE = 0x10,
    DISP_CTL_FAULT = 0x20,
    DISP_CTL_ROOM_NAME = 0x40,
    DISP_CTL_OWE = 0x80,
}DISP_CTL_BIT;

// 系统参数，需要存储与EEPROM中
typedef struct _SysParams{
    //签到
    u8      factory_flag;		/*!< \brief 出厂标识 */                   // 0
    u8      padding1;                                                             // 1
    u16     Device_GN;			//产品组号		2字节             // 2
    u32	    Device_SN;			//产品序列号	4字节                     // 4
    u8	    soft_persion;		//软件版本		1字节             // 8
    u8	    hardware_persion;	        //硬件版本		1字节             // 9
    u8	    limte_te_flag;		//极限温度设置标志                        // 10
    u8	    limte_te_value;		//极限温度设置值                          // 11
    u8	    workflg;			//是否在供暖季                            // 12
    u8	    sys_set_temp_flg;		//系统设置温度有效标志                    // 13
    u16     temp_set;                   /*!< \brief 温度设置 */                   // 14
    u32     heating_quantity;		/*!< \brief 累计热量  */                  // 16
    u32     heating_time;		/*!< \brief 用热时间 单位：h */           // 20
    u32	    com_ok;			//面板通信成功次数                        // 24
    u32	    com_total;		        //面板通信总计次数                        // 28

//    u8      factory_flag;			/*!< \brief 出厂标识 */
//    u8      padding1;
    u8      addr[LENGTH_ADDR];                  /*!< \brief 设备地址 */           // 32
    u8      heating_period_st[2];		/*!< \brief 供暖期开始时间 MMDD */// 40
    u8      heating_period_et[2];		/*!< \brief 供暖期结束时间 MMDD */// 42
//    u8      padding3;
    u8      temp_room_uplimits;		        /*!< \brief 室温设定上限 单位：℃ */// 44
//    u8      padding4;
    u8      temp_room_lowlimits;	        /*!< \brief 室温设定下限 单位：℃ */// 45
//    u8      padding5;
    u8      rf_fre;                             /*!< \brief RF基础频率 */           // 46
//    u8      padding6;
    u8      rf_channel;                         /*!< \brief RF通道号 */            // 47
//    u8      padding7;
    u8      rf_power;                           /*!< \brief RF功率 */             // 48
//    u8      padding8;
    u8      disp_ctl;                           /*!< \brief 显示控制位 */            // 49



    //定时上传
////    u16     temp_set;                           /*!< \brief 温度设置 */  // ***
    u8      room_name[6];                       /*!< \brief 房间名称  */            // 50
//    u16     cycle_sample;			/*!< \brief 采样周期 单位：Min */
//    u16     sn_recharge;		        /*!< \brief 充值序列号 */
////    u32     heating_quantity;		        /*!< \brief 累计热量  */            // ***
//    u8	    heating_quantity_unit;	        /*!< \brief 累计热量单位 单位定义见188协议 */
//    u8	    balance_unit;		        /*!< \brief 可用热量单位 定义见188协议 */
//    u8      balance_alarm;			/*!< \brief 余额警告值 单位：KWH */
//    long    balance;			        /*!< \brief 可用余额 单位：0.01,可以为负值 */
////    u32     heating_time;		        /*!< \brief 用热时间 单位：h */                // ***
//    u32     heating_cost_total;		        /*!< \brief 总累计分摊热费 单位：0.01元 */
//    u32     heating_time_day;		        /*!< \brief 日累计开阀时间 单位：h */
//    u32     heating_cost_day;		        /*!< \brief 日累计分摊热费 单位：0.01元 */
//    u32     heating_quantity_day;		/*!< \brief 日累计分摊热量 */
//    u8	    heating_quantity_day_unit;	        /*!< \brief 日累计分摊热量单位 单位定义见188协议 */

    // 丰源加
//    u32		com_ok;			    //面板通信成功次数
//    u32		com_total;		    //面板通信总计次数
    // 头数据

//    u32		Device_SN;			//产品序列号	4字节
//    u16		Device_DESN;			//保留字节		1字节
//    u16		Device_GN;			//产品组号		2字节
//    u8		soft_persion;			//软件版本		1字节
//    u8		hardware_persion;	        //硬件版本		1字节

//    u8		workflg;			//是否在供暖季
//    u8		sys_set_temp_flg;		//系统设置温度有效标志
//    u8		limte_te_flag;		//极限温度设置标志
//    u8		limte_te_value;		//极限温度设置值

}SysParams;

// 运行参数，仅存在于内存中
typedef struct _RunParams{
    u8      adc_keytrigger;                     /*!< \brief 按键触发adc快速采样标志 */
    u8      ds18b20_keytrigger;                 /*!< \brief 按键触发ds18b20快速采样标志 */
    u8      menu_id;                            /*!< \brief 菜单编号 */
    u8      view_idx;                           /*!< \brief 查询编号 */
    u8      date[7];                            /*!< \brief 日期 */
    //长显
    s16     temp;                               /*!< \brief 温度 */                       // ***
    s16     temp_last;                           /*!< \brief 上一次的温度 */
    u8      ant_flash;			        /*!< \brief 通讯失连时，无线闪烁标识 */
    int     batt_lvl;                           /*!< \brief 电池级数 */
    u8      low_power;                           /*!< \brief 低电量 */
    u8      ant_sts;                            /*!< \brief 无线状态:0-无信号；1-有信号；2-无信号，连接中... */
    u16     st;                                 /*!< \brief 状态ST 用于判断是否故障、警告、阀门开关状态 */

    u8      valve_state;                        /*!< \brief 阀门的状态 */            // ***
    u8      arrears_state;                      /*!< \brief 欠费状态 */             // ***
    //协议
    u8      sts_login;                          /*!< \brief 登陆状态 */
    u16     temp_return;                        /*!< \brief 设定温度的运行参数 */
    u8      set_id;                             /*!< \brief 设定id */
    u8      key_long;                           /* 按键按下此标志为1，释放为0 */

    // 丰源加
    s16	    ave_temp_100;			//平均温度，扩大100倍
    u8	    face_state;				//面板电量低状态
    u8	    actionflag;			        //动作标志;0X00 无效  0X55  开阀  0XAA关阀
    u8	    valve_position;	                //开关阀阀门位置
    u8	    valve_lockstate;                    //阀门堵转状态

    u16		year;				//年
    u8		month;			//月
    u8		day;				//日
    u8		hour;				//时
    u8		minute;			//分
    u8		second;			//秒
    u8		week;			//周
    u8		batt_collect_flag;			//周
    u8          rf_send_start;           // 无线开始发送标志
}RunParams;


// 信号状态
typedef enum _AntSts{
    ANT_NO_SIGNAL = 0,
    ANT_CONNECTRED,
    ANT_CONNECTING,
}AntSts;

typedef enum
{
    State_OK = (u8)0x00,
    State_Error= (u8)0xAA
} State_TypeDef;

typedef struct
{
    u8   	Second;
    u8   	Minute;
    u8  	Hour;
    u8   	Week;
    u8   	Day;
    u8  	Month;
    u16  	Year;
    u16  	Msec;
}RealClock_Str;

typedef enum
{
    Force_NOR = (u8)0X00,		//非强制状态    (没有欠费)
    Force_Close	= (u8)0XAA,		//强制关阀     	(欠费)
    Force_Open = (u8)0X55,		//强制开阀
    Valve_NOR = (u8)0X00,		//阀门无动作请求
    Valve_Open = (u8)0X0A,		//开阀状态
    Valve_Middle = (u8)0X55,		//中间状态
    Valve_Execute = (u8)0XAA,		//正在执行
    Valve_Close	= (u8)0XA0,		//关阀状态
    Valve_Lock = (u8)0XEE,		//堵转状态
    Valve_Break	= (u8)0XFF,		//故障状态

}Valve_Type;

extern SysParams g_sys_params;   //g_sys_params结构体变量
extern RunParams g_run_params;

void eeprom_write_sys(u8 offset, void* sys_params, u8 length);

#endif