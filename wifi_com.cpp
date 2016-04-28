#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define BAUD 57600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define WBAUD 115200
#define WBAUDRATE 0x0008 //((F_CPU)/(WBAUD*16UL)-1)*/ 
#define UART1_AVAILABLE bit_is_set(UCSR1A, RXC1)

//Serial Communication inicialization 
void uart_init(void)
{
	UBRR0H = (BAUDRATE >> 8);
	UBRR0L = BAUDRATE;
	UCSR0B |= (1 << TXEN0)|(1 << RXEN0);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
}

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

void uart_flush(void)
{
unsigned char dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int uart_send(char *data)
{
int i=0;
char c;
while ((c=data[i++])!='\0')
	uart_transmit(c);
	uart_transmit('\r');
	uart_transmit('\n');
return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char msg[100];

char *uart_read()
{
int i=0;
char c;
while (((c=uart_recieve())!='\r') && (i < 99))
{
	msg[i++]=c;
}
msg[i]='\0';
return msg;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////Serial1 (WiFi) Communication/////////////////////////////////////////////////////////////////////

//Serial1 Communication inicialization 
void uart1_init(void)
{
	UBRR1H = (WBAUDRATE >> 8);
	UBRR1L = WBAUDRATE;
	UCSR1B |= (1 << TXEN1)|(1 << RXEN1);
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
}

//Function to send data
void uart1_transmit(unsigned char data)
{
   // loop_until_bit_is_set(UCSR1A, UDRE1);
   while (!( UCSR1A & (1<<UDRE1)));                // wait while register is free
    UDR1 = data;                                   // load data in the register
}

//Function to receive data
char msg0[100];
unsigned char uart1_recieve (void)
{
	/*uart_send("................");
	sprintf(msg0,"register before: %x",!(UCSR1A & 1<<RXC1)),
	uart_send(msg0);
	sprintf(msg0,"register after: %x",!(UCSR1A & 1<<RXC1));
	uart_send(msg0);
	*/
	//while(!(UCSR1A & 1<<RXC1));
	loop_until_bit_is_set(UCSR1A, RXC1);
    //unsigned char res = UDR1; 
	//uart_send("................");
	return UDR1;                                   // return 8-bit data
	
}

void uart1_flush(void)
{
unsigned char dummy;
while (!( UCSR1A & (1<<RXC1)) ) dummy = UDR1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int uart1_send(char *data)
{
int i=0;
char c;
while ((c=data[i++])!='\0')
	uart1_transmit(c);
uart1_transmit('\r');
uart1_transmit('\n');
return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char msg1[1000];

 	

char *uart1_read()
{
int i=0;
unsigned char c;
int count_ok=0;
while   (i<1000) //
{
	//if(UART1_AVAILABLE)
    c=uart1_recieve();
	msg1[i]=c;
	//uart_transmit(c);
	if ((msg1[i-1]=='#') && (msg1[i]=='!')) break;
	if ((msg1[i-4]=='E') && (msg1[i-3]=='R')&&(msg1[i-2]=='R') && (msg1[i-1]=='O')&& (msg1[i]=='R')) break;
	i++;
}
msg1[i+1]='\0';
char  hola[10];
sprintf(hola, "LAST I: %d\n",i);
uart_send(hola);
return msg1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void timer1_init()
{
	TCCR1B |= (1 << CS10);
	TCNT1 = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	uart_init();
	uart1_init();
	uart_send("**************");
	uart1_send("AT+RST");
	//uart_send(uart1_read());
	_delay_ms(2000);
	uart_send("Reset OK");
	uart1_send("AT+CIPMUX=1");
	//uart_send(uart1_read());
	_delay_ms(500);
	uart_send("CIPMUX OK");
	uart1_send("AT+CWMODE=1");
	//uart_send(uart1_read());
	_delay_ms(500);
	uart_send("CWMODE OK");
	uart1_send("AT+CWJAP=\"atlasgw\",\"SiLabaLiS\"");
	//uart_send(uart1_read());
	_delay_ms(8000);
    uart_send("JAP OK");
	//char cmd[100] ="AT+CIPSTART=4,\"TCP\",\"192.168.1.216\",80";
	uart1_send("AT+CIPSTART=4,\"TCP\",\"192.168.1.216\",80");
	//uart_send(uart1_read());
	uart_send("CIPSTART OK");
	_delay_ms(3000);
	uart1_send("AT+CIPSEND=4,18");
    //uart_send(uart1_read());	
	uart_send("CIPSEND OK");
	uart1_send("GET /getdata.php");
	uart_send(uart1_read());
	uart_send("OK");
	
		/*while(1)
	{	
		//uart_send(uart1_read());
		uart_send("**************");
		uart1_send("AT+CWLAP");
		uart_send(uart1_read());
		uart_send("########");
		//uart1_send("AT+RST");
		//_delay_ms(500);
		//uart_send("**************");
		//if(UART1_AVAILABLE)
		//_delay_ms(0);
		//uart_send("--------------");
		//uart_flush();
		//uart1_flush();
	}*/
	while(1);
	return 0;
}