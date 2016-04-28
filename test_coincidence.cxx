#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <avr/interrupt.h>	

//Coincidence
float cont = 0;
int cont2 = 0;   
float f_inst = 0;
int taim = 0;
float mov_av = 0;


//Global variable to count the number of overflows
volatile uint8_t tot_overflow;

//Timer inialization 
void timer0_init()
{
	TCCR1B|= (1 << CS11);   // set up timer1 with 8 prescaler
	TIMSK1|= (1 << TOIE1);  // enable overflow interrupt 
	TCNT1 = 0;               // initialize counter 
	sei();                   // enable global interrupts
	tot_overflow = 0;
}


// TIMER1 overflow interrupt service routine
// called whenever TCNT1 overflows
ISR(TIMER1_OVF_vect)
{
	tot_overflow++;    //keep a track of number of overflows
	if (tot_overflow >= 61)
	{
		PORTB ^= (1<<PB7);
		tot_overflow = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	
	DDRB|=(1<<PB7); 	//Output LED for counting coincidences
	timer0_init();      
	
	/*while(1) {
		if (!((~PINC)&0x0C))     //Coincidence
		{      
		  cont++;
		  cont2++;
	  
			//Cálculo de frecuencia 
			if (cont2 %10 == 0)       //Se calcula un nuevo valor cada 10 coincidencias
			{
				taim = TCNT1-taim;
				f_inst = 10./(taim*0.064/1000);      // 	Frecuencia instantánea
				taim = TCNT1; 
				mov_av = 0.7*f_inst + 0.3*mov_av;   // Some Filter   
			}
		  _delay_ms(2.5);               // Pulse width
		}
	  }*/    

	
	return 0;
}


