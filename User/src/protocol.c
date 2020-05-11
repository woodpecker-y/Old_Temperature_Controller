#include <stdio.h>
#include "adf.h"
#include "sysparams.h"
#include "cj188.h"
#include "protocol.h"

char room_name[7] = {0};

/*
函数名称:LittleANDBig_ModeCON2
函数功能:对一个INT16U数据进行大小端模式切换

*/
u16 LittleANDBig_ModeCON2(u16*Input)
{
	u16 DATAOUT =0;
	u16 DATAIN =*Input;
	DATAOUT =((DATAIN&0X00FF )<<8)|((DATAIN&0XFF00 )>>8);
	return DATAOUT;
}


/*
函数名称:LittleANDBig_ModeCON4
函数功能:对一个INT32U数据进行大小端模式切换

*/
u32 LittleANDBig_ModeCON4(u32*Input)
{
	u32 DATAOUT =0;
	u32 DATAIN =*Input;

	DATAOUT =((DATAIN&0X000000FF )<<24)|((DATAIN&0X0000FF00 )<<8)|((DATAIN&0X00FF0000 )>>8)|((DATAIN&0XFF000000 )>>24);
	return DATAOUT;
}

/*! \brief
*      签到请求
* \param pkg[IN]            - cj188的参数
* \param sys_params[IN]     - 系统参数
* \param run_params[IN]     - 运行参数
*
* \note
*
*/
u8 app_login_request(CJ188Pkg *pkg, const SysParams *sys_params, RunParams run_params)
{
    // 仪表地址
    memcpy(pkg->addr, sys_params->addr, sizeof(pkg->addr));
    // 控制码
    pkg->ctrl_code = CTRL_CODE_DEV_LOGIN;
    // 数据域长度
    pkg->msg_size  = 0x00;

    return 0;
}

