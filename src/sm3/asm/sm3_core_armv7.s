.syntax unified
.arch armv7
.thumb

# State update function for 0~15 rounds, sp register points to the message w[i], r0 is the constant 0x79cc4519
.macro rf0 a b c d e f g h i
    ldr r1, [sp], #0x04                         // r1 = w[i]
    ldr r2, [sp, #0x0C]                         // r2 = w[i+4]
    eor r2, r1                                  // r2 = w[i] ^ w[i+4]
    add \h, r1                                  // h += w[i]                (r1 is free now)
    add \d, r2                                  // d += w[i] ^ w[i + 4]     (r2 is free now)

	add r1, \e, r0, ror #(32-\i%32)%32          // r1 = ss1 = ((a <<< 12) + e + T[i]) <<< 7
    add r1, r1, \a, ror #20
    ror r1, #25

    eor r3, \e, \f                              // h += (e ^ f ^ g) + ss1
    eor r3, \g
    add \h, r3
    add \h, r1

    eor r3, \h, \h, ror #23                     // h = h ^ (h <<< 9) ^ (h <<< 17)
    eor \h, r3, \h, ror #15

    eor r2, r1, \a, ror #20                     // r2 = ss2 = (a <<< 12) ^ ss1
    eor r3, \a, \b                              // d += (a ^ b ^ c)+ ss2
    eor r3, \c
    add \d, r3
	add \d, r2

    ror \b, #23                                 // b = b <<< 9, f = f <<< 19
    ror \f, #13
.endm

# State update function for rounds 16~63, sp register points to the message w[i], r0 is the constant 0x7a879d8a
.macro rf1 a b c d e f g h i
    ldr r1, [sp], #0x04                         // r1 = w[i]
    ldr r2, [sp, #0x0C]                         // r2 = w[i+4]
    eor r2, r1                                  // r2 = w[i] ^ w[i+4]
    add \h, r1                                  // h += w[i]                (r1 is free now)
    add \d, r2                                  // d += w[i] ^ w[i + 4]     (r2 is free now)

	add r1, \e, r0, ror #(32-\i%32)%32          // r1 = ss1 = ((a <<< 12) + e + T[i]) <<< 7
    add r1, r1, \a, ror #20
    ror r1, #25

	eor r3, \f, \g                              // h += (e & f) | (~e & g) + ss1 = ((f ^ g) & e) ^ g + ss1
    and r3, \e
    eor r3, \g
    add \h, r3
	add \h, r1

    eor r3, \h, \h, ror #23                     // h = P0(h) = h ^ (h <<< 9) ^ (h <<< 17)
    eor \h, r3, \h, ror #15

    eor r2, r1, \a, ror #20                     // ss2 = (a <<< 12) ^ ss1   (r1 is free now)
    eor r3, \b, \c                              // d += ((a & b) | (a & c) | (b & c)) + ss2  = (a & (b | c)) | ((b & c)) + ss2
    and r3, \a
    and r1, \b, \c
    eor r3, r1
    add \d, r3
    add \d, r2

    ror \b, #23                                 // b = b <<< 9, f = f <<< 19
    ror \f, #13
.endm

# Message expansion: w[i+16] = P1(w[i] ^ w[i+7] ^ (w[i+13] <<< 15)) ^ (w[i+3] <<< 7) ^ w[i+10]
# P1(x) = x ^ x <<< 15 ^ x <<< 23 = x ^ x >>> 17 ^ x >>> 9
# Since the width of the sliding registers (w0-w13) is 14, there are no additional registers available for calculating P1(x).
# Therefore, w7 will be used as a temporary register here and restored from memory later.
# We hope you can have a better way to avoid reading the memory one more time.

.macro msgexp w0 w3 w7 w10 w13 i
    ldr \w13, [sp, #((13 + \i) << 2)]
    eor \w0, \w0, \w7
    eor \w0, \w0, \w13, ror #17
    eor \w7, \w0, \w0,  ror #17
    eor \w0, \w7, \w0,  ror #9
    eor \w0, \w0, \w3,  ror #25
    eor \w0, \w10
    ldr \w7, [sp, #((7 + \i) << 2)]
    str \w0, [sp, #((16 + \i) << 2)]
.endm

.section .text, "ax"
.thumb_func
.align 4

# void sm3_compress(u32 buf[8], const u8 *msg, u32 num);
.global sm3_compress
.type   sm3_compress, %function
sm3_compress:
    push {v1-ip, lr}
	.Lloop_start:
	subs r2, r2, 1
	bcc .Lloop_end
	push {r0-r2}
    sub sp, #(52<<2)
    add r1, #0x40
    ldr v3, [r1, #-4]!
    ldr v2, [r1, #-4]!
    ldr v1, [r1, #-4]!
    rev v1, v1
    rev v2, v2
    rev v3, v3
    push {v1-v3}
    ldr r12, [r1, #-4]!
    ldr r11, [r1, #-4]!
    ldr r10, [r1, #-4]!
    ldr r9, [r1, #-4]!
    ldr r8, [r1, #-4]!
    ldr r7, [r1, #-4]!
    ldr r6, [r1, #-4]!
    ldr r5, [r1, #-4]!
    ldr r4, [r1, #-4]!
    ldr r3, [r1, #-4]!
    ldr r2, [r1, #-4]!
    ldr r0, [r1, #-8]!
    ldr r1, [r1, #4]
    rev r0, r0
    rev r1, r1
    rev r2, r2
    rev r3, r3
    rev r4, r4
    rev r5, r5
    rev r6, r6
    rev r7, r7
    rev r8, r8
    rev r9, r9
    rev r10, r10
    rev r11, r11
    rev r12, r12
    push {r0-r12}
    msgexp r0  r3  r7  r10 r14 0
    msgexp r1  r4  r8  r11 r0  1
    msgexp r2  r5  r9  r12 r1  2
    msgexp r3  r6  r10 r14 r2  3
    msgexp r4  r7  r11 r0  r3  4
    msgexp r5  r8  r12 r1  r4  5
    msgexp r6  r9  r14 r2  r5  6
    msgexp r7  r10 r0  r3  r6  7
    msgexp r8  r11 r1  r4  r7  8
    msgexp r9  r12 r2  r5  r8  9
    msgexp r10 r14 r3  r6  r9  10
    msgexp r11 r0  r4  r7  r10 11
    msgexp r12 r1  r5  r8  r11 12
    msgexp r14 r2  r6  r9  r12 13
    msgexp r0  r3  r7  r10 r14 14
    msgexp r1  r4  r8  r11 r0  15
    msgexp r2  r5  r9  r12 r1  16
    msgexp r3  r6  r10 r14 r2  17
    msgexp r4  r7  r11 r0  r3  18
    msgexp r5  r8  r12 r1  r4  19
    msgexp r6  r9  r14 r2  r5  20
    msgexp r7  r10 r0  r3  r6  21
    msgexp r8  r11 r1  r4  r7  22
    msgexp r9  r12 r2  r5  r8  23
    msgexp r10 r14 r3  r6  r9  24
    msgexp r11 r0  r4  r7  r10 25
    msgexp r12 r1  r5  r8  r11 26
    msgexp r14 r2  r6  r9  r12 27
    msgexp r0  r3  r7  r10 r14 28
    msgexp r1  r4  r8  r11 r0  29
    msgexp r2  r5  r9  r12 r1  30
    msgexp r3  r6  r10 r14 r2  31
    msgexp r4  r7  r11 r0  r3  32
    msgexp r5  r8  r12 r1  r4  33
    msgexp r6  r9  r14 r2  r5  34
    msgexp r7  r10 r0  r3  r6  35
    msgexp r8  r11 r1  r4  r7  36
    msgexp r9  r12 r2  r5  r8  37
    msgexp r10 r14 r3  r6  r9  38
    msgexp r11 r0  r4  r7  r10 39
    msgexp r12 r1  r5  r8  r11 40
    msgexp r14 r2  r6  r9  r12 41
    msgexp r0  r3  r7  r10 r14 42
    msgexp r1  r4  r8  r11 r0  43
    msgexp r2  r5  r9  r12 r1  44
    msgexp r3  r6  r10 r14 r2  45
    msgexp r4  r7  r11 r0  r3  46
    msgexp r5  r8  r12 r1  r4  47
    msgexp r6  r9  r14 r2  r5  48
    msgexp r7  r10 r0  r3  r6  49
    msgexp r8  r11 r1  r4  r7  50
    msgexp r9  r12 r2  r5  r8  51
    # Load the state.
    ldr r0, =0x79cc4519
    ldr r1, [sp, #(68 << 2)]
    ldm r1, {v1-v8}
    # Note: Since the ldr offset relative to the current PC value cannot exceed 4KB in ARMV7,
    # and there are approximately 2000 lines of instructions inside this function that are out of the offset range,
    # we declare the literal pool here and skip it.
    b 1f
    .ltorg
1:  # 0-15
    rf0 v1 v2 v3 v4 v5 v6 v7 v8 0
    rf0 v4 v1 v2 v3 v8 v5 v6 v7 1
    rf0 v3 v4 v1 v2 v7 v8 v5 v6 2
    rf0 v2 v3 v4 v1 v6 v7 v8 v5 3
    rf0 v1 v2 v3 v4 v5 v6 v7 v8 4
    rf0 v4 v1 v2 v3 v8 v5 v6 v7 5
    rf0 v3 v4 v1 v2 v7 v8 v5 v6 6
    rf0 v2 v3 v4 v1 v6 v7 v8 v5 7
    rf0 v1 v2 v3 v4 v5 v6 v7 v8 8
    rf0 v4 v1 v2 v3 v8 v5 v6 v7 9
    rf0 v3 v4 v1 v2 v7 v8 v5 v6 10
    rf0 v2 v3 v4 v1 v6 v7 v8 v5 11
    rf0 v1 v2 v3 v4 v5 v6 v7 v8 12
    rf0 v4 v1 v2 v3 v8 v5 v6 v7 13
    rf0 v3 v4 v1 v2 v7 v8 v5 v6 14
    rf0 v2 v3 v4 v1 v6 v7 v8 v5 15
    # 16-31
    ldr r0 , =0x7a879d8a
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 16
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 17
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 18
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 19
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 20
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 21
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 22
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 23
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 24
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 25
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 26
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 27
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 28
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 29
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 30
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 31
    # 32-47
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 32
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 33
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 34
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 35
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 36
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 37
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 38
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 39
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 40
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 41
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 42
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 43
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 44
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 45
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 46
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 47
    # 48-63
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 48
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 49
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 50
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 51
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 52
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 53
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 54
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 55
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 56
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 57
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 58
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 59
    rf1 v1 v2 v3 v4 v5 v6 v7 v8 60
    rf1 v4 v1 v2 v3 v8 v5 v6 v7 61
    rf1 v3 v4 v1 v2 v7 v8 v5 v6 62
    rf1 v2 v3 v4 v1 v6 v7 v8 v5 63
    # Add the compressed chunk to the current hash value.
    add sp, sp, #16
    ldr ip, [sp]
    ldm ip!, {r0-r3}
    eor v1, r0
    eor v2, r1
    eor v3, r2
    eor v4, r3
    ldm ip!, {r0-r3}
    eor v5, r0
    eor v6, r1
    eor v7, r2
    eor v8, r3
    # Store the updated state.
    stmdb ip, {v1-v8}
    pop {r0-r2}
    add r1, r1, #0x40
    # Next block
	b .Lloop_start
	.Lloop_end:
	# Restore registers and return
    pop {v1-ip, lr}
    mov pc, lr
.end
