/*******************************************************************************
* 文件名	: main.c
* 作  者	: GGTEAM
* 版  本	: V1.0
* 日  期	: 2020-10-07
* 描  述	:
*******************************************************************************/
#include "sys.h"         //系统配置
#include "delay.h"       //延时
#include "usart.h"       //串口  		
#include "led.h"
#include "ADS1292.h"
#include "Timer.h"
#include "dma.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
#include "pulse.h"
#include "stdbool.h"
void HMISendb(u8 k);

s32 get_volt(u32 num);//把采到的3个字节补码转成有符号32位数
u8 text_buffer[128];
//main
#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
uchar_T Flag_Run=1;
extern unsigned int Uart2_Rx,Uart2_Tx;
u16 StepCount,Distance;
unsigned int Uart2_Sta;
extern u8 USART2_RX_BUF[UART_LENGTH];
extern unsigned int Uart2_Len;
u8 step_str[5];
u8 distance_str[5];


int main(void)
{
    u16 adcx;
   // u8 count=0;
    s32 int_val;
    double temp;
    double tem;
    u8 res,i,sum;
    u8 data_to_send[60];//串口发送缓存
    u8 usbstatus=0;
    s32 cannle[2];	//存储两个通道的数据
    s32	p_Temp[2];	//数据缓存
    // float temperature[10];
    u32 times_ECG,times_T;
    double  sum_T ,j;
	u32 count=0;
	u32 temp_count = 0;


    int32_t data_rdc_in;
	int16_t data_data;
	

    u32 data_count = 0;
	// 导联脱落检测
	u32 status_byte;
	u8 LeadStatus;
	bool leadoff_deteted;
	long Mac = 0;
	s32 line_part;
	int16_t data_part[DATA_SIZE] = {0}; 
    //  float data_send[20];//串口发送缓存
	

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级


    Adc_Init();


//初始化系统时钟	 72M
    SystemInit();
    delay_init();
    delay_ms(2000);
   
	
	
	uart1_init(115200);	 	//串口初始化为115200
    uart2_init(115200);
    DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);//串口1DMA设置
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA
    LED_Init();

    ADS1292_Init();	//初始化ads1292
    while(Set_ADS1292_Collect(0))//0 正常采集  //1 1mV1Hz内部侧试信号 //2 内部短接噪声测试
    {
        printf("1292寄存器设置失败\r\n");
        delay_s(1);
  
    }
    printf("寄存器设置成功\r\n");
    delay_s(1);



    Subsystem_initialize();


    EXTI->IMR |= EXTI_Line5;//开DRDY中断




    while(1)//循环发送数据
    {

	
        if(	Flag_Run) {
			

			
	
			
            if(ads1292_recive_flag)
            {
				PAout(7) = 1;
                //	cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//获取原始数据
                cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
				status_byte=ads1292_Cache[0]<<16 | ads1292_Cache[1]<<8 | ads1292_Cache[2];//获取原始数据

                //p_Temp[0] = get_volt(cannle[0]);	//把采到的3个字节转成有符号32位数
                p_Temp[1] = get_volt(cannle[1]);	//把采到的3个字节转成有符号32位数
          //     rtU.In1	= (int16_t)p_Temp[1] >> 8;
           rtU.In1	= (int16_t)(ads1292_Cache[6]<<8 | ads1292_Cache[7]);//p_Temp[1] >> 8;
                Subsystem_step();

                


				
				
    status_byte  = (status_byte & 0x0f8000) >> 15;  // bit15 gives the lead status
    
    LeadStatus = (unsigned char ) status_byte ;  

    if(!((LeadStatus & 0x1c) == 0 ))
      leadoff_deteted  = true; 
    else
      leadoff_deteted  = false;
				
				
				
			
				
				
				
                cannle[1]=(int32_t)rtY.Out1;
                data_rdc_in = (int32_t)rtY.Out1;
				
				if(leadoff_deteted)
				{
					data_rdc_in = 0;
					printf("t5.txt=\"---\"\xff\xff\xff");
					//printf("\"\xff\xff\xff");
				}
	
	

				
				
				//计算直流分量
				
			//	data_part[0] = data_rdc_in;

					
				data_part[0] = data_rdc_in;

				for ( i = (DATA_SIZE-1); i > 0; i--)
				{
					data_part[i] = data_part[i - 1];
					Mac += data_part[i];
				}

				line_part=Mac/DATA_SIZE;
				
				
				
				
				 preReadData = readData;	
				readData = (data_rdc_in-line_part)/10;
			 if((readData - preReadData) < filter) 
				  data[idx++] = readData;	


        if(idx >= DATA_SIZE)
        {
            idx = 0;	// 数组填满，从头再填

            // 通过缓存数组获取脉冲信号的波峰、波谷值，并计算中间值作为判定参考阈值
            max = Get_Array_Max(data, DATA_SIZE);
            min = Get_Array_Min(data, DATA_SIZE);
            mid = (max + min)* 8/10;
			//mid = (max + mid) *8/10;
            filter = (max - min) / 2;
        }

        PRE_PULSE = PULSE;	// 保存当前脉冲状态
        PULSE = (readData > mid) ? TRUE : FALSE;  // 采样值大于中间值为有效脉冲

        if(PRE_PULSE == FALSE && PULSE == TRUE)   // 寻找到“信号上升到振幅中间位置”的特征点，检测到一次有效脉搏
        {
            pulseCount++;
            pulseCount %= 2;

            if(pulseCount == 1) // 两次脉搏的第一次
            {
                firstTimeCount = timeCount;   // 记录第一次脉搏时间
            }
            if(pulseCount == 0)  // 两次脉搏的第二次
            {
                secondTimeCount = timeCount;  // 记录第二次脉搏时间
                timeCount = 0;

                if((secondTimeCount > firstTimeCount))
                {
                    IBI = (secondTimeCount - firstTimeCount) * SAMPLE_PERIOD;	// 计算相邻两次脉搏的时间，得到 IBI
                    BPM = 60000 / IBI;  // 通过 IBI 得到心率值 BPM
                    if(BPM > 200)     //限制BPM最高显示值
                        BPM = 200;
                    if(BPM < 30)     //限制BPM最低显示值
                        BPM = 30;
                }
            }

		
			printf("t5.txt=\"%d\"\xff\xff\xff",(int)BPM);
		
		
        }

        timeCount++;  // 时间计数累加
       Mac = 0;
			
		



			printf("add 1,0,%d\xff\xff\xff",(data_rdc_in - line_part)/10);
         

                ads1292_recive_flag=0;
   
				
                
                count++;

				
            }
			
			
			
			if(Uart2_Sta)
			{
				StepCount = USART2_RX_BUF[1]<<8|USART2_RX_BUF[2];
				Distance = USART2_RX_BUF[3]<<8|USART2_RX_BUF[4];
				for(i=0;i<UART_LENGTH;++i)
				USART2_RX_BUF[i] = 0;
				Uart2_Rx = 0;
				Uart2_Tx = 0;
				Uart2_Sta=0;
			}
			
			
if(count % 250 == 0){
		   temp_count++;
           adcx=Get_Adc(ADC_Channel_1);
           
           temp=(double)adcx*(3.3/4096)*1000;
			
           tem =(double)((-1.809628e-9)*temp*temp*temp+(-3.325395e-6)*temp*temp+(-1.814103e-1)*temp+205.5894);
           sum_T+=tem;
if(temp_count>6){
	sum_T=sum_T/7;
	printf("t2.txt=\"%2.2f\"\xff\xff\xff",sum_T);

				   temp_count=0;
				   sum_T=0;
				   j=0;
	      
}
        
			
 			
				
				
		
				
if(count % 500 == 0){
			
				printf("t8.txt=\"%d\"\xff\xff\xff",StepCount);
			
}
if(count % 750 == 0){					
printf("t9.txt=\"%d\"\xff\xff\xff",Distance);
count=0;	
}
           }
			
		   	PAout(7) = 0;	

        }
		

    }
}



/*功能：把采到的3个字节转成有符号32位数 */
s32 get_volt(u32 num)
{
    s32 temp;
    temp = num;
    temp <<= 8;
    temp >>= 8;
    return temp;
}




/**********************************************************************
编译结果里面的几个数据的意义：
Code：表示程序所占用 FLASH 的大小（FLASH）
RO-data：即 Read Only-data， 表示程序定义的常量，如 const 类型（FLASH）
RW-data：即 Read Write-data， 表示已被初始化的全局变量（SRAM）
ZI-data：即 Zero Init-data， 表示未被初始化的全局变量(SRAM)
***********************************************************************/

void HMISendb(u8 k)		         //字节发送函数
{
    u8 i;
    for(i=0; i<3; i++)
    {
        printf("%c",k);
    }
}