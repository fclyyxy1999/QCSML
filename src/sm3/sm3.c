#include <string.h>
#include "sm3.h"

#define ROL(x, n)       (((x) << (n)) | ((x) >> (32 - (n))))
#define P0(x)           ((x) ^ ROL((x),  9) ^ ROL((x), 17))
#define P1(x)           ((x) ^ ROL((x), 15) ^ ROL((x), 23))
#define FF0(x, y, z)    (x ^ y ^ z)
#define FF1(x, y, z)    ((x & y) | (x & z) | (y & z))
#define GG0(x, y, z)    (x ^ y ^ z)
#define GG1(x, y, z)    (((y ^ z) & x) ^ z)

#define MSGEXP(W16, W0, W3, W7, W10, W13)               \
    W16 = P1(W0 ^ W7 ^ ROL(W13, 15)) ^ ROL(W3, 7) ^ W10

#define RF0(a, b, c, d, e, f, g, h, wi, wj, i)          \
    do {                                                \
        ss1 = ROL(a, 12) + e + T;                       \
        ss1 = ROL(ss1, 7);                              \
        ss2 = ROL(a, 12) ^ ss1;                         \
        d += FF0(a, b, c) + ss2 + (wj);                 \
        h += GG0(e, f, g) + ss1 + wi;                   \
        h = P0(h);                                      \
        b = ROL(b, 9);                                  \
        f = ROL(f, 19);                                 \
        T = ROL(T, 1);                                  \
    } while (0)

#define RF1(a, b, c, d, e, f, g, h, wi, wj, i)          \
    do {                                                \
        ss1 = ROL(a, 12) + e + T;                       \
        ss1 = ROL(ss1, 7);                              \
        ss2 = ROL(a, 12) ^ ss1;                         \
        d += FF1(a, b, c) + ss2 + (wj);                 \
        h += GG1(e, f, g) + ss1 + wi;                   \
        h = P0(h);                                      \
        b = ROL(b, 9);                                  \
        f = ROL(f, 19);                                 \
        T = ROL(T, 1);                                  \
    } while (0)

