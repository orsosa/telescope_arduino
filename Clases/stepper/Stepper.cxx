#include <avr/io.h>
#include "Stepper.h"

void Stepper::stepperInit()
{
	__STEPPER_DDR|=(0X0F<<STEPPER_POS);
	__STEPPER_PORT|=(1<<STEPPER_POS);
}

void Stepper::stepperStepCCW()
{
	uint8_t temp=(__STEPPER_PORT & (0x0F<<STEPPER_POS));
	temp=temp>>(STEPPER_POS);
	temp=temp<<1;

	if(temp>(8))
		temp=1;
	
	__STEPPER_PORT=(__STEPPER_PORT & ~(0x0F<<STEPPER_POS))|(temp<<STEPPER_POS);
}

void Stepper::stepperStepCW()
{
	uint8_t temp=(__STEPPER_PORT & (0x0F<<STEPPER_POS));
	temp=temp>>(STEPPER_POS);
	temp=temp>>1;

	if(temp==0)
		temp=8;
	
	__STEPPER_PORT=(__STEPPER_PORT & ~(0x0F<<STEPPER_POS))|(temp<<STEPPER_POS);
}