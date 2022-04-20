#ifndef __LOOP_H_
#define __LOOP_H_

#include "Defination.h"
#include "main.h"
#include "stdbool.h"


#define CONTROL_FREQ_HZ 20000   //设置后台最小时间间隔 20kHZ   Systick函数 频率 20KHZ


extern bool sys_tick_changed_flag;

void loop(void);
void TaskCheck(void);
void SysTickUpdate(void);

void time_systick_flag_func(void);
void time_1ms_clock(void);

void time_1ms_flag_func(void);
void time_10ms_flag_func(void);
void time_20ms_flag_func(void);
void time_50ms_flag_func(void);
void time_100ms_flag_func(void);

#endif
