.syntax unified
.thumb

# sm2_p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
# sm2_q = 0x00000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
# sm2_s = 0x7FFFFFFF7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF800000008000000000000000
# sm2_n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
# sm2_m = 0x000000010000000000000000000000008DFC2094DE39FAD4AC440BF6C62ABEDD
# sm2_l = 0x7FFFFFFF7FFFFFFFFFFFFFFFFFFFFFFFB901EFB590E30295A9DDFA049CEAA092
# Note: sm2_q = - sm2_p mod 2^256, sm2_s = (sm2_p + 1) / 2.
# Note: sm2_m = - sm2_n mod 2^256, sm2_l = (sm2_n + 1) / 2.
.equ sm2_p1, 0xFFFFFFFF
.equ sm2_p2, 0xFFFFFFFF
.equ sm2_p3, 0x00000000
.equ sm2_p4, 0xFFFFFFFF
.equ sm2_p5, 0xFFFFFFFF
.equ sm2_p6, 0xFFFFFFFF
.equ sm2_p7, 0xFFFFFFFF
.equ sm2_p8, 0xFFFFFFFE
.equ sm2_q1, 0x00000001
.equ sm2_q2, 0x00000000
.equ sm2_q3, 0xFFFFFFFF
.equ sm2_q4, 0x00000000
.equ sm2_q5, 0x00000000
.equ sm2_q6, 0x00000000
.equ sm2_q7, 0x00000000
.equ sm2_q8, 0x00000001
.equ sm2_s1, 0x00000000
.equ sm2_s2, 0x80000000
.equ sm2_s3, 0x80000000
.equ sm2_s4, 0xFFFFFFFF
.equ sm2_s5, 0xFFFFFFFF
.equ sm2_s6, 0xFFFFFFFF
.equ sm2_s7, 0x7FFFFFFF
.equ sm2_s8, 0x7FFFFFFF
.equ sm2_n1, 0x39D54123
.equ sm2_n2, 0x53BBF409
.equ sm2_n3, 0x21C6052B
.equ sm2_n4, 0x7203DF6B
.equ sm2_n5, 0xFFFFFFFF
.equ sm2_n6, 0xFFFFFFFF
.equ sm2_n7, 0xFFFFFFFF
.equ sm2_n8, 0xFFFFFFFE
.equ sm2_m1, 0xC62ABEDD
.equ sm2_m2, 0xAC440BF6
.equ sm2_m3, 0xDE39FAD4
.equ sm2_m4, 0x8DFC2094
.equ sm2_m5, 0x00000000
.equ sm2_m6, 0x00000000
.equ sm2_m7, 0x00000000
.equ sm2_m8, 0x00000001
.equ sm2_l1, 0x9CEAA092
.equ sm2_l2, 0xA9DDFA04
.equ sm2_l3, 0x90E30295
.equ sm2_l4, 0xB901EFB5
.equ sm2_l5, 0xFFFFFFFF
.equ sm2_l6, 0xFFFFFFFF
.equ sm2_l7, 0x7FFFFFFF
.equ sm2_l8, 0x7FFFFFFF

# Note: In next, V refers specifically to the reglist {v1-v8} that stores 256-bit numbers.
# Return.
.macro ret
    mov pc, lr
.endm

# v = n
.macro vldr n1 n2 n3 n4 n5 n6 n7 n8
    ldr v1, =\n1
    ldr v2, =\n2
    ldr v3, =\n3
    ldr v4, =\n4
    ldr v5, =\n5
    ldr v6, =\n6
    ldr v7, =\n7
    ldr v8, =\n8
.endm

# v = v + n
.macro vadd n1 n2 n3 n4 n5 n6 n7 n8
	adds v1, \n1
	adcs v2, \n2
	adcs v3, \n3
	adcs v4, \n4
	adcs v5, \n5
	adcs v6, \n6
	adcs v7, \n7
	adcs v8, \n8
.endm

# v = v - n
.macro vsub n1 n2 n3 n4 n5 n6 n7 n8
	subs v1, \n1
	sbcs v2, \n2
	sbcs v3, \n3
	sbcs v4, \n4
	sbcs v5, \n5
	sbcs v6, \n6
	sbcs v7, \n7
	sbcs v8, \n8
.endm

# v = v - sm2_q
.macro vsubq
	vsub sm2_q1, sm2_q2, sm2_q3, sm2_q4, sm2_q5, sm2_q6, sm2_q7, sm2_q8
.endm

# v = v + sm2_s
.macro vadds
	vadd sm2_s1, sm2_s2, sm2_s3, sm2_s4, sm2_s5, sm2_s6, sm2_s7, sm2_s8
.endm

# v = v - sm2_m, Where sm2_m1~sm2_m4 is not an immediate value and must be loaded into a register.
.macro vsubm reg1 reg2
	ldr \reg1, =sm2_m1
	ldr \reg2, =sm2_m2
	subs v1, \reg1
	sbcs v2, \reg2
	ldr \reg1, =sm2_m3
	ldr \reg2, =sm2_m4
	sbcs v3, \reg1
	sbcs v4, \reg2
	sbcs v5, #sm2_m5
	sbcs v6, #sm2_m6
	sbcs v7, #sm2_m7
	sbcs v8, #sm2_m8
.endm

# v = v - sm2_l, Where sm2_l1~sm2_l4 is not an immediate value and must be loaded into a register.
.macro vsubl reg1 reg2
	ldr \reg1, =sm2_l1
	ldr \reg2, =sm2_l2
	adds v1, \reg1
	adcs v2, \reg2
	ldr \reg1, =sm2_l3
	ldr \reg2, =sm2_l4
	adcs v3, \reg1
	adcs v4, \reg2
	adcs v5, #sm2_l5
	adcs v6, #sm2_l6
	adcs v7, #sm2_l7
	adcs v8, #sm2_l8
.endm

# v = v >> 1
.macro vlsr
    lsrs v8, #1
    rrxs v7, v7
    rrxs v6, v6
    rrxs v5, v5
    rrxs v4, v4
    rrxs v3, v3
    rrxs v2, v2
    rrxs v1, v1
.endm

# v = v >> 1 mod modulus
.macro vhaf modulus
    vlsr
    bcc 0f
    .ifc "\modulus", "sm2_p"
        vadds
    .endif
    .ifc "\modulus", "sm2_n"
        vsubl ip lr
    .endif
    0:
.endm

# v = v mod modulus
.macro vmod modulus
	bcs 0f
    .ifc "\modulus", "sm2_p"
        vsubq
    .endif
    .ifc "\modulus", "sm2_n"
        vsubm ip lr
    .endif
    0:
.endm

# v = - v
.macro vneg
	mvn v1, v1
	mvn v2, v2
	mvn v3, v3
	mvn v4, v4
	mvn v5, v5
	mvn v6, v6
	mvn v7, v7
	mvn v8, v8
	adds v1, #1
	adcs v2, #0
	adcs v3, #0
	adcs v4, #0
	adcs v5, #0
	adcs v6, #0
	adcs v7, #0
	adcs v8, #0
.endm

# The number pointed to by \reg1 minus the number pointed to by \reg2.
.macro psub reg1 reg2
	ldm \reg1!, {v1-v4}
	ldm \reg2!, {v5-v8}
	subs v1, v5
	sbcs v2, v6
	sbcs v3, v7
	sbcs v4, v8
	push {v1-v4}
	ldm \reg1, {v5-v8}
	ldm \reg2, {v1-v4}
	sbcs v5, v1
	sbcs v6, v2
	sbcs v7, v3
	sbcs v8, v4
	sub \reg1, #0x10
	sub \reg2, #0x10
	pop {v1-v4}
.endm

