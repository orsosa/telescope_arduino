#ifndef FDCR_H
#define FDCR_h

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

class Fdcr{
    
    public:
    bool val1;
    bool val2;
    bool val3;
    bool val4;    
    
    Fdcr();
    ~Fdcr();
    void init();
    void readState();


};


#endif
