#include <string.h>
#include "sm3.h"

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


