#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "common.h"

// Known-Answer Tests (KATs)
int kat_fp(void);
int kat_fn(void);
int kat_point(void);
int kat_sm3(void);
int kat_sm2_sig(void);
int kat_sm2_enc(void);
int kat_sm4(void);

// Performance Benchmarks
typedef void (*timestart_func)(void);
typedef double (*timeend_func)(void);
void time_reg(timestart_func s, timeend_func e);

typedef struct {
    char    name[16];       // 模块名
    u32     count;          // 次数
    double  time;           // 时间
    double  speed;          // 速度
    char    unit[8];        // 单位
} PBUint;

void pb_fp_add(PBUint *u, u32 count);
void pb_fp_sub(PBUint *u, u32 count);
void pb_fp_haf(PBUint *u, u32 count);
void pb_fp_dbl(PBUint *u, u32 count);
void pb_fp_mul(PBUint *u, u32 count);
void pb_fp_sqr(PBUint *u, u32 count);
void pb_fp_inv(PBUint *u, u32 count);
void pb_point_add(PBUint *u, u32 count);
void pb_japoint_add(PBUint *u, u32 count);
void pb_point_dbl(PBUint *u, u32 count);
void pb_point_mdo(PBUint *u, u32 count);
void pb_point_mul(PBUint *u, u32 count);
void pb_point_gen(PBUint *u, u32 count);
void pb_sm3(PBUint *u, u32 count);
void pb_sm2_sig(PBUint *u, u32 count);
void pb_sm2_vfy(PBUint *u, u32 count);
void pb_sm2_enc(PBUint *u, u32 count);
void pb_sm2_dec(PBUint *u, u32 count);
void pb_sm4_enc(PBUint *u, u32 count);
void pb_sm4_dec(PBUint *u, u32 count);



#endif //BENCHMARK_H