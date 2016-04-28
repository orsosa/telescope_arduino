//// First Aproach for the database actualization

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


float new_gate;
float new_pa;         // Values read from database query response
float new_aa;
float new_dx;

int go_ahead = -1;           // Flag for avoid the interrupt coincidences 
bool begin_en = false;

// There exist a limit to the gate value to avoid TIMER5'S reset. TCNT5 cannot overcome 65535 in the gate time.

float gate = 20;  //   initialization value, in seconds ...


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////      COMMUNICATION      //////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////	Serial Communication 

//Inicialization
void uart_init(void)
{
	UBRR0H = (BAUDRATE >> 8);                  //Set Baud Rate
	UBRR0L = BAUDRATE;
	UCSR0B |= (1 << TXEN0)|(1 << RXEN0);	   //Transmit and Recieve enable
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);         // Set 8-bit data transfer
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


//Function to clean RCX register
void uart_flush(void)
{
unsigned char dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}


//Function to send a char array
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

//Function to read a char array (from UDR0 buffer)
char msg[100];                // my buffer to store data

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


//////////////////////////////////	Serial1 Communication (WiFi Module)

//Inicialization
void uart1_init(void)
{
	UBRR1H = (WBAUDRATE >> 8);             // Set Baud Rate
	UBRR1L = WBAUDRATE;
	UCSR1B |= (1 << TXEN1)|(1 << RXEN1);   // Transmit and Recieve enable
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);     // Set 8-bit data transfer
}

//Function to send data
void uart1_transmit(unsigned char data)
{
   // loop_until_bit_is_set(UCSR1A, UDRE1);       // alternative way 
   while (!( UCSR1A & (1<<UDRE1)));               // wait while register is free
   UDR1 = data;                                   // load data in the register
}

//Function to receive data
char msg0[100];
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

//Function to send a char array
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

//Function to read a char array (from UDR1 buffer)   ------------->  Designed for ESP8266 responses

char msg1[1000];             // my buffer to store data


////////////Command Responses

char *uart1_read_cmd(bool type=0)
{
int i=0;
unsigned char c;
int count_ok=0;
while   (i<1000) //
{
	//if(UART1_AVAILABLE)               //alternative way
    c=uart1_recieve();
	msg1[i]=c;
	if ((msg1[i-1]=='O') && (msg1[i]=='K')) break;           // #! characteristic flag
	if ((msg1[i-4]=='E') && (msg1[i-3]=='R')&&(msg1[i-2]=='R') && (msg1[i-1]=='O')&& (msg1[i]=='R')) break;  // break when an error is announced 
	i++;
}
msg1[i+1]='\0';
char  hola[10];
sprintf(hola, "LAST I: %d\n",i);
uart_send(hola);
return msg1;
}


//////////////Query Responses

char *uart1_read_query(bool type=0)
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
		msg1[i]=c;
		if ((msg1[i-1]=='#') && (msg1[i]=='!')) break;           // #! characteristic flag
		if ((msg1[i-4]=='E') && (msg1[i-3]=='R')&&(msg1[i-2]=='R') && (msg1[i-1]=='O')&& (msg1[i]=='R')) break;  // break when an error is announced 
		i++;
	}
}
msg1[i+1]='\0';
//char  hola[10];
//sprintf(hola, "LAST I: %d\n",i);
//uart_send(hola);
return msg1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////     COUNTERS/TIMERS     //////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // Variables for frequency count

int tot_overflow;                									  // Timer3 Total Overflow counter
char data_send[38] = "GET /store.php?field=freq&value=";             // Buffer to frequency value storage      
double freq;
char freq_char[4];
//int count = 0;                 									  //IDK yet


// Counter5 initialization
void timer5_init()
{
	TCCR5B |= (1 << CS52)|(1 << CS51);          // External clock mode ------> On Pin 47 in Arduino ATMEGA1280
	TCNT5 = 0;                                  // coincidence counter
}


//Timer 4 initialization 
 void timer4_init()
{
	TCCR4B |= (1 << CS42)|(1 << CS40);
	TIMSK4 |= (1 << TOIE4);
	sei();
	TCNT4 = 0;
}


