#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "i2c_master.h"
#include "ImuDof.h"
#include "Communication.h"


uint8_t buffer[6];
char gyr[6] ="Gyro\n";
char acc[14] ="Acceleracion\n";
char mag[14] ="Compass\n";
char mag2[14] ="Compass Cal\n";
char x1[4] = "x: ";
char y1[4] = "y: ";
char z1[4] = "z: ";
char l[14] ="Leyendo.....\n";
char t[2] = "\t";
char ct[4] = ":(";
char n[2] = "\n";
char f[10] ="Todo ok!\n";
uint8_t data[100];
uint8_t addr=(0x68<<1);
uint8_t Maddr = (0x0C<<1);
int16_t ax; 
int16_t ay;
int16_t az;
int16_t gx;
int16_t gy;
int16_t gz;
float *a;
float *g;
float *m;
int temp;
char *str;
char *float2str(float n);
char str1[8];
//+++++++++++MAIN++++++++++++

int main(void){
    
    SerialCom uart;
    ImuDof imu;
    imu.init();
	uart.init(57600);
	
	
    
    if (imu.testConnection()){
        uart.sendData(f);
    }
    else{
        uart.sendData(ct);
    }
     uart.sendData(l);
     uart.sendData(n);

    while(1){ 
     
     //   temp = imu.getTemp();
     //   itoa(temp,str, 10);
     //   uart.sendData(str);
     //   uart.sendData(n);
     //   uart.sendData(n);

        uart.sendData(gyr);
        g = imu.getGyro(&ax, &ay, &az, &gx, &gy, &gz);
        str =float2str(*g);
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(g+1));
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(g+2));
        uart.sendData(str);           
        uart.sendData(t);
        uart.sendData(n);
        str =itoa(gx,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        str= itoa(gy,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        str=itoa(gz,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        uart.sendData(n);
        
        _delay_ms(1000);
        uart.sendData(gyr);
        g = imu.getGyro(&ax, &ay, &az, &gx, &gy, &gz);
        str =float2str(*g);
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(g+1));
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(g+2));
        uart.sendData(str);           
        uart.sendData(t);
        uart.sendData(n);
        str =itoa(gx,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        str= itoa(gy,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        str=itoa(gz,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        uart.sendData(n);
        
        _delay_ms(1000);

        uart.sendData(acc);
        a = imu.getAcc(&ax, &ay, &az, &gx, &gy, &gz);
        str =float2str(*a);
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(a+1));
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(a+2));
        uart.sendData(str);           
        uart.sendData(t);
        str =itoa(ax, str,10);
        uart.sendData(str);           
        uart.sendData(t);
        str= itoa(ay, str,10);
        uart.sendData(str);           
        uart.sendData(t);
        str= itoa(az,str,10);
        uart.sendData(str);           
        uart.sendData(t);
        uart.sendData(n);
        
        _delay_ms(1000);
        
        uart.sendData(mag);
        m = imu.getComp();
        str =float2str(m[0]);
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(m+1));
        uart.sendData(str);           
        uart.sendData(t);
        str= float2str(*(m+2));
        uart.sendData(str);           
        uart.sendData(t);
  
        uart.sendData(n);
        uart.sendData(n);
        uart.sendData(n);
        _delay_ms(5000);


    }
	return 0;
}

char *float2str(float n){
    
    char freq_char[7];                              //dtostrf() on stdlib.h, to cast a float to char*
    dtostrf(n,7, 3, (char*)freq_char);                              //dtostrf() on stdlib.h, to cast a float to char*
	str1[0] = freq_char[0];
	str1[1] = freq_char[1];
	str1[2] = freq_char[2];
	str1[3] = freq_char[3];
    str1[4] = freq_char[4];
    str1[5] = freq_char[5];
    str1[6] = freq_char[6];
	str1[7] = '\0';
	return str1;
}


