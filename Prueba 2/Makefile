db_t3: db_test3.cpp
	avr-gcc -mmcu=atmega1280 -Os db_test3.cpp -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o db_test3.o
	avr-objcopy -j .text -j .data -O ihex db_test3.o db_test3.hex		
 
send: 
	avrdude -c arduino -p m1280 -P com6 -b 57600 -F -U flash:w:db_test3.hex


#all: db_t3 send
all: db_t3 send


clean:
	rm 


