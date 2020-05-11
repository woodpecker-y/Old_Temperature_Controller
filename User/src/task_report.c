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
*   �ϱ�������
*
* \note
*
*/
//u8  rv  = 0;
static CJ188Pkg pkg;//������һ���ṹ�����pkg

void task_report_proc(void)
{
    u8  rv  = 0;
    //CJ188Pkg pkg;//������һ���ṹ�����pkg

    switch(s_report_handler.sts)
    {
    case E_REPORT_IDLE://����״̬
        task_report_schedule();
        break;
    case E_REPORT_READY://׼��״̬
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
                s_report_handler.sts = E_REPORT_PACKAGE;//תΪ���״̬
            }
            else
            {
                s_report_handler.sts = E_REPORT_IDLE;//תΪ����״̬
                //printf("rf_init error[%d]\r\n", rv);
            }
        }
        else
        {
            s_report_handler.sts = E_REPORT_IDLE;//תΪ����״̬
        }
        break;

    case E_REPORT_PACKAGE://���״̬
        //printf("E_REPORT_PACKAGE\r\n");
        // ������ͨѶ����
        eeprom_read(28, (u8*)&g_sys_params.com_total, 4);
//        printf("com_total = %lu\r\n", g_sys_params.com_total);
        g_sys_params.com_total++;
        eeprom_write_sys(28, (u8*)&g_sys_params.com_total, 4);

        memset(s_report_handler.pkg_snd, 0 ,sizeof(s_report_handler.pkg_snd));//�������
        s_report_handler.pkg_snd_len = sizeof(s_report_handler.pkg_snd);
        //�������󣬷���
        s_report_handler.cmd = CTRL_CODE_REPORT;        // ǿ��ִ���ϱ�
        task_report_request(s_report_handler.cmd, s_report_handler.pkg_snd, &(s_report_handler.pkg_snd_len));
        s_report_handler.sts = E_REPORT_SEND;//תΪ����״̬
        break;

    case E_REPORT_SEND://����״̬
        g_run_params.rf_send_start = 1;
        task_rf_triggered(ANT_CONNECTING);
        //printf("E_REPORT_SEND\r\n");
        rf_rcv_init();
#ifdef TASK_PRINTF
        MYLOG_DEBUG_HEXDUMP("RF Send:", s_report_handler.pkg_snd, s_report_handler.pkg_snd_len);
#endif
        rf_snd_data(s_report_handler.pkg_snd, s_report_handler.pkg_snd_len);//���ͱ�������
        s_report_handler.timeout = MAX_RECV_TIMEOUT;//��ʱʱ�丳��ֵ
        s_report_handler.sts = E_REPORT_RECV;//תΪ����״̬
        break;

    case E_REPORT_RECV://����״̬
        //printf("E_REPORT_RECV\r\n");
        s_report_handler.pkg_rcv = NULL;
        s_report_handler.pkg_rcv_len = 0;

        rv = rf_rcv_data(&(s_report_handler.pkg_rcv), &(s_report_handler.pkg_rcv_len));
        //printf("rv=%d\r\n" ,rv);
