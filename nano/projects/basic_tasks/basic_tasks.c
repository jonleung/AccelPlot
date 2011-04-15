/******************************************************************************
 *  Nano-RK, a real-time operating system for sensor networks.
 *  Copyright (C) 2007, Real-Time and Multimedia Lab, Carnegie Mellon University
 *  All rights reserved.
 *
 *  This is the Open Source Version of Nano-RK included as part of a Dual
 *  Licensing Model. If you are unsure which license to use please refer to:
 *  http://www.nanork.org/nano-RK/wiki/Licensing
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.0 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************/

#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>


NRK_STK Stack1[NRK_APP_STACKSIZE];
nrk_task_type TaskOne;
void Task1(void);

NRK_STK Stack2[NRK_APP_STACKSIZE];
nrk_task_type TaskTwo;
void Task2 (void);

void nrk_create_taskset();
uint8_t kill_stack(uint8_t val);

int
main ()
{
	nrk_setup_ports();
	nrk_setup_uart(UART_BAUDRATE_115K2);

	nrk_init();

	nrk_led_clr(ORANGE_LED);
	nrk_led_clr(BLUE_LED);
	nrk_led_clr(GREEN_LED);
	nrk_led_clr(RED_LED);

	nrk_time_set(0,0);
	nrk_create_taskset ();
	nrk_start();

	return 0;
}


void Task1()
{
	uint16_t cnt;
	cnt=0;
	nrk_kprintf( PSTR("Nano-RK Version ") );
	printf( "%d\r\n",NRK_VERSION );

	printf( "My node's address is %u\r\n",NODE_ADDR );

	printf( "Task1 PID=%u\r\n",nrk_get_pid());

	while(1) {
		nrk_led_toggle(ORANGE_LED);
		nrk_gpio_toggle(NRK_DEBUG_0);
		printf( "Task1 cnt=%u\r\n",cnt );
		nrk_wait_until_next_period();
		// Uncomment this line to cause a stack overflow
		// if(cnt>20) kill_stack(10);
		cnt++;
	}
}

void Task2()
{
	int16_t cnt;
	printf( "Task2 PID=%u\r\n",nrk_get_pid());
	cnt=0;
	while(1) {
		nrk_led_toggle(BLUE_LED);
		nrk_gpio_toggle(NRK_DEBUG_1);
		printf( "Task2 signed cnt=%d\r\n",cnt );
		nrk_wait_until_next_period();
		cnt--;
	}
}


	void
nrk_create_taskset()
{
	nrk_task_set_entry_function( &TaskOne, Task1);
	nrk_task_set_stk( &TaskOne, Stack1, NRK_APP_STACKSIZE);
	TaskOne.prio = 1;
	TaskOne.FirstActivation = TRUE;
	TaskOne.Type = BASIC_TASK;
	TaskOne.SchType = PREEMPTIVE;
	TaskOne.period.secs = 0;
	TaskOne.period.nano_secs = 100*NANOS_PER_MS;
	TaskOne.cpu_reserve.secs = 0;
	TaskOne.cpu_reserve.nano_secs = 50*NANOS_PER_MS;
	TaskOne.offset.secs = 0;
	TaskOne.offset.nano_secs= 0;
	nrk_activate_task (&TaskOne);

	nrk_task_set_entry_function( &TaskTwo, Task2);
	nrk_task_set_stk( &TaskTwo, Stack2, NRK_APP_STACKSIZE);
	TaskTwo.prio = 2;
	TaskTwo.FirstActivation = TRUE;
	TaskTwo.Type = BASIC_TASK;
	TaskTwo.SchType = PREEMPTIVE;
	TaskTwo.period.secs = 0;
	TaskTwo.period.nano_secs = 500*NANOS_PER_MS;
	TaskTwo.cpu_reserve.secs = 0;
	TaskTwo.cpu_reserve.nano_secs = 100*NANOS_PER_MS;
	TaskTwo.offset.secs = 0;
	TaskTwo.offset.nano_secs= 0;
	nrk_activate_task (&TaskTwo);





}

uint8_t kill_stack(uint8_t val)
{
	char bad_memory[10];
	uint8_t i;
	for(i=0; i<10; i++ ) bad_memory[i]=i;
	for(i=0; i<10; i++ ) printf( "%d ", bad_memory[i]);
	printf( "Die Stack %d\r\n",val );
	if(val>1) kill_stack(val-1);
	return 0;
}


