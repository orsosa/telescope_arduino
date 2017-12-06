#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "stepper.h"

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


int main(void){
    int i;
    Stepper stepx(&PORTE, &PORTE, X_DIR, X_STP, X_FdC);
    Stepper stepy(&PORTH, &PORTE, Y_DIR, Y_STP, Y_FdC);
    Stepper stepz(&PORTH, &PORTG, Z_DIR, Z_STP, Z_FdC);
    Stepper stepa(&PORTB, &PORTB, A_DIR, A_STP, A_FdC);
    
    while(1){
        for(int i = 0; i<200; i++){
            stepx.stepUp(true);
            stepy.stepUp(true);
            stepz.stepUp(true);
            stepa.stepUp(true);
            //_delay_ms(60);
            stepx.stepDown(true);
            stepy.stepDown(true);
            stepz.stepDown(true);
            stepa.stepDown(true);
            //_delay_ms(60);
            
        }
        _delay_ms(1000);
        for(int i = 0; i<200; i++){
            stepx.stepUp(false);
            stepy.stepUp(false);
            stepz.stepUp(false);
            stepa.stepUp(false);
            //_delay_ms(60);
            stepx.stepDown(false);
            stepy.stepDown(false);
            stepz.stepDown(false);
            stepa.stepDown(false);
            //_delay_ms(60);
            
        }
        _delay_ms(1000);
    }
    return 1;

}
