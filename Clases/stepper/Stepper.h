#ifndef XSTEEPER_H
#define XSTEEPER_H

/*****************************************
Configuration area
******************************************/

/*
Port where stepper is connected
*/
#define STEPPER_PORT 	C	



/*
Position where 1st coil is connected.
All other coils like 2nd,3rd and 4th
must be connected in sequence after the 1st.

For this configuration the connection is 
like:-

PC0 -> Coil 1  -> Pin 37
PC1 -> Coil 2  -> Pin 36
PC2 -> Coil 3  -> Pin 35
PC3 -> Coil 4  -> Pin 34

Don't connect port pins directly to coil,
use a driver like ULN2003A etc.

*/
#define STEPPER_POS		0	
							


/*
Don't touch the line below
*/

#define _CONCAT(a,b) a##b
 #define PORT(x) _CONCAT(PORT,x)
 #define PIN(x) _CONCAT(PIN,x)
 #define DDR(x) _CONCAT(DDR,x)

#define __STEPPER_PORT 	PORT(STEPPER_PORT)
#define __STEPPER_DDR	DDR(STEPPER_PORT)

class Stepper
{

public:

void stepperInit();
void stepperStepCW();
void stepperStepCCW();
};
#endif