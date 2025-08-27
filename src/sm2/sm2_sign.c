#include <stdlib.h>
#include "asn1.h"
#include "sm2.h"


static void sm2_compute_z(sm3_digest z, const sm2_pubkey *pubkey, const char *id, const u32 idlen) {
    static u32 date[32] = {
        0xfeffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0xfcffffff,
        0x9efae928, 0x345e9f9d, 0x4b9e5a4d, 0xa70965cf, 0xf58997f3, 0x928fab15, 0x41bdbcdd, 0x930e944d,
        0x2caec432, 0x1981191f, 0x4604995f, 0x94c9396a, 0xbf0be38f, 0xe10b66f2, 0x89455a71, 0xc7744c33,
        0xa23637bc, 0x9c77f6f4, 0xe3cebd59, 0x5321696b, 0x7c87a9d0, 0x40472ac6, 0xe532df02, 0xa0f03921,
    };

    // compute Hash(entl || id || a || b || Gx || Gy || Px || Py)
    sm3_ctx ctx;
    sm3_init(&ctx);
    ctx.msg[0] = idlen >> 5;
    ctx.msg[1] = idlen << 3;
    ctx.len += 2;
    sm3_update(&ctx, id, idlen);
    sm3_update(&ctx, date, 128);
    sm3_update(&ctx, pubkey, 64);
    sm3_finish(&ctx, z);
}

static void sm2_compute_e(sm3_digest e, const sm3_digest z, const u8 *msg, const u64 msglen) {
    sm3_ctx ctx;
    sm3_init(&ctx);
    sm3_update(&ctx, z, 32);
    sm3_update(&ctx, msg, msglen);
    sm3_finish(&ctx, e);
}

void sm2_sign_core(sm2_signature *sig, const sm2_prikey *prikey, sm3_digest dgst) {
    sm2_fp r, s, k;
    sm2_apoint kg;

retry:
    // generate random number k in [1, n - 1], compute [k]G
    sm2_fn_rand(k);
    sm2_apoint_gen(&kg, k);

    //r = e + x_1 (mod n)
    sm2_bn_to_fp(dgst, r);
    sm2_fn_add(r, r, kg.x);

    // if r == 0 or r + k == n re-generate k
    sm2_fn_add(s, r, k);
    if (sm2_fp_is_zero(r) || sm2_fp_is_zero(s))
        goto retry;

    // s = ((1 + d)^(-1) * (k - r * d)) mod n  = (k + r)*(1 + d)^-1 - r
    sm2_fn_add(s, k ,r);
    sm2_fn_mul(s, s, prikey->d_inv);
    sm2_fn_sub(s, s, r);

    // check s != 0
    if (sm2_fp_is_zero(s))
        goto retry;

    // return r, s
    sm2_fp_to_bn(r, sig->r);
    sm2_fp_to_bn(s, sig->s);

    // clean
    memset(&kg, 0, sizeof(sm2_apoint));
    memset(&r, 0, sizeof(sm2_fp));
    memset(&s, 0, sizeof(sm2_fp));
    memset(&k, 0, sizeof(sm2_fp));
}

int sm2_verify_core(const sm2_signature *sig, const sm2_pubkey *publickey, sm3_digest dgst){

    sm2_fp sigr, sigs, t, r;
    sm2_apoint pubkey, p;

    // retrieve public key
    sm2_point_to_apoint(publickey, &pubkey);

    // check r′, s′ ∈[1,n-1]
    sm2_bn_to_fp(sig->r, sigr);
    sm2_bn_to_fp(sig->s, sigs);
    if (sm2_fp_equ(sigr, sm2_zero) || sm2_fp_cmp(sigr, sm2_n) || sm2_fp_equ(sigs, sm2_zero) || sm2_fp_cmp(sigs, sm2_n))
        return 0;

    // compute t = r' + s' mod n
    sm2_fn_add(t, sigr, sigs);
    if (sm2_fp_equ(t, sm2_zero))
        return 0;

    // compute s′G + t'p
    sm2_apoint_mutmul(&p, sigs, t, &pubkey);

    // compute r = (e′ + x_1′) mod n
    sm2_bn_to_fp(dgst, r);
    sm2_fn_add(r, r, p.x);

    // check r = r'?
    if (sm2_fp_equ(r, sigr))
        return 1;
    return 0;
}