/*! \brief
*      签到反馈
* \param pkg[IN]            - cj188的参数
* \param sys_params[IN]     - 系统参数
* \param run_params[IN]     - 运行参数
*
* \note
*
*/
u8 app_login_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params)
{
////    //char room_name[7] = {0};
////
////    // 地址码检查
////    if (memcmp(pkg->addr, sys_params->addr, sizeof(pkg->addr)) != 0)
////    {
////        return CJ188_ERR_ADDR;
////    }
////
////    // 控制码
////    if (pkg->ctrl_code != (CTRL_CODE_DEV_LOGIN | 0x80))
////    {
////        return CJ188_ERR_CTRL_CODE;
////    }
////
////#ifdef DEBUG
////    //MYLOG_DEBUG_HEXDUMP("MsgData:", pkg->msg_data, pkg->msg_size);
////#endif
////
////    // 供暖起始时间
////    sys_params->heating_period_st[0] = pkg->msg_data[1];
////    sys_params->heating_period_st[1] = pkg->msg_data[0];
////    // 供暖结束时间
////    sys_params->heating_period_et[0] = pkg->msg_data[3];
////    sys_params->heating_period_et[1] = pkg->msg_data[2];
////    //    printf("g_sys_params.heating_period_st = %02x\r\n" ,g_sys_params.heating_period_st);
////    //    printf("g_sys_params.heating_period_et = %02x\r\n" ,g_sys_params.heating_period_et);
////    // 温度设定上限
////    sys_params->temp_room_uplimits   = pkg->msg_data[4];
////    // 温度设定下限
////    sys_params->temp_room_lowlimits  = pkg->msg_data[5];
////    // 采样周期(此算法为字节倒叙，把包里的intel字节序，转换过来，例：3C 00转换为00 3C正好为1分钟)
////    //sys_params->cycle_sample = pkg->msg_data[7]<<8 | pkg->msg_data[6];
////    sys_params->cycle_sample = bcd_2_dec_type(&(pkg->msg_data[6]), 2, ORD_INTEL);
////    // 余额警告值
////    sys_params->balance_alarm        = pkg->msg_data[8];
////    // 显示控制位
////    sys_params->disp_ctl             = pkg->msg_data[9];
////    // 房间编号
////    if (pkg->msg_data[12]>=0x10 && pkg->msg_data[11]>=0x10)
////    {
////        //sprintf(room_name, "%X%02X", pkg->msg_data[11], pkg->msg_data[10]);
////        room_name[0] = (pkg->msg_data[11]>>4)+'0';
////        room_name[1] = (pkg->msg_data[11]%0x0f)+'0';
////        room_name[2] = (pkg->msg_data[10]>>4)+'0';
////        room_name[3] = (pkg->msg_data[10]%0x0f)+'0';
////        memcpy(sys_params->room_name, room_name, sizeof(sys_params->room_name));
////    }
////    else
////    {
////        //sprintf(room_name, "%X-%X%02X", pkg->msg_data[12], pkg->msg_data[11], pkg->msg_data[10]);
////        room_name[0] = (pkg->msg_data[12]%0x0f)+'0';
////        room_name[1] = '-';
////        if ((pkg->msg_data[11]>>4) > 0)
////        {
////            room_name[2] = (pkg->msg_data[11]>>4)+'0';
////            room_name[3] = (pkg->msg_data[11]%0x0f)+'0';
////            room_name[4] = (pkg->msg_data[10]>>4)+'0';
////            room_name[5] = (pkg->msg_data[10]%0x0f)+'0';
////        }
////        else
////        {
////            room_name[2] = (pkg->msg_data[11]%0x0f)+'0';
////            room_name[3] = (pkg->msg_data[10]>>4)+'0';
////            room_name[4] = (pkg->msg_data[10]%0x0f)+'0';
////            room_name[5] = '\0';
////        }
////
////        memcpy(sys_params->room_name, room_name, sizeof(sys_params->room_name));
////    }
////
////    //    printf("g_sys_params.temp_room_uplimits = %d\r\n" ,g_sys_params.temp_room_uplimits);
////    //    printf("g_sys_params.temp_room_lowlimits = %d\r\n" ,g_sys_params.temp_room_lowlimits);
////    // 温度上下限改变后需要重新计算档位
////    if (sys_params->temp_set > sys_params->temp_room_uplimits*10)
////    {
////        sys_params->temp_set = sys_params->temp_room_uplimits*10;
////        run_params->temp_return = sys_params->temp_set;
////    }
////    else if (sys_params->temp_set < sys_params->temp_room_lowlimits*10)
////    {
////        sys_params->temp_set = sys_params->temp_room_lowlimits*10;
////        run_params->temp_return = sys_params->temp_set;
////    }
////
////    //printf("Heating Period:%02X-%02X/%02X-%02X\r\n", sys_params->heating_period_st[0], sys_params->heating_period_st[1], sys_params->heating_period_et[0], sys_params->heating_period_et[1]);
////    //printf("RoomName:%s\r\n", sys_params->room_name);
////    //printf("DispCtrl:%02X/%02X\r\n", pkg->msg_data[9], sys_params->disp_ctl);
////
////    //MYLOG_DEBUG("cycle_sample:%u\r\n", sys_params->cycle_sample);
    return 0;
}

 /*! \brief
*      出厂配置
* \param pkg[IN]            - cj188的参数
* \param sys_params[IN]     - 系统参数
* \param run_params[IN]     - 运行参数
*
* \note
*
*/
u8 app_factory_request(CJ188Pkg *pkg, /*const*/ SysParams *sys_params, RunParams run_params)
{
    u8 offset = 0;
    u32 sn_data = 0;
    u16 gn_data = 0;

    /* -------------------没有那些固定的常量------------------- */
    /* SN */
    sn_data = LittleANDBig_ModeCON4(&(sys_params->Device_SN));
    memcpy(&(pkg->sn), &(sn_data), sizeof(pkg->sn));
    /* GN */
    gn_data = LittleANDBig_ModeCON2(&(sys_params->Device_GN));
    memcpy(&(pkg->gn), &(gn_data), sizeof(pkg->gn));
    /* 数据类型 */
    pkg->ctrl_code = 1;
    /* 数据区 */
    offset = 0;
    /* SN */
    memcpy(pkg->msg_data+offset, &(sn_data), sizeof(pkg->sn));
    offset += 4;
    /* GN */
    memcpy(pkg->msg_data+offset, &(gn_data), sizeof(pkg->gn));
    offset += 2;
    /* 硬件版本 */
    sys_params->hardware_persion = 0x00;
    pkg->msg_data[offset] = sys_params->hardware_persion;
    offset += 1;
    /* 软件版本 */
    sys_params->soft_persion = 0x60;
    pkg->msg_data[offset] = sys_params->soft_persion;
    offset += 1;
    /* 执行标志 */
    pkg->msg_data[offset] = 0xAA;
    offset += 1;
    /* 数据负载长度 */
    pkg->msg_size = offset;
    return 0;
}

