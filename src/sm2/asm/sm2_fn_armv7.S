.syntax unified
.thumb

// sm2_n
.equ sm2_n1, 0x39D54123
.equ sm2_n2, 0x53BBF409
.equ sm2_n3, 0x21C6052B
.equ sm2_n4, 0x7203DF6B
.equ sm2_n5, 0xFFFFFFFF
.equ sm2_n6, 0xFFFFFFFF
.equ sm2_n7, 0xFFFFFFFF
.equ sm2_n8, 0xFFFFFFFE

// save v and return
.macro SAVE
	STM r0, {v1-v8}
	POP {v1-v8}
	MOV pc, lr
.endm

// v = v - n if v >= n
.macro VTST
	CMP v8, #sm2_n8
	BCC 99f
 	BHI 98f
 	CMP v7, #sm2_n7
	BCC 99f
 	BHI 98f
	CMP v6, #sm2_n6
	BCC 99f
 	BHI 98f
	CMP v5, #sm2_n5
	BCC 99f
 	BHI 98f
	LDR ip, =sm2_n
	LDR lr, [ip, #0x0C]
	CMP v4, lr
	BCC 99f
 	BHI 98f
	LDR lr, [ip, #0x08]
	CMP v3, lr
	BCC 99f
 	BHI 98f
	LDR lr, [ip, #0x04]
	CMP v2, lr
	BCC 99f
 	BHI 98f
	LDR lr, [ip]
	CMP v1, lr
	BCC 99f
 98:VSUBN
 99://exit
.endm

// reg1 + reg2
.macro LADD reg1 reg2
	LDM \reg1!, {v1-v4}
	LDM \reg2!, {v5-v8}
	ADDS v1, v5
	ADCS v2, v6
	ADCS v3, v7
	ADCS v4, v8
	PUSH {v1-v4}
	LDM \reg1, {v5-v8}
	LDM \reg2, {v1-v4}
	ADCS v5, v1
	ADCS v6, v2
	ADCS v7, v3
	ADCS v8, v4
	POP {v1-v4}
.endm

// v = v - m
.macro VSUBM
	LDR ip, =sm2_m
	PUSH {v5-v8}
	LDM ip, {v5-v8}
	SUBS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	POP {v5-v8}
	SBCS v5, #0
	SBCS v6, #0
	SBCS v7, #0
	SBCS v8, #1 
.endm

// v = v - n
.macro VSUBN
	LDR ip, =sm2_n
	PUSH {v5-v8}
	LDM ip, {v5-v8}
	SUBS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	POP {v5-v8}
	SBCS v5, sm2_n5
	SBCS v6, sm2_n6
	SBCS v7, sm2_n7
	LDR ip, =sm2_n8
	SBCS v8, ip
.endm

// v = v - l
.macro VADDL
	LDR ip, =sm2_l
	PUSH {v5-v8}
	LDM ip, {v5-v8}
	ADDS v1, v5
	ADCS v2, v6
	ADCS v3, v7
	ADCS v4, v8
	POP {v5-v8}
	ADCS v5, #0xffffffff
	ADCS v6, #0xffffffff
	ADCS v7, #0x7fffffff
	ADCS v8, #0x7fffffff
.endm

// v = v >> 1 
.macro VLSR
	LSRS v1, v1, #1
	EOR v1, v1, v2, LSL #31
	LSR v2, v2, #1
	EOR v2, v2, v3, LSL #31
	LSR v3, v3, #1
	EOR v3, v3, v4, LSL #31
	LSR v4, v4, #1
	EOR v4, v4, v5, LSL #31
	LSR v5, v5, #1
	EOR v5, v5, v6, LSL #31
	LSR v6, v6, #1
	EOR v6, v6, v7, LSL #31
	LSR v7, v7, #1
	EOR v7, v7, v8, LSL #31
	LSR v8, v8, #1
.endm

// v = - v mod r
.macro VNEG
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
.endm


// LDP ptr to r and LDM to rlist
.macro LDP r ptr rlist
    LDR \r, =\ptr
    LDM \r, \rlist
.endm

// LDP ptr to r and STM rlist
.macro STP r ptr rlist
    LDR \r, =\ptr
    STM \r, \rlist
.endm

// reg1 - reg2, using in fp_inv
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

.macro MUL
	.rept 4
		PUSH {r0-r2}
		LDM r0, {v3-v8}
		LDM r1, {v1-v2}
		LDM r2, {r0-r3}
		MOV ip, #0
		MOV lr, #0
		UMAAL v3, ip, v1, r0
		UMAAL v4, ip, v1, r1
		UMAAL v5, ip, v1, r2
		UMAAL v6, ip, v1, r3
		UMAAL v4, lr, v2, r0
		UMAAL v5, lr, v2, r1
		UMAAL v6, lr, v2, r2
		UMAAL v7, lr, v2, r3
		ADDS v7, ip
		ADCS v8, lr
		POP {r0-r2}
		STM r0, {v3-v8}
		ADD r0, #0x10
		ADD r2, #0x10
		PUSH {r0-r2}
		LDM r0, {v3-v8}
		LDM r2, {r0-r3}
		MOV ip, #0
		MOV lr, #0
		UMAAL v3, ip, v1, r0
		UMAAL v4, ip, v1, r1
		UMAAL v5, ip, v1, r2
		UMAAL v6, ip, v1, r3
		UMAAL v4, lr, v2, r0
		UMAAL v5, lr, v2, r1
		UMAAL v6, lr, v2, r2
		UMAAL v7, lr, v2, r3
		ADCS v5, #0
		ADCS v6, #0
		ADDS v7, ip
		ADCS v8, lr
		POP {r0-r2}
		STM r0, {v3-v8}
		SUB r0, #0x08
		ADD r1, #0x08
		SUB r2, #0x10
	.endr
.endm


.section .rodate, "a"
	sm2_n: .word 0x39d54123, 0x53bbf409, 0x21c6052b, 0x7203df6b, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe
	sm2_m: .word 0xc62abedd, 0xac440bf6, 0xde39fad4, 0x8dfc2094, 0x00000000, 0x00000000, 0x00000000, 0x00000001
	sm2_l: .word 0x9CEAA092, 0xA9DDFA04, 0x90E30295, 0xB901EFB5, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF
	sm2_u: .word 0xF15149A0, 0x12AC6361, 0xFA323C01, 0x8DFC2096, 0x00000001, 0x00000001, 0x00000001, 0x00000001

.section .date, "aw"
	td1: .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    td2: .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    td3: .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	tu: .word 0, 0, 0, 0, 0, 0, 0, 0
	tv: .word 0, 0, 0, 0, 0, 0, 0, 0
	ta: .word 0, 0, 0, 0, 0, 0, 0, 0
	tc: .word 0, 0, 0, 0, 0, 0, 0, 0
	tw: .word 0, 0, 0, 0, 0, 0, 0, 0


.section .text, "ax"
	.global sm2_fn_add
	.global sm2_fn_sub
	.global sm2_fn_mul
	.global sm2_fn_inv
	

.thumb_func
sm2_fn_add:
	PUSH {v1-v8, ip}
    MOV ip, r2
    PUSH {r0}
    LDM r1, {v1-v8}
    LDM ip!, {r0-r3}
	ADDS v1, r0
	ADCS v2, r1
	ADCS v3, r2
	ADCS v4, r3
    LDM ip, {r0-r3}
	ADCS v5, r0
	ADCS v6, r1
	ADCS v7, r2
	ADCS v8, r3
	BCC 0f
	LDR ip, =sm2_m
    LDM ip, {r0-r3}
	ADDS v1, r0
	ADCS v2, r1
	ADCS v3, r2
	ADCS v4, r3
	ADCS v5, #0
	ADCS v6, #0
	ADCS v7, #0
	ADCS v8, #1
    POP {r0}
    STM r0, {v1-v8}
	POP {v1-v8, ip}
	MOV pc, lr
 0: LDR ip, =sm2_m
    LDM ip, {r0-r3}
    CMP v8, #sm2_n8
	BCC 2f
 	BHI 1f
 	CMP v7, #sm2_n7
	BCC 2f
 	BHI 1f
	CMP v6, #sm2_n6
	BCC 2f
 	BHI 1f
	CMP v5, #sm2_n5
	BCC 2f
 	BHI 1f
	CMP v4, r3
	BCC 2f
 	BHI 1f
	CMP v3, r2
	BCC 2f
 	BHI 1f
	CMP v2, r1
	BCC 2f
 	BHI 1f
	CMP v1, r0
	BCC 2f
 1: SUBS v1, r0
    SBCS v2, r1
    SBCS v3, r2
    SBCS v4, r3
    SBCS v5, #sm2_n5
    SBCS v6, #sm2_n6
    SBCS v7, #sm2_n7
    SBCS v8, #sm2_n8
 2: POP {r0}
    STM r0, {v1-v8}
	POP {v1-v8, ip}
	MOV pc, lr

sm2_fn_sub:
	PUSH {v1-v8, ip}
	MOV ip, r2
    PUSH {r0}
    LDM r1, {v1-v8}
    LDM ip!, {r0-r3}
	SUBS v1, r0
	SBCS v2, r1
	SBCS v3, r2
	SBCS v4, r3
    LDM ip!, {r0-r3}
	SBCS v5, r0
	SBCS v6, r1
	SBCS v7, r2
	SBCS v8, r3
	BCS 0f
	LDR ip, =sm2_m
    LDM ip, {r0-r3}
	SUBS v1, r0
	SBCS v2, r1
	SBCS v3, r2
	SBCS v4, r3
	SBCS v5, #0
	SBCS v6, #0
	SBCS v7, #0
	SBCS v8, #1
 0: POP {r0}
    STM r0, {v1-v8}
	POP {v1-v8, ip}
	MOV pc, lr

sm2_fn_mul:
	PUSH {v1-v8, ip, lr}
	PUSH {r0}
	// celar tdate
	LDP r3, sm2_zero, {v1-v8}
	LDR r0, =td1
	STM r0!, {v1-v8}
	STM r0!, {v1-v8}
	STM r0!, {v1-v8}
	STM r0!, {v1-v8}
	STM r0!, {v1-v8}
	STM r0!, {v1-v8}
	SUB r0, #0xC0

	// c = (a * b) >> 256
	MUL
	// e = (u * c) >> 256 + c ( u共257比特，其最高位为1，存储时省略最高位， 此处+ c 是补u的第257位的1)
	LDR r0, =td2
	LDR r1, =sm2_u
	SUB r2, r0, #0x20
	MUL
	LDR r1, =td2 + 0x20
	SUB r2, r1, #0x40
	LADD r1, r2
    MOV ip, #0
    ADC ip, #0
    PUSH {ip}
	SUB r1, #0x10
	STM r1, {v1-v8}

	// compute e * n
	LDR r0, =td3
	LDR r2, =sm2_n
	MUL
	POP {ip}
	TEQ ip, #0
	BEQ 0f
	LDR r1, =td3 + 0x20
	LDR r2, =sm2_n
	LADD r1, r2
	STM r1, {v1-v8}

	// a * b - e * n
 0: LDR r0, =td1
	LDR r1, =td3
	LDM r0!, {v1-v4}
	LDM r1!, {v5-v8}
	SUBS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	STMDB r0, {v1-v4}
	LDM r0!, {v1-v4}
	LDM r1!, {v5-v8}
	SBCS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	STMDB r0, {v1-v4}
	LDM r0!, {v1-v4}
	LDM r1!, {v5-v8}
	SBCS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	STMDB r0, {v1-v4}
	LDM r0!, {v1-v4}
	LDM r1!, {v5-v8}
	SBCS v1, v5
	SBCS v2, v6
	SBCS v3, v7
	SBCS v4, v8
	STMDB r0, {v1-v4}
	SUB r0, #0x40
	LDM r0, {v1-v8, ip}

 1: TEQ ip, #0
    BEQ 2f
    LDR lr, =sm2_n
	LDM lr!, {r0-r3}
    SUBS v1, r0
    SBCS v2, r1
    SBCS v3, r2
    SBCS v4, r3
	SBCS v5, sm2_n5
	SBCS v6, sm2_n6
	SBCS v7, sm2_n7
	SBCS v8, sm2_n8
	SBC ip, #0
    TEQ ip, #0
    BNE 1b
 2: // if v >= n, v = v -n
    LDR ip, =sm2_m
    LDM ip, {r0-r3}
    CMP v8, #sm2_n8
	BCC 4f
 	BHI 3f
 	CMP v7, #sm2_n7
	BCC 4f
 	BHI 3f
	CMP v6, #sm2_n6
	BCC 4f
 	BHI 3f
	CMP v5, #sm2_n5
	BCC 4f
 	BHI 3f
	CMP v4, r3
	BCC 4f
 	BHI 3f
	CMP v3, r2
	BCC 4f
 	BHI 3f
	CMP v2, r1
	BCC 4f
 	BHI 3f
	CMP v1, r0
	BCC 4f
 3: SUBS v1, r0
    SBCS v2, r1
    SBCS v3, r2
    SBCS v4, r3
    SBCS v5, #sm2_n5
    SBCS v6, #sm2_n6
    SBCS v7, #sm2_n7
    SBCS v8, #sm2_n8
 4: POP {r0}
    STM r0, {v1-v8}
	POP {v1-v8, ip, lr}
    MOV pc, lr


sm2_fn_inv:
	PUSH {r0, v1-v8}
	// u = input, v = n, a = 1, c = 0
	LDM r1, {v1-v8}
	STP r0, tu, {v1-v8}
	LDP r1, sm2_n, {v1-v8}
	STP r1, tv, {v1-v8}
	LDP r2, sm2_one, {v1-v8}
	STP r2, ta, {v1-v8}
	LDP r3, sm2_zero, {v1-v8}
	STP r3, tc, {v1-v8}

	// while u >= 1 do inv_loop 
 	inv_while: 
 	LDM r0, {v1-v8}
	TEQ v1, #0
	BNE inv_loop
	TEQ v2, #0
	BNE inv_loop
	TEQ v3, #0
	BNE inv_loop
	TEQ v4, #0
	BNE inv_loop
	TEQ v5, #0
	BNE inv_loop
	TEQ v6, #0
	BNE inv_loop
	TEQ v7, #0
	BNE inv_loop
	TEQ v8, #0
	BNE inv_loop
	B inv_end

	inv_loop:
	// while u is even, u = u >> 1, a = a / 2 mod p,
	inv_u_loop: 
	LDR v1, [r0]
	TST v1, #1
	BNE inv_v_loop
	LDM r0, {v1-v8}
	VLSR
	STM r0, {v1-v8}
	LDM r2, {v1-v8}
	VLSR
	BCC 0f
	VADDL
	VTST
 0: STM r2, {v1-v8}
	B inv_u_loop

	// while v is even, v = v >> 1, c = c / 2 mod n
	inv_v_loop: 
	LDR v1, [r1]
	TST v1, #1
	BNE inv_update_uv
	LDM r1, {v1-v8}
	VLSR
	STM r1, {v1-v8}
	LDM r3, {v1-v8}
	VLSR
	BCC 1f
	VADDL
	VTST
 1: STM r3, {v1-v8}
	B inv_v_loop

	// if u >= v, u = u - v, a = a - c mod n, else v = v - u, c = c - a mod n
	inv_update_uv: 
	SUBUV r0, r1
	BCC v_is_bigger
	STM r0, {v1-v8}
	SUBUV r2, r3
	BCS 2f
	VSUBM
 2:	STM r2, {v1-v8}
	B inv_while

	v_is_bigger:
	VNEG
	STM r1, {v1-v8}
	SUBUV r3, r2
	BCS 3f
	VSUBM
 3:	STM r3, {v1-v8}
	B inv_while

	inv_end: // end while
	POP {r0}
	LDM r3, {v1-v8}
	SAVE
.end