#include <stdint.h>
//寄存器基地址
#define		DWT_CR    					*(uint32_t *)0xE0001000
#define    	DWT_CYCCNT    				*(uint32_t *)0xE0001004
#define    	DEM_CR    					*(uint32_t *)0xE000EDFC
 
//定义需使能位
#define 	DEM_CR_TRCENA    			(1<<24)
#define    	DWT_CR_CYCCNTENA    		(1<<0)
#define		SystemCoreClockFrequency 	168000000

void DWT_Start(void);

double DWT_End(void);


