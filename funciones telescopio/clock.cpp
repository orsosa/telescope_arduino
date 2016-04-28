// Timer 1 -> General Clock 

int timer0_overflow_count = 0;
unsigned long timer0_millis = 0;
unsigned char timer0_fract;


void timer0_init(){
	TCCR0B |= (1 << CS01)|(1 << CS00);
	TIMSK0 |= (1 << TOIE0);
	sei();
	TCNT0 = 0;
}
	
SIGNAL(TIMER0_OVF_vect)
{
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;
 
    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }
 
    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}
    
unsigned long millis()
{
    unsigned long m;
    uint8_t oldSREG = SREG;
 
    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();
    m = timer0_millis;
    SREG = oldSREG;    // Set the previus parameters (Status Register) 
    return m;
}
