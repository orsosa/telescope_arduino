#include "Sonar.h"
 
volatile uint32_t overFlowCounter = 0;
volatile uint32_t trig_counter = 0;
volatile uint32_t no_of_ticks = 0;
 
/********** ...- . . .-. --- -... --- - *********************************
 * Initiate Ultrasonic Module Ports and Pins
 * Input:   none
 * Returns: none
*********** ...- . . .-. --- -... --- - *********************************/
void Sonar::initSonar(){
    TRIG_OUTPUT_MODE();     // Set Trigger pin as output
    ECHO_INPUT_MODE();      // Set Echo pin as input
	
}
 
/********** ...- . . .-. --- -... --- - *********************************
 * Send 10us pulse on Sonar Trigger pin
 * 1.   Clear trigger pin before sending a pulse
 * 2.   Send high pulse to trigger pin for 10us
 * 3.   Clear trigger pin to pull it trigger pin low
 *  Input:   none
 *  Returns: none
********** ...- . . .-. --- -... --- - *********************************/
void Sonar::triggerSonar(){
    TRIG_LOW();             // Clear pin before setting it high
    _delay_us(1);           // Clear to zero and give time for electronics to set
    TRIG_HIGH();            // Set pin high
    _delay_us(12);          // Send high pulse for minimum 10us
    TRIG_LOW();             // Clear pin
    _delay_us(1);           // Delay not required, but just in case...
}
 
/********** ...- . . .-. --- -... --- - *********************************
 * Increment timer on each overflow
 * Input:   none
 * Returns: none
********** ...- . . .-. --- -... --- - *********************************/
ISR(TIMER1_OVF_vect){   // Timer1 overflow interrupt
    overFlowCounter++;
    TCNT1=0;
}
 
/********** ...- . . .-. --- -... --- - *********************************
 * Calculate and store echo time and return distance
 * Input:   none
 * Returns: 1. -1       :   Indicates trigger error. Could not pull trigger high
 *          2. -2       :   Indicates echo error. No echo received within range
 *          3. Distance :   Sonar calculated distance in cm.
********** ...- . . .-. --- -... --- - *********************************/
void Sonar::readDistance(){
    //int dist_in_cm = 0;
    initSonar();                       // Setup pins and ports
    triggerSonar();                    // send a 10us high pulse
	
	TCCR1B |= (1 << CS10);                                       

 
    while(!(ECHO_PIN & (1<<ECHO_BIT))){   // while echo pin is still low
        trig_counter++;
         uint32_t max_response_time = SONAR_TIMEOUT;
        if (trig_counter > max_response_time){   // SONAR_TIMEOUT
            kDistance_in_cm = TRIG_ERROR;
        }
    }
 
    TCNT1=0;                            // reset timer
    TCCR1B |= (1<<CS10);              // start 16 bit timer with no prescaler
    TIMSK1 |= (1<<TOIE1);             // enable overflow interrupt on timer1
    overFlowCounter=0;                  // reset overflow counter
    sei();                              // enable global interrupts
 
    while((ECHO_PIN & (1<<ECHO_BIT))){    // while echo pin is still high
        if (((overFlowCounter*TIMER_MAX)+TCNT1) > SONAR_TIMEOUT){
            kDistance_in_cm = ECHO_ERROR;          // No echo within sonar range
        }
    };
 
    TCCR1B = 0x00;                      // stop 16 bit timer with no prescaler
    cli();                              // disable global interrupts
    no_of_ticks = ((overFlowCounter*TIMER_MAX)+TCNT1);  // counter count
    kDistance_in_cm = (no_of_ticks/(CONVERT_TO_CM*CYCLES_PER_US));   // distance in cm
}