#include "loop_it.h"
#include "loop.h"
uint16_t system_new_clock = 0;
uint8_t system_new_clock_cnt = CONTROL_FREQ_HZ / 1000;

void SysTick_Handler(void)
{
	
	if(sys_tick_changed_flag) 
	{
		system_new_clock++;
		time_systick_flag_func();
		if(system_new_clock_cnt == system_new_clock)
		{
			HAL_IncTick();
			system_new_clock = 0;
		}
	}
	else
		HAL_IncTick();  //��δ��ϵͳ�ӹܣ�Systick 1msһ��
}
