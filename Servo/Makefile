servo: servo.cpp
	avr-gcc -mmcu=atmega1280 -Os servo.cpp -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o servo.o
	avr-objcopy -j .text -j .data -O ihex servo.o servo.hex		
 
send: 
	avrdude -c arduino -p m1280 -P com6 -b 57600 -F -U flash:w:servo.hex


all: servo send


clean:
	rm 


