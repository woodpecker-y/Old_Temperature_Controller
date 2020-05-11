#ifndef _TASK_LIST_H
#define _TASK_LIST_H
#include "task_reader.h"
#include "task_key.h"
#include "task_beep.h"
#include "task_motor.h"
#include "task_config.h"
#include "task_comm.h"
#include "task_power.h"

// ии???ивDб└ик
TaskItem_t task_list[] = {
    {1, task_adc_proc},
};

#endif