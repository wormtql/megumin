	.text
	.file	"main.cc"
	.globl	_ZNK6sphere3hitERK3rayffR10hit_record // -- Begin function _ZNK6sphere3hitERK3rayffR10hit_record
	.p2align	2
	.type	_ZNK6sphere3hitERK3rayffR10hit_record,@function
_ZNK6sphere3hitERK3rayffR10hit_record:  // @_ZNK6sphere3hitERK3rayffR10hit_record
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             // 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldp	s4, s2, [x1]
	ldp	s5, s3, [x0, #8]
	fsub	s2, s2, s3
	fsub	s4, s4, s5
	ldp	s3, s7, [x1, #8]
	ldp	s5, s17, [x0, #16]
	fmul	s6, s2, s2
	fsub	s3, s3, s5
	ldp	s16, s5, [x1, #16]
	fmadd	s19, s4, s4, s6
	fmul	s18, s16, s16
	fmul	s2, s2, s16
	fmadd	s16, s3, s3, s19
	fmadd	s6, s7, s7, s18
	fmadd	s2, s4, s7, s2
	fmsub	s7, s17, s17, s16
	fnmadd	s16, s5, s5, s6
	fmadd	s4, s3, s5, s2
	fmul	s2, s7, s16
	fmadd	s2, s4, s4, s2
	fcmp	s2, #0.0
	b.le	.LBB0_7
// %bb.1:
	fsqrt	s3, s2
	fmadd	s5, s5, s5, s6
	fneg	s6, s4
	fcmp	s3, s3
	b.vs	.LBB0_9
// %bb.2:
	fsub	s3, s6, s3
	fdiv	s3, s3, s5
	fcmp	s3, s1
	fccmp	s3, s0, #4, mi
	b.gt	.LBB0_10
.LBB0_3:
	fsqrt	s3, s2
	fcmp	s3, s3
	b.vs	.LBB0_12
// %bb.4:
	fsub	s2, s3, s4
	mov	w8, wzr
	fdiv	s2, s2, s5
	fcmp	s2, s1
	b.pl	.LBB0_8
.LBB0_5:
	fcmp	s2, s0
	b.le	.LBB0_8
// %bb.6:
	str	s2, [x2]
	ldur	d0, [x1, #12]
	ldr	s1, [x1, #20]
	ldr	d3, [x1]
	fmul	v0.2s, v0.2s, v2.s[0]
	fmul	s1, s2, s1
	ldr	s2, [x1, #8]
	fadd	v0.2s, v0.2s, v3.2s
	fadd	s1, s1, s2
	b	.LBB0_11
.LBB0_7:
	mov	w8, wzr
.LBB0_8:
	ldp	x29, x30, [sp, #64]             // 16-byte Folded Reload
	mov	w0, w8
	add	sp, sp, #80
	ret
.LBB0_9:
	stur	s0, [x29, #-4]                  // 4-byte Folded Spill
	fmov	s0, s2
	stur	x2, [x29, #-16]                 // 8-byte Folded Spill
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	stur	x1, [x29, #-24]                 // 8-byte Folded Spill
	stp	s2, s1, [sp, #16]               // 8-byte Folded Spill
	stur	s4, [x29, #-28]                 // 4-byte Folded Spill
	str	s5, [sp, #32]                   // 4-byte Folded Spill
	str	s6, [sp, #12]                   // 4-byte Folded Spill
	bl	sqrtf
	ldp	x1, x2, [x29, #-24]             // 16-byte Folded Reload
	fmov	s3, s0
	ldp	s6, s2, [sp, #12]               // 8-byte Folded Reload
	ldr	s5, [sp, #32]                   // 4-byte Folded Reload
	ldur	s4, [x29, #-28]                 // 4-byte Folded Reload
	ldr	s1, [sp, #20]                   // 4-byte Folded Reload
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	ldur	s0, [x29, #-4]                  // 4-byte Folded Reload
	fsub	s3, s6, s3
	fdiv	s3, s3, s5
	fcmp	s3, s1
	fccmp	s3, s0, #4, mi
	b.le	.LBB0_3
.LBB0_10:
	str	s3, [x2]
	ldur	d0, [x1, #12]
	ldr	s1, [x1, #20]
	ldr	d2, [x1]
	fmul	v0.2s, v0.2s, v3.s[0]
	fmul	s1, s3, s1
	ldr	s3, [x1, #8]
	fadd	v0.2s, v0.2s, v2.2s
	fadd	s1, s1, s3
.LBB0_11:
	stur	d0, [x2, #4]
	mov	w8, #1
	str	s1, [x2, #12]
	ldr	d2, [x0, #8]
	ldr	s3, [x0, #20]
	fsub	v0.2s, v0.2s, v2.2s
	dup	v2.2s, v3.s[0]
	fdiv	v0.2s, v0.2s, v2.2s
	ldr	s2, [x0, #16]
	fsub	s1, s1, s2
	fdiv	s1, s1, s3
	str	d0, [x2, #16]
	str	s1, [x2, #24]
	ldr	x9, [x0, #24]
	str	x9, [x2, #32]
	ldp	x29, x30, [sp, #64]             // 16-byte Folded Reload
	mov	w0, w8
	add	sp, sp, #80
	ret
.LBB0_12:
	stur	s0, [x29, #-4]                  // 4-byte Folded Spill
	fmov	s0, s2
	stur	x2, [x29, #-16]                 // 8-byte Folded Spill
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	stur	x1, [x29, #-24]                 // 8-byte Folded Spill
	str	s1, [sp, #20]                   // 4-byte Folded Spill
	stur	s4, [x29, #-28]                 // 4-byte Folded Spill
	str	s5, [sp, #32]                   // 4-byte Folded Spill
	bl	sqrtf
	ldp	x1, x2, [x29, #-24]             // 16-byte Folded Reload
	fmov	s3, s0
	ldr	s5, [sp, #32]                   // 4-byte Folded Reload
	ldur	s4, [x29, #-28]                 // 4-byte Folded Reload
	ldr	s1, [sp, #20]                   // 4-byte Folded Reload
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	ldur	s0, [x29, #-4]                  // 4-byte Folded Reload
	fsub	s2, s3, s4
	mov	w8, wzr
	fdiv	s2, s2, s5
	fcmp	s2, s1
	b.mi	.LBB0_5
	b	.LBB0_8
.Lfunc_end0:
	.size	_ZNK6sphere3hitERK3rayffR10hit_record, .Lfunc_end0-_ZNK6sphere3hitERK3rayffR10hit_record
	.cfi_endproc
                                        // -- End function
	.globl	_ZNK12hitable_list3hitERK3rayffR10hit_record // -- Begin function _ZNK12hitable_list3hitERK3rayffR10hit_record
	.p2align	2
	.type	_ZNK12hitable_list3hitERK3rayffR10hit_record,@function
_ZNK12hitable_list3hitERK3rayffR10hit_record: // @_ZNK12hitable_list3hitERK3rayffR10hit_record
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #128
	stp	d9, d8, [sp, #48]               // 16-byte Folded Spill
	stp	x29, x30, [sp, #64]             // 16-byte Folded Spill
	add	x29, sp, #64
	str	x23, [sp, #80]                  // 8-byte Folded Spill
	stp	x22, x21, [sp, #96]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #112]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 64
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -48
	.cfi_offset w30, -56
	.cfi_offset w29, -64
	.cfi_offset b8, -72
	.cfi_offset b9, -80
	ldr	w8, [x0, #16]
	cmp	w8, #1
	b.lt	.LBB1_5
// %bb.1:
	fmov	s8, s0
	fcvt	d9, s1
	mov	x19, x2
	mov	x20, x0
	mov	x21, x1
	mov	x22, xzr
	mov	w23, wzr
	b	.LBB1_3
.LBB1_2:                                //   in Loop: Header=BB1_3 Depth=1
	ldrsw	x8, [x20, #16]
	add	x22, x22, #1
	cmp	x22, x8
	b.ge	.LBB1_6
.LBB1_3:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x20, #8]
	fcvt	s1, d9
	fmov	s0, s8
	add	x2, sp, #8
	ldr	x0, [x8, x22, lsl #3]
	mov	x1, x21
	ldr	x8, [x0]
	ldr	x8, [x8]
	blr	x8
	tbz	w0, #0, .LBB1_2
// %bb.4:                               //   in Loop: Header=BB1_3 Depth=1
	ldr	s0, [sp, #8]
	mov	w23, #1
	ldur	q1, [sp, #8]
	ldur	q2, [sp, #24]
	fcvt	d9, s0
	ldr	x8, [sp, #40]
	stp	q1, q2, [x19]
	str	x8, [x19, #32]
	b	.LBB1_2
.LBB1_5:
	mov	w23, wzr
.LBB1_6:
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	and	w0, w23, #0x1
	ldp	x22, x21, [sp, #96]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #64]             // 16-byte Folded Reload
	ldp	d9, d8, [sp, #48]               // 16-byte Folded Reload
	ldr	x23, [sp, #80]                  // 8-byte Folded Reload
	add	sp, sp, #128
	ret
.Lfunc_end1:
	.size	_ZNK12hitable_list3hitERK3rayffR10hit_record, .Lfunc_end1-_ZNK12hitable_list3hitERK3rayffR10hit_record
	.cfi_endproc
                                        // -- End function
	.globl	_Z19random_in_unit_diskv        // -- Begin function _Z19random_in_unit_diskv
	.p2align	2
	.type	_Z19random_in_unit_diskv,@function
_Z19random_in_unit_diskv:               // @_Z19random_in_unit_diskv
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #64
	str	d10, [sp, #16]                  // 8-byte Folded Spill
	stp	d9, d8, [sp, #24]               // 16-byte Folded Spill
	stp	x29, x30, [sp, #40]             // 16-byte Folded Spill
	add	x29, sp, #40
	str	x19, [sp, #56]                  // 8-byte Folded Spill
	.cfi_def_cfa w29, 24
	.cfi_offset w19, -8
	.cfi_offset w30, -16
	.cfi_offset w29, -24
	.cfi_offset b8, -32
	.cfi_offset b9, -40
	.cfi_offset b10, -48
	mov	x8, #4467570830351532032
	fmov	v8.2s, #-1.00000000
	movi	d9, #0000000000000000
	fmov	s10, #1.00000000
	dup	v0.2d, x8
	str	q0, [sp]                        // 16-byte Folded Spill
.LBB2_1:                                // =>This Inner Loop Header: Depth=1
	bl	rand
	mov	w19, w0
	bl	rand
	fmov	s0, w19
	ldr	q1, [sp]                        // 16-byte Folded Reload
	mov	v0.s[1], w0
	sshll	v0.2d, v0.2s, #0
	scvtf	v0.2d, v0.2d
	fmul	v0.2d, v0.2d, v1.2d
	fcvtn	v0.2s, v0.2d
	fadd	v0.2s, v0.2s, v0.2s
	fadd	v0.2s, v0.2s, v8.2s
	mov	s1, v0.s[1]
	fmul	s2, s1, v0.s[1]
	fmla	s2, s0, v0.s[0]
	fadd	s2, s2, s9
	fcmp	s2, s10
	b.ge	.LBB2_1
// %bb.2:
	ldp	x29, x30, [sp, #40]             // 16-byte Folded Reload
	movi	d2, #0000000000000000
                                        // kill: def $s0 killed $s0 killed $q0
	ldp	d9, d8, [sp, #24]               // 16-byte Folded Reload
	ldr	x19, [sp, #56]                  // 8-byte Folded Reload
	ldr	d10, [sp, #16]                  // 8-byte Folded Reload
	add	sp, sp, #64
	ret
.Lfunc_end2:
	.size	_Z19random_in_unit_diskv, .Lfunc_end2-_Z19random_in_unit_diskv
	.cfi_endproc
                                        // -- End function
	.globl	_Z7schlickff                    // -- Begin function _Z7schlickff
	.p2align	2
	.type	_Z7schlickff,@function
_Z7schlickff:                           // @_Z7schlickff
	.cfi_startproc
// %bb.0:
	stp	d9, d8, [sp, #-32]!             // 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset b8, -24
	.cfi_offset b9, -32
	fmov	s2, #1.00000000
	fsub	s3, s2, s1
	fadd	s1, s1, s2
	fsub	s0, s2, s0
	fdiv	s1, s3, s1
	fcvt	d0, s0
	fmul	s1, s1, s1
	fsub	s3, s2, s1
	fcvt	d8, s1
	fmov	d1, #5.00000000
	fcvt	d9, s3
	bl	pow
	fmadd	d0, d9, d0, d8
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	fcvt	s0, d0
	ldp	d9, d8, [sp], #32               // 16-byte Folded Reload
	ret
.Lfunc_end3:
	.size	_Z7schlickff, .Lfunc_end3-_Z7schlickff
	.cfi_endproc
                                        // -- End function
	.globl	_Z7refractRK4vec3S1_fRS_        // -- Begin function _Z7refractRK4vec3S1_fRS_
	.p2align	2
	.type	_Z7refractRK4vec3S1_fRS_,@function
_Z7refractRK4vec3S1_fRS_:               // @_Z7refractRK4vec3S1_fRS_
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             // 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	d2, [x0]
	fmov	s16, s0
	ldr	s3, [x0, #8]
	fmul	v0.2s, v2.2s, v2.2s
	mov	s0, v0.s[1]
	fmla	s0, s2, v2.s[0]
	fmadd	s1, s3, s3, s0
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vs	.LBB4_5
.LBB4_1:
	dup	v1.2s, v0.s[0]
	ldr	d4, [x1]
	fdiv	s3, s3, s0
	ldr	s5, [x1, #8]
	fdiv	v2.2s, v2.2s, v1.2s
	fmul	s1, s16, s16
	fmul	v0.2s, v2.2s, v4.2s
	mov	s0, v0.s[1]
	fmla	s0, s4, v2.s[0]
	fmadd	s6, s3, s5, s0
	fmov	s0, #1.00000000
	fmsub	s7, s6, s6, s0
	fmul	s1, s1, s7
	fsub	s1, s0, s1
	fcmp	s1, #0.0
	b.le	.LBB4_4
// %bb.2:
	fsqrt	s0, s1
	fmul	v4.2s, v4.2s, v6.s[0]
	fmul	s5, s5, s6
	fsub	v2.2s, v2.2s, v4.2s
	fsub	s4, s3, s5
	fmul	v3.2s, v2.2s, v16.s[0]
	fmul	s2, s4, s16
	fcmp	s0, s0
	b.vs	.LBB4_6
.LBB4_3:
	ldr	d4, [x1]
	ldr	s5, [x1, #8]
	fmul	v4.2s, v4.2s, v0.s[0]
	fmul	s0, s0, s5
	fsub	v3.2s, v3.2s, v4.2s
	fsub	s0, s2, s0
	str	d3, [x2]
	str	s0, [x2, #8]
.LBB4_4:
	ldp	x29, x30, [sp, #64]             // 16-byte Folded Reload
	fcmp	s1, #0.0
	cset	w0, gt
	add	sp, sp, #80
	ret
.LBB4_5:
	fmov	s0, s1
	stp	x1, x2, [x29, #-16]             // 16-byte Folded Spill
	stur	s3, [x29, #-20]                 // 4-byte Folded Spill
	stp	q2, q16, [sp]                   // 32-byte Folded Spill
	bl	sqrtf
	ldp	q2, q16, [sp]                   // 32-byte Folded Reload
                                        // kill: def $s0 killed $s0 def $q0
	ldp	x1, x2, [x29, #-16]             // 16-byte Folded Reload
	ldur	s3, [x29, #-20]                 // 4-byte Folded Reload
	b	.LBB4_1
.LBB4_6:
	stur	s1, [x29, #-20]                 // 4-byte Folded Spill
	ldur	s0, [x29, #-20]                 // 4-byte Folded Reload
	stp	x1, x2, [x29, #-16]             // 16-byte Folded Spill
	str	s2, [sp, #16]                   // 4-byte Folded Spill
	str	d3, [sp]                        // 8-byte Folded Spill
	bl	sqrtf
	ldp	x1, x2, [x29, #-16]             // 16-byte Folded Reload
                                        // kill: def $s0 killed $s0 def $q0
	ldr	d3, [sp]                        // 8-byte Folded Reload
	ldr	s2, [sp, #16]                   // 4-byte Folded Reload
	ldur	s1, [x29, #-20]                 // 4-byte Folded Reload
	b	.LBB4_3
.Lfunc_end4:
	.size	_Z7refractRK4vec3S1_fRS_, .Lfunc_end4-_Z7refractRK4vec3S1_fRS_
	.cfi_endproc
                                        // -- End function
	.globl	_Z7reflectRK4vec3S1_            // -- Begin function _Z7reflectRK4vec3S1_
	.p2align	2
	.type	_Z7reflectRK4vec3S1_,@function
_Z7reflectRK4vec3S1_:                   // @_Z7reflectRK4vec3S1_
	.cfi_startproc
// %bb.0:
	ldp	s2, s1, [x0]
	ldp	s4, s0, [x1]
	ldr	s5, [x0, #8]
	ldr	s6, [x1, #8]
	fmul	s3, s1, s0
	fmadd	s3, s2, s4, s3
	fmadd	s3, s5, s6, s3
	fadd	s3, s3, s3
	fmul	s4, s4, s3
	fmul	s7, s0, s3
	fmul	s3, s6, s3
	fsub	s0, s2, s4
	fsub	s1, s1, s7
	fsub	s2, s5, s3
	ret
.Lfunc_end5:
	.size	_Z7reflectRK4vec3S1_, .Lfunc_end5-_Z7reflectRK4vec3S1_
	.cfi_endproc
                                        // -- End function
	.globl	_Z21random_in_unit_spherev      // -- Begin function _Z21random_in_unit_spherev
	.p2align	2
	.type	_Z21random_in_unit_spherev,@function
_Z21random_in_unit_spherev:             // @_Z21random_in_unit_spherev
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #80
	stp	d11, d10, [sp, #16]             // 16-byte Folded Spill
	stp	d9, d8, [sp, #32]               // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             // 16-byte Folded Spill
	add	x29, sp, #48
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	.cfi_def_cfa w29, 32
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w30, -24
	.cfi_offset w29, -32
	.cfi_offset b8, -40
	.cfi_offset b9, -48
	.cfi_offset b10, -56
	.cfi_offset b11, -64
	mov	x20, #4467570830351532032
	fmov	v8.2s, #-1.00000000
	fmov	s9, #-1.00000000
	fmov	s10, #1.00000000
	dup	v0.2d, x20
	str	q0, [sp]                        // 16-byte Folded Spill
.LBB6_1:                                // =>This Inner Loop Header: Depth=1
	bl	rand
	mov	w19, w0
	bl	rand
	fmov	s0, w19
	ldr	q1, [sp]                        // 16-byte Folded Reload
	mov	v0.s[1], w0
	sshll	v0.2d, v0.2s, #0
	scvtf	v0.2d, v0.2d
	fmul	v0.2d, v0.2d, v1.2d
	fcvtn	v11.2s, v0.2d
	bl	rand
	fadd	v0.2s, v11.2s, v11.2s
	scvtf	d1, w0
	fmov	d2, x20
	fmul	d1, d1, d2
	fadd	v0.2s, v0.2s, v8.2s
	fcvt	s2, d1
	mov	s1, v0.s[1]
	fadd	s2, s2, s2
	fmul	s3, s1, v0.s[1]
	fadd	s2, s2, s9
	fmla	s3, s0, v0.s[0]
	fmadd	s3, s2, s2, s3
	fcmp	s3, s10
	b.ge	.LBB6_1
// %bb.2:
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
                                        // kill: def $s0 killed $s0 killed $q0
	ldp	x29, x30, [sp, #48]             // 16-byte Folded Reload
	ldp	d9, d8, [sp, #32]               // 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #80
	ret
.Lfunc_end6:
	.size	_Z21random_in_unit_spherev, .Lfunc_end6-_Z21random_in_unit_spherev
	.cfi_endproc
                                        // -- End function
	.globl	_Z5colorRK3rayP7hitablei        // -- Begin function _Z5colorRK3rayP7hitablei
	.p2align	2
	.type	_Z5colorRK3rayP7hitablei,@function
_Z5colorRK3rayP7hitablei:               // @_Z5colorRK3rayP7hitablei
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #128
	str	d8, [sp, #80]                   // 8-byte Folded Spill
	stp	x29, x30, [sp, #88]             // 16-byte Folded Spill
	add	x29, sp, #88
	str	x21, [sp, #104]                 // 8-byte Folded Spill
	stp	x20, x19, [sp, #112]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 40
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w30, -32
	.cfi_offset w29, -40
	.cfi_offset b8, -48
	mov	w9, #4719
	ldr	x8, [x1]
	movk	w9, #14979, lsl #16
	mov	w10, #2139095039
	mov	x21, x0
	mov	w20, w2
	mov	x19, x1
	ldr	x8, [x8]
	fmov	s0, w9
	fmov	s1, w10
	add	x2, sp, #40
	mov	x0, x1
	mov	x1, x21
	blr	x8
	tbz	w0, #0, .LBB7_4
// %bb.1:
	movi	d8, #0000000000000000
	cmp	w20, #49
	b.gt	.LBB7_6
// %bb.2:
	ldr	x0, [sp, #72]
	add	x2, sp, #40
	mov	x3, sp
	add	x4, sp, #16
	mov	x1, x21
	ldr	x8, [x0]
	ldr	x8, [x8]
	blr	x8
	movi	d1, #0000000000000000
	movi	d2, #0000000000000000
	tbz	w0, #0, .LBB7_7
// %bb.3:
	add	w2, w20, #1
	add	x0, sp, #16
	mov	x1, x19
	bl	_Z5colorRK3rayP7hitablei
	ldp	s3, s4, [sp]
	ldr	s5, [sp, #8]
	fmul	s8, s3, s0
	fmul	s1, s4, s1
	fmul	s2, s2, s5
	b	.LBB7_7
.LBB7_4:
	ldp	s1, s8, [x21, #12]
	fmul	s0, s8, s8
	fmadd	s0, s1, s1, s0
	ldr	s1, [x21, #20]
	fmadd	s1, s1, s1, s0
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vs	.LBB7_8
.LBB7_5:
	fdiv	s0, s8, s0
	fmov	d1, #1.00000000
	mov	w8, #13107
	fmov	s2, #0.50000000
	movk	w8, #16179, lsl #16
	fmov	s3, w8
	fcvt	d0, s0
	fadd	d0, d0, d1
	fmov	d1, #0.50000000
	fmul	d0, d0, d1
	fmov	s1, #1.00000000
	fcvt	s0, d0
	fsub	s4, s1, s0
	fmul	s1, s0, s2
	fmul	s2, s0, s3
	fadd	s8, s4, s1
	fadd	s1, s4, s2
	fadd	s2, s4, s0
	b	.LBB7_7
.LBB7_6:
	movi	d1, #0000000000000000
	movi	d2, #0000000000000000
.LBB7_7:
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	fmov	s0, s8
	ldp	x29, x30, [sp, #88]             // 16-byte Folded Reload
	ldr	x21, [sp, #104]                 // 8-byte Folded Reload
	ldr	d8, [sp, #80]                   // 8-byte Folded Reload
	add	sp, sp, #128
	ret
.LBB7_8:
	fmov	s0, s1
	bl	sqrtf
	b	.LBB7_5
.Lfunc_end7:
	.size	_Z5colorRK3rayP7hitablei, .Lfunc_end7-_Z5colorRK3rayP7hitablei
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4                               // -- Begin function _Z12random_scenev
.LCPI8_0:
	.word	0x00000000                      // float 0
	.word	0xc47a0000                      // float -1000
	.word	0x00000000                      // float 0
	.word	0x447a0000                      // float 1000
.LCPI8_3:
	.word	0x00000000                      // float 0
	.word	0x3f800000                      // float 1
	.word	0x00000000                      // float 0
	.word	0x3f800000                      // float 1
.LCPI8_5:
	.word	0xc0800000                      // float -4
	.word	0x3f800000                      // float 1
	.word	0x00000000                      // float 0
	.word	0x3f800000                      // float 1
.LCPI8_6:
	.word	0x3f333333                      // float 0.699999988
	.word	0x3f19999a                      // float 0.600000024
	.word	0x3f000000                      // float 0.5
	.word	0x00000000                      // float 0
.LCPI8_7:
	.word	0x40800000                      // float 4
	.word	0x3f800000                      // float 1
	.word	0x00000000                      // float 0
	.word	0x3f800000                      // float 1
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI8_1:
	.xword	0x3feccccccccccccd              // double 0.90000000000000002
.LCPI8_2:
	.xword	0x3fe999999999999a              // double 0.80000000000000004
.LCPI8_4:
	.word	0x3ecccccd                      // float 0.400000006
	.word	0x3e4ccccd                      // float 0.200000003
	.text
	.globl	_Z12random_scenev
	.p2align	2
	.type	_Z12random_scenev,@function
_Z12random_scenev:                      // @_Z12random_scenev
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 156, DW.ref.__gxx_personality_v0
	.cfi_lsda 28, .Lexception0
// %bb.0:
	sub	sp, sp, #224
	stp	d15, d14, [sp, #64]             // 16-byte Folded Spill
	stp	d13, d12, [sp, #80]             // 16-byte Folded Spill
	stp	d11, d10, [sp, #96]             // 16-byte Folded Spill
	stp	d9, d8, [sp, #112]              // 16-byte Folded Spill
	stp	x29, x30, [sp, #128]            // 16-byte Folded Spill
	add	x29, sp, #128
	stp	x28, x27, [sp, #144]            // 16-byte Folded Spill
	stp	x26, x25, [sp, #160]            // 16-byte Folded Spill
	stp	x24, x23, [sp, #176]            // 16-byte Folded Spill
	stp	x22, x21, [sp, #192]            // 16-byte Folded Spill
	stp	x20, x19, [sp, #208]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 96
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w25, -56
	.cfi_offset w26, -64
	.cfi_offset w27, -72
	.cfi_offset w28, -80
	.cfi_offset w30, -88
	.cfi_offset w29, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	.cfi_offset b10, -120
	.cfi_offset b11, -128
	.cfi_offset b12, -136
	.cfi_offset b13, -144
	.cfi_offset b14, -152
	.cfi_offset b15, -160
	mov	w0, #4008
	bl	_Znam
	mov	x19, x0
	mov	w0, #32
	bl	_Znwm
	mov	x20, x0
.Ltmp0:
	mov	w0, #24
	bl	_Znwm
.Ltmp1:
// %bb.1:
	movi	v0.2s, #63, lsl #24
	adrp	x9, .LCPI8_0
	mov	w8, #1056964608
	mov	x26, #4467570830351532032
	fmov	v1.2d, #1.00000000
	adrp	x22, _ZTV10lambertian+16
	movi	d10, #0000000000000000
	mov	w27, #52429
	str	d0, [x0, #8]
	ldr	q0, [x9, :lo12:.LCPI8_0]
	adrp	x9, .LCPI8_2
	str	w8, [x0, #16]
	adrp	x8, _ZTV6sphere+16
	add	x22, x22, :lo12:_ZTV10lambertian+16
	add	x8, x8, :lo12:_ZTV6sphere+16
	stur	q0, [x20, #8]
	ldr	d0, [x9, :lo12:.LCPI8_2]
	mov	w28, #-11
	mov	w24, #1
	fmov	s9, #-4.00000000
	str	x8, [x20]
	adrp	x8, .LCPI8_1
	str	d0, [sp, #56]                   // 8-byte Folded Spill
	movk	w27, #15948, lsl #16
	fmov	v0.2d, #0.50000000
	str	x22, [x0]
	ldr	d8, [x8, :lo12:.LCPI8_1]
	str	x0, [x20, #24]
	str	x20, [x19]
	stp	q0, q1, [sp, #16]               // 32-byte Folded Spill
	dup	v0.2d, x26
	str	q0, [sp]                        // 16-byte Folded Spill
	b	.LBB8_3
.LBB8_2:                                //   in Loop: Header=BB8_3 Depth=1
	add	w28, w28, #1
	cmp	w28, #11
	b.eq	.LBB8_17
.LBB8_3:                                // =>This Loop Header: Depth=1
                                        //     Child Loop BB8_8 Depth 2
	scvtf	d15, w28
	mov	w25, #-11
	b	.LBB8_8
.LBB8_4:                                //   in Loop: Header=BB8_8 Depth=2
.Ltmp3:
	mov	w0, #16
	bl	_Znwm
.Ltmp4:
// %bb.5:                               //   in Loop: Header=BB8_8 Depth=2
	adrp	x8, _ZTV10dielectric+16
	mov	x21, x0
	add	x8, x8, :lo12:_ZTV10dielectric+16
	str	x8, [x0]
	mov	w8, #1069547520
	str	w8, [x0, #8]
.LBB8_6:                                //   in Loop: Header=BB8_8 Depth=2
	adrp	x8, _ZTV6sphere+16
	str	x20, [x19, w24, sxtw #3]
	add	x8, x8, :lo12:_ZTV6sphere+16
	str	s12, [x20, #8]
	str	w27, [x20, #12]
	str	s14, [x20, #16]
	str	x8, [x20]
	add	w8, w24, #1
	mov	w24, w8
	str	w27, [x20, #20]
	str	x21, [x20, #24]
.LBB8_7:                                //   in Loop: Header=BB8_8 Depth=2
	add	w25, w25, #1
	cmp	w25, #11
	b.eq	.LBB8_2
.LBB8_8:                                //   Parent Loop BB8_3 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	bl	rand
	mov	w20, w0
	bl	rand
	scvtf	d0, w0
	fmov	d13, x26
	scvtf	d14, w25
	fmul	d0, d0, d13
	fmadd	d0, d0, d8, d15
	fcvt	s12, d0
	bl	rand
	scvtf	d1, w0
	fadd	s0, s12, s9
	fmul	d1, d1, d13
	fmadd	s0, s0, s0, s10
	fmadd	d1, d1, d8, d14
	fcvt	s14, d1
	fmadd	s1, s14, s14, s0
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vs	.LBB8_16
// %bb.9:                               //   in Loop: Header=BB8_8 Depth=2
	fcvt	d0, s0
	fcmp	d0, d8
	b.le	.LBB8_7
.LBB8_10:                               //   in Loop: Header=BB8_8 Depth=2
	scvtf	d0, w20
	fmov	d1, x26
	mov	w0, #32
	fmul	d0, d0, d1
	fcvt	s0, d0
	fcvt	d13, s0
	ldr	d0, [sp, #56]                   // 8-byte Folded Reload
	fcmp	d13, d0
	b.pl	.LBB8_13
// %bb.11:                              //   in Loop: Header=BB8_8 Depth=2
	bl	_Znwm
	mov	x20, x0
.Ltmp9:
	mov	w0, #24
	bl	_Znwm
.Ltmp10:
// %bb.12:                              //   in Loop: Header=BB8_8 Depth=2
	mov	x21, x0
	bl	rand
	scvtf	d0, w0
	fmov	d13, x26
	fmul	d9, d0, d13
	bl	rand
	scvtf	d0, w0
	fmul	d0, d0, d13
	fmul	d0, d9, d0
	fcvt	s9, d0
	bl	rand
	scvtf	d0, w0
	fmul	d10, d0, d13
	bl	rand
	scvtf	d0, w0
	fmul	d0, d0, d13
	fmul	d0, d10, d0
	fcvt	s10, d0
	bl	rand
	scvtf	d0, w0
	fmul	d11, d0, d13
	bl	rand
	scvtf	d0, w0
	str	x22, [x21]
	str	s9, [x21, #8]
	fmov	s9, #-4.00000000
	str	s10, [x21, #12]
	movi	d10, #0000000000000000
	fmul	d0, d0, d13
	fmul	d0, d11, d0
	fcvt	s0, d0
	str	s0, [x21, #16]
	b	.LBB8_6
.LBB8_13:                               //   in Loop: Header=BB8_8 Depth=2
	bl	_Znwm
	mov	x8, #7378697629483820646
	mov	x20, x0
	movk	x8, #16366, lsl #48
	fmov	d0, x8
	fcmp	d13, d0
	b.pl	.LBB8_4
// %bb.14:                              //   in Loop: Header=BB8_8 Depth=2
.Ltmp6:
	mov	w0, #24
	mov	x23, x22
	bl	_Znwm
.Ltmp7:
// %bb.15:                              //   in Loop: Header=BB8_8 Depth=2
	mov	x21, x0
	bl	rand
	mov	w22, w0
	bl	rand
	fmov	s0, w22
	ldr	q1, [sp]                        // 16-byte Folded Reload
	mov	v0.s[1], w0
	sshll	v0.2d, v0.2s, #0
	scvtf	v0.2d, v0.2d
	fmul	v0.2d, v0.2d, v1.2d
	ldr	q1, [sp, #32]                   // 16-byte Folded Reload
	fadd	v0.2d, v0.2d, v1.2d
	ldr	q1, [sp, #16]                   // 16-byte Folded Reload
	fmul	v0.2d, v0.2d, v1.2d
	fcvtn	v9.2s, v0.2d
	bl	rand
	scvtf	d0, w0
	fmov	d10, x26
	fmov	d1, #1.00000000
	fmov	d13, #0.50000000
	fmul	d0, d0, d10
	fadd	d0, d0, d1
	fmul	d0, d0, d13
	fcvt	s11, d0
	bl	rand
	scvtf	d0, w0
	fmov	s1, #1.00000000
	adrp	x8, _ZTV5metal+16
	str	d9, [x21, #8]
	add	x8, x8, :lo12:_ZTV5metal+16
	str	s11, [x21, #16]
	mov	x22, x23
	fmov	s9, #-4.00000000
	fmul	d0, d0, d10
	movi	d10, #0000000000000000
	str	x8, [x21]
	fmul	d0, d0, d13
	fcvt	s0, d0
	fminnm	s0, s0, s1
	str	s0, [x21, #20]
	b	.LBB8_6
.LBB8_16:                               //   in Loop: Header=BB8_8 Depth=2
	fmov	s0, s1
	bl	sqrtf
	fcvt	d0, s0
	fcmp	d0, d8
	b.le	.LBB8_7
	b	.LBB8_10
.LBB8_17:
	mov	w0, #32
	bl	_Znwm
	mov	x20, x0
.Ltmp12:
	mov	w0, #16
	bl	_Znwm
.Ltmp13:
// %bb.18:
	adrp	x9, _ZTV10dielectric+16
	adrp	x8, .LCPI8_3
	add	x9, x9, :lo12:_ZTV10dielectric+16
	adrp	x23, _ZTV6sphere+16
	add	x23, x23, :lo12:_ZTV6sphere+16
	str	x0, [x20, #24]
	ldr	q0, [x8, :lo12:.LCPI8_3]
	str	x20, [x19, w24, sxtw #3]
	str	x9, [x0]
	mov	w9, #1069547520
	str	x23, [x20]
	stur	q0, [x20, #8]
	str	w9, [x0, #8]
	mov	w0, #32
	bl	_Znwm
	mov	x20, x0
.Ltmp15:
	mov	w0, #24
	bl	_Znwm
.Ltmp16:
// %bb.19:
	adrp	x8, .LCPI8_4
	mov	w11, #52429
	adrp	x10, .LCPI8_5
	movk	w11, #15820, lsl #16
	sxtw	x9, w24
	str	x22, [x0]
	ldr	d0, [x8, :lo12:.LCPI8_4]
	str	x0, [x20, #24]
	ldr	q1, [x10, :lo12:.LCPI8_5]
	add	x21, x19, x9, lsl #3
	str	w11, [x0, #16]
	str	d0, [x0, #8]
	mov	w0, #32
	str	x23, [x20]
	stur	q1, [x20, #8]
	str	x20, [x21, #8]
	bl	_Znwm
	mov	x20, x0
.Ltmp18:
	mov	w0, #24
	bl	_Znwm
.Ltmp19:
// %bb.20:
	adrp	x8, .LCPI8_6
	adrp	x10, _ZTV5metal+16
	adrp	x9, .LCPI8_7
	add	x10, x10, :lo12:_ZTV5metal+16
	str	x0, [x20, #24]
	add	w22, w24, #3
	ldr	q0, [x8, :lo12:.LCPI8_6]
	str	x23, [x20]
	str	x10, [x0]
	ldr	q1, [x9, :lo12:.LCPI8_7]
	str	x20, [x21, #16]
	stur	q0, [x0, #8]
	mov	w0, #24
	stur	q1, [x20, #8]
	bl	_Znwm
	adrp	x8, _ZTV12hitable_list+16
	str	w22, [x0, #16]
	add	x8, x8, :lo12:_ZTV12hitable_list+16
	ldp	x22, x21, [sp, #192]            // 16-byte Folded Reload
	stp	x8, x19, [x0]
	ldp	x20, x19, [sp, #208]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #176]            // 16-byte Folded Reload
	ldp	x26, x25, [sp, #160]            // 16-byte Folded Reload
	ldp	x28, x27, [sp, #144]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #128]            // 16-byte Folded Reload
	ldp	d9, d8, [sp, #112]              // 16-byte Folded Reload
	ldp	d11, d10, [sp, #96]             // 16-byte Folded Reload
	ldp	d13, d12, [sp, #80]             // 16-byte Folded Reload
	ldp	d15, d14, [sp, #64]             // 16-byte Folded Reload
	add	sp, sp, #224
	ret
.LBB8_21:
.Ltmp20:
	b	.LBB8_28
.LBB8_22:
.Ltmp17:
	b	.LBB8_28
.LBB8_23:
.Ltmp14:
	b	.LBB8_28
.LBB8_24:
.Ltmp2:
	b	.LBB8_28
.LBB8_25:
.Ltmp8:
	b	.LBB8_28
.LBB8_26:
.Ltmp5:
	b	.LBB8_28
.LBB8_27:
.Ltmp11:
.LBB8_28:
	mov	x19, x0
	mov	x0, x20
	bl	_ZdlPv
	mov	x0, x19
	bl	_Unwind_Resume
.Lfunc_end8:
	.size	_Z12random_scenev, .Lfunc_end8-_Z12random_scenev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table8:
.Lexception0:
	.byte	255                             // @LPStart Encoding = omit
	.byte	255                             // @TType Encoding = omit
	.byte	1                               // Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    // >> Call Site 1 <<
	.uleb128 .Ltmp0-.Lfunc_begin0           //   Call between .Lfunc_begin0 and .Ltmp0
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp0-.Lfunc_begin0           // >> Call Site 2 <<
	.uleb128 .Ltmp1-.Ltmp0                  //   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0           //     jumps to .Ltmp2
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp3-.Lfunc_begin0           // >> Call Site 3 <<
	.uleb128 .Ltmp4-.Ltmp3                  //   Call between .Ltmp3 and .Ltmp4
	.uleb128 .Ltmp5-.Lfunc_begin0           //     jumps to .Ltmp5
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp4-.Lfunc_begin0           // >> Call Site 4 <<
	.uleb128 .Ltmp9-.Ltmp4                  //   Call between .Ltmp4 and .Ltmp9
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp9-.Lfunc_begin0           // >> Call Site 5 <<
	.uleb128 .Ltmp10-.Ltmp9                 //   Call between .Ltmp9 and .Ltmp10
	.uleb128 .Ltmp11-.Lfunc_begin0          //     jumps to .Ltmp11
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp10-.Lfunc_begin0          // >> Call Site 6 <<
	.uleb128 .Ltmp6-.Ltmp10                 //   Call between .Ltmp10 and .Ltmp6
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp6-.Lfunc_begin0           // >> Call Site 7 <<
	.uleb128 .Ltmp7-.Ltmp6                  //   Call between .Ltmp6 and .Ltmp7
	.uleb128 .Ltmp8-.Lfunc_begin0           //     jumps to .Ltmp8
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp7-.Lfunc_begin0           // >> Call Site 8 <<
	.uleb128 .Ltmp12-.Ltmp7                 //   Call between .Ltmp7 and .Ltmp12
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp12-.Lfunc_begin0          // >> Call Site 9 <<
	.uleb128 .Ltmp13-.Ltmp12                //   Call between .Ltmp12 and .Ltmp13
	.uleb128 .Ltmp14-.Lfunc_begin0          //     jumps to .Ltmp14
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp13-.Lfunc_begin0          // >> Call Site 10 <<
	.uleb128 .Ltmp15-.Ltmp13                //   Call between .Ltmp13 and .Ltmp15
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp15-.Lfunc_begin0          // >> Call Site 11 <<
	.uleb128 .Ltmp16-.Ltmp15                //   Call between .Ltmp15 and .Ltmp16
	.uleb128 .Ltmp17-.Lfunc_begin0          //     jumps to .Ltmp17
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp16-.Lfunc_begin0          // >> Call Site 12 <<
	.uleb128 .Ltmp18-.Ltmp16                //   Call between .Ltmp16 and .Ltmp18
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp18-.Lfunc_begin0          // >> Call Site 13 <<
	.uleb128 .Ltmp19-.Ltmp18                //   Call between .Ltmp18 and .Ltmp19
	.uleb128 .Ltmp20-.Lfunc_begin0          //     jumps to .Ltmp20
	.byte	0                               //   On action: cleanup
	.uleb128 .Ltmp19-.Lfunc_begin0          // >> Call Site 14 <<
	.uleb128 .Lfunc_end8-.Ltmp19            //   Call between .Ltmp19 and .Lfunc_end8
	.byte	0                               //     has no landing pad
	.byte	0                               //   On action: cleanup
.Lcst_end0:
	.p2align	2
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               // -- Begin function main
.LCPI9_0:
	.word	0x3e664192                      // float 0.224859506
	.word	0x00000000                      // float 0
.LCPI9_1:
	.word	0x3e13eb95                      // float 0.144453362
	.word	0x3f7d2bd2                      // float 0.988949894
.LCPI9_2:
	.word	0x41500000                      // float 13
	.word	0x40000000                      // float 2
.LCPI9_3:
	.word	0x3f98405a                      // float 1.18946385
	.word	0x00000000                      // float 0
.LCPI9_4:
	.word	0x404184e8                      // float 3.02373695
	.word	0xbf9cf6e1                      // float -1.22628415
.LCPI9_5:
	.word	0x3f026962                      // float 0.509420514
	.word	0x405f345d                      // float 3.487571
.LCPI9_6:
	.word	0xc1500000                      // float -13
	.word	0xc0000000                      // float -2
.LCPI9_7:
	.xword	0x406fffae147ae148              // double 255.99000000000001
	.text
	.globl	main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #304
	stp	d15, d14, [sp, #144]            // 16-byte Folded Spill
	stp	d13, d12, [sp, #160]            // 16-byte Folded Spill
	stp	d11, d10, [sp, #176]            // 16-byte Folded Spill
	stp	d9, d8, [sp, #192]              // 16-byte Folded Spill
	stp	x29, x30, [sp, #208]            // 16-byte Folded Spill
	add	x29, sp, #208
	stp	x28, x27, [sp, #224]            // 16-byte Folded Spill
	stp	x26, x25, [sp, #240]            // 16-byte Folded Spill
	stp	x24, x23, [sp, #256]            // 16-byte Folded Spill
	stp	x22, x21, [sp, #272]            // 16-byte Folded Spill
	stp	x20, x19, [sp, #288]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 96
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w25, -56
	.cfi_offset w26, -64
	.cfi_offset w27, -72
	.cfi_offset w28, -80
	.cfi_offset w30, -88
	.cfi_offset w29, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	.cfi_offset b10, -120
	.cfi_offset b11, -128
	.cfi_offset b12, -136
	.cfi_offset b13, -144
	.cfi_offset b14, -152
	.cfi_offset b15, -160
	adrp	x19, :got:_ZSt4cout
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	mov	w2, #3
	ldr	x19, [x19, :got_lo12:_ZSt4cout]
	mov	x0, x19
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	mov	x0, x19
	mov	w1, #1200
	bl	_ZNSolsEi
	adrp	x1, .L.str.1
	mov	w2, #1
	add	x1, x1, :lo12:.L.str.1
	mov	x21, x0
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	mov	x0, x21
	mov	w1, #800
	bl	_ZNSolsEi
	adrp	x1, .L.str.2
	mov	w2, #5
	add	x1, x1, :lo12:.L.str.2
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	mov	w23, #799
	bl	_Z12random_scenev
	adrp	x8, .LCPI9_0
	adrp	x9, .LCPI9_1
	adrp	x10, .LCPI9_2
	movi	d11, #0000000000000000
	mov	w19, #52429
	mov	w22, #29107
	ldr	d0, [x8, :lo12:.LCPI9_0]
	adrp	x8, .LCPI9_3
	mov	w20, #35505
	mov	w24, #61537
	mov	w27, #24968
	mov	w28, #49805
	str	d0, [sp, #80]                   // 8-byte Folded Spill
	ldr	d0, [x9, :lo12:.LCPI9_1]
	adrp	x9, .LCPI9_4
	mov	x21, x0
	mov	x26, #4467570830351532032
	fmov	s10, #-1.00000000
	str	d0, [sp, #72]                   // 8-byte Folded Spill
	ldr	d0, [x10, :lo12:.LCPI9_2]
	adrp	x10, .LCPI9_6
	fmov	s12, #1.00000000
	mov	w25, #1150681088
	movk	w19, #15692, lsl #16
	str	d0, [sp, #64]                   // 8-byte Folded Spill
	ldr	d0, [x8, :lo12:.LCPI9_3]
	adrp	x8, .LCPI9_5
	movk	w22, #49017, lsl #16
	movk	w20, #48392, lsl #16
	movk	w24, #49316, lsl #16
	str	d0, [sp, #56]                   // 8-byte Folded Spill
	ldr	d0, [x9, :lo12:.LCPI9_4]
	movk	w27, #16474, lsl #16
	movk	w28, #48624, lsl #16
	str	d0, [sp, #48]                   // 8-byte Folded Spill
	ldr	d0, [x8, :lo12:.LCPI9_5]
	adrp	x8, .LCPI9_7
	str	d0, [sp, #40]                   // 8-byte Folded Spill
	ldr	d0, [x10, :lo12:.LCPI9_6]
	str	d0, [sp, #32]                   // 8-byte Folded Spill
	ldr	d0, [x8, :lo12:.LCPI9_7]
	str	d0, [sp, #16]                   // 8-byte Folded Spill
	b	.LBB9_2
.LBB9_1:                                //   in Loop: Header=BB9_2 Depth=1
	ldr	w8, [sp, #12]                   // 4-byte Folded Reload
	sub	w23, w8, #1
	cbz	w8, .LBB9_14
.LBB9_2:                                // =>This Loop Header: Depth=1
                                        //     Child Loop BB9_3 Depth 2
                                        //       Child Loop BB9_4 Depth 3
                                        //         Child Loop BB9_5 Depth 4
	scvtf	d1, w23
	movi	d0, #0000000000000000
	mov	w8, wzr
	str	w23, [sp, #12]                  // 4-byte Folded Spill
	stp	d1, d0, [sp, #88]               // 16-byte Folded Spill
.LBB9_3:                                //   Parent Loop BB9_2 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB9_4 Depth 3
                                        //         Child Loop BB9_5 Depth 4
	movi	d8, #0000000000000000
	movi	d13, #0000000000000000
	movi	d14, #0000000000000000
	mov	w23, wzr
	str	w8, [sp, #28]                   // 4-byte Folded Spill
.LBB9_4:                                //   Parent Loop BB9_2 Depth=1
                                        //     Parent Loop BB9_3 Depth=2
                                        // =>    This Loop Header: Depth=3
                                        //         Child Loop BB9_5 Depth 4
	stur	s8, [x29, #-100]                // 4-byte Folded Spill
	bl	rand
	scvtf	d0, w0
	fmov	d8, x26
	ldr	d1, [sp, #96]                   // 8-byte Folded Reload
	fmul	d0, d0, d8
	fadd	d0, d0, d1
	fcvt	s0, d0
	str	s0, [sp, #104]                  // 4-byte Folded Spill
	bl	rand
	scvtf	d0, w0
	ldr	d1, [sp, #88]                   // 8-byte Folded Reload
	fmul	d0, d0, d8
	fadd	d0, d0, d1
	fcvt	s8, d0
.LBB9_5:                                //   Parent Loop BB9_2 Depth=1
                                        //     Parent Loop BB9_3 Depth=2
                                        //       Parent Loop BB9_4 Depth=3
                                        // =>      This Inner Loop Header: Depth=4
	bl	rand
	scvtf	d0, w0
	fmov	d9, x26
	fmul	d0, d0, d9
	fcvt	s15, d0
	bl	rand
	scvtf	d1, w0
	fadd	s0, s15, s15
	fmul	d1, d1, d9
	fadd	s0, s0, s10
	fcvt	s1, d1
	fadd	s1, s1, s1
	fadd	s1, s1, s10
	fmul	s2, s1, s1
	fmadd	s2, s0, s0, s2
	fadd	s2, s2, s11
	fcmp	s2, s12
	b.ge	.LBB9_5
// %bb.6:                               //   in Loop: Header=BB9_4 Depth=3
	fmov	s2, w25
	ldr	s3, [sp, #104]                  // 4-byte Folded Reload
	mov	w8, #1145569280
	fmov	s4, w19
	ldp	d16, d5, [sp, #48]              // 16-byte Folded Reload
	fdiv	s2, s3, s2
	fmov	s7, w24
	fmov	s3, w8
	fmul	s0, s0, s4
	fmul	s1, s1, s4
	ldr	d4, [sp, #40]                   // 8-byte Folded Reload
	ldp	d19, d18, [sp, #72]             // 16-byte Folded Reload
	fmov	s17, w28
	fmov	s6, w22
	sub	x0, x29, #96
	mov	x1, x21
	mov	w2, wzr
	fmul	v19.2s, v19.2s, v1.s[0]
	fmul	v18.2s, v18.2s, v0.s[0]
	fmul	s0, s0, s6
	fdiv	s3, s8, s3
	fadd	v6.2s, v18.2s, v19.2s
	fmul	v5.2s, v5.2s, v2.s[0]
	fmul	s2, s2, s7
	fmov	s7, w27
	fadd	v5.2s, v5.2s, v16.2s
	fmov	s16, w20
	fadd	s2, s2, s7
	fmul	s1, s1, s16
	fmul	v4.2s, v4.2s, v3.s[0]
	fmul	s3, s3, s17
	fadd	s0, s0, s1
	ldr	d1, [sp, #32]                   // 8-byte Folded Reload
	fsub	v20.2s, v5.2s, v4.2s
	fadd	v4.2s, v5.2s, v4.2s
	fsub	v5.2s, v18.2s, v19.2s
	fadd	s2, s2, s3
	fmov	s3, #-3.00000000
	mov	v20.s[1], v4.s[1]
	ldr	d4, [sp, #64]                   // 8-byte Folded Reload
	mov	v5.s[1], v6.s[1]
	fadd	s2, s2, s3
	fmov	s3, #3.00000000
	fadd	v1.2s, v20.2s, v1.2s
	fadd	v4.2s, v5.2s, v4.2s
	fadd	s3, s0, s3
	fsub	s0, s2, s0
	fsub	v1.2s, v1.2s, v5.2s
	stur	d4, [x29, #-96]
	stur	s3, [x29, #-88]
	stur	s0, [x29, #-76]
	stur	d1, [x29, #-84]
	bl	_Z5colorRK3rayP7hitablei
	ldur	s8, [x29, #-100]                // 4-byte Folded Reload
	fadd	s14, s14, s0
	fadd	s13, s13, s1
	add	w23, w23, #1
	cmp	w23, #10
	fadd	s8, s8, s2
	b.ne	.LBB9_4
// %bb.7:                               //   in Loop: Header=BB9_3 Depth=2
	mov	w8, #52429
	movk	w8, #15820, lsl #16
	fmov	s15, w8
	fmul	s0, s14, s15
	fsqrt	s9, s0
	fcmp	s9, s9
	b.vs	.LBB9_11
// %bb.8:                               //   in Loop: Header=BB9_3 Depth=2
	fmul	s0, s13, s15
	fsqrt	s13, s0
	fcmp	s13, s13
	b.vs	.LBB9_12
.LBB9_9:                                //   in Loop: Header=BB9_3 Depth=2
	fmul	s1, s8, s15
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vs	.LBB9_13
.LBB9_10:                               //   in Loop: Header=BB9_3 Depth=2
	fcvt	d0, s0
	ldr	d3, [sp, #16]                   // 8-byte Folded Reload
	fcvt	d1, s9
	fcvt	d2, s13
	adrp	x0, :got:_ZSt4cout
	fmul	d0, d0, d3
	fmul	d1, d1, d3
	fmul	d2, d2, d3
	fcvtzs	w8, d0
	fcvtzs	w1, d1
	fcvtzs	w23, d2
	stur	w8, [x29, #-100]                // 4-byte Folded Spill
	ldr	x0, [x0, :got_lo12:_ZSt4cout]
	bl	_ZNSolsEi
	adrp	x1, .L.str.1
	mov	w2, #1
	add	x1, x1, :lo12:.L.str.1
	mov	x25, x0
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	mov	x0, x25
	mov	w1, w23
	bl	_ZNSolsEi
	adrp	x1, .L.str.1
	mov	w2, #1
	add	x1, x1, :lo12:.L.str.1
	mov	x23, x0
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	mov	x0, x23
	ldur	w1, [x29, #-100]                // 4-byte Folded Reload
	bl	_ZNSolsEi
	adrp	x1, .L.str.3
	mov	w2, #1
	add	x1, x1, :lo12:.L.str.3
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	fmov	d0, #1.00000000
	ldr	d1, [sp, #96]                   // 8-byte Folded Reload
	ldr	w8, [sp, #28]                   // 4-byte Folded Reload
	mov	w25, #1150681088
	fadd	d1, d1, d0
	add	w8, w8, #1
	cmp	w8, #1200
	str	d1, [sp, #96]                   // 8-byte Folded Spill
	b.ne	.LBB9_3
	b	.LBB9_1
.LBB9_11:                               //   in Loop: Header=BB9_3 Depth=2
	bl	sqrtf
	fmov	s9, s0
	fmul	s0, s13, s15
	fsqrt	s13, s0
	fcmp	s13, s13
	b.vc	.LBB9_9
.LBB9_12:                               //   in Loop: Header=BB9_3 Depth=2
	bl	sqrtf
	fmov	s13, s0
	fmul	s1, s8, s15
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vc	.LBB9_10
.LBB9_13:                               //   in Loop: Header=BB9_3 Depth=2
	fmov	s0, s1
	bl	sqrtf
	b	.LBB9_10
.LBB9_14:
	ldp	x20, x19, [sp, #288]            // 16-byte Folded Reload
	mov	w0, wzr
	ldp	x22, x21, [sp, #272]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #256]            // 16-byte Folded Reload
	ldp	x26, x25, [sp, #240]            // 16-byte Folded Reload
	ldp	x28, x27, [sp, #224]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #208]            // 16-byte Folded Reload
	ldp	d9, d8, [sp, #192]              // 16-byte Folded Reload
	ldp	d11, d10, [sp, #176]            // 16-byte Folded Reload
	ldp	d13, d12, [sp, #160]            // 16-byte Folded Reload
	ldp	d15, d14, [sp, #144]            // 16-byte Folded Reload
	add	sp, sp, #304
	ret
.Lfunc_end9:
	.size	main, .Lfunc_end9-main
	.cfi_endproc
                                        // -- End function
	.section	.text._ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_,"axG",@progbits,_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_,comdat
	.weak	_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_ // -- Begin function _ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_
	.p2align	2
	.type	_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_,@function
_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_: // @_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #160
	stp	d15, d14, [sp, #16]             // 16-byte Folded Spill
	stp	d13, d12, [sp, #32]             // 16-byte Folded Spill
	stp	d11, d10, [sp, #48]             // 16-byte Folded Spill
	stp	d9, d8, [sp, #64]               // 16-byte Folded Spill
	stp	x29, x30, [sp, #80]             // 16-byte Folded Spill
	add	x29, sp, #80
	str	x25, [sp, #96]                  // 8-byte Folded Spill
	stp	x24, x23, [sp, #112]            // 16-byte Folded Spill
	stp	x22, x21, [sp, #128]            // 16-byte Folded Spill
	stp	x20, x19, [sp, #144]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 80
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w25, -64
	.cfi_offset w30, -72
	.cfi_offset w29, -80
	.cfi_offset b8, -88
	.cfi_offset b9, -96
	.cfi_offset b10, -104
	.cfi_offset b11, -112
	.cfi_offset b12, -120
	.cfi_offset b13, -128
	.cfi_offset b14, -136
	.cfi_offset b15, -144
	mov	x24, x2
	mov	x25, #4467570830351532032
	fmov	v12.2s, #-1.00000000
	mov	x20, x4
	mov	x19, x3
	mov	x22, x2
	mov	x21, x0
	fmov	s13, #-1.00000000
	ldr	d8, [x24, #4]!
	ldur	d9, [x24, #12]
	fmov	s14, #1.00000000
	ldr	s10, [x24, #8]
	dup	v0.2d, x25
	ldr	s11, [x24, #20]
	str	q0, [sp]                        // 16-byte Folded Spill
.LBB10_1:                               // =>This Inner Loop Header: Depth=1
	bl	rand
	mov	w23, w0
	bl	rand
	fmov	s0, w23
	ldr	q1, [sp]                        // 16-byte Folded Reload
	mov	v0.s[1], w0
	sshll	v0.2d, v0.2s, #0
	scvtf	v0.2d, v0.2d
	fmul	v0.2d, v0.2d, v1.2d
	fcvtn	v15.2s, v0.2d
	bl	rand
	fadd	v0.2s, v15.2s, v15.2s
	scvtf	d1, w0
	fmov	d2, x25
	fmul	d1, d1, d2
	fadd	v0.2s, v0.2s, v12.2s
	fcvt	s1, d1
	fmul	v2.2s, v0.2s, v0.2s
	fadd	s1, s1, s1
	mov	s2, v2.s[1]
	fadd	s1, s1, s13
	fmla	s2, s0, v0.s[0]
	fmadd	s2, s1, s1, s2
	fcmp	s2, s14
	b.ge	.LBB10_1
// %bb.2:
	fadd	v2.2s, v8.2s, v9.2s
	fadd	s3, s10, s11
	ldr	w8, [x24, #8]
	mov	w0, #1
	ldr	x9, [x24]
	ldp	x24, x23, [sp, #112]            // 16-byte Folded Reload
	fadd	v0.2s, v2.2s, v0.2s
	fadd	s1, s3, s1
	ldur	d2, [x22, #4]
	ldr	s3, [x22, #12]
	str	w8, [x20, #8]
	str	x9, [x20]
	ldr	x25, [sp, #96]                  // 8-byte Folded Reload
	fsub	v0.2s, v0.2s, v2.2s
	fsub	s1, s1, s3
	ldp	x29, x30, [sp, #80]             // 16-byte Folded Reload
	stur	d0, [x20, #12]
	str	s1, [x20, #20]
	ldr	w8, [x21, #16]
	ldr	x9, [x21, #8]
	ldp	x22, x21, [sp, #128]            // 16-byte Folded Reload
	str	w8, [x19, #8]
	str	x9, [x19]
	ldp	x20, x19, [sp, #144]            // 16-byte Folded Reload
	ldp	d9, d8, [sp, #64]               // 16-byte Folded Reload
	ldp	d11, d10, [sp, #48]             // 16-byte Folded Reload
	ldp	d13, d12, [sp, #32]             // 16-byte Folded Reload
	ldp	d15, d14, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #160
	ret
.Lfunc_end10:
	.size	_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_, .Lfunc_end10-_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_,"axG",@progbits,_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_,comdat
	.weak	_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_ // -- Begin function _ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_
	.p2align	2
	.type	_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_,@function
_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_: // @_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #176
	stp	d13, d12, [sp, #64]             // 16-byte Folded Spill
	stp	d11, d10, [sp, #80]             // 16-byte Folded Spill
	stp	d9, d8, [sp, #96]               // 16-byte Folded Spill
	stp	x29, x30, [sp, #112]            // 16-byte Folded Spill
	add	x29, sp, #112
	stp	x24, x23, [sp, #128]            // 16-byte Folded Spill
	stp	x22, x21, [sp, #144]            // 16-byte Folded Spill
	stp	x20, x19, [sp, #160]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 64
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w30, -56
	.cfi_offset w29, -64
	.cfi_offset b8, -72
	.cfi_offset b9, -80
	.cfi_offset b10, -88
	.cfi_offset b11, -96
	.cfi_offset b12, -104
	.cfi_offset b13, -112
	ldur	d2, [x1, #12]
	mov	x19, x4
	ldr	s8, [x1, #20]
	mov	x21, x3
	mov	x20, x2
	mov	x22, x0
	fmul	v0.2s, v2.2s, v2.2s
	mov	s0, v0.s[1]
	fmla	s0, s2, v2.s[0]
	fmadd	s1, s8, s8, s0
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vs	.LBB11_4
.LBB11_1:
	dup	v1.2s, v0.s[0]
	fdiv	s8, s8, s0
	ldr	d0, [x20, #16]
	mov	x24, #4467570830351532032
	fmov	v10.2s, #-1.00000000
	ldr	s9, [x20, #24]
	fmov	s11, #-1.00000000
	fmov	s12, #1.00000000
	str	q0, [sp]                        // 16-byte Folded Spill
	ldr	s0, [x22, #20]
	fdiv	v1.2s, v2.2s, v1.2s
	stp	q0, q1, [sp, #16]               // 32-byte Folded Spill
	dup	v0.2d, x24
	str	q0, [sp, #48]                   // 16-byte Folded Spill
.LBB11_2:                               // =>This Inner Loop Header: Depth=1
	bl	rand
	mov	w23, w0
	bl	rand
	fmov	s0, w23
	ldr	q1, [sp, #48]                   // 16-byte Folded Reload
	mov	v0.s[1], w0
	sshll	v0.2d, v0.2s, #0
	scvtf	v0.2d, v0.2d
	fmul	v0.2d, v0.2d, v1.2d
	fcvtn	v13.2s, v0.2d
	bl	rand
	fadd	v0.2s, v13.2s, v13.2s
	scvtf	d1, w0
	fmov	d2, x24
	fmul	d1, d1, d2
	fadd	v0.2s, v0.2s, v10.2s
	fcvt	s1, d1
	fmul	v2.2s, v0.2s, v0.2s
	fadd	s1, s1, s1
	mov	s2, v2.s[1]
	fadd	s1, s1, s11
	fmla	s2, s0, v0.s[0]
	fmadd	s2, s1, s1, s2
	fcmp	s2, s12
	b.ge	.LBB11_2
// %bb.3:
	ldp	q5, q4, [sp, #16]               // 32-byte Folded Reload
	fmul	v0.2s, v0.2s, v5.s[0]
	fmul	s1, s5, s1
	ldr	q3, [sp]                        // 16-byte Folded Reload
	ldr	w8, [x20, #12]
	ldur	x9, [x20, #4]
	fmul	v2.2s, v4.2s, v3.2s
	str	w8, [x19, #8]
	str	x9, [x19]
	ldp	x24, x23, [sp, #128]            // 16-byte Folded Reload
	mov	s2, v2.s[1]
	ldp	x29, x30, [sp, #112]            // 16-byte Folded Reload
	ldp	d11, d10, [sp, #80]             // 16-byte Folded Reload
	fmla	s2, s3, v4.s[0]
	ldp	d13, d12, [sp, #64]             // 16-byte Folded Reload
	fmadd	s2, s8, s9, s2
	fadd	s2, s2, s2
	fmul	v3.2s, v3.2s, v2.s[0]
	fmul	s2, s9, s2
	fsub	v3.2s, v4.2s, v3.2s
	fsub	s2, s8, s2
	ldp	d9, d8, [sp, #96]               // 16-byte Folded Reload
	fadd	v0.2s, v3.2s, v0.2s
	fadd	s1, s2, s1
	stur	d0, [x19, #12]
	str	s1, [x19, #20]
	ldr	w8, [x22, #16]
	ldr	x9, [x22, #8]
	str	w8, [x21, #8]
	str	x9, [x21]
	ldp	s2, s0, [x19, #12]
	ldr	s1, [x20, #20]
	ldr	s3, [x19, #20]
	ldp	x22, x21, [sp, #144]            // 16-byte Folded Reload
	fmul	s0, s0, s1
	ldr	s1, [x20, #16]
	fmadd	s0, s2, s1, s0
	ldr	s1, [x20, #24]
	ldp	x20, x19, [sp, #160]            // 16-byte Folded Reload
	fmadd	s0, s3, s1, s0
	fcmp	s0, #0.0
	cset	w0, gt
	add	sp, sp, #176
	ret
.LBB11_4:
	fmov	s0, s1
	str	q2, [sp, #48]                   // 16-byte Folded Spill
	bl	sqrtf
	ldr	q2, [sp, #48]                   // 16-byte Folded Reload
                                        // kill: def $s0 killed $s0 def $q0
	b	.LBB11_1
.Lfunc_end11:
	.size	_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_, .Lfunc_end11-_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_,"axG",@progbits,_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_,comdat
	.weak	_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_ // -- Begin function _ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_
	.p2align	2
	.type	_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_,@function
_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_: // @_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #144
	stp	d15, d14, [sp, #48]             // 16-byte Folded Spill
	stp	d13, d12, [sp, #64]             // 16-byte Folded Spill
	stp	d11, d10, [sp, #80]             // 16-byte Folded Spill
	stp	d9, d8, [sp, #96]               // 16-byte Folded Spill
	stp	x29, x30, [sp, #112]            // 16-byte Folded Spill
	add	x29, sp, #112
	stp	x20, x19, [sp, #128]            // 16-byte Folded Spill
	.cfi_def_cfa w29, 32
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w30, -24
	.cfi_offset w29, -32
	.cfi_offset b8, -40
	.cfi_offset b9, -48
	.cfi_offset b10, -56
	.cfi_offset b11, -64
	.cfi_offset b12, -72
	.cfi_offset b13, -80
	.cfi_offset b14, -88
	.cfi_offset b15, -96
	fmov	v0.2s, #1.00000000
	mov	w8, #1065353216
	ldp	s20, s21, [x1, #12]
	ldr	s22, [x1, #20]
	mov	x19, x4
	ldp	s23, s12, [x2, #16]
	ldr	s13, [x2, #24]
	str	d0, [x3]
	str	w8, [x3, #8]
	mov	x20, x2
	ldp	s2, s0, [x1, #12]
	ldp	s4, s3, [x2, #16]
	ldr	s1, [x1, #20]
	fmul	s7, s21, s12
	fmul	s5, s0, s3
	fmul	s0, s0, s0
	fmadd	s7, s20, s23, s7
	fmadd	s6, s2, s4, s5
	fmadd	s0, s2, s2, s0
	ldr	s5, [x2, #24]
	fmadd	s8, s1, s5, s6
	fmadd	s1, s1, s1, s0
	fsqrt	s0, s1
	fcmp	s8, #0.0
	b.le	.LBB12_4
// %bb.1:
	fcmp	s0, s0
	ldr	s6, [x0, #8]
	b.vs	.LBB12_13
.LBB12_2:
	fdiv	s0, s8, s0
	ldr	s1, [x0, #8]
	fmov	s16, #1.00000000
	fneg	s4, s4
	fneg	s3, s3
	fneg	s5, s5
	fneg	s2, s1
	fmul	s1, s1, s2
	fmsub	s0, s0, s0, s16
	fmadd	s1, s1, s0, s16
	fsqrt	s0, s1
	fcmp	s0, s0
	b.vc	.LBB12_6
// %bb.3:
	fmov	s0, s1
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	str	x1, [sp, #8]                    // 8-byte Folded Spill
	fmov	s14, s4
	str	s3, [sp, #16]                   // 4-byte Folded Spill
	fmov	s9, s5
	fmov	s15, s6
	fmov	s10, s7
	fmov	s11, s20
	stp	s21, s23, [sp, #40]             // 8-byte Folded Spill
	fmov	s8, s22
	bl	sqrtf
	ldp	s21, s23, [sp, #40]             // 8-byte Folded Reload
	ldr	s3, [sp, #16]                   // 4-byte Folded Reload
	fmov	s22, s8
	ldr	x1, [sp, #8]                    // 8-byte Folded Reload
	fmov	s20, s11
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	fmov	s7, s10
	fmov	s6, s15
	fmov	s5, s9
	fmov	s4, s14
	b	.LBB12_6
.LBB12_4:
	fneg	s8, s8
	fcmp	s0, s0
	ldr	s9, [x0, #8]
	fmov	s10, #1.00000000
	b.vs	.LBB12_14
.LBB12_5:
	fdiv	s6, s10, s9
	fdiv	s0, s8, s0
.LBB12_6:
	ldp	s14, s9, [x1, #12]
	ldr	s10, [x1, #20]
	fmadd	s8, s22, s13, s7
	fmul	s1, s9, s9
	fmadd	s1, s14, s14, s1
	fmadd	s2, s10, s10, s1
	fsqrt	s1, s2
	fcmp	s1, s1
	b.vs	.LBB12_12
.LBB12_7:
	fdiv	s16, s9, s1
	fmul	s2, s6, s6
	fdiv	s7, s14, s1
	fadd	s14, s8, s8
	fdiv	s17, s10, s1
	fmul	s1, s3, s16
	fmadd	s1, s7, s4, s1
	fmadd	s18, s17, s5, s1
	fmov	s1, #1.00000000
	fmsub	s19, s18, s18, s1
	fmul	s2, s2, s19
	fsub	s2, s1, s2
	fcmp	s2, #0.0
	b.le	.LBB12_10
// %bb.8:
	fsqrt	s1, s2
	stp	s20, s22, [sp, #32]             // 8-byte Folded Spill
	fmul	s19, s4, s18
	fmul	s20, s3, s18
	fmul	s18, s5, s18
	stp	s21, s23, [sp, #40]             // 8-byte Folded Spill
	fsub	s7, s7, s19
	fsub	s16, s16, s20
	fsub	s17, s17, s18
	fmul	s8, s6, s7
	fmul	s10, s6, s16
	fmul	s15, s6, s17
	fcmp	s1, s1
	b.vs	.LBB12_15
.LBB12_9:
	ldr	s2, [x0, #8]
	fmov	s6, #1.00000000
	fmul	s4, s4, s1
	fmul	s3, s3, s1
	fmul	s1, s5, s1
	fsub	s7, s6, s2
	fadd	s2, s2, s6
	fsub	s0, s6, s0
	fsub	s9, s8, s4
	fsub	s8, s10, s3
	fsub	s15, s15, s1
	fmov	d1, #5.00000000
	fdiv	s2, s7, s2
	fcvt	d0, s0
	fmul	s2, s2, s2
	fsub	s5, s6, s2
	fcvt	d10, s2
	fcvt	d11, s5
	bl	pow
	fmadd	d0, d11, d0, d10
	ldp	s20, s22, [sp, #32]             // 8-byte Folded Reload
	ldp	s21, s23, [sp, #40]             // 8-byte Folded Reload
	fcvt	s0, d0
	fcvt	d10, s0
	b	.LBB12_11
.LBB12_10:
	fmov	d10, #1.00000000
                                        // implicit-def: $s15
                                        // implicit-def: $s8
                                        // implicit-def: $s9
.LBB12_11:
	fmul	s0, s13, s14
	fmul	s1, s12, s14
	fmul	s2, s23, s14
	fsub	s11, s22, s0
	fsub	s12, s21, s1
	fsub	s13, s20, s2
	bl	rand
	mov	x8, #4467570830351532032
	scvtf	d0, w0
	ldr	w9, [x20, #12]
	mov	w0, #1
	ldp	x29, x30, [sp, #112]            // 16-byte Folded Reload
	fmov	d1, x8
	ldur	x8, [x20, #4]
	str	w9, [x19, #8]
	fmul	d0, d0, d1
	str	x8, [x19]
	fcmp	d0, d10
	fcsel	s0, s13, s9, mi
	fcsel	s1, s12, s8, mi
	fcsel	s2, s11, s15, mi
	ldp	d9, d8, [sp, #96]               // 16-byte Folded Reload
	stp	s0, s1, [x19, #12]
	str	s2, [x19, #20]
	ldp	x20, x19, [sp, #128]            // 16-byte Folded Reload
	ldp	d11, d10, [sp, #80]             // 16-byte Folded Reload
	ldp	d13, d12, [sp, #64]             // 16-byte Folded Reload
	ldp	d15, d14, [sp, #48]             // 16-byte Folded Reload
	add	sp, sp, #144
	ret
.LBB12_12:
	str	s0, [sp, #8]                    // 4-byte Folded Spill
	fmov	s0, s2
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	fmov	s15, s3
	stp	s4, s5, [sp, #16]               // 8-byte Folded Spill
	fmov	s11, s20
	stp	s6, s22, [sp, #32]              // 8-byte Folded Spill
	stp	s21, s23, [sp, #40]             // 8-byte Folded Spill
	bl	sqrtf
	ldp	s21, s23, [sp, #40]             // 8-byte Folded Reload
	ldp	s6, s22, [sp, #32]              // 8-byte Folded Reload
	fmov	s20, s11
	fmov	s3, s15
	ldp	s4, s5, [sp, #16]               // 8-byte Folded Reload
	fmov	s1, s0
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	ldr	s0, [sp, #8]                    // 4-byte Folded Reload
	b	.LBB12_7
.LBB12_13:
	fmov	s0, s1
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	str	x1, [sp, #8]                    // 8-byte Folded Spill
	fmov	s15, s4
	stp	s3, s5, [sp, #16]               // 8-byte Folded Spill
	fmov	s9, s6
	fmov	s14, s7
	fmov	s11, s20
	stp	s21, s23, [sp, #40]             // 8-byte Folded Spill
	fmov	s10, s22
	bl	sqrtf
	ldp	s21, s23, [sp, #40]             // 8-byte Folded Reload
	fmov	s22, s10
	fmov	s20, s11
	fmov	s7, s14
	fmov	s6, s9
	ldp	s3, s5, [sp, #16]               // 8-byte Folded Reload
	fmov	s4, s15
	ldr	x1, [sp, #8]                    // 8-byte Folded Reload
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	b	.LBB12_2
.LBB12_14:
	fmov	s0, s1
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	str	x1, [sp, #8]                    // 8-byte Folded Spill
	fmov	s14, s4
	stp	s3, s5, [sp, #16]               // 8-byte Folded Spill
	fmov	s15, s7
	stp	s20, s22, [sp, #32]             // 8-byte Folded Spill
	fmov	s11, s21
	str	s23, [sp, #44]                  // 4-byte Folded Spill
	bl	sqrtf
	ldp	s20, s22, [sp, #32]             // 8-byte Folded Reload
	fmov	s21, s11
	fmov	s7, s15
	ldp	s3, s5, [sp, #16]               // 8-byte Folded Reload
	fmov	s4, s14
	ldr	s23, [sp, #44]                  // 4-byte Folded Reload
	ldr	x1, [sp, #8]                    // 8-byte Folded Reload
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	b	.LBB12_5
.LBB12_15:
	str	s0, [sp, #8]                    // 4-byte Folded Spill
	fmov	s0, s2
	str	x0, [sp, #24]                   // 8-byte Folded Spill
	fmov	s9, s4
	stp	s3, s5, [sp, #16]               // 8-byte Folded Spill
	bl	sqrtf
	ldp	s3, s5, [sp, #16]               // 8-byte Folded Reload
	fmov	s4, s9
	fmov	s1, s0
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	ldr	s0, [sp, #8]                    // 4-byte Folded Reload
	b	.LBB12_9
.Lfunc_end12:
	.size	_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_, .Lfunc_end12-_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_
	.cfi_endproc
                                        // -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	2                               // -- Begin function _GLOBAL__sub_I_main.cc
	.type	_GLOBAL__sub_I_main.cc,@function
_GLOBAL__sub_I_main.cc:                 // @_GLOBAL__sub_I_main.cc
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 32
	.cfi_offset w19, -16
	.cfi_offset w30, -24
	.cfi_offset w29, -32
	adrp	x19, _ZStL8__ioinit
	add	x19, x19, :lo12:_ZStL8__ioinit
	mov	x0, x19
	bl	_ZNSt8ios_base4InitC1Ev
	adrp	x0, :got:_ZNSt8ios_base4InitD1Ev
	mov	x1, x19
	adrp	x2, __dso_handle
	add	x2, x2, :lo12:__dso_handle
	ldr	x0, [x0, :got_lo12:_ZNSt8ios_base4InitD1Ev]
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	b	__cxa_atexit
.Lfunc_end13:
	.size	_GLOBAL__sub_I_main.cc, .Lfunc_end13-_GLOBAL__sub_I_main.cc
	.cfi_endproc
                                        // -- End function
	.type	_ZStL8__ioinit,@object          // @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"P3\n"
	.size	.L.str, 4

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	" "
	.size	.L.str.1, 2

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"\n255\n"
	.size	.L.str.2, 6

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"\n"
	.size	.L.str.3, 2

	.type	_ZTV6sphere,@object             // @_ZTV6sphere
	.section	.data.rel.ro,"aw",@progbits
	.globl	_ZTV6sphere
	.p2align	3
_ZTV6sphere:
	.xword	0
	.xword	_ZTI6sphere
	.xword	_ZNK6sphere3hitERK3rayffR10hit_record
	.size	_ZTV6sphere, 24

	.type	_ZTS6sphere,@object             // @_ZTS6sphere
	.section	.rodata,"a",@progbits
	.globl	_ZTS6sphere
_ZTS6sphere:
	.asciz	"6sphere"
	.size	_ZTS6sphere, 8

	.type	_ZTS7hitable,@object            // @_ZTS7hitable
	.section	.rodata._ZTS7hitable,"aG",@progbits,_ZTS7hitable,comdat
	.weak	_ZTS7hitable
_ZTS7hitable:
	.asciz	"7hitable"
	.size	_ZTS7hitable, 9

	.type	_ZTI7hitable,@object            // @_ZTI7hitable
	.section	.data.rel.ro._ZTI7hitable,"aGw",@progbits,_ZTI7hitable,comdat
	.weak	_ZTI7hitable
	.p2align	3
_ZTI7hitable:
	.xword	_ZTVN10__cxxabiv117__class_type_infoE+16
	.xword	_ZTS7hitable
	.size	_ZTI7hitable, 16

	.type	_ZTI6sphere,@object             // @_ZTI6sphere
	.section	.data.rel.ro,"aw",@progbits
	.globl	_ZTI6sphere
	.p2align	3
_ZTI6sphere:
	.xword	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.xword	_ZTS6sphere
	.xword	_ZTI7hitable
	.size	_ZTI6sphere, 24

	.type	_ZTV12hitable_list,@object      // @_ZTV12hitable_list
	.globl	_ZTV12hitable_list
	.p2align	3
_ZTV12hitable_list:
	.xword	0
	.xword	_ZTI12hitable_list
	.xword	_ZNK12hitable_list3hitERK3rayffR10hit_record
	.size	_ZTV12hitable_list, 24

	.type	_ZTS12hitable_list,@object      // @_ZTS12hitable_list
	.section	.rodata,"a",@progbits
	.globl	_ZTS12hitable_list
_ZTS12hitable_list:
	.asciz	"12hitable_list"
	.size	_ZTS12hitable_list, 15

	.type	_ZTI12hitable_list,@object      // @_ZTI12hitable_list
	.section	.data.rel.ro,"aw",@progbits
	.globl	_ZTI12hitable_list
	.p2align	3
_ZTI12hitable_list:
	.xword	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.xword	_ZTS12hitable_list
	.xword	_ZTI7hitable
	.size	_ZTI12hitable_list, 24

	.type	_ZTV10lambertian,@object        // @_ZTV10lambertian
	.section	.data.rel.ro._ZTV10lambertian,"aGw",@progbits,_ZTV10lambertian,comdat
	.weak	_ZTV10lambertian
	.p2align	3
_ZTV10lambertian:
	.xword	0
	.xword	_ZTI10lambertian
	.xword	_ZNK10lambertian7scatterERK3rayRK10hit_recordR4vec3RS0_
	.size	_ZTV10lambertian, 24

	.type	_ZTS10lambertian,@object        // @_ZTS10lambertian
	.section	.rodata._ZTS10lambertian,"aG",@progbits,_ZTS10lambertian,comdat
	.weak	_ZTS10lambertian
_ZTS10lambertian:
	.asciz	"10lambertian"
	.size	_ZTS10lambertian, 13

	.type	_ZTS8material,@object           // @_ZTS8material
	.section	.rodata._ZTS8material,"aG",@progbits,_ZTS8material,comdat
	.weak	_ZTS8material
_ZTS8material:
	.asciz	"8material"
	.size	_ZTS8material, 10

	.type	_ZTI8material,@object           // @_ZTI8material
	.section	.data.rel.ro._ZTI8material,"aGw",@progbits,_ZTI8material,comdat
	.weak	_ZTI8material
	.p2align	3
_ZTI8material:
	.xword	_ZTVN10__cxxabiv117__class_type_infoE+16
	.xword	_ZTS8material
	.size	_ZTI8material, 16

	.type	_ZTI10lambertian,@object        // @_ZTI10lambertian
	.section	.data.rel.ro._ZTI10lambertian,"aGw",@progbits,_ZTI10lambertian,comdat
	.weak	_ZTI10lambertian
	.p2align	3
_ZTI10lambertian:
	.xword	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.xword	_ZTS10lambertian
	.xword	_ZTI8material
	.size	_ZTI10lambertian, 24

	.type	_ZTV5metal,@object              // @_ZTV5metal
	.section	.data.rel.ro._ZTV5metal,"aGw",@progbits,_ZTV5metal,comdat
	.weak	_ZTV5metal
	.p2align	3
_ZTV5metal:
	.xword	0
	.xword	_ZTI5metal
	.xword	_ZNK5metal7scatterERK3rayRK10hit_recordR4vec3RS0_
	.size	_ZTV5metal, 24

	.type	_ZTS5metal,@object              // @_ZTS5metal
	.section	.rodata._ZTS5metal,"aG",@progbits,_ZTS5metal,comdat
	.weak	_ZTS5metal
_ZTS5metal:
	.asciz	"5metal"
	.size	_ZTS5metal, 7

	.type	_ZTI5metal,@object              // @_ZTI5metal
	.section	.data.rel.ro._ZTI5metal,"aGw",@progbits,_ZTI5metal,comdat
	.weak	_ZTI5metal
	.p2align	3
_ZTI5metal:
	.xword	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.xword	_ZTS5metal
	.xword	_ZTI8material
	.size	_ZTI5metal, 24

	.type	_ZTV10dielectric,@object        // @_ZTV10dielectric
	.section	.data.rel.ro._ZTV10dielectric,"aGw",@progbits,_ZTV10dielectric,comdat
	.weak	_ZTV10dielectric
	.p2align	3
_ZTV10dielectric:
	.xword	0
	.xword	_ZTI10dielectric
	.xword	_ZNK10dielectric7scatterERK3rayRK10hit_recordR4vec3RS0_
	.size	_ZTV10dielectric, 24

	.type	_ZTS10dielectric,@object        // @_ZTS10dielectric
	.section	.rodata._ZTS10dielectric,"aG",@progbits,_ZTS10dielectric,comdat
	.weak	_ZTS10dielectric
_ZTS10dielectric:
	.asciz	"10dielectric"
	.size	_ZTS10dielectric, 13

	.type	_ZTI10dielectric,@object        // @_ZTI10dielectric
	.section	.data.rel.ro._ZTI10dielectric,"aGw",@progbits,_ZTI10dielectric,comdat
	.weak	_ZTI10dielectric
	.p2align	3
_ZTI10dielectric:
	.xword	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.xword	_ZTS10dielectric
	.xword	_ZTI8material
	.size	_ZTI10dielectric, 24

	.section	.init_array,"aw",@init_array
	.p2align	3
	.xword	_GLOBAL__sub_I_main.cc
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.DW.ref.__gxx_personality_v0,"aGw",@progbits,DW.ref.__gxx_personality_v0,comdat
	.p2align	3
	.type	DW.ref.__gxx_personality_v0,@object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.xword	__gxx_personality_v0
	.ident	"Ubuntu clang version 14.0.0-1ubuntu1.1"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym _GLOBAL__sub_I_main.cc
	.addrsig_sym _Unwind_Resume
	.addrsig_sym _ZStL8__ioinit
	.addrsig_sym __dso_handle
	.addrsig_sym _ZSt4cout
	.addrsig_sym _ZTVN10__cxxabiv120__si_class_type_infoE
	.addrsig_sym _ZTS6sphere
	.addrsig_sym _ZTVN10__cxxabiv117__class_type_infoE
	.addrsig_sym _ZTS7hitable
	.addrsig_sym _ZTI7hitable
	.addrsig_sym _ZTI6sphere
	.addrsig_sym _ZTS12hitable_list
	.addrsig_sym _ZTI12hitable_list
	.addrsig_sym _ZTS10lambertian
	.addrsig_sym _ZTS8material
	.addrsig_sym _ZTI8material
	.addrsig_sym _ZTI10lambertian
	.addrsig_sym _ZTS5metal
	.addrsig_sym _ZTI5metal
	.addrsig_sym _ZTS10dielectric
	.addrsig_sym _ZTI10dielectric
