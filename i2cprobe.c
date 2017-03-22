#include <stdio.h>
#include <wiringPiI2C.h>

const int ADXL345_ID = 0x53;
const int ADXL345_X0 = 0x50;
const int ADXL345_X1 = 0x51;
const int ADXL345_Y0 = 0x52;
const int ADXL345_Y1 = 0x53;
const int ADXL345_Z0 = 0x54;
const int ADXL345_Z1 = 0x55;

int main(void):
    int fd = wiringPiI2CSetup(ADXL345_ID)

    while(1) {
        delay(1000)

        int x0 = wiringPiI2CReadReg8(fd, ADXL345_X0) 
        int x1 = wiringPiI2CReadReg8(fd, ADXL345_X1) 

        printf("X0 %d, X1 %d", x0, x1)
    }