// Timer3 initialization
void timer3_init()
{
	TCCR3B |= (1 << CS32)|(1 << CS30);          							// Set 1024 prescaling
	tot_overflow = OVERFLOW(gate);              							// Set the total overflows for a given gate time
	TCNT3 = INIT_COUNT(gate-MAX_GATE*OVERFLOW(gate))*(tot_overflow==0);     // Counter value depends on overflow value
	TIMSK3 |= (1 << TOIE3);                                                 
	sei();																	// Set the interrups enable
	tot_overflow--;
}

///////////////////////////////////////////////////////////////////////////  Interrupts ///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// Get parameters from db
float get_par(char *p){
	char par[20];
	strcpy(par,p);
	strcat(par,":");
	char *ps = strtok(msg1," ");
	while(ps)
	{
		if(!strcmp(ps,par)) break;
		ps = strtok(NULL," ");
	}
	ps = strtok(NULL," ");
	return atof(ps);
}
///////////////

char interrup[5];

// Timer3 Overflow Interrupt     -------------->   Here the data is send
ISR(TIMER3_OVF_vect)
{
	if(begin_en)
	{
		while(go_ahead == 0){};
		go_ahead++;
		
		TCNT3 = INIT_COUNT(gate-MAX_GATE*OVERFLOW(gate))*(tot_overflow==0);    // Counter value depends on overflow value, same as the initialization  
		if(tot_overflow<0)                                                     // Condition for send the value (gate time is reached)
		{
			freq = (float)((TCNT5)/gate);                                    
			TCNT5=0;														   // Reset the coincidence counter
			dtostrf(freq,6, 3, (char*)freq_char);                              //dtostrf() on stdlib.h, to cast a float to char*
			data_send[32] = freq_char[1];
			data_send[33] = freq_char[2];
			data_send[34] = freq_char[3];
			data_send[35] = freq_char[4];
			data_send[36] = '\0';
			
			uart_send("----------");
			uart_send(data_send);
			uart_send("----------");
			
			uart1_send("AT+CIPSEND=4,38");                             
			uart_send(uart1_read_cmd());
			uart1_send(data_send);
			uart_send(uart1_read_cmd());                                    // Read the response
			
			gate=new_gate;
			
			uart1_send("AT+CIPCLOSE=4");
			uart_send(uart1_read_cmd());
			
			uart1_send("AT+CIPSTART=4,\"TCP\",\"192.168.1.216\",80");       // Gate time works like the delay
			uart_send(uart1_read_cmd());
			
			
			tot_overflow=OVERFLOW(gate);                                    // Same as the initialization
		}
		tot_overflow--;
		
		go_ahead--;
	}
}



// Timer4 Overflow Interrupt  -------------->   Here the Database is read

char prueba[10];

ISR(TIMER4_OVF_vect)
{   
	if(begin_en)
	{
		while(go_ahead == 0){};
		go_ahead++;
	
		uart1_send("AT+CIPSEND=4,18");
		uart_send(uart1_read_cmd());
		
		uart1_send("GET /getdata.php");                 				// Send the query
		uart_send(uart1_read_query());
		
		uart_send("5555555555555555");
		uart_send(msg1);
		uart_send("5555555555555555");

		new_gate=get_par("gate");
		sprintf(prueba, "gate: %f", new_gate);
		
		uart_send(prueba);

		uart1_send("AT+CIPSTART=4,\"TCP\",\"192.168.1.216\",80");       // Gate time works like the delay
		uart_send(uart1_read_cmd());

		go_ahead--;
	}
	TCNT4 = 0;
}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////     MAIN     ////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	timer5_init();
	timer3_init();                      // Initialization 
	timer4_init();
	uart_init();
	uart1_init();
	
	uart_send("**************");
	
	uart1_send("AT+RST"); _delay_ms(2000);            // Reset the module
	uart_send("RESET OK");
	
	uart1_send("AT+CIPMUX=1"); _delay_ms(500);        // Configured for multiple connections
	uart_send("CIPMUX OK");
	
	uart1_send("AT+CWMODE=1"); _delay_ms(500);        // Configured as STA
	uart_send("CWMODE OK");
	
	uart1_send("AT+CWJAP=\"atlasgw\",\"SiLabaLiS\""); _delay_ms(5000);       // Connect to Local Network
    uart_send(uart1_read_cmd());
	
	uart1_send("AT+CIPSTART=4,\"TCP\",\"192.168.1.216\",80"); _delay_ms(3000);       // Connect to Server
	uart_send("ready");
	
	begin_en = true;
	
	while(1);
	return 0;
}
