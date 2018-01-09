/*
Libreria para el uso del prototiopo de hodoscopio SiLab 
Versión enero 2018
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "i2c_master.h"
#include "imudof.h"
#include "communication.h"
#include "Servo.h"
#include "us.h"
#include "stepper.h"
#include "fdcr.h"
//#include "wifi.h" 
    
//char f[10] ="Accel Ok\n";
//char ct[10] = "Accel Nok\n";
char *dataInC;
int  dataInI;
//int ang = 0;
int i;

int main(void){

    //Stepper stepx(&PORTE, &PORTE, X_DIR, X_STP, X_FdC);
    //Stepper stepy(&PORTH, &PORTE, Y_DIR, Y_STP, Y_FdC);
    //Stepper stepz(&PORTH, &PORTG, Z_DIR, Z_STP, Z_FdC);
    //Stepper stepa(&PORTB, &PORTB, A_DIR, A_STP, A_FdC);
        
    Fdcr finButton;
    SerialCom uart;
    //imudof imu;
    //Servo servo1;
    ultraSonic us1;
    us1.init();
	uart.init(57600);   
    finButton.init(); 
   // Wifi com;
    //com.init();
/*
    com.sendData("AT+RST");
    uart.sendData("Reset Ok");
    _delay_ms(1000);
    com.sendData("AT+CIPMUX=1"); 
    _delay_ms(1000);
    uart.sendData("CIPMUX OK");
    com.sendData("AT+CWMODE=1");
    _delay_ms(1000);
    com.sendData("AT+CWJAP=\"atlasgw\",\"SiLabaLiS\"");
    _delay_ms(5000);
    uart.sendData("CONECTION OK");
    com.sendData("AT+CIPSTART=4,\"TCP\",\"192.168.1.216\",80");
    _delay_ms(3000);
    uart.sendData("IP OK");
    com.sendData("AT+CIPSEND=4,18");
    com.sendData("GET /getdata.php");
    uart.sendData(com.readData());
*/

    //imu.init();
    //servo1.init();

    //if (imu.testConnection()) uart.sendData(f);
    //else uart.sendData(ct);
    
   // imu.readData();
    
    /*while(imu.kGyroF[2] > -90 && imu.kGyroF[1] < 0) {
        servo1.setAngle(ang);
        imu.readData(); 
        uart.sendData("z: ");uart.sendData(imu.kGyroC[2]);  uart.sendData("\n");  
        if(imu.kGyroF[2]<0 && imu.kGyroF[1]<0) ang++;
        if(imu.kGyroF[2]<0 && imu.kGyroF[1]>0) ang--;  
    }
    */
    while(1){
    //_delay_ms(100);
    
    /*dataInC = uart.readData();
    dataInI = atoi(dataInC);
    uart.sendData(dataInC);
    imu.readData();
    
    while(imu.kGyroF[1] != dataInI) {
        servo1.setAngle(ang);
        imu.readData(); 
        uart.sendData("Cambiando ang: ");uart.sendData(imu.kGyroC[1]);  uart.sendData("\n");  
        if(imu.kGyroF[1]< dataInI) ang++;
        if(imu.kGyroF[1]> dataInI) ang--;  
    } 

    uart.sendData("Acelerometro: ");uart.sendData("\n");
    uart.sendData("x: ");uart.sendData(imu.kAccelC[0]);  uart.sendData("\t");
    uart.sendData("y: ");uart.sendData(imu.kAccelC[1]);  uart.sendData("\t");
    uart.sendData("z: ");uart.sendData(imu.kAccelC[2]);  uart.sendData("\n");

    uart.sendData("Giroscopio: ");uart.sendData("\n");
    uart.sendData("x: ");uart.sendData(imu.kGyroC[0]);  uart.sendData("\t");
    uart.sendData("y: ");uart.sendData(imu.kGyroC[1]);  uart.sendData("\t");
    uart.sendData("z: ");uart.sendData(imu.kGyroC[2]);  uart.sendData("\n");    
     
    uart.sendData("Magnetometro: ");uart.sendData("\n");
    uart.sendData("x: ");uart.sendData(imu.kMagC[0]);  uart.sendData("\t");
    uart.sendData("y: ");uart.sendData(imu.kMagC[1]);  uart.sendData("\t");
    uart.sendData("z: ");uart.sendData(imu.kMagC[2]);  uart.sendData("\n");
*/  
    //ang=0;
       finButton.readState();
       if(finButton.val1!=1) uart.sendData("Fin de carrera 1 ON\n");
       if(finButton.val2!=1) uart.sendData("Fin de carrera 2 ON\n");
       if(finButton.val3!=1) uart.sendData("Fin de carrera 3 ON\n");
       if(finButton.val4!=1) uart.sendData("Fin de carrera 4 ON\n");
        /*
        us1.sendPulse();
        us1.getDistance();
        uart.sendData("Distancia: ");
        uart.sendData(us1.buff);
        uart.sendData("\n");
        _delay_ms(100);

        for(i = 0; i<500; i++){
            stepx.stepUp(false);
            stepy.stepUp(false);
            stepz.stepUp(false);
            stepa.stepUp(false);
    
            stepx.stepDown(false);
            stepy.stepDown(false);
            stepz.stepDown(false);
            stepa.stepDown(false); 
        }
        for(i=0; i<500; i++){
            stepx.stepUp(true);
            stepy.stepUp(true);
            stepz.stepUp(true);
            stepa.stepUp(true);
    
            stepx.stepDown(true);
            stepy.stepDown(true);
            stepz.stepDown(true);
            stepa.stepDown(true);
        }
    */
        
    }
    
    return 0;
}




