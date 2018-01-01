#ifndef __GPIO_IIC_H
#define __GPIO_IIC_H

extern float magX, magY, magZ; 
extern float magX_off, magY_off, magZ_off; 
extern float magX_cc, magY_cc, magZ_cc;  

void InitHMC5883(void);
void hmc_correct(void);
void updateHMC5883(void);

#endif /* __GPIO_IIC_H */
