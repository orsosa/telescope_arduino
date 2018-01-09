#include "wifi.h"

void transmit(unsigned char data){
    while (!( UCSR1A & (1<<UDRE1)));                
    UDR1 = data;                                   
}

unsigned char recieve(void){
    loop_until_bit_is_set(UCSR1A, RXC1);
	return UDR1;   
}

Wifi::Wifi(){};
Wifi::~Wifi(){};

void Wifi::init(){
    UBRR1H = (WBAUDRATE >> 8);
	UBRR1L = WBAUDRATE;
	UCSR1B |= (1 << TXEN1)|(1 << RXEN1);
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
}

void Wifi::flush(){
    unsigned char dummy;
    while (!( UCSR1A & (1<<RXC1)) ) dummy = UDR1;
}

int Wifi::sendData(char *data)
{
    int i=0;
    char c;
    while ((c=data[i++])!='\0'){
        c = data[i];
	    transmit(c);
        i++;
    }
    transmit('\r');
    transmit('\n');
    return 1;
}

char *Wifi::readData(){
    
    int i=0;
    unsigned char c;
    int count_ok=0;
    while   (i<1000) //
    {
	    //if(UART1_AVAILABLE)
        c=recieve();
	    msg1[i]=c;
	    //uart_transmit(c);
	    if ((msg1[i-1]=='#') && (msg1[i]=='!')) break;
	    if ((msg1[i-4]=='E') && (msg1[i-3]=='R')&&(msg1[i-2]=='R') && (msg1[i-1]=='O')&& (msg1[i]=='R')) break;
	    i++;
    }
    return msg1;
}


