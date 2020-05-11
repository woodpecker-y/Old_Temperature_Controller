#ifndef _MENU_H
#define _MENU_H

/*! \brief 菜单界面 */
typedef enum _Menu{
    MENU_MAIN = 0,  //显示主界面
    MENU_SET_TEMP,  //设定温度
    MENU_VIEW,      //菜单
    MENU_ID,        //设置ID
}Menu;

/*! \brief 信息查看索引 */
typedef enum _MenuViewIdx{
    MENU_VIEW_IDX_TEMP=0,                       // 室温
    MENU_VIEW_IDX_TEMP_SET,                     // 设定温度
//    MENU_VIEW_IDX_BALANCE,                      // 剩余热量   
    MENU_VIEW_IDX_HEATING_QUANTITY_TOTAL,       // 累计热量
    MENU_VIEW_IDX_HEATING_TIME_TOTAL,           // 用热时间
    MENU_VIEW_IDX_FAULT,                        // 故障 
    MENU_VIEW_IDX_ROOM_ID,			// 房间号	
    MENU_VIEW_IDX_ID_LEFT,                      // ID LEFT 
    MENU_VIEW_IDX_ID_RIGHT,		        // ID Right	
    MENU_VIEW_IDX_TIME,                         // 时间
}MenuViewIdx;

/* 设置温度 */
void menu_set_temp(void);
/* 一直检测显示 */
void menu_common(void);
/* 主屏幕显示温度 */
void menu_main(void);
/* 设置ID */
void menu_ID(void);
/* 显示选择 */
void menu_disp(u8 menu_id);
/* 菜单 */
void menu_view(void);
/* 电池电量的选择 */
void batter_value(void);

#endif