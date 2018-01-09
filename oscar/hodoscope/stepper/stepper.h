#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

# define EN 0x20 // activa el motor PH5
# define X_DIR  0x08 // setea la direccion del eje x PE3
# define Y_DIR  0x08 // setea la direccion del eje y PH3
# define Z_DIR  0x10// setea la dirección del eje x PH4
# define A_DIR  0x80 // setea la direccin del motor a PB7

# define X_STP  0x10// define el numero de pasos del eje x PE4
# define Y_STP  0x20// define el numero de pasos del eje y PE5
# define Z_STP  0x20// define el numero de pasos del eje z PG5
# define A_STP  0x40// define el numero de pasos del motor a PB6

# define X_FdC  0x01// define el numero de pasos del eje x PA0
# define Y_FdC  0x02// define el numero de pasos del eje y PA1
# define Z_FdC  0x04// define el numero de pasos del eje z PA2
# define A_FdC  0x08// define el numero de pasos del motor a PA3

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
