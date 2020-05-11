#include "adf.h"
#include "task_config.h"
#include "sysparams.h"
#include "modbus.h"
#include "rf.h"
#include "delay.h"
#include "cj188.h"
#include "protocol.h"
#include "task_report.h"

/*! \brief
*      ����������
*
* \note
*
*/

//u8 rv = 0;
//    u8 *rcv_dat = NULL;
//    u8 rcv_len = 0;
//    u8  pkg_dat[64] = {0};
//    u8  snd_len = 0;
void task_config_proc_test(void)
{
    u8 rv = 0;
    u8 *rcv_dat = NULL;
    u8 rcv_len = 0;
    u8  pkg_dat[64] = {0};
    u8  snd_len = 0;

    if(g_run_params.set_id == 1)
    {
//        rv = rf_get_recv_flag();
        //printf("flag:%d\r\n", rv);
//        if (rv == E_RF_RX_FLAG_RECV_COMPLETE)
//        {
            rv = rf_rcv_data(&rcv_dat, &rcv_len);
            //delay_ms(40);
            //printf("rv111111111111 = %d\r\n", rv );
            if(rv == 0)
            {
//                MYLOG_DEBUG_HEXDUMP("RF Config Recv:", rcv_dat, rcv_len);
                rv = params_service(pkg_dat, &snd_len, rcv_dat, rcv_len);
                //printf("rv111111111111 = %d\r\n", rv );
                if (rv == 0)
                {
                    rf_snd_data(pkg_dat, snd_len);
//                    MYLOG_DEBUG_HEXDUMP("RF Config Send:", pkg_dat, snd_len);
                }
                rf_rcv_init();
            }
    }
}

