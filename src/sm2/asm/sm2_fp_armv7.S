.syntax unified
.thumb

// sm2_p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
.equ sm2_p1, 0xFFFFFFFF
.equ sm2_p2, 0xFFFFFFFF
.equ sm2_p3, 0x00000000
.equ sm2_p4, 0xFFFFFFFF
.equ sm2_p5, 0xFFFFFFFF
.equ sm2_p6, 0xFFFFFFFF
.equ sm2_p7, 0xFFFFFFFF
.equ sm2_p8, 0xFFFFFFFE
// sm2_q = - sm2_p = 0x00000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
.equ sm2_q1, 0x00000001
.equ sm2_q2, 0x00000000
.equ sm2_q3, 0xFFFFFFFF
.equ sm2_q4, 0x00000000
.equ sm2_q5, 0x00000000
.equ sm2_q6, 0x00000000
.equ sm2_q7, 0x00000000
.equ sm2_q8, 0x00000001
// sm2_s = (sm2_p + 1) / 2 = 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF800000007FFFFFFFFFFFFFFF
.equ sm2_s1, 0x00000000
.equ sm2_s2, 0x80000000
.equ sm2_s3, 0x80000000
.equ sm2_s4, 0xFFFFFFFF
.equ sm2_s5, 0xFFFFFFFF
.equ sm2_s6, 0xFFFFFFFF
.equ sm2_s7, 0x7FFFFFFF
.equ sm2_s8, 0x7FFFFFFF


# Return.
.macro RET
    MOV pc, lr
.endm

# In next, V refers specifically to the reglist {v1-v8} that stores 256-bit numbers
# v load n
.macro VLDR n1 n2 n3 n4 n5 n6 n7 n8
	LDR v1, =\n1
    LDR v2, =\n2
    LDR v3, =\n3
    LDR v4, =\n4
    LDR v5, =\n5
    LDR v6, =\n6
    LDR v7, =\n7
    LDR v8, =\n8
.endm

# v = v + n
.macro VADD n1 n2 n3 n4 n5 n6 n7 n8
	ADDS v1, \n1
	ADCS v2, \n2
	ADCS v3, \n3
	ADCS v4, \n4
	ADCS v5, \n5
	ADCS v6, \n6
	ADCS v7, \n7
	ADCS v8, \n8
.endm

# v = v - n
.macro VSUB n1 n2 n3 n4 n5 n6 n7 n8
	SUBS v1, \n1
	SBCS v2, \n2
	SBCS v3, \n3
	SBCS v4, \n4
	SBCS v5, \n5
	SBCS v6, \n6
	SBCS v7, \n7
	SBCS v8, \n8
.endm


# v = v - sm2_q
.macro VSUBQ
	VSUB sm2_q1, sm2_q2, sm2_q3, sm2_q4, sm2_q5, sm2_q6, sm2_q7, sm2_q8
.endm

# v = v + sm2_s
.macro VADDS
	VADD sm2_s1, sm2_s2, sm2_s3, sm2_s4, sm2_s5, sm2_s6, sm2_s7, sm2_s8
.endm

# v = v >> 1
.macro VLSR
    LSRS v8, v8, #1
    RRXS v7, v7
    RRXS v6, v6
    RRXS v5, v5
    RRXS v4, v4
    RRXS v3, v3
    RRXS v2, v2
    RRXS v1, v1
.endm

# reg1 - reg2, using in fp_inv
.macro SUBUV reg1 reg2
	LDM \reg1!, {v1-v4}
	LDM \reg2!, {v5-v8}
	SUBS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	PUSH {v1-v4}
	LDM \reg1, {v5-v8}
	LDM \reg2, {v1-v4}
	SBCS v5, v1
	SBCS v6, v2
	SBCS v7, v3
	SBCS v8, v4
	SUB \reg1, #0x10
	SUB \reg2, #0x10
	POP {v1-v4}
.endm


.section .text, "ax"
.thumb_func
.align 4

