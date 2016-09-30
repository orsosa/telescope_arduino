//// First Aproach for the database actualization

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/wdt.h>     // watchdog

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

//// For timer0 and millis() function 

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


float new_gate = 20;
float new_pa;         // Values read from database query response
float new_aa;
float new_dx;
float timeout;

int go_ahead = -1;           // Flag for avoid the interrupt coincidences 

// There exist a limit to the gate value to avoid TIMER5'S reset. TCNT5 cannot overcome 65535 in the gate time.

float gate = 20;  //   initialization value, in seconds ...

bool begin_en = true;

//////////// Global Clock Timer0

/*

int timer0_overflow_count = 0;
unsigned long timer0_millis = 0;
unsigned char timer0_fract;

void timer0_init(){
	TCCR0B |= (1 << CS01)|(1 << CS00);
	TIMSK0 |= (1 << TOIE0);
	sei();
	TCNT0 = 0;
}
	
SIGNAL(TIMER0_OVF_vect)
{
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;
 
    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }
 
    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}
    
unsigned long millis()
{
    unsigned long m;
    uint8_t oldSREG = SREG;
 
    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value 
    cli();
    m = timer0_millis;
    SREG = oldSREG;    // Set the previus parameters (Status Register) 
    return m;
}

*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////      COMMUNICATION      //////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Watchdog off

void WDT_off(void)
{
cli();
wdt_reset();
/* Clear WDRF in MCUSR */
MCUSR &= ~(1<<WDRF);
/* Write logical one to WDCE and WDE */
/* Keep old prescaler setting to prevent unintentional time-out
*/
WDTCSR |= (1<<WDCE) | (1<<WDE);
/* Turn off WDT */
WDTCSR = 0x00;
sei();
}


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
char msg[1000];                // my buffer to store data

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
char data_send[59] = "GET /orsosa/store.php?word=LetMeIn!&field=freq&value=";             // Buffer to frequency value storage      
double freq;
char freq_char[4];



// Counter5 initialization
void timer5_init()
{
	TCCR5B |= (1 << CS52)|(1 << CS51);          // External clock mode ------> On Pin 47 in Arduino ATMEGA1280
	TCNT5 = 0;                                  // coincidence counter
}



// Timer3 initialization
void timer3_init()
{
	TCCR3B |= (1 << CS32)|(1 << CS30);          							// Set 1024 prescaling
	tot_overflow = OVERFLOW(gate);              							// Set the total overflows for a given gate time
	TCNT3 = INIT_COUNT(gate-MAX_GATE*OVERFLOW(gate))*(tot_overflow==0);     // Counter value depends on overflow value
	TIMSK3 |= (1 << TOIE3);                                                 
	sei();																	// Enable global interrupts
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



// Timer3 Overflow Interrupt     -------------->   Here the data is send
ISR(TIMER3_OVF_vect)
{
	
	TCNT3 = INIT_COUNT(gate-MAX_GATE*OVERFLOW(gate))*(tot_overflow==0);    // Counter value depends on overflow value, same as the initialization  
	if(tot_overflow<0)                                                     // Condition for send the value (gate time is reached)
	{
		uart1_send("··················");
		uart1_flush();
		
		
		freq = (float)((TCNT5)/gate);                                    
		TCNT5=0;														   // Reset the coincidence counter
		dtostrf(freq,6, 3, (char*)freq_char);                              //dtostrf() on stdlib.h, to cast a float to char*
		data_send[53] = freq_char[1];
		data_send[54] = freq_char[2];
		data_send[55] = freq_char[3];
		data_send[56] = freq_char[4];
		data_send[57] = '\0';
		
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
		
		//uart1_flush();
		
		tot_overflow=OVERFLOW(gate);                                    // Same as the initialization
	}
	tot_overflow--;
	

}



void prog_init()      //  Initialization function ...
{
	WDT_off();
	timer5_init();
	timer3_init();                      // Initialization 
	uart_init();
	uart1_init();
	
	
	uart1_send("AT+RST"); _delay_ms(2000);            // Reset the module
	uart_send("RESET OK");
	
	uart1_send("AT+CIPMUX=1"); _delay_ms(300);        // Configured for multiple connections
	uart_send("CIPMUX OK");
	
	uart1_send("AT+CWMODE=1"); _delay_ms(300);        // Configured as STA
	uart_send("CWMODE OK");
	
	uart1_send("AT+CWJAP=\"atlasgw\",\"SiLabaLiS\""); _delay_ms(5000);       // Connect to Local Network
    uart_send(uart1_read_cmd());
	
	uart1_send("AT+CIPSTART=4,\"TCP\",\"200.1.16.248\",80");       // Port to Send Data
	_delay_ms(500);
	uart1_send("AT+CIPSTART=3,\"TCP\",\"200.1.16.248\",80");       // Port to Read Dara
	_delay_ms(500);
	uart_send("ready");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////     MAIN     ////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	prog_init();
	
	while(1)     
	{

		uart1_send("AT+CIPSEND=3,25");
		uart1_flush();
		_delay_ms(100);
	
		uart1_send("GET /orsosa/getdata.php");                 				// Send the query
		uart_send(uart1_read_query());
	
		uart_send("-----------------");
		uart_send(msg1);
		uart_send("-----------------");

		new_gate = get_par("gate");
		
		uart1_send("AT+CIPCLOSE=3");      

		_delay_ms(1000);
		uart1_send("AT+CIPSTART=3,\"TCP\",\"200.1.16.248\",80");       // Gate time works like the delay
		//uart_send(uart1_read_cmd());
		_delay_ms(500);
		
	}
	
	return 0;
}