/*! \brief
*      定时上报请求
* \param pkg[IN]            - cj188的参数
* \param sys_params[IN]     - 系统参数
* \param run_params[IN]     - 运行参数
*
* \note
*
*/
u8 app_report_request(CJ188Pkg *pkg, /*const*/ SysParams *sys_params, RunParams *run_params)
{
    u8 offset = 0;
    u32 sn_data = 0;
    u16 gn_data = 0;
    u16 data_first = 0xAAAA;
    u16 temp_set = 0;
    u16 temp = 0;
    s16 ave_temp_100 = 0;
    u32 com_ok = 0;
    u32 com_total = 0;

    /* -------------------没有那些固定的常量------------------- */
    /* SN */
    sn_data = LittleANDBig_ModeCON4(&(sys_params->Device_SN));
    memcpy(&(pkg->sn), &(sn_data), sizeof(pkg->sn));
    /* GN */
    gn_data = LittleANDBig_ModeCON2(&(sys_params->Device_GN));
    memcpy(&(pkg->gn), &(gn_data), sizeof(pkg->gn));
    /* 数据类型 */
    pkg->ctrl_code = 200;
    /* 数据区 */
    offset = 0;
    memcpy(pkg->msg_data+offset, &data_first, sizeof(data_first));
    offset += sizeof(data_first);
    /* 温度设置标志位 */
    pkg->msg_data[offset] = sys_params->sys_set_temp_flg;
    offset += 1;
    /* 设定温度 */
    temp_set = sys_params->temp_set;
    temp_set = LittleANDBig_ModeCON2(&(temp_set));
    memcpy(pkg->msg_data+offset, &(temp_set), sizeof(sys_params->temp_set));
    offset += sizeof(sys_params->temp_set);
    /* 当前室内温度 */
    temp = run_params->temp;
    temp = LittleANDBig_ModeCON2(&(temp));
    memcpy(pkg->msg_data+offset, &(temp), sizeof(run_params->temp));
    offset += sizeof(run_params->temp);
    /* 室内平均温度 */
    ave_temp_100 = LittleANDBig_ModeCON2((u16*)&(run_params->ave_temp_100));
    memcpy(pkg->msg_data+offset, &(ave_temp_100), sizeof(run_params->ave_temp_100));
    offset += sizeof(run_params->ave_temp_100);
    /* 面板状态 */
    pkg->msg_data[offset] = run_params->face_state;
    offset += 1;
    /* 通讯成功次数 */
    com_ok = LittleANDBig_ModeCON4(&(sys_params->com_ok));
    memcpy(pkg->msg_data+offset, &(com_ok), sizeof(sys_params->com_ok));
    offset += sizeof(sys_params->com_ok);
    /* 通讯总次数 */
    com_total = LittleANDBig_ModeCON4(&(sys_params->com_total));
    memcpy(pkg->msg_data+offset, &(com_total), sizeof(sys_params->com_total));
    offset += sizeof(sys_params->com_total);
    /* 动作请求标志 */
    if (sys_params->temp_set >= run_params->temp)
    {
      run_params->actionflag = 0x0A;
      run_params->valve_position = 0x0A;
    }
    else
    {
      run_params->actionflag = 0xA0;
      run_params->valve_position = 0xA0;
    }
    pkg->msg_data[offset] = run_params->actionflag;
    offset += 1;
    /* 数据负载长度 */
    pkg->msg_size = offset;
    return 0;


//////    //    int i = 0;
//////    // 仪表地址
//////    memcpy(pkg->addr, sys_params->addr, sizeof(pkg->addr));
//////
//////    //    for(i=0; i<5; i++)
//////    //    {
//////    //        printf("pkg->addr1111111 = %x\r\n" ,sys_params->addr[i]);
//////    //    }
//////
//////    // 控制码
//////    pkg->ctrl_code = CTRL_CODE_REPORT;
//////    // 数据负载
//////    offset = 0;
//////    // 室内温度
//////    ////#if 0
//////    ////    dec_2_bcd_type(pkg->msg_data+offset, 2, 235, ORD_INTEL);
//////    ////#else
//////    dec_2_bcd_type(pkg->msg_data+offset, 2, run_params.temp, ORD_INTEL);
//////    ////#endif
//////    offset += 2;
//////    // 设定温度
//////    //dec_2_bcd_type(pkg->msg_data+offset, 2, run_params.temp_return, ORD_INTEL);
//////    //printf("run_params.temp_return = %d\r\n" ,run_params.temp_return);
//////    //dec_2_bcd_type(pkg->msg_data+offset, 2, run_params.temp_return, ORD_INTEL);
//////    dec_2_bcd_type(pkg->msg_data+offset, 2, sys_params->temp_set, ORD_INTEL);
//////    //printf("pkg->msg_data+offset = %d\r\n" ,dec_2_bcd_type(pkg->msg_data+offset, 2, run_params.temp_return, ORD_INTEL));
//////    offset += 2;
//////    // 运行模式
//////    pkg->msg_data[offset] = 0;
//////    offset += 1;
//////    // 电池电压
//////    //printf("run_params.batt_lvl = %d\r\n" ,run_params.batt_lvl);
//////    dec_2_bcd_type(pkg->msg_data+offset, 2, run_params.batt_lvl, ORD_INTEL);
//////    offset += 2;
//////    // 操作数
//////    pkg->msg_data[offset] = 0;
//////    offset += 1;
//////    // ST
//////    dec_2_hex_type(pkg->msg_data+offset, 2, run_params.st, ORD_INTEL);
//////    offset += 2;
//////    // 数据负载长度
//////    pkg->msg_size = offset;
//////
//////
//////    return 0;
}

