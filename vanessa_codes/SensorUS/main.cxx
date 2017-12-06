#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "Communication.h"

/*  HC_SR04 ULTRASONIC SENSOR using Atmega2560
 *  Trigger pin = PC4 / Digital pin 33
 *  Echo pin    = PC3 / Digital pin 34
 */

uint16_t rising, falling;
volatile uint32_t counts;
volatile uint32_t dist;
uint16_t us_per_count;



/*uint32_t getDistance()
{
  //Digital pin 34
    PORTC |= 1 << PINC4;
    _delay_us(12);
    PORTC &= ~(1 << PINC4);
    
  if (TCCR4B & (1<<ICES4)) 
  {
    TCCR4B &= ~(1<<ICES1); 
    rising = ICR1;
  }
  else 
  {
    TCCR4B |= (1<<ICES4); // Next time detect falling edge
    falling = ICR4; // Save current count
    counts = (uint32_t)falling - (uint32_t)rising;
    dist = (uint32_t)us_per_count * counts / 58; // useconds / 58 to get distance in cm
  }
}

*/
void init_timer4()
{
  
  TCCR4A &= ~(1 << WGM41)& ~(1 << WGM40);
  TCCR4B &= ~(1 << CS42)& ~(1 << CS40)&~(1 << WGM42)&~(1 << WGM43); // prescaler 8;
  TCCR4B |= (1 << CS41);


  //TIMSK4 |= 1 << OCIE4A | 1 << ICIE4;
  
  
  //TCCR4B |= 1 << ICES4; // Input capture on rising edge
 
  //OCR4A = 8750;
  //sei();
  //us_per_count = 16; //
  TCNT4 = 0 ;
}

int main()
{
  char inicial[16]="Mandando Pulso";   
  char up[8]="Subio!";
  char down[8]="Bajo!"; 
  
  //Inicializaciones
  SerialCom uart;
  DDRA |= 1 << PINA6;     // Pin 28 -> Trigger
  DDRC &= ~(1 << PINC7);  //Pin 30 -> Echo
  PORTA &= ~(1 << PINA6);
  uart.init(57600);
  char buff[6];
  char n[2]="\n";
  init_timer4();  

while(1)
  {
  //Se manda el Pulso inicial
  uart.sendData(inicial);
  uart.sendData(n);
  PORTA |= 1 << PINA6;
  _delay_us(15);
  PORTA &= ~(1 << PINA6);
 //_delay_us(15);
  
  while (!(PINC & 0x80)){
  }
  TCNT4 = 0 ;
  //uart.sendData(up);
  //uart.sendData(n);
  
  while (PINC & 0x80){
  }
  falling = TCNT4; // Save current count
  uart.sendData(down);
  uart.sendData(n);
  counts = (uint32_t)falling;
  dist = (uint32_t) counts / (58*2); // useconds / 58 to get distance in cm
 
  itoa(dist, buff,10);
  uart.sendData(buff);
  uart.sendData(n);
  _delay_ms(2000);
  }
}
