#include "loop.h"
#include "vofa.h"


bool sys_tick_changed_flag = false;

uint16_t time_1ms_flag = 0;
uint8_t time_10ms_flag = 0;
uint8_t time_20ms_flag = 0;
uint8_t time_50ms_flag = 0;
uint8_t time_100ms_flag = 0;

void loop(void)
{
	HAL_Delay(100);  //等待系统硬件稳定

	//Device Init//

	//Communication Init//
	vofa.init();

	//中断接管，心跳程序变成20kHZ (可以更高)
	SysTickUpdate();
	while (1)
	{
		TaskCheck();
	}
}


void TaskCheck(void)
{
	if(time_10ms_flag)		{time_10ms_flag--;		time_10ms_flag_func();		}
	if(time_20ms_flag)		{time_20ms_flag--;		time_20ms_flag_func();		}
	if(time_50ms_flag)		{time_50ms_flag--;		time_50ms_flag_func();		}
	if(time_100ms_flag)		{time_100ms_flag--;		time_100ms_flag_func();	}
}
uint32_t motor_dgree_want = 0;
uint32_t motor_dgree_dbug_want = 17000;

double time_slice = 2*3.1415926535897931/20000;

void time_systick_flag_func()
{
	
}
void time_1ms_flag_func(void)
{
	
//	vofa.fdata.set_data(0,tad2141.angle_data);
//	vofa.fdata.set_data(1,tad2141.rectify_angle);
//	vofa.fdata.set_data(2,motor_control.real_location);
//	vofa.fdata.set_data(3,motor_control.est_location);
//	vofa.fdata.set_data(4,motor_control.goal_location);
//	vofa.fdata.set_data(5,((double)(motor_control.goal_location - motor_control.real_location))/65535.0*360);
//	vofa.fdata.send(6);
}

void time_10ms_flag_func(void)
{
	
}

void time_20ms_flag_func(void)
{

}

void time_50ms_flag_func(void)
{
	
}

void time_100ms_flag_func(void)
{}

void time_1ms_clock(void)
{
	time_1ms_flag++;
	time_1ms_flag_func();
	if(!(time_1ms_flag % 10))		{time_10ms_flag++;		}
	if(!(time_1ms_flag % 20))		{time_20ms_flag++;		}
	if(!(time_1ms_flag % 50))		{time_50ms_flag++;		}
	if(!(time_1ms_flag % 100))	{time_100ms_flag++;		}
	if(!(time_1ms_flag % 1000))	{time_1ms_flag = 0;		}
}

void SysTickUpdate(void)
{
	sys_tick_changed_flag = true;
	HAL_SYSTICK_Config(SystemCoreClock / CONTROL_FREQ_HZ);	//更新为20K中断
}