# Multiplication iteration macro, one can refer to functions(sm2_fp_mul/sqr) that do not use macro expansion.
.macro MULIT at, ap, cp, i

    mov lr, #0

    # Select the loading method based on the parameter type of at:
    # imm: load immediate values;
    # oft: load from stack using stack pointer(sp) + offset.
    .ifc \at, imm
    ldr ip, =\ap
    .else
    ldr ip, [sp, #\ap + 0x04 * \i]
    .endif

    # When i == 0, there is no need to load the multiplier from memory.
    .if \i == 0
        mov r0, #0
        mov r1, #0
        mov r2, #0
        mov r3, #0
    .else
        ldr r0, [sp, #\cp + 0x00 + 0x04 * \i]
        ldr r1, [sp, #\cp + 0x04 + 0x04 * \i]
        ldr r2, [sp, #\cp + 0x08 + 0x04 * \i]
        ldr r3, [sp, #\cp + 0x0C + 0x04 * \i]
    .endif

    umaal r0, lr, ip, v1
    umaal r1, lr, ip, v2
    umaal r2, lr, ip, v3
    umaal r3, lr, ip, v4

    str r0, [sp, #\cp + 0x00 + 0x04 * \i]
    str r1, [sp, #\cp + 0x04 + 0x04 * \i]
    str r2, [sp, #\cp + 0x08 + 0x04 * \i]
    str r3, [sp, #\cp + 0x0C + 0x04 * \i]

    # When i == 0, there is no need to load the multiplier from memory.
    .if \i == 0
        mov r0, #0
        mov r1, #0
        mov r2, #0
        mov r3, #0
    .else
        ldr r0, [sp, #\cp + 0x10 + 0x04 * \i]
        ldr r1, [sp, #\cp + 0x14 + 0x04 * \i]
        ldr r2, [sp, #\cp + 0x18 + 0x04 * \i]
        ldr r3, [sp, #\cp + 0x1C + 0x04 * \i]
    .endif

    umaal r0, lr, ip, v5
    umaal r1, lr, ip, v6
    umaal r2, lr, ip, v7
    umaal r3, lr, ip, v8

    str r0, [sp, #\cp + 0x10 + 0x04 * \i]
    str r1, [sp, #\cp + 0x14 + 0x04 * \i]
    str r2, [sp, #\cp + 0x18 + 0x04 * \i]
    str r3, [sp, #\cp + 0x1C + 0x04 * \i]
    str lr, [sp, #\cp + 0x20 + 0x04 * \i]
.endm

.section .text, "ax"
.thumb_func
.align 4

### Compare a and b. If a >= b, return 1; otherwise, return 0.
# int sm2_fp_cmp(const sm2_fp a, const sm2_fp b);
.global sm2_fp_cmp
.type   sm2_fp_cmp, %function
sm2_fp_cmp:
	ldr r2, [r0, #0x1C]
	ldr r3, [r1, #0x1C]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x18]
	ldr r3, [r1, #0x18]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x14]
	ldr r3, [r1, #0x14]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x10]
	ldr r3, [r1, #0x10]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x0C]
	ldr r3, [r1, #0x0C]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x08]
	ldr r3, [r1, #0x08]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x04]
	ldr r3, [r1, #0x04]
	cmp r2, r3
	bhi 1f
	bcc 0f
	ldr r2, [r0, #0x00]
	ldr r3, [r1, #0x00]
	cmp r2, r3
	bcc 0f
 1: mov r0, 1
    ret
 0: mov r0, 0
    ret

### Compute r ≡ a + b mod sm2_p.
# void sm2_fp_add(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fp_add
.type   sm2_fp_add, %function
sm2_fp_add:
	push {v1-v8}
	ldm  r2, {v1-v8}
	ldr  r2, [r1, #0x00]
	ldr  r3, [r1, #0x04]
	adds v1, r2
	adcs v2, r3
	ldr  r2, [r1, #0x08]
	ldr  r3, [r1, #0x0C]
	adcs v3, r2
	adcs v4, r3
	ldr  r2, [r1, #0x10]
	ldr  r3, [r1, #0x14]
	adcs v5, r2
	adcs v6, r3
	ldr  r2, [r1, #0x18]
	ldr  r3, [r1, #0x1C]
	adcs v7, r2
	adcs v8, r3
	bcc  0f
	adds v1, #sm2_q1
	adcs v2, #sm2_q2
	adcs v3, #sm2_q3
	adcs v4, #sm2_q4
	adcs v5, #sm2_q5
	adcs v6, #sm2_q6
	adcs v7, #sm2_q7
	adcs v8, #sm2_q8
 0: stm  r0, {v1-v8}
	pop  {v1-v8}
	ret

### Compute r ≡ a - b mod sm2_p.
# void sm2_fp_sub(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fp_sub
.type   sm2_fp_sub, %function
sm2_fp_sub:
	push {v1-v8}
	ldm  r1, {v1-v8}
	ldr  r1, [r2, #0x00]
	ldr  r3, [r2, #0x04]
	subs v1, r1
	sbcs v2, r3
	ldr  r1, [r2, #0x08]
	ldr  r3, [r2, #0x0C]
	sbcs v3, r1
	sbcs v4, r3
	ldr  r1, [r2, #0x10]
	ldr  r3, [r2, #0x14]
	sbcs v5, r1
	sbcs v6, r3
	ldr  r1, [r2, #0x18]
	ldr  r3, [r2, #0x1C]
	sbcs v7, r1
	sbcs v8, r3
	bcs  0f
	subs v1, #sm2_q1
	sbcs v2, #sm2_q2
	sbcs v3, #sm2_q3
	sbcs v4, #sm2_q4
	sbcs v5, #sm2_q5
	sbcs v6, #sm2_q6
	sbcs v7, #sm2_q7
	sbcs v8, #sm2_q8
 0: stm  r0, {v1-v8}
	pop  {v1-v8}
	ret

### Compute r ≡ -a mod sm2_p.
# void sm2_fp_neg(sm2_fp r, const sm2_fp a);
.global sm2_fp_neg
.type   sm2_fp_neg, %function
sm2_fp_neg:
	push {r4-r6}
	ldm  r1!, {r2-r4}
	rsb  r2, #sm2_p1
	rsb  r3, #sm2_p2
	rsbs r4, #sm2_p3
	stm  r0!, {r2-r4}
	ldm  r1, {r2-r6}
	rsb  r2, #sm2_p4
	rsb  r3, #sm2_p5
	rsb  r4, #sm2_p6
	rsb  r5, #sm2_p7
	rsb  r6, #sm2_p7
	sbcs r2, #0
	sbcs r4, #0
	sbcs r5, #0
	sbcs r6, #1
	stm  r0, {r2-r6}
	pop  {r4-r6}
	ret

### Compute r ≡ 2a mod sm2_p.
# void sm2_fp_dbl(sm2_fp r, const sm2_fp a);
.global sm2_fp_dbl
.type   sm2_fp_dbl, %function
sm2_fp_dbl:
	push {r4-r8}
	ldm  r1, {r1-r8}
	adds r1, r1
	adcs r2, r2
	adcs r3, r3
	adcs r4, r4
	adcs r5, r5
	adcs r6, r6
	adcs r7, r7
	adcs r8, r8
	bcc  0f
	adds r1, #sm2_q1
	adcs r2, #sm2_q2
	adcs r3, #sm2_q3
	adcs r4, #sm2_q4
	adcs r5, #sm2_q5
	adcs r6, #sm2_q6
	adcs r7, #sm2_q7
	adcs r8, #sm2_q8
 0: stm  r0, {r1-r8}
	pop  {r4-r8}
	ret

### Compute r ≡ a/2 mod sm2_p.
# void sm2_fp_haf(sm2_fp r, const sm2_fp a);
.global sm2_fp_haf
.type   sm2_fp_haf, %function
sm2_fp_haf:
	push {r4-r8}
	ldm  r1, {r1-r8}
    lsrs r8, #1
    rrxs r7, r7
    rrxs r6, r6
    rrxs r5, r5
    rrxs r4, r4
    rrxs r3, r3
    rrxs r2, r2
    rrxs r1, r1
	bcc  0f
	adds r1, #sm2_s1
	adcs r2, #sm2_s2
	adcs r3, #sm2_s3
	adcs r4, #sm2_s4
	adcs r5, #sm2_s5
	adcs r6, #sm2_s6
	adcs r7, #sm2_s7
	adcs r8, #sm2_s8
 0: stm  r0, {r1-r8}
	pop  {r4-r8}
	ret

### Compute r ≡ a * b mod sm2_p.
# void sm2_fp_mul(sm2_fp r, const sm2_fp a, const sm2_fp b);

## Step of computing r ≡ a * b mod sm2_p:
# 1. compute c = a * b.
# 2. compute t ≡ c mod sm2_p using the Fast modular reduction.
# 3. Compute r ≡ t mod sm2_p.

## Fast modular reduction:
    #      | c7 | c6 | c5 | c4 | c3 | c2 | c1 | c0 | (+)
    #      | c8 | cb | ca | c9 | c8 |  0 | c9 | c8 | (+)
    #      | c9 | ce | cd | cc | cb |  0 | ca | c9 | (+)
    #      | ca | cf | ce | cd | cc |  0 | cb | ca | (+)
    #      | cb |  0 | cf | ce | cd |  0 | cc | cb | (+)
    #      | cc |  0 | cf | ce | cd |  0 | cd | cc | (+)
    #      | cc |  0 |  0 | cf | ce |  0 | ce | cd | (+)
    #      | cd |  0 |  0 |  0 | cf |  0 | ce | cd | (+)
    #      | cd |  0 |  0 |  0 |  0 |  0 | cf | ce | (+)
    #      | ce |  0 |  0 |  0 |  0 |  0 | cf | ce | (+)
    #      | ce |  0 |  0 |  0 |  0 |  0 |  0 | cf | (+)
    #      | cf |  0 |  0 |  0 |  0 |  0 |  0 | cf | (+)
    #      | cf |  0 |  0 |  0 |  0 |  0 |  0 |  0 | (+)
    #      | cf |  0 |  0 |  0 |  0 |  0 |  0 |  0 | (+)
    #      |  0 |  0 |  0 |  0 |  0 | c8 |  0 |  0 | (-)
    #      |  0 |  0 |  0 |  0 |  0 | c9 |  0 |  0 | (-)
    #      |  0 |  0 |  0 |  0 |  0 | cd |  0 |  0 | (-)
    #      |  0 |  0 |  0 |  0 |  0 | ce |  0 |  0 | (-)
    # ———————————————————————————————————————————————————————
    # | t8 | t7 | t6 | t5 | t4 | t3 | t2 | t1 | t0 |

## Tips in step 2: one can calculate the repeating middle value first.
    # w0 = c8 + c9 + c10 + c11
    # w1 = c8 + c13
    # w2 = c9 + c14
    # w3 = c14 + c15
    # w4 = w3 + c13
    # w5 = w4 + c12
    # w0 = w0 + w5
    # T0 = c0 + w0 + w4
    # T1 = c1 + w0 + w4 - w1
    # T2 = c2 - w1 - w2
    # T3 = c3 + w5 + w1 + cb
    # T4 = c4 + w5 + w2
    # T5 = c5 + w4 + ca + cf
    # T6 = c6 + w3 + cb
    # T7 = c7 + w0 + w5 + cf
    # Obtain t0~t8 after processing the above carry in T0~T7.

## Tips in step 3: one can use the fast modular reduction algorithm again. At this point, c1,c2,c3,...cf = 0. Thus
    # r = t7 + t8 | t6 | t5 | t4 | t3 + t8 | t2 - t8 | t1 | t0 + t8
    #        ↑                        ↑         ↑              ↑
    # However, it is essential to ensure that the carry is handled.

## The 0x60 bytes of stack space allocated in the sm2_fp_mul function are stored as follows:
    # 0x00 : c0 c1 c2 c3 c4 c5 c6 c7
    # 0x20 : c8 c9 ca cb cc cd ce cf
    # 0x40 : a0 a1 a2 a3 a4 a5 a6 a7

.global sm2_fp_mul
.type   sm2_fp_mul, %function
sm2_fp_mul:
	push {v1-v8, ip, lr}
	push {r0}

	# Allocate stack space, load and store a, load b.
	sub sp, #0x20
	ldm r1, {v1-v8}
	stm sp, {v1-v8}
	ldm r2, {v1-v8}

	# Allocate stack space, compute and store c.
	sub sp, #0x40
	# 0
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x00]
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x00]
	str r1, [sp, #0x04 + 0x00]
	str r2, [sp, #0x08 + 0x00]
	str r3, [sp, #0x0C + 0x00]
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x00]
	str r1, [sp, #0x14 + 0x00]
	str r2, [sp, #0x18 + 0x00]
	str r3, [sp, #0x1C + 0x00]
	str lr, [sp, #0x20 + 0x00]
	# 1
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x04]
	ldr r0, [sp, #0x00 + 0x04]
	ldr r1, [sp, #0x04 + 0x04]
	ldr r2, [sp, #0x08 + 0x04]
	ldr r3, [sp, #0x0C + 0x04]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x04]
	str r1, [sp, #0x04 + 0x04]
	str r2, [sp, #0x08 + 0x04]
	str r3, [sp, #0x0C + 0x04]
	ldr r0, [sp, #0x10 + 0x04]
	ldr r1, [sp, #0x14 + 0x04]
	ldr r2, [sp, #0x18 + 0x04]
	ldr r3, [sp, #0x1C + 0x04]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x04]
	str r1, [sp, #0x14 + 0x04]
	str r2, [sp, #0x18 + 0x04]
	str r3, [sp, #0x1C + 0x04]
	str lr, [sp, #0x20 + 0x04]
	# 2
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x08]
	ldr r0, [sp, #0x00 + 0x08]
	ldr r1, [sp, #0x04 + 0x08]
	ldr r2, [sp, #0x08 + 0x08]
	ldr r3, [sp, #0x0C + 0x08]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x08]
	str r1, [sp, #0x04 + 0x08]
	str r2, [sp, #0x08 + 0x08]
	str r3, [sp, #0x0C + 0x08]
	ldr r0, [sp, #0x10 + 0x08]
	ldr r1, [sp, #0x14 + 0x08]
	ldr r2, [sp, #0x18 + 0x08]
	ldr r3, [sp, #0x1C + 0x08]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x08]
	str r1, [sp, #0x14 + 0x08]
	str r2, [sp, #0x18 + 0x08]
	str r3, [sp, #0x1C + 0x08]
	str lr, [sp, #0x20 + 0x08]
	# 3
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x0C]
	ldr r0, [sp, #0x00 + 0x0C]
	ldr r1, [sp, #0x04 + 0x0C]
	ldr r2, [sp, #0x08 + 0x0C]
	ldr r3, [sp, #0x0C + 0x0C]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x0C]
	str r1, [sp, #0x04 + 0x0C]
	str r2, [sp, #0x08 + 0x0C]
	str r3, [sp, #0x0C + 0x0C]
	ldr r0, [sp, #0x10 + 0x0C]
	ldr r1, [sp, #0x14 + 0x0C]
	ldr r2, [sp, #0x18 + 0x0C]
	ldr r3, [sp, #0x1C + 0x0C]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x0C]
	str r1, [sp, #0x14 + 0x0C]
	str r2, [sp, #0x18 + 0x0C]
	str r3, [sp, #0x1C + 0x0C]
	str lr, [sp, #0x20 + 0x0C]
	# 4
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x10]
	ldr r0, [sp, #0x00 + 0x10]
	ldr r1, [sp, #0x04 + 0x10]
	ldr r2, [sp, #0x08 + 0x10]
	ldr r3, [sp, #0x0C + 0x10]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x10]
	str r1, [sp, #0x04 + 0x10]
	str r2, [sp, #0x08 + 0x10]
	str r3, [sp, #0x0C + 0x10]
	ldr r0, [sp, #0x10 + 0x10]
	ldr r1, [sp, #0x14 + 0x10]
	ldr r2, [sp, #0x18 + 0x10]
	ldr r3, [sp, #0x1C + 0x10]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x10]
	str r1, [sp, #0x14 + 0x10]
	str r2, [sp, #0x18 + 0x10]
	str r3, [sp, #0x1C + 0x10]
	str lr, [sp, #0x20 + 0x10]
	# 5
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x14]
	ldr r0, [sp, #0x00 + 0x14]
	ldr r1, [sp, #0x04 + 0x14]
	ldr r2, [sp, #0x08 + 0x14]
	ldr r3, [sp, #0x0C + 0x14]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x14]
	str r1, [sp, #0x04 + 0x14]
	str r2, [sp, #0x08 + 0x14]
	str r3, [sp, #0x0C + 0x14]
	ldr r0, [sp, #0x10 + 0x14]
	ldr r1, [sp, #0x14 + 0x14]
	ldr r2, [sp, #0x18 + 0x14]
	ldr r3, [sp, #0x1C + 0x14]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x14]
	str r1, [sp, #0x14 + 0x14]
	str r2, [sp, #0x18 + 0x14]
	str r3, [sp, #0x1C + 0x14]
	str lr, [sp, #0x20 + 0x14]
	# 6
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x18]
	ldr r0, [sp, #0x00 + 0x18]
	ldr r1, [sp, #0x04 + 0x18]
	ldr r2, [sp, #0x08 + 0x18]
	ldr r3, [sp, #0x0C + 0x18]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x18]
	str r1, [sp, #0x04 + 0x18]
	str r2, [sp, #0x08 + 0x18]
	str r3, [sp, #0x0C + 0x18]
	ldr r0, [sp, #0x10 + 0x18]
	ldr r1, [sp, #0x14 + 0x18]
	ldr r2, [sp, #0x18 + 0x18]
	ldr r3, [sp, #0x1C + 0x18]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x18]
	str r1, [sp, #0x14 + 0x18]
	str r2, [sp, #0x18 + 0x18]
	str r3, [sp, #0x1C + 0x18]
	str lr, [sp, #0x20 + 0x18]
	# 7
	mov lr, #0
	ldr ip, [sp, #0x40 + 0x1C]
	ldr r0, [sp, #0x00 + 0x1C]
	ldr r1, [sp, #0x04 + 0x1C]
	ldr r2, [sp, #0x08 + 0x1C]
	ldr r3, [sp, #0x0C + 0x1C]
	umaal r0, lr, ip, v1
	umaal r1, lr, ip, v2
	umaal r2, lr, ip, v3
	umaal r3, lr, ip, v4
	str r0, [sp, #0x00 + 0x1C]
	str r1, [sp, #0x04 + 0x1C]
	str r2, [sp, #0x08 + 0x1C]
	str r3, [sp, #0x0C + 0x1C]
	ldr r0, [sp, #0x10 + 0x1C]
	ldr r1, [sp, #0x14 + 0x1C]
	ldr r2, [sp, #0x18 + 0x1C]
	ldr r3, [sp, #0x1C + 0x1C]
	umaal r0, lr, ip, v5
	umaal r1, lr, ip, v6
	umaal r2, lr, ip, v7
	umaal r3, lr, ip, v8
	str r0, [sp, #0x10 + 0x1C]
	str r1, [sp, #0x14 + 0x1C]
	str r2, [sp, #0x18 + 0x1C]
	str r3, [sp, #0x1C + 0x1C]
	str lr, [sp, #0x20 + 0x1C]

    # Load c8~cf and compute w0~w5.
	# Tips: set ip = 1 such that umlal ra, rb, rc, ip ==> (rb|ra) += rc.
	ldr v1, [sp, 0x20]
	ldr v2, [sp, 0x24]
	ldr v3, [sp, 0x28]
	ldr v4, [sp, 0x2C]
	ldr v5, [sp, 0x30]
	ldr v6, [sp, 0x34]
	ldr v7, [sp, 0x38]
	ldr v8, [sp, 0x3C]
    mov ip, #1
    mov lr, #0

    # Note, The 4 bits overflow values of w0, w1...w5 are stored in bits 0-3, 4-8...20-24 of register lr, respectively.
    # w0 = c8 + c9 + c10 + c11
	umull r0, r1, v1, ip
	umlal r0, r1, v2, ip
	umlal r0, r1, v3, ip
	umlal r0, r1, v4, ip
    bfi lr, r1, #0, #4

    # w1 = c8 + c13
	umull r1, r2, v1, ip
	umlal r1, r2, v6, ip
	bfi lr, r2, #4, #4

    # w2 = c9 + c14
	umull r2, r3, v2, ip
	umlal r2, r3, v7, ip
	bfi lr, r3, #8, #4

    # w3 = c14 + c15, register r4,r5,r6,r7 is free now.
	umull r6, r7, v7, ip
	umlal r6, r7, v8, ip
	bfi lr, r7, #12, #4
	mov r3, r6

    # w4 = w3 + c13
	umlal r6, r7, v6, ip
	bfi lr, r7, #16, #4
	mov r4, r6

    # w5 = w4 + c12
	umlal r6, r7, v5, ip
	bfi lr, r7, #20, #4
	mov r5, r6

    # w0 = w0 + w5
    ubfx v5, lr, #0, #4
    adds r6, r6, r0
    adcs r7, r7, v5
    bfi lr, r7, #0, #4
    mov r0, r6
    mov r7, lr

    # Load c0,c1,c2,c3 and compute t0,t1,t2,t3 where lr is used as the carry register.
    # Tips: set ip = 0 such that umaal ra, lr, rx, ip ==> (lr|ra) = ra + lr.
    ldm sp, {v5-v8}
    mov ip, #0
    mov lr, #0

    # t0 = c0 + w0 + w4
    ubfx r6, r7, #0, #4
	adds v5, r0
	adcs lr, r6
    ubfx r6, r7, #16, #4
    adds v5, r4
    adcs lr, r6

    # t1 = c1 + w0 + w4 - w1
	umaal v6, lr, r0, ip
	adds v6, r4
	adcs lr, r6
	ubfx r6, r7, #0, #4
	adds v6, r0
	adcs lr, r6
	ubfx r6, r7, #4, #4
	subs v6, r1
	sbcs lr, r6

    # t2 = c2 - w1 - w2
	umaal v7, lr, r0, ip
	subs v7, r1
	sbcs lr, r6
	ubfx r6, r7, #8, #4
	subs v7, r2
	sbcs lr, r6

    # t3 = c3 + w5 + w1 + cb, register r1 is free now, load cb to r1
	adds v8, v8, lr
	mov lr, #0
	ubfx r6, r7, #4, #4
	adds v8, r1
	adcs lr, r6
	ubfx r6, r7, #20, #4
	adds v8, r5
	adcs lr, r6
	ldr r1, [sp, #0x2C]
	adds v8, r1
	adcs lr, #0

	# Stroe t0,t1,t2,t3, load c4,c5,c6,c7, compute t4,t5,t6,t7,t8
	stm sp, {v5-v8}
	ldr v5, [sp, #0x10]
	ldr v6, [sp, #0x14]
	ldr v7, [sp, #0x18]
	ldr v8, [sp, #0x1C]

    # t4 = c4 + w5 + w2
	umaal v5, lr, r0, ip
	adds v5, r5
    adcs lr, r6
    ubfx r6, r7, #8, #4
	adds v5, r2
	adcs lr, r6

    # t5 = c5 + w4 + ca + cf, register r2 and r4 is free now, load ca to r2 and cf to r4
	umaal v6, lr, r0, ip
	ubfx r6, r7, #16, #4
	adds v6, r4
	adcs lr, r6
	ldr r2, [sp, #0x28]
    ldr r4, [sp, #0x3C]
    adds v6, r2
    adcs lr, #0
    adds v6, r4
    adcs lr, #0

    # t6 = c6 + w3 + cb
	umaal v7, lr, r0, ip
	ubfx r6, r7, #12, #4
	adds v7, r3
	adcs lr, r6
	adds v7, r1
	adcs lr, #0

    # t7 = c7 + w0 + w5 + cf, t8 = lr
	umaal v8, lr, r0, ip
	ubfx r6, r7, #0, #4
	adds v8, r0
	adcs lr, r6
	ubfx r6, r7, #20, #4
	adds v8, r5
	adcs lr, r6
    adds v8, r4
    adcs lr, #0

	# compute r = t7 + t8 | t6 | t5 | t4 | t3 + t8 | t2 - t8 | t1 | t0 + t8
    ldm sp, {v1-v4}
	adds v1, lr
	adcs v2, #0
	adcs v3, #0
	adcs v4, lr
	adcs v5, #0
	adcs v6, #0
	adcs v7, #0
	adcs v8, lr
	subs v3, lr
	sbcs v4, #0
	sbcs v5, #0
	sbcs v6, #0
	sbcs v7, #0
	sbcs v8, #0
	add sp, #0x60
	pop {r0}
	stm r0, {v1-v8}
	pop {v1-v8, ip, lr}
	ret

### Compute r ≡ a² mod sm2_p.
# void sm2_fp_sqr(sm2_fp r, const sm2_fp a);
.global sm2_fp_sqr
.type   sm2_fp_sqr, %function
sm2_fp_sqr:
	push {v1-v8, ip, lr}
	push {r0}

	# Allocate stack space, load a, compute c = a * a
    ldm r1, {v1-v8}
    sub sp, #0x40

	# 0
	mov lr, #0
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	umaal r0, lr, v1, v1
	umaal r1, lr, v1, v2
	umaal r2, lr, v1, v3
	umaal r3, lr, v1, v4
	str r0, [sp, #0x00 + 0x00]
	str r1, [sp, #0x04 + 0x00]
	str r2, [sp, #0x08 + 0x00]
	str r3, [sp, #0x0C + 0x00]
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	umaal r0, lr, v1, v5
	umaal r1, lr, v1, v6
	umaal r2, lr, v1, v7
	umaal r3, lr, v1, v8
	str r0, [sp, #0x10 + 0x00]
	str r1, [sp, #0x14 + 0x00]
	str r2, [sp, #0x18 + 0x00]
	str r3, [sp, #0x1C + 0x00]
	str lr, [sp, #0x20 + 0x00]
	# 1
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x04]
	ldr r1, [sp, #0x04 + 0x04]
	ldr r2, [sp, #0x08 + 0x04]
	ldr r3, [sp, #0x0C + 0x04]
	umaal r0, lr, v2, v1
	umaal r1, lr, v2, v2
	umaal r2, lr, v2, v3
	umaal r3, lr, v2, v4
	str r0, [sp, #0x00 + 0x04]
	str r1, [sp, #0x04 + 0x04]
	str r2, [sp, #0x08 + 0x04]
	str r3, [sp, #0x0C + 0x04]
	ldr r0, [sp, #0x10 + 0x04]
	ldr r1, [sp, #0x14 + 0x04]
	ldr r2, [sp, #0x18 + 0x04]
	ldr r3, [sp, #0x1C + 0x04]
	umaal r0, lr, v2, v5
	umaal r1, lr, v2, v6
	umaal r2, lr, v2, v7
	umaal r3, lr, v2, v8
	str r0, [sp, #0x10 + 0x04]
	str r1, [sp, #0x14 + 0x04]
	str r2, [sp, #0x18 + 0x04]
	str r3, [sp, #0x1C + 0x04]
	str lr, [sp, #0x20 + 0x04]
	# 2
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x08]
	ldr r1, [sp, #0x04 + 0x08]
	ldr r2, [sp, #0x08 + 0x08]
	ldr r3, [sp, #0x0C + 0x08]
	umaal r0, lr, v3, v1
	umaal r1, lr, v3, v2
	umaal r2, lr, v3, v3
	umaal r3, lr, v3, v4
	str r0, [sp, #0x00 + 0x08]
	str r1, [sp, #0x04 + 0x08]
	str r2, [sp, #0x08 + 0x08]
	str r3, [sp, #0x0C + 0x08]
	ldr r0, [sp, #0x10 + 0x08]
	ldr r1, [sp, #0x14 + 0x08]
	ldr r2, [sp, #0x18 + 0x08]
	ldr r3, [sp, #0x1C + 0x08]
	umaal r0, lr, v3, v5
	umaal r1, lr, v3, v6
	umaal r2, lr, v3, v7
	umaal r3, lr, v3, v8
	str r0, [sp, #0x10 + 0x08]
	str r1, [sp, #0x14 + 0x08]
	str r2, [sp, #0x18 + 0x08]
	str r3, [sp, #0x1C + 0x08]
	str lr, [sp, #0x20 + 0x08]
	# 3
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x0C]
	ldr r1, [sp, #0x04 + 0x0C]
	ldr r2, [sp, #0x08 + 0x0C]
	ldr r3, [sp, #0x0C + 0x0C]
	umaal r0, lr, v4, v1
	umaal r1, lr, v4, v2
	umaal r2, lr, v4, v3
	umaal r3, lr, v4, v4
	str r0, [sp, #0x00 + 0x0C]
	str r1, [sp, #0x04 + 0x0C]
	str r2, [sp, #0x08 + 0x0C]
	str r3, [sp, #0x0C + 0x0C]
	ldr r0, [sp, #0x10 + 0x0C]
	ldr r1, [sp, #0x14 + 0x0C]
	ldr r2, [sp, #0x18 + 0x0C]
	ldr r3, [sp, #0x1C + 0x0C]
	umaal r0, lr, v4, v5
	umaal r1, lr, v4, v6
	umaal r2, lr, v4, v7
	umaal r3, lr, v4, v8
	str r0, [sp, #0x10 + 0x0C]
	str r1, [sp, #0x14 + 0x0C]
	str r2, [sp, #0x18 + 0x0C]
	str r3, [sp, #0x1C + 0x0C]
	str lr, [sp, #0x20 + 0x0C]
	# 4
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x10]
	ldr r1, [sp, #0x04 + 0x10]
	ldr r2, [sp, #0x08 + 0x10]
	ldr r3, [sp, #0x0C + 0x10]
	umaal r0, lr, v5, v1
	umaal r1, lr, v5, v2
	umaal r2, lr, v5, v3
	umaal r3, lr, v5, v4
	str r0, [sp, #0x00 + 0x10]
	str r1, [sp, #0x04 + 0x10]
	str r2, [sp, #0x08 + 0x10]
	str r3, [sp, #0x0C + 0x10]
	ldr r0, [sp, #0x10 + 0x10]
	ldr r1, [sp, #0x14 + 0x10]
	ldr r2, [sp, #0x18 + 0x10]
	ldr r3, [sp, #0x1C + 0x10]
	umaal r0, lr, v5, v5
	umaal r1, lr, v5, v6
	umaal r2, lr, v5, v7
	umaal r3, lr, v5, v8
	str r0, [sp, #0x10 + 0x10]
	str r1, [sp, #0x14 + 0x10]
	str r2, [sp, #0x18 + 0x10]
	str r3, [sp, #0x1C + 0x10]
	str lr, [sp, #0x20 + 0x10]
	# 5
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x14]
	ldr r1, [sp, #0x04 + 0x14]
	ldr r2, [sp, #0x08 + 0x14]
	ldr r3, [sp, #0x0C + 0x14]
	umaal r0, lr, v6, v1
	umaal r1, lr, v6, v2
	umaal r2, lr, v6, v3
	umaal r3, lr, v6, v4
	str r0, [sp, #0x00 + 0x14]
	str r1, [sp, #0x04 + 0x14]
	str r2, [sp, #0x08 + 0x14]
	str r3, [sp, #0x0C + 0x14]
	ldr r0, [sp, #0x10 + 0x14]
	ldr r1, [sp, #0x14 + 0x14]
	ldr r2, [sp, #0x18 + 0x14]
	ldr r3, [sp, #0x1C + 0x14]
	umaal r0, lr, v6, v5
	umaal r1, lr, v6, v6
	umaal r2, lr, v6, v7
	umaal r3, lr, v6, v8
	str r0, [sp, #0x10 + 0x14]
	str r1, [sp, #0x14 + 0x14]
	str r2, [sp, #0x18 + 0x14]
	str r3, [sp, #0x1C + 0x14]
	str lr, [sp, #0x20 + 0x14]
	# 6
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x18]
	ldr r1, [sp, #0x04 + 0x18]
	ldr r2, [sp, #0x08 + 0x18]
	ldr r3, [sp, #0x0C + 0x18]
	umaal r0, lr, v7, v1
	umaal r1, lr, v7, v2
	umaal r2, lr, v7, v3
	umaal r3, lr, v7, v4
	str r0, [sp, #0x00 + 0x18]
	str r1, [sp, #0x04 + 0x18]
	str r2, [sp, #0x08 + 0x18]
	str r3, [sp, #0x0C + 0x18]
	ldr r0, [sp, #0x10 + 0x18]
	ldr r1, [sp, #0x14 + 0x18]
	ldr r2, [sp, #0x18 + 0x18]
	ldr r3, [sp, #0x1C + 0x18]
	umaal r0, lr, v7, v5
	umaal r1, lr, v7, v6
	umaal r2, lr, v7, v7
	umaal r3, lr, v7, v8
	str r0, [sp, #0x10 + 0x18]
	str r1, [sp, #0x14 + 0x18]
	str r2, [sp, #0x18 + 0x18]
	str r3, [sp, #0x1C + 0x18]
	str lr, [sp, #0x20 + 0x18]
	# 7
	mov lr, #0
	ldr r0, [sp, #0x00 + 0x1C]
	ldr r1, [sp, #0x04 + 0x1C]
	ldr r2, [sp, #0x08 + 0x1C]
	ldr r3, [sp, #0x0C + 0x1C]
	umaal r0, lr, v8, v1
	umaal r1, lr, v8, v2
	umaal r2, lr, v8, v3
	umaal r3, lr, v8, v4
	str r0, [sp, #0x00 + 0x1C]
	str r1, [sp, #0x04 + 0x1C]
	str r2, [sp, #0x08 + 0x1C]
	str r3, [sp, #0x0C + 0x1C]
	ldr r0, [sp, #0x10 + 0x1C]
	ldr r1, [sp, #0x14 + 0x1C]
	ldr r2, [sp, #0x18 + 0x1C]
	ldr r3, [sp, #0x1C + 0x1C]
	umaal r0, lr, v8, v5
	umaal r1, lr, v8, v6
	umaal r2, lr, v8, v7
	umaal r3, lr, v8, v8
	str r0, [sp, #0x10 + 0x1C]
	str r1, [sp, #0x14 + 0x1C]
	str r2, [sp, #0x18 + 0x1C]
	str r3, [sp, #0x1C + 0x1C]
	str lr, [sp, #0x20 + 0x1C]

    # Load c8~cf and compute w0~w5.
	# Tips: set ip = 1 such that umlal ra, rb, rc, ip ==> (rb|ra) += rc.
	ldr v1, [sp, 0x20]
	ldr v2, [sp, 0x24]
	ldr v3, [sp, 0x28]
	ldr v4, [sp, 0x2C]
	ldr v5, [sp, 0x30]
	ldr v6, [sp, 0x34]
	ldr v7, [sp, 0x38]
	ldr v8, [sp, 0x3C]
    mov ip, #1
    mov lr, #0

    # Note, The 4 bits overflow values of w0, w1...w5 are stored in bits 0-3, 4-8...20-24 of register lr, respectively.
    # w0 = c8 + c9 + c10 + c11
	umull r0, r1, v1, ip
	umlal r0, r1, v2, ip
	umlal r0, r1, v3, ip
	umlal r0, r1, v4, ip
    bfi lr, r1, #0, #4

    # w1 = c8 + c13
	umull r1, r2, v1, ip
	umlal r1, r2, v6, ip
	bfi lr, r2, #4, #4

    # w2 = c9 + c14
	umull r2, r3, v2, ip
	umlal r2, r3, v7, ip
	bfi lr, r3, #8, #4

    # w3 = c14 + c15, register r4,r5,r6,r7 is free now.
	umull r6, r7, v7, ip
	umlal r6, r7, v8, ip
	bfi lr, r7, #12, #4
	mov r3, r6

    # w4 = w3 + c13
	umlal r6, r7, v6, ip
	bfi lr, r7, #16, #4
	mov r4, r6

    # w5 = w4 + c12
	umlal r6, r7, v5, ip
	bfi lr, r7, #20, #4
	mov r5, r6

    # w0 = w0 + w5
    ubfx v5, lr, #0, #4
    adds r6, r6, r0
    adcs r7, r7, v5
    bfi lr, r7, #0, #4
    mov r0, r6
    mov r7, lr

    # Load c0,c1,c2,c3 and compute t0,t1,t2,t3 where lr is used as the carry register.
    # Tips: set ip = 0 such that umaal ra, lr, rx, ip ==> (lr|ra) = ra + lr.
    ldm sp, {v5-v8}
    mov ip, #0
    mov lr, #0

    # t0 = c0 + w0 + w4
    ubfx r6, r7, #0, #4
	adds v5, r0
	adcs lr, r6
    ubfx r6, r7, #16, #4
    adds v5, r4
    adcs lr, r6

    # t1 = c1 + w0 + w4 - w1
	umaal v6, lr, r0, ip
	adds v6, r4
	adcs lr, r6
	ubfx r6, r7, #0, #4
	adds v6, r0
	adcs lr, r6
	ubfx r6, r7, #4, #4
	subs v6, r1
	sbcs lr, r6

    # t2 = c2 - w1 - w2
	umaal v7, lr, r0, ip
	subs v7, r1
	sbcs lr, r6
	ubfx r6, r7, #8, #4
	subs v7, r2
	sbcs lr, r6

    # t3 = c3 + w5 + w1 + cb, register r1 is free now, load cb to r1
	adds v8, v8, lr
	mov lr, #0
	ubfx r6, r7, #4, #4
	adds v8, r1
	adcs lr, r6
	ubfx r6, r7, #20, #4
	adds v8, r5
	adcs lr, r6
	ldr r1, [sp, #0x2C]
	adds v8, r1
	adcs lr, #0

	# Stroe t0,t1,t2,t3, load c4,c5,c6,c7, compute t4,t5,t6,t7,t8
	stm sp, {v5-v8}
	ldr v5, [sp, #0x10]
	ldr v6, [sp, #0x14]
	ldr v7, [sp, #0x18]
	ldr v8, [sp, #0x1C]

    # t4 = c4 + w5 + w2
	umaal v5, lr, r0, ip
	adds v5, r5
    adcs lr, r6
    ubfx r6, r7, #8, #4
	adds v5, r2
	adcs lr, r6

    # t5 = c5 + w4 + ca + cf, register r2 and r4 is free now, load ca to r2 and cf to r4
	umaal v6, lr, r0, ip
	ubfx r6, r7, #16, #4
	adds v6, r4
	adcs lr, r6
	ldr r2, [sp, #0x28]
    ldr r4, [sp, #0x3C]
    adds v6, r2
    adcs lr, #0
    adds v6, r4
    adcs lr, #0

    # t6 = c6 + w3 + cb
	umaal v7, lr, r0, ip
	ubfx r6, r7, #12, #4
	adds v7, r3
	adcs lr, r6
	adds v7, r1
	adcs lr, #0

    # t7 = c7 + w0 + w5 + cf, t8 = lr
	umaal v8, lr, r0, ip
	ubfx r6, r7, #0, #4
	adds v8, r0
	adcs lr, r6
	ubfx r6, r7, #20, #4
	adds v8, r5
	adcs lr, r6
    adds v8, r4
    adcs lr, #0

	# compute r = t7 + t8 | t6 | t5 | t4 | t3 + t8 | t2 - t8 | t1 | t0 + t8
    ldm sp, {v1-v4}
	adds v1, lr
	adcs v2, #0
	adcs v3, #0
	adcs v4, lr
	adcs v5, #0
	adcs v6, #0
	adcs v7, #0
	adcs v8, lr
	subs v3, lr
	sbcs v4, #0
	sbcs v5, #0
	sbcs v6, #0
	sbcs v7, #0
	sbcs v8, #0
	add sp, #0x40
	pop {r0}
	stm r0, {v1-v8}
	pop {v1-v8, ip, lr}
	ret

### Compute r ≡ a⁻¹ mod sm2_p.
# void sm2_fp_inv(sm2_fp r, const sm2_fp q);
# Note: Binary algorithm for inversion.
#   u = a, v = p, a = 1, c = 0
#   While(u > 0)
#       While(u is even)
#           u = u >> 1, a = a / 2 mod p
#       While(v is even)
#           v = v >> 1, c = c / 2 mod p
#       If (u >= v)
#           u = u - v, a = a - c mod p
#       Else
#           v = v - u, c = c - a mod p
#   return c
.global sm2_fp_inv
.type   sm2_fp_inv, %function
sm2_fp_inv:
	push {v1-v8, ip, lr}
	push {r0}
	ldm r1, {v1-v8}
	sub sp, sp, #0x80
	add r0, sp, #0x00
	add r1, sp, #0x20
	add r2, sp, #0x40
	add r3, sp, #0x60
	stm r0, {v1-v8}
    vldr sm2_p1, sm2_p2, sm2_p3, sm2_p4, sm2_p5, sm2_p6, sm2_p7, sm2_p8
    stm r1, {v1-v8}
    vldr 1, 0, 0, 0, 0, 0, 0, 0
    stm r2, {v1-v8}
    mov v1, 0
    stm r3, {v1-v8}

	## while u >= 1 do inv_loop
 	.Lfp_inv_while:
	ldr v1, [r0]
	teq v1, #0
	bne .Lfp_inv_loop
	ldm r0, {v1-v8}
	teq v2, #0
	bne .Lfp_inv_loop
	teq v3, #0
	bne .Lfp_inv_loop
	teq v4, #0
	bne .Lfp_inv_loop
	teq v5, #0
	bne .Lfp_inv_loop
	teq v6, #0
	bne .Lfp_inv_loop
	teq v7, #0
	bne .Lfp_inv_loop
	teq v8, #0
	bne .Lfp_inv_loop
	b .Lfp_inv_end

	.Lfp_inv_loop:
	## while u is even, u = u >> 1, a = a / 2 mod p,
	.Lfp_inv_u_loop:
	ldr v1, [r0]
	tst v1, #1
	bne .Lfp_inv_v_loop
	ldm r0, {v1-v8}
	vlsr
	stm r0, {v1-v8}
	ldm r2, {v1-v8}
    vhaf sm2_p
    stm r2, {v1-v8}
	b .Lfp_inv_u_loop

	## while v is even, v = v >> 1, c = c / 2 mod p
	.Lfp_inv_v_loop:
	ldr v1, [r1]
	tst v1, #1
	bne .Lfp_inv_update_uv
	ldm r1, {v1-v8}
	vlsr
	stm r1, {v1-v8}
	ldm r3, {v1-v8}
    vhaf sm2_p
    stm r3, {v1-v8}
	b .Lfp_inv_v_loop

	## if u >= v, u = u - v, a = a - c mod p, else v = v - u, c = c - a mod p
	.Lfp_inv_update_uv:
	psub r0, r1
	bcc .Lfp_v_is_bigger

	.Lfp_u_is_bigger:
	stm r0, {v1-v8}
	psub r2, r3
    vmod sm2_p
    stm r2, {v1-v8}
	b .Lfp_inv_while

	.Lfp_v_is_bigger:
    vneg
	stm r1, {v1-v8}
	psub r3, r2
    vmod sm2_p
    stm r3, {v1-v8}
	b .Lfp_inv_while

	.Lfp_inv_end:
	add sp, sp, #0x80
	pop {r0}
	ldm r3, {v1-v8}
	stm r0, {v1-v8}
	pop {v1-v8, ip, lr}
	ret

### Compute r ≡ a + b mod sm2_n.
# void sm2_fn_add(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fn_add
.type   sm2_fn_add, %function
sm2_fn_add:
	push {v1-v8}
	ldm  r2, {v1-v8}
	ldr  r2, [r1, #0x00]
	ldr  r3, [r1, #0x04]
	adds v1, r2
	adcs v2, r3
	ldr  r2, [r1, #0x08]
	ldr  r3, [r1, #0x0C]
	adcs v3, r2
	adcs v4, r3
	ldr  r2, [r1, #0x10]
	ldr  r3, [r1, #0x14]
	adcs v5, r2
	adcs v6, r3
	ldr  r2, [r1, #0x18]
	ldr  r3, [r1, #0x1C]
	adcs v7, r2
	adcs v8, r3
	bcc  0f
	ldr  r2, =sm2_m1
	ldr  r3, =sm2_m2
	adds v1, r2
	adcs v2, r3
	ldr  r2, =sm2_m3
	ldr  r3, =sm2_m4
	adcs v3, r2
	adcs v4, r3
	adcs v5, #sm2_m5
	adcs v6, #sm2_m6
	adcs v7, #sm2_m7
	adcs v8, #sm2_m8
 0: stm  r0, {v1-v8}
	pop  {v1-v8}
	ret

### Compute r ≡ a - b mod sm2_n.
# void sm2_fn_sub(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fn_sub
.type   sm2_fn_sub, %function
sm2_fn_sub:
	push {v1-v8}
	ldm r1, {v1-v8}
	ldr r1, [r2, #0x00]
	ldr r3, [r2, #0x04]
	subs v1, r1
	sbcs v2, r3
	ldr r1, [r2, #0x08]
	ldr r3, [r2, #0x0C]
	sbcs v3, r1
	sbcs v4, r3
	ldr r1, [r2, #0x10]
	ldr r3, [r2, #0x14]
	sbcs v5, r1
	sbcs v6, r3
	ldr r1, [r2, #0x18]
	ldr r3, [r2, #0x1C]
	sbcs v7, r1
	sbcs v8, r3
	bcs 0f
	ldr r2, =sm2_m1
	ldr r3, =sm2_m2
	subs v1, r2
	sbcs v2, r3
	ldr r2, =sm2_m3
	ldr r3, =sm2_m4
	sbcs v3, r2
	sbcs v4, r3
	sbcs v5, #sm2_m5
	sbcs v6, #sm2_m6
	sbcs v7, #sm2_m7
	sbcs v8, #sm2_m8
 0: stm r0, {v1-v8}
	pop {v1-v8}
	ret

### Compute r ≡ a * b mod sm2_n.
# void sm2_fn_mul(sm2_fp r, const sm2_fp a, const sm2_fp b);

## Step of computing r ≡ a * b mod sm2_n by using Barrett Reduction:
# 1. compute c = a * b.
# 2. compute d = (c >> 256) * u, where u = 2^512 / sm2_n = 0x1000000010000000100000001000000018DFC2096FA323C0112AC6361F15149A0.
# 3. compute e = (d >> 256) * sm2_n.
# 4. compute r = c - e, one only needs to calculate the lower 32*9 bits because bits above 288 are necessarily 0.
# 5. compute r = r mod sm2_n.
## Note that since u is 257 bits, the storage of d and e uses 32 * 17 = 544 bits.

## The 0xE8 bytes of stack space allocated in the sm2_fn_mul function are stored as follows:
    # 0x00 : a0 a1 a2 a3 a4 a5 a6 a7
    # 0x20 : c0 c1 c2 c3 c4 c5 c6 c7
    # 0x40 : c8 c9 ca cb cc cd ce cf
    # 0x60 : d0 d1 d2 d3 d4 d5 d6 d7
    # 0x80 : d8 d9 da db dc dd de df dg
    # 0xA4 : e0 e1 e2 e3 e4 e5 e6 e7
    # 0xC4 : e8 e9 ea eb ec ee ee ef eg
.global sm2_fn_mul
.type   sm2_fn_mul, %function
sm2_fn_mul:
	push {v1-v8, ip, lr}
	push {r0}

	# Allocate stack space, load and store a, .
    sub sp, #0xE8
	ldm r1, {v1-v8}
	stm sp, {v1-v8}

	# load b in V and compute c = a * b.
	ldm r2, {v1-v8}
    mulit oft 0x0 0x20 0
    mulit oft 0x0 0x20 1
    mulit oft 0x0 0x20 2
    mulit oft 0x0 0x20 3
    mulit oft 0x0 0x20 4
    mulit oft 0x0 0x20 5
    mulit oft 0x0 0x20 6
    mulit oft 0x0 0x20 7

    # load c >> 256 in V and compute d = (c >> 256) * u
    adds ip, sp, #0x40
    ldm ip, {v1-v8}
    mulit imm 0xF15149A0 0x60 0
    mulit imm 0x12AC6361 0x60 1
    mulit imm 0xFA323C01 0x60 2
    mulit imm 0x8DFC2096 0x60 3
    mulit imm 0x00000001 0x60 4
    mulit imm 0x00000001 0x60 5
    mulit imm 0x00000001 0x60 6
    mulit imm 0x00000001 0x60 7
    mulit imm 0x00000001 0x60 8

    # load sm2_n in V and compute e = (d >> 256) * sm2_n.
    vldr sm2_n1, sm2_n2, sm2_n3, sm2_n4, sm2_n5, sm2_n6, sm2_n7, sm2_n8
    mulit oft 0x80 0xA4 0
    mulit oft 0x80 0xA4 1
    mulit oft 0x80 0xA4 2
    mulit oft 0x80 0xA4 3
    mulit oft 0x80 0xA4 4
    mulit oft 0x80 0xA4 5
    mulit oft 0x80 0xA4 6
    mulit oft 0x80 0xA4 7
    mulit oft 0x80 0xA4 8

	# compute r = c - e
	adds r0, sp, #0x20
	adds r1, sp, #0xA4
 	ldm r0!, {v1-v4}
 	ldm r1!, {v5-v8}
 	subs v1, v5
 	sbcs v2, v6
 	sbcs v3, v7
 	sbcs v4, v8
 	ldm r0, {v5-v8, ip}
 	ldm r1, {r0-r3, lr}
 	sbcs v5, r0
 	sbcs v6, r1
 	sbcs v7, r2
 	sbcs v8, r3
 	sbcs ip, lr

 	# compute r = r mod sm2_n: while(r >= sm2_n){ r = r - sm2_n}
 	ldr r0, =sm2_n1
    ldr r1, =sm2_n2
    ldr r2, =sm2_n3
    ldr r3, =sm2_n4
 0: teq ip, #0
    BEQ 1f
    subs v1, r0
    sbcs v2, r1
    sbcs v3, r2
    sbcs v4, r3
	sbcs v5, sm2_n5
	sbcs v6, sm2_n6
	sbcs v7, sm2_n7
	sbcs v8, sm2_n8
	sbcs ip, #0
    teq ip, #0
    bne 0b
 1: cmp v8, #sm2_n8
	bcc 3f
 	bhi 2f
 	cmp v7, #sm2_n7
	bcc 3f
 	bhi 2f
	cmp v6, #sm2_n6
	bcc 3f
 	bhi 2f
	cmp v5, #sm2_n5
	bcc 3f
 	bhi 2f
	cmp v4, r3
	bcc 3f
 	bhi 2f
	cmp v3, r2
	bcc 3f
 	bhi 2f
	cmp v2, r1
	bcc 3f
 	bhi 2f
	cmp v1, r0
	bcc 3f
 2: subs v1, r0
    sbcs v2, r1
    sbcs v3, r2
    sbcs v4, r3
    sbcs v5, #sm2_n5
    sbcs v6, #sm2_n6
    sbcs v7, #sm2_n7
    sbcs v8, #sm2_n8
 3: adds sp, #0xE8
    pop {r0}
    stm r0, {v1-v8}
    pop {v1-v8, ip, lr}
    ret

### Compute r ≡ a^(-1) mod sm2_n.
# void sm2_fn_inv(sm2_fp r, const sm2_fp q);
.global sm2_fn_inv
.type   sm2_fn_inv, %function
sm2_fn_inv:
	push {v1-v8, ip, lr}
	push {r0}
	# u = input, v = n, a = 1, c = 0
	ldm r1, {v1-v8}
	subs sp, sp, #0x80
	adds r0, sp, #0x00
	adds r1, sp, #0x20
	adds r2, sp, #0x40
	adds r3, sp, #0x60
	stm r0, {v1-v8}
    vldr sm2_n1, sm2_n2, sm2_n3, sm2_n4, sm2_n5, sm2_n6, sm2_n7, sm2_n8
    stm r1, {v1-v8}
    vldr 1, 0, 0, 0, 0, 0, 0, 0
    stm r2, {v1-v8}
    mov v1, 0
    stm r3, {v1-v8}

	## while u >= 1 do inv_loop
 	.Lfn_inv_while:
	ldr v1, [r0]
	teq v1, #0
	bne .Lfn_inv_loop
	ldm r0, {v1-v8}
	teq v2, #0
	bne .Lfn_inv_loop
	teq v3, #0
	bne .Lfn_inv_loop
	teq v4, #0
	bne .Lfn_inv_loop
	teq v5, #0
	bne .Lfn_inv_loop
	teq v6, #0
	bne .Lfn_inv_loop
	teq v7, #0
	bne .Lfn_inv_loop
	teq v8, #0
	bne .Lfn_inv_loop
	b .Lfn_inv_end

	.Lfn_inv_loop:

	## while u is even, u = u >> 1, a = a / 2 mod n

	.Lfn_inv_u_loop:
	ldr v1, [r0]
	tst v1, #1
	bne .Lfn_inv_v_loop
	ldm r0, {v1-v8}
	vlsr
	stm r0, {v1-v8}
	ldm r2, {v1-v8}
	vhaf sm2_n
    stm r2, {v1-v8}
	b .Lfn_inv_u_loop

	## while v is even, v = v >> 1, c = c / 2 mod n
	.Lfn_inv_v_loop:
	ldr v1, [r1]
	tst v1, #1
	bne .Lfn_inv_update_uv
	ldm r1, {v1-v8}
	vlsr
	stm r1, {v1-v8}
	ldm r3, {v1-v8}
    vhaf sm2_n
    stm r3, {v1-v8}
	b .Lfn_inv_v_loop

	## if u >= v, u = u - v, a = a - c mod n, else v = v - u, c = c - a mod n
	.Lfn_inv_update_uv:
	psub r0, r1
	bcc .Lfn_v_is_bigger

	.Lfn_u_is_bigger:
	stm r0, {v1-v8}
	psub r2, r3
    vmod sm2_n
    stm r2, {v1-v8}
	b .Lfn_inv_while

	.Lfn_v_is_bigger:
    vneg
	stm r1, {v1-v8}
	psub r3, r2
    vmod sm2_n
    stm r3, {v1-v8}
	b .Lfn_inv_while

	.Lfn_inv_end:
	adds sp, sp, #0x80
	pop {r0}
	ldm r3, {v1-v8}
	stm r0, {v1-v8}
	pop {v1-v8, ip, lr}
	ret
.end