/*! \brief
*      出厂前配置阀门ID反馈
* \param pkg[IN]            - cj188的参数
* \param sys_params[IN]     - 系统参数
* \param run_params[IN]     - 运行参数
* \param ack[IN]            - 附加数据标志
*
* \note
*
*/
u8 app_factory_report_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack)
{
//    char factory_sn[4] = {0xAA, 0xAA, 0xAA, 0xAA};
    //char factory_gn[2] = {0x00, 0x00};
    //u8 sn_1 = 0;

    // SN检查
//    printf("pkg->sn = %lu\r\n", pkg->sn);
//    if (memcmp(&pkg->sn, factory_sn, sizeof(pkg->sn)) != 0)
//    {
//        return CJ188_ERR_ADDR;
//    }
    //sn_1 = (pkg->sn & 0x0000FFFF)>>12;

    // SN检查 4频段的必须下载 ID为4频段的ID才能生效 否则不能下载
//    if (((pkg->sn & 0x0000FFFF)>>12) != DOWNLOAD_ID_CHANNEL/10)
//    {
//        rf_snd_data(&sn_1, 1);
//        return CJ188_ERR_ADDR;
//    }

    // GN检查
//    if (memcmp(&pkg->gn, factory_gn, sizeof(pkg->gn)) != 0)
//    {
//        return CJ188_ERR_ADDR;
//    }

    //    int i = 0;
    //    for(i=0; i<5; i++)
    //    {
    //        printf("pkg->addr = %x\r\n" ,g_sys_params.addr[i]);
    //    }

    // 控制码
    if (pkg->ctrl_code != 0)
    {
        return CJ188_ERR_CTRL_CODE;
    }

    return app_factory_response(pkg, sys_params, run_params, ack);
}

/*! \brief
*      出厂前配置阀门ID反馈
* \param pkg[IN]           - 解析后的数据报文
* \param sys_params[OUT]   - 系统参数
* \param run_params[OUT]   - 运行参数
*
* \return
* \note
*      该函数不检查报文合法性，调用之前需要检查报文合法性
*
*/
u8 app_factory_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack)
{
    u8 offset = 0;

    u32 Device_SN = 0;
    u16 Device_GN = 0;

    if (pkg->msg_size < 8)
        return CJ188_ERR_LENGTH;

    //printf("Resport Success\r\n");

    offset = 0;
    /* 设备的SN,进行存储 */
    memcpy(&Device_SN, pkg->msg_data+offset, sizeof(sys_params->Device_SN));
    Device_SN = LittleANDBig_ModeCON4(&Device_SN);
    if (((Device_SN & 0x0000FFFF)>>12) != DOWNLOAD_ID_CHANNEL/10)
    {
        return 1;
    }

    memcpy(&sys_params->Device_SN, &Device_SN, sizeof(sys_params->Device_SN));
    offset += 4;
    /* 设备的GN,进行存储 */
    memcpy(&Device_GN, pkg->msg_data+offset, sizeof(sys_params->Device_GN));
    Device_GN = LittleANDBig_ModeCON2(&Device_GN);
    memcpy(&sys_params->Device_GN, &Device_GN, sizeof(sys_params->Device_GN));
    offset += 2;
    /* 硬件版本 */
    sys_params->hardware_persion = pkg->msg_data[offset];
    offset += 1;
    /* 软件版本 */
    sys_params->soft_persion = pkg->msg_data[offset];

    return 0;
}

