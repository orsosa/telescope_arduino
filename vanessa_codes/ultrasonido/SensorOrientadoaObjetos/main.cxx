#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "Communication.h"
#include "sensorUS.h"

char *float2str(float n);
char str1[8];


int main()
{ 
  SerialCom uart;
  sensorUS sen;
  uint32_t dist;
  char * buff;
  char n[2]="\n";

while(1)
  {
  dist = sen.getDistance(); 
  itoa(dist, buff,10);
  uart.sendData(buff);
  uart.sendData(n);
  _delay_ms(2000);
  }
}


