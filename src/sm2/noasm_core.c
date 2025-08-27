#include "sm2_core.h"

#define ADD_ITERAT(u, r, a, b)      u  = a + u + b; r = u; u >>= 32;
#define SUB_ITERAT(u, r, a, b)      u  = a - u - b; r = u; u >>= 63;
#define MUL_ITERAT(u, r, a, b)      u += r + a * b; r = u; u >>= 32;

static void add(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    register u64 u = 0;
    ADD_ITERAT(u, r[0], a[0], b[0]);
    ADD_ITERAT(u, r[1], a[1], b[1])
    ADD_ITERAT(u, r[2], a[2], b[2])
    ADD_ITERAT(u, r[3], a[3], b[3])
    ADD_ITERAT(u, r[4], a[4], b[4])
    ADD_ITERAT(u, r[5], a[5], b[5])
    ADD_ITERAT(u, r[6], a[6], b[6])
    ADD_ITERAT(u, r[7], a[7], b[7])
}

static void sub(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    register u64 u = 0;
    SUB_ITERAT(u, r[0], a[0], b[0])
    SUB_ITERAT(u, r[1], a[1], b[1])
    SUB_ITERAT(u, r[2], a[2], b[2])
    SUB_ITERAT(u, r[3], a[3], b[3])
    SUB_ITERAT(u, r[4], a[4], b[4])
    SUB_ITERAT(u, r[5], a[5], b[5])
    SUB_ITERAT(u, r[6], a[6], b[6])
    SUB_ITERAT(u, r[7], a[7], b[7])
}

static void mul(sm2_dfp r, const sm2_fp a, const sm2_fp b) {
    register u64 u, v;
    for (int i = 0; i < 8; i++) {
        u = 0, v = a[i];
        MUL_ITERAT(u, r[i + 0], v, b[0])
        MUL_ITERAT(u, r[i + 1], v, b[1])
        MUL_ITERAT(u, r[i + 2], v, b[2])
        MUL_ITERAT(u, r[i + 3], v, b[3])
        MUL_ITERAT(u, r[i + 4], v, b[4])
        MUL_ITERAT(u, r[i + 5], v, b[5])
        MUL_ITERAT(u, r[i + 6], v, b[6])
        MUL_ITERAT(u, r[i + 7], v, b[7])
        r[i + 8] = u;
    }
}

