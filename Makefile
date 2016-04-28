blink: blink.c 
	avr-gcc -mmcu=atmega1280 -Os blink.c -o spi_test.o 
	avr-objcopy -j .text -j .data -O ihex spi_test.o spi_test.hex


blink2: blink2.cpp
	avr-gcc -mmcu=atmega1280 -Os blink2.cpp -o blink2.o
	avr-objcopy -j .text -j data -O ihex blink2.o blink2.hex


testc: test_coincidence.cxx 
	avr-gcc -mmcu=atmega1280 -Os test_coincidence.cxx -o test_coincidence.o -I"C:\Users\diegoj\Desktop\arduino-1.6.7\hardware\arduino\avr\variants\mega" -I"C:\Users\diegoj\Desktop\arduino-1.6.7\hardware\arduino\avr\cores\arduino"
	avr-objcopy -j .text -j .data -O ihex test_coincidence.o test_coincidence.hex


serial: serialc.cpp
	avr-gcc -mmcu=atmega1280 -Os serialc.cpp -o serialc.o
	avr-objcopy -j .text -j .data -O ihex serialc.o serialc.hex

	
freq: freqq.cpp
	avr-gcc -mmcu=atmega1280 -Os freqq.cpp -o freqq.o
	avr-objcopy -j .text -j .data -O ihex freqq.o freqq.hex


wifi: wifi_com.cpp
	avr-gcc -mmcu=atmega1280 -Os wifi_com.cpp -o wifi_com.o
	avr-objcopy -j .text -j .data -O ihex wifi_com.o wifi_com.hex

	
connect: database_connect.cpp
	avr-gcc -mmcu=atmega1280 -Os database_connect.cpp -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o database_connect.o
	avr-objcopy -j .text -j .data -O ihex database_connect.o database_connect.hex		


db_t2: db_test2.cpp
	avr-gcc -mmcu=atmega1280 -Os db_test2.cpp -Wl,-u,vfscanf -lscanf_min -lm  -Wl,-u,vfprintf -lprintf_flt -lm -o db_test2.o
	avr-objcopy -j .text -j .data -O ihex db_test2.o db_test2.hex		
 

send:
	avrdude -c arduino -p m1280 -P com6 -b 57600 -F -U flash:w:database_connect.hex

send2: 
	avrdude -c arduino -p m1280 -P com6 -b 57600 -F -U flash:w:db_test2.hex


all: connect send

all2: db_t2 send2
	
clean:
	rm spi_test.o spi_test.hex test_coincidence.hex test_coincidence.o blink2.hex blink2.o freqq.hex freqq.o test_coincidence.o test_coincidence.hex serialc.o serialc.hex wifi_com.o wifi_com.hex database_connect.o database_connect.hex freqq.o freqq.hex



