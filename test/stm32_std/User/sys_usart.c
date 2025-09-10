#include "sys_usart.h"

void USART2_Init(uint32_t bound) {
	
	// GPIO初始化和配置
	GPIO_InitTypeDef GPIO_InitStruct;	
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);	// GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);	// GPIOA3复用为USART2
	
	// 配置PA2为USART2_TX引脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// 配置PA3为USART2_RX引脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // RX引脚通常不需要上拉
	GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	// USART2初始化和配置
	USART_InitStruct.USART_BaudRate = bound;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2,&USART_InitStruct);
	
	USART_Cmd(USART2,ENABLE);		// 使能USART1
	// USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		// 设置串口中断类型并使能

	// NVIC_Config_USART();
}

void USART2_SendStr(char *str) {
    while(*str) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *str++);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
}


// /** NVIC初始化和配置 **/
// void NVIC_Config_USART(void) {
// 	NVIC_InitTypeDef NVIC_InitStruct;
	
// 	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
// 	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
// 	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
// 	NVIC_Init(&NVIC_InitStruct);
// }


// /** 串口中断服务函数 **/
// void USART2_IRQHandler(void) {
// 	u8 r;
// 	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) {
// 		r = USART_ReceiveData(USART2);
// 		USART_SendData(USART2,r);
// 		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
// 	}		
// 	USART_ClearFlag(USART2,USART_FLAG_TC);
// }