_asmweak void sm3_compress(u32 buf[8], const u8 *msg, u32 num) {
    register u32 a, b, c, d, e, f, g, h, ss1, ss2, T;
    u32 w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, wa, wb, wc, wd, we, wf;
    while (num--){
        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];
        e = buf[4];
        f = buf[5];
        g = buf[6];
        h = buf[7];
        w0 = GET32(msg + 0);
        w1 = GET32(msg + 4);
        w2 = GET32(msg + 8);
        w3 = GET32(msg + 12);
        w4 = GET32(msg + 16);
        w5 = GET32(msg + 20);
        w6 = GET32(msg + 24);
        w7 = GET32(msg + 28);
        w8 = GET32(msg + 32);
        w9 = GET32(msg + 36);
        wa = GET32(msg + 40);
        wb = GET32(msg + 44);
        wc = GET32(msg + 48);
        wd = GET32(msg + 52);
        we = GET32(msg + 56);
        wf = GET32(msg + 60);
        T = 0x79cc4519;
        RF0(a, b, c, d, e, f, g, h, w0, w0 ^ w4, 0);
        RF0(d, a, b, c, h, e, f, g, w1, w1 ^ w5, 1);
        RF0(c, d, a, b, g, h, e, f, w2, w2 ^ w6, 2);
        RF0(b, c, d, a, f, g, h, e, w3, w3 ^ w7, 3);
        RF0(a, b, c, d, e, f, g, h, w4, w4 ^ w8, 4);
        RF0(d, a, b, c, h, e, f, g, w5, w5 ^ w9, 5);
        RF0(c, d, a, b, g, h, e, f, w6, w6 ^ wa, 6);
        RF0(b, c, d, a, f, g, h, e, w7, w7 ^ wb, 7);
        RF0(a, b, c, d, e, f, g, h, w8, w8 ^ wc, 8);
        RF0(d, a, b, c, h, e, f, g, w9, w9 ^ wd, 9);
        RF0(c, d, a, b, g, h, e, f, wa, wa ^ we, 10);
        RF0(b, c, d, a, f, g, h, e, wb, wb ^ wf, 11);
        MSGEXP(w0, w0, w3, w7, wa, wd);
        RF0(a, b, c, d, e, f, g, h, wc, wc ^ w0, 12);
        MSGEXP(w1, w1, w4, w8, wb, we);
        RF0(d, a, b, c, h, e, f, g, wd, wd ^ w1, 13);
        MSGEXP(w2, w2, w5, w9, wc, wf);
        RF0(c, d, a, b, g, h, e, f, we, we ^ w2, 14);
        MSGEXP(w3, w3, w6, wa, wd, w0);
        RF0(b, c, d, a, f, g, h, e, wf, wf ^ w3, 15);

        T = 0x9d8a7a87;
        MSGEXP(w4, w4, w7, wb, we, w1);
        RF1(a, b, c, d, e, f, g, h, w0, w0 ^ w4, 16);
        MSGEXP(w5, w5, w8, wc, wf, w2);
        RF1(d, a, b, c, h, e, f, g, w1, w1 ^ w5, 17);
        MSGEXP(w6, w6, w9, wd, w0, w3);
        RF1(c, d, a, b, g, h, e, f, w2, w2 ^ w6, 18);
        MSGEXP(w7, w7, wa, we, w1, w4);
        RF1(b, c, d, a, f, g, h, e, w3, w3 ^ w7, 19);
        MSGEXP(w8, w8, wb, wf, w2, w5);
        RF1(a, b, c, d, e, f, g, h, w4, w4 ^ w8, 20);
        MSGEXP(w9, w9, wc, w0, w3, w6);
        RF1(d, a, b, c, h, e, f, g, w5, w5 ^ w9, 21);
        MSGEXP(wa, wa, wd, w1, w4, w7);
        RF1(c, d, a, b, g, h, e, f, w6, w6 ^ wa, 22);
        MSGEXP(wb, wb, we, w2, w5, w8);
        RF1(b, c, d, a, f, g, h, e, w7, w7 ^ wb, 23);
        MSGEXP(wc, wc, wf, w3, w6, w9);
        RF1(a, b, c, d, e, f, g, h, w8, w8 ^ wc, 24);
        MSGEXP(wd, wd, w0, w4, w7, wa);
        RF1(d, a, b, c, h, e, f, g, w9, w9 ^ wd, 25);
        MSGEXP(we, we, w1, w5, w8, wb);
        RF1(c, d, a, b, g, h, e, f, wa, wa ^ we, 26);
        MSGEXP(wf, wf, w2, w6, w9, wc);
        RF1(b, c, d, a, f, g, h, e, wb, wb ^ wf, 27);
        MSGEXP(w0, w0, w3, w7, wa, wd);
        RF1(a, b, c, d, e, f, g, h, wc, wc ^ w0, 28);
        MSGEXP(w1, w1, w4, w8, wb, we);
        RF1(d, a, b, c, h, e, f, g, wd, wd ^ w1, 29);
        MSGEXP(w2, w2, w5, w9, wc, wf);
        RF1(c, d, a, b, g, h, e, f, we, we ^ w2, 30);
        MSGEXP(w3, w3, w6, wa, wd, w0);
        RF1(b, c, d, a, f, g, h, e, wf, wf ^ w3, 31);
        MSGEXP(w4, w4, w7, wb, we, w1);
        RF1(a, b, c, d, e, f, g, h, w0, w0 ^ w4, 32);
        MSGEXP(w5, w5, w8, wc, wf, w2);
        RF1(d, a, b, c, h, e, f, g, w1, w1 ^ w5, 33);
        MSGEXP(w6, w6, w9, wd, w0, w3);
        RF1(c, d, a, b, g, h, e, f, w2, w2 ^ w6, 34);
        MSGEXP(w7, w7, wa, we, w1, w4);
        RF1(b, c, d, a, f, g, h, e, w3, w3 ^ w7, 35);
        MSGEXP(w8, w8, wb, wf, w2, w5);
        RF1(a, b, c, d, e, f, g, h, w4, w4 ^ w8, 36);
        MSGEXP(w9, w9, wc, w0, w3, w6);
        RF1(d, a, b, c, h, e, f, g, w5, w5 ^ w9, 37);
        MSGEXP(wa, wa, wd, w1, w4, w7);
        RF1(c, d, a, b, g, h, e, f, w6, w6 ^ wa, 38);
        MSGEXP(wb, wb, we, w2, w5, w8);
        RF1(b, c, d, a, f, g, h, e, w7, w7 ^ wb, 39);
        MSGEXP(wc, wc, wf, w3, w6, w9);
        RF1(a, b, c, d, e, f, g, h, w8, w8 ^ wc, 40);
        MSGEXP(wd, wd, w0, w4, w7, wa);
        RF1(d, a, b, c, h, e, f, g, w9, w9 ^ wd, 41);
        MSGEXP(we, we, w1, w5, w8, wb);
        RF1(c, d, a, b, g, h, e, f, wa, wa ^ we, 42);
        MSGEXP(wf, wf, w2, w6, w9, wc);
        RF1(b, c, d, a, f, g, h, e, wb, wb ^ wf, 43);
        MSGEXP(w0, w0, w3, w7, wa, wd);
        RF1(a, b, c, d, e, f, g, h, wc, wc ^ w0, 44);
        MSGEXP(w1, w1, w4, w8, wb, we);
        RF1(d, a, b, c, h, e, f, g, wd, wd ^ w1, 45);
        MSGEXP(w2, w2, w5, w9, wc, wf);
        RF1(c, d, a, b, g, h, e, f, we, we ^ w2, 46);
        MSGEXP(w3, w3, w6, wa, wd, w0);
        RF1(b, c, d, a, f, g, h, e, wf, wf ^ w3, 47);
        MSGEXP(w4, w4, w7, wb, we, w1);
        RF1(a, b, c, d, e, f, g, h, w0, w0 ^ w4, 48);
        MSGEXP(w5, w5, w8, wc, wf, w2);
        RF1(d, a, b, c, h, e, f, g, w1, w1 ^ w5, 49);
        MSGEXP(w6, w6, w9, wd, w0, w3);
        RF1(c, d, a, b, g, h, e, f, w2, w2 ^ w6, 50);
        MSGEXP(w7, w7, wa, we, w1, w4);
        RF1(b, c, d, a, f, g, h, e, w3, w3 ^ w7, 51);
        MSGEXP(w8, w8, wb, wf, w2, w5);
        RF1(a, b, c, d, e, f, g, h, w4, w4 ^ w8, 52);
        MSGEXP(w9, w9, wc, w0, w3, w6);
        RF1(d, a, b, c, h, e, f, g, w5, w5 ^ w9, 53);
        MSGEXP(wa, wa, wd, w1, w4, w7);
        RF1(c, d, a, b, g, h, e, f, w6, w6 ^ wa, 54);
        MSGEXP(wb, wb, we, w2, w5, w8);
        RF1(b, c, d, a, f, g, h, e, w7, w7 ^ wb, 55);
        MSGEXP(wc, wc, wf, w3, w6, w9);
        RF1(a, b, c, d, e, f, g, h, w8, w8 ^ wc, 56);
        MSGEXP(wd, wd, w0, w4, w7, wa);
        RF1(d, a, b, c, h, e, f, g, w9, w9 ^ wd, 57);
        MSGEXP(we, we, w1, w5, w8, wb);
        RF1(c, d, a, b, g, h, e, f, wa, wa ^ we, 58);
        MSGEXP(wf, wf, w2, w6, w9, wc);
        RF1(b, c, d, a, f, g, h, e, wb, wb ^ wf, 59);
        MSGEXP(w0, w0, w3, w7, wa, wd);
        RF1(a, b, c, d, e, f, g, h, wc, wc ^ w0, 60);
        MSGEXP(w1, w1, w4, w8, wb, we);
        RF1(d, a, b, c, h, e, f, g, wd, wd ^ w1, 61);
        MSGEXP(w2, w2, w5, w9, wc, wf);
        RF1(c, d, a, b, g, h, e, f, we, we ^ w2, 62);
        MSGEXP(w3, w3, w6, wa, wd, w0);
        RF1(b, c, d, a, f, g, h, e, wf, wf ^ w3, 63);

        buf[0] ^= a;
        buf[1] ^= b;
        buf[2] ^= c;
        buf[3] ^= d;
        buf[4] ^= e;
        buf[5] ^= f;
        buf[6] ^= g;
        buf[7] ^= h;
        msg += 64;
    }
}


