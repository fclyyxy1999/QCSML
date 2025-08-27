#include <stdlib.h>
#include <string.h>
#include "sm2.h"
#include "sm3.h"

void sm2_keydepair(u8 *output, const sm2_point* p, const u32 klen) {
    u8 reg[68];
    u32 ct = 1;
    const u32 n = klen >> 5;
    memcpy(reg, p, 64);
    for (; ct < n + 1; ct++) {
        PUT32(reg + 64, ct);
        sm3(output, reg, 68);
        output += 32;
    }
    PUT32(reg + 64, ct);
    sm3(reg, reg, 68);
    memcpy(output, reg, klen & 0x1f);
}

void sm2_encrypt_core(sm2_ciptext *ciphertext, const sm2_pubkey *pubkey, const char *msg, const u64 msglen) {
    // rand k in [1, n - 1]
    sm2_fp k;
    sm2_fn_rand(k);

    // compute c1 = [k]G
    sm2_point_gen(&ciphertext->C1, k);

    // compute c2 = [k]pubkey = (x2, y2)
    sm2_point kp;
    sm2_point_mul(&kp, k, pubkey);
    sm2_keydepair(ciphertext->C2, &kp, msglen);
    for (u32 i = 0; i < msglen; i++)
        ciphertext->C2[i] ^= msg[i];

    // compute hash(x2 || M || y2)
    sm3_ctx ctx;
    sm3_init(&ctx);
    sm3_update(&ctx, kp.x, 32);
    sm3_update(&ctx, msg, msglen);
    sm3_update(&ctx, kp.y, 32);
    sm3_finish(&ctx, ciphertext->C3);
    ciphertext->clen = msglen;
}

int sm2_decrypt_core(u8 *pt, u64 *ptlen, const sm2_prikey *prikey, const sm2_ciptext *ciptext) {
    // check point on curve
    if (sm2_point_is_on_curve(&ciptext->C1) == 0)
        return -1;

    // compute c2 = [d] c1
    sm2_point C2;
    sm2_point_mul(&C2, prikey->d, &ciptext->C1);

    // compute plaintext
    sm2_keydepair(pt, &C2, ciptext->clen);
    for (u32 i = 0; i < ciptext->clen; i++)
        pt[i] ^= ciptext->C2[i];

    // compute c3 = hash(x2 || M || y2)
    sm3_digest C3;
    sm3_ctx ctx;
    sm3_init(&ctx);
    sm3_update(&ctx, C2.x, 32);
    sm3_update(&ctx, pt, ciptext->clen);
    sm3_update(&ctx, C2.y, 32);
    sm3_finish(&ctx, C3);

    // check c3
    for (int i = 0; i <= 31; i++)
        if (C3[i] != ciptext->C3[i])
            return 0;
    *ptlen = ciptext->clen;
    return 1;
}

void sm2_encrypt(const sm2_pubkey *pubkey, u8 *ct, u64 *ctlen, const void* pt, const u64 ptlen) {
    sm2_ciptext ciptext;
    ciptext.C2 = malloc(ptlen);
    sm2_encrypt_core(&ciptext, pubkey, pt, ptlen);
    sm2_ciptext_to_der(&ciptext, &ct, ctlen);
    free(ciptext.C2);
}

int sm2_decrypt(const sm2_prikey *prikey, u8 *pt, u64 *ptlen, const void *ct, u64 ctlen){
    int ret = 1;
    sm2_ciptext ciptext = {0};
    const u8 *_ct = ct;
    ciptext.C2 = malloc(ctlen - 96);
    sm2_ciptext_from_der(&ciptext, &_ct, &ctlen);
    ret &= sm2_decrypt_core(pt, ptlen, prikey, &ciptext);
    free(ciptext.C2);
    return ret;
}

void sm2_ciptext_to_der(const sm2_ciptext* ct, u8 **dst, u64 *dstl) {
    u8 *buf = malloc(110 + ct->clen), *_buf = buf;
    u64 blen = 0;
    *dstl = 0;
    asn1_integer_to_der(ct->C1.x, 32, &_buf, &blen);
    asn1_integer_to_der(ct->C1.y, 32, &_buf, &blen);
    asn1_octets_to_der(ct->C3, 32, &_buf, &blen);
    asn1_octets_to_der(ct->C2, ct->clen, &_buf, &blen);
    asn1_sequence_to_der(buf, blen, dst, dstl);
    free(buf);
}

void sm2_ciptext_from_der(sm2_ciptext *ct, const u8 **src, u64 *srcl) {
    u8 *buf = malloc(*srcl);
    const u8 *_buf = buf;
    u64 seqlen = 0, genlen = 0, clen;
    asn1_sequence_from_der(buf, &seqlen, src, srcl);
    asn1_integer_from_der(ct->C1.x, &genlen, &_buf, &seqlen);
    asn1_integer_from_der(ct->C1.y, &genlen, &_buf, &seqlen);
    asn1_octets_from_der(ct->C3, &genlen, &_buf, &seqlen);
    asn1_octets_from_der(ct->C2, &clen, &_buf, &seqlen);
    ct->clen = (u32) clen;
    free(buf);
}

// int sm2_encrypt_init();
//
// int sm2_encrypt_update();
//
// int sm2_encrypt_finish();
//
// int sm2_decrypt_init();
//
// int sm2_decrypt_update();
//
// int sm2_decrypt_finish();