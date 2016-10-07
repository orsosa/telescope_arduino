

#all: db_t3 send
all: test_class.hex #send

test_class.hex: Servo.cxx WifiCom.cxx test_class.cxx 
	avr-gcc -mmcu=atmega1280 -Os test_class.cxx Servo.cxx WifiCom.cxx -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o test_class.o
	avr-objcopy -j .text -j .data -O ihex test_class.o test_class.hex	

send: 
	avrdude -c arduino -p m1280 -P com6 -b 57600 -F -U flash:w:test_class.hex


clean:
	rm 

