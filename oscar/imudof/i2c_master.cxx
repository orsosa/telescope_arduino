#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

#include "i2c_master.h"

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)
#define TWCR_DEFAULT

void I2C_master::init(){
    uint8_t sreg;
    sreg = SREG;
    //cli();
	TWBR = (uint8_t)TWBR_val;
    TWSR &= ~(_BV(TWPS1) | _BV(TWPS0));
    PORTC |= _BV(PC5) | _BV(PC4);
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
    TWAR = 0;
    SREG = sreg;

}

uint8_t I2C_master::start(uint8_t address){

	// reset TWI control register
	TWCR = 0;
	// transmit START condition 
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	if((TWSR & 0xF8) != TW_START){ return 1; }  
	
	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
    
	return 0;
}

int8_t I2C_master::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b;
    uint8_t count = readByte(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    I2C_master::stop();
    return count;
}


int8_t I2C_master::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = readByte(devAddr, regAddr, &b)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
    I2C_master::stop();
    return count;
}

int8_t I2C_master::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data) {
    return readBytes(devAddr, regAddr, 1, data);
}

bool I2C_master::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    readByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(devAddr, regAddr, b);
}



bool I2C_master::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (readByte(devAddr, regAddr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(devAddr, regAddr, b);
    } else {
        return false;
    }
}

bool I2C_master::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return writeBytes(devAddr, regAddr, 1, &data);
}

///////////////////////////////////////////////////////////


uint8_t I2C_master::write(uint8_t data){
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while( !(TWCR & (1 << TWINT)) );
	
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK ){ return 1; }
    
	
	return 0;
}

uint8_t I2C_master::read_ack(void){
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
    
	return TWDR;
}

uint8_t I2C_master::read_nack(void){
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
    
	return TWDR;
}

uint8_t I2C_master::transmit(uint8_t address, uint8_t* data, uint16_t length){

	if (I2C_master::start(address | I2C_WRITE)) return 1;
	
	for (uint16_t i = 0; i < length; i++)
	{
		if (I2C_master::write(data[i])) return 1;
	}
	
	I2C_master::stop();
	
	return 0;
}

uint8_t I2C_master::receive(uint8_t address, uint8_t* data, uint16_t length){

	if (I2C_master::start(address | I2C_READ)) return 1;
	
	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = I2C_master::read_ack();
	}
	data[(length-1)] = I2C_master::read_nack();
	
	I2C_master::stop();
	
	return 0;
}



bool I2C_master::writeBytes(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t *data){

	if (I2C_master::start(devaddr | 0x00)) return true;

	I2C_master::write(regaddr);

	for (uint16_t i = 0; i < length; i++)
	{
		if (I2C_master::write(data[i])) return true;
	}

	I2C_master::stop();

	return false;
}

uint8_t I2C_master::readBytes(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t *data){

	if (I2C_master::start(devaddr)) return 1;

	I2C_master::write(regaddr);

	if (I2C_master::start(devaddr | 0x01)) return 1;

	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = I2C_master::read_ack();
	}
	data[(length-1)] = I2C_master::read_nack();


	return 0;
}

void I2C_master::stop(void){

	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
