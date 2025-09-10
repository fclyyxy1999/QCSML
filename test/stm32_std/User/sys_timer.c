#include "sys_timer.h"

static uint16_t fac_us=0,fac_ms=0;
volatile uint32_t timer_overflow_count = 0;

void SystemClock_Config(void)
{
    // 配置系统时钟为168MHz，使用HSE（8MHz外部晶振）
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    if(RCC_WaitForHSEStartUp() == SUCCESS)
    {
        RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
        
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div4); // APB1 = 42MHz
        RCC_PCLK2Config(RCC_HCLK_Div2);
        
        FLASH_SetLatency(FLASH_Latency_5);
        
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08) {}
    }
}

void SysTick_Init(uint8_t SYSCLK) {
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK/8;
	fac_ms = (uint16_t)fac_us*1000;
}

void delay_nms(uint16_t nms) {
    uint32_t temp;
	// printf("\r\ncntTim1:%d \r\n",(uint32_t)fac_ms*nms);
    SysTick->LOAD = (uint32_t)fac_ms*nms; // 计算所需的加载值
	// printf("\r\ncntTim2:%d \r\n",SysTick->LOAD);
    SysTick->VAL = 0x00; // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 使能计数器
    do {
        temp = SysTick->CTRL; // 读取当前倒计数值
    } while ((temp & SysTick_CTRL_COUNTFLAG_Msk) == 0); // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0x00; // 清空计数器
}

void delay_ms(uint16_t nms) {
	uint8_t repeat = nms/540;
	uint16_t remain = nms%540;
	while(repeat) {
		delay_nms(540);
		repeat--;
	}
	if(remain) delay_nms(remain);
}

void TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // APB1 时钟为 42MHz，但 TIM2 时钟为 84MHz（因为APB1预分频不为1）
    // 将 TIM2 配置为 1MHz 计数频率
    TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 84; // 84MHz / 84 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        timer_overflow_count++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void start_timer(void)
{
    timer_overflow_count = 0;
    // 停止计数器以进行复位
    TIM_Cmd(TIM2, DISABLE);
    TIM_SetCounter(TIM2, 0);
    // 重新启动计数器
    TIM_Cmd(TIM2, ENABLE);
}

uint64_t stop_timer(void)
{
    uint32_t count = TIM_GetCounter(TIM2);
    return ((uint64_t)timer_overflow_count << 32) | count;
}

float get_execution_time_sec(void)
{
    uint64_t ticks = stop_timer();
    return (float)ticks / 1000000.0f;
}
