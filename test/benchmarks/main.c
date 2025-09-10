#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "benchmark.h"

#define pb_printf(u) printf("%-16s%-12d%-12.3f%.3f %s\n", u->name, u->count, u->time, u->speed, u->unit);

void KnownAnswerTests(void){
    int ret;
    const char* result_string[] = { "Fail", "Pass" };
    printf("Start testing each module for correctness...\n");
    printf("Module\t\t\tResult\n");

    ret = 1;
    ret &= kat_fp();
    printf("%-16s%s\n", "FP", result_string[ret]);

    ret = 1;
    ret &= kat_fn();
    printf("%-16s%s\n", "FN", result_string[ret]);

    ret = 1;
    ret &= kat_point();
    printf("%-16s%s\n", "POINT", result_string[ret]);

    ret = 1;
    ret &= kat_sm2_sig();
    printf("%-16s%s\n", "SM2SIG", result_string[ret]);

    ret = 1;
    ret &= kat_sm2_enc();
    printf("%-16s%s\n", "SM2ENC", result_string[ret]);

    ret = 1;
    ret &= kat_sm3();
    printf("%-16s%s\n", "SM3", result_string[ret]);

    ret = 1;
    ret &= kat_sm4();
    printf("%-16s%s\n", "SM4", result_string[ret]);
    printf("The test is over.\n\n");
}

struct timespec start, end;

void TIME_START(){
    clock_gettime(CLOCK_MONOTONIC, &start);
}

double TIME_END(){
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (double)end.tv_sec - (double)start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / 1E9;
}

void PerformanceBenchmarks(){
    time_reg(TIME_START, TIME_END);
    PBUint* u = malloc(sizeof(PBUint));
    printf("Start testing the performance benchmarks of each function...\n");
    printf("%-16s%-12s%-12s%-12s\n","Function", "Count", "Time", "Speed");

    // pb_fp_add(u, 100000000);
    // pb_printf(u);
    // pb_fp_sub(u, 100000000);
    // pb_printf(u);
    // pb_fp_dbl(u, 100000000);
    // pb_printf(u);
    // pb_fp_haf(u, 100000000);
    // pb_printf(u);
    // pb_fp_mul(u, 100000000);
    // pb_printf(u);
    // pb_fp_sqr(u, 100000000);
    // pb_printf(u);
    // pb_fp_inv(u, 1000000);
    // pb_printf(u);
    //
    //
    // pb_point_add(u, 1000000);
    // pb_printf(u);
    // pb_japoint_add(u, 1000000);
    // pb_printf(u);
    // pb_point_dbl(u, 1000000);
    // pb_printf(u);
    // pb_point_mdo(u, 1000000);
    // pb_printf(u);
    // pb_point_gen(u, 100000);
    // pb_printf(u);
    // pb_point_mul(u, 10000);
    // pb_printf(u);
    //
    pb_sm2_sig(u, 100000);
    pb_printf(u);
    pb_sm2_vfy(u, 10000);
    pb_printf(u);
    pb_sm2_enc(u, 10000);
    pb_printf(u);
    pb_sm2_dec(u, 10000);
    pb_printf(u);


    pb_sm3(u, 1000000);
    pb_printf(u);
    pb_sm4_enc(u, 10000000);
    pb_printf(u);
    pb_sm4_dec(u, 10000000);
    pb_printf(u);

    free(u);
}

int main(void) {

    KnownAnswerTests();
    PerformanceBenchmarks();
}