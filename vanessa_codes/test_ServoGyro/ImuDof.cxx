#include "ImuDof.h"

ImuDof::ImuDof(){
    i2c.init();
    _delay_ms(100);
}


void ImuDof::init(){
    //Hard Reset
    //i2c.writeBit(ImuAddr, 0x6B, 7, 1); //
    
    //Set Clock
    i2c.writeBits(ImuAddr,0x6B, 2, 3, 0x01);
    //i2c.writeByte(ImuAddr, 0x6B, 0x21); 
    
    //GYRO_CONFIG
    i2c.writeBits(ImuAddr, 0x1B, 4, 2, 0x00);
    //ACCEL_CONFIG
    i2c.writeBits(ImuAddr, 0x1C, 4, 2, 0x00);
    //Disable Sleep Mode
    i2c.writeBit(ImuAddr, 0x6B, 6, false);
    for (int k =0;k<3;k++)
    {
      kAccelC[k]=(char*)malloc(8);
      kGyroC[k] =(char*)malloc(8);

      kAccelC[k][7]='\0';
      kGyroC[k][7]='\0';
    }
    
}


bool ImuDof::testConnection() {
    i2c.readByte(ImuAddr, 0x75, data);	
    if (data[0]==0x71){
        return true;
    }
    else{
        return false;
    }
}

void ImuDof::readData()
{
    ImuDof::clearData();
    i2c.readBytes(ImuAddr, 0x3B, 14, data);  
    kAccel[0] = (((int16_t)data[0]) << 8) | data[1]; //x
    kAccel[1] = (((int16_t)data[2]) << 8) | data[3]; //y
    kAccel[2] = (((int16_t)data[4]) << 8) | data[5]; //z

    kGyro[0] = (((int16_t)data[8])  << 8) | data[9]; //x
    kGyro[1] = (((int16_t)data[10]) << 8) | data[11];//y
    kGyro[2] = (((int16_t)data[12]) << 8) | data[13];//z

    for (int k=0;k<3;k++)
    {
      kAccelF[k] = (float) kAccel[k]/ 16384;//x
      kGyroF[k] = (float) kGyro[k] *250/32768;//x
      dtostrf(kAccelF[k],7, 3, (char*)kAccelC[k]);
      dtostrf(kGyroF[k],7, 3, (char*)kGyroC[k]);
    }
}

void ImuDof::getData(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz){
    ImuDof::clearData();
    i2c.readBytes(ImuAddr, 0x3B, 14, data);  
    *ax = (((int16_t)data[0]) << 8) | data[1];
    *ay = (((int16_t)data[2]) << 8) | data[3];
    *az = (((int16_t)data[4]) << 8) | data[5];
    *gx = (((int16_t)data[8]) << 8) | data[9];
    *gy = (((int16_t)data[10]) << 8) | data[11];
    *gz = (((int16_t)data[12]) << 8) | data[13];
}


float * ImuDof::getAcc(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz){    
    ImuDof::getData(ax,ay,az,gx,gy,gz);
   
    //Do the conversion
    Accel[0] = (float) *ax/ 16384;
    Accel[1] = (float) *ay/ 16384;
    Accel[2] = (float) *az/ 16384;  
    // 2¹⁴ = 16384  / 65535
    return Accel;
}


/*int ImuDof::getTemp(){
    ImuDof::getData();
    Aux[0] =(((int16_t)data[6])<<8)| data[7] ;// Temperature
    Temp = (float) (Aux[0]/160)+10;
    return (int)Temp;
}*/


float * ImuDof::getGyro(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz){
    ImuDof::getData(ax,ay,az,gx,gy,gz);
     
    //Do de Conversion
    Gyro[0] = (float) (*gx) *250/32768 ;
    Gyro[1] = (float) (*gy) *250/32768 ;
    Gyro[2] = (float) (*gz) *250/32768 ;
    return Gyro;    
}


float * ImuDof::getComp(){
    int16_t *sample;
    sample = ImuDof::getCompSample();
    //Do de conversion
    Mag[0] = ((float) *sample* 1200 / 16384) ;
    Mag[1] = ((float) *(sample+1) * 1200 / 16384) ;
    Mag[2] = ((float) *(sample+2) * 1200 / 16384);
    return Mag;
}

int16_t * ImuDof::getCompSample(){
    //ImuDof::clearData();
    i2c.writeByte(ImuAddr,  0x37, 0x02); //set i2c bypass enable pin to true to access magnetometer
    _delay_ms(10);
    i2c.writeByte(0x18, 0x0A, 0x01); //enable the magnetometer
    _delay_ms(10);
    i2c.readBytes(0x18, 0x03, 6, data);
    Aux[0] =(((int16_t)data[0])<<8)| data[1] ;// component x of the magnetometer   
    Aux[1] =(((int16_t)data[2])<<8)| data[3] ;// component y of the magnetometer   
    Aux[2] =(((int16_t)data[4])<<8)| data[5] ;// component z of the magnetometer 
    return Aux;
}


void ImuDof::CompassCalibrate(){
    float *aux;
    aux = getComp();
    mx_min = *aux; 
    mx_max = *aux;
    my_min = *(aux+1);
    my_max = *(aux+1);
    mz_min = *(aux+2);
    mz_max = *(aux+2);
    
   for (int i = 0; i < 1000; i++){
        aux = getComp();
        mx_sample = *aux;
        my_sample = *(aux+1);
        mz_sample = *(aux+2);
        //X samples
        if (mx_sample > mx_max & mx_sample < (mx_max + 10) ){ 
            mx_max = mx_sample;
        }
        if (mx_sample < mx_min & mx_sample > (mx_min - 10)){
            mx_min = mx_sample;
        }
        //Y samples
        if (my_sample > my_max & my_sample < (my_max + 10)){ 
            my_max = my_sample;
        }
        if (my_sample < my_min & my_sample > (my_max - 10)){ 
            my_min = my_sample;
        }
        //Z samples
        if (mz_sample > mz_max & mz_sample < (mz_max + 10)){ 
            mz_max = mz_sample; 
        }
        if (mz_sample < mz_min & mz_sample > (mz_max - 10)){ 
            mz_min = mz_sample;
        }    
    }
    mx_center = (mx_max + mx_min)/2;
    my_center = (my_max + my_min)/2;
    mz_center = (mz_max + mz_min)/2;
     
    
}   

float * ImuDof::getCompCal(){
    ImuDof::getComp();
    Mag[0] = Mag[0]-((float) mx_center* 1200 / 16384) ;
    Mag[1] = Mag[1]+((float) my_center * 1200 / 16384) ;
    Mag[2] = Mag[2]-((float) mz_center * 1200 / 16384);
    return Mag;
}


void ImuDof::clearData(){
    //Clear the data array
    for (int i = 0; i<14;i++){
        data[i] = 0; 
    }
}
