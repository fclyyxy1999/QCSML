#include "sm2_core.h"

const sm2_fp sm2_zero = {0};
const sm2_fp sm2_one = {1};
const sm2_fp sm2_p = {0xFFFFFFFFU, 0xFFFFFFFFU, 0x00000000U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU};
const sm2_fp sm2_n = {0x39D54123U, 0x53BBF409U, 0x21C6052BU, 0x7203DF6BU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU};
const sm2_fp sm2_a = {0xFFFFFFFCU, 0xFFFFFFFFU, 0x00000000U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU};
const sm2_fp sm2_b = {0x4D940E93U, 0xDDBCBD41U, 0x15AB8F92U, 0xF39789F5U, 0xCF6509A7U, 0x4D5A9E4BU, 0x9D9F5E34U, 0x28E9FA9EU};
const sm2_fp sm2_s = {0x00000000U, 0x80000000U, 0x80000000U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x7FFFFFFFU, 0x7FFFFFFFU};

void sm2_fp_set(sm2_fp r, const sm2_fp a) {
    memcpy(r, a, sizeof(sm2_fp));
}

int sm2_fp_is_odd(const sm2_fp a){
    return (int) a[0] & 1;
}

int sm2_fp_is_even(const sm2_fp a){
    return (int) (a[0] & 1) ^ 1;
}

int sm2_fp_is_zero(const sm2_fp a) {
    return a[0] == 0 && a[1] == 0 && a[2] == 0 && a[3] == 0 && a[4] == 0 && a[5] == 0 && a[6] == 0 && a[7] == 0;
}

int sm2_fp_non_zero(const sm2_fp a) {
    return a[0] > 0 || a[1] > 0 || a[2] > 0 || a[3] > 0 || a[4] > 0 || a[5] > 0 || a[6] > 0 || a[7] > 0;
}

int sm2_fp_equ(const sm2_fp a, const sm2_fp b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] && a[4] == b[4] && a[5] == b[5] && a[6] == b[6] && a[7] == b[7];
}

void sm2_fp_tri(sm2_fp r, const sm2_fp a){
    sm2_fp_dbl(r, a);
    sm2_fp_add(r, r, a);
}

void sm2_fp_cub(sm2_fp r, const sm2_fp a) {
    sm2_fp_sqr(r, a);
    sm2_fp_mul(r, r, a);
}

void sm2_fn_rand(sm2_fp r) {
    sm2_bn tmp = {0};
    do {
        if (rand_gen(tmp, sizeof(sm2_fp)) != 0) {
            fprintf(stderr, "Error generating random bytes\n");
            return;
        }
        sm2_bn_to_fp(tmp, r);
    } while (sm2_fp_cmp(r, sm2_n) || sm2_fp_is_zero(r));
}
// Definition of function for type conversion. *******************************************************************************************************

static const u8 hstr_to_byte[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
        0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void bytes_from_hexstr(void *b, const char* s, const u32 nbytes) {
    u8 *byte = b;
    for (u32 i = 0; i < nbytes; i ++)
        byte[i] = hstr_to_byte[(u8)s[i << 1]] << 4 | hstr_to_byte[(u8)s[(i << 1) + 1]];
}

void sm2_fp_from_str(sm2_fp n, const char* s) {
    u32 t[64];
    for (int i = 0; i < 64; i++)
        t[63 - i] = hstr_to_byte[(u8)s[i]];
    for (int i = 0; i < 8; i++) {
        n[i] = 0;
        for (int j = 0; j < 8; j++)
            n[i] |= t[(i << 3) + j] << (j << 2);
    }
}

void sm2_fp_to_str(const sm2_fp n, char* s) {
    sm2_bn t;
    sm2_fp_to_bn(n, t);
    sm2_bn_to_str(t, s);
}

void sm2_bn_from_str(sm2_bn b, const char* s) {
    bytes_from_hexstr(b, s, 32);
}

void sm2_bn_to_str(const sm2_bn b, char* s) {
    static const char lbyt_to_str[256] = {
            '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
            '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3',
            '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5',
            '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7',
            '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9',
            'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
            'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D',
            'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F'};
    static const char rbyt_to_str[256] = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0, j = 0; i < 32; i++, j += 2) {
        s[j] = lbyt_to_str[b[i]];
        s[j + 1] = rbyt_to_str[b[i]];
    }
}

