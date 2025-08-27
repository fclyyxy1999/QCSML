#include "system.h"
#include "speed.h"
#include "test.h"
#include "rng.h"
#include "sys_usart.h"
#include "sys_timer.h"


void led_test() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER &= ~(GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER14);
	GPIOD->MODER |= (GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0);

    GPIOD->ODR |= (1 << 12);
    GPIOD->ODR |= (1 << 13);
    GPIOD->ODR |= (1 << 14);
//    delay_ms(1000);
//    GPIOD->ODR &= (0 << 12);
//    GPIOD->ODR &= (0 << 13);
//    GPIOD->ODR &= (0 << 14);
//    delay_ms(1000);

}


void led_init() {

	GPIO_InitTypeDef GPIO_InitStruct1;		// 初始化GPIO结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		// 使能GPIOD时钟
	
	GPIO_InitStruct1.GPIO_Mode=GPIO_Mode_OUT;	// 通用输出模式
	GPIO_InitStruct1.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //	设置引脚
	GPIO_InitStruct1.GPIO_Speed=GPIO_Speed_50MHz;  // 速度
	GPIO_InitStruct1.GPIO_OType = GPIO_OType_PP;  // 输出类型为推挽
	GPIO_InitStruct1.GPIO_PuPd = GPIO_PuPd_UP;		 // 上拉
	
	GPIO_Init(GPIOD,&GPIO_InitStruct1);
	
	// 默认高电平
	// GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}


int main(void) {
    led_init();
    led_test();
    SystemClock_Config();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RNG_Init();
    USART2_Init(115200);
    TIM_Config();
    // 测试RNG
    uint8_t random_bytes[16];
    int result = stm32_rng(random_bytes, sizeof(random_bytes));

    // 注册随机数生成器
    rand_reg(stm32_rng);
//    test_all();
    speed_all();
}
