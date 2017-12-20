#include"Servo.h"


int main(){
    Servo servo;
    servo.init();
    //Prueba se ,amdas distintos àngulos
    while(true){
        servo.setAngle(120);
        _delay_ms(5000);
        servo.setAngle(30);
        _delay_ms(5000);
        
    }
}
