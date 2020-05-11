

#ifndef _MOTOR_H
#define _MOTOR_H

#ifdef HAVE_MOTOR

/*! \brief
*      Load Motor Port Configure
* \param handler[IN]        - board motor port configure handler
*/
void motor_load_config(const MotorConfig *handler);

/*! \brief
*       motor power on
*/
void motor_power_on(void);


/*! \brief
*       motor power off
*/
void motor_power_off(void);


/*! \brief
*       motor initilization
*/
void motor_init(void);

/*! \brief
*       motor close
*/
void motor_close(void);

/*! \brief
*       motor run by direction
* \param dir[IN]            - Motor run direction
*        MOTOR_DIR_FORWORD  - Run forword, the state of motor should be closed.
*        MOTOR_DIR_REVERSE  - Run Reverse, the state of motor should be opened.
*/
void motor_run(u8 dir);

/*! \brief
*       motor stop
* \note
*       motor should be stoped when the state of motor is closed or opened max.
*/
void motor_stop(void);

/*! \brief
*       get motor run state
* \return
*       MOTOR_STATE_CLOSED      - motor is closed
*       MOTOR_STATE_OPEND       - motor is opened max
*       MOTOR_STATE_OTHERS      - motor is in other state
*/
u8   motor_get_state(void);

#endif

#endif