#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdint.h>

#define I2C_READ 0x01
#define I2C_WRITE 0x00

class I2C_master{

public:
    static void init (void);
    static uint8_t start(uint8_t address);
    static uint8_t write(uint8_t data);
    static uint8_t read_ack(void);
    static uint8_t read_nack(void);
    static uint8_t transmit(uint8_t address, uint8_t* data, uint16_t length);
    static uint8_t receive(uint8_t address, uint8_t* data, uint16_t length);
 
    static void stop(void);

    static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);


    static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
        
    static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
       
    static uint8_t readBytes(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t *data);
        
    static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
        
    static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
        
    static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
        
    static bool writeBytes(uint8_t devaddr, uint8_t regaddr, uint16_t length, uint8_t *data);
        
};

#endif // I2C_MASTER_H
