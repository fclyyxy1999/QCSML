#include <stdio.h>
#include <stdlib.h>
#include "sm2.h"
#include "sm3.h"
#include "sm4.h"
#include "dwt.h"

#define runningtime RunningTime
#define time_start() DWT_start()
#define time_end() DWT_end()



void speed_fp_add() {
    int n = 1000000;
    sm2_fp x, y, z, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "FDDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(z, "0DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_add(t, x, y);
        sm2_fp_add(t, x, z);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_add", n << 1, runningtime, (n << 1) / runningtime);
}

void speed_fp_sub() {
    int n = 1000000;
    sm2_fp x, y, z, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "FDDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(z, "0DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_sub(t, x, y);
        sm2_fp_sub(t, x, z);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_sub", n << 1, runningtime, (n << 1) / runningtime);
}

void speed_fp_haf() {
    int n = 1000000;
    sm2_fp x, y, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020EC");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_haf(t, x);
        sm2_fp_haf(t, y);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_haf", n << 1, runningtime, (n << 1) / runningtime);
}

void speed_fp_dbl() {
    int n = 1000000;
    sm2_fp x, y, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "17BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_dbl(t, x);
        sm2_fp_dbl(t, y);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_dbl", n << 1, runningtime, (n << 1) / runningtime);
}

void speed_fp_mul() {
    int n = 100000;
    sm2_fp x, y, z;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_mul(z, x, y);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_mul", n, runningtime, n / runningtime);
}

void speed_fp_sqr() {
    const int n = 100000;
    sm2_fp x, y;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_sqr(y, x);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_sqr", n, runningtime, n / runningtime);
}

void speed_fp_inv() {
    const int n = 10000;
    sm2_fp x, t;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_fp_inv(t, x);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "fp_inv", n, runningtime, n / runningtime);
}

void speed_jpoint_add() {
    int n = 10000;
    sm2_jpoint jp, jq, jt;
    sm2_jpoint_from_str(&jp,    "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED"
                                "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_jpoint_from_str(&jq,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");


    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_add(&jt, &jp, &jq);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "jpoint_add", n, runningtime, n / runningtime);

}

void speed_japoint_add() {
    int n = 10000;
    sm2_apoint q;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED"
                                "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_apoint_from_str(&q,     "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_japoint_add(&jt, &jp, &q);
    }
    time_end();

    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "japoint_add", n, runningtime, n / runningtime);
}

void speed_point_dou() {
    int n = 10000;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_dou(&jt, &jp);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "jpoint_dou", n, runningtime, n / runningtime);
}

void speed_point_mdo() {
    int n = 10000;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_mdo(&jt, 6, &jp);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "jpoint_mdb", n, runningtime, n / runningtime);
}

void speed_point_mul() {
    int n = 100;
    sm2_fp k;
    sm2_jpoint jp, jt;
    sm2_fp_from_str(k, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");
    sm2_jpoint_from_str(&jp, "04EBFC718E8D1798620432268E77FEB6415E2EDE0E073C0F4F640ECD2E149A73"
                             "E858F9D81E5430A57B36DAAB8F950A3C64E6EE6A63094D99283AFF767E124DF0"
                             "0000000000000000000000000000000000000000000000000000000000000001");


    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_mul(&jt, k, &jp);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "jpoint_mul", n, runningtime, n / runningtime);
}

void speed_point_gen() {
    int n = 1000;
    sm2_fp k;
    sm2_jpoint jt;
    sm2_fp_from_str(k, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_jpoint_gen(&jt, k);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "jpoint_gen", n, runningtime, n / runningtime);
}

void speed_sm2_sign() {
    int n = 1000;
    sm2_prikey prikey;
    sm2_signature signature;
    u8 dgst[32] = {0xF0, 0xB4, 0x3E, 0x94, 0xBA, 0x45, 0xAC, 0xCA, 0xAC, 0xE6, 0x9E, 0xD5,
                   0x48, 0xB6, 0xBA, 0xC1, 0xF4, 0x48, 0x6F, 0xDF, 0xC0, 0xD2, 0x86, 0x40};
    sm2_prikey_from_str(&prikey, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_sign_core(&signature, &prikey, dgst);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "sm2_sign", n, runningtime, n / runningtime);
}

void speed_sm2_verify() {
    int n = 100;
    sm2_pubkey pubkey;
    sm2_signature signature;
    u8 dgst[32] = {0xF0, 0xB4, 0x3E, 0x94, 0xBA, 0x45, 0xAC, 0xCA, 0xAC, 0xE6, 0x9E, 0xD5,
                   0x48, 0xB6, 0xBA, 0xC1, 0xF4, 0x48, 0x6F, 0xDF, 0xC0, 0xD2, 0x86, 0x40};
    sm2_pubkey_from_str(&pubkey, "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020"
                                 "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13");
    sm2_bn_from_str(signature.r, "f5a03b0648d2c4630eeac513e1bb81a15944da3827d5b74143ac7eaceee720b3");
    sm2_bn_from_str(signature.s, "b1b6aa29df212fd8763182bc0d421ca1bb9038fd1f7f42d4840b69c485bbc1aa");


    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_verify_core(&signature, &pubkey, dgst);
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "sm2_verify", n, runningtime, n / runningtime);
}

