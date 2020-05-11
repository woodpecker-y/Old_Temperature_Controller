#ifndef _TASK_KEY_H
#define _TASK_KEY_H

/*! \brief key status definition */
typedef enum _KeySts{
    KEY_STS_IDLE = 0,               // 空闲状态
    KEY_STS_PUSH,                   // 按键按下
    KEY_STS_VALID,                  // 按键释放
    KEY_STS_DEAL,                   // 按键处理
    KEY_STC_FINISH,                  //按键完成
}KeySts;

/*! \brief 按键句柄 */
typedef struct _KeyHandler{
    u8  sts;                    // 按键状态
    u8  key_val;                // 按键键值	
    u32 timer;                  // 按键显示消失的时间
    u32 timer_long;             // 按键长按时间
}KeyHandler;

/* 按键进程 */
void task_key_proc(void);
/* 上键进程 */
void key_action_add(void);
/* 下键进程 */
void key_action_sub(void);
/* 查询键进程 */
void key_action_view(void);
/* 下载ID进程 */
void key_action_setid(void);
/* 按键触发 */
void task_key_triggered(u8 key_code);

/* key status */
extern KeyHandler s_key_handler; //定义了一个结构体变量

#endif