void sm3_init(sm3_ctx *ctx){
    memset(ctx, 0, sizeof(sm3_ctx));
    ctx->reg[0] = 0x7380166f;
    ctx->reg[1] = 0x4914b2b9;
    ctx->reg[2] = 0x172442d7;
    ctx->reg[3] = 0xda8a0600;
    ctx->reg[4] = 0xa96f30bc;
    ctx->reg[5] = 0x163138aa;
    ctx->reg[6] = 0xe38dee4d;
    ctx->reg[7] = 0xb0fb0e4e;
}

void sm3_update(sm3_ctx *ctx, const void *msg, u64 dlen){
    if (dlen == 0)
        return;
    const u64 used = ctx->len & 0x3f;
    u8 *data = (u8 *)msg;
    ctx->len += dlen;

    if (used) {
        const u64 free = 64 - used;
        if (dlen <= free) {
            memcpy(ctx->msg + used, data, dlen);
            return;
        }
        memcpy(ctx->msg + used, data, free);
        sm3_compress(ctx->reg, ctx->msg, 1);
        data += free;
        dlen -= free;
    }
    if (dlen) {
        const u64 blocks = dlen >> 6;
        if (blocks) {
            sm3_compress(ctx->reg, data, blocks);
            data += blocks << 6;
            dlen -= blocks << 6;
        }
        memcpy(ctx->msg, data, dlen);
    }
}

