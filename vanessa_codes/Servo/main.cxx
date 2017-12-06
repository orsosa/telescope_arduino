#include"Servo.h"


int main(){
    Servo servo;
    servo.init();
    servo.test();
    _delay_ms(10000);
    servo.setAngle(180);
}
