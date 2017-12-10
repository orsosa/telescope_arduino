#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#define TRIG (1<<PINC7)//0x80 //C7
#define ECHO (1<<PINA6)//0x40 //A6

class sensorUS{
  
 private:  
  uint16_t us_per_count;
  void pulse();
  void init_timer4();  

 public:
  uint32_t distance; //cm
  sensorUS();
  void  getDistance();
};
