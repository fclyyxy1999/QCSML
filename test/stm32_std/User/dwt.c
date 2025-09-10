#include "dwt.h"

void DWT_Start(void){
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;				// 使能DWT
	DWT_CYCCNT = (uint32_t)0;						// 寄存器清零
	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;			// 使能寄存器，开始计时
}

double DWT_End(void){
	DWT_CR |= (uint32_t)0;			// 去能寄存器，关闭计时
	return (double) DWT_CYCCNT / SystemCoreClockFrequency;
}

