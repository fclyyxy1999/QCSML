#include "dwt.h"

double RunningTime;

void DWT_start(void){
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;				// 使能DWT
	DWT_CYCCNT = (uint32_t)0;						// 寄存器清零
	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;			// 使能寄存器，开始计时
}

void DWT_end(void){
	RunningTime = (double) DWT_CYCCNT;
	DWT_CR |= (uint32_t)0;			// 去能寄存器，关闭计时	
	RunningTime = RunningTime / SystemCoreClockFrequency;
}

