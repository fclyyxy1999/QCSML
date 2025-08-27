#include "sm4.h"

#define RT(x) rt[0][(x) & 255] ^ rt[1][(x >> 8) & 255] ^ rt[2][(x >> 16) & 255] ^ rt[3][x >> 24]
#define KT(x) kt[0][x & 255] ^ kt[1][(x >> 8) & 255] ^ kt[2][(x >> 16) & 255] ^ kt[3][x >> 24]

void sm4_setkey_core(u32 *rk, const u8 *key){
    // keyscdule
    register u32 buf;
    register u32 k0 = GET32(key) ^ 0xa3b1bac6;
    register u32 k1 = GET32(key + 4) ^ 0x56aa3350;
    register u32 k2 = GET32(key + 8) ^ 0x677d9197;
    register u32 k3 = GET32(key + 12) ^ 0xb27022dc;
    // 1~4
    buf = k1 ^ k2 ^ k3 ^ 0x00070e15;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0x1c232a31;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0x383f464d;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0x545b6269;
    k3 ^= KT(buf);
    rk[0] = k0;
    rk[1] = k1;
    rk[2] = k2;
    rk[3] = k3;
    // 5~8
    buf = k1 ^ k2 ^ k3 ^ 0x70777e85;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0x8c939aa1;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0xa8afb6bd;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0xc4cbd2d9;
    k3 ^= KT(buf);
    rk[4] = k0;
    rk[5] = k1;
    rk[6] = k2;
    rk[7] = k3;
    // 9~12
    buf = k1 ^ k2 ^ k3 ^ 0xe0e7eef5;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0xfc030a11;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0x181f262d;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0x343b4249;
    k3 ^= KT(buf);
    rk[8] = k0;
    rk[9] = k1;
    rk[10] = k2;
    rk[11] = k3;
    // 13~16
    buf = k1 ^ k2 ^ k3 ^ 0x50575e65;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0x6c737a81;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0x888f969d;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0xa4abb2b9;
    k3 ^= KT(buf);
    rk[12] = k0;
    rk[13] = k1;
    rk[14] = k2;
    rk[15] = k3;
    // 17~20
    buf = k1 ^ k2 ^ k3 ^ 0xc0c7ced5;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0xdce3eaf1;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0xf8ff060d;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0x141b2229;
    k3 ^= KT(buf);
    rk[16] = k0;
    rk[17] = k1;
    rk[18] = k2;
    rk[19] = k3;
    // 21~24
    buf = k1 ^ k2 ^ k3 ^ 0x30373e45;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0x4c535a61;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0x686f767d;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0x848b9299;
    k3 ^= KT(buf);
    rk[20] = k0;
    rk[21] = k1;
    rk[22] = k2;
    rk[23] = k3;
    // 25~28
    buf = k1 ^ k2 ^ k3 ^ 0xa0a7aeb5;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0xbcc3cad1;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0xd8dfe6ed;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0xf4fb0209;
    k3 ^= KT(buf);
    rk[24] = k0;
    rk[25] = k1;
    rk[26] = k2;
    rk[27] = k3;
    // 29~32
    buf = k1 ^ k2 ^ k3 ^ 0x10171e25;
    k0 ^= KT(buf);
    buf = k2 ^ k3 ^ k0 ^ 0x2c333a41;
    k1 ^= KT(buf);
    buf = k3 ^ k0 ^ k1 ^ 0x484f565d;
    k2 ^= KT(buf);
    buf = k0 ^ k1 ^ k2 ^ 0x646b7279;
    k3 ^= KT(buf);
    rk[28] = k0;
    rk[29] = k1;
    rk[30] = k2;
    rk[31] = k3;
}

void sm4_enc_core(u8 *out, const u8 *in, const u32 *rk) {
    register u32 buf;
    register u32 r0 = GET32(in);
    register u32 r1 = GET32(in + 4);
    register u32 r2 = GET32(in + 8);
    register u32 r3 = GET32(in + 12);
    // 1~4
    buf = r1 ^ r2 ^ r3 ^ rk[0];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[1];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[2];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[3];
    r3 ^= RT(buf);
    // 5~8
    buf = r1 ^ r2 ^ r3 ^ rk[4];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[5];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[6];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[7];
    r3 ^= RT(buf);
    // 9~12
    buf = r1 ^ r2 ^ r3 ^ rk[8];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[9];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[10];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[11];
    r3 ^= RT(buf);
    // 13~16
    buf = r1 ^ r2 ^ r3 ^ rk[12];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[13];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[14];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[15];
    r3 ^= RT(buf);
    // 17~20
    buf = r1 ^ r2 ^ r3 ^ rk[16];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[17];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[18];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[19];
    r3 ^= RT(buf);
    // 21~24
    buf = r1 ^ r2 ^ r3 ^ rk[20];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[21];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[22];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[23];
    r3 ^= RT(buf);
    // 25~28
    buf = r1 ^ r2 ^ r3 ^ rk[24];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[25];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[26];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[27];
    r3 ^= RT(buf);
    // 29~32
    buf = r1 ^ r2 ^ r3 ^ rk[28];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[29];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[30];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[31];
    r3 ^= RT(buf);
    // Output the result
    PUT32(out, r3);
    PUT32(out + 4, r2);
    PUT32(out + 8, r1);
    PUT32(out + 12, r0);
}

void sm4_dec_core(u8 *out, const u8 *in, const u32 *rk) {
    register u32 buf;
    register u32 r0 = GET32(in);
    register u32 r1 = GET32(in + 4);
    register u32 r2 = GET32(in + 8);
    register u32 r3 = GET32(in + 12);
    // 1~4
    buf = r1 ^ r2 ^ r3 ^ rk[31];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[30];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[29];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[28];
    r3 ^= RT(buf);
    // 5~8
    buf = r1 ^ r2 ^ r3 ^ rk[27];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[26];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[25];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[24];
    r3 ^= RT(buf);
    // 9~12
    buf = r1 ^ r2 ^ r3 ^ rk[23];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[22];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[21];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[20];
    r3 ^= RT(buf);
    // 13~16
    buf = r1 ^ r2 ^ r3 ^ rk[19];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[18];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[17];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[16];
    r3 ^= RT(buf);
    // 17~20
    buf = r1 ^ r2 ^ r3 ^ rk[15];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[14];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[13];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[12];
    r3 ^= RT(buf);
    // 21~24
    buf = r1 ^ r2 ^ r3 ^ rk[11];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[10];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[9];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[8];
    r3 ^= RT(buf);
    // 25~28
    buf = r1 ^ r2 ^ r3 ^ rk[7];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[6];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[5];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[4];
    r3 ^= RT(buf);
    // 29~32
    buf = r1 ^ r2 ^ r3 ^ rk[3];
    r0 ^= RT(buf);
    buf = r2 ^ r3 ^ r0 ^ rk[2];
    r1 ^= RT(buf);
    buf = r3 ^ r0 ^ r1 ^ rk[1];
    r2 ^= RT(buf);
    buf = r0 ^ r1 ^ r2 ^ rk[0];
    r3 ^= RT(buf);
    // Output the result
    PUT32(out, r3);
    PUT32(out + 4, r2);
    PUT32(out + 8, r1);
    PUT32(out + 12, r0);
}