#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Servo.h"
#include "WifiCom.h"
#include "Communication.h"
//#include "Counter.h"
//#include "GenClock.h"


int main()
{
  Servo s;
  s.setAngle(30);
  WifiCom w;
  w.sendData("Hola");
  
  return 0;
}

