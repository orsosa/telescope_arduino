#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

/*#define DD_MOSI PD5
#define DD_MISO PD6
#define DD_SCK PD7
#define DDR_SPI DDRB
*/


uint8_t valor=0;

void SPI_SlaveInit(void)
{	
/*	//DDRD&=~(1<<PD0);
	PORTD|=(1<<PD0);
	//EICRB&=~((1<<ISC00)|(1<<ISC01));
	EIMSK|=(1<<INT0);
	sei();
*/

		}

uint8_t SPI_SlaveReceive(void)
{
	uint8_t he=1,in=0, heant=0,leer,final=0;
	
	
	//PORTA|=(1<<PA0);
	//PORTB|=(1<<PB6);
	
	while(he<=8){
		
		
	
	if(PINC&(1<<PC0)){
			//while(PINA&(1<<PA2));
		//PORTB|=(1<<PB6);
		/*_delay_ms(10);
		PORTB &=~(1<<PB6);
		_delay_ms(10);
		*/
		//leyendo el CLock externo
		
			//PORTB |=(1<<PB6);
		if(PINC&(1<<PC4)){
			in=1; //leendo el dato 
			final=(final<<1)+in;
			//final=(final<<1)+in;
			//PORTB|=(1<<PB7);
			he++;
			}else	{
			in=0;
			final=(final<<1)+in;
			//PORTB&=~(1<<PB7);
			he++;}
	
	
	while(PINC&(1<<PC0));
	//PORTB&=~(1<<PB6);
	
	//for (int i=1; i<=8; i++){
	//PORTB|=(1<<PB1);
	//if(PINB&(1<<PB3)){ 


	
//	while(PINA&(1<<PA2));
//}

	//printf("Transferencia terminada \n");
}
	}
		//PORTB &=~(1<<PB6);
	 	//PORTA &=~(1<<PA0);
		/*if(PINA&(1<<PA2)){ PORTB|=(1<<PB7);
			_delay_ms(1000);
			PORTB&=~(1<<PB7);
			_delay_ms(10);
		}*/
		
		PORTB&=~(1<<PB7);
		
	return final;
	}
	

/*ISR(INT0_vect){
	
	PORTB|=(1<<PB6);
	_delay_ms(500);
	valor=SPI_SlaveReceive();
	
	/*PORTB&=~(1<<PB6);
	_delay_ms(100);
	//while(1);
}*/



	
int main()
{
	
	
	//set interrupt
	/*DDRE&=~(1<<PE0);
	PORTE|=(1<<PE0);
	//EICRB&=~((1<<ISC00)|(1<<ISC01));
	EIMSK|=(1<<INT0);
	sei();
	//set OUT
	DDRB|=((1<<PB7)|(1<<PB6));
	DDRC=0xFF;
	PORTA|=((1<<PA2)|(1<<PA3));//(1<<PA1)|
	
	*/
	//uint8_t valor=0;
	DDRB|=(1<<PB7);
	
	
	
	SPI_SlaveInit();
	
	while(1)
	{
		//PORTB &=~(1<<PB7);
		
		if(!(PINC&(1<<PC6))) {valor=SPI_SlaveReceive();
		
		for(int a=0;a<valor;a++){
			//PORTB|=(1<<PB7);
			//_delay_ms(100);
			PORTB|=(1<<PB7);
			_delay_ms(100);			
			PORTB&=~(1<<PB7);
			_delay_ms(200);
							
			}
		}
			/*else{PORTB|=(1<<PB7);
			_delay_ms(10);
			PORTB&=~(1<<PB7);
			_delay_ms(10);
			PORTB|=(1<<PB7);
			_delay_ms(10);
			PORTB&=~(1<<PB7);
			_delay_ms(10);
		}*/
		
		//PORTC = valor;
		
	
		
		/*if (valor==8) */
		//PORTB|=(1<<PB7);
		//_delay_ms(1000);
		//PORTB &=~(1<<PB7);
		//_delay_ms(50);
	}
	return 0;
}