void sm2_fp_to_bn(const sm2_fp n, sm2_bn b) {
    const u8 *t = (const u8 *) n;
    for (int i = 0; i < 32; i++)
        b[i] = t[31 - i];
}

void sm2_bn_to_fp(const sm2_bn b, sm2_fp n) {
    u8 *t = (u8 *) n;
    for (int i = 0; i < 32; i++)
        t[31 - i] = b[i];
}

void sm2_fp_naf(sm2_naf r, const u8 w, const sm2_fp n) {
    if (w > 7)
        return;     // w > 7 is not supported unless the sm2_naf type definition is expanded from i8[257] to int[257]

    int i = 256;
    sm2_fp k, t;
    sm2_fp_set(k, n);
    while (sm2_fp_cmp(k, sm2_one)) {
        if (sm2_fp_is_odd(k)) {
            sm2_fp_set(t, sm2_zero);
            t[0] = k[0] & ((1 << w) - 1);
            if (t[0] >> (w - 1)) {
                t[0] = (1 << w) - t[0];
                r[i] = (i8) -t[0];
            } else {
                r[i] = (i8) t[0];
            }
            if (r[i] > 0)
                sm2_fp_sub(k, k, t);
            else
                sm2_fp_add(k, k, t);
        } else
            r[i] = 0;
        sm2_fp_haf(k, k);
        i--;
    }
    while (i >= 0) {
        r[i--] = 0;
    }
}

void sm2_fp_nafp(i8 K[52], const sm2_fp k) {
    sm2_naf kn;
    sm2_fp_naf(kn, 2, k);
    int i, j = 0;
    for (i = 256; i > 2; i = i-5){
        K[j++] = (i8) (kn[i] + (kn[i-1]<<1) + (kn[i-2]<<2) + (kn[i-3]<<3) + (kn[i-4]<<4));
    }
    K[j] = (i8) (kn[1] + (kn[0]<<1));
}

void sm2_point_gen(sm2_point *r, const sm2_fp k) {
    sm2_apoint p;
    sm2_apoint_gen(&p, k);
    sm2_apoint_to_point(&p, r);
}

void sm2_point_mul(sm2_point *r, const sm2_fp k, const sm2_point *p){
    sm2_apoint t;
    sm2_point_to_apoint(p, &t);
    sm2_apoint_mul(&t, k, &t);
    sm2_apoint_to_point(&t, r);
}

int sm2_point_is_on_curve(const sm2_point *r) {
    sm2_fp x, y, t1, t2;
    sm2_bn_to_fp(r->x, x);
    sm2_bn_to_fp(r->y, y);
    sm2_fp_cub(t1, x);
    sm2_fp_mul(t2, x, sm2_a);
    sm2_fp_add(t1, t1, t2);
    sm2_fp_add(t1, t1, sm2_b);
    sm2_fp_sqr(t2, y);
    return sm2_fp_cmp(t1, t2);
}

void sm2_point_from_str(sm2_point* p, const char *s) {
    const u32 slen = strlen(s);
    switch (slen) {
        case 128:
            sm2_bn_from_str(p->x, s);
            sm2_bn_from_str(p->y, s + 64);
            break;
        case 130:
            sm2_bn_from_str(p->x, s + 2);
            sm2_bn_from_str(p->y, s + 66);
            break;
        case 66:
            // not support for compressed point
        default:
            ;
    }
}

void sm2_point_from_bytes(sm2_point *p, const u8 *b, const u32 blen) {
    switch (blen) {
        case 64:
            memcpy(p, b, 64);
            break;
        case 65:
            memcpy(p, b + 1, 64);
            break;
        case 33:
            //not support for compressed pubkey
        default: ;
    }
}

void sm2_point_to_apoint(const sm2_point *src, sm2_apoint *p){
    sm2_bn_to_fp(src->x, p->x);
    sm2_bn_to_fp(src->y, p->y);
}

//**********************************************************************************************************************

void sm2_apoint_to_point(const sm2_apoint* p, sm2_point *dst){
    sm2_fp_to_bn(p->x, dst->x);
    sm2_fp_to_bn(p->y, dst->y);
}

void sm2_apoint_set(sm2_apoint *p, const sm2_fp x, const sm2_fp y) {
    sm2_fp_set(p->x, x);
    sm2_fp_set(p->y, y);
}

