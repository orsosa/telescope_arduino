#include "sensorUS.h"

uint16_t init;

sensorUS::sensorUS(){
    DDRC |= TRIG;     // Pin 30 -> Trigger
    DDRA &= ~(ECHO);  //Pin 28 -> Echo
    PORTC &= ~(TRIG); // Inicializa la salida en 0   
    init_timer4();
}

void sensorUS::init_timer4(){
  TCCR4A &= ~(1 << WGM41)& ~(1 << WGM40);
  TCCR4B &= ~(1 << CS42)& ~(1 << CS40)&~(1 << WGM42)&~(1 << WGM43); // prescaler 8;
  TCCR4B |= (1 << CS41);
  TCNT4 = 0 ;
}

void sensorUS::pulse()
{
  PORTC |= TRIG;
  _delay_us(15);
  PORTC &= ~(TRIG);
}
void sensorUS::getDistance(){
    //Manda pulso inicial
  TCNT4=0; // warning reset timer could take come time.
  pulse();
  while (!(PINA & ECHO)); 
  init=TCNT4;
  
  while (PINA & ECHO); 
  distance = (uint32_t) (TCNT4-init) / (58*2); // useconds / 58 to get distance in cm.

}
