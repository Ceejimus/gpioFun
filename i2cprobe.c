#include <stdio.h>
#include <wiringPiI2C.h>

const int ADXL345_ID = 0x53;

const int ADXL345_DATA_FORMAT = 0x31;

const int ADXL345_X0 = 0x32;
const int ADXL345_X1 = 0x33;
const int ADXL345_Y0 = 0x34;
const int ADXL345_Y1 = 0x35;
const int ADXL345_Z0 = 0x36;
const int ADXL345_Z1 = 0x37;

const int ADXL345_PCTL = 0x2D;

int InitAccelerometerRead(short pcl, short dataFormat);
short ReadAccelerometerAxis(short *x, short *y, short *z);
short AxisConvert(short lsB, short msB);

int fd = -1;

int main(void) {

    short x, y, z;

    fd = InitAccelerometerRead(0x08, 0x00);

    int flush_count = 0;

    while (666) {
        if (ReadAccelerometerAxis(&x, &y, &z)) {
            fprintf(stdout, "%3d, %3d, %3d\n", x, y, z);
        }
        if (flush_count++ > 10) {
            fflush(stdout);
            flush_count = 0;
        }
        delay(100);
    }

    return 0;
}

int InitAccelerometerRead(short pcl, short dataFormat) {
    int fd = wiringPiI2CSetup(ADXL345_ID);

    if (wiringPiI2CWriteReg8(fd, ADXL345_PCTL, pcl) < 0) {
        return -1;
    }

    if (wiringPiI2CWriteReg8(fd, ADXL345_DATA_FORMAT, dataFormat) < 0) {
        return -1;
    }

    return fd;
}

short ReadAccelerometerAxis(short *x, short *y, short *z) {
    if (fd < 0) {
        return 0;
    }

    short x0 = wiringPiI2CReadReg8(fd, ADXL345_X0);
    short x1 = wiringPiI2CReadReg8(fd, ADXL345_X1);

    short y0 = wiringPiI2CReadReg8(fd, ADXL345_Y0);
    short y1 = wiringPiI2CReadReg8(fd, ADXL345_Y1);

    short z0 = wiringPiI2CReadReg8(fd, ADXL345_Z0);
    short z1 = wiringPiI2CReadReg8(fd, ADXL345_Z1);

    *x = AxisConvert(x0, x1);
    *y = AxisConvert(y0, y1);
    *z = AxisConvert(z0, z1);

    return 1;
}

short AxisConvert(short lsB, short msB) {
    return (short)(lsB | (msB << 8));
}