void sm2_apoint_init(sm2_apoint *r) {
    sm2_apoint_set(r, sm2_zero, sm2_zero);
}

int sm2_apoint_at_infinity(const sm2_apoint *r) {
    return sm2_fp_equ(r->x, sm2_zero) && sm2_fp_equ(r->y, sm2_zero);
}

void sm2_apoint_from_str(sm2_apoint *p, const char *s) {
    const u32 slen = strlen(s);
    switch (slen) {
        case 128:
            sm2_fp_from_str(p->x, s);
            sm2_fp_from_str(p->y, s + 64);
            break;
        case 130:
            sm2_fp_from_str(p->x, s + 2);
            sm2_fp_from_str(p->y, s + 66);
            break;
        case 66:
            // not support for compressed point
        default:
            ;
    }
}

void sm2_apoint_from_bytes(sm2_apoint *p, const u8 *b, const u32 blen) {
    switch (blen) {
        case 64:
            sm2_bn_to_fp(b,      p->x);
            sm2_bn_to_fp(b + 32, p->y);
            break;
        case 65:
            sm2_bn_to_fp(b + 1,  p->x);
            sm2_bn_to_fp(b + 33, p->y);
            break;
        case 33:
            //not support for compressed pubkey
        default: ;
    }
}

void sm2_apoint_mul(sm2_apoint *r, const sm2_fp k, const sm2_apoint *p) {
    sm2_jpoint t;
    sm2_apoint_to_jpoint(p, &t);
    sm2_jpoint_mul(&t, k, &t);
    sm2_jpoint_to_apoint(&t, r);
}

void sm2_apoint_gen(sm2_apoint *r, const sm2_fp k) {
    sm2_jpoint g;
    sm2_jpoint_gen(&g, k);
    sm2_jpoint_to_apoint(&g, r);
}

void sm2_apoint_mutmul(sm2_apoint *r, sm2_fp s, sm2_fp t, const sm2_apoint *q) {
    sm2_jpoint g, t1, t2;
    sm2_jpoint_gen(&t1, s);
    sm2_jpoint_set(&g, q->x, q->y, sm2_one);
    sm2_jpoint_mul(&t2, t, &g);
    sm2_jpoint_add(&g, &t1, &t2);
    sm2_jpoint_to_apoint(&g, r);
}

void sm2_apoint_to_jpoint(const sm2_apoint *p, sm2_jpoint *r){
    memcpy(r, p, sizeof(sm2_apoint));
    if (sm2_apoint_at_infinity(p))
        memcpy(r->z, sm2_zero, sizeof(sm2_fp));
    else
        memcpy(r->z, sm2_one, sizeof(sm2_fp));
}

//**********************************************************************************************************************

void sm2_jpoint_to_apoint(const sm2_jpoint *a, sm2_apoint *r) {
    sm2_fp t1, t2;
    sm2_fp_inv(t1, a->z);
    sm2_fp_sqr(t2, t1);
    sm2_fp_mul(r->x, t2, a->x);
    sm2_fp_mul(t2, t2, t1);
    sm2_fp_mul(r->y, t2, a->y);
}

void sm2_jpoint_copy(sm2_jpoint *p, const sm2_jpoint *q) {
    memcpy(p, q, 96);
}

void sm2_jpoint_set(sm2_jpoint *p, const sm2_fp x, const sm2_fp y, const sm2_fp z) {
    sm2_fp_set(p->x, x);
    sm2_fp_set(p->y, y);
    sm2_fp_set(p->z, z);
}

void sm2_jpoint_init(sm2_jpoint *r) {
    sm2_jpoint_set(r, sm2_one, sm2_one, sm2_zero);
}

int sm2_jpoint_at_infinity(const sm2_jpoint *r){
    return sm2_fp_is_zero(r->z);
}

void sm2_jpoint_from_str(sm2_jpoint *p, const char *s) {
    sm2_fp_from_str(p->x, s);
    sm2_fp_from_str(p->y, s + 64);
    sm2_fp_from_str(p->z, s + 128);
}

