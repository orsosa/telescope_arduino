#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "us.h"

ultraSonic::ultraSonic(){};
ultraSonic::~ultraSonic(){};

void ultraSonic::init(){
    //Timer init
    TCCR4A &= ~(1 << WGM41)& ~(1 << WGM40);
    TCCR4B &= ~(1 << CS42)& ~(1 << CS40)&~(1 << WGM42)&~(1 << WGM43); // prescaler 8;
    TCCR4B |= (1 << CS41);
    TCNT4 = 0 ;

    //Port init
    DDRC |= 1 << PINC7;     // Pin 30 -> Trigger
    DDRA &= ~(1 << PINA6);  //Pin 28 -> Echo
    PORTC &= ~(1 << PINC7);
}

void ultraSonic::sendPulse(){
    PORTC |= 1 << PINC7;
    _delay_us(15);
    PORTC &= ~(1 << PINC7);    
}

void ultraSonic::getDistance(){
    
    while (!(PINA & 0x40)){
    }
    TCNT4 = 0 ;
    while (PINA & 0x40){
     }
    falling = TCNT4;
    counts = (uint32_t)falling;
    dist = (uint32_t) counts / (58*2); // useconds / 58 to get distance in cm    
    itoa(dist, buff,10);
}
