main: main.cxx Sonar.cxx uart.cxx
	avr-gcc -mmcu=atmega1280 -Os main.cxx uart.cxx Sonar.cxx -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o main.o
	avr-objcopy -j .text -j .data -O ihex main.o main.hex		
	
test: test.c
	avr-gcc -mmcu=atmega1280 -Os test.c -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o test.o
	avr-objcopy -j .text -j .data -O ihex test.o test.hex		
	
	
test_class: main.cxx Sonar.cxx uart.cxx
	avr-gcc -mmcu=atmega1280 -Os main.cxx uart.cxx Sonar.cxx -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o main.o
	avr-objcopy -j .text -j .data -O ihex main.o main.hex	
	
 
send: 
	avrdude -c arduino -p m1280 -P com7 -b 57600 -F -U flash:w:main.hex


all: main send

all2: test_class send


clean:
	rm 


   