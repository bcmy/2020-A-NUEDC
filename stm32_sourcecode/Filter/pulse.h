#ifndef __PULSE__H_
#define __PULSE__H_
#include "stdint.h"
#define DATA_SIZE 20

typedef enum {FALSE, TRUE} BOOL;
#define SAMPLE_PERIOD 2

extern uint16_t readData, preReadData;		   		// 读取到的 AD 值
extern uint16_t timeCount ;         // 采样周期计数变量
extern uint16_t firstTimeCount ;    // 第一个心跳时间
extern uint16_t secondTimeCount ;   // 第二个心跳时间

extern uint16_t IBI, BPM, SIG;

extern uint16_t data[DATA_SIZE]; // 采样数据缓存
extern uint16_t idx, filter, pulseCount, max, min, mid;;				// 滤波值

extern BOOL PULSE ;				// 有效心跳
extern BOOL PRE_PULSE ;         // 先前有效心跳



 uint16_t Get_Array_Max(uint16_t* array, uint32_t size);
 uint16_t Get_Array_Min(uint16_t* array, uint32_t size);


#endif