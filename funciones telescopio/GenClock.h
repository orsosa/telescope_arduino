
//General system clock -> Timer0

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class GenClock
{
	public:
	
	GenClock();
	void init(void);
	unsigned long millis(void);
}


