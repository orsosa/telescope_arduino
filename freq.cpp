#define F_CPU 16000000UL
#include <avr/io.h>

void timer1_init()
{
	TCC1RB |= (1 << C10);
	TCNT1 = 0;
}
//

int main()
{
	DDRA |= (1 << PA0);
	while(1){PORTA ^= (1 << 0);}
	return 0;
}