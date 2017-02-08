#include "WifiCom.h"

void WifiCom::init(void)
{
	UBRR1H = (WBAUDRATE >> 8);             // Set Baud Rate
	UBRR1L = WBAUDRATE;
	UCSR1B |= (1 << TXEN1)|(1 << RXEN1);   // Transmit and Recieve enable
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);     // Set 8-bit data transfer
}

// Timer3 for Interrupts

void WifiCom::interruptInit(void)
{
	TCCR3B |= (1 << CS32)|(1 << CS30);          					// Set 1024 prescaling
	kTotalOverflow = OVERFLOW(kGate);              		     			// Set the total overflows for a given gate time
	TCNT3 = INIT_COUNT(kGate-MAX_GATE*OVERFLOW(kGate))*(kTotalOverflow==0);  	// Counter value depends on overflow value
	TIMSK3 |= (1 << TOIE3);                                              
	sei();										// Enable global interrupts
	kTotalOverflow --;
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
char *WifiCom::readCmd(bool type)
{
	int i=0;
	unsigned char c;
	int count_ok=0;
	while   (i<1000) //
	{
		//if(UART1_AVAILABLE)               //alternative way
		c=uart1_recieve();
		kBuffer[i]=c;
		if ((kBuffer[i-1]=='O') && (kBuffer[i]=='K')) break;           // #! characteristic flag
		if ((kBuffer[i-4]=='E') && (kBuffer[i-3]=='R')&&(kBuffer[i-2]=='R') && (kBuffer[i-1]=='O')&& (kBuffer[i]=='R')) break;  // break when an error is announced 
		i++;
	}
	kBuffer[i+1]='\0';
	return kBuffer;
}


// READ QUERYS OF ESP268
char *WifiCom::readQuery(bool type)
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
			kBuffer[i]=c;
			if ((kBuffer[i-1]=='#') && (kBuffer[i]=='!')) break;           // #! characteristic flag
			if ((kBuffer[i-4]=='E') && (kBuffer[i-3]=='R')&&(kBuffer[i-2]=='R') && (kBuffer[i-1]=='O')&& (kBuffer[i]=='R')) break;  // break when an error is announced 
			i++;
		}
	}
	kBuffer[i+1]='\0';
	//char  hola[10];
	//sprintf(hola, "LAST I: %d\n",i);
	//uart_send(hola);
	return kBuffer;
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


//////////////////7777777777777777777777777777777777777777777777777////7777777//////77777777
////7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
///77777777777777777777777777777777777777777777777777777777777777777777777777777777777777777


// Timer3 Overflow Interrupt     -------------->   Here the data is send
ISR(TIMER3_OVF_vect)
{
/*
	TCNT3 = INIT_COUNT(kGate-MAX_GATE*OVERFLOW(kGate))*(kTotalOverflow==0);    // Counter value depends on overflow value, same as the initialization  
	if(kTotalOverflow<0)                                                     // Condition for send the value (gate time is reached)
	{
		uart1_send("\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7\B7");
		uart1_flush();
		
		
		kFreq = (float)((TCNT5)/kGate);                                    
		TCNT5=0;														   // Reset the coincidence counter
		dtostrf(kFreq,6, 3, (char*)kFreqChar);                              //dtostrf() on stdlib.h, to cast a float to char*
		kSendChar[53] = kFreqChar[1];
		kSendChar[54] = kFreqChar[2];
		kSendChar[55] = kFreqChar[3];
		kSendChar[56] = kFreqChar[4];
		kSendChar[57] = '\0';
		
		uart_send("----------");
		uart_send(data_send);
		uart_send("----------");
		
		uart1_send("AT+CIPSEND=4,59");                             
		uart_send(uart1_read_cmd());
		uart1_send(data_send);
		uart_send(uart1_read_cmd());                                    // Read the response
		
		gate=new_gate;
		
		uart1_send("AT+CIPCLOSE=4");
		uart_send(uart1_read_cmd());
		
		uart1_send("AT+CIPSTART=4,\"TCP\",\"200.1.16.248\",80");       // Gate time works like the delay
		uart_send(uart1_read_cmd());

		tot_overflow=OVERFLOW(gate);                                    // Same as the initialization
	}
	tot_overflow--;
	
*/
}



