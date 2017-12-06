//// Counter-Timer 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>

class Counter
{
	public:
	int kCounts;
	
	Counter() : kCounts(0) {;}
	void init(void);
	
};