#include "sm4.h"

void sm4_setkey(sm4_ctx *ctx, const u8 *key, const u32 keyLen) {
    // Check for null pointers and key length
    if (ctx == NULL || key == NULL) {
        perror("sm4_setkey: ctx or key is NULL\n");
        return;
    }
    // Check if the key length is valid
    if (keyLen != SM4_BLOCKSIZE) {
        perror("sm4_setkey: key length must be 16 bytes");
        return;
    }
    // Initialize the context
    memset(ctx, 0, sizeof(sm4_ctx));
    sm4_setkey_core(ctx->rk, key);
}

void sm4_encrypt(const sm4_ctx *ctx, const u8 *in, u8 *out, const u32 length) {
    if (ctx == NULL || in == NULL || out == NULL || length == 0) {
        perror("sm4_encrypt: ctx, in, out or length is NULL or zero\n");
        return;
    }

    if (length % SM4_BLOCKSIZE != 0) {
        perror("sm4_encrypt: length must be a multiple of 16 bytes");
        return;
    }

    const u32 blocks = length / SM4_BLOCKSIZE;
    for (u32 i = 0; i < blocks; i++) {
        sm4_enc_core(out + i * SM4_BLOCKSIZE, in + i * SM4_BLOCKSIZE, ctx->rk);
    }
}

void sm4_decrypt(const sm4_ctx *ctx, const u8 *in, u8 *out, const u32 length) {
    if (ctx == NULL || in == NULL || out == NULL || length == 0) {
        perror("sm4_decrypt: ctx, in, out or length is NULL or zero\n");
        return;
    }

    if (length % SM4_BLOCKSIZE != 0) {
        perror("sm4_decrypt: length must be a multiple of 16 bytes");
        return;
    }

    const u32 blocks = length / SM4_BLOCKSIZE;
    for (u32 i = 0; i < blocks; i++) {
        sm4_dec_core(out + i * SM4_BLOCKSIZE, in + i * SM4_BLOCKSIZE, ctx->rk);
    }
}
