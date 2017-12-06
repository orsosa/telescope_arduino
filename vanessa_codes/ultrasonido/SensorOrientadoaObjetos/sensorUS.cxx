#include "sensorUS.h"


sensorUS::sensorUS(){
    DDRC |= TRIG;     // Pin 30 -> Trigger
    DDRA &= ~(ECHO);  //Pin 28 -> Echo
    PORTC &= ~(TRIG); // Inicializa la salida en 0   
    
    sensorUS::init_timer4();
}

void sensorUS::init_timer4(){
  TCCR4A &= ~(1 << WGM41)& ~(1 << WGM40);
  TCCR4B &= ~(1 << CS42)& ~(1 << CS40)&~(1 << WGM42)&~(1 << WGM43); // prescaler 8;
  TCCR4B |= (1 << CS41);
  TCNT4 = 0 ;
}



uint32_t sensorUS::getDistance(){
    //Manda pulso inicial
   PORTC |= TRIG;
   _delay_us(15);
   PORTC &= ~(TRIG);

    while (!(PINA & ECHO)){
    }
    TCNT4 = 0 ;

    while (PINA & ECHO){
    }

    falling = TCNT4; // Save current count
    counts = (uint32_t)falling;
    dist = (uint32_t) counts / (58*2); // useconds / 58 to get distance in cm
    return dist;    
}
