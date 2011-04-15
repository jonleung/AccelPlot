#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <hal.h>
#include <bmac.h>
#include <nrk_error.h>

#include <nrk_driver_list.h>
#include <nrk_driver.h>
#include <ff_basic_sensor.h>

nrk_task_type TX_TASK;
NRK_STK tx_task_stack[NRK_APP_STACKSIZE];
void tx_task (void);

void nrk_create_taskset ();
void nrk_register_drivers();

uint8_t tx_buf[RF_MAX_PAYLOAD_SIZE];

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

   nrk_register_drivers();
   nrk_create_taskset ();
   nrk_start ();

   return 0;
}

void tx_task ()
{
   uint8_t j, i, val, len, cnt,fd;
   nrk_sig_t tx_done_signal;
   uint16_t adxl_x, adxl_y, adxl_z, mic, light, temp, bat;
   nrk_sig_mask_t ret;

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

   printf ("tx_task PID=%d\r\n", nrk_get_pid ());

   // Wait until the tx_task starts up bmac
   // This should be called by all tasks using bmac that
   // do not call bmac_init()...
   while (!bmac_started ())
       nrk_wait_until_next_period ();

   // Get and register the tx_done_signal if you want to
   // do non-blocking transmits
   tx_done_signal = bmac_get_tx_done_signal ();
   nrk_signal_register (tx_done_signal);

   // Open ADC device as read
   fd=nrk_open(FIREFLY_SENSOR_BASIC,READ);
   if(fd==NRK_ERROR) nrk_kprintf(PSTR("Failed to open sensor driver\r\n"));

   cnt = 0;
   while (1) {

       val=nrk_set_status(fd,SENSOR_SELECT,BAT);
       val=nrk_read(fd,&bat,2);

       // Build a sensor packet
       //sprintf (tx_buf, "S %d\r\n", bat);
       //printf( "tx_buf=%s",tx_buf );

       // TODO: Please enter code to input ADC values from all the sensors
       //Please modify the rest of the code in order to send all the sensor values to the server node
       //adxl_x, adxl_y, adxl_z, mic, light, temp, bat;
       
       val=nrk_set_status(fd,SENSOR_SELECT,ACC_X);
       val=nrk_read(fd,&adxl_x,4);
       
       val=nrk_set_status(fd,SENSOR_SELECT,ACC_Y);
       val=nrk_read(fd,&adxl_y,4);
       
       val=nrk_set_status(fd,SENSOR_SELECT,ACC_Z);
       val=nrk_read(fd,&adxl_z,4);
       
       // Build a sensor packet
       sprintf (tx_buf,
           "x=%d,y=%d,z=%d\n",
           adxl_x, adxl_y, adxl_z);
       printf( "tx_buf=%s",tx_buf );

       cnt++;
       nrk_led_set (BLUE_LED);

       // For blocking transmits, use the following function call.
       // For this there is no need to register  
       // val=bmac_tx_packet(tx_buf, strlen(tx_buf));

       // This function shows how to transmit packets in a
       // non-blocking manner  
       val = bmac_tx_pkt_nonblocking(tx_buf, strlen (tx_buf));
       nrk_kprintf (PSTR ("Tx packet enqueued\r\n"));
       // This functions waits on the tx_done_signal
       ret = nrk_event_wait (SIG(tx_done_signal));

       // Just check to be sure signal is okay
       if(ret & SIG(tx_done_signal) == 0 )
           nrk_kprintf (PSTR ("TX done signal error\r\n"));

       // Task gets control again after TX complete
       nrk_kprintf (PSTR ("Tx task sent data!\r\n"));
       nrk_led_clr (BLUE_LED);
       nrk_wait_until_next_period ();
   }

}

void nrk_create_taskset ()
{
   TX_TASK.task = tx_task;
   TX_TASK.Ptos = (void *) &tx_task_stack[NRK_APP_STACKSIZE - 1];
   TX_TASK.Pbos = (void *) &tx_task_stack[0];
   TX_TASK.prio = 2;
   TX_TASK.FirstActivation = TRUE;
   TX_TASK.Type = BASIC_TASK;
   TX_TASK.SchType = PREEMPTIVE;
   TX_TASK.period.secs = 0;
   TX_TASK.period.nano_secs = 250* NANOS_PER_MS;
   TX_TASK.cpu_reserve.secs = 1;
   TX_TASK.cpu_reserve.nano_secs = 500 * NANOS_PER_MS;
   TX_TASK.offset.secs = 0;
   TX_TASK.offset.nano_secs = 0;
   nrk_activate_task (&TX_TASK);
}

void nrk_register_drivers()
{
   int8_t val;

   // Register the Basic FireFly Sensor device driver
   // Make sure to add:
   //     #define NRK_MAX_DRIVER_CNT  
   //     in nrk_cfg.h
   // Make sure to add:
   //     SRC += $(ROOT_DIR)/src/drivers/platform/$(PLATFORM_TYPE)/source/ff_basic_sensor.c
   //     in makefile
   val=nrk_register_driver( &dev_manager_ff_sensors,FIREFLY_SENSOR_BASIC);
   if(val==NRK_ERROR) nrk_kprintf( PSTR("Failed to load my ADC driver\r\n") );

}