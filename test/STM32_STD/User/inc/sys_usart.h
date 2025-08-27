#ifndef SM_SYS_USART_H
#define SM_SYS_USART_H

#include "system.h"
#include <stdio.h>

void USART2_Init(uint32_t bound);
void NVIC_Config_USART(void);
void USART2_IRQHandler(void);


#endif


