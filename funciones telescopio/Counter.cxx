#include "Counter.h"

void Counter::init(void)
{
	TCCR5B |= (1 << CS52)|(1 << CS51);          // External clock mode ------> On Pin 47 in Arduino ATMEGA1280
	TCNT5 = 0;                                  // coincidence counter
}

