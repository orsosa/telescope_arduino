#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#define TRIG 0x80 //C7
#define ECHO 0x40 //A6

class sensorUS{

private:
    uint16_t falling;
    volatile uint32_t counts;
    volatile uint32_t dist;
    uint16_t us_per_count;
    void init_timer4(void);
    void pulse(void);
    
public:
    sensorUS();
    uint32_t getDistance(void);
};
