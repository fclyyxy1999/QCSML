#include "sm2.h"

int sm2_keygen(sm2_prikey *prikey) {
    if (!prikey) {
        return -1; // error
    }

    sm2_fn_rand(prikey->d);
    sm2_fp_to_bn(prikey->d, prikey->prikey);
    sm2_point_gen(&prikey->pubkey, prikey->d);
    sm2_fn_add(prikey->d_inv, prikey->d, sm2_one);
    sm2_fn_inv(prikey->d_inv, prikey->d_inv);
    return 1;
}


void sm2_prikey_from_str(sm2_prikey *prikey, const char *s) {
    memset(prikey, 0, sizeof(sm2_prikey));
    const u32 slen = strlen(s);
    switch (slen){
    case 64:
        sm2_bn_from_str(prikey->prikey, s);
        break;
    case 192:
        sm2_bn_from_str(prikey->prikey, s);
        sm2_point_from_str(&prikey->pubkey, s + 64);
        break;
    default:
        ;
    }
    sm2_prikey_check(prikey);
}

void sm2_prikey_from_bytes(sm2_prikey *prikey, const u8 *b, const u32 blen) {
    memset(prikey, 0, sizeof(sm2_prikey));
    switch (blen) {
    case 32:
        memcpy(prikey->prikey, b, 32);
        break;
    case 192:
        memcpy(prikey->prikey, b, 192);
        break;
    default: ;
    }
    sm2_prikey_check(prikey);
}

inline void sm2_pubkey_from_str(sm2_pubkey *pubkey, const char *s) {
    sm2_point_from_str(pubkey, s);
}

inline void sm2_pubkey_from_bytes(sm2_pubkey *pubkey, const u8 *b, const u32 blen) {
    sm2_point_from_bytes(pubkey, b, blen);
}

void sm2_prikey_check(sm2_prikey *prikey) {
    if (sm2_fp_equ(prikey->d, sm2_zero)) {
        sm2_bn_to_fp(prikey->prikey, prikey->d);
    }
    if (memcmp(prikey->pubkey.x, sm2_zero, 32) == 0 && memcmp(prikey->pubkey.y, sm2_zero, 32) == 0) {
        sm2_apoint p;
        sm2_apoint_gen(&p, prikey->d);
        sm2_apoint_to_point(&p, &prikey->pubkey);
    }
    if (sm2_fp_equ(prikey->d_inv, sm2_zero)) {
        sm2_fn_add(prikey->d_inv, prikey->d, sm2_one);
        sm2_fn_inv(prikey->d_inv, prikey->d_inv);
    }
}