static void sqr(sm2_dfp r, const sm2_fp a) {
    register u64 u;
    const register u64 a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3];
    const register u64 a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7];
    u = 0;
    MUL_ITERAT(u, r[0 + 0], a0, a0)
    MUL_ITERAT(u, r[0 + 1], a0, a1)
    MUL_ITERAT(u, r[0 + 2], a0, a2)
    MUL_ITERAT(u, r[0 + 3], a0, a3)
    MUL_ITERAT(u, r[0 + 4], a0, a4)
    MUL_ITERAT(u, r[0 + 5], a0, a5)
    MUL_ITERAT(u, r[0 + 6], a0, a6)
    MUL_ITERAT(u, r[0 + 7], a0, a7)
    r[0 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[1 + 0], a1, a0)
    MUL_ITERAT(u, r[1 + 1], a1, a1)
    MUL_ITERAT(u, r[1 + 2], a1, a2)
    MUL_ITERAT(u, r[1 + 3], a1, a3)
    MUL_ITERAT(u, r[1 + 4], a1, a4)
    MUL_ITERAT(u, r[1 + 5], a1, a5)
    MUL_ITERAT(u, r[1 + 6], a1, a6)
    MUL_ITERAT(u, r[1 + 7], a1, a7)
    r[1 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[2 + 0], a2, a0)
    MUL_ITERAT(u, r[2 + 1], a2, a1)
    MUL_ITERAT(u, r[2 + 2], a2, a2)
    MUL_ITERAT(u, r[2 + 3], a2, a3)
    MUL_ITERAT(u, r[2 + 4], a2, a4)
    MUL_ITERAT(u, r[2 + 5], a2, a5)
    MUL_ITERAT(u, r[2 + 6], a2, a6)
    MUL_ITERAT(u, r[2 + 7], a2, a7)
    r[2 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[3 + 0], a3, a0)
    MUL_ITERAT(u, r[3 + 1], a3, a1)
    MUL_ITERAT(u, r[3 + 2], a3, a2)
    MUL_ITERAT(u, r[3 + 3], a3, a3)
    MUL_ITERAT(u, r[3 + 4], a3, a4)
    MUL_ITERAT(u, r[3 + 5], a3, a5)
    MUL_ITERAT(u, r[3 + 6], a3, a6)
    MUL_ITERAT(u, r[3 + 7], a3, a7)
    r[3 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[4 + 0], a4, a0)
    MUL_ITERAT(u, r[4 + 1], a4, a1)
    MUL_ITERAT(u, r[4 + 2], a4, a2)
    MUL_ITERAT(u, r[4 + 3], a4, a3)
    MUL_ITERAT(u, r[4 + 4], a4, a4)
    MUL_ITERAT(u, r[4 + 5], a4, a5)
    MUL_ITERAT(u, r[4 + 6], a4, a6)
    MUL_ITERAT(u, r[4 + 7], a4, a7)
    r[4 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[5 + 0], a5, a0)
    MUL_ITERAT(u, r[5 + 1], a5, a1)
    MUL_ITERAT(u, r[5 + 2], a5, a2)
    MUL_ITERAT(u, r[5 + 3], a5, a3)
    MUL_ITERAT(u, r[5 + 4], a5, a4)
    MUL_ITERAT(u, r[5 + 5], a5, a5)
    MUL_ITERAT(u, r[5 + 6], a5, a6)
    MUL_ITERAT(u, r[5 + 7], a5, a7)
    r[5 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[6 + 0], a6, a0)
    MUL_ITERAT(u, r[6 + 1], a6, a1)
    MUL_ITERAT(u, r[6 + 2], a6, a2)
    MUL_ITERAT(u, r[6 + 3], a6, a3)
    MUL_ITERAT(u, r[6 + 4], a6, a4)
    MUL_ITERAT(u, r[6 + 5], a6, a5)
    MUL_ITERAT(u, r[6 + 6], a6, a6)
    MUL_ITERAT(u, r[6 + 7], a6, a7)
    r[6 + 8] = u;

    u = 0;
    MUL_ITERAT(u, r[7 + 0], a7, a0)
    MUL_ITERAT(u, r[7 + 1], a7, a1)
    MUL_ITERAT(u, r[7 + 2], a7, a2)
    MUL_ITERAT(u, r[7 + 3], a7, a3)
    MUL_ITERAT(u, r[7 + 4], a7, a4)
    MUL_ITERAT(u, r[7 + 5], a7, a5)
    MUL_ITERAT(u, r[7 + 6], a7, a6)
    MUL_ITERAT(u, r[7 + 7], a7, a7)
    r[7 + 8] = u;

}

static void haf(sm2_fp r, const sm2_fp a) {
    r[0] = a[0] >> 1 | a[1] << 31;
    r[1] = a[1] >> 1 | a[2] << 31;
    r[2] = a[2] >> 1 | a[3] << 31;
    r[3] = a[3] >> 1 | a[4] << 31;
    r[4] = a[4] >> 1 | a[5] << 31;
    r[5] = a[5] >> 1 | a[6] << 31;
    r[6] = a[6] >> 1 | a[7] << 31;
    r[7] = a[7] >> 1;
}

static void dbl(sm2_fp r, const sm2_fp a) {
    r[7] = a[7] << 1 | a[6] >> 31;
    r[6] = a[6] << 1 | a[5] >> 31;
    r[5] = a[5] << 1 | a[4] >> 31;
    r[4] = a[4] << 1 | a[3] >> 31;
    r[3] = a[3] << 1 | a[2] >> 31;
    r[2] = a[2] << 1 | a[1] >> 31;
    r[1] = a[1] << 1 | a[0] >> 31;
    r[0] = a[0] << 1;
}

static u32 adds(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    register u64 u = 0;
    ADD_ITERAT(u, r[0], a[0], b[0])
    ADD_ITERAT(u, r[1], a[1], b[1])
    ADD_ITERAT(u, r[2], a[2], b[2])
    ADD_ITERAT(u, r[3], a[3], b[3])
    ADD_ITERAT(u, r[4], a[4], b[4])
    ADD_ITERAT(u, r[5], a[5], b[5])
    ADD_ITERAT(u, r[6], a[6], b[6])
    ADD_ITERAT(u, r[7], a[7], b[7])
    return u;
}

