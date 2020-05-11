#ifndef _TASK_DISP_H
#define _TASK_DISP_H

#include "sysparams.h"

typedef enum {
    E_TASK_DISP_IDLE = 0,   //空闲状态
    E_TASK_DISP_REFRESH,    //刷新状态
}TASK_DISP_STATE;

typedef struct _TASK_DISP_HANDLER{
    u8  sts;    //状态
    u32 menu;   //菜单
}TASK_DISP_HANDLER;

/* 显示的主进程 */
void task_disp_proc();
/* 显示的触发函数 */
void task_lcd_triggered(u8 menu_id);

#endif