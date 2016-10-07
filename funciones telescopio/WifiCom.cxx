#include 'WifiCom.h'

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


void WifiCom::init(void)
{
	UBRR1H = (WBAUDRATE >> 8);             // Set Baud Rate
	UBRR1L = WBAUDRATE;
	UCSR1B |= (1 << TXEN1)|(1 << RXEN1);   // Transmit and Recieve enable
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);     // Set 8-bit data transfer
}

// SEND DATA TO ESP2866
int WifiCom::sendData(char *data)
{
	int i=0;
	char c;
	while ((c=data[i++])!='\0')
		uart1_transmit(c);
	uart1_transmit('\r');
	uart1_transmit('\n');
	return 1;
}

// READ COMMANDS OF ESP2866
char *WifiCom::readCmd(bool type=0))
{
	int i=0;
	unsigned char c;
	int count_ok=0;
	while   (i<1000) //
	{
		//if(UART1_AVAILABLE)               //alternative way
		c=uart1_recieve();
		buffer[i]=c;
		if ((buffer[i-1]=='O') && (buffer[i]=='K')) break;           // #! characteristic flag
		if ((buffer[i-4]=='E') && (buffer[i-3]=='R')&&(buffer[i-2]=='R') && (buffer[i-1]=='O')&& (buffer[i]=='R')) break;  // break when an error is announced 
		i++;
	}
	buffer[i+1]='\0';
	return buffer;
}


// READ QUERYS OF ESP268
char *WifiCom::readQuery(bool type=0)
{
	bool store_en = false;
	int i=0;
	unsigned char p = '\0';
	unsigned char c = '\0';
	int count_ok=0;
	while   (i<1000) //
	{
		//if(UART1_AVAILABLE)               //alternative way
		c=uart1_recieve();
		if((c=='#')&&(p=='!')){store_en = true;}
		p=c;
		if(store_en)
		{
			buffer[i]=c;
			if ((buffer[i-1]=='#') && (buffer[i]=='!')) break;           // #! characteristic flag
			if ((buffer[i-4]=='E') && (buffer[i-3]=='R')&&(buffer[i-2]=='R') && (buffer[i-1]=='O')&& (buffer[i]=='R')) break;  // break when an error is announced 
			i++;
		}
	}
	buffer[i+1]='\0';
	//char  hola[10];
	//sprintf(hola, "LAST I: %d\n",i);
	//uart_send(hola);
	return buffer;
}

/////// Auxiliary  functions

//Function to send data
void uart1_transmit(unsigned char data)
{
   // loop_until_bit_is_set(UCSR1A, UDRE1);       // alternative way 
   while (!( UCSR1A & (1<<UDRE1)));               // wait while register is free
   UDR1 = data;                                   // load data in the register
}

//Function to receive data
unsigned char uart1_recieve (void)
{
	//while(!(UCSR1A & 1<<RXC1));                  // alternative way
	loop_until_bit_is_set(UCSR1A, RXC1);
	return UDR1;                                   // return 8-bit data	
}

//Function to send a char array
void uart1_flush(void)
{
	unsigned char dummy;
	while (!( UCSR1A & (1<<RXC1)) ) dummy = UDR1;
}

