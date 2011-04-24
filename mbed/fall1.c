#include "mbed.h"

Serial pc(USBTX, USBRX);
AnalogIn acc_z(p15);
AnalogIn acc_x(p16);
AnalogIn acc_y(p17);
AnalogIn gyro_x(p18);
AnalogIn gyro_y(p19);
//AnalogIn gyro_z(p20);

Timer timer;

int main() {
	
	int DATA_SIZE = 10;
	int i = 0;
	float data[DATA_SIZE][6];
	int data_time[DATA_SIZE];
	
	timer.start();

	while(1) {
		data_time[i] = timer.read_us();
		data[i][0] = acc_x.read();
		data[i][1] = acc_y.read();
		data[i][2] = acc_z.read();
		data[i][3] = gyro_x.read();
		data[i][4] = gyro_y.read();

		printf("%f,%f,%f\r\n",data[i][1], data[i][2], data[i][3]);
		if (i == DATA_SIZE - 1) {
			i = 0;
		}
		else {
			i = i+1;
		}
	}
	

}