### Compare a and b. If a >= b, return 1; otherwise, return 0.
# int sm2_fp_cmp(const sm2_fp a, const sm2_fp b);
.global sm2_fp_cmp
.type   sm2_fp_cmp, %function
sm2_fp_cmp:
	LDR r2, [r0, #0x1C]
	LDR r3, [r1, #0x1C]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x18]
	LDR r3, [r1, #0x18]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x14]
	LDR r3, [r1, #0x14]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x10]
	LDR r3, [r1, #0x10]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x0C]
	LDR r3, [r1, #0x0C]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x08]
	LDR r3, [r1, #0x08]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x04]
	LDR r3, [r1, #0x04]
	CMP r2, r3
	BHI 1f
	BCC 0f
	LDR r2, [r0, #0x00]
	LDR r3, [r1, #0x00]
	CMP r2, r3
	BCC 0f
 1: MOV r0, 1
    RET
 0: MOV r0, 0
    RET

### Compute r ≡ a + b mod sm2_p.
# void sm2_fp_add(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fp_add
.type   sm2_fp_add, %function
sm2_fp_add:
	PUSH {v1-v8}
	LDM r2, {v1-v8}
	LDR r2, [r1, #0x00]
	LDR r3, [r1, #0x04]
	ADDS v1, r2
	ADCS v2, r3
	LDR r2, [r1, #0x08]
	LDR r3, [r1, #0x0C]
	ADCS v3, r2
	ADCS v4, r3
	LDR r2, [r1, #0x10]
	LDR r3, [r1, #0x14]
	ADCS v5, r2
	ADCS v6, r3
	LDR r2, [r1, #0x18]
	LDR r3, [r1, #0x1C]
	ADCS v7, r2
	ADCS v8, r3
	BCC 0f
	ADDS v1, #sm2_q1
	ADCS v2, #sm2_q2
	ADCS v3, #sm2_q3
	ADCS v4, #sm2_q4
	ADCS v5, #sm2_q5
	ADCS v6, #sm2_q6
	ADCS v7, #sm2_q7
	ADCS v8, #sm2_q8
 0: STM r0, {v1-v8}
	POP {v1-v8}
	RET

### Compute r ≡ a - b mod sm2_p.
# void sm2_fp_sub(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fp_sub
.type   sm2_fp_sub, %function
sm2_fp_sub:
	PUSH {v1-v8}
	LDM r1, {v1-v8}
	LDR r1, [r2, #0x00]
	LDR r3, [r2, #0x04]
	SUBS v1, r1
	SBCS v2, r3
	LDR r1, [r2, #0x08]
	LDR r3, [r2, #0x0C]
	SBCS v3, r1
	SBCS v4, r3
	LDR r1, [r2, #0x10]
	LDR r3, [r2, #0x14]
	SBCS v5, r1
	SBCS v6, r3
	LDR r1, [r2, #0x18]
	LDR r3, [r2, #0x1C]
	SBCS v7, r1
	SBCS v8, r3
	BCS 0f
	SUBS v1, #sm2_q1
	SBCS v2, #sm2_q2
	SBCS v3, #sm2_q3
	SBCS v4, #sm2_q4
	SBCS v5, #sm2_q5
	SBCS v6, #sm2_q6
	SBCS v7, #sm2_q7
	SBCS v8, #sm2_q8
 0: STM r0, {v1-v8}
	POP {v1-v8}
	RET

### Compute r ≡ -a mod sm2_p.
# void sm2_fp_neg(sm2_fp r, const sm2_fp a);
.global sm2_fp_neg
.type   sm2_fp_neg, %function
sm2_fp_neg:
	PUSH {r4-r6}
	LDM  r1!, {r2-r4}
	RSB  r2, #sm2_p1
	RSB  r3, #sm2_p2
	RSBS r4, #sm2_p3
	STM  r0!, {r2-r4}
	LDM  r1, {r2-r6}
	RSB  r2, #sm2_p4
	RSB  r3, #sm2_p5
	RSB  r4, #sm2_p6
	RSB  r5, #sm2_p7
	RSB  r6, #sm2_p7
	SBCS r2, #0
	SBCS r4, #0
	SBCS r5, #0
	SBCS r6, #1
	STM  r0, {r2-r6}
	POP  {r4-r6}
	RET

### Compute r ≡ 2a mod sm2_p.
# void sm2_fp_dbl(sm2_fp r, const sm2_fp a);
.global sm2_fp_dbl
.type   sm2_fp_dbl, %function
sm2_fp_dbl:
	PUSH {r4-r8}
	LDM r1, {r1-r8}
	ADDS r1, r1
	ADCS r2, r2
	ADCS r3, r3
	ADCS r4, r4
	ADCS r5, r5
	ADCS r6, r6
	ADCS r7, r7
	ADCS r8, r8
	BCC 0f
	ADDS r1, #sm2_q1
	ADCS r2, #sm2_q2
	ADCS r3, #sm2_q3
	ADCS r4, #sm2_q4
	ADCS r5, #sm2_q5
	ADCS r6, #sm2_q6
	ADCS r7, #sm2_q7
	ADCS r8, #sm2_q8
 0: STM  r0, {r1-r8}
	POP  {r4-r8}
	RET

### Compute r ≡ a/2 mod sm2_p.
# void sm2_fp_haf(sm2_fp r, const sm2_fp a);
.global sm2_fp_haf
.type   sm2_fp_haf, %function
sm2_fp_haf:
	PUSH {r4-r8}
	LDM r1, {r1-r8}
    LSRS r8, r8, #1
    RRXS r7, r7
    RRXS r6, r6
    RRXS r5, r5
    RRXS r4, r4
    RRXS r3, r3
    RRXS r2, r2
    RRXS r1, r1
	BCC 0f
	ADDS r1, #sm2_s1
	ADCS r2, #sm2_s2
	ADCS r3, #sm2_s3
	ADCS r4, #sm2_s4
	ADCS r5, #sm2_s5
	ADCS r6, #sm2_s6
	ADCS r7, #sm2_s7
	ADCS r8, #sm2_s8
 0: STM  r0, {r1-r8}
	POP  {r4-r8}
	RET

### Compute r ≡ a^(-1) mod sm2_p.
# void sm2_fp_inv(sm2_fp r, const sm2_fp q);
.global sm2_fp_inv
.type   sm2_fp_inv, %function
sm2_fp_inv:
	PUSH {v1-v8}
	PUSH {r0}
	# u = input, v = p, a = 1, c = 0
	LDM r1, {v1-v8}
	SUBS r0, sp, #0x90
	SUBS r1, sp, #0x70
	SUBS r2, sp, #0x50
	SUBS r3, sp, #0x30
	STM r0, {v1-v8}
    VLDR sm2_p1, sm2_p2, sm2_p3, sm2_p4, sm2_p5, sm2_p6, sm2_p7, sm2_p8
    STM r1, {v1-v8}
    VLDR 1, 0, 0, 0, 0, 0, 0, 0
    STM r2, {v1-v8}
    MOV v1, 0
    STM r3, {v1-v8}

	## while u >= 1 do inv_loop
 	.Linv_while:
	LDR v1, [r0]
	TEQ v1, #0
	BNE .Linv_loop
	LDM r0, {v1-v8}
	TEQ v2, #0
	BNE .Linv_loop
	TEQ v3, #0
	BNE .Linv_loop
	TEQ v4, #0
	BNE .Linv_loop
	TEQ v5, #0
	BNE .Linv_loop
	TEQ v6, #0
	BNE .Linv_loop
	TEQ v7, #0
	BNE .Linv_loop
	TEQ v8, #0
	BNE .Linv_loop
	B .Linv_end

	.Linv_loop:

	## while u is even, u = u >> 1, a = a / 2 mod p,
	.Linv_u_loop:
	LDR v1, [r0]
	TST v1, #1
	BNE .Linv_v_loop
	LDM r0, {v1-v8}
	VLSR
	STM r0, {v1-v8}
	LDM r2, {v1-v8}
	VLSR
	BCC 0f
	VADDS
 0: STM r2, {v1-v8}
	B .Linv_u_loop

	## while v is even, v = v >> 1, c = c / 2 mod p
	.Linv_v_loop:
	LDR v1, [r1]
	TST v1, #1
	BNE .Linv_update_uv
	LDM r1, {v1-v8}
	VLSR
	STM r1, {v1-v8}
	LDM r3, {v1-v8}
	VLSR
	BCC 1f
	VADDS
 1: STM r3, {v1-v8}
	B .Linv_v_loop

	## if u >= v, u = u - v, a = a - c mod p, else v = v - u, c = c - a mod p
	.Linv_update_uv:
	SUBUV r0, r1
	BCC .Lv_is_bigger

	.Lu_is_bigger:
	STM r0, {v1-v8}
	SUBUV r2, r3
	BCS 2f
	VSUBQ
 2:	STM r2, {v1-v8}
	B .Linv_while

	.Lv_is_bigger:
	MVN v1, v1
	MVN v2, v2
	MVN v3, v3
	MVN v4, v4
	MVN v5, v5
	MVN v6, v6
	MVN v7, v7
	MVN v8, v8
	ADDS v1, #1
	ADCS v2, #0
	ADCS v3, #0
	ADCS v4, #0
	ADCS v5, #0
	ADCS v6, #0
	ADCS v7, #0
	ADCS v8, #0
	STM r1, {v1-v8}
	SUBUV r3, r2
	BCS 3f
	VSUBQ
 3:	STM r3, {v1-v8}
	B .Linv_while

	.Linv_end:
	POP {r0}
	LDM r3, {v1-v8}
	STM r0, {v1-v8}
	POP {v1-v8}
	RET

### Compute r ≡ a * b mod sm2_p.
# void sm2_fp_mul(sm2_fp r, const sm2_fp a, const sm2_fp b);
.global sm2_fp_mul
.type   sm2_fp_mul, %function
sm2_fp_mul:
	PUSH {v1-v8, ip, lr}
	PUSH {r0}
    SUBS sp, #0x40
	LDM r1, {v1-v8}
	STMDB sp, {v1-v8}
	LDM r2, {v1-v8}
	# c = a * b
	# 1
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	MOV r0, #0
	MOV r1, #0
	MOV r2, #0
	MOV r3, #0
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	MOV r0, #0
	MOV r1, #0
	MOV r2, #0
	MOV r3, #0
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 2
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 3
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 4
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 5
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 6
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 7
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 8
	MOV lr, #0
	LDR ip, [sp, #-0x20]
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v1
	UMAAL r1, lr, ip, v2
	UMAAL r2, lr, ip, v3
	UMAAL r3, lr, ip, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, ip, v5
	UMAAL r1, lr, ip, v6
	UMAAL r2, lr, ip, v7
	UMAAL r3, lr, ip, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C

	## Fast modular reduction: r = c mod sm2p
	# c = c15 | c14 | ... | c0, where ci cre 32–bit quantities
	# | c7  | c6  | c5  | c4  | c3  | c2  | c1  | c0  | (+)
	# | c8  | c11 | c10 | c9  | c8  |   0 | c9  | c8  | (+)
	# | c9  | c14 | c13 | c12 | c11 |   0 | c10 | c9  | (+)
	# | c10 | c15 | c14 | c13 | c12 |   0 | c11 | c10 | (+)
	# | c11 |   0 | c15 | c14 | c13 |   0 | c12 | c11 | (+)
	# | c12 |   0 | c15 | c14 | c13 |   0 | c13 | c12 | (+)
	# | c12 |   0 |   0 | c15 | c14 |   0 | c14 | c13 | (+)
	# | c13 |   0 |   0 |   0 | c15 |   0 | c14 | c13 | (+)
	# | c13 |   0 |   0 |   0 |   0 |   0 | c15 | c14 | (+)
	# | c14 |   0 |   0 |   0 |   0 |   0 | c15 | c14 | (+)
	# | c14 |   0 |   0 |   0 |   0 |   0 |   0 | c15 | (+)
	# | c15 |   0 |   0 |   0 |   0 |   0 |   0 | c15 | (+)
	# | c15 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | (+)
	# | c15 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | (+)
	# |   0 |   0 |   0 |   0 |   0 | c8  |   0 |   0 | (-)
	# |   0 |   0 |   0 |   0 |   0 | c9  |   0 |   0 | (-)
	# |   0 |   0 |   0 |   0 |   0 | c13 |   0 |   0 | (-)
	# |   0 |   0 |   0 |   0 |   0 | c14 |   0 |   0 | (-)
	# —————————————————————————————————————————————————————
	#   r7  | r6  | r5  | r4  | r3  | r2  | r1  | r0
	# Compute the middle value in the fast rection at frist.
	# w0 = c[8] + c[9] + c[10] + c[11]
	# w1 = c[8] + c[13]
	# w2 = c[9] + c[14]
	# w3 = c[14] + c[15]
	# w4 = w3 + c[13]
	# w5 = w4 + c[12]
	# w0 = w0 + w5

	# Load c[8]-c[15] and set ip = 1 such that UMLAL ra, rb, rc, ip ==> (ra, rb) += rc.
	LDMIA.W sp, {v1-v8}
    MOV ip, #1
    MOV lr, #0

    # Compute w0-w5
	UMULL r0, r1, v1, ip		    // (r0, r1)  = c[8]
	UMLAL r0, r1, v2, ip		    // (r0, r1) += c[9]
	UMLAL r0, r1, v3, ip		    // (r0, r1) += c[10]
	UMLAL r0, r1, v4, ip		    // (r0, r1) += c[11]
    BFI lr, r1, #0, #4              // store w0 in (r0, lr[1-4])

	UMULL r1, r2, v1, ip		    // (r1, r2)  = c[8]
	UMLAL r1, r2, v6, ip		    // (r1, r2) += c[13]
	BFI lr, r2, #4, #4              // store w1 in (r1, lr[4-8])

	UMULL r2, r3, v2, ip		    // (r2, r3)  = c[9]
	UMLAL r2, r3, v7, ip		    // (r2, r3) += c[14]
	BFI lr, r3, #8, #4              // store w2 in (r2, lr[8-12])

	# (r4-r7) = (c[8-11]) is free now.
	UMULL r6, r7, v7, ip		    // (r6, r7)  = c[14]
	UMLAL r6, r7, v8, ip		    // (r6, r7) += c[15]
	BFI lr, r7, #12, #4             // store w3 in (r3, lr[12-16])
	MOV r3, r6

	UMLAL r6, r7, v6, ip		    // (r6, r7) += c[13]
	BFI lr, r7, #16, #4
	MOV r4, r6                      // store w4 in (r4, lr[16-20])

	UMLAL r6, r7, v5, ip		    // (r6, r7) += c[12]
	BFI lr, r7, #20, #4
	MOV r5, r6                      // store w5 in (r5, lr[20-24])

    UBFX v5, lr, #0, #4
    ADDS r6, r6, r0                 // (r6, r7) += w0
    ADCS r7, r7, v5
    BFI lr, r7, #0, #4
    MOV r0, r6
    MOV r7, lr                      // store w0 in (r0, lr[1-4])

    # Load c[0]-c[3] and set ip = 0, lr = 0 such that UMAAL ra, lr, rx, ip ==> (ra, lr) = ra + lr.
    SUBS sp, #0x20
    LDMIA.W sp, {v5-v8}
    MOV ip, #0
    MOV lr, #0

    # c[0] += w0 + w4
    UBFX r6, r7, #0, #4
	ADDS v5, r0
	ADCS lr, r6
    UBFX r6, r7, #16, #4
    ADDS v5, r4
    ADCS lr, r6

	# c[1] = w0 + w4 - w1
	UMAAL v6, lr, r0, ip
	ADDS v6, r4
	ADCS lr, r6
	UBFX r6, r7, #0, #4
	ADDS v6, r0
	ADCS lr, r6
	UBFX r6, r7, #4, #4
	SUBS v6, r1
	SBCS lr, r6

	# c[2] -= w1 + w2
	UMAAL v7, lr, r0, ip
	SUBS v7, r1
	SBCS lr, r6
	UBFX r6, r7, #8, #4
	SUBS v7, r2
	SBCS lr, r6

	# c[3] += w5 + w1 + c[11], after this, w1 is free now, load c[11] to r1
	ADDS v8, v8, lr
	MOV lr, #0
	UBFX r6, r7, #4, #4
	ADDS v8, r1
	ADCS lr, r6
	UBFX r6, r7, #20, #4
	ADDS v8, r5
	ADCS lr, r6
	LDR r1, [sp, #0x2C]
	ADDS v8, r1
	ADCS lr, #0

	# Stroe c[0]-c[3], Load c[4]-c[7]
	STMIA.W sp!, {v5-v8}
	LDMIA.W sp,  {v5-v8}

	# c[4] += w5 + w2
	UMAAL v5, lr, r0, ip
	ADDS v5, r5
    ADCS lr, r6
    UBFX r6, r7, #8, #4
	ADDS v5, r2
	ADCS lr, r6

	# c[5] += w4 + c[10] + c[15], after this, w2, w4 is free now, load c[10] to r2 and c[15] to r4
	UMAAL v6, lr, r0, ip
	UBFX r6, r7, #16, #4
	ADDS v6, r4
	ADCS lr, r6
	LDR r2, [sp, #0x18]
    LDR r4, [sp, #0x2C]
    ADDS v6, r2
    ADCS lr, #0
    ADDS v6, r4
    ADCS lr, #0

	# c[6] += w3 + c[11]
	UMAAL v7, lr, r0, ip
	UBFX r6, r7, #12, #4
	ADDS v7, r3
	ADCS lr, r6
	ADDS v7, r1
	ADCS lr, #0

	# c[7] += w0 + w5 + c[15]
	UMAAL v8, lr, r0, ip
	UBFX r6, r7, #0, #4
	ADDS v8, r0
	ADCS lr, r6
	UBFX r6, r7, #20, #4
	ADDS v8, r5
	ADCS lr, r6
    ADDS v8, r4
    ADCS lr, #0

	# small reduction
	LDMDB.W sp, {v1-v4}
	ADDS v1, lr
	ADCS v2, #0
	ADCS v3, #0
	ADCS v4, lr
	ADCS v5, #0
	ADCS v6, #0
	ADCS v7, #0
	ADCS v8, lr
	SUBS v3, lr
	SBCS v4, #0
	SBCS v5, #0
	SBCS v6, #0
	SBCS v7, #0
	SBCS v8, #0
	ADDS sp, #0x30
	POP {r0}
	STM r0, {v1-v8}
	POP {v1-v8, ip, lr}
	RET

### Compute r ≡ a^2 mod sm2_p.
# void sm2_fp_sqr(sm2_fp r, const sm2_fp a);
.global sm2_fp_sqr
.type   sm2_fp_sqr, %function
sm2_fp_sqr:
	PUSH {v1-v8, ip, lr}
	PUSH {r0}
    SUBS sp, #0x40
    LDM r1, {v1-v8}
    # tc = a * b
    # 1
	MOV lr, #0
	MOV r0, #0
	MOV r1, #0
	MOV r2, #0
	MOV r3, #0
	UMAAL r0, lr, v1, v1
	UMAAL r1, lr, v1, v2
	UMAAL r2, lr, v1, v3
	UMAAL r3, lr, v1, v4
    STMIA.W sp!, {r0-r3}
	MOV r0, #0
	MOV r1, #0
	MOV r2, #0
	MOV r3, #0
	UMAAL r0, lr, v1, v5
	UMAAL r1, lr, v1, v6
	UMAAL r2, lr, v1, v7
	UMAAL r3, lr, v1, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 2
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v2, v1
	UMAAL r1, lr, v2, v2
	UMAAL r2, lr, v2, v3
	UMAAL r3, lr, v2, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v2, v5
	UMAAL r1, lr, v2, v6
	UMAAL r2, lr, v2, v7
	UMAAL r3, lr, v2, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 3
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v3, v1
	UMAAL r1, lr, v3, v2
	UMAAL r2, lr, v3, v3
	UMAAL r3, lr, v3, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v3, v5
	UMAAL r1, lr, v3, v6
	UMAAL r2, lr, v3, v7
	UMAAL r3, lr, v3, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 4
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v4, v1
	UMAAL r1, lr, v4, v2
	UMAAL r2, lr, v4, v3
	UMAAL r3, lr, v4, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v4, v5
	UMAAL r1, lr, v4, v6
	UMAAL r2, lr, v4, v7
	UMAAL r3, lr, v4, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 5
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v5, v1
	UMAAL r1, lr, v5, v2
	UMAAL r2, lr, v5, v3
	UMAAL r3, lr, v5, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v5, v5
	UMAAL r1, lr, v5, v6
	UMAAL r2, lr, v5, v7
	UMAAL r3, lr, v5, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 6
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v6, v1
	UMAAL r1, lr, v6, v2
	UMAAL r2, lr, v6, v3
	UMAAL r3, lr, v6, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v6, v5
	UMAAL r1, lr, v6, v6
	UMAAL r2, lr, v6, v7
	UMAAL r3, lr, v6, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 7
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v7, v1
	UMAAL r1, lr, v7, v2
	UMAAL r2, lr, v7, v3
	UMAAL r3, lr, v7, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v7, v5
	UMAAL r1, lr, v7, v6
	UMAAL r2, lr, v7, v7
	UMAAL r3, lr, v7, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C
	# 8
	MOV lr, #0
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v8, v1
	UMAAL r1, lr, v8, v2
	UMAAL r2, lr, v8, v3
	UMAAL r3, lr, v8, v4
    STMIA.W sp!, {r0-r3}
	LDMIA.W sp, {r0-r3}
	UMAAL r0, lr, v8, v5
	UMAAL r1, lr, v8, v6
	UMAAL r2, lr, v8, v7
	UMAAL r3, lr, v8, v8
    STMIA.W sp, {r0-r3, lr}
	SUBS sp, #0x0C

	## Fast modular reduction: r = c mod sm2p
	# c = c15 | c14 | ... | c0, where ci cre 32–bit quantities
	# | c7  | c6  | c5  | c4  | c3  | c2  | c1  | c0  | (+)
	# | c8  | c11 | c10 | c9  | c8  |   0 | c9  | c8  | (+)
	# | c9  | c14 | c13 | c12 | c11 |   0 | c10 | c9  | (+)
	# | c10 | c15 | c14 | c13 | c12 |   0 | c11 | c10 | (+)
	# | c11 |   0 | c15 | c14 | c13 |   0 | c12 | c11 | (+)
	# | c12 |   0 | c15 | c14 | c13 |   0 | c13 | c12 | (+)
	# | c12 |   0 |   0 | c15 | c14 |   0 | c14 | c13 | (+)
	# | c13 |   0 |   0 |   0 | c15 |   0 | c14 | c13 | (+)
	# | c13 |   0 |   0 |   0 |   0 |   0 | c15 | c14 | (+)
	# | c14 |   0 |   0 |   0 |   0 |   0 | c15 | c14 | (+)
	# | c14 |   0 |   0 |   0 |   0 |   0 |   0 | c15 | (+)
	# | c15 |   0 |   0 |   0 |   0 |   0 |   0 | c15 | (+)
	# | c15 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | (+)
	# | c15 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | (+)
	# |   0 |   0 |   0 |   0 |   0 | c8  |   0 |   0 | (-)
	# |   0 |   0 |   0 |   0 |   0 | c9  |   0 |   0 | (-)
	# |   0 |   0 |   0 |   0 |   0 | c13 |   0 |   0 | (-)
	# |   0 |   0 |   0 |   0 |   0 | c14 |   0 |   0 | (-)
	# —————————————————————————————————————————————————————
	#   r7  | r6  | r5  | r4  | r3  | r2  | r1  | r0
	# Compute the middle value in the fast rection at frist.
	# w0 = c[8] + c[9] + c[10] + c[11]
	# w1 = c[8] + c[13]
	# w2 = c[9] + c[14]
	# w3 = c[14] + c[15]
	# w4 = w3 + c[13]
	# w5 = w4 + c[12]
	# w0 = w0 + w5

	# Load c[8]-c[15] and set ip = 1 such that UMLAL ra, rb, rc, ip ==> (ra, rb) += rc.
	LDMIA.W sp, {v1-v8}
    MOV ip, #1
    MOV lr, #0

    # Compute w0-w5
	UMULL r0, r1, v1, ip		    // (r0, r1)  = c[8]
	UMLAL r0, r1, v2, ip		    // (r0, r1) += c[9]
	UMLAL r0, r1, v3, ip		    // (r0, r1) += c[10]
	UMLAL r0, r1, v4, ip		    // (r0, r1) += c[11]
    BFI lr, r1, #0, #4              // store w0 in (r0, lr[1-4])

	UMULL r1, r2, v1, ip		    // (r1, r2)  = c[8]
	UMLAL r1, r2, v6, ip		    // (r1, r2) += c[13]
	BFI lr, r2, #4, #4              // store w1 in (r1, lr[4-8])

	UMULL r2, r3, v2, ip		    // (r2, r3)  = c[9]
	UMLAL r2, r3, v7, ip		    // (r2, r3) += c[14]
	BFI lr, r3, #8, #4              // store w2 in (r2, lr[8-12])

	# (r4-r7) = (c[8-11]) is free now.
	UMULL r6, r7, v7, ip		    // (r6, r7)  = c[14]
	UMLAL r6, r7, v8, ip		    // (r6, r7) += c[15]
	BFI lr, r7, #12, #4             // store w3 in (r3, lr[12-16])
	MOV r3, r6

	UMLAL r6, r7, v6, ip		    // (r6, r7) += c[13]
	BFI lr, r7, #16, #4
	MOV r4, r6                      // store w4 in (r4, lr[16-20])

	UMLAL r6, r7, v5, ip		    // (r6, r7) += c[12]
	BFI lr, r7, #20, #4
	MOV r5, r6                      // store w5 in (r5, lr[20-24])

    UBFX v5, lr, #0, #4
    ADDS r6, r6, r0                 // (r6, r7) += w0
    ADCS r7, r7, v5
    BFI lr, r7, #0, #4
    MOV r0, r6
    MOV r7, lr                      // store w0 in (r0, lr[1-4])

    # Load c[0]-c[3] and set ip = 0, lr = 0 such that UMAAL ra, lr, rx, ip ==> (ra, lr) = ra + lr.
    SUBS sp, #0x20
    LDMIA.W sp, {v5-v8}
    MOV ip, #0
    MOV lr, #0

    # c[0] += w0 + w4
    UBFX r6, r7, #0, #4
	ADDS v5, r0
	ADCS lr, r6
    UBFX r6, r7, #16, #4
    ADDS v5, r4
    ADCS lr, r6

	# c[1] = w0 + w4 - w1
	UMAAL v6, lr, r0, ip
	ADDS v6, r4
	ADCS lr, r6
	UBFX r6, r7, #0, #4
	ADDS v6, r0
	ADCS lr, r6
	UBFX r6, r7, #4, #4
	SUBS v6, r1
	SBCS lr, r6

	# c[2] -= w1 + w2
	UMAAL v7, lr, r0, ip
	SUBS v7, r1
	SBCS lr, r6
	UBFX r6, r7, #8, #4
	SUBS v7, r2
	SBCS lr, r6

	# c[3] += w5 + w1 + c[11], after this, w1 is free now, load c[11] to r1
	ADDS v8, v8, lr
	MOV lr, #0
	UBFX r6, r7, #4, #4
	ADDS v8, r1
	ADCS lr, r6
	UBFX r6, r7, #20, #4
	ADDS v8, r5
	ADCS lr, r6
	LDR r1, [sp, #0x2C]
	ADDS v8, r1
	ADCS lr, #0

	# Stroe c[0]-c[3], Load c[4]-c[7]
	STMIA.W sp!, {v5-v8}
	LDMIA.W sp,  {v5-v8}

	# c[4] += w5 + w2
	UMAAL v5, lr, r0, ip
	ADDS v5, r5
    ADCS lr, r6
    UBFX r6, r7, #8, #4
	ADDS v5, r2
	ADCS lr, r6

	# c[5] += w4 + c[10] + c[15], after this, w2, w4 is free now, load c[10] to r2 and c[15] to r4
	UMAAL v6, lr, r0, ip
	UBFX r6, r7, #16, #4
	ADDS v6, r4
	ADCS lr, r6
	LDR r2, [sp, #0x18]
    LDR r4, [sp, #0x2C]
    ADDS v6, r2
    ADCS lr, #0
    ADDS v6, r4
    ADCS lr, #0

	# c[6] += w3 + c[11]
	UMAAL v7, lr, r0, ip
	UBFX r6, r7, #12, #4
	ADDS v7, r3
	ADCS lr, r6
	ADDS v7, r1
	ADCS lr, #0

	# c[7] += w0 + w5 + c[15]
	UMAAL v8, lr, r0, ip
	UBFX r6, r7, #0, #4
	ADDS v8, r0
	ADCS lr, r6
	UBFX r6, r7, #20, #4
	ADDS v8, r5
	ADCS lr, r6
    ADDS v8, r4
    ADCS lr, #0

	# small reduction
	LDMDB.W sp, {v1-v4}
	ADDS v1, lr
	ADCS v2, #0
	ADCS v3, #0
	ADCS v4, lr
	ADCS v5, #0
	ADCS v6, #0
	ADCS v7, #0
	ADCS v8, lr
	SUBS v3, lr
	SBCS v4, #0
	SBCS v5, #0
	SBCS v6, #0
	SBCS v7, #0
	SBCS v8, #0
	ADDS sp, #0x30
	POP {r0}
	STM r0, {v1-v8}
	POP {v1-v8, ip, lr}
	RET
.end