/*! \brief
*      定时上报反馈
* \param pkg[IN]            - cj188的参数
* \param sys_params[IN]     - 系统参数
* \param run_params[IN]     - 运行参数
* \param ack[IN]            - 附加数据标志
*
* \note
*
*/
u8 app_report_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack)
{

    // GN检查
    if (memcmp(&pkg->sn, &sys_params->Device_SN, sizeof(pkg->gn)) != 0)
    {
        return CJ188_ERR_ADDR;
    }

    //    int i = 0;
    //    for(i=0; i<5; i++)
    //    {
    //        printf("pkg->addr = %x\r\n" ,g_sys_params.addr[i]);
    //    }

    // 控制码
    if (pkg->ctrl_code != 201)
    {
        return CJ188_ERR_CTRL_CODE;
    }

    return app_normal_response(pkg, sys_params, run_params, ack);

//////    // 地址码检查
//////    if (memcmp(pkg->addr, sys_params->addr, sizeof(pkg->addr)) != 0)
//////    {
//////        return CJ188_ERR_ADDR;
//////    }
//////
//////    //    int i = 0;
//////    //    for(i=0; i<5; i++)
//////    //    {
//////    //        printf("pkg->addr = %x\r\n" ,g_sys_params.addr[i]);
//////    //    }
//////
//////    // 控制码
//////    if (pkg->ctrl_code != (CTRL_CODE_REPORT | 0x80))
//////    {
//////        return CJ188_ERR_CTRL_CODE;
//////    }
//////
//////    return app_normal_response(pkg, sys_params, run_params, ack);
}

