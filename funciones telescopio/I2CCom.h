//I2C Communication

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <compat/twi.h>

class I2CCom
{
	public:
	unsigned int kI2CAddress;
	unsigned int kDevID;
	unsigned int kDevAddress;
	char data;
	
	void init();
	void 
}