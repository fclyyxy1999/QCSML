.syntax unified
.arch armv7
.thumb

# keyschdul round function
.macro krf k0 k1 k2 k3 ck
	ldr r0, =\ck
	eor r0, \k1
	eor r0, \k2
	eor r0, \k3
	ubfx r1, r0, #8,  #8
	ubfx r2, r0, #16, #8
	ubfx r3, r0, #24, #8
	ubfx r0, r0, #0,  #8
	ldr r0, [v5, r0, LSL #2]
	ldr r1, [v6, r1, LSL #2]
	ldr r2, [v7, r2, LSL #2]
	ldr r3, [v8, r3, LSL #2]
	eor \k0, r0
	eor \k0, r1
	eor \k0, r2
	eor \k0, r3
	str \k0, [ip], #4
.endm

# encrypt round function
.macro erf p0 p1 p2 p3
	ldr r0, [ip], #4
	eor r0, \p1
	eor r0, \p2
	eor r0, \p3
	ubfx r1, r0, #8,  #8
	ubfx r2, r0, #16, #8
	ubfx r3, r0, #24, #8
	ubfx r0, r0, #0,  #8
	ldr r0, [v5, r0, LSL #2]
	ldr r1, [v6, r1, LSL #2]
	ldr r2, [v7, r2, LSL #2]
	ldr r3, [v8, r3, LSL #2]
	eor \p0, r0
	eor \p0, r1
	eor \p0, r2
	eor \p0, r3
.endm

# decrypt round function
.macro drf p0 p1 p2 p3
	ldr r0, [ip], #-4
	eor r0, \p1
	eor r0, \p2
	eor r0, \p3
	ubfx r1, r0, #8,  #8
	ubfx r2, r0, #16, #8
	ubfx r3, r0, #24, #8
	ubfx r0, r0, #0,  #8
	ldr r0, [v5, r0, LSL #2]
	ldr r1, [v6, r1, LSL #2]
	ldr r2, [v7, r2, LSL #2]
	ldr r3, [v8, r3, LSL #2]
	eor \p0, r0
	eor \p0, r1
	eor \p0, r2
	eor \p0, r3
.endm

.section .text, "ax"
.thumb_func
.align 4

# void sm4_setkey_core(u32 *rk, const u8 *key);
.global sm4_setkey_core
.type   sm4_setkey_core, %function
sm4_setkey_core:
	push {v1-v8, ip}
    mov ip, r0
	ldm r1, {v1-v4}
	ldr r0, = 0xa3b1bac6
	ldr r1, = 0x56aa3350
	ldr r2, = 0x677d9197
	ldr r3, = 0xb27022dc
	rev v1, v1
	rev v2, v2
	rev v3, v3
	rev v4, v4
	eor v1, r0
	eor v2, r1
	eor v3, r2
	eor v4, r3
	ldr v5, = kt
	add v6, v5, 0x400
	add v7, v6, 0x400
	add v8, v7, 0x400
    # 1-8
    krf v1 v2 v3 v4 0x00070e15
    krf v2 v3 v4 v1 0x1c232a31
    krf v3 v4 v1 v2 0x383f464d
    krf v4 v1 v2 v3 0x545b6269
    krf v1 v2 v3 v4 0x70777e85
    krf v2 v3 v4 v1 0x8c939aa1
    krf v3 v4 v1 v2 0xa8afb6bd
    krf v4 v1 v2 v3 0xc4cbd2d9
	# 9-16
    krf v1 v2 v3 v4 0xe0e7eef5
    krf v2 v3 v4 v1 0xfc030a11
    krf v3 v4 v1 v2 0x181f262d
    krf v4 v1 v2 v3 0x343b4249
    krf v1 v2 v3 v4 0x50575e65
    krf v2 v3 v4 v1 0x6c737a81
    krf v3 v4 v1 v2 0x888f969d
    krf v4 v1 v2 v3 0xa4abb2b9
	# 17-24
    krf v1 v2 v3 v4 0xc0c7ced5
    krf v2 v3 v4 v1 0xdce3eaf1
    krf v3 v4 v1 v2 0xf8ff060d
    krf v4 v1 v2 v3 0x141b2229
    krf v1 v2 v3 v4 0x30373e45
    krf v2 v3 v4 v1 0x4c535a61
    krf v3 v4 v1 v2 0x686f767d
    krf v4 v1 v2 v3 0x848b9299
	# 25-32
    krf v1 v2 v3 v4 0xa0a7aeb5
    krf v2 v3 v4 v1 0xbcc3cad1
    krf v3 v4 v1 v2 0xd8dfe6ed
    krf v4 v1 v2 v3 0xf4fb0209
    krf v1 v2 v3 v4 0x10171e25
    krf v2 v3 v4 v1 0x2c333a41
    krf v3 v4 v1 v2 0x484f565d
    krf v4 v1 v2 v3 0x646b7279
    pop {v1-v8, ip}
    mov pc, lr
    .ltorg

# void sm4_enc_core(u8 *out, const u8 *in, const u32 *rk);
.global sm4_enc_core
.type   sm4_enc_core, %function
sm4_enc_core:
    push {v1-v8, ip}
    push {r0}
    ldm r1, {v1-v4}
    mov ip, r2
	rev v1, v1
	rev v2, v2
	rev v3, v3
	rev v4, v4
	ldr v5, = rt
	add v6, v5, 0x400
	add v7, v6, 0x400
	add v8, v7, 0x400
	# 1-8
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
	# 9-16
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
	# 17-24
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
	# 25-32
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3
    erf v1 v2 v3 v4
    erf v2 v3 v4 v1
    erf v3 v4 v1 v2
    erf v4 v1 v2 v3

	rev v1, v1
	rev v2, v2
	rev v3, v3
	rev v4, v4
	pop {r0}
	str v4, [r0, #0x0]
	str v3, [r0, #0x4]
	str v2, [r0, #0x8]
	str v1, [r0, #0xc]
	pop {v1-v8, ip}
	mov pc, lr
    .ltorg

# void sm4_dec_core(u8 *out, const u8 *in, const u32 *rk);
.global sm4_dec_core
.type   sm4_dec_core, %function
sm4_dec_core:
    push {v1-v8, ip}
    push {r0}
    ldm r1, {v1-v4}
    mov ip, r2
	rev v1, v1
	rev v2, v2
	rev v3, v3
	rev v4, v4
	ldr v5, =rt
	add v6, v5, 0x400
	add v7, v6, 0x400
	add v8, v7, 0x400
	add ip, 0x7C
	# 1-8
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
	# 9-16
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
	# 17-24
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
	# 25-32
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3
    drf v1 v2 v3 v4
    drf v2 v3 v4 v1
    drf v3 v4 v1 v2
    drf v4 v1 v2 v3

	rev v1, v1
	rev v2, v2
	rev v3, v3
	rev v4, v4
	pop {r0}
	str v4, [r0, #0x0]
	str v3, [r0, #0x4]
	str v2, [r0, #0x8]
	str v1, [r0, #0xc]
	pop {v1-v8, ip}
	mov pc, lr
    .ltorg
.end