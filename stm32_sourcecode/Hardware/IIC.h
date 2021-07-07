#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"
#define SCL_H         GPIOB->BSRR = GPIO_Pin_0
#define SCL_L         GPIOB->BRR  = GPIO_Pin_0
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_1
#define SDA_L         GPIOB->BRR  = GPIO_Pin_1

#define SCL_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
void I2C_GPIO_Config(void);
void I2C_Stop(void);

extern u8 MailBoxSend[32];
extern u8 MailBoxRec[32];
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 *data);
u8 Single_ReadI2C(u8 Slave_Address,u8 *data,u8 length);
u8 I2c_WriteRegister(u8 Slave_Address,u8 RegAddress,u8 *data,u8 length);
u8 I2c_ReadRegister(u8 Slave_Address,u8 RegAddress,u8 *data,u8 length);
#endif
