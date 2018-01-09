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

char f[10] ="Todo ok!\n";
char ct[4] = ":(";

int main(void){
    
    SerialCom uart;
    imudof imu;
        
	uart.init(57600);
    
    imu.init();
 

    if (imu.testConnection()) uart.sendData(f);
    else uart.sendData(ct);
    
    
    while(1){
    _delay_ms(300);
    imu.readData();
    


    uart.sendData("Acelerometro: ");uart.sendData("\n");
    uart.sendData("x: ");uart.sendData(imu.kAccelC[0]);  uart.sendData("\t");
    uart.sendData("y: ");uart.sendData(imu.kAccelC[1]);  uart.sendData("\t");
    uart.sendData("z: ");uart.sendData(imu.kAccelC[2]);  uart.sendData("\n");
   /*
    uart.sendData("Giroscopio: ");uart.sendData("\n");
    uart.sendData("x: ");uart.sendData(imu.kGyroC[0]);  uart.sendData("\t");
    uart.sendData("y: ");uart.sendData(imu.kGyroC[1]);  uart.sendData("\t");
    uart.sendData("z: ");uart.sendData(imu.kGyroC[2]);  uart.sendData("\n");
    
    uart.sendData("Magnetometro: ");uart.sendData("\n");
    uart.sendData("x: ");uart.sendData(imu.kMagC[0]);  uart.sendData("[uT]\t");
    uart.sendData("y: ");uart.sendData(imu.kMagC[1]);  uart.sendData("[uT]\t");
    uart.sendData("z: ");uart.sendData(imu.kMagC[2]);  uart.sendData("[uT]\n");
*/
    }

    return 0;
}



