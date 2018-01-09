#ifndef US_H
#define US_H

#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>


class ultraSonic{
    public:
   
    uint16_t rising;
    uint16_t falling;
    volatile uint32_t counts;
    volatile uint32_t dist;
    char buff[6]; 
    
    ultraSonic();
    ~ultraSonic();
    void init();
    void sendPulse();
    void getDistance();
};

#endif 
