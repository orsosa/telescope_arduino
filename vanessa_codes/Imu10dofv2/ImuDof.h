#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "i2c_master.h"
#include<stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#define ImuAddr    (0x68<<1)


class ImuDof{
    private:
        I2C_master i2c;
        uint8_t data[14];
        uint8_t datam[6];
        float Accel[3];
        float Temp;
        float Gyro[3];
        float Mag[3];
        int16_t Aux[3];
        float mx_sample;
        float my_sample;
        float mz_sample;
        
        float mx_min = 0;
        float mx_max = 0;
        float my_min = 0;
        float my_max = 0;
        float mz_min = 0;
        float mz_max = 0;
        char str[8];     
    public:
        float mx_center=0;
        float my_center=0;
        float mz_center=0;
        ImuDof();
        void init(void);
        bool testConnection(void);
        void getData(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
        float * getAcc(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
 //       int getTemp(void);
        float * getGyro(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
        float * getComp(void);
        int16_t * getCompSample(void);
        void CompassCalibrate(void);
        float * getCompCal(void);
        
        void clearData(void);
};