/*! \brief
*      常规反馈
* \param pkg[IN]           - 解析后的数据报文
* \param sys_params[OUT]   - 系统参数
* \param run_params[OUT]   - 运行参数
*
* \return
* \note
*      该函数不检查报文合法性，调用之前需要检查报文合法性
*
*/
u8 app_normal_response(CJ188Pkg *pkg, SysParams *sys_params, RunParams *run_params, u8 *ack)
{
//////    u8 i = 0;
//////    u8 tmp[8] = {0};
//////    u8 offset = 0;
//////    u8 flag = 0;
//////    s32 balance = 0;
//////    g_sys_params.sn_recharge = 0;
//////    //char room_name[7] = {0};
    u8 offset = 0;
    u32 heating_time = 0;
    u32 heating_quantity = 0;
    u16 year = 0;
    u16 temp_set = 0;
    u16 limte_te_value = 0;
    struct tm t;

    if (pkg->msg_size < 27)
        return CJ188_ERR_LENGTH;

    //printf("Resport Success\r\n");


    offset = 0;
    /* 开关阀阀门位置 */
    run_params->valve_position = pkg->msg_data[offset];
    offset += 1;
    /* 开关阀阀门状态 */
    run_params->valve_state = pkg->msg_data[offset];
    offset += 1;
    /* 阀门堵转状态 */
    run_params->valve_lockstate = pkg->msg_data[offset];
    offset += 1;
    /* 累计开阀时间 */
    memcpy(&heating_time, pkg->msg_data+offset, sizeof(sys_params->heating_time));
    heating_time = LittleANDBig_ModeCON4(&heating_time);//累计开阀时间
    memcpy(&sys_params->heating_time, &heating_time, sizeof(sys_params->heating_time));
    offset += 4;
    /* 累计热量 */
    memcpy(&heating_quantity, pkg->msg_data+offset, sizeof(sys_params->heating_quantity));
    heating_quantity = LittleANDBig_ModeCON4(&heating_quantity);
    memcpy(&sys_params->heating_quantity, &heating_quantity, sizeof(sys_params->heating_quantity));
    offset += 4;
    /* 日期-年 */
    memcpy(&year, pkg->msg_data+offset, sizeof(run_params->year));
    year = LittleANDBig_ModeCON2(&year);
    memcpy(&run_params->year, &year, sizeof(run_params->year));
    offset += 2;
    /* 日期-月 */
    run_params->month = pkg->msg_data[offset];
    offset += 1;
    /* 日期-日 */
    run_params->day = pkg->msg_data[offset];
    offset += 1;
    /* 日期-时 */
    run_params->hour = pkg->msg_data[offset];
    offset += 1;
    /* 日期-分 */
    run_params->minute = pkg->msg_data[offset];
    offset += 1;
    /* 日期-秒 */
    run_params->second = pkg->msg_data[offset];
    offset += 1;
    /* 日期-周 */
    run_params->week = pkg->msg_data[offset];
    offset += 1;
    t.tm_year = run_params->year-2000;
    t.tm_mon  = run_params->month;
    t.tm_mday = run_params->day;
    t.tm_hour = run_params->hour;
    t.tm_min  = run_params->minute;
    t.tm_sec  = run_params->second;
    t.tm_week = run_params->week;
    rtc_write(t);   //把时钟写进去
    /* 是否在供暖期 */
    sys_params->workflg = pkg->msg_data[offset];
    offset += 1;
    /* 系统设置温度有效标志位 */
    sys_params->sys_set_temp_flg = pkg->msg_data[offset];
    offset += 1;
    /* 设定温度 */
    if (sys_params->sys_set_temp_flg == 0xAA)
    {
      memcpy(&temp_set, pkg->msg_data+offset, sizeof(sys_params->temp_set));
      temp_set = LittleANDBig_ModeCON2(&temp_set);
      temp_set *= 10;
      memcpy(&sys_params->temp_set, &temp_set, sizeof(sys_params->temp_set));
    }
    offset += 2;
    /* 欠费标志 */
    run_params->arrears_state = pkg->msg_data[offset];
    offset += 1;
    /* 极限温度设置标志 */
    sys_params->limte_te_flag = pkg->msg_data[offset];
    offset += 1;
    if (sys_params->limte_te_flag == 0xAA)
    {
      /* 极限温度就是设定室温的上限 */
      memcpy(&limte_te_value, pkg->msg_data+offset, sizeof(limte_te_value));       // 2个字节
      limte_te_value = LittleANDBig_ModeCON2(&limte_te_value);
      limte_te_value = limte_te_value / 100;
      memcpy(&sys_params->temp_room_uplimits, &limte_te_value+1, sizeof(sys_params->temp_room_uplimits));   // 1个字节,取低字节
    }

    return 0;

//////    offset = 0;
//////
//////    // 总累计热量
//////    memcpy(tmp, pkg->msg_data+offset, 4); //memcpy函数的前两位是地址
//////    sys_params->heating_quantity = bcd_2_dec_type(tmp, 4, ORD_INTEL); //sys_params是地址指向结构体里的成员
//////    offset += 4; //offset为地址偏移量,为让包里的地址改变
//////
//////    // 总累计热量单位
//////    sys_params->heating_quantity_unit = pkg->msg_data[offset];
//////    offset += 1;
//////
//////    // 总累计热费
//////    memcpy(tmp, pkg->msg_data+offset, 4);
//////    sys_params->heating_cost_total = bcd_2_dec_type(tmp, 4, ORD_INTEL);
//////    offset += 4;
//////
//////    // 总累计时间
//////    memcpy(tmp, pkg->msg_data+offset, 3);
//////    sys_params->heating_time = bcd_2_dec_type(tmp, 3, ORD_INTEL);
//////    offset += 3;
//////
//////    // 日累计热量
//////    memcpy(tmp, pkg->msg_data+offset, 4);
//////    sys_params->heating_quantity_day = bcd_2_dec_type(tmp, 4, ORD_INTEL);
//////    offset += 4;
//////
//////    // 日累计热量单位
//////    sys_params->heating_quantity_day_unit = pkg->msg_data[offset];
//////    offset += 1;
//////
//////    // 日累计热费
//////    memcpy(tmp, pkg->msg_data+offset, 4);
//////    sys_params->heating_cost_day = bcd_2_dec_type(tmp, 4, ORD_INTEL);
//////    offset += 4;
//////
//////    // 日累计时间
//////    memcpy(tmp, pkg->msg_data+offset, 3);
//////    sys_params->heating_time_day = bcd_2_dec_type(tmp, 3, ORD_INTEL);
//////    offset += 3;
//////
//////    // 余额
//////    memcpy(tmp, pkg->msg_data+offset, 4);
//////    if ((tmp[3]&0x80) == 0)
//////    {
//////        // 正值余额
//////        balance = (long)bcd_2_dec_type(tmp, 4, ORD_INTEL);
//////        //printf("[2]balance:%ld\r\n", balance);
//////
//////        sys_params->balance = balance;
//////    }
//////    else
//////    {
//////        // 负值余额
//////        tmp[3] &= ~0x80;
//////        balance = (long)bcd_2_dec_type(tmp, 4, ORD_INTEL);
//////        balance *= -1;
//////
//////        sys_params->balance = balance;
//////    }
//////    offset += 4;
//////
//////    // 余额单位
//////    if (pkg->msg_data[offset]==UNIT_KWH || pkg->msg_data[offset]==UNIT_GJ)
//////    {
//////        sys_params->balance_unit = pkg->msg_data[offset];
//////    }
//////    else
//////    {
//////        sys_params->balance_unit = UNIT_KWH;
//////    }
//////    offset += 1;
//////
//////    // 系统时间
//////    memcpy(tmp, pkg->msg_data+offset, 7);
//////    bytes_reverse(tmp, 7);//正好rtc_write_bcd里需要BCD码，所以不需要转换
//////    //run_params->date[0] = bcd_2_dec_type(tmp, 7, ORD_INTEL);
//////
//////    // 同步时间
//////    rtc_write_bcd(tmp, 7);
//////    //rtc_write_bcd(run_params->date, 7);
//////    offset += 7;
//////
//////    // 状态ST
//////    memcpy(tmp, pkg->msg_data+offset, 2);
//////    run_params->st = bcd_2_dec_type(tmp, 2, ORD_INTEL);
//////    offset += 2;
//////
//////    //printf("ST:%04X\r\n", run_params->st);
//////
//////    // 附加数据标志位
//////    flag = pkg->msg_data[offset];
//////    offset += 1;
//////#ifdef TASK_PRINTF
//////    MYLOG_DEBUG("FLAG:%02x\r\n", flag);
//////#endif
//////    // 需要反馈
//////    if (flag != 0)
//////    {
//////        *ack = 1;
//////        //printf("ack22222222222222222222222222222222222:%d\r\n", *ack);
//////    }
//////
//////    //MYLOG_DEBUG("FLAG:%02X\r\n", flag);
//////
//////    for (i=0; i<8; ++i)
//////    {
//////        if (flag&0x01)  //让flag一直右移什么时候不为0时证明最低位为1了，移动的位数就为i的大小
//////        {
//////            switch(i)
//////            {
//////            case AD_IDX_HP:
//////                // 供暖起始时间
//////                sys_params->heating_period_st[1] = pkg->msg_data[offset++];
//////                sys_params->heating_period_st[0] = pkg->msg_data[offset++];
//////                // 供暖结束时间
//////                sys_params->heating_period_et[1] = pkg->msg_data[offset++];
//////                sys_params->heating_period_et[0] = pkg->msg_data[offset++];
//////
//////                //printf("g_sys_params.heating_period_st = %d\r\n" ,g_sys_params.heating_period_st);
//////                //printf("g_sys_params.heating_period_et = %d\r\n" ,g_sys_params.heating_period_et);
//////                //printf("HP[%02X-%02X/%02X-%02X]\r\n", sys_params->heating_period_st[0], sys_params->heating_period_st[1], sys_params->heating_period_et[0], sys_params->heating_period_et[1]);
//////                break;
//////            case AD_IDX_TEMP_SET:
//////                //printf("TEMP_SET[%X%02X]\r\n", pkg->msg_data[offset+1], pkg->msg_data[offset]);
//////                sys_params->temp_set = bcd_2_dec_type(pkg->msg_data+offset, 2, ORD_INTEL);
//////                offset += 2;
//////
//////
//////                //                printf("g_sys_params.temp_room_uplimits = %d\r\n" ,g_sys_params.temp_room_uplimits);
//////                //                printf("g_sys_params.temp_room_lowlimits = %d\r\n" ,g_sys_params.temp_room_lowlimits);
//////                if (sys_params->temp_set > sys_params->temp_room_uplimits*10)
//////                {
//////                    sys_params->temp_set = sys_params->temp_room_uplimits*10;
//////                }
//////                else if (sys_params->temp_set < sys_params->temp_room_lowlimits*10)
//////                {
//////                    sys_params->temp_set = sys_params->temp_room_lowlimits*10;
//////                }
//////
//////                //printf("TEMP_SET[%u]\r\n", sys_params->temp_set);
//////
//////                break;
//////            case AD_IDX_TEMP_SET_LIMITS:
//////                // 温度设定上限
//////                sys_params->temp_room_uplimits   = pkg->msg_data[offset++];
//////                // 温度设定下限
//////                sys_params->temp_room_lowlimits  = pkg->msg_data[offset++];
////////                printf("2222222222222222222222222222222222222222222");
////////                printf("sys_params->temp_set = %d\r\n", sys_params->temp_set);
////////                printf("sys_params->temp_room_uplimits = %d\r\n", sys_params->temp_room_uplimits);
//////                //温度上下限改变后需要重新计算档位
//////                if (sys_params->temp_set > sys_params->temp_room_uplimits*10)
//////                {
//////                    //sys_params->temp_set = sys_params->temp_room_uplimits*10;
//////                    //printf("1111111111111111111111111111111111111");
//////                    sys_params->temp_set = sys_params->temp_room_uplimits*10;
//////                    run_params->temp_return = sys_params->temp_set;
//////                    //printf("run_params->temp_return = %u\r\n" ,run_params->temp_return);
//////                }
//////                else if (sys_params->temp_set < sys_params->temp_room_lowlimits*10)
//////                {
//////                    sys_params->temp_set = sys_params->temp_room_lowlimits*10;
//////                    run_params->temp_return = sys_params->temp_set;
//////                }
//////
//////                //printf("TEMP_SET_LIMITS[%u, %u]\r\n", sys_params->temp_room_uplimits, sys_params->temp_room_lowlimits);
//////                break;
//////            case AD_IDX_RECHARGE:
//////                sys_params->sn_recharge = bcd_2_dec_type(pkg->msg_data+offset, 4, ORD_INTEL);
//////                offset += 4;
//////
//////                //printf("RECHARGE[%u]\r\n", sys_params->sn_recharge);
//////                break;
//////            case AD_IDX_BALANCE_LINMITS:
//////                sys_params->balance_alarm = pkg->msg_data[offset++];
//////
//////                //printf("BALANCE_LINMITS[%u]\r\n", sys_params->balance_alarm);
//////                break;
//////            case AD_IDX_DISP_CTL:
//////                sys_params->disp_ctl = pkg->msg_data[offset++];
//////
//////                //printf("DISPLAY_CTRL[%02X]\r\n", sys_params->disp_ctl);
//////                break;
//////            case AD_IDX_ROOM_NAME:
//////                //sprintf(room_name, "%X-%X%02X", pkg->msg_data[offset+2], pkg->msg_data[offset+1], pkg->msg_data[offset]);
//////                room_name[0] = (pkg->msg_data[offset+2]%0x0f)+'0';
//////                room_name[1] = '-';
//////                if ((pkg->msg_data[offset+1]>>4) > 0)
//////                {
//////                    room_name[2] = (pkg->msg_data[offset+1]>>4)+'0';
//////                    room_name[3] = (pkg->msg_data[offset+1]%0x0f)+'0';
//////                    room_name[4] = (pkg->msg_data[offset+0]>>4)+'0';
//////                    room_name[5] = (pkg->msg_data[offset+0]%0x0f)+'0';
//////                }
//////                else
//////                {
//////                    room_name[2] = (pkg->msg_data[offset+1]%0x0f)+'0';
//////                    room_name[3] = (pkg->msg_data[offset+0]>>4)+'0';
//////                    room_name[4] = (pkg->msg_data[offset+0]%0x0f)+'0';
//////                    room_name[5] = '\0';
//////                }
//////
//////                memcpy(sys_params->room_name, room_name, sizeof(sys_params->room_name));
//////                offset += 3;
//////
//////                //printf("ROOM_NAME[%s]\r\n", sys_params->room_name);
//////                break;
//////            default:
//////                break;
//////            }
//////        }
//////
//////        flag >>= 1;
//////    }
//////
//////    return 0;
}

/*! \brief
*      确认反馈
* \param pkg[IN]           - 解析后的数据报文
* \param sys_params[OUT]   - 系统参数
* \param run_params[OUT]   - 运行参数
*
* \return
* \note
*
*
*/
u8 app_ack_request(CJ188Pkg *pkg, const SysParams *sys_params, RunParams run_params)
{
    // 仪表地址
    memcpy(pkg->addr, sys_params->addr, sizeof(pkg->addr));
    // 控制码
    pkg->ctrl_code = CTRL_CODE_ACK;
    // 数据域长度
    pkg->msg_size  = 0x00;

    return 0;
}