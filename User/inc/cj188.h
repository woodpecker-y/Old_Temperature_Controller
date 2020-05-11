#ifndef _CJ188_H
#define _CJ188_H

#include "sysparams.h"

// 最大数据负载长度
#define MSG_DATA_MAX    80
// 起始帧
#define CJ188_STX       0x68
// 结束帧
#define CJ188_ETX       0x16
// 通断时间面积法设备类型定义
#define CJ188_MTYPE_THERMOSTAT         0x41
#define CJ188_MTYPE_ONOFF_ACTUATORS    0x42
// 控制码
#define CTRL_CODE_DEV_LOGIN         0x20    // 设备签到
#define CTRL_CODE_REPORT            0x21    // 定时上报
#define CTRL_CODE_FACTORY           0x22    // 出厂配置
#define CTRL_CODE_ACK               0x26    // 确认反馈

/* CJ188协议帧定义 */
typedef struct _CJ188Pkg{
    u8  addr[LENGTH_ADDR];      // 仪表地址
    u8  ctrl_code;              // 控制码
    u8  msg_data[MSG_DATA_MAX]; // 数据
    u8  msg_size;               // 数据长度
    
    u32		sn;			//产品序列号	4字节
    u16		gn;			//产品组号		2字节
//    u8		ctrl_type;		//控制类型		1字节
//    u8		msg_length;		//数据长度		1字节
}CJ188Pkg;

/* cj188解包错误的枚举 */
typedef enum _CJ188Ret{
    CJ188_OK = 0,
    CJ188_ERR_CHECKSUM,         // 校验和错误
    CJ188_ERR_LESS_MEMORY,      // 内存不足
    CJ188_ERR_LENGTH,           // 长度错误
    CJ188_ERR_ADDR,             // 地址不匹配
    CJ188_ERR_INVALID_PKG,      // 数据包错误
    CJ188_ERR_CTRL_CODE,        // 控制码错误
}CJ188Ret;

/* 定时上报附加数据的枚举 */
typedef enum _AddtionDataIdx{
    AD_IDX_HP = 0,			// 供暖期
    AD_IDX_TEMP_SET,			// 远程设定温度
    AD_IDX_TEMP_SET_LIMITS,		// 室温设定限值
    AD_IDX_RECHARGE,			// 充值序号
    AD_IDX_BALANCE_LINMITS,		// 余额限值
    AD_IDX_DISP_CTL,                    // 显示控制位
    AD_IDX_ROOM_NAME,                   // 房间名称
}AddtionDataIdx;

/*! \brief cj188协议打包 */
u8 cj188_pack(CJ188Pkg *pkg, u8 *out_data, u8 *out_size);
/*! \brief cj188协议解包 */
u8 cj188_unpack(CJ188Pkg *pkg, u8 *in_data, u8 in_size);

#endif