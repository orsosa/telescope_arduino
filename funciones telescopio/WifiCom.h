//// WiFi Communicaction

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUD 57600									// baud rate for serial communication
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define WBAUD 115200
#define WBAUDRATE 0x0008                            // baud rate for wifi-module communication
#define UART1_AVAILABLE bit_is_set(UCSR1A, RXC1)
#define TIMER_FREQ  F_CPU/1024.0
#define TOTAL_COUNTS  65535
#define INIT_COUNT(x)  (TOTAL_COUNTS - (x)*TIMER_FREQ)
#define MAX_GATE 4.194
#define OVERFLOW(y)  (unsigned int)((y)/MAX_GATE)

void uart1_transmit(unsigned char data);
unsigned char uart1_recieve ();
void uart1_flush();
class WifiCom 
{
	public:
	int kBuffSize;
	char kBuffer[1000];
	WifiCom() : kBuffSize(1000) {memset(kBuffer,0,kBuffSize);}
	
	void init(void);
	int sendData(char *data);
	char *readCmd(bool type=0);//
	char *readQuery(bool type=0);
	
};

