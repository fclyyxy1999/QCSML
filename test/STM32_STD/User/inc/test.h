#ifndef SM_TEST_H
#define SM_TEST_H

#include "sm4.h"
#include "sm3.h"
#include "sm2.h"

void test_all(void);
int test_fp_add(void);
int test_fp_sub(void);
int test_fp_neg(void);
int test_fp_dou(void);
int test_fp_haf(void);
int test_bn_mul(void);
int test_fp_mul(void);
int test_fp_sqr(void);
int test_fp_inv(void);
int test_fp_mul(void);

int test_fn_add(void);
int test_fn_sub(void);
int test_fn_inv(void);
int test_fn_mul(void);


int test_point_add(void);
int test_point_add_affine_jacobian(void);
int test_point_dou(void);
int test_point_mdo(void);
int test_point_mul(void);
int test_point_mul_fixed(void);


int test_sm2_sign(void);
int test_sm2_verify(void);
int test_sm2_enc(void);
int test_sm2_dec(void);
int test_sm3(void);
int test_sm4_enc(void);
int test_sm4_dec(void);

#endif //SM_TEST_H