void sm2_sign(const sm2_prikey *prikey, u8 *sig, u64 *siglen, const char *id, const u32 idlen, const void *msg, const u64 msglen){
    sm3_digest dgst;
    // compute Za = H ( entl || id || a ||b || Gx || Gy || Px || Py)
    sm2_compute_z(dgst, &prikey->pubkey, id, idlen);
    // compute e = H ( Za || M)
    sm2_compute_e(dgst, dgst, msg, msglen);
    // sign
    sm2_signature signature;
    sm2_sign_core(&signature, prikey, dgst);
    // signature to der
    sm2_sig_to_der(&signature, &sig, siglen);
}

void sm2_sign_init(sm2_sign_ctx *ctx, const sm2_prikey *prikey, const char *id, const u32 idlen) {
    ctx->prikey = prikey;
    ctx->sm3_ctx = malloc(sizeof(sm3_ctx));
    sm2_compute_z(ctx->dgst, &ctx->prikey->pubkey, id, idlen);
    sm3_init(ctx->sm3_ctx);
    sm3_update(ctx->sm3_ctx, ctx->dgst, 32);
}

void sm2_sign_update(const sm2_sign_ctx *ctx, const void *msg, const u64 msglen){
    sm3_update(ctx->sm3_ctx, msg, msglen);
}

void sm2_sign_finish(sm2_sign_ctx *ctx, u8 *sig, u64 *siglen){
    sm3_finish(ctx->sm3_ctx, ctx->dgst);
    free(ctx->sm3_ctx);
    sm2_sign_core(&ctx->sig, ctx->prikey, ctx->dgst);
    sm2_sig_to_der(&ctx->sig, &sig, siglen);
}

int sm2_verify_init(sm2_verify_ctx *ctx, const sm2_pubkey *pubkey, const char *id, const u64 idlen) {
    if (!ctx || !id) return -1;
    ctx->pubkey = pubkey;
    ctx->sm3_ctx = malloc(sizeof(sm3_ctx));
    sm2_compute_z(ctx->dgst, ctx->pubkey, id, idlen);
    sm3_init(ctx->sm3_ctx);
    sm3_update(ctx->sm3_ctx, ctx->dgst, 32);
    return 1;
}

int sm2_verify_update(sm2_verify_ctx *ctx, const void *msg, const u64 msglen){
    if (!ctx || !msg)
        return -1;
    sm3_update(ctx->sm3_ctx, msg, msglen);
    return 1;
}

int sm2_verify_finish(sm2_verify_ctx *ctx, const u8 *sig, u64 siglen){
    if (!ctx || !sig) return -1;
    sm2_signature signature;
    sm2_sig_from_der(&signature, &sig, &siglen);
    sm3_finish(ctx->sm3_ctx, ctx->dgst);
    free(ctx->sm3_ctx);
    return sm2_verify_core(&signature, ctx->pubkey, ctx->dgst);
}

int sm2_verify(const sm2_pubkey *pubkey, const u8 *sig, u64 siglen, const char *id, const u64 idlen, const void *msg, const u64 msglen) {
    sm3_digest dgst;
    // compute Za = H ( entl || id || a ||b || Gx || Gy || Px || Py)
    sm2_compute_z(dgst, pubkey, id, idlen);
    // compute e = H ( Za || M)
    sm2_compute_e(dgst, dgst, msg, msglen);

    // verify
    sm2_signature signature;
    sm2_sig_from_der(&signature, &sig, &siglen);
    return sm2_verify_core(&signature, pubkey, dgst);
}

#define Sign_sequence_max_size 70

void sm2_sig_to_der(const sm2_signature* sig, u8 **dst, u64 *dstl) {
    u8 buf[Sign_sequence_max_size], *_buf = buf;
    u64 blen = 0;
    *dstl = 0;
    asn1_integer_to_der(sig->r, 32, &_buf, &blen);
    asn1_integer_to_der(sig->s, 32, &_buf, &blen);
    asn1_sequence_to_der(buf, blen, dst, dstl);
}

void sm2_sig_from_der(sm2_signature *sig, const u8 **src, u64 *srcl) {
    u8 buf[Sign_sequence_max_size];
    const u8 *_buf = buf;
    u64 blen = 0, rlen, slen;
    asn1_sequence_from_der(buf, &blen, src, srcl);
    asn1_integer_from_der(sig->r, &rlen, &_buf, &blen);
    asn1_integer_from_der(sig->s, &slen, &_buf, &blen);
}