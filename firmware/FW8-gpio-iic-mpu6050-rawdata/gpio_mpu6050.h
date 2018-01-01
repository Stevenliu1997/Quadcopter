#ifndef __GPIO_MPU6050
#define __GPIO_MPU6050
#include <stdint.h> // u8 define in stm32f4xx.h for legacy

/* 加速度 角速度 循环均值滤波后的值 循环次数 为宏定义correct_num */
/* 量程见init的量程设置以及相关函数 */
extern float ax_cc, ay_cc, az_cc;
extern float gx_cc, gy_cc, gz_cc;
extern float ax_off, ay_off, az_off;
extern float gx_off, gy_off, gz_off;
extern float ax_buf, ay_buf, az_buf;
extern float gx_buf, gy_buf, gz_buf;

uint8_t MPU_Init(void);

/** @brief 均值滤波读六轴数据.
    把结果写到全局变量 AB_CC 上
*/
void acc_correct(void);
void gyro_correct(void);
void acc_get(void);
void gyro_get(void);
#endif /* __GPIO_MPU6050 */
