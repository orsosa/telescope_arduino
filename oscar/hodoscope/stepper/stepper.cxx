#include "stepper.h"

Stepper::Stepper(volatile uint8_t *pDir, volatile uint8_t *pStep, volatile uint8_t dPin, volatile uint8_t sPin, volatile uint8_t fdc){
    dirPort = pDir;
    stepPort = pStep;
    dirPin = dPin;
    stepPin = sPin;
    FdC = fdc;

    //DDRX, dirPin and stepPin as output
    *(dirPort-1) |= dirPin;
    *(stepPort-1) |= stepPin;

    //Enable CNC Shield
    DDRH |= EN;
    PORTH &= ~EN;
    //Fin de carrera
    DDRA &= ~fdc; 
    
    //Put the output low 
    *stepPort &= ~stepPin;
}

void Stepper::setDir(bool dir){
    if (dir){
        *dirPort |= dirPin;
    }
    else {
        *dirPort &= ~dirPin;
    }
}

void Stepper::stepUp(bool dir){
    //Set the direction
    Stepper::setDir(dir);
    //if (!dir){
        *stepPort |= stepPin;
        _delay_us(200);

        *stepPort &= ~stepPin;
        _delay_us(200);
    //}
  
}

void Stepper::stepDown(bool dir){
    //Set the direction
    Stepper::setDir(dir);
    *stepPort |= stepPin;
            _delay_us(200);

            *stepPort &= ~stepPin;
            _delay_us(200);
}

void Stepper::test(){
    for(int i=0;i<=200;i++){
        Stepper::stepUp(true);
        Stepper::stepDown(true);
    }
    _delay_ms(2000);
    for( int i=0;i<=200;i++){
        Stepper::stepUp(false);
        Stepper::stepDown(false);
    }    
    _delay_ms(2000);  
}

