#ifndef __DV_MMA9553_H__
#define __DV_MMA9553_H__

/******************************************************************** 
*          宏定义
********************************************************************/ 
#define MMA9553_Slave_Addr  0x98 
#define MMA9553_Sub_Addr    0x00



/******************************************************************** 
*         全局变量 
********************************************************************/


/******************************************************************** 
*         函数声明
********************************************************************/ 
void pedometer_main(void);
void pedometer_init(void);
void pedometer_reset(void);
void pedometer_enable(void);
void pedometer_disable(void);
void pedometer_active(void);
void pedometer_write_config(void);
void pedometer_afe_config(void); // wenxue
void pedometer_wakeup(void);
void pedometer_int0_enable(void);
void pedometer_cmd_readstatus(void);
void pedometer_cmd_readconfig(void);
void MMA9555L_Read(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len);
void MMA9555L_Write(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len);
unsigned char MMA9555L_I2C_RecvByte(void);
void MMA9555L_I2C_SendByte(unsigned char data);
void MMA9555L_I2C_Start(void);
void MMA9555L_I2C_Stop(void);
void MMA9555L_I2C_Ack(unsigned char ack);
void MMA9555L_init(void);
void SixDirection_Init(void);
void SixDirection_Statues(unsigned char *data);
#endif


