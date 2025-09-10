.syntax unified
.arch armv7
.thumb

# keyschdul round function
.macro KRF k0 k1 k2 k3 ck
	LDR r0, =\ck
	EOR r0, \k1
	EOR r0, \k2
	EOR r0, \k3
	UBFX r1, r0, #8,  #8
	UBFX r2, r0, #16, #8
	UBFX r3, r0, #24, #8
	UBFX r0, r0, #0,  #8
	LDR r0, [v5, r0, LSL #2]
	LDR r1, [v6, r1, LSL #2]
	LDR r2, [v7, r2, LSL #2]
	LDR r3, [v8, r3, LSL #2]
	EOR \k0, r0
	EOR \k0, r1
	EOR \k0, r2
	EOR \k0, r3
	STR \k0, [ip], #4
.endm

# encrypt round function
.macro ERF p0 p1 p2 p3 
	LDR r0, [ip], #4
	EOR r0, \p1
	EOR r0, \p2
	EOR r0, \p3
	UBFX r1, r0, #8,  #8
	UBFX r2, r0, #16, #8
	UBFX r3, r0, #24, #8
	UBFX r0, r0, #0,  #8
	LDR r0, [v5, r0, LSL #2]
	LDR r1, [v6, r1, LSL #2]
	LDR r2, [v7, r2, LSL #2]
	LDR r3, [v8, r3, LSL #2]
	EOR \p0, r0
	EOR \p0, r1
	EOR \p0, r2
	EOR \p0, r3
.endm

# decrypt round function
.macro DRF p0 p1 p2 p3 
	LDR r0, [ip], #-4
	EOR r0, \p1
	EOR r0, \p2
	EOR r0, \p3
	UBFX r1, r0, #8,  #8
	UBFX r2, r0, #16, #8
	UBFX r3, r0, #24, #8
	UBFX r0, r0, #0,  #8
	LDR r0, [v5, r0, LSL #2]
	LDR r1, [v6, r1, LSL #2]
	LDR r2, [v7, r2, LSL #2]
	LDR r3, [v8, r3, LSL #2]
	EOR \p0, r0
	EOR \p0, r1
	EOR \p0, r2
	EOR \p0, r3
.endm

.section .text, "ax"
.thumb_func
.align 4

# void sm4_setkey_core(u32 *rk, const u8 *key);
.global sm4_setkey_core
.type   sm4_setkey_core, %function
sm4_setkey_core:
	PUSH {v1-v8, ip}
    MOV ip, r0
	LDM r1, {v1-v4}
	LDR r0, = 0xa3b1bac6
	LDR r1, = 0x56aa3350
	LDR r2, = 0x677d9197
	LDR r3, = 0xb27022dc
	REV v1, v1
	REV v2, v2
	REV v3, v3
	REV v4, v4
	EOR v1, r0
	EOR v2, r1
	EOR v3, r2
	EOR v4, r3
	LDR v5, = kt
	ADD v6, v5, 0x400
	ADD v7, v6, 0x400
	ADD v8, v7, 0x400
    # 1-8
    KRF v1 v2 v3 v4 0x00070e15
    KRF v2 v3 v4 v1 0x1c232a31
    KRF v3 v4 v1 v2 0x383f464d
    KRF v4 v1 v2 v3 0x545b6269
    KRF v1 v2 v3 v4 0x70777e85
    KRF v2 v3 v4 v1 0x8c939aa1
    KRF v3 v4 v1 v2 0xa8afb6bd
    KRF v4 v1 v2 v3 0xc4cbd2d9
	# 9-16
    KRF v1 v2 v3 v4 0xe0e7eef5
    KRF v2 v3 v4 v1 0xfc030a11
    KRF v3 v4 v1 v2 0x181f262d
    KRF v4 v1 v2 v3 0x343b4249
    KRF v1 v2 v3 v4 0x50575e65
    KRF v2 v3 v4 v1 0x6c737a81
    KRF v3 v4 v1 v2 0x888f969d
    KRF v4 v1 v2 v3 0xa4abb2b9
	# 17-24
    KRF v1 v2 v3 v4 0xc0c7ced5
    KRF v2 v3 v4 v1 0xdce3eaf1
    KRF v3 v4 v1 v2 0xf8ff060d
    KRF v4 v1 v2 v3 0x141b2229
    KRF v1 v2 v3 v4 0x30373e45
    KRF v2 v3 v4 v1 0x4c535a61
    KRF v3 v4 v1 v2 0x686f767d
    KRF v4 v1 v2 v3 0x848b9299
	# 25-32
    KRF v1 v2 v3 v4 0xa0a7aeb5
    KRF v2 v3 v4 v1 0xbcc3cad1
    KRF v3 v4 v1 v2 0xd8dfe6ed
    KRF v4 v1 v2 v3 0xf4fb0209
    KRF v1 v2 v3 v4 0x10171e25
    KRF v2 v3 v4 v1 0x2c333a41
    KRF v3 v4 v1 v2 0x484f565d
    KRF v4 v1 v2 v3 0x646b7279
    POP {v1-v8, ip}
    MOV pc, lr
    .ltorg

# void sm4_enc_core(u8 *out, const u8 *in, const u32 *rk);
.global sm4_enc_core
.type   sm4_enc_core, %function
sm4_enc_core:
    PUSH {v1-v8, ip}
    PUSH {r0}
    LDM r1, {v1-v4}
    MOV ip, r2
	REV v1, v1
	REV v2, v2
	REV v3, v3
	REV v4, v4
	LDR v5, = rt
	ADD v6, v5, 0x400
	ADD v7, v6, 0x400
	ADD v8, v7, 0x400
	# 1-8
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
	# 9-16
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
	# 17-24
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
	# 25-32
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3
    ERF v1 v2 v3 v4
    ERF v2 v3 v4 v1
    ERF v3 v4 v1 v2
    ERF v4 v1 v2 v3

	REV v1, v1
	REV v2, v2
	REV v3, v3
	REV v4, v4
	POP {r0}
	STR v4, [r0, #0x0]
	STR v3, [r0, #0x4]
	STR v2, [r0, #0x8]
	STR v1, [r0, #0xc]
	POP {v1-v8, ip}
	MOV pc, lr
    .ltorg

# void sm4_dec_core(u8 *out, const u8 *in, const u32 *rk);
.global sm4_dec_core
.type   sm4_dec_core, %function
sm4_dec_core:
    PUSH {v1-v8, ip}
    PUSH {r0}
    LDM r1, {v1-v4}
    MOV ip, r2
	REV v1, v1
	REV v2, v2
	REV v3, v3
	REV v4, v4
	LDR v5, =rt
	ADD v6, v5, 0x400
	ADD v7, v6, 0x400
	ADD v8, v7, 0x400
	ADD ip, 0x7C
	# 1-8
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
	# 9-16
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
	# 17-24
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
	# 25-32
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3
    DRF v1 v2 v3 v4
    DRF v2 v3 v4 v1
    DRF v3 v4 v1 v2
    DRF v4 v1 v2 v3

	REV v1, v1
	REV v2, v2
	REV v3, v3
	REV v4, v4
	POP {r0}
	STR v4, [r0, #0x0]
	STR v3, [r0, #0x4]
	STR v2, [r0, #0x8]
	STR v1, [r0, #0xc]
	POP {v1-v8, ip}
	MOV pc, lr
    .ltorg
.end