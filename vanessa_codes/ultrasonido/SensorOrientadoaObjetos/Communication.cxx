#include "Communication.h"

void SerialCom::init(uint16_t baudrate)
{
    uint16_t BAUDRATE = (F_CPU/16)/(baudrate-1);

	UBRR0H = (BAUDRATE >> 8);                  //Set Baud Rate
	UBRR0L = BAUDRATE;
	UCSR0B |= (1 << TXEN0)|(1 << RXEN0);	   //Transmit and Recieve enable
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);         // Set 8-bit data transfer
}


//Function to send a char array
void SerialCom::sendData(char *data)
{
int i=0;
char c;
while ((c=data[i++])!='\0')
	uart_transmit(c);

}

//Function to read a char array (from UDR0 buffer)

char *SerialCom::readData(void)
{
int i=0;
char c;
while (((c=uart_recieve())!='\r') && (i < 99))
{
	kBuffer[i++]=c;
}
kBuffer[i]='\0';
return kBuffer;
}


/////// Auxiliary functions 

//Function to send data
void uart_transmit(unsigned char data)
{
    while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
    UDR0 = data;                                   // load data in the register
}

//Function to receive data
unsigned char uart_recieve (void)
{
    while(!(UCSR0A & 1<<RXC0));                   // wait while data is being received
    return UDR0;                                   // return 8-bit data
}

//Function to clean RCX register
void uart_flush(void)
{
unsigned char dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