void sm2_jpoint_add(sm2_jpoint *r, const sm2_jpoint *p, const sm2_jpoint *q) {
    // Check if one of the points is the point at infinity
    if (sm2_jpoint_at_infinity(p)) {
        sm2_jpoint_copy(r, q);
        return;
    }
    if (sm2_jpoint_at_infinity(q)) {
        sm2_jpoint_copy(r, p);
        return;
    }

    const u32 *x1 = p->x, *y1 = p->y, *z1 = p->z;
    const u32 *x2 = q->x, *y2 = q->y, *z2 = q->z;
    sm2_fp x3, y3, z3, u1, u2, s1, s2, h, n, h2, h3, u1h2, t1, t2;

    sm2_fp_sqr(t1, z1);                     // t1 = z1^2
    sm2_fp_sqr(t2, z2);                     // t2 = z2^2
    sm2_fp_mul(u1, x1, t2);                 // u1 = x1 * z2^2
    sm2_fp_mul(u2, x2, t1);                 // u2 = x2 * z1^2
    sm2_fp_mul(t1, t1, z1);                 // t1 = z1^3
    sm2_fp_mul(t2, t2, z2);                 // t2 = z2^3
    sm2_fp_mul(s1, y1, t2);                 // s1 = y1 * z2^3
    sm2_fp_mul(s2, y2, t1);                 // s2 = y2 * z1^3
    if (sm2_fp_equ(u1, u2)) {
        if (sm2_fp_equ(s1, s2))
            sm2_jpoint_dou(r, p);
        else
            sm2_jpoint_init(r);
        return;
    }
    sm2_fp_sub(h, u2, u1);                  // h = u2 - u1
    sm2_fp_sub(n, s2, s1);                  // n = s2 - s1
    sm2_fp_sqr(h2, h);                      // h2 = h^2
    sm2_fp_mul(h3, h2, h);                  // h3 = h^3
    sm2_fp_mul(u1h2, u1, h2);               // u1h2 = u1 * h^2
    sm2_fp_dbl(t1, u1h2);                   // t1 = 2u1h2
    sm2_fp_sqr(x3, n);                      // x3 = n^2
    sm2_fp_sub(x3, x3, h3);                 // x3 = n^2 - h3
    sm2_fp_sub(x3, x3, t1);                 // x3 = n^2 - h3 - 2u1h2
    sm2_fp_mul(t1, s1, h3);                 // t1 = s1 * h3
    sm2_fp_sub(y3, u1h2, x3);               // y3 = u1h2 - x3
    sm2_fp_mul(y3, y3, n);                  // y3 = n * (u1h2 - x3)
    sm2_fp_sub(y3, y3, t1);                 // y3 = n * (u1h2 - x3) - s1 * h3
    sm2_fp_mul(z3, z1, z2);                 // z3 = z1 * z2
    sm2_fp_mul(z3, z3, h);                  // z3 = h * z1 * z2
    sm2_jpoint_set(r, x3, y3, z3);
}