void sm3_finish(sm3_ctx *ctx, u8 dgst[32]) {
    const u64 used = ctx->len & 0x3f;
    // Padding
    if (0 < used && used < 56) {
        ctx->msg[used] = 0x80;
        memset(ctx->msg + used + 1, 0, 55 - used);
    } else if (used > 55) {
        ctx->msg[used] = 0x80;
        memset(ctx->msg + used + 1, 0, 63 - used);
        sm3_compress(ctx->reg, ctx->msg, 1);
        memset(ctx->msg, 0, 56);
    } else {
        ctx->msg[0] = 0x80;
        memset(ctx->msg + 1, 0, 55);
    }
    PUT32(ctx->msg + 56, ctx->len >> 29);
    PUT32(ctx->msg + 60, ctx->len << 03);
    sm3_compress(ctx->reg, ctx->msg, 1);
    // Output
    for (int i = 0; i < 8; i++) {
        PUT32(dgst + (i << 2), ctx->reg[i]);
    }
    //Clean
    memset(ctx, 0, sizeof(*ctx));
}

void sm3(u8 digest[32], const void *msg, const u64 mlen) {
    // init
    u32 reg[8] = {0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e};
    // update
    sm3_compress(reg, msg, mlen >> 6);
    msg += mlen >> 6 << 6;
    // padding and finish
    u8 buf[64];
    const u32 n = mlen & 63;
    if (0 < n && n < 56) {
        memcpy(buf, msg, n);
        buf[n] = 0x80;
        memset(buf + n + 1, 0, 55 - n);
    } else if (n > 55) {
        memcpy(buf, msg, n);
        buf[n] = 0x80;
        memset(buf + n + 1, 0, 63 - n);
        sm3_compress(reg, buf, 1);
        memset(buf, 0, 56);
    } else {
        buf[0] = 0x80;
        memset(buf + 1, 0, 55);
    }
    PUT32(buf + 56, mlen >> 29);
    PUT32(buf + 60, mlen << 03);
    sm3_compress(reg, buf, 1);
    // Output
    for (int i = 0; i < 8; i++) {
        PUT32(digest + (i << 2), reg[i]);
    }
}


