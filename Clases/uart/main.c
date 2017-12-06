#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>


#define BAUD 57600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#include "uart.h"

int main()
{
	uart_init();
	sei();
	while(1){uart_puts(uart_read()); uart_puts("\r\n");}
	return 0;
}
