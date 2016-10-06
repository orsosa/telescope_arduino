
//Servo -> Timer1 on PWM mode

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>

#define angle2counts(angle) (int)(89*(angle) + 16000)
 // angle in degrees, Proportional relation : 1ms->16000counts-> 0º, 2ms->32000counts -> 180º
#define counts2angle(counts) (int)(((counts) - 16000.0)/89.0)

class Servo
{	
public: 
int kAngle;
int kDuty;

 Servo() : kAngle(0) {kDuty =angle2counts(kAngle)/65535*100;}
void init(void);

void setAngle(int ang_pos);

};




	

