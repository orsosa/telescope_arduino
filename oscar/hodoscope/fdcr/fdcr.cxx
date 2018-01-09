#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "fdcr.h"

Fdcr::Fdcr(){};
Fdcr::~Fdcr(){};

void Fdcr::init(){
        
    DDRB  &= ~_BV(DDB1)&~_BV(DDB3);//~(1<<DDB1)&~(1<<DDB3);
    PORTB &= ~_BV(PB1)&~_BV(PB3);//~(1<<PB1)&~(1<<PB3);    
    DDRL  &= ~_BV(DDL1)&_BV(DDL3);//~(1<<DDL1)&~(1<<DDL3);
    PORTL &= ~_BV(PL1)&~_BV(PL3);//~(1<<PL1)&~(1<<PL3);
      
}

void Fdcr::readState(){

    val1 = PINL &_BV(PL1);
    val2 = PINL &_BV(PL3);
    val3 = PINB &_BV(PB1);
    val4 = PINB &_BV(PB3);
}
