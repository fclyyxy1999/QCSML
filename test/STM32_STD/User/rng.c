#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#include "rng.h"

// RNG初始化函数
void RNG_Init(void)
{
    // 1. 使能RNG时钟（位于AHB2总线）
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

    // 2. 初始化RNG外设
    RNG_Cmd(ENABLE);

    // 3. 等待RNG稳定（约40个时钟周期）
    volatile int i;
    for(i = 0; i < 100; i++); // 简单延时

    // 4. 清除所有错误标志
    RNG_ClearFlag(RNG_FLAG_CECS | RNG_FLAG_SECS);
}

// 获取随机数函数
int stm32_rng(void *buf, uint32_t len)
{
    if (buf == NULL)
        return -1;

    uint8_t *output = (uint8_t *)buf;
    uint32_t remaining = len;
    uint32_t timeout = 0;
#define RNG_TIMEOUT 1000000  // 超时计数值

    // 检查RNG是否已启用
    if (RNG->CR & RNG_CR_RNGEN) {
        // RNG已启用，清除可能的错误状态
        RNG_ClearFlag(RNG_FLAG_CECS | RNG_FLAG_SECS);
    } else {
        // RNG未启用，重新初始化
        RNG_Init();
    }

    while (remaining > 0)
    {
        // 检查错误标志
        if (RNG_GetFlagStatus(RNG_FLAG_SECS))
        {
            // 种子错误 - 需要重新初始化
            RNG_Cmd(DISABLE);
            RNG_Init();
            return -2; // 返回错误代码
        }

        if (RNG_GetFlagStatus(RNG_FLAG_CECS))
        {
            // 时钟错误 - 无法恢复
            return -3; // 返回错误代码
        }

        // 等待数据就绪
        timeout = 0;
        while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET)
        {
            if (timeout++ > RNG_TIMEOUT)
            {
                return -4; // 超时错误
            }

            // 检查等待过程中是否出现错误
            if (RNG_GetFlagStatus(RNG_FLAG_SECS) || RNG_GetFlagStatus(RNG_FLAG_CECS))
            {
                break;
            }
        }

        // 再次检查错误标志（可能在等待期间发生变化）
        if (RNG_GetFlagStatus(RNG_FLAG_SECS) || RNG_GetFlagStatus(RNG_FLAG_CECS))
        {
            continue; // 重新尝试
        }

        // 获取32位随机数
        uint32_t random_word = RNG_GetRandomNumber();

        // 根据剩余长度复制数据
        uint32_t copy_size = (remaining >= 4) ? 4 : remaining;

        for (uint32_t i = 0; i < copy_size; i++)
        {
            *output++ = (random_word >> (i * 8)) & 0xFF;
        }

        remaining -= copy_size;
    }

    return 0; // 成功
}