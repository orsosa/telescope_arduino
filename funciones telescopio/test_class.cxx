#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Servo.h"

int main()
{
  Servo s;
  s.setAngle(30);
  return 0;
}
