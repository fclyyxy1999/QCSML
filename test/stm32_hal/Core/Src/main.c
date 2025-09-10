/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rng.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "rand.h"
#include "benchmark.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define   DWT_CR    					      *(u32*)0xE0001000
#define   DWT_CYCCNT    				    *(u32*)0xE0001004
#define   DEM_CR    					      *(u32*)0xE000EDFC
#define 	DEM_CR_TRCENA    			    (1<<24)
#define   DWT_CR_CYCCNTENA    		  (1<<0)
#define		SystemCoreClockFrequency 	168000000

void DWT_Start(void){
  DEM_CR |= (u32)DEM_CR_TRCENA;				  // 使能DWT
  DWT_CYCCNT = (u32)0;						      // 寄存器清零
  DWT_CR |= (u32)DWT_CR_CYCCNTENA;			// 使能寄存器，开始计时
}
double DWT_End(void){
  DWT_CR |= (u32)0;			// 去能寄存器，关闭计时
  return (double) DWT_CYCCNT / SystemCoreClockFrequency;
}

#ifdef USE_FULL_LL_DRIVER
uint8_t __io_putchar(const int ch){
  const uint8_t temp[1]={ch};
  LL_USART_TransmitData8(USART2,temp[0]);
  while (LL_USART_IsActiveFlag_TXE(USART2) == RESET) {;};
  return (ch);
}

int stm32_rng(void *buf, u32 len) {
  if (buf == NULL)
    return -1;
  const u32 wl = len >> 2, bl = len & 0x3;
  u32 *wbuf = buf;
  for (u32 i = 0; i < wl; i++) {
      wbuf[i] = LL_RNG_ReadRandData32(RNG);
  }
  u8 *bbuf = (u8 *)(wbuf + wl);
  u32 tmp = LL_RNG_ReadRandData32(RNG);
  if (bl > 0) {
    for (u32 i = 0; i < bl; i++) {
      bbuf[i] = tmp & 0xff;
      tmp >>= 8;
    }
  }
  return 0;
}
#endif
#ifdef USE_HAL_DRIVER
int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}
int stm32_rng(void *buff, u32 len)
{
  u8 *buf = buff;
  uint32_t i = 0;
  while (i < len) {
    uint32_t rnd32;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &rnd32) != HAL_OK) {
      return -1;
    }
    size_t cplen = (len - i) > 4 ? 4 : (len - i);
    memcpy(buf + i, &rnd32, cplen);
    i += cplen;
  }
  return 0;
}
#endif

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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RNG_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  rand_reg(stm32_rng);
  KnownAnswerTests();
  PerformanceBenchmarks();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifdef USE_FULL_LL_DRIVER
    LL_mDelay(500);
    LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_12);
    LL_mDelay(500);
    LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_15);
    printf("Hello World!\r\n");
#endif
#ifdef USE_HAL_DRIVER
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
    printf("Hello World!\r\n");
#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLQ_DIV_7);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(168000000);
  LL_SetSystemCoreClock(168000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
