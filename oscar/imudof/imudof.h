/*
    Library for IMU 10DOF Adafruit Breakout:
    Motion Processing Unit (MPU 9250) and Digital Pressure Sensor (BMP280) 
    
    Based on:
        MPU 9250 Product Specification Revision 1.1
        Document number: PS-MPU-9250A-01 Release date: 06/20/2016
 
        MPU 9250 register map Revision 1.6
        Document number: RM-MPU-9250A-00 Release date: 01/07/2015 
        
        BMP280 BOCH datasheet Revision 1.14
        Document number: BST-BMP280-DS001-11 Release date: 05/05/2015        
*/
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef IMUDOF_H
#define IMUDOF_H

#define IMUADDR 0xD0   //MPU 9250 I2C ADO = 0 addres
#define IMUID   0x71

/****** Principal register addresses ******/

//Config registers
#define CONFIG                  0x1A
#define GYRO_CONFIG             0x1B
#define ACCEL_CONFIG            0x1C
#define FIFO_EN                 0x23
#define SINGAL_PATH_RESET       0x68 //BIT0 = TEMP_RST, BIT1 = ACCEL_RST, BIT2 = GYRO_RST     

#define USER_CTRL               0x6A 
#define PWR_MGMT_1              0x6B
#define PWR_MGMT_2              0x6C
#define WHO_AM_I                0x75

/**************************************************
//Accelerometer data out registers
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40

//Temperature sensor data out registers
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42

//Gyroscope data out registers
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
*************************************************/

class imudof{
    public:
    //Variables
        uint8_t data[14];   
        //float   Accel[3];
        int16_t kAccel[3];
        float   kAccelF[3];
        char*   kAccelC[3];

        //float Gyro[3];
        int16_t kGyro[3];
        float kGyroF[3];
        char* kGyroC[3];

        int16_t kMag[3];
        float kMagF[3];
        char* kMagC[3];
    
    //Functions        
        imudof();
        ~imudof();
        void init();
        bool testConnection();
        void readData(); 
};

#endif
