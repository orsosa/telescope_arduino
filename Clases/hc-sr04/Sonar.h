#ifndef SONAR_H_
#define SONAR_H_
 
#ifndef F_CPU
	#define F_CPU 16000000UL
    //#define F_CPU 1000000UL     // CPU Frequency
#endif
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 
/*...- . . .-. --- -... --- -
 * Define Ports and Pins as required
 * Modify Maximum response time and delay as required
 * MAX_RESP_TIME : default: 300
 * DELAY_BETWEEN_TESTS : default: 50
 */
#define TRIG_DDR    DDRH           // Trigger Port
#define TRIG_PORT   PORTH
#define TRIG_PIN    PINC
#define TRIG_BIT    PH5             // Trigger Pin -> Pin 8 on Arduino Atmega
 
#define ECHO_DDR    DDRH            // Echo Port
#define ECHO_PORT   PORTH
#define ECHO_PIN    PINH
#define ECHO_BIT    PH6             // Echo Pin -> Pin 9   see pin mapping
 
// Speed of sound
// Default: 343 meters per second in dry air at room temperature (~20C)
#define SPEED_OF_SOUND  343
#define MAX_SONAR_RANGE 10          // This is trigger + echo range (in meters) for SR04
#define DELAY_BETWEEN_TESTS 500     // Echo canceling time between sampling. Default: 500us
#define TIMER_MAX 65535             // 65535 for 16 bit timer and 255 for 8 bit timer
 
/* ...- . . .-. --- -... --- -
 * Do not change anything further unless you know what you're doing
 * */
#define TRIG_ERROR -1
#define ECHO_ERROR -2
 
#define CYCLES_PER_US (F_CPU/1000000)// instructions per microsecond
#define CYCLES_PER_MS (F_CPU/1000)      // instructions per millisecond
// Timeout. Decreasing this decreases measuring distance
// but gives faster sampling
#define SONAR_TIMEOUT ((F_CPU*MAX_SONAR_RANGE)/SPEED_OF_SOUND)
 
#define TRIG_INPUT_MODE() TRIG_DDR &= ~(1<<TRIG_BIT)
#define TRIG_OUTPUT_MODE() TRIG_DDR |= (1<<TRIG_BIT)
#define TRIG_LOW() TRIG_PORT &= ~(1<<TRIG_BIT)
#define TRIG_HIGH() TRIG_PORT |=(1<<TRIG_BIT)
 
#define ECHO_INPUT_MODE() ECHO_DDR &= ~(1<<ECHO_BIT)
#define ECHO_OUTPUT_MODE() ECHO_DDR |= (1<<ECHO_BIT)
#define ECHO_LOW() ECHO_PORT &= ~(1<<ECHO_BIT)
#define ECHO_HIGH() ECHO_PORT |=(1<<ECHO_BIT)
 
#define CONVERT_TO_CM ((10000*2)/SPEED_OF_SOUND)    // or simply 58
 
class Sonar
{
public:
uint32_t kDistance_in_cm; 
 
/** ...- . . .-. --- -... --- -
 * @brief   Initiate Ports for Trigger and Echo pins
 * @param   void
 * @return  none
*/
void initSonar();
 
/**  ...- . . .-. --- -... --- -
 * @brief   Send 10us pulse on Ultrasonic Trigger pin
 * @param   void
 * @return  none
*/
void triggerSonar();
 
/**  ...- . . .-. --- -... --- -
 * @brief   Calculate and store echo time and return distance
 * @param   void
 * @return  none 
*/
void readDistance();

};
#endif /* SONAR_H_ */