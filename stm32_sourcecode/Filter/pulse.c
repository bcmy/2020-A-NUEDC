#include "pulse.h"

uint16_t readData, preReadData;		   		// 读取到的 AD 值
uint16_t timeCount = 0;         // 采样周期计数变量
uint16_t firstTimeCount = 0;    // 第一个心跳时间
uint16_t secondTimeCount = 0;   // 第二个心跳时间

uint16_t IBI, BPM, SIG;

uint16_t data[DATA_SIZE] = {0}; // 采样数据缓存

uint16_t idx, filter, pulseCount, max, min, mid;			// 滤波值

BOOL PULSE = FALSE;				// 有效脉搏
BOOL PRE_PULSE = FALSE;         // 先前有效脉搏



uint16_t Get_Array_Max(uint16_t* array, uint32_t size)
{
    uint16_t max = array[0];
    uint32_t i;
    for(i = 1; i < size; i++)
    {
        if(array[i] > max)
            max = array[i];
    }
    return max;
}

uint16_t Get_Array_Min(uint16_t* array, uint32_t size)
{
    uint16_t min = array[0];
    uint32_t i;
    for(i = 1; i < size; i++)
    {
        if(array[i] < min)
            min = array[i];
    }
    return min;
}