static u32 subs(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    register u64 u = 0;
    SUB_ITERAT(u, r[0], a[0], b[0])
    SUB_ITERAT(u, r[1], a[1], b[1])
    SUB_ITERAT(u, r[2], a[2], b[2])
    SUB_ITERAT(u, r[3], a[3], b[3])
    SUB_ITERAT(u, r[4], a[4], b[4])
    SUB_ITERAT(u, r[5], a[5], b[5])
    SUB_ITERAT(u, r[6], a[6], b[6])
    SUB_ITERAT(u, r[7], a[7], b[7])
    return u;
}
int sm2_fp_cmp(const sm2_fp a, const sm2_fp b) {
    if (a[7] > b[7]) return 1;
    if (a[7] < b[7]) return 0;
    if (a[6] > b[6]) return 1;
    if (a[6] < b[6]) return 0;
    if (a[5] > b[5]) return 1;
    if (a[5] < b[5]) return 0;
    if (a[4] > b[4]) return 1;
    if (a[4] < b[4]) return 0;
    if (a[3] > b[3]) return 1;
    if (a[3] < b[3]) return 0;
    if (a[2] > b[2]) return 1;
    if (a[2] < b[2]) return 0;
    if (a[1] > b[1]) return 1;
    if (a[1] < b[1]) return 0;
    if (a[0] > b[0]) return 1;
    if (a[0] < b[0]) return 0;
    return 1;
}

void sm2_fp_add(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    if (adds(r, a, b)) {
        sub(r, r, sm2_p);
    }
}

void sm2_fp_sub(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    if (subs(r, a, b)) {
        add(r, r, sm2_p);
    }
}

void sm2_fp_neg(sm2_fp r, const sm2_fp a) {
    sub(r, sm2_p, a);
}

void sm2_fp_haf(sm2_fp r, const sm2_fp a) {
    if (sm2_fp_is_even(a)) {
        haf(r,  a);
    } else {
        haf(r, a);
        add(r, r, sm2_s);
    }
}

void sm2_fp_dbl(sm2_fp r, const sm2_fp a) {
    if (a[7] >> 31) {
        dbl(r, a);
        sub(r, r, sm2_p);
    }else {
        dbl(r, a);
    }
}

void sm2_fp_rec(sm2_fp r, const sm2_dfp c) {
    // the middle value
    u64 w0 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0;
    w0 = w0 + c[8] + c[9] + c[10] + c[11];
    w1 = w1 + c[8] + c[13];
    w2 = w2 + c[9] + c[14];
    w3 = w3 + c[14] + c[15];
    w4 = w4 + w3 + c[13];
    w5 = w5 + w4 + c[12];
    w0 = w0 + w5;

    // t = c mod sm2_p
    u64 t0, t1, t2, t3, t4, t5, t6, t7;
    t0 = c[0] + w0 + w4;
    t1 = c[1] + w0 + w4 - w1;
    t2 = c[2] - w1 - w2;
    t3 = c[3] + w5 + w1 + c[11];
    t4 = c[4] + w5 + w2;
    t5 = c[5] + w4 + c[10] + c[15];
    t6 = c[6] + w3 + c[11];
    t7 = c[7] + w0 + w5 + c[15];

    // clear the top 32 bits
    t1 += (int) (t0 >> 32);
    t0 &= 0xFFFFFFFF;
    t2 += (int) (t1 >> 32);
    t1 &= 0xFFFFFFFF;
    t3 += (int) (t2 >> 32);
    t2 &= 0xFFFFFFFF;
    t4 += (int) (t3 >> 32);
    t3 &= 0xFFFFFFFF;
    t5 += (int) (t4 >> 32);
    t4 &= 0xFFFFFFFF;
    t6 += (int) (t5 >> 32);
    t5 &= 0xFFFFFFFF;
    t7 += (int) (t6 >> 32);
    t6 &= 0xFFFFFFFF;

    // small reduction
    t0 += (int) (t7 >> 32);
    t2 -= (int) (t7 >> 32);
    t3 += (int) (t7 >> 32);
    t7 += (int) (t7 >> 32);
    t1 += (int) (t0 >> 32);
    t2 += (int) (t1 >> 32);
    t3 += (int) (t2 >> 32);
    t4 += (int) (t3 >> 32);
    t5 += (int) (t4 >> 32);
    t6 += (int) (t5 >> 32);
    t7 += (int) (t6 >> 32);

    // store
    r[0] = t0;
    r[1] = t1;
    r[2] = t2;
    r[3] = t3;
    r[4] = t4;
    r[5] = t5;
    r[6] = t6;
    r[7] = t7;
}

