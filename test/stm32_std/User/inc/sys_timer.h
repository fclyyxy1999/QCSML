#ifndef SM_SYS_TIMER_H
#define SM_SYS_TIMER_H

#include "stm32f4xx.h"
#include <stdio.h>

void SystemClock_Config(void);

/* 延时函数初始化和方法 */
void SysTick_Init(uint8_t SYSCLK);
void delay_nms(uint16_t nms);
void delay_ms(uint16_t nms);

/* 时间测量初始化和方法*/
void TIM_Config(void);
void TIM2_IRQHandler(void);
void start_timer(void);
uint64_t stop_timer(void);
float get_execution_time_sec(void);


#endif
