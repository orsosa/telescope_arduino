#include "Servo.h"


//Set the servo in angle 0
int Servo::init(){

    TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); //Non inverted PWM
    TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);// Prescaler =64 Mode 14 (fast PWM)

    ICR1 = 4999;  //fPWM=50Hz (Period = 20ms Standard).

    DDRB |= (1<<PB5);   //PWM Pin 11 as Out
    
    int i = lastDuty;    
    
    for(i; i>96; i--){
        OCR1A = i;
        _delay_ms(100);
    }
    lastDuty = 97;
    return lastDuty;
}


void Servo::test(){
    for (int i = 97;i < 536; i++){
    // 97 is 0 degree
    // 535 is 180 degree
        OCR1A = i;
        _delay_ms(100);
        lastDuty = i;
    }
    for (int i = 535;i < 966; i--){
        OCR1A = i;
        _delay_ms(100);
        lastDuty = i;
    }

}

int Servo::setAngle(int angle){
    double duty = (2.4333* angle) + 97;
    int i = lastDuty;
    if (duty < lastDuty){
        for (i; i > (int) duty; i-- ){    
            OCR1A = i;
            _delay_ms(100);
            lastDuty = i;
        }
    }
    if (duty > lastDuty){
        for (i; i < (int) duty; i++ ){    
            OCR1A = i;
            _delay_ms(100);
            lastDuty = i;
        }
    }

    return lastDuty;

}


