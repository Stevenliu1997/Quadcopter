#include "gpio_hmc5883.h"
#include "gpio_i2c.h"
#include "delay.h"
#include <stdint.h> // u8 define in stm32f4xx.h for legacy

#define    HMC5883_Addr   0x1E

unsigned char BUF[8];

float magX, magY, magZ;
float magX_off, magY_off, magZ_off;
float magX_cc, magY_cc, magZ_cc;

uint8_t HMC_Write_Byte(uint8_t reg, uint8_t data)
{
    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|0);
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}



uint8_t HMC_Read_Byte(uint8_t reg)
{
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|0);
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|1);
    IIC_Wait_Ack();
    res = IIC_Read_Byte(0);
    IIC_Stop();
    return res;
}


void InitHMC5883()
{
    HMC_Write_Byte(0x02,0x00);
    HMC_Write_Byte(0x01,0xE0);
}


void updateHMC5883()
{
    HMC_Write_Byte(0x00,0x14);
    HMC_Write_Byte(0x02,0x00);
    // Delayms(10);

    BUF[1]=HMC_Read_Byte(0x03);//OUT_X_L_A
    BUF[2]=HMC_Read_Byte(0x04);//OUT_X_H_A
    BUF[3]=HMC_Read_Byte(0x07);//OUT_Y_L_A
    BUF[4]=HMC_Read_Byte(0x08);//OUT_Y_H_A
    BUF[5]=HMC_Read_Byte(0x05);//OUT_Z_L_A
    BUF[6]=HMC_Read_Byte(0x06);//OUT_Y_H_A

    magX=(BUF[1] << 8) | BUF[2]; //Combine MSB and LSB of X Data output register
    magY=(BUF[3] << 8) | BUF[4]; //Combine MSB and LSB of Y Data output register
    magZ=(BUF[5] << 8) | BUF[6]; //Combine MSB and LSB of Z Data output register

    // if(magX>0x7fff)magX-=0xffff;//?????~???????
    // if(magY>0x7fff)magY-=0xffff;
    // if(magZ>0x7fff)magZ-=0xffff;
}

void hmc_correct()
{
    unsigned char i=0;
    unsigned char nummg=200;
    
    int mgx=0;
    int mgy=0;
    int mgz=0;                          

    for(i=0;i<nummg;i++)
    {       
        updateHMC5883();
        mgx+=magX;
        mgy+=magY;
        mgz+=magZ;      
        delay(2);
    }   

    magX_off = mgx/nummg;
    magY_off = mgy/nummg;
    magZ_off = mgz/nummg;
    
}

