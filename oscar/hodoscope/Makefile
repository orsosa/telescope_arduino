object: communication/communication.cxx i2c_master/i2c_master.cxx imudof/imudof.cxx Servo/Servo.cxx us/us.cxx stepper/stepper.cxx fdcr/fdcr.cxx wifi/wifi.cxx main.cxx
	avr-gcc -mmcu=atmega1280 -std=c++11 -Icommunication -Ii2c_master -Ifdcr -Iimudof -IServo -Istepper -Ius -Iwifi -Os $^ -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o main.o
	avr-objcopy -j .text -j .data -O ihex main.o main.hex

send:
	avrdude -c arduino -p m1280 -P /dev/ttyUSB0 -b 57600 -F -U flash:w:main.hex

clean:
	rm *.o *.hex



