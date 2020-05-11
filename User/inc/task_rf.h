#ifndef _TASK_RF_H
#define _TASK_RF_H

/*! \brief key status definition */
typedef enum _TASK_RFSts{
    TASK_RF_STS_IDLE = 0,               // 空闲状态
    TASK_RF_STS_refresh,                // 无线标识刷新
}TASK_RFSts;

/*! \brief 无线句柄 */
typedef struct _TASK_RF_HANDLER{
    u8  sts;                    // 无线状态
    u32 timer;                   //无线标志反转时间
}TASK_RF_HANDLER;

/* 无线标识闪烁的主进程 */
void task_rf_proc(void);
/* 无线标识触发函数 */
void task_rf_triggered(u8 ant_sts_triggered);



#endif