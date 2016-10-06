#include "Servo.h"

void Servo::init(void)
{
	// initialize TCCR0 as per requirement, say as follows
  //??TCCR1B |= (1<<WGM10)|(1<<COM11)|(1<<WGM11)|(1<<CS10);
  TCCR1B |= (1<<WGM10)|(1<<CS11)|(1<<WGM11)|(1<<CS10);//mod COM11->CS11
  
	// make OC0 pin (pin PB3 for atmega32) as output pin
    DDRB |= (1<<PB3);

    //initialize counter??
}


//Timer1 is 16-bit counter -> Max_counts = 65535 -> 4.096ms

void Servo::setAngle(int ang_pos)
{
  int counts = angle2counts(ang_pos);
  OCR1A = counts;
  kAngle = ang_pos;
  kDuty = (counts/65535)*100;
}
