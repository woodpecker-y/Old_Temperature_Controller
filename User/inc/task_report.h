#ifndef _TASK_REPORT_H
#define _TASK_REPORT_H

#include "sysparams.h"

// 最大报文长度
#define MAX_PACKAGE_LEN     64
// 最大接收超时时间:2s
#define MAX_RECV_TIMEOUT    (UNIT_SECOND*2)
// 最大重试次数
#define MAX_RETRY_CNT       3

typedef enum _E_REPORT_STATE{
    E_REPORT_IDLE = 0,  //空闲状态
    E_REPORT_READY,     //准备状态
    E_REPORT_PACKAGE,   //组包状态
    E_REPORT_SEND,      //发送状态
    E_REPORT_RECV,      //接收状态
    E_REPORT_DEAL,      //处理状态
    E_REPORT_SUCCESS,   //成功状态
    E_REPORT_FINISH,    //完成状态
}E_REPORT_STATE;

typedef struct _ReportHandler{
    u8          sts;                        //状态
    u8          cmd;                        //上报的方式选择
    u8          pkg_snd[MAX_PACKAGE_LEN];   //发送的数据
    u8          pkg_snd_len;                //发送数据的长度
    u8         *pkg_rcv;                    //数据包的数据
    u8          pkg_rcv_len;                //数据包接收的长度
    u16         timeout;                    //超时时间
    u8          retry_cnt;                  //重试次数
    u32         report_period_timer;        //在供暖期
    u32         report_nperiod_timer;       //不在供暖期
//    u32         switch_login_timer;          //24小时签到一次
}ReportHandler;

/* 上报初始化 */
void task_report_init(void);
/* 上报主进程 */
void task_report_proc(void);
/* 上报触发 */
void task_report_triggered(u8 cmd);
/* 上报请求 */
void task_report_request(u8 cmd, u8 *pkg_snd, u8 *pkg_len);
/* 触发时间的选择 */
void task_report_schedule(void);

void task_report_request(u8 cmd, u8 *pkg_snd, u8 *pkg_len);

#endif