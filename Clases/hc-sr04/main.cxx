
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "math.h"

#define UART_BAUD_RATE 57600
#include "uart.h"
#include "Sonar.h"

int main()
{
	uart_init();
	sei();
	char itmp[10];
	Sonar s;
	while(1)
	{
		s.readDistance();
		itoa(s.kDistance_in_cm, itmp, 10);
		uart_puts(itmp);
		uart_puts("\r\n");
	}
	
}
	
	