void sm2_jpoint_sub(sm2_jpoint *r, const sm2_jpoint *p, const sm2_jpoint *q) {
    // Check if one of the points is the point at infinity
    if (sm2_jpoint_at_infinity(p)) {
        sm2_jpoint_copy(r, q);
        return;
    }
    if (sm2_jpoint_at_infinity(q)) {
        sm2_jpoint_copy(r, p);
        return;
    }

    const u32 *x1 = p->x, *y1 = p->y, *z1 = p->z, *x2 = q->x, *z2 = q->z;
    sm2_fp y2, x3, y3, z3, u1, u2, s1, s2, h, n, h2, h3, u1h2, t1, t2;
    sm2_fp_neg(y2, q->y);
    sm2_fp_sqr(t1, z1);                     // t1 = z1^2
    sm2_fp_sqr(t2, z2);                     // t2 = z2^2
    sm2_fp_mul(u1, x1, t2);                 // u1 = x1 * z2^2
    sm2_fp_mul(u2, x2, t1);                 // u2 = x2 * z1^2
    sm2_fp_mul(t1, t1, z1);                 // t1 = z1^3
    sm2_fp_mul(t2, t2, z2);                 // t2 = z2^3
    sm2_fp_mul(s1, y1, t2);                 // s1 = y1 * z2^3
    sm2_fp_mul(s2, y2, t1);                 // s2 = y2 * z1^3
    if (sm2_fp_equ(u1, u2)) {
        if (sm2_fp_equ(s1, s2))
            sm2_jpoint_dou(r, p);
        else
            sm2_jpoint_init(r);
        return;
    }
    sm2_fp_sub(h, u2, u1);                  // h = u2 - u1
    sm2_fp_sub(n, s2, s1);                  // n = s2 - s1
    sm2_fp_sqr(h2, h);                      // h2 = h^2
    sm2_fp_mul(h3, h2, h);                  // h3 = h^3
    sm2_fp_mul(u1h2, u1, h2);               // u1h2 = u1 * h^2
    sm2_fp_dbl(t1, u1h2);                   // t1 = 2u1h2
    sm2_fp_sqr(x3, n);                      // x3 = n^2
    sm2_fp_sub(x3, x3, h3);                 // x3 = n^2 - h3
    sm2_fp_sub(x3, x3, t1);                 // x3 = n^2 - h3 - 2u1h2
    sm2_fp_mul(t1, s1, h3);                 // t1 = s1 * h3
    sm2_fp_sub(y3, u1h2, x3);               // y3 = u1h2 - x3
    sm2_fp_mul(y3, y3, n);                  // y3 = n * (u1h2 - x3)
    sm2_fp_sub(y3, y3, t1);                 // y3 = n * (u1h2 - x3) - s1 * h3
    sm2_fp_mul(z3, z1, z2);                 // z3 = z1 * z2
    sm2_fp_mul(z3, z3, h);                  // z3 = h * z1 * z2
    sm2_jpoint_set(r, x3, y3, z3);
}

void sm2_japoint_add(sm2_jpoint *r, const sm2_jpoint *p, const sm2_apoint *q) {
    if (sm2_jpoint_at_infinity(p)) {
        sm2_apoint_to_jpoint(q, r);
        return;
    }
    if (sm2_apoint_at_infinity(q)) {
        sm2_jpoint_copy(r, p);
        return;
    }

    const u32 *x1 = p->x, *y1 = p->y, *z1 = p->z, *x2 = q->x, *y2 = q->y;
    sm2_fp x3, y3, z3, t1, t2, t3, t4;

    sm2_fp_sqr(t1, z1);                     // t1 = A = z1^2
    sm2_fp_mul(t2, t1, z1);                 // t2 = B = z1 * A
    sm2_fp_mul(t1, t1, x2);                 // t1 = C = x2 * A
    sm2_fp_mul(t2, t2, y2);                 // t2 = D = y2 * B
    sm2_fp_sub(t1, t1, x1);                 // t1 = E = C - x1
    sm2_fp_sub(t2, t2, y1);                 // t2 = F = D - y1
    if (sm2_fp_equ(t1, sm2_zero)) {
        if (sm2_fp_equ(t2, sm2_zero)) {
            sm2_jpoint t;
            sm2_jpoint_set(&t, x2, y2, sm2_one);
            sm2_jpoint_dou(r, &t);
        } else {
            sm2_jpoint_init(r);
        }
        return;
    }
    sm2_fp_mul(z3, z1, t1);                 // z3 = z1 * E
    sm2_fp_sqr(t3, t1);                     // t3 = G = E^2
    sm2_fp_mul(t4, t3, t1);                 // t4 = H = E^3
    sm2_fp_mul(t3, t3, x1);                 // t3 = I = x1 * G
    sm2_fp_dbl(t1, t3);                     // t1 = 2I
    sm2_fp_sqr(x3, t2);                     // x3 = F^2
    sm2_fp_sub(x3, x3, t1);                 // x3 = F^2 - 2I
    sm2_fp_sub(x3, x3, t4);                 // x3 = F^2 - 2I - H
    sm2_fp_sub(t3, t3, x3);                 // t3 = I - x3
    sm2_fp_mul(t3, t3, t2);                 // t3 = (I - x3) * F
    sm2_fp_mul(t4, t4, y1);                 // t4 = y1 * H
    sm2_fp_sub(y3, t3, t4);                 // y3 = (I - x3) * F - y1 * H
    sm2_jpoint_set(r, x3, y3, z3);
}

