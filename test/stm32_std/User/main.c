#include <stdlib.h>
#include "system.h"
#include "sys_usart.h"
#include "sys_timer.h"
#include "rng.h"
#include "dwt.h"
#include "rand.h"
#include "benchmark.h"

void led_test(void) {
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

void led_init(void) {

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

void KnownAnswerTests(void){
    int ret;
    const char* result_string[] = { "Fail", "Pass" };
    printf("Start testing each module for correctness...\r\n");
    printf("Module\t\tResult\r\n");

    ret = 1;
    ret &= kat_fp();
    printf("%-16s%s\r\n", "FP", result_string[ret]);

    ret = 1;
    ret &= kat_fn();
    printf("%-16s%s\r\n", "FN", result_string[ret]);

    ret = 1;
    ret &= kat_point();
    printf("%-16s%s\r\n", "POINT", result_string[ret]);

    ret = 1;
    ret &= kat_sm2_sig();
    printf("%-16s%s\r\n", "SM2SIG", result_string[ret]);

    ret = 1;
    ret &= kat_sm2_enc();
    printf("%-16s%s\r\n", "SM2ENC", result_string[ret]);

    ret = 1;
    ret &= kat_sm3();
    printf("%-16s%s\r\n", "SM3", result_string[ret]);

    ret = 1;
    ret &= kat_sm4();
    printf("%-16s%s\r\n", "SM4", result_string[ret]);
    printf("The test is over.\r\n\r\n");
}

#define pb_printf(u) printf("%-16s%-12lu%-12.3f%.3f %s\r\n", u->name, u->count, u->time, u->speed, u->unit);

void PerformanceBenchmarks(void){
    time_reg(DWT_Start, DWT_End);
    PBUint* u = malloc(sizeof(PBUint));
    printf("Start testing the performance benchmarks of each function...\r\n");
    printf("%-16s%-12s%-12s%-12s\r\n","Function", "Count", "Time", "Speed");

    // pb_fp_add(u, 1000000);
    // pb_printf(u);
    // pb_fp_sub(u, 1000000);
    // pb_printf(u);
    // pb_fp_dbl(u, 1000000);
    // pb_printf(u);
    // pb_fp_haf(u, 1000000);
    // pb_printf(u);
    // pb_fp_mul(u, 100000);
    // pb_printf(u);
    // pb_fp_sqr(u, 100000);
    // pb_printf(u);
    // pb_fp_inv(u, 10000);
    // pb_printf(u);
    //
    //
    // pb_point_add(u, 10000);
    // pb_printf(u);
    // pb_japoint_add(u, 10000);
    // pb_printf(u);
    // pb_point_dbl(u, 10000);
    // pb_printf(u);
    // pb_point_mdo(u, 10000);
    // pb_printf(u);
    // pb_point_gen(u, 1000);
    // pb_printf(u);
    // pb_point_mul(u, 100);
    // pb_printf(u);
    //
    pb_sm2_sig(u, 1000);
    pb_printf(u);
    pb_sm2_vfy(u, 100);
    pb_printf(u);
    pb_sm2_enc(u, 100);
    pb_printf(u);
    pb_sm2_dec(u, 100);
    pb_printf(u);


    pb_sm3(u, 10000);
    pb_printf(u);
    pb_sm4_enc(u, 100000);
    pb_printf(u);
    pb_sm4_dec(u, 100000);
    pb_printf(u);

    free(u);
}

int main(void) {
    led_init();
    led_test();
    SystemClock_Config();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RNG_Init();
    USART2_Init(115200);
    TIM_Config();

    // 注册随机数生成器
    rand_reg(stm32_rng);
	KnownAnswerTests();
	PerformanceBenchmarks();
}
