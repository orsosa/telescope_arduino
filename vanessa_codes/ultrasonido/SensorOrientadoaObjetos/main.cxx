#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "sensorUS.h"
#include "Communication.h"

char *float2str(float n);
char str1[8];


int main()
{ 
  SerialCom uart;
  sensorUS sen;
  
  char * buff;
  char n[2]="\n";

  uint32_t dist;
  uint16_t init;
  char inicial[16]="Mandando Pulso";   
  char up[8]="Subio!";
  char down[8]="Bajo!"; 

  while(1)
  {
    sen.getDistance();
    itoa(sen.distance, buff,10);
    uart.sendData(buff);
    uart.sendData(n);
    _delay_ms(1000);
  }
}


