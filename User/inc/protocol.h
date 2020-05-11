#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "sysparams.h"

// 设备签到的请求和反馈
u8 app_login_request(CJ188Pkg *pkg, const SysParams *sys_params, RunParams run_params);
u8 app_login_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params);

// 定时上报的请求和反馈
u8 app_report_request(CJ188Pkg *pkg, /*const*/ SysParams *sys_params, RunParams *run_params);
u8 app_report_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack);

// 确认反馈的请求和反馈
u8 app_ack_request(CJ188Pkg *pkg, const SysParams *sys_params, RunParams run_params);
u8 app_normal_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack);

// 出厂前配置函数
u8 app_factory_request(CJ188Pkg *pkg, /*const*/ SysParams *sys_params, RunParams run_params);
u8 app_factory_report_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack);
u8 app_factory_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack);

u16 LittleANDBig_ModeCON2(u16*Input);
u32 LittleANDBig_ModeCON4(u32*Input);

#endif