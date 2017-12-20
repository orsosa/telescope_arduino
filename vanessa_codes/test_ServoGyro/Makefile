object: main.cxx i2c_master.cxx ImuDof.cxx Communication.cxx Servo.cxx
	avr-gcc -mmcu=atmega1280 -std=c++11 -Os $^ -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o main.o
	avr-objcopy -j .text -j .data -O ihex main.o main.hex

send:
	avrdude -c arduino -p m1280 -P /dev/ttyUSB0 -b 57600 -F -U flash:w:main.hex

clean:
	rm *.o *.hex
