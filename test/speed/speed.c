#include <stdio.h>
#include <stdlib.h>

#include "sm2.h"
#include "sm3.h"
#include "sm4.h"

#define runingtime RunningTime

#ifdef _WIN32
// Windows Speed Test Code
#include <windows.h>
#define time_start() start = GetTickCount();
#define time_end() end = GetTickCount(); times = (double)(end - start) / 1000.0;
#else
// Linux/Unix Speed Test Code
#include <time.h>
#define TIME_START() clock_gettime(CLOCK_MONOTONIC, &start)
#define TIME_END() clock_gettime(CLOCK_MONOTONIC, &end); runingtime = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1E9;
#endif

#ifdef _WIN32
DWORD start, end;
#else
struct timespec start, end;
#endif

static double RunningTime;

void speed_fp_add() {
    u32 n = 100000000;
    sm2_fp x, y, z, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "FDDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(z, "0DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_add(t, x, y);
        sm2_fp_add(t, x, z);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_add", n << 1, runingtime, (n << 1) / runingtime);
}

void speed_fp_sub() {
    u32 n = 100000000;
    sm2_fp x, y, z, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "FDDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(z, "0DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_sub(t, x, y);
        sm2_fp_sub(t, x, z);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_sub", n << 1, runingtime, (n << 1) / runingtime);
}

void speed_fp_haf() {
    u32 n = 100000000;
    sm2_fp x, y, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020EC");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_haf(t, x);
        sm2_fp_haf(t, y);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_haf", n << 1, runingtime, (n << 1) / runingtime);
}

void speed_fp_dbl() {
    u32 n = 100000000;
    sm2_fp x, y, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "17BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_dbl(t, x);
        sm2_fp_dbl(t, y);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_dbl", n << 1, runingtime, (n << 1) / runingtime);
}

void speed_fp_mul() {
    u32 n = 100000000;
    sm2_fp x, y, z;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_mul(z, x, y);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_mul", n, runingtime, n / runingtime);
}

void speed_fp_sqr() {
    const u32 n = 100000000;
    sm2_fp x, y;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_sqr(y, x);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_sqr", n, runingtime, n / runingtime);
}

void speed_fp_inv() {
    const u32 n = 1000000;
    sm2_fp x, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_inv(t, x);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "fp_inv", n, runingtime, n / runingtime);
}

void speed_jpoint_add() {
    u32 n = 1000000;
    sm2_jpoint jp, jq, jt;
    sm2_jpoint_from_str(&jp,    "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED"
                                "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_jpoint_from_str(&jq,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");


    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_add(&jt, &jp, &jq);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "jpoint_add", n, runingtime, n / runingtime);

}

void speed_japoint_add() {
    u32 n = 1000000;
    sm2_apoint q;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED"
                                "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_apoint_from_str(&q,     "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_japoint_add(&jt, &jp, &q);
    }
    TIME_END();

    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "japoint_add", n, runingtime, n / runingtime);
}

void speed_point_dou() {
    u32 n = 1000000;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_dou(&jt, &jp);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "jpoint_dou", n, runingtime, n / runingtime);
}

void speed_point_mdo() {
    u32 n = 1000000;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_mdo(&jt, 6, &jp);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "jpoint_mdb", n, runingtime, n / runingtime);
}

void speed_point_mul() {
    u32 n = 10000;
    sm2_fp k;
    sm2_jpoint jp, jt;
    sm2_fp_from_str(k, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");
    sm2_jpoint_from_str(&jp, "04EBFC718E8D1798620432268E77FEB6415E2EDE0E073C0F4F640ECD2E149A73"
                             "E858F9D81E5430A57B36DAAB8F950A3C64E6EE6A63094D99283AFF767E124DF0"
                             "0000000000000000000000000000000000000000000000000000000000000001");


    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_mul(&jt, k, &jp);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "jpoint_mul", n, runingtime, n / runingtime);
}

void speed_point_gen() {
    u32 n = 100000;
    sm2_fp k;
    sm2_jpoint jt;
    sm2_fp_from_str(k, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_gen(&jt, k);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "jpoint_gen", n, runingtime, n / runingtime);
}

void speed_sm2_sign() {
    u32 n = 100000;
    sm2_prikey prikey;
    sm2_signature signature;
    u8 dgst[32] = {0xF0, 0xB4, 0x3E, 0x94, 0xBA, 0x45, 0xAC, 0xCA, 0xAC, 0xE6, 0x9E, 0xD5,
                   0x48, 0xB6, 0xBA, 0xC1, 0xF4, 0x48, 0x6F, 0xDF, 0xC0, 0xD2, 0x86, 0x40};
    sm2_prikey_from_str(&prikey, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_sign_core(&signature, &prikey, dgst);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "sm2_sign", n, runingtime, n / runingtime);
}

void speed_sm2_verify() {
    u32 n = 10000;
    sm2_pubkey pubkey;
    sm2_signature signature;
    u8 dgst[32] = {0xF0, 0xB4, 0x3E, 0x94, 0xBA, 0x45, 0xAC, 0xCA, 0xAC, 0xE6, 0x9E, 0xD5,
                   0x48, 0xB6, 0xBA, 0xC1, 0xF4, 0x48, 0x6F, 0xDF, 0xC0, 0xD2, 0x86, 0x40};
    sm2_pubkey_from_str(&pubkey, "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020"
                                 "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13");
    sm2_bn_from_str(signature.r, "f5a03b0648d2c4630eeac513e1bb81a15944da3827d5b74143ac7eaceee720b3");
    sm2_bn_from_str(signature.s, "b1b6aa29df212fd8763182bc0d421ca1bb9038fd1f7f42d4840b69c485bbc1aa");


    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_verify_core(&signature, &pubkey, dgst);
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "sm2_verify", n, runingtime, n / runingtime);
}

void speed_sm2_enc() {
    const u32 n = 10000;
    const char msg[] = "encryption standard";
    sm2_point pubkey;
    sm2_ciptext ctext;
    u8 ciphertext[sizeof(msg)];
    ctext.C2 = ciphertext;
    sm2_pubkey_from_str(&pubkey, "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13");


    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm2_encrypt_core(&ctext, &pubkey, msg, strlen(msg));
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "sm2_enc", n, runingtime, n / runingtime);
}

void speed_sm2_dec() {
    u32 n = 10000;
    char msg[] = "encryption standard";
    u8 ciptext[126];
    sm2_prikey prikey;
    sm2_prikey_from_str(&prikey, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");
    bytes_from_hexstr(ciptext,  "307c022004ebfc718e8d1798620432268e77feb6415e2ede0e073c0f4f640ecd2e149a73022100e858f9d81e5430a57b3"
                                "6daab8f950a3c64e6ee6a63094d99283aff767e124df0042059983c18f809e262923c53aec295d30383b54e39d609d160"
                                "afcb1908d0bd8766041321886ca989ca9c7d58087307ca93092d651efa", 126);



    sm2_ciptext ct;
    u8 pt[sizeof(msg) - 1];
    u64 plen = 0, clen = 126;
    ct.C2 = malloc(sizeof(msg));

    const u8 *_ct = ciptext;
    sm2_ciptext_from_der(&ct, &_ct, &clen);


    TIME_START();
    for (u32 i = n; i > 0; i--) {
        plen = 0;
        sm2_decrypt_core(pt, &plen, &prikey, &ct);
    }
    TIME_END();

    printf("%-16s%-12d%-12.3f%.3f ops/s\n", "sm2_dec", n, runingtime, n / runingtime);
    free(ct.C2);
}

void speed_sm4_enc() {
    u32 n = 10000000;
    u8 key[16] = {0};
    u8 pt[16] = {0};
    u8 ct[16] = {0};
    sm4_ctx *ctx = malloc(sizeof(sm4_ctx));
    sm4_setkey(ctx, key, sizeof(key));

    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm4_encrypt(ctx, pt, ct, sizeof(pt));
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f Mbps/s\n", "sm4_enc", n, runingtime, (double) (n << 7) / 1000000 / runingtime);
    free(ctx);
}

void speed_sm4_dec() {
    u32 n = 10000000;
    u8 key[16] = {0};
    u8 pt[16] = {0};
    u8 ct[16] = {0};
    sm4_ctx *ctx = malloc(sizeof(sm4_ctx));
    sm4_setkey(ctx, key, sizeof(key));
    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm4_encrypt(ctx, ct, pt, sizeof(pt));
    }
    TIME_END();
    printf("%-16s%-12d%-12.3f%.3f Mbps/s\n", "sm4_dec", n, runingtime, (double) (n << 7) / 1000000 / runingtime);
    free(ctx);
}

void speed_sm3() {
    u32 n = 1000, m = 10000;
    u8 *message = malloc(m * 64);
    u32 digest[8] = {0};
    TIME_START();
    for (u32 i = n; i > 0; i--) {
        sm3_compress(digest, message, m);
    }
    TIME_END();
    n = n * m;
    printf("%-16s%-12d%-12.3f%.3f Mbps/s\n", "sm3", n, runingtime, (double) (n/1000000 << 9) / runingtime);
    free(message);
    // int i = 8;
    // for (; i > 0; i--) {
    //     printf("%08X", digest[i]);
    // }
    // printf("\n");
}

void speed_all() {
    printf("Start testing the speed of each function...\n");
    printf("%-16s%-12s%-12s%-12s\n","Function", "Number", "Time", "Speed");
    // speed_fp_add();
    // speed_fp_sub();
    // speed_fp_dbl();
    // speed_fp_haf();
    // speed_fp_mul();
    // speed_fp_sqr();
    // speed_fp_inv();
    //
    // speed_jpoint_add();
    // speed_japoint_add();
    // speed_point_dou();
    // speed_point_mdo();
    // speed_point_mul();
    // speed_point_gen();
    speed_sm3();
    speed_sm4_enc();
    speed_sm4_dec();
    speed_sm2_sign();
    speed_sm2_verify();
    speed_sm2_enc();
    speed_sm2_dec();
    printf("The test is over.\n\n");
}

int main(void) {
    speed_all();
    return 0;
}