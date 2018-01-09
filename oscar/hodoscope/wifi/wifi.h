#ifndef WIFI_H
#define WIFI_H


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>

#define UART1_AVAILABLE bit_is_set(UCSR1A, RXC1)
#define BAUD 57600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define WBAUD 115200
#define WBAUDRATE 0x0008 //((F_CPU)/(WBAUD*16UL)-1)*/ 


unsigned char recieve(void);
void transmit(unsigned char data);
char msg1[1000]; 

class Wifi{
    public:
    
    Wifi();
    ~Wifi();
    

    void init();    
    int sendData(char *data);
	char *readData();
    void flush(void);

};

#endif
