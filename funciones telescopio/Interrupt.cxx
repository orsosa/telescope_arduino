#include "Interrupt.h"

void Interrupt::init(void)
{
	TCCR3B |= (1 << CS32)|(1 << CS30);          					// Set 1024 prescaling
	kTotalOverflow = OVERFLOW(kGate);              		     			// Set the total overflows for a given gate time
	TCNT3 = INIT_COUNT(kGate-MAX_GATE*OVERFLOW(kGate))*(kTotalOverflow==0);  	// Counter value depends on overflow value
	TIMSK3 |= (1 << TOIE3);                                              
	sei();										// Enable global interrupts
	kTotalOverflow --;
}








	