void sm2_fp_mul(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    sm2_dfp c = {0};
    mul(c, a, b);
    sm2_fp_rec(r, c);
}

void sm2_fp_sqr(sm2_fp r, const sm2_fp a) {
    sm2_dfp c = {0};
    sqr(c, a);
    sm2_fp_rec(r, c);
}

void sm2_fp_inv(sm2_fp r, const sm2_fp q) {
    sm2_fp u, v, a = {1}, c = {0};
    sm2_fp_set(u, q);
    sm2_fp_set(v, sm2_p);
    while (sm2_fp_non_zero(u)) {
        while (u[0]){
            while (sm2_fp_is_even(u)) {
                haf(u, u);
                sm2_fp_haf(a, a);
            }
            while (sm2_fp_is_even(v)) {
                haf(v, v);
                sm2_fp_haf(c, c);
            }
            if (sm2_fp_cmp(u, v)) {
                sub(u, u, v);
                if (subs(a, a, c))
                    add(a, a, sm2_p);
            } else {
                sub(v, v, u);
                if (subs(c, c, a))
                    add(c, c, sm2_p);
            }
        }
    }
    sm2_fp_set(r, c);
}
void sm2_fn_add(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    if (adds(r, a, b)) {
        sub(r, r, sm2_n);
    }
}

void sm2_fn_sub(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    if (subs(r, a, b)) {
        add(r, r, sm2_n);
    }
}

void sm2_fn_rec(sm2_fp r, const sm2_dfp a) {
    //nu = 2^512 / sm2_n = 0x1000000010000000100000001000000018DFC2096FA323C0112AC6361F15149A0, 存储时省略了第257位, 计算时补上
    static const sm2_fp u = {0xF15149A0U, 0x12AC6361U, 0xFA323C01U, 0x8DFC2096U, 0x00000001U, 0x00000001U, 0x00000001U, 0x00000001U};
    //b = (a * u) >> 256
    sm2_dfp b = {0}, c = {0};
    mul(b, a + 8, u);
    const u32 k = adds(b + 8, b + 8, a + 8);

    // c = b * n
    mul(c, b + 8, sm2_n);
    if (k)
        add(c + 8, c + 8, sm2_n);

    // r = a - c , 257位以上必然为0
    if (a[8] - c[8] - subs(r, a, c)){
        sub(r, r, sm2_n);
    }
}

void sm2_fn_mul(sm2_fp r, const sm2_fp a, const sm2_fp b) {
    sm2_dfp c = {0};
    mul(c, a, b);
    sm2_fn_rec(r, c);
}

void fn_haf(sm2_fp r, const sm2_fp a) {
    static const sm2_fp t = {0x9CEAA092U, 0xA9DDFA04U, 0x90E30295U, 0xB901EFB5U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x7FFFFFFFU, 0x7FFFFFFFU};
    if (a[0] & 1) {
        haf(r,  a);
        add(r, r, t);
    } else {
        haf(r, a);
    }
}

void sm2_fn_inv(sm2_fp r, const sm2_fp q) {
    sm2_fp u, v, a = {1}, c = {0};
    sm2_fp_set(u, q);
    sm2_fp_set(v, sm2_n);
    while (sm2_fp_non_zero(u)) {
        while (u[0]) {
            while (sm2_fp_is_even(u)) {
                haf(u, u);
                fn_haf(a, a);
            }
            while (sm2_fp_is_even(v)) {
                haf(v, v);
                fn_haf(c, c);
            }
            if (sm2_fp_cmp(u, v)) {
                sub(u, u, v);
                if (subs(a, a, c))
                    add(a, a, sm2_n);
            } else {
                sub(v, v, u);
                if (subs(c, c, a))
                    add(c, c, sm2_n);
            }
        }
    }
    sm2_fp_set(r, c);
}
