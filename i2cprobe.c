#include <wiringPiI2C.h>

const int ADXL345_ID = 0x53;

int main(void):
    return wiringPiI2CSetup(ADXL345_ID)