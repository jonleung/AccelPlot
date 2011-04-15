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
#include <avr/sleep.h>
#include <hal.h>
#include <bmac.h>
#include <nrk_error.h>

nrk_task_type RX_TASK;
NRK_STK rx_task_stack[NRK_APP_STACKSIZE];
void rx_task (void);

void nrk_create_taskset ();

uint8_t rx_buf[RF_MAX_PAYLOAD_SIZE];

int main ()
{
	uint16_t div;
	nrk_setup_ports ();
	nrk_setup_uart (UART_BAUDRATE_115K2);

	nrk_init ();

	nrk_led_clr (0);
	nrk_led_clr (1);
	nrk_led_clr (2);
	nrk_led_clr (3);

	nrk_time_set (0, 0);

	bmac_task_config ();

	nrk_create_taskset ();
	nrk_start ();

	return 0;
}

void rx_task ()
{
	uint8_t i, len;
	int8_t rssi, val;
	uint8_t *local_rx_buf;
	nrk_time_t check_period;
	printf ("rx_task PID=%d\r\n", nrk_get_pid ());

	// init bmac on channel 25 
	bmac_init (25);

	// By default the RX check rate is 100ms
	// below shows how to change that
	//check_period.secs=0;
	//check_period.nano_secs=200*NANOS_PER_MS;
	//val=bmac_set_rx_check_rate(check_period);

	// The default Clear Channel Assement RSSI threshold is -45
	// Setting this value higher means that you will only trigger
	// receive with a very strong signal.  Setting this lower means
	// bmac will try to receive fainter packets.  If the value is set
	// too high or too low performance will suffer greatly.
	// bmac_set_cca_thresh(-45); 


	if(val==NRK_ERROR) nrk_kprintf( PSTR("ERROR setting bmac rate\r\n" ));
	// This sets the next RX buffer.
	// This can be called at anytime before releaseing the packet
	// if you wish to do a zero-copy buffer switch
	bmac_rx_pkt_set_buffer (rx_buf, RF_MAX_PAYLOAD_SIZE);

	while (1) {
		// Wait until an RX packet is received
		val = bmac_wait_until_rx_pkt ();
		// Get the RX packet 
		nrk_led_set (ORANGE_LED);
		local_rx_buf = bmac_rx_pkt_get (&len, &rssi);
		for (i = 0; i < len; i++)
			printf ("%c", rx_buf[i]);
		nrk_led_clr (ORANGE_LED);
		// Release the RX buffer so future packets can arrive 
		bmac_rx_pkt_release ();
	}

}

void nrk_create_taskset ()
{
	RX_TASK.task = rx_task;
	RX_TASK.Ptos = (void *) &rx_task_stack[NRK_APP_STACKSIZE - 1];
	RX_TASK.Pbos = (void *) &rx_task_stack[0];
	RX_TASK.prio = 2;
	RX_TASK.FirstActivation = TRUE;
	RX_TASK.Type = BASIC_TASK;
	RX_TASK.SchType = PREEMPTIVE;
	RX_TASK.period.secs = 1;
	RX_TASK.period.nano_secs = 0;
	RX_TASK.cpu_reserve.secs = 1;
	RX_TASK.cpu_reserve.nano_secs = 500 * NANOS_PER_MS;
	RX_TASK.offset.secs = 0;
	RX_TASK.offset.nano_secs = 0;
	nrk_activate_task (&RX_TASK);

	printf ("Create done\r\n");
}