void sm2_japoint_sub(sm2_jpoint *r, const sm2_jpoint *p, const sm2_apoint *q) {
    const u32 *x1 = p->x, *y1 = p->y, *z1 = p->z, *x2 = q->x;
    sm2_fp y2, x3, y3, z3, t1, t2, t3, t4;
    sm2_fp_neg(y2, q->y);

    if (sm2_jpoint_at_infinity(p)) {
        sm2_jpoint_set(r, x2, y2, sm2_one);
        return;
    }
    if (sm2_apoint_at_infinity(q)) {
        sm2_jpoint_copy(r, p);
        return;
    }

    sm2_fp_sqr(t1, z1);                     // t1 = A = z1^2
    sm2_fp_mul(t2, t1, z1);                 // t2 = B = z1 * A
    sm2_fp_mul(t1, t1, x2);                 // t1 = C = x2 * A
    sm2_fp_mul(t2, t2, y2);                 // t2 = D = y2 * B
    sm2_fp_sub(t1, t1, x1);                 // t1 = E = C - x1
    sm2_fp_sub(t2, t2, y1);                 // t2 = F = D - y1
    if (sm2_fp_equ(t1, sm2_zero)) {
        if (sm2_fp_equ(t2, sm2_zero)) {
            sm2_jpoint t;
            sm2_jpoint_set(&t, x2, y2, sm2_one);
            sm2_jpoint_dou(r, &t);
        } else {
            sm2_jpoint_init(r);
        }
        return;
    }
    sm2_fp_mul(z3, z1, t1);                 // z3 = z1 * E
    sm2_fp_sqr(t3, t1);                     // t3 = G = E^2
    sm2_fp_mul(t4, t3, t1);                 // t4 = H = E^3
    sm2_fp_mul(t3, t3, x1);                 // t3 = I = x1 * G
    sm2_fp_dbl(t1, t3);                     // t1 = 2I
    sm2_fp_sqr(x3, t2);                     // x3 = F^2
    sm2_fp_sub(x3, x3, t1);                 // x3 = F^2 - 2I
    sm2_fp_sub(x3, x3, t4);                 // x3 = F^2 - 2I - H
    sm2_fp_sub(t3, t3, x3);                 // t3 = I - x3
    sm2_fp_mul(t3, t3, t2);                 // t3 = (I - x3) * F
    sm2_fp_mul(t4, t4, y1);                 // t4 = y1 * H
    sm2_fp_sub(y3, t3, t4);                 // y3 = (I - x3) * F - y1 * H
    sm2_jpoint_set(r, x3, y3, z3);
}

void sm2_jpoint_dou(sm2_jpoint *r, const sm2_jpoint *a) {
    if (sm2_jpoint_at_infinity(a)) {
        sm2_jpoint_copy(r, a);
        return;
    }
    // A = 3(x1 - z1^2) * (x1 + z1^2)
    // B = 2Y1, z3 = B * z1, C = B^2
    // D = C * x1, x3 = A^2 - 2D, (D - x3) * A - C^2/2
    const u32 *x1 = a->x, *y1 = a->y, *z1 = a->z;
    sm2_fp t1, t2, t3, x3, y3, z3;
    sm2_fp_sqr(t1, z1);                     // t1 = z1^2
    sm2_fp_sub(t2, x1, t1);                 // t2 = x1 - z1^2
    sm2_fp_add(t1, x1, t1);                 // t1 = x1 + z1^2
    sm2_fp_mul(t2, t2, t1);                 // t2 = x1^2 - z1^4
    sm2_fp_dbl(t3, t2);                     // t3 = 2(x1^2 - z1^4)
    sm2_fp_add(t2, t2, t3);                 // t2 = A = 3t2 = 3(x1^2 - z1^4)
    sm2_fp_dbl(y3, y1);                     // y3 = B = 2y1
    sm2_fp_mul(z3, y3, z1);                 // z3 = B * z1
    sm2_fp_sqr(y3, y3);                     // y3 = C = B^2
    sm2_fp_mul(t3, y3, x1);                 // t3 = D = C * x1
    sm2_fp_sqr(y3, y3);                     // y3 = C^2
    sm2_fp_haf(y3, y3);                     // y3 = C^2/2
    sm2_fp_sqr(x3, t2);                     // x3 = A^2
    sm2_fp_dbl(t1, t3);                     // t1 = 2D
    sm2_fp_sub(x3, x3, t1);                 // x3 = A^2 - 2D
    sm2_fp_sub(t1, t3, x3);                 // t1 = D - x3
    sm2_fp_mul(t1, t1, t2);                 // t1 = (D - x3) * A
    sm2_fp_sub(y3, t1, y3);                 // y3 = (D - x3) * A - C^2/2
    sm2_jpoint_set(r, x3, y3, z3);
}