static u8 rv = 0;
u8 *rcv_dat = NULL;
u16 rcv_len = 0;
u8  rcv_rflen = 0;
u8  pkg_dat[64] = {0};
u8  snd_len = 0;
static CJ188Pkg pkg;//������һ���ṹ�����pkg
static u8 ack = 0;
void task_config_proc(void)
{
//    u8 rv = 0;
//    u8 *rcv_dat = NULL;
//    u16 rcv_len = 0;
//    u8  rcv_rflen = 0;
//    u8  pkg_dat[64] = {0};
//    u8  snd_len = 0;

    //    u8  pkg_buf[64] = {0};
    //    u8  pkg_len = 0;

    //printf("params_service = %d\r\n" ,com_recv_data(COM1, &rcv_dat, &rcv_len));

    // ����豸�ѳ�����ͨѶ��������1�Σ���رմ���
    //printf("g_sys_params.factory_flag = %x\r\n" ,g_sys_params.factory_flag);
    if (g_sys_params.factory_flag == 0x01)
    {
        //        printf("report cnt:%d\r\n", g_run_params.report_cnt);
        //        if (g_run_params.report_cnt >= 1)
        //        {
        //printf("device is not in factory mode\r\n");
        //com_close(COM1);
        //        }

        return;
    }

    //printf("sysparams setting\r\n");
//    rv = com_recv_data(COM1, &rcv_dat, &rcv_len);
//    if (rv == 0)
//    {
//      if (rcv_len > 0)
//      {
//        printf("params_service\r\n");
//        rv = params_service(pkg_dat, &snd_len, rcv_dat, rcv_len);   //MODBUSЭ�鴦��
//#if TASK_PRINTF
//        MYLOG_DEBUG_HEXDUMP("COM Config rev:", rcv_dat, rcv_len);
//#endif
//        rv = cj188_unpack(&pkg, rcv_dat, rcv_len); //���н��
//        printf("cj188_unpack rv = %d\r\n", rv);
//        if (rv == 0)
//        {
//            printf("COM Config cj188_unpack ok\r\n");
//            rv = app_factory_report_response(&pkg, &g_sys_params, &g_run_params, &ack);
//            printf("app_factory_report_response rv = %d\r\n", rv);
//            if (rv == 0)
//            {
//                  eeprom_write_sys(2, &g_sys_params.Device_GN, 2);
//                  eeprom_write_sys(4, &g_sys_params.Device_SN, 4);
//                  task_report_request(CTRL_CODE_FACTORY, pkg_dat, &snd_len);
//                  MYLOG_DEBUG_HEXDUMP("COM Config Send:", pkg_dat, snd_len);
//                  com_send_data(COM1, pkg_dat, snd_len);

//        }
//


//      com_recv_init(COM1);
//    }

    //////return;



    if(g_run_params.set_id == 1)
    {
//        rv = rf_get_recv_flag();
        //printf("flag:%d\r\n", rv);
//        if (rv == E_RF_RX_FLAG_RECV_COMPLETE)
//        {
            rv = rf_rcv_data(&rcv_dat, &rcv_rflen);
            //delay_ms(40);
            //printf("rv111111111111 = %d\r\n", rv );
            if(rv == 0)
            {
                //rf_snd_data(rcv_dat, rcv_rflen);//����ʲô��������
//                MYLOG_DEBUG_HEXDUMP("RF Config Recv:", rcv_dat, rcv_len);
//                rv = params_service(pkg_dat, &snd_len, rcv_dat, rcv_rflen);
                rv = cj188_unpack(&pkg, rcv_dat, rcv_rflen); //���н��
                //printf("rv111111111111 = %d\r\n", rv );
                if (rv == 0)
                {

                    rv = app_factory_report_response(&pkg, &g_sys_params, &g_run_params, &ack);
                    //rf_snd_data(&rv, 1);
                    if (rv == 0)
                    {
                          eeprom_write_sys(2, &g_sys_params.Device_GN, 2);
                          eeprom_write_sys(4, &g_sys_params.Device_SN, 4);
                          task_report_request(CTRL_CODE_FACTORY, pkg_dat, &snd_len);
                          rf_snd_data(pkg_dat, snd_len);
                          //MYLOG_DEBUG_HEXDUMP("RF Config Send:", pkg_dat, snd_len);
                    }
                }
                 rf_rcv_init();
            }
    }

//////    if(g_run_params.set_id == 1)
//////    {
////////        rv = rf_get_recv_flag();
//////        //printf("flag:%d\r\n", rv);
////////        if (rv == E_RF_RX_FLAG_RECV_COMPLETE)
////////        {
//////            rv = rf_rcv_data(&rcv_dat, &rcv_rflen);
//////            //delay_ms(40);
//////            //printf("rv111111111111 = %d\r\n", rv );
//////            if(rv == 0)
//////            {
////////                MYLOG_DEBUG_HEXDUMP("RF Config Recv:", rcv_dat, rcv_len);
//////                rv = params_service(pkg_dat, &snd_len, rcv_dat, rcv_rflen);
//////                //printf("rv111111111111 = %d\r\n", rv );
//////                if (rv == 0)
//////                {
//////                    rf_snd_data(pkg_dat, snd_len);
//////                    //MYLOG_DEBUG_HEXDUMP("RF Config Send:", pkg_dat, snd_len);
//////                }
//////            }
//////        rf_rcv_init();
//////    }
}

/*! \brief
*   ��ȡ�Ĵ���ֵ
* \param reg_val[OUT]       - �Ĵ�������
* \param reg_len[OUT]       - �Ĵ������ݳ���
* \param reg_st[IN]         - �Ĵ�����ʼ��ַ
* \param reg_cnt[IN]        - �Ĵ�������
*
* \note
*   ϵͳ�����ṹ��ָ�롢��ֵ��¼�ṹ��ָ��ʵ����ӳ���ַ��STATIC_BUFFER
*/
u8 params_read_reg(u8 *reg_val, u8 *reg_len, u16 reg_st, u16 reg_cnt)
{
    u8 i = 0;
    u8 offset = 0;

    for (i=reg_st; i<reg_st+reg_cnt; ++i)   //����ʼ�����ݳ��ȵĸ���
    {
        reg_val[offset++] = ((u8*)&g_sys_params)[i*2];
        reg_val[offset++] = ((u8*)&g_sys_params)[i*2+1];
    }

//    memcpy(reg_val ,((u8*)&g_sys_params+reg_st*2) ,reg_cnt*2);
//    *reg_len = reg_cnt*2;//��dat_len�����ֵ���ı���
    *reg_len = offset;
    return 0;
}

