#ifndef _TASK_CONFIG_H
#define _TASK_CONFIG_H

/* 配置的主进程 */
void task_config_proc(void);
void task_config_proc_test(void);
/* 出厂检测 */
void factory_check(void);
/* MODBUS协议处理 */
u8 params_service(u8 *snd_buf, u8 *snd_len, u8 *rcv_buf, u8 rcv_len);

#endif