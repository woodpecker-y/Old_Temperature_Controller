#ifndef _MODBUS_H
#define _MODBUS_H

// MODBUS命令码
#define MODBUS_CMD_READ_MULTI		0x03	// 读多个寄存器
#define MODBUS_CMD_WRITE_SINGLE		0x06	// 写单个寄存器
#define MODBUS_CMD_WRITE_MULTI		0x10	// 写多个寄存器

// MODBUS地址码
#define MODBUS_ADDR	0xFF

typedef enum _ModbusRet{
    MODBUS_OK = 0,
    MODBUS_ERR_INVALID_CMD,		 // 未知功能码
    MODBUS_ERR_CRC16,			 // 校验和错误
    MODBUS_ERR_LESS_MEMORY,              // 内存不足
    MODBUS_ERR_ADDR,                     // 地址不匹配
    MODBUS_ERR_LENGTH			 // 长度错误
}ModbusRet;

u8 modbus_pack(u8 *package, u8 *size, u8 addr, u8 cmd, u8 *dat, u8 len);
u8 modbus_unpack(u8 *package, u8 size, u8 *addr, u8 *cmd, u8 **dat, u8 *len);

#endif