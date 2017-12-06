#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

# define EN 0x20 // activa el motor PH5

class Stepper{
private:
    volatile uint8_t *dirPort; 
    volatile uint8_t *stepPort;
    uint8_t dirPin; 
    uint8_t stepPin;
    uint8_t FdC;
    void setDir(bool dir);

public:
    Stepper(volatile uint8_t *pDir, volatile uint8_t *pStep, volatile uint8_t dPin, volatile uint8_t sPin, volatile uint8_t fdc);
    void test(void);
    void stepUp(bool dir);
    void stepDown(bool dir);
};
