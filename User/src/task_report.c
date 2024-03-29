#include "adf.h"
#include "task_report.h"
#include "sysparams.h"
#include "cj188.h"
#include "protocol.h"
//#include "si4432.h"
#include "bsp_si4432.h"
#include "task_rf.h"
#include "gpio.h"
#include "error.h"
#include "board.h"

static ReportHandler s_report_handler;
u8  ack = 0;

/*! \brief
*   上报主进程
*
* \note
*
*/
//u8  rv  = 0;
static CJ188Pkg pkg;//定义了一个结构体变量pkg

void task_report_proc(void)
{
    u8  rv  = 0;
    //CJ188Pkg pkg;//定义了一个结构体变量pkg

    switch(s_report_handler.sts)
    {
    case E_REPORT_IDLE://空闲状态
        task_report_schedule();
        break;
    case E_REPORT_READY://准备状态
        //printf("E_REPORT_READY\r\n");
        if(g_run_params.set_id == 0)
        {
            rf_power_on();
//          printf("g_sys_params.rf_fre = %d\r\n" ,g_sys_params.rf_fre);
//          printf("g_sys_params.rf_channel = %d\r\n" ,g_sys_params.rf_channel);
            rv = rf_init((RF_FRE)g_sys_params.rf_fre, RF_DATA_RATE_1200, TXPOW_20dBm, g_sys_params.rf_channel);
//          rv = rf_init(RF_FRE_470_5M, RF_DATA_RATE_1200, TXPOW_20dBm, 0);

            if (rv == 0)
            {
                s_report_handler.sts = E_REPORT_PACKAGE;//转为打包状态
            }
            else
            {
                s_report_handler.sts = E_REPORT_IDLE;//转为空闲状态
                //printf("rf_init error[%d]\r\n", rv);
            }
        }
        else
        {
            s_report_handler.sts = E_REPORT_IDLE;//转为空闲状态
        }
        break;

    case E_REPORT_PACKAGE://打包状态
        //printf("E_REPORT_PACKAGE\r\n");
        // 新增加通讯次数
        eeprom_read(28, (u8*)&g_sys_params.com_total, 4);
//        printf("com_total = %lu\r\n", g_sys_params.com_total);
        g_sys_params.com_total++;
        eeprom_write_sys(28, (u8*)&g_sys_params.com_total, 4);

        memset(s_report_handler.pkg_snd, 0 ,sizeof(s_report_handler.pkg_snd));//清零操作
        s_report_handler.pkg_snd_len = sizeof(s_report_handler.pkg_snd);
        //报文请求，发送
        s_report_handler.cmd = CTRL_CODE_REPORT;        // 强制执行上报
        task_report_request(s_report_handler.cmd, s_report_handler.pkg_snd, &(s_report_handler.pkg_snd_len));
        s_report_handler.sts = E_REPORT_SEND;//转为发送状态
        break;

    case E_REPORT_SEND://发送状态
        g_run_params.rf_send_start = 1;
        task_rf_triggered(ANT_CONNECTING);
        //printf("E_REPORT_SEND\r\n");
        rf_rcv_init();
#ifdef TASK_PRINTF
        MYLOG_DEBUG_HEXDUMP("RF Send:", s_report_handler.pkg_snd, s_report_handler.pkg_snd_len);
#endif
        rf_snd_data(s_report_handler.pkg_snd, s_report_handler.pkg_snd_len);//发送报文数据
        s_report_handler.timeout = MAX_RECV_TIMEOUT;//超时时间赋初值
        s_report_handler.sts = E_REPORT_RECV;//转为接收状态
        break;

    case E_REPORT_RECV://接收状态
        //printf("E_REPORT_RECV\r\n");
        s_report_handler.pkg_rcv = NULL;
        s_report_handler.pkg_rcv_len = 0;

        rv = rf_rcv_data(&(s_report_handler.pkg_rcv), &(s_report_handler.pkg_rcv_len));
        //printf("rv=%d\r\n" ,rv);
///* ******************************* */        rv = 0; // debug测试
        if (rv == 0)//如果接收到了
        {
            /* 首次上报温度时打印收到的数据到串口，方便生产调试，之后串口会关闭
            if (g_run_params.report_cnt == 0 && g_run_params.sts_login == 1)
            {
            u8 tmp[2] = {0};

            dec_2_hex_type(tmp, 2, g_run_params.temp_inlet, ORD_MOTOR);
            com_send_data(COM1, tmp, 2);
            com_send_data(COM1, s_report_handler.pkg_rcv, s_report_handler.pkg_rcv_len);
        }*/

#ifdef TASK_PRINTF
            MYLOG_DEBUG_HEXDUMP("RF Recv:", s_report_handler.pkg_rcv, s_report_handler.pkg_rcv_len);
#endif
            s_report_handler.sts = E_REPORT_DEAL;//转移为报文处理阶段
            break;
        }

        if (s_report_handler.timeout > 0)
        {
            s_report_handler.timeout--;
            if (s_report_handler.timeout == 0)
            {
                // 不会触发
                if (s_report_handler.cmd == CTRL_CODE_ACK)//如果cmd的状态为确认反馈
                {
                    task_report_triggered(CTRL_CODE_REPORT);
                    //s_report_handler.sts = E_REPORT_FINISH;//转为完成状态
                    break;
                }

                //printf("E_REPORT_RECV timeout\r\n");
                if (s_report_handler.retry_cnt < MAX_RETRY_CNT)//小于最大重试次数
                {
                    //printf("recv error,retry:%d\r\n", s_report_handler.retry_cnt);
                    s_report_handler.retry_cnt++;         //重试次数加一
                    s_report_handler.sts = E_REPORT_SEND;//返回发送状态
                }
                else
                {
                    task_rf_triggered(ANT_NO_SIGNAL);//如果收不到返回报文，程序就只执行到这了
                    //error_set(FAULT_COMM);
                    //printf("retry exceed max count:%d\r\n", s_report_handler.retry_cnt);
                    s_report_handler.sts = E_REPORT_FINISH;//转为完成状态
                }
            }
        }
        break;

    case E_REPORT_DEAL:
        //printf("E_REPORT_DEAL\r\n");
        //MYLOG_DEBUG_HEXDUMP("RF Recv:", s_report_handler.pkg_rcv, s_report_handler.pkg_rcv_len);
        //printf("s_report_handler.pkg_rcv_len=%d\r\n",s_report_handler.pkg_rcv_len);
        rv = cj188_unpack(&pkg, s_report_handler.pkg_rcv, s_report_handler.pkg_rcv_len); //进行解包
        //printf("rv=%d\r\n" ,rv);
        if (rv != 0)//解包没成功
        {
            if (s_report_handler.retry_cnt < MAX_RETRY_CNT)
            {
                //printf("Unpack error:%d,retry:%d\r\n",rv, s_report_handler.retry_cnt);
                s_report_handler.retry_cnt++;
                s_report_handler.sts = E_REPORT_SEND;
            }
            else
            {
                task_rf_triggered(ANT_NO_SIGNAL);
                s_report_handler.sts = E_REPORT_FINISH;//转为完成状态
            }

            break;
        }

        if (1 == g_run_params.sts_login)//如果为登陆状态
        {
            rv = app_report_response(&pkg, &g_sys_params, &g_run_params, &ack);
            if (rv == 0)
            {
                s_report_handler.sts = E_REPORT_SUCCESS; //转为成功状态
                break;
            }
            else
            {

                if (s_report_handler.retry_cnt < MAX_RETRY_CNT)
                {
#ifdef TASK_PRINTF
                    printf("report error,retry:%d\r\n",s_report_handler.retry_cnt);
#endif
                    s_report_handler.retry_cnt++;
                    s_report_handler.sts = E_REPORT_SEND;//转为发送
                    break;
                }
                else
                {
                    task_rf_triggered(ANT_NO_SIGNAL);
                    s_report_handler.sts = E_REPORT_FINISH;//转为完成状态
                    break;
                }
            }
        }
        else    // 不会触发
        {
            rv = app_login_response(&pkg, &g_sys_params, &g_run_params);//签到反馈
            //printf("login resp:%d\r\n", rv);
            if (rv == 0)
            {
                s_report_handler.sts = E_REPORT_SUCCESS;
                break;
            }
            else
            {
                if (s_report_handler.retry_cnt < MAX_RETRY_CNT)
                {
                    //printf("login error,retry:%d\r\n",s_report_handler.retry_cnt);
                    s_report_handler.retry_cnt++;
                    s_report_handler.sts = E_REPORT_SEND;
                    break;
                }
                else
                {
                    task_rf_triggered(ANT_NO_SIGNAL);
                    s_report_handler.sts = E_REPORT_FINISH;
                    break;
                }
            }
        }
        //s_report_handler.sts = E_REPORT_FINISH;
        break;
    case E_REPORT_SUCCESS:
        //error_clr(FAULT_COMM);
        task_rf_triggered(ANT_CONNECTRED);
        //printf("E_REPORT_SUCCESS\r\n");
        if (1 == g_run_params.sts_login)//如果为登陆状态
        {
            //printf("report ok\r\n");

            // 新增加通讯成功次数
            eeprom_read(24, (u8*)&g_sys_params.com_ok, 4);
//            printf("com_ok = %d\r\n", g_sys_params.com_ok);
            g_sys_params.com_ok++;

            // 保存配置参数
            eeprom_init();
            eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
            eeprom_close();

            //printf("eeprom ok\r\n");
            //printf("ack triggered!!!!!!!!!!!!!!!!!!!!=%d\r\n",ack);
            //发送ACK
//////            if (ack == 1)
//////            {
//////                //printf("ack triggered!\r\n");
//////                task_report_triggered(CTRL_CODE_ACK);//触发为确认反馈
//////                ack = 0;
//////                break;//当触发确认反馈时，不执行完成状态
//////            }

            s_report_handler.sts = E_REPORT_FINISH;//平时定时上报完要进入完成状态，等待下一次时间触发
        }
        else    // 不会触发
        {
            //printf("login ok\r\n");

            eeprom_init();
            eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
            eeprom_close();

            // 签到成功后立即启动温度上报
            g_run_params.sts_login = 1;
            task_report_triggered(CTRL_CODE_REPORT);  //把状态改变定时上报
        }
        break;
    case E_REPORT_FINISH:
        g_run_params.rf_send_start = 0;
        //printf("E_REPORT_FINISH\r\n");
        rf_close();
        rf_power_off();
        //GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
        s_report_handler.sts = E_REPORT_IDLE;   //转为空闲状态
        //printf("back to idle\r\n");
        break;
    default:
        break;
    }
}

/*! \brief
*   上报触发
* \param cmd[IN]       - 上报的方式选择
*
* \note
*
*/
void task_report_triggered(u8 cmd)
{
    if (1 == g_run_params.sts_login)
    {
        s_report_handler.cmd = cmd;
    }
    else
    {
        s_report_handler.cmd = CTRL_CODE_DEV_LOGIN;//设备签到
    }

    s_report_handler.sts = E_REPORT_READY;//转为准备模式
    s_report_handler.retry_cnt = 0;       //重试次数赋值为0
    s_report_handler.report_period_timer = (HALF_HOUR - 15*UNIT_SECOND);
    s_report_handler.report_nperiod_timer = (UNIT_DAY - 15*UNIT_SECOND);
}

/*! \brief
*   上报请求
* \param cmd[IN]           - 上报的方式选择
* \param pkg_snd[IN]       - 包的数据
* \param pkg_len[IN]       - 包的长度
*
* \note
*
*/
void task_report_request(u8 cmd, u8 *pkg_snd, u8 *pkg_len)
{
    CJ188Pkg pkg;

    memset(&pkg, 0, sizeof(pkg));

    //printf("cmd:%02X\r\n", cmd);

    /*request请求为发送打包、response响应为接收的判断*/

    switch(cmd)
    {
//    case CTRL_CODE_DEV_LOGIN://设备签到
//        app_login_request(&pkg, &g_sys_params, g_run_params);//pkg为变量，要取地址
//        break;
    case CTRL_CODE_REPORT://定时上报
        app_report_request(&pkg, &g_sys_params, &g_run_params);
        break;
    case CTRL_CODE_FACTORY://出厂配置
        app_factory_request(&pkg, &g_sys_params, g_run_params);
        break;
//    case CTRL_CODE_ACK://确认反馈
//        app_ack_request(&pkg, &g_sys_params, g_run_params);
//        break;
    default:
        break;
    }

    cj188_pack(&pkg, pkg_snd, pkg_len);//报文打包

    //MYLOG_DEBUG_HEXDUMP("RF Send:", pkg_snd, *pkg_len);

    return;
}

/*! \brief
*   触发时间的选择
*
* \note
*
*/
    //static struct tm t;
    u16 st=0, et=0, cur=0;
    //static int e = 0;  //如果不给变量赋初值，那么默认为0，这条语句是把局部变量变为全局变量，变量赋初值只执行一次

void task_report_schedule(void)
{
    if (g_sys_params.workflg == 0xAA)
    {
      s_report_handler.report_period_timer++;
      //半小时数据上传一次
      if(s_report_handler.report_period_timer >= HALF_HOUR)
      {
          s_report_handler.sts = E_REPORT_READY;
          s_report_handler.report_period_timer = 0;
      }
    }
    else
    {
      s_report_handler.report_nperiod_timer++;
      //非供暖期一天进行进行通讯一次
      if(s_report_handler.report_nperiod_timer >= UNIT_DAY)
      {
          s_report_handler.sts = E_REPORT_READY;
          s_report_handler.report_nperiod_timer = 0;
      }
    }



//////    struct tm t;
//////    u16 st=0, et=0, cur=0;
//////    static int e = 0;  //如果不给变量赋初值，那么默认为0，这条语句是把局部变量变为全局变量，变量赋初值只执行一次
////
////    s_report_handler.report_period_timer++;
////    //半小时数据上传一次
////    if(s_report_handler.report_period_timer >= HALF_HOUR)
////    {
////        rtc_read(&t);   //读时间
////
////        //    printf("%04d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min,t.tm_sec);
////        cur = (t.tm_mon+1)*100+t.tm_mday;   //计算出现在的时间 四位数“？？？？”
////        st  = bcd_2_dec_type(g_sys_params.heating_period_st, 2, ORD_MOTOR); //计算出供暖期开始时间 四位数“？？？？”
////        et  = bcd_2_dec_type(g_sys_params.heating_period_et, 2, ORD_MOTOR); //计算出供暖期结束时间 四位数“？？？？”
//////                printf("cur = %d\r\n" ,cur);
//////                printf("g_sys_params.heating_period_st = %d\r\n" ,g_sys_params.heating_period_st);
//////                printf("g_sys_params.heating_period_et = %d\r\n" ,g_sys_params.heating_period_et);
//////                printf("st = %d\r\n" ,st);
//////                printf("et = %d\r\n" ,et);
////
////        /* 供暖期内执行上报或签到 */
////        if (cur>=st || cur<=et)
////        {
////            e++;
////            //printf("e = %d\r\n", e);
////            /* 24小时设备签到一次（一次是半小时，*48为24小时）不到24小时为设备上报 */
////            if(e >= 2)
////            {
////                //printf("2222222222222222222\r\n");
////                g_run_params.sts_login = 0;
////                e = 0;
////                s_report_handler.report_period_timer = 0;
////                //s_report_handler.sts = E_REPORT_READY;
////                task_report_triggered(CTRL_CODE_DEV_LOGIN);
////            }
////            else
////            {
////                //printf("iiiiiiiiiiiiiiiiii\r\n");
////                s_report_handler.sts = E_REPORT_READY;
////                s_report_handler.report_period_timer = 0;
////            }
////        }
////        else
////        {
////            if (s_report_handler.report_period_timer >= UNIT_DAY)
////            {
////                //printf("333333333333333333333333333\r\n");
////                g_run_params.sts_login = 0;
////                s_report_handler.report_period_timer = 0;
////                //s_report_handler.sts = E_REPORT_READY;
////                task_report_triggered(CTRL_CODE_DEV_LOGIN);
////            }
////            //printf("system is not in period,timer:%ld\r\n", s_schedule_handler.timer);
////        }
////    }
}

/*! \brief
*   上报初始化
*
* \note
*
*/
void task_report_init(void)
{
    s_report_handler.report_period_timer = (HALF_HOUR - 3*THREE_SECOND);       // 供暖期3s后进行上报
    s_report_handler.report_nperiod_timer = (UNIT_DAY - 3*THREE_SECOND);       // 非供暖期3s后进行上报
    //s_report_handler.switch_login_timer = UNIT_DAY-1;
//    s_report_handler.cmd = CTRL_CODE_DEV_LOGIN;
}