void sm2_jpoint_mdo(sm2_jpoint *r, u32 m, const sm2_jpoint *p) {
    if (sm2_jpoint_at_infinity(p)) {
        sm2_jpoint_set(r, p->x, p->y, p->z); return;
    }

    sm2_fp x, y, z, w, a, b, t;
    sm2_fp_set(x, p->x);
    sm2_fp_set(y, p->y);
    sm2_fp_set(z, p->z);
    sm2_fp_dbl(y, y);                   // y = 2y
    sm2_fp_sqr(w, z);
    sm2_fp_sqr(w, w);                   // w = z^4
    while(m--) {
        sm2_fp_sqr(a, x);
        sm2_fp_sub(a, a, w);
        sm2_fp_dbl(t, a);
        sm2_fp_add(a, a, t);            // a = 3(x^2 - w)
        sm2_fp_sqr(b, y);
        sm2_fp_mul(b, x, b);            // b = x * y^2
        sm2_fp_sqr(x, a);
        sm2_fp_sub(x, x, b);
        sm2_fp_sub(x, x, b);            // x = a^2 - 2b
        sm2_fp_mul(z, z, y);            // z = z * y
        sm2_fp_sqr(t, y);
        sm2_fp_sqr(t, t);               // t = y^4
        if (m)
            sm2_fp_mul(w, w, t);        // w = w * y^4
        sm2_fp_sub(y, b, x);
        sm2_fp_mul(y, y, a);
        sm2_fp_dbl(y, y);
        sm2_fp_sub(y, y, t);            // y = 2(b-x) - y^4
    }
    sm2_fp_haf(y, y);
    sm2_jpoint_set(r, x, y, z);
}

void sm2_jpoint_mul(sm2_jpoint *r, const sm2_fp k, const sm2_jpoint *g) {
    static sm2_naf K;
    static sm2_jpoint upt[8];

    // compute the sm2_naf of k
    sm2_fp_naf(K, 4, k);

    // compute the table of point g: {g, 3g, 5g, 7g, ...}
    sm2_jpoint_copy(&upt[0], g);
    sm2_jpoint_dou(r, g);
    for (u32 i = 1; i < 8; i++) {
        sm2_jpoint_add(&upt[i], &upt[i - 1], r);
    }

    // compute the result
    u32 i = 0, j = 1;
    sm2_jpoint_init(r);
    do {
        if (K[i] == 0) {
            j++;
        } else {
            sm2_jpoint_mdo(r, j, r);
            if (K[i] > 0)
                sm2_jpoint_add(r, r, &upt[K[i] >> 1]);
            else
                sm2_jpoint_sub(r, r, &upt[-K[i] >> 1]);
            j = 1;
        }
    } while (++i <= 256);
    sm2_jpoint_mdo(r, j - 1, r);
}

#if Large_lookup_table_allowed == 1
void sm2_jpoint_gen(sm2_jpoint *r, const sm2_fp k) {
    u8 K[32];
    memcpy(K, k, 32);
    sm2_apoint_to_jpoint(&sm2_pct[0][K[0]], r);
    for (int i = 1; i < 32; i++) {
        sm2_japoint_add(r, r, &sm2_pct[i][K[i]]);
    }
}
#else
void sm2_jpoint_gen(sm2_jpoint *r, const sm2_fp k) {
    static sm2_jpoint a, b;
    static i8 K[52];

    // set infinity jpoint(1, 1, 0)
    sm2_jpoint_init(&a);
    sm2_jpoint_init(&b);

    // compute the sm2_naf of k
    sm2_fp_nafp(K, k);

    // compute the result
    for (int j = 21; j > 0; j--) {
        for (int i = 0; i < 52; i++) {
            if (K[i] == j)
                sm2_japoint_add(&b, &b, &fpt[i]);
            if (K[i] == -j)
                sm2_japoint_sub(&b, &b, &fpt[i]);
        }
        sm2_jpoint_add(&a, &a, &b);
    }
    sm2_jpoint_copy(r, &a);
}
#endif