/*! \brief
*   ���¼Ĵ���ֵ
* \param reg_val[IN]       - �Ĵ�������
* \param reg_len[IN]       - �Ĵ������ݳ���
* \param reg_st[IN]         - �Ĵ�����ʼ��ַ
* \param reg_cnt[IN]        - �Ĵ�������
*
* \note
*   ϵͳ�����ṹ��ָ�롢��ֵ��¼�ṹ��ָ��ʵ����ӳ���ַ��STATIC_BUFFER
*/
void params_write_reg_multi(u8 *reg_val, u8 reg_len, u16 reg_st, u16 reg_cnt)
{
    //    u8 i = 0;
    //    u8 offset = 0;
    //
    eeprom_init();
    //    for (i=reg_st; i<reg_st+reg_cnt; ++i)
    //    {
    //        ((u8*)&g_sys_params)[i*2+0] = reg_val[offset++];
    //        ((u8*)&g_sys_params)[i*2+1] = reg_val[offset++];
    //        eeprom_write(i*2, ((u8*)&g_sys_params)+i*2, 2);
    //    }
    memcpy(((u8*)&g_sys_params+reg_st*2) ,reg_val ,reg_cnt*2);
    eeprom_write(reg_st*2, ((u8*)&g_sys_params)+reg_st*2, reg_cnt*2);
    eeprom_close();
}

/*! \brief
*      MODBUSЭ�鴦��
* \param snd_buf[OUT]       - MODBUS��������
* \param snd_len[OUT]       - MODBUS�������ĳ���
* \param rcv_buf[IN]        - MODBUS������
* \param rcv_len[IN]        - MODBUS�����ĳ���
*
* \note
*
*/
u8 params_service(u8 *snd_buf, u8 *snd_len, u8 *rcv_buf, u8 rcv_len)
{
    u8 *dat_ptr = NULL;
    u8  dat_len = 0;

    u8  rv = 0;
    u8  addr = 0;
    u8  cmd  = 0;

    u16 reg_st = 0;
    u16 reg_cnt = 0;

    // MODBUSЭ����
    rv = modbus_unpack(rcv_buf, rcv_len, &addr, &cmd, &dat_ptr, &dat_len);

    if (rv != 0)    //����д���
    {
        return rv;
    }

    // ҵ����
    switch(cmd)
    {
    case MODBUS_CMD_READ_MULTI:		// ���Ĵ���
        //        if (addr != 0x00)
        //        {
        //            printf("MODBUS_ERR_ADDR\r\n");
        //            return MODBUS_ERR_ADDR;   //���ȴ���
        //        }

        if (dat_len != 4)
        {
            return MODBUS_ERR_LENGTH;   //���ȴ���
        }

        reg_st  = dat_ptr[0]<<8 | dat_ptr[1];//dat_ptrΪ����������0��1����Ĵ�������ʼ��ַ
        reg_cnt = dat_ptr[2]<<8 | dat_ptr[3];//dat_ptrΪ����������2��3����Ĵ���������

        dat_len = 0;
        dat_ptr = rcv_buf+2;    //�����ļ�2�������������

        // ���Ĵ���
        //����params_read_reg�ܸı�dat_ptr+1��ֵ
        //dat_ptr+1��Ϊ�������Ĵ������ȵ����������׵�ַ
        params_read_reg(dat_ptr+1, &dat_len, reg_st, reg_cnt);//dat_lenΪ������ĳ���

        // �����ֽ�
        // MODSCAN32֧�֣�MODSCAN32�������ֽڼ��ʱ��̫�������½��������쳣�����Ը���MODBUS POLL���ߣ�
        dat_ptr[0] = dat_len/2; //�Ĵ������ݵĳ��ȴ�С��Ϊ���ݵ��׵�ַ
        //dat_ptr[0] = dat_len; //�Ĵ������ݵĳ��ȴ�С��Ϊ���ݵ��׵�ַ
        // MODBUS POLL֧��
        //dat_ptr[0] = dat_len;
        dat_len += 1;//dat_lenΪ6����һΪ7
        //printf("dat_len = %d\r\n" ,dat_len);
        //MYLOG_DEBUG_HEXDUMP("Reg Value:", dat_ptr, dat_len);
        break;
    case MODBUS_CMD_WRITE_SINGLE:	 // д�����Ĵ���
        if (dat_len != 4)
        {
            return MODBUS_ERR_LENGTH;
        }

        reg_st = dat_ptr[0]<<8 | dat_ptr[1];
        // д�Ĵ���
        params_write_reg_multi(dat_ptr+2, 2, reg_st, 1);//�����dat_ptr+2�����ǵ�ַ�Ӷ�Ϊ�Ĵ�������ֵ����ֵ���ڶ���2û��
        // �������ݰ�Ϊԭʼ���ݰ�
        dat_len = 4;
        break;
    case MODBUS_CMD_WRITE_MULTI:	// д����Ĵ���
        if (dat_len != (dat_ptr[4]+5))//�������dat_ptr[4]Ϊ�ֽ������ټ���ǰ���5λ֤��Ϊ������ĳ���
        {
            return MODBUS_ERR_LENGTH;
        }

        reg_st  = dat_ptr[0]<<8 | dat_ptr[1];
        reg_cnt = dat_ptr[2]<<8 | dat_ptr[3];

        if (reg_cnt*2 != dat_ptr[4])
        {
            return MODBUS_ERR_LENGTH;
        }

        // д�Ĵ�������
        params_write_reg_multi(dat_ptr+5, dat_len-5, reg_st, reg_cnt);

        // �������ݰ�Ϊԭʼ���ݰ�������ǰ4�ֽڡ�
        dat_len = 4;
        break;
    default:
        return MODBUS_ERR_INVALID_CMD;
        break;
    }

    // ���(snd_buf,snd_len����������������ɺ����ĸ������Լ���϶���)
    modbus_pack(snd_buf, snd_len, addr, cmd, dat_ptr, dat_len);

    return 0;
}

