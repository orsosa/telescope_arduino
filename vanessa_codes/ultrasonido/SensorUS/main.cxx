#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "Communication.h"

/*  HC_SR04 ULTRASONIC SENSOR using Atmega2560
 *  Trigger pin = PC7 / Digital pin 30
 *  Echo pin    = PA6 / Digital pin 28
 */

uint16_t rising, falling;
volatile uint32_t counts;
volatile uint32_t dist;
uint16_t us_per_count;


void init_timer4()
{
  TCCR4A &= ~(1 << WGM41)& ~(1 << WGM40);
  TCCR4B &= ~(1 << CS42)& ~(1 << CS40)&~(1 << WGM42)&~(1 << WGM43); // prescaler 8;
  TCCR4B |= (1 << CS41);
  TCNT4 = 0 ;
}

int main()
{
  char inicial[16]="Mandando Pulso";   
  char up[8]="Subio!";
  char down[8]="Bajo!"; 
  
  //Inicializaciones
  SerialCom uart;
  DDRC |= 1 << PINC7;     // Pin 30 -> Trigger
  DDRA &= ~(1 << PINA6);  //Pin 28 -> Echo
  PORTC &= ~(1 << PINC7);
  uart.init(57600);
  char buff[6];
  char n[2]="\n";
  init_timer4();  

while(1)
  {
  //Se manda el Pulso inicial
  uart.sendData(inicial);
  uart.sendData(n);
  PORTC |= 1 << PINC7;
  _delay_us(15);
  PORTC &= ~(1 << PINC7);
  
  while (!(PINA & 0x40)){
  }
  TCNT4 = 0 ;
  
  while (PINA & 0x40){
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
