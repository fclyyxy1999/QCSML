//
// Created by Lixk on 2025/8/19.
//

#ifndef QCSML_RNG_H
#define QCSML_RNG_H
void RNG_Init(void);
int stm32_rng(void *buf, uint32_t len);
#endif //QCSML_RNG_H