void factory_check(void)
{

    if (memcmp(&g_sys_params.Device_SN, "\x00\x00\x00\x00", 4)!=0 /*&& memcmp(&g_sys_params.Device_GN, "\x00\x00", 2)!=0*/)   //��Ҫ����������
    {
        //printf("g_sys_params.factory_flag = %x\r\n" ,g_sys_params.factory_flag);
        g_sys_params.factory_flag = 1;
        //printf("g_sys_params.factory_flag = %x\r\n" ,g_sys_params.factory_flag);

        g_sys_params.temp_set = 2500;
        g_run_params.temp_return = g_sys_params.temp_set;
        //printf("g_sys_params.temp_set = %d\r\n" ,g_sys_params.temp_set);

        //u8 addr[5] = {0x90,0x05,0x00,0x01,0x01};//����һ������
        //u8 addr[5] = {0x01,0x01,0x00,0x05,0x90};//����һ������
        //memcpy(g_sys_params.addr, addr, sizeof(g_sys_params.addr)/sizeof(g_sys_params.addr[0]));
        //printf("g_sys_params.addr = %x\r\n" ,g_sys_params.addr);

        rf_close();
        rf_power_off();

        eeprom_init();
        eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
        eeprom_close();
//        com_close(COM1);
    }
////    if (g_sys_params.factory_flag == 0
////        && memcmp(g_sys_params.addr, "\x00\x00\x00\x00\x00", 5)!=0
////            && memcmp(g_sys_params.addr, "\x00\x00\x00\x00\x01", 5)!=0
////            && memcmp(g_sys_params.addr, "\x41\x23\x00\x01\x01", 5)!=0
////            && memcmp(g_sys_params.addr, "\x41\x24\x00\x01\x01", 5)!=0)   //��Ҫ����������
////    {
////        //printf("g_sys_params.factory_flag = %x\r\n" ,g_sys_params.factory_flag);
////        g_sys_params.factory_flag = 1;
////        //printf("g_sys_params.factory_flag = %x\r\n" ,g_sys_params.factory_flag);
////
////        g_sys_params.temp_set = 250;
////        g_run_params.temp_return = g_sys_params.temp_set;
////        //printf("g_sys_params.temp_set = %d\r\n" ,g_sys_params.temp_set);
////
////        //u8 addr[5] = {0x90,0x05,0x00,0x01,0x01};//����һ������
////        //u8 addr[5] = {0x01,0x01,0x00,0x05,0x90};//����һ������
////        //memcpy(g_sys_params.addr, addr, sizeof(g_sys_params.addr)/sizeof(g_sys_params.addr[0]));
////        //printf("g_sys_params.addr = %x\r\n" ,g_sys_params.addr);
////
////        rf_close();
////        rf_power_off();
////
////        eeprom_init();
////        eeprom_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));
////        eeprom_close();
////        com_close(COM1);
////    }
}