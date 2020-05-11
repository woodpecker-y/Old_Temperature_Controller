
#ifndef _BSP_ALARM_H
#define _BSP_ALARM_H

#ifdef HAVE_ALARM

/*! \brief alarm confirgure Definition */
typedef struct _AlarmConfig{
    u8  date_weekday_select;    /*!< \brief alarm date_weekday_select */
    u8  date_weekday;           /*!< \brief alarm date_weekday */    
    u8  hour;                   /*!< \brief alarm hour */
    u8  min;                    /*!< \brief alarm min */
    u8  sec;                    /*!< \brief alarm sec */
    u8  mask;                   /*!< \brief alarm mask */
}AlarmConfig;

/*! \brief
*   load alarm default config
*/
void BSP_ALARM_LoadConfig(AlarmConfig *alarm_cfg);

/*! \brief
*   alarm initilization
*/
void BSP_ALARM_Init(void);

/*! \brief
*       update alarm setting
* \param date_weekday[IN]	- alarm date or weekday
* \param hour[IN]	        - alarm hour
* \param min[IN]	        - alarm min
* \param sec[IN]	        - alarm sec
* \param sec[IN]	        - alarm mask
*
*/
void BSP_ALARM_Update(u8 date_weekday, u8 hour, u8 min, u8 sec, u8 mask);

/*! \brief
*       update alarm enable of disable
* \param sts[IN]	- alarm enable state
*
*/
void BSP_ALARM_Ctrl(u8 sts);

#endif

#endif