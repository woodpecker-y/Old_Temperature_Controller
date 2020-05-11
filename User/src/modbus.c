#include "adf.h"
#include "modbus.h"

/*! \brief 
*      MODBUS协议组包
* \param package[OUT]          - 组包后的数据包
* \param size[OUT]				- 组包后的数据包长度
* \param addr[IN]				- 地址码
* \param cmd[IN]				- 功能码
* \param dat[IN]				- 数据域数据
* \param dat[IN]				- 数据域数据长度
* 
* \return  
*      0                   - 成功
* \note  
*		size作为即作为输入参数，又作为输出参数，作为输入参数时指定package缓冲区大小
* 
*/
u8 modbus_pack(u8 *package, u8 *size, u8 addr, u8 cmd, u8 *dat, u8 dat_len)
{
    u8 i = 0;
    u8 offset = 0;
    u16 crc = 0;
    
    // 地址码
    package[offset++] = addr;
    // 功能码
    package[offset++] = cmd;
    // 数据域
    for (i=0; i<dat_len; ++i)
    {
        package[offset++] = dat[i];
    }
    
    // CRC-16 Modbus
    crc = crc_16_modbus(package, offset);
    package[offset++] = crc&0xFF;
    package[offset++] = (crc>>8)&0xFF;
    
    *size = offset;
    
    return 0;
}

/*! \brief 
*      MODBUS协议解包
* \param package[IN]			- 原始报文
* \param size[IN]				- 原始报文长度
* \param addr[OUT]				- 地址码
* \param cmd[OUT]				- 功能码
* \param dat[OUT]				- 数据域数据
* \param dat[OUT]				- 数据域数据长度
* 
* \return  
*      0                   - 成功
* \note  
*		size作为即作为输入参数，又作为输出参数，作为输入参数时指定package缓冲区大小
* 
*/
u8 modbus_unpack(u8 *package, u8 size, u8 *addr, u8 *cmd, u8 **dat, u8 *len)
{
    u8  offset = 0;
    u16 crc = 0;
    
    crc = crc_16_modbus(package, size-2);
    if ((crc&0xFF)!=package[size-2] || ((crc>>8)&0xFF)!=package[size-1])
        return MODBUS_ERR_CRC16;
    
    // 地址码
    *addr = package[offset];
    offset += 1;
    
    // 功能码
    if (package[offset]!=MODBUS_CMD_READ_MULTI && package[offset]!=MODBUS_CMD_WRITE_SINGLE && package[offset]!=MODBUS_CMD_WRITE_MULTI)
        return MODBUS_ERR_INVALID_CMD;
    
    *cmd = package[offset];
    offset += 1;
    
    // 数据域
    *len = size-4;
    *dat = package + offset;
    
    return 0;
}

u8 modbus_read_multi_request_unpack(u8 *data_field, u8 data_field_len, u16 *reg_val, u16 *reg_cnt)
{
    if (data_field_len != 4)
        return MODBUS_ERR_LENGTH;
    
    *reg_val = (data_field[0]<<8) | data_field[1];
    *reg_cnt = (data_field[2]<<8) | data_field[3];
    
    return 0;
}

u8 modbus_read_multi_response_pack(u8 *data_field, u8 data_field_len, u16 *reg_addr, u16 *reg_cnt)
{
    if (data_field_len != 4)
        return MODBUS_ERR_LENGTH;
    
    *reg_addr = (data_field[0]<<8) | data_field[1];
    *reg_cnt  = (data_field[2]<<8) | data_field[3];
    
    return 0;
}

u8 modbus_write_single_request_unpack(u8 *data_field, u8 data_field_len, u16 *reg_addr, u16 *reg_val)
{
    if (data_field_len != 4)
        return MODBUS_ERR_LENGTH;
    
    *reg_addr = (data_field[0]<<8) | data_field[1];
    *reg_val  = (data_field[2]<<8) | data_field[3];
    
    return 0;	
}

u8 modbus_write_multi_request_unpack(u8 *data_field, u8 data_field_len, u16 *reg_addr_st, u8 *reg_val, u8 *reg_cnt)
{
    u8 i = 0;
    u8 offset = 0;
    
    // 数据域总长度判断
    if (data_field_len != data_field[4]+4)
        return MODBUS_ERR_LENGTH;
    
    // 寄存器起始地址
    *reg_addr_st = (data_field[0]<<8) | data_field[1];
    // 寄存器总数
    *reg_cnt     = (data_field[2]<<8) | data_field[3];
    
    // 寄存器值长度判断
    if (*reg_cnt*2 != data_field[4])
        return MODBUS_ERR_LENGTH;
    
    offset = 5;
    
    // 寄存器值
    for (i=0; i<data_field[4]; ++i)
    {
        reg_val[i] = data_field[offset];
        offset++;
    }
    
    return 0;	
}