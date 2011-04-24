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
	
	int DATA_SIZE = 500;
	int i = 0;
	float data[DATA_SIZE][6];
	float data_2[DATA_SIZE][6];
	float data_3[DATA_SIZE][6];
	float data_4[DATA_SIZE][6];
	float data_5[DATA_SIZE][6];
	float data_6[DATA_SIZE][6];
	float data_7[DATA_SIZE][6];
	float data_8[DATA_SIZE][6];
	float data_9[DATA_SIZE][6];
	float data_99[DATA_SIZE][6];
	float data_91[DATA_SIZE][6];
	float data_52[DATA_SIZE][6];
	float data_53[DATA_SIZE][6];
	float data_54[DATA_SIZE][6];
	float data_55[DATA_SIZE][6];
//	int data_time[DATA_SIZE];
	
	timer.start();

	while(1) {
	//	data_time[i] = timer.read_us();
		data[i][0] = acc_x.read();
		data[i][1] = acc_y.read();
		data[i][2] = acc_z.read();
		data[i][3] = gyro_x.read();
		data[i][4] = gyro_y.read();
		printf("hellooo");
	//	printf("%d,%f,%f,%f,%f,%f\r\n",data_time[i],data[i][0],data[i][1], data[i][2], data[i][3], data[i][4]);
		if (i == DATA_SIZE - 1) {
			i = 0;
		}
		else {
			i = i+1;
		}
	}
	

}