///* ******************************* */        rv = 0; // debug����
        if (rv == 0)//������յ���
        {
            /* �״��ϱ��¶�ʱ��ӡ�յ������ݵ����ڣ������������ԣ�֮�󴮿ڻ�ر�
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
            s_report_handler.sts = E_REPORT_DEAL;//ת��Ϊ���Ĵ����׶�
            break;
        }

        if (s_report_handler.timeout > 0)
        {
            s_report_handler.timeout--;
            if (s_report_handler.timeout == 0)
            {
                // ���ᴥ��
                if (s_report_handler.cmd == CTRL_CODE_ACK)//���cmd��״̬Ϊȷ�Ϸ���
                {
                    task_report_triggered(CTRL_CODE_REPORT);
                    //s_report_handler.sts = E_REPORT_FINISH;//תΪ���״̬
                    break;
                }

                //printf("E_REPORT_RECV timeout\r\n");
                if (s_report_handler.retry_cnt < MAX_RETRY_CNT)//С��������Դ���
                {
                    //printf("recv error,retry:%d\r\n", s_report_handler.retry_cnt);
                    s_report_handler.retry_cnt++;         //���Դ�����һ
                    s_report_handler.sts = E_REPORT_SEND;//���ط���״̬
                }
                else
                {
                    task_rf_triggered(ANT_NO_SIGNAL);//����ղ������ر��ģ������ִֻ�е�����
                    //error_set(FAULT_COMM);
                    //printf("retry exceed max count:%d\r\n", s_report_handler.retry_cnt);
                    s_report_handler.sts = E_REPORT_FINISH;//תΪ���״̬
                }
            }
        }
        break;

    case E_REPORT_DEAL:
        //printf("E_REPORT_DEAL\r\n");
        //MYLOG_DEBUG_HEXDUMP("RF Recv:", s_report_handler.pkg_rcv, s_report_handler.pkg_rcv_len);
        //printf("s_report_handler.pkg_rcv_len=%d\r\n",s_report_handler.pkg_rcv_len);
        rv = cj188_unpack(&pkg, s_report_handler.pkg_rcv, s_report_handler.pkg_rcv_len); //���н��
        //printf("rv=%d\r\n" ,rv);
        if (rv != 0)//���û�ɹ�
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
                s_report_handler.sts = E_REPORT_FINISH;//תΪ���״̬
            }

            break;
        }

        if (1 == g_run_params.sts_login)//���Ϊ��½״̬
        {
            rv = app_report_response(&pkg, &g_sys_params, &g_run_params, &ack);
            if (rv == 0)
            {
                s_report_handler.sts = E_REPORT_SUCCESS; //תΪ�ɹ�״̬
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
                    s_report_handler.sts = E_REPORT_SEND;//תΪ����
                    break;
                }
                else
                {
                    task_rf_triggered(ANT_NO_SIGNAL);
                    s_report_handler.sts = E_REPORT_FINISH;//תΪ���״̬
                    break;
                }
            }
        }
        else    // ���ᴥ��
        {
            rv = app_login_response(&pkg, &g_sys_params, &g_run_params);//ǩ������
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
        if (1 == g_run_params.sts_login)//���Ϊ��½״̬
        {
            //printf("report ok\r\n");

            // ������ͨѶ�ɹ�����
            eeprom_read(24, (u8*)&g_sys_params.com_ok, 4);
//            printf("com_ok = %d\r\n", g_sys_params.com_ok);
            g_sys_params.com_ok++;

            // �������ò���
            eeprom_init();
            eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
            eeprom_close();

            //printf("eeprom ok\r\n");
            //printf("ack triggered!!!!!!!!!!!!!!!!!!!!=%d\r\n",ack);
            //����ACK
//////            if (ack == 1)
//////            {
//////                //printf("ack triggered!\r\n");
//////                task_report_triggered(CTRL_CODE_ACK);//����Ϊȷ�Ϸ���
//////                ack = 0;
//////                break;//������ȷ�Ϸ���ʱ����ִ�����״̬
//////            }

            s_report_handler.sts = E_REPORT_FINISH;//ƽʱ��ʱ�ϱ���Ҫ�������״̬���ȴ���һ��ʱ�䴥��
        }
        else    // ���ᴥ��
        {
            //printf("login ok\r\n");

            eeprom_init();
            eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
            eeprom_close();

            // ǩ���ɹ������������¶��ϱ�
            g_run_params.sts_login = 1;
            task_report_triggered(CTRL_CODE_REPORT);  //��״̬�ı䶨ʱ�ϱ�
        }
        break;
    case E_REPORT_FINISH:
        g_run_params.rf_send_start = 0;
        //printf("E_REPORT_FINISH\r\n");
        rf_close();
        rf_power_off();
        //GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
        s_report_handler.sts = E_REPORT_IDLE;   //תΪ����״̬
        //printf("back to idle\r\n");
        break;
    default:
        break;
    }
}

/*! \brief
*   �ϱ�����
* \param cmd[IN]       - �ϱ��ķ�ʽѡ��
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
        s_report_handler.cmd = CTRL_CODE_DEV_LOGIN;//�豸ǩ��
    }

    s_report_handler.sts = E_REPORT_READY;//תΪ׼��ģʽ
    s_report_handler.retry_cnt = 0;       //���Դ�����ֵΪ0
    s_report_handler.report_period_timer = (HALF_HOUR - 15*UNIT_SECOND);
    s_report_handler.report_nperiod_timer = (UNIT_DAY - 15*UNIT_SECOND);
}

/*! \brief
*   �ϱ�����
* \param cmd[IN]           - �ϱ��ķ�ʽѡ��
* \param pkg_snd[IN]       - ��������
* \param pkg_len[IN]       - ���ĳ���
*
* \note
*
*/
void task_report_request(u8 cmd, u8 *pkg_snd, u8 *pkg_len)
{
    CJ188Pkg pkg;

    memset(&pkg, 0, sizeof(pkg));

    //printf("cmd:%02X\r\n", cmd);

    /*request����Ϊ���ʹ����response��ӦΪ���յ��ж�*/

    switch(cmd)
    {
//    case CTRL_CODE_DEV_LOGIN://�豸ǩ��
//        app_login_request(&pkg, &g_sys_params, g_run_params);//pkgΪ������Ҫȡ��ַ
//        break;
    case CTRL_CODE_REPORT://��ʱ�ϱ�
        app_report_request(&pkg, &g_sys_params, &g_run_params);
        break;
    case CTRL_CODE_FACTORY://��������
        app_factory_request(&pkg, &g_sys_params, g_run_params);
        break;
//    case CTRL_CODE_ACK://ȷ�Ϸ���
//        app_ack_request(&pkg, &g_sys_params, g_run_params);
//        break;
    default:
        break;
    }

    cj188_pack(&pkg, pkg_snd, pkg_len);//���Ĵ��

    //MYLOG_DEBUG_HEXDUMP("RF Send:", pkg_snd, *pkg_len);

    return;
}

/*! \brief
*   ����ʱ���ѡ��
*
* \note
*
*/
    //static struct tm t;
    u16 st=0, et=0, cur=0;
    //static int e = 0;  //���������������ֵ����ôĬ��Ϊ0����������ǰѾֲ�������Ϊȫ�ֱ�������������ִֵֻ��һ��

void task_report_schedule(void)
{
    if (g_sys_params.workflg == 0xAA)
    {
      s_report_handler.report_period_timer++;
      //��Сʱ�����ϴ�һ��
      if(s_report_handler.report_period_timer >= HALF_HOUR)
      {
          s_report_handler.sts = E_REPORT_READY;
          s_report_handler.report_period_timer = 0;
      }
    }
    else
    {
      s_report_handler.report_nperiod_timer++;
      //�ǹ�ů��һ����н���ͨѶһ��
      if(s_report_handler.report_nperiod_timer >= UNIT_DAY)
      {
          s_report_handler.sts = E_REPORT_READY;
          s_report_handler.report_nperiod_timer = 0;
      }
    }



//////    struct tm t;
//////    u16 st=0, et=0, cur=0;
//////    static int e = 0;  //���������������ֵ����ôĬ��Ϊ0����������ǰѾֲ�������Ϊȫ�ֱ�������������ִֵֻ��һ��
////
////    s_report_handler.report_period_timer++;
////    //��Сʱ�����ϴ�һ��
////    if(s_report_handler.report_period_timer >= HALF_HOUR)
////    {
////        rtc_read(&t);   //��ʱ��
////
////        //    printf("%04d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min,t.tm_sec);
////        cur = (t.tm_mon+1)*100+t.tm_mday;   //��������ڵ�ʱ�� ��λ��������������
////        st  = bcd_2_dec_type(g_sys_params.heating_period_st, 2, ORD_MOTOR); //�������ů�ڿ�ʼʱ�� ��λ��������������
////        et  = bcd_2_dec_type(g_sys_params.heating_period_et, 2, ORD_MOTOR); //�������ů�ڽ���ʱ�� ��λ��������������
//////                printf("cur = %d\r\n" ,cur);
//////                printf("g_sys_params.heating_period_st = %d\r\n" ,g_sys_params.heating_period_st);
//////                printf("g_sys_params.heating_period_et = %d\r\n" ,g_sys_params.heating_period_et);
//////                printf("st = %d\r\n" ,st);
//////                printf("et = %d\r\n" ,et);
////
////        /* ��ů����ִ���ϱ���ǩ�� */
////        if (cur>=st || cur<=et)
////        {
////            e++;
////            //printf("e = %d\r\n", e);
////            /* 24Сʱ�豸ǩ��һ�Σ�һ���ǰ�Сʱ��*48Ϊ24Сʱ������24СʱΪ�豸�ϱ� */
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
*   �ϱ���ʼ��
*
* \note
*
*/
void task_report_init(void)
{
    s_report_handler.report_period_timer = (HALF_HOUR - 3*THREE_SECOND);       // ��ů��3s������ϱ�
    s_report_handler.report_nperiod_timer = (UNIT_DAY - 3*THREE_SECOND);       // �ǹ�ů��3s������ϱ�
    //s_report_handler.switch_login_timer = UNIT_DAY-1;
//    s_report_handler.cmd = CTRL_CODE_DEV_LOGIN;
}