void speed_sm2_enc() {
    const int n = 100;
    const char msg[] = "encryption standard";
    sm2_point pubkey;
    sm2_ciptext ctext;
    u8 ciphertext[sizeof(msg)];
    ctext.C2 = ciphertext;
    sm2_pubkey_from_str(&pubkey, "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13");


    time_start();
    for (u32 i = n; i > 0; i--) {
        sm2_encrypt_core(&ctext, &pubkey, msg, strlen(msg));
    }
    time_end();
    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "sm2_enc", n, runningtime, n / runningtime);
}

void speed_sm2_dec() {
    int n = 100;
    char msg[] = "encryption standard";
    u8 ciptext[126];
    sm2_prikey prikey;
    sm2_prikey_from_str(&prikey, "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");
    bytes_from_hexstr(ciptext, "307c022004ebfc718e8d1798620432268e77feb6415e2ede0e073c0f4f640ecd2e149a73022100e858f9d81e5430a57b3"
                               "6daab8f950a3c64e6ee6a63094d99283aff767e124df0042059983c18f809e262923c53aec295d30383b54e39d609d160"
                               "afcb1908d0bd8766041321886ca989ca9c7d58087307ca93092d651efa", 126);



    sm2_ciptext ct;
    u8 pt[sizeof(msg) - 1];
    u64 plen = 0, clen = 126;
    ct.C2 = malloc(sizeof(msg));

    const u8 *_ct = ciptext;
    sm2_ciptext_from_der(&ct, &_ct, &clen);


    time_start();
    for (u32 i = n; i > 0; i--) {
        plen = 0;
        sm2_decrypt_core(pt, &plen, &prikey, &ct);
    }
    time_end();

    printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", "sm2_dec", n, runningtime, n / runningtime);
    free(ct.C2);
}

void speed_sm4_enc() {
    int n = 100000;
    u8 key[16] = {0};
    u8 pt[16] = {0};
    u8 ct[16] = {0};
    // prevent compiler optimization
    volatile u32 length = sizeof(pt);
    sm4_ctx *ctx = malloc(sizeof(sm4_ctx));
    sm4_setkey(ctx, key, sizeof(key));

    time_start();
    for (u32 i = n; i > 0; i--) {
        sm4_encrypt(ctx, pt, ct, length);
    }
    time_end();
    double total_bits = n * 128.0;
    double mbps = total_bits / (runningtime * 1000000.0);
    printf("%-16s%-12d%-12.3f%.3f Mbps\r\n", "sm4_enc", n, runningtime, mbps);
}

void speed_sm4_dec() {
    int n = 100000;
    u8 key[16] = {0};
    u8 pt[16] = {0};
    u8 ct[16] = {0};
    // prevent compiler optimization
    volatile u32 length = sizeof(ct);
    sm4_ctx *ctx = malloc(sizeof(sm4_ctx));
    sm4_setkey(ctx, key, sizeof(key));
    time_start();
    for (u32 i = n; i > 0; i--) {
        sm4_decrypt(ctx,ct,pt,length);
    }
    time_end();
    double total_bits = n * 128.0;
    double mbps = total_bits / (runningtime * 1000000.0);
    printf("%-16s%-12d%-12.3f%.3f Mbps\r\n", "sm4_dec", n, runningtime, mbps);
    free(ctx);
}

void speed_sm3() {
    u32 n = 100, m = 1000;
    u8 *message = malloc(m * 64);
    if (!message) {
        printf("Memory allocation failed for message buffer.\r\n");
        return;
    }

    // prevent compiler optimization
    volatile u32 digest[8] = {0};
    sm3_compress((u32*)digest, message, 1);
    time_start();
    for (u32 i = 0; i < n; i++) {
        sm3_compress((u32*)digest, message, m);
    }
    time_end();

    double total_bits = (double)n * m * 512.0;
    double mbps = total_bits / (runningtime * 1000000.0);
    printf("%-16s%-12lu%-12.3f%.3f Mbps\r\n", "sm3", (n * m), runningtime, mbps);
    free(message);
}

void speed_all(void) {
    printf("Start testing the speed of each function...\r\n");
    printf("%-16s%-12s%-12s%-12s\r\n","Function", "Number", "Time", "Speed");
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
     speed_point_mul();
     speed_point_gen();
    speed_sm3();
    speed_sm4_enc();
    speed_sm4_dec();
    speed_sm2_sign();
    speed_sm2_verify();
    speed_sm2_enc();
    speed_sm2_dec();
    printf("The test is over.\r\n");
}
