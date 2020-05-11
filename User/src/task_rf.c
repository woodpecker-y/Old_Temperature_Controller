#include "adf.h"
#include "sysparams.h"
#include "board.h"
#include "task_rf.h"
#include "task_report.h"
#include "menu.h"
#include "task_disp.h"
#include "disp.h"
#include "error.h"

static TASK_RF_HANDLER s_rf_handler; //定义了一个结构体变量

/*! \brief
*  无线标识闪烁主进程
*/
void task_rf_proc(void)
{
    switch(s_rf_handler.sts)
    {
    case TASK_RF_STS_IDLE:
        break;
    case TASK_RF_STS_refresh:   
        if (g_run_params.ant_sts <= ANT_CONNECTRED)//如果信号连接或没连接
        {
            disp_ant(DISPLAY_ON, g_run_params.ant_sts);//为无线的格数显示
            /* 下面为雷达信号的显示 */
            if (g_run_params.ant_sts == 0)//它的值用枚举表示，分为三种信号状态
            {
//////                disp_rf_state(DISPLAY_OFF);
                disp_rf_state(DISPLAY_ON);
////                error_set(FAULT_COMM);
                error_set_u8(FAULT_RF);
            }
            else
            {
//////                disp_rf_state(DISPLAY_ON);
                disp_rf_state(DISPLAY_OFF);
////                error_clr(FAULT_COMM);
                error_clr_u8(FAULT_RF);
            }
            s_rf_handler.sts = TASK_RF_STS_IDLE;
        }
        else
        {
            s_rf_handler.timer++;
            if(s_rf_handler.timer == HALF_SECOND)
            {
                s_rf_handler.timer = 0;
                g_run_params.ant_flash ^= 0x01;
                //printf("g_run_params.ant_flash=%x" ,g_run_params.ant_flash);
                disp_ant(DISPLAY_ON, g_run_params.ant_flash);
            }
        }
//////        disp_update();//刷新屏幕
        
        break;
    }
}

/*! \brief
*  无线标识亮灭的触发函数
*/
void task_rf_triggered(u8 ant_sts_triggered)
{
    g_run_params.ant_sts = ant_sts_triggered;
    s_rf_handler.sts = TASK_RF_STS_refresh;  
}


