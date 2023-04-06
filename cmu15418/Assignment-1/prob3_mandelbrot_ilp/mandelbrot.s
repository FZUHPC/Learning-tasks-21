	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 3
	.globl	__Z10mandel_refffi              ; -- Begin function _Z10mandel_refffi
	.p2align	2
__Z10mandel_refffi:                     ; @_Z10mandel_refffi
	.cfi_startproc
; %bb.0:
	cmp	w0, #1
	b.lt	LBB0_6
; %bb.1:
	mov	x8, x0
	mov	w0, #0
	fmov	s2, #4.00000000
	fmov	s3, s0
	fmov	s4, s1
LBB0_2:                                 ; =>This Inner Loop Header: Depth=1
	fmul	s5, s4, s4
	fmadd	s6, s3, s3, s5
	fcmp	s6, s2
	b.gt	LBB0_5
; %bb.3:                                ;   in Loop: Header=BB0_2 Depth=1
	fnmsub	s5, s3, s3, s5
	fadd	s3, s3, s3
	fmul	s4, s4, s3
	fadd	s3, s5, s0
	fadd	s4, s4, s1
	add	w0, w0, #1
	cmp	w8, w0
	b.ne	LBB0_2
; %bb.4:
	mov	x0, x8
LBB0_5:
	ret
LBB0_6:
	mov	w0, #0
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi1EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi1EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi1EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi1EEvPfS0_iPi:         ; @_Z10mandel_parILi1EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	cmp	w2, #1
	b.lt	LBB1_7
; %bb.1:
	mov	w8, #0
	mov	w9, #0
	mov	w10, #0
	ldr	s0, [x0]
	ldr	s1, [x1]
	fmov	s2, #4.00000000
	fmov	s3, s1
	fmov	s4, s0
LBB1_2:                                 ; =>This Inner Loop Header: Depth=1
	fmul	s5, s3, s3
	fmadd	s5, s4, s4, s5
	fcmp	s5, s2
	cset	w11, gt
	cmp	w10, #0
	csel	w9, w8, w9, eq
	orr	w11, w10, w11
	tbnz	w11, #0, LBB1_5
; %bb.3:                                ;   in Loop: Header=BB1_2 Depth=1
	and	w10, w11, #0xff
	fneg	s5, s3
	fmul	s5, s3, s5
	fmadd	s5, s4, s4, s5
	fadd	s4, s4, s4
	fmul	s3, s4, s3
	fadd	s4, s0, s5
	fadd	s3, s1, s3
	add	w8, w8, #1
	cmp	w2, w8
	b.ne	LBB1_2
; %bb.4:
	mov	x8, x2
LBB1_5:
	cbz	w11, LBB1_8
; %bb.6:
	str	w9, [x3]
	ret
LBB1_7:
	mov	w8, #0
LBB1_8:
	mov	x9, x8
	str	w9, [x3]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi2EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi2EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi2EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi2EEvPfS0_iPi:         ; @_Z10mandel_parILi2EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	cmp	w2, #1
	b.lt	LBB2_6
; %bb.1:
	mov	w8, #0
	ldr	d1, [x0]
	ldr	d2, [x1]
	movi.2d	v3, #0000000000000000
	movi	d4, #0x0000ff000000ff
	fmov.2s	v5, #4.00000000
	movi.2s	v6, #1
	fmov	d7, d1
	fmov	d16, d2
	movi.2d	v0, #0000000000000000
LBB2_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v3, v3, v4
	fmul.2s	v17, v16, v16
	cmeq.2s	v18, v3, #0
	dup.2s	v19, w8
	bit.8b	v0, v19, v18
	fmla.2s	v17, v7, v7
	fcmgt.2s	v17, v17, v5
	and.8b	v17, v17, v6
	orr.8b	v3, v17, v3
	fmov	w9, s3
	mov.s	w10, v3[1]
	and	w9, w9, w10
	tbnz	w9, #0, LBB2_5
; %bb.3:                                ;   in Loop: Header=BB2_2 Depth=1
	fneg.2s	v17, v16
	fadd.2s	v18, v7, v7
	fmul.2s	v17, v16, v17
	fmla.2s	v17, v7, v7
	fmul.2s	v16, v18, v16
	fadd.2s	v7, v1, v17
	fadd.2s	v16, v16, v2
	add	w8, w8, #1
	cmp	w2, w8
	b.ne	LBB2_2
; %bb.4:
	mov	x8, x2
LBB2_5:
	cmeq.2s	v1, v3, #0
	mov.s	w9, v1[1]
	fmov	w10, s1
	fmov	w11, s0
	tst	w10, #0x1
	csel	w10, w8, w11, ne
	str	w10, [x3]
	mov.s	w10, v0[1]
	tst	w9, #0x1
	csel	w8, w8, w10, ne
	str	w8, [x3, #4]
	ret
LBB2_6:
	str	xzr, [x3]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi3EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi3EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi3EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi3EEvPfS0_iPi:         ; @_Z10mandel_parILi3EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	cmp	w2, #1
	b.lt	LBB3_6
; %bb.1:
	mov	w10, #0
	mov	w8, #0
	mov	w9, #0
	ldr	d0, [x0]
	ldr	s1, [x0, #8]
	ldr	d2, [x1]
	ldr	s3, [x1, #8]
	movi.2d	v4, #0000000000000000
	movi	d5, #0x0000ff000000ff
	fmov.2s	v6, #4.00000000
	movi.2s	v7, #1
	fmov	s17, #4.00000000
	fmov	s18, s1
	fmov	s19, s3
	fmov	d21, d0
	fmov	d20, d2
	movi.2d	v16, #0000000000000000
LBB3_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v4, v4, v5
	cmeq.2s	v22, v4, #0
	dup.2s	v23, w9
	fmul.2s	v24, v20, v20
	bit.8b	v16, v23, v22
	fmla.2s	v24, v21, v21
	fcmgt.2s	v22, v24, v6
	and.8b	v22, v22, v7
	orr.8b	v4, v22, v4
	fmov	w11, s4
	mov.s	w12, v4[1]
	and	w12, w11, w12
	fmul	s22, s19, s19
	fmadd	s22, s18, s18, s22
	fcmp	s22, s17
	cset	w11, gt
	orr	w11, w11, w10
	and	w12, w12, w11
	cmp	w10, #0
	csel	w8, w9, w8, eq
	tbnz	w12, #0, LBB3_5
; %bb.3:                                ;   in Loop: Header=BB3_2 Depth=1
	and	w10, w11, #0xff
	fneg.2s	v22, v20
	fmul.2s	v22, v20, v22
	fmla.2s	v22, v21, v21
	fadd.2s	v21, v21, v21
	fadd.2s	v22, v0, v22
	fmul.2s	v20, v21, v20
	fadd.2s	v20, v20, v2
	fneg	s21, s19
	fmul	s21, s19, s21
	fmadd	s21, s18, s18, s21
	fadd	s18, s18, s18
	fmul	s19, s18, s19
	fadd	s18, s1, s21
	fadd	s19, s19, s3
	add	w9, w9, #1
	fmov	d21, d22
	cmp	w2, w9
	b.ne	LBB3_2
; %bb.4:
	mov	x9, x2
LBB3_5:
	cmeq.2s	v0, v4, #0
	dup.2s	v1, w9
	bsl.8b	v0, v1, v16
	str	d0, [x3]
	cmp	w11, #0
	csel	w8, w9, w8, eq
	str	w8, [x3, #8]
	ret
LBB3_6:
	str	xzr, [x3]
	str	wzr, [x3, #8]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi4EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi4EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi4EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi4EEvPfS0_iPi:         ; @_Z10mandel_parILi4EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	cmp	w2, #1
	b.lt	LBB4_6
; %bb.1:
	mov	w8, #0
	ldp	d1, d2, [x0]
	movi.2d	v5, #0000000000000000
	ldp	d3, d4, [x1]
	movi	d7, #0x0000ff000000ff
	fmov.2s	v16, #4.00000000
	movi.2s	v17, #1
	fmov	d22, d1
	fmov	d19, d3
	movi.2d	v6, #0000000000000000
	fmov	d20, d2
	fmov	d21, d4
	movi.2d	v18, #0000000000000000
	movi.2d	v0, #0000000000000000
LBB4_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v5, v5, v7
	cmeq.2s	v23, v5, #0
	fmul.2s	v24, v19, v19
	dup.2s	v25, w8
	bit.8b	v6, v25, v23
	fmla.2s	v24, v22, v22
	fcmgt.2s	v23, v24, v16
	and.8b	v23, v23, v17
	orr.8b	v5, v23, v5
	fmov	w9, s5
	mov.s	w10, v5[1]
	and	w9, w9, w10
	and.8b	v23, v18, v7
	fmul.2s	v18, v21, v21
	fmla.2s	v18, v20, v20
	fcmgt.2s	v18, v18, v16
	and.8b	v18, v18, v17
	orr.8b	v18, v18, v23
	fmov	w10, s18
	and	w9, w9, w10
	cmeq.2s	v23, v23, #0
	bit.8b	v0, v25, v23
	mov.s	w10, v18[1]
	and	w9, w9, w10
	tbnz	w9, #0, LBB4_5
; %bb.3:                                ;   in Loop: Header=BB4_2 Depth=1
	fneg.2s	v23, v19
	fmul.2s	v23, v19, v23
	fmla.2s	v23, v22, v22
	fadd.2s	v22, v22, v22
	fadd.2s	v23, v1, v23
	fmul.2s	v19, v22, v19
	fadd.2s	v19, v19, v3
	fneg.2s	v22, v21
	fadd.2s	v24, v20, v20
	fmul.2s	v22, v21, v22
	fmla.2s	v22, v20, v20
	fmul.2s	v21, v24, v21
	fadd.2s	v20, v2, v22
	fadd.2s	v21, v21, v4
	add	w8, w8, #1
	fmov	d22, d23
	cmp	w2, w8
	b.ne	LBB4_2
; %bb.4:
	mov	x8, x2
LBB4_5:
	cmeq.2s	v1, v5, #0
	dup.2s	v2, w8
	bsl.8b	v1, v2, v6
	str	d1, [x3]
	cmeq.2s	v1, v18, #0
	mov.s	w9, v1[1]
	fmov	w10, s1
	fmov	w11, s0
	tst	w10, #0x1
	csel	w10, w8, w11, ne
	str	w10, [x3, #8]
	mov.s	w10, v0[1]
	tst	w9, #0x1
	csel	w8, w8, w10, ne
	str	w8, [x3, #12]
	ret
LBB4_6:
	stp	xzr, xzr, [x3]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi5EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi5EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi5EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi5EEvPfS0_iPi:         ; @_Z10mandel_parILi5EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	cmp	w2, #1
	b.lt	LBB5_6
; %bb.1:
	mov	w10, #0
	mov	w8, #0
	mov	w9, #0
	ldp	d0, d1, [x0]
	ldr	s2, [x0, #16]
	ldp	d3, d4, [x1]
	ldr	s5, [x1, #16]
	movi.2d	v6, #0000000000000000
	movi	d7, #0x0000ff000000ff
	fmov.2s	v16, #4.00000000
	movi.2s	v17, #1
	fmov	s19, #4.00000000
	fmov	s21, s2
	fmov	s22, s5
	fmov	d26, d0
	fmov	d24, d3
	movi.2d	v18, #0000000000000000
	fmov	d27, d1
	fmov	d25, d4
	movi.2d	v23, #0000000000000000
	movi.2d	v20, #0000000000000000
LBB5_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v6, v6, v7
	cmeq.2s	v28, v6, #0
	fmul.2s	v29, v24, v24
	dup.2s	v30, w9
	bit.8b	v18, v30, v28
	fmla.2s	v29, v26, v26
	fcmgt.2s	v28, v29, v16
	and.8b	v28, v28, v17
	orr.8b	v6, v28, v6
	fmov	w11, s6
	mov.s	w12, v6[1]
	and	w11, w11, w12
	and.8b	v28, v23, v7
	fmul.2s	v23, v25, v25
	fmla.2s	v23, v27, v27
	fcmgt.2s	v23, v23, v16
	and.8b	v23, v23, v17
	orr.8b	v23, v23, v28
	fmov	w12, s23
	and	w11, w11, w12
	cmeq.2s	v28, v28, #0
	bit.8b	v20, v30, v28
	mov.s	w12, v23[1]
	and	w12, w11, w12
	fmul	s28, s22, s22
	fmadd	s28, s21, s21, s28
	fcmp	s28, s19
	cset	w11, gt
	orr	w11, w11, w10
	and	w12, w12, w11
	cmp	w10, #0
	csel	w8, w9, w8, eq
	tbnz	w12, #0, LBB5_5
; %bb.3:                                ;   in Loop: Header=BB5_2 Depth=1
	and	w10, w11, #0xff
	fneg.2s	v28, v24
	fmul.2s	v28, v24, v28
	fmla.2s	v28, v26, v26
	fadd.2s	v26, v26, v26
	fadd.2s	v28, v0, v28
	fmul.2s	v24, v26, v24
	fadd.2s	v24, v24, v3
	fneg.2s	v26, v25
	fmul.2s	v26, v25, v26
	fmla.2s	v26, v27, v27
	fadd.2s	v27, v27, v27
	fadd.2s	v29, v1, v26
	fmul.2s	v25, v27, v25
	fadd.2s	v25, v25, v4
	fneg	s26, s22
	fmul	s26, s22, s26
	fmadd	s26, s21, s21, s26
	fadd	s21, s21, s21
	fmul	s22, s21, s22
	fadd	s21, s2, s26
	fadd	s22, s22, s5
	add	w9, w9, #1
	fmov	d26, d28
	fmov	d27, d29
	cmp	w2, w9
	b.ne	LBB5_2
; %bb.4:
	mov	x9, x2
LBB5_5:
	cmeq.2s	v0, v6, #0
	dup.2s	v1, w9
	bsl.8b	v0, v1, v18
	cmeq.2s	v2, v23, #0
	bif.8b	v1, v20, v2
	stp	d0, d1, [x3]
	cmp	w11, #0
	csel	w8, w9, w8, eq
	str	w8, [x3, #16]
	ret
LBB5_6:
	stp	xzr, xzr, [x3]
	str	wzr, [x3, #16]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi6EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi6EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi6EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi6EEvPfS0_iPi:         ; @_Z10mandel_parILi6EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	cmp	w2, #1
	b.lt	LBB6_6
; %bb.1:
	mov	w8, #0
	ldp	d0, d2, [x0]
	ldr	d3, [x0, #16]
	ldp	d4, d5, [x1]
	ldr	d6, [x1, #16]
	movi.2d	v7, #0000000000000000
	movi	d18, #0x0000ff000000ff
	fmov.2s	v19, #4.00000000
	movi.2s	v20, #1
	fmov	d27, d0
	fmov	d23, d4
	movi.2d	v16, #0000000000000000
	fmov	d28, d2
	fmov	d24, d5
	movi.2d	v21, #0000000000000000
	movi.2d	v17, #0000000000000000
	fmov	d25, d3
	fmov	d26, d6
	movi.2d	v22, #0000000000000000
	movi.2d	v1, #0000000000000000
LBB6_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v7, v7, v18
	cmeq.2s	v29, v7, #0
	dup.2s	v30, w8
	fmul.2s	v31, v23, v23
	bit.8b	v16, v30, v29
	fmla.2s	v31, v27, v27
	fcmgt.2s	v29, v31, v19
	and.8b	v29, v29, v20
	orr.8b	v7, v29, v7
	fmov	w9, s7
	mov.s	w10, v7[1]
	and	w9, w9, w10
	and.8b	v29, v21, v18
	fmul.2s	v21, v24, v24
	fmla.2s	v21, v28, v28
	fcmgt.2s	v21, v21, v19
	and.8b	v21, v21, v20
	orr.8b	v21, v21, v29
	fmov	w10, s21
	and	w9, w9, w10
	cmeq.2s	v29, v29, #0
	bit.8b	v17, v30, v29
	mov.s	w10, v21[1]
	and	w9, w9, w10
	and.8b	v29, v22, v18
	fmul.2s	v22, v26, v26
	fmla.2s	v22, v25, v25
	fcmgt.2s	v22, v22, v19
	and.8b	v22, v22, v20
	orr.8b	v22, v22, v29
	fmov	w10, s22
	and	w9, w9, w10
	cmeq.2s	v29, v29, #0
	bit.8b	v1, v30, v29
	mov.s	w10, v22[1]
	and	w9, w9, w10
	tbnz	w9, #0, LBB6_5
; %bb.3:                                ;   in Loop: Header=BB6_2 Depth=1
	fneg.2s	v29, v23
	fmul.2s	v29, v23, v29
	fmla.2s	v29, v27, v27
	fadd.2s	v27, v27, v27
	fadd.2s	v29, v0, v29
	fmul.2s	v23, v27, v23
	fadd.2s	v23, v23, v4
	fneg.2s	v27, v24
	fmul.2s	v27, v24, v27
	fmla.2s	v27, v28, v28
	fadd.2s	v28, v28, v28
	fadd.2s	v30, v2, v27
	fmul.2s	v24, v28, v24
	fadd.2s	v24, v24, v5
	fneg.2s	v27, v26
	fadd.2s	v28, v25, v25
	fmul.2s	v27, v26, v27
	fmla.2s	v27, v25, v25
	fmul.2s	v26, v28, v26
	fadd.2s	v25, v3, v27
	fadd.2s	v26, v26, v6
	add	w8, w8, #1
	fmov	d27, d29
	fmov	d28, d30
	cmp	w2, w8
	b.ne	LBB6_2
; %bb.4:
	mov	x8, x2
LBB6_5:
	dup.2s	v0, w8
	cmeq.2s	v2, v7, #0
	bsl.8b	v2, v0, v16
	str	d2, [x3]
	cmeq.2s	v2, v21, #0
	bif.8b	v0, v17, v2
	str	d0, [x3, #8]
	cmeq.2s	v0, v22, #0
	mov.s	w9, v0[1]
	fmov	w10, s0
	tst	w10, #0x1
	fmov	w10, s1
	csel	w11, w8, w10, ne
	mov.s	w10, v1[1]
	tst	w9, #0x1
	csel	w8, w8, w10, ne
	stp	w11, w8, [x3, #16]
	ret
LBB6_6:
	stp	xzr, xzr, [x3]
	str	xzr, [x3, #16]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi7EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi7EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi7EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi7EEvPfS0_iPi:         ; @_Z10mandel_parILi7EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	stp	d13, d12, [sp, #-48]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	stp	d11, d10, [sp, #16]             ; 16-byte Folded Spill
	stp	d9, d8, [sp, #32]               ; 16-byte Folded Spill
	.cfi_offset b8, -8
	.cfi_offset b9, -16
	.cfi_offset b10, -24
	.cfi_offset b11, -32
	.cfi_offset b12, -40
	.cfi_offset b13, -48
	cmp	w2, #1
	b.lt	LBB7_6
; %bb.1:
	mov	w10, #0
	mov	w8, #0
	mov	w9, #0
	ldp	d0, d1, [x0]
	ldr	d2, [x0, #16]
	ldr	s3, [x0, #24]
	ldp	d4, d5, [x1]
	ldr	d6, [x1, #16]
	ldr	s7, [x1, #24]
	movi.2d	v16, #0000000000000000
	movi	d17, #0x0000ff000000ff
	fmov.2s	v18, #4.00000000
	movi.2s	v19, #1
	fmov	s21, #4.00000000
	fmov	s24, s3
	fmov	s25, s7
	fmov	d31, d0
	fmov	d28, d4
	movi.2d	v20, #0000000000000000
	fmov	d8, d1
	fmov	d29, d5
	movi.2d	v26, #0000000000000000
	movi.2d	v22, #0000000000000000
	fmov	d9, d2
	fmov	d30, d6
	movi.2d	v27, #0000000000000000
	movi.2d	v23, #0000000000000000
LBB7_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v16, v16, v17
	cmeq.2s	v10, v16, #0
	fmul.2s	v11, v28, v28
	dup.2s	v12, w9
	bit.8b	v20, v12, v10
	fmla.2s	v11, v31, v31
	fcmgt.2s	v10, v11, v18
	and.8b	v10, v10, v19
	orr.8b	v16, v10, v16
	mov.s	w11, v16[1]
	fmov	w12, s16
	and	w11, w12, w11
	and.8b	v10, v26, v17
	fmul.2s	v26, v29, v29
	fmla.2s	v26, v8, v8
	fcmgt.2s	v26, v26, v18
	and.8b	v26, v26, v19
	orr.8b	v26, v26, v10
	fmov	w12, s26
	and	w11, w11, w12
	cmeq.2s	v10, v10, #0
	bit.8b	v22, v12, v10
	mov.s	w12, v26[1]
	and	w11, w11, w12
	and.8b	v10, v27, v17
	fmul.2s	v27, v30, v30
	fmla.2s	v27, v9, v9
	fcmgt.2s	v27, v27, v18
	and.8b	v27, v27, v19
	orr.8b	v27, v27, v10
	fmov	w12, s27
	and	w11, w11, w12
	cmeq.2s	v10, v10, #0
	bit.8b	v23, v12, v10
	mov.s	w12, v27[1]
	and	w12, w11, w12
	fmul	s10, s25, s25
	fmadd	s10, s24, s24, s10
	fcmp	s10, s21
	cset	w11, gt
	orr	w11, w11, w10
	and	w12, w12, w11
	cmp	w10, #0
	csel	w8, w9, w8, eq
	tbnz	w12, #0, LBB7_5
; %bb.3:                                ;   in Loop: Header=BB7_2 Depth=1
	and	w10, w11, #0xff
	fneg.2s	v10, v28
	fmul.2s	v10, v28, v10
	fmla.2s	v10, v31, v31
	fadd.2s	v31, v31, v31
	fadd.2s	v10, v0, v10
	fmul.2s	v28, v31, v28
	fadd.2s	v28, v28, v4
	fneg.2s	v31, v29
	fmul.2s	v31, v29, v31
	fmla.2s	v31, v8, v8
	fadd.2s	v8, v8, v8
	fadd.2s	v11, v1, v31
	fmul.2s	v29, v8, v29
	fadd.2s	v29, v29, v5
	fneg.2s	v31, v30
	fmul.2s	v31, v30, v31
	fmla.2s	v31, v9, v9
	fadd.2s	v8, v9, v9
	fadd.2s	v9, v2, v31
	fmul.2s	v30, v8, v30
	fadd.2s	v30, v30, v6
	fneg	s31, s25
	fmul	s31, s25, s31
	fmadd	s31, s24, s24, s31
	fadd	s24, s24, s24
	fmul	s25, s24, s25
	fadd	s24, s3, s31
	add	w9, w9, #1
	fadd	s25, s25, s7
	fmov	d31, d10
	fmov	d8, d11
	cmp	w2, w9
	b.ne	LBB7_2
; %bb.4:
	mov	x9, x2
LBB7_5:
	cmeq.2s	v0, v16, #0
	dup.2s	v1, w9
	bsl.8b	v0, v1, v20
	cmeq.2s	v2, v26, #0
	bsl.8b	v2, v1, v22
	stp	d0, d2, [x3]
	cmeq.2s	v0, v27, #0
	bsl.8b	v0, v1, v23
	str	d0, [x3, #16]
	cmp	w11, #0
	csel	w8, w9, w8, eq
	str	w8, [x3, #24]
	ldp	d9, d8, [sp, #32]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]             ; 16-byte Folded Reload
	ldp	d13, d12, [sp], #48             ; 16-byte Folded Reload
	ret
LBB7_6:
	mov	w8, #0
	stp	xzr, xzr, [x3]
	str	xzr, [x3, #16]
	str	w8, [x3, #24]
	ldp	d9, d8, [sp, #32]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]             ; 16-byte Folded Reload
	ldp	d13, d12, [sp], #48             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z10mandel_parILi8EEvPfS0_iPi  ; -- Begin function _Z10mandel_parILi8EEvPfS0_iPi
	.weak_definition	__Z10mandel_parILi8EEvPfS0_iPi
	.p2align	2
__Z10mandel_parILi8EEvPfS0_iPi:         ; @_Z10mandel_parILi8EEvPfS0_iPi
	.cfi_startproc
; %bb.0:
	stp	d13, d12, [sp, #-48]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	stp	d11, d10, [sp, #16]             ; 16-byte Folded Spill
	stp	d9, d8, [sp, #32]               ; 16-byte Folded Spill
	.cfi_offset b8, -8
	.cfi_offset b9, -16
	.cfi_offset b10, -24
	.cfi_offset b11, -32
	.cfi_offset b12, -40
	.cfi_offset b13, -48
	cmp	w2, #1
	b.lt	LBB8_6
; %bb.1:
	mov	w8, #0
	ldp	d0, d1, [x0]
	ldp	d2, d4, [x0, #16]
	ldp	d5, d6, [x1]
	movi.2d	v7, #0000000000000000
	movi	d19, #0x0000ff000000ff
	ldp	d20, d21, [x1, #16]
	fmov.2s	v22, #4.00000000
	movi.2s	v23, #1
	fmov	d8, d0
	fmov	d26, d5
	movi.2d	v16, #0000000000000000
	fmov	d9, d1
	fmov	d28, d6
	movi.2d	v24, #0000000000000000
	movi.2d	v17, #0000000000000000
	fmov	d10, d2
	fmov	d29, d20
	movi.2d	v25, #0000000000000000
	movi.2d	v18, #0000000000000000
	fmov	d30, d4
	fmov	d31, d21
	movi.2d	v27, #0000000000000000
	movi.2d	v3, #0000000000000000
LBB8_2:                                 ; =>This Inner Loop Header: Depth=1
	and.8b	v7, v7, v19
	cmeq.2s	v11, v7, #0
	fmul.2s	v12, v26, v26
	dup.2s	v13, w8
	bit.8b	v16, v13, v11
	fmla.2s	v12, v8, v8
	fcmgt.2s	v11, v12, v22
	and.8b	v11, v11, v23
	orr.8b	v7, v11, v7
	fmov	w9, s7
	mov.s	w10, v7[1]
	and	w9, w9, w10
	and.8b	v11, v24, v19
	fmul.2s	v24, v28, v28
	fmla.2s	v24, v9, v9
	fcmgt.2s	v24, v24, v22
	and.8b	v24, v24, v23
	orr.8b	v24, v24, v11
	fmov	w10, s24
	and	w9, w9, w10
	cmeq.2s	v11, v11, #0
	bit.8b	v17, v13, v11
	mov.s	w10, v24[1]
	and	w9, w9, w10
	and.8b	v11, v25, v19
	fmul.2s	v25, v29, v29
	fmla.2s	v25, v10, v10
	fcmgt.2s	v25, v25, v22
	and.8b	v25, v25, v23
	orr.8b	v25, v25, v11
	fmov	w10, s25
	and	w9, w9, w10
	cmeq.2s	v11, v11, #0
	bit.8b	v18, v13, v11
	mov.s	w10, v25[1]
	and	w9, w9, w10
	and.8b	v11, v27, v19
	fmul.2s	v27, v31, v31
	fmla.2s	v27, v30, v30
	fcmgt.2s	v27, v27, v22
	and.8b	v27, v27, v23
	orr.8b	v27, v27, v11
	fmov	w10, s27
	and	w9, w9, w10
	cmeq.2s	v11, v11, #0
	bit.8b	v3, v13, v11
	mov.s	w10, v27[1]
	and	w9, w9, w10
	tbnz	w9, #0, LBB8_5
; %bb.3:                                ;   in Loop: Header=BB8_2 Depth=1
	fneg.2s	v11, v26
	fmul.2s	v11, v26, v11
	fmla.2s	v11, v8, v8
	fadd.2s	v8, v8, v8
	fadd.2s	v11, v0, v11
	fmul.2s	v26, v8, v26
	fadd.2s	v26, v26, v5
	fneg.2s	v8, v28
	fmul.2s	v8, v28, v8
	fmla.2s	v8, v9, v9
	fadd.2s	v9, v9, v9
	fadd.2s	v12, v1, v8
	fmul.2s	v28, v9, v28
	fadd.2s	v28, v28, v6
	fneg.2s	v8, v29
	fmul.2s	v8, v29, v8
	fmla.2s	v8, v10, v10
	fadd.2s	v9, v10, v10
	fadd.2s	v10, v2, v8
	fmul.2s	v29, v9, v29
	fadd.2s	v29, v29, v20
	fneg.2s	v8, v31
	fadd.2s	v9, v30, v30
	fmul.2s	v8, v31, v8
	fmla.2s	v8, v30, v30
	fmul.2s	v31, v9, v31
	fadd.2s	v30, v4, v8
	add	w8, w8, #1
	fadd.2s	v31, v31, v21
	fmov	d8, d11
	fmov	d9, d12
	cmp	w2, w8
	b.ne	LBB8_2
; %bb.4:
	mov	x8, x2
LBB8_5:
	cmeq.2s	v0, v7, #0
	dup.2s	v1, w8
	bsl.8b	v0, v1, v16
	str	d0, [x3]
	cmeq.2s	v0, v24, #0
	bsl.8b	v0, v1, v17
	str	d0, [x3, #8]
	cmeq.2s	v0, v25, #0
	bsl.8b	v0, v1, v18
	str	d0, [x3, #16]
	cmeq.2s	v0, v27, #0
	mov.s	w9, v0[1]
	fmov	w10, s0
	tst	w10, #0x1
	fmov	w10, s3
	csel	w11, w8, w10, ne
	mov.s	w10, v3[1]
	tst	w9, #0x1
	csel	w8, w8, w10, ne
	stp	w11, w8, [x3, #24]
	ldp	d9, d8, [sp, #32]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]             ; 16-byte Folded Reload
	ldp	d13, d12, [sp], #48             ; 16-byte Folded Reload
	ret
LBB8_6:
	mov	w8, #0
	stp	xzr, xzr, [x3]
	stp	wzr, w8, [x3, #24]
	str	xzr, [x3, #16]
	ldp	d9, d8, [sp, #32]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]             ; 16-byte Folded Reload
	ldp	d13, d12, [sp], #48             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z16mandelbrotSerialPFiffiEffffiiiiiPi ; -- Begin function _Z16mandelbrotSerialPFiffiEffffiiiiiPi
	.p2align	2
__Z16mandelbrotSerialPFiffiEffffiiiiiPi: ; @_Z16mandelbrotSerialPFiffiEffffiiiiiPi
	.cfi_startproc
; %bb.0:
	stp	d13, d12, [sp, #-144]!          ; 16-byte Folded Spill
	.cfi_def_cfa_offset 144
	stp	d11, d10, [sp, #16]             ; 16-byte Folded Spill
	stp	d9, d8, [sp, #32]               ; 16-byte Folded Spill
	stp	x28, x27, [sp, #48]             ; 16-byte Folded Spill
	stp	x26, x25, [sp, #64]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #80]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #96]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #112]            ; 16-byte Folded Spill
	stp	x29, x30, [sp, #128]            ; 16-byte Folded Spill
	add	x29, sp, #128
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	.cfi_offset b10, -120
	.cfi_offset b11, -128
	.cfi_offset b12, -136
	.cfi_offset b13, -144
                                        ; kill: def $w3 killed $w3 def $x3
	mov	x20, #0
	cmp	w4, #1
	b.lt	LBB9_6
; %bb.1:
	cmp	w1, #1
	b.lt	LBB9_6
; %bb.2:
	mov	x19, x5
	fmov	s8, s1
	fmov	s9, s0
	mov	x21, x0
	mov	x20, #0
	fsub	s0, s2, s0
	scvtf	s1, w1
	fdiv	s11, s0, s1
	fsub	s0, s3, s8
	scvtf	s1, w2
	add	w8, w4, w3
	sxtw	x22, w3
	mov	w23, w1
	sxtw	x24, w8
	mul	x8, x22, x23
	add	x25, x6, x8, lsl #2
	lsl	x26, x23, #2
	fdiv	s12, s0, s1
LBB9_3:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB9_4 Depth 2
	mov	x27, #0
	scvtf	s0, w22
	fmadd	s10, s0, s12, s8
LBB9_4:                                 ;   Parent Loop BB9_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	scvtf	s0, w27
	fmadd	s0, s0, s11, s9
	fmov	s1, s10
	mov	x0, x19
	blr	x21
	add	x20, x20, w0, sxtw
	str	w0, [x25, x27, lsl #2]
	add	x27, x27, #1
	cmp	x23, x27
	b.ne	LBB9_4
; %bb.5:                                ;   in Loop: Header=BB9_3 Depth=1
	add	x22, x22, #1
	add	x25, x25, x26
	cmp	x22, x24
	b.lt	LBB9_3
LBB9_6:
	mov	x0, x20
	ldp	x29, x30, [sp, #128]            ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #112]            ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]             ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #32]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]             ; 16-byte Folded Reload
	ldp	d13, d12, [sp], #144            ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4                               ; -- Begin function _Z18mandelbrotParallelPFvPfS_iPiEiffffiiiiiS0_
lCPI10_0:
	.long	0                               ; 0x0
	.long	1                               ; 0x1
	.long	2                               ; 0x2
	.long	3                               ; 0x3
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z18mandelbrotParallelPFvPfS_iPiEiffffiiiiiS0_
	.p2align	2
__Z18mandelbrotParallelPFvPfS_iPiEiffffiiiiiS0_: ; @_Z18mandelbrotParallelPFvPfS_iPiEiffffiiiiiS0_
	.cfi_startproc
; %bb.0:
	stp	d9, d8, [sp, #-112]!            ; 16-byte Folded Spill
	.cfi_def_cfa_offset 112
	stp	x28, x27, [sp, #16]             ; 16-byte Folded Spill
	stp	x26, x25, [sp, #32]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	add	x29, sp, #96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	sub	sp, sp, #368
	mov	x24, x7
	mov	x20, x6
	mov	x26, x4
                                        ; kill: def $s1 killed $s1 def $q1
	fmov	s8, s0
Lloh0:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh1:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh2:
	ldr	x8, [x8]
	stur	x8, [x29, #-112]
	fsub	s0, s2, s0
	scvtf	s2, w2
	fdiv	s9, s0, s2
	fsub	s0, s3, s1
	scvtf	s2, w3
	add	w10, w5, w4
	sub	w19, w10, w1
	fdiv	s16, s0, s2
	cmp	w19, w4
	b.lt	LBB10_22
; %bb.1:
	cmp	w2, #0
	b.le	LBB10_21
; %bb.2:
	sub	x8, x29, #200
	stur	x10, [x8, #-256]                ; 8-byte Folded Spill
	mov	w21, w1
	cmp	w1, #0
	stp	q16, q1, [x29, #-224]           ; 32-byte Folded Spill
	sub	x8, x29, #188
	stur	w2, [x8, #-256]                 ; 4-byte Folded Spill
	stur	w1, [x29, #-116]                ; 4-byte Folded Spill
	stur	x0, [x29, #-168]                ; 8-byte Folded Spill
	b.le	LBB10_24
; %bb.3:
	mov	x22, #0
	lsl	x8, x21, #2
	stur	x8, [x29, #-184]                ; 8-byte Folded Spill
	mov	w10, w2
	sxtw	x26, w26
	sxtw	x8, w19
	sub	x9, x29, #176
	stur	x8, [x9, #-256]                 ; 8-byte Folded Spill
	and	x9, x21, #0xfffffff0
	dup.4s	v0, v16[0]
	dup.4s	v1, v1[0]
	and	x8, x21, #0xfffffffe
	stp	x8, x9, [x29, #-240]            ; 16-byte Folded Spill
	mul	x8, x26, x10
	add	x9, x10, x8
	add	x14, x24, x9, lsl #2
	mul	x9, x10, x21
	lsl	x9, x9, #2
	sub	x11, x29, #184
	stur	x9, [x11, #-256]                ; 8-byte Folded Spill
	lsl	x19, x10, #3
	stur	x24, [x29, #-248]               ; 8-byte Folded Spill
	add	x24, x24, x8, lsl #2
Lloh3:
	adrp	x8, lCPI10_0@PAGE
Lloh4:
	ldr	q2, [x8, lCPI10_0@PAGEOFF]
	sub	x8, x29, #144
	stur	q2, [x8, #-256]                 ; 16-byte Folded Spill
	stur	x10, [x29, #-160]               ; 8-byte Folded Spill
	lsl	x23, x10, #2
	sub	x8, x29, #112
	stur	q0, [x8, #-256]                 ; 16-byte Folded Spill
	sub	x8, x29, #128
	stur	q1, [x8, #-256]                 ; 16-byte Folded Spill
	b	LBB10_5
LBB10_4:                                ;   in Loop: Header=BB10_5 Depth=1
	sub	x8, x29, #168
	ldur	x8, [x8, #-256]                 ; 8-byte Folded Reload
	mov	sp, x8
	sub	x8, x29, #152
	ldur	x14, [x8, #-256]                ; 8-byte Folded Reload
	sub	x8, x29, #184
	ldur	x8, [x8, #-256]                 ; 8-byte Folded Reload
	add	x14, x14, x8
	sub	x9, x29, #160
	ldur	x24, [x9, #-256]                ; 8-byte Folded Reload
	add	x24, x24, x8
	ldur	x26, [x29, #-176]               ; 8-byte Folded Reload
	add	x26, x26, x21
	sub	x8, x29, #176
	ldur	x8, [x8, #-256]                 ; 8-byte Folded Reload
	cmp	x26, x8
	b.gt	LBB10_23
LBB10_5:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB10_7 Depth 2
                                        ;       Child Loop BB10_10 Depth 3
                                        ;       Child Loop BB10_13 Depth 3
                                        ;       Child Loop BB10_17 Depth 3
                                        ;       Child Loop BB10_20 Depth 3
	mov	x8, sp
	sub	x9, x29, #168
	stur	x8, [x9, #-256]                 ; 8-byte Folded Spill
	ldur	x13, [x29, #-184]               ; 8-byte Folded Reload
	mov	x9, x13
Lloh5:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh6:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	mov	x8, sp
	add	x9, x13, #15
	and	x12, x9, #0x7fffffff0
	sub	x15, x8, x12
	mov	sp, x15
	mov	x9, x13
Lloh7:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh8:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	mov	x8, sp
	sub	x17, x8, x12
	mov	sp, x17
	mov	x9, x13
Lloh9:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh10:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	mov	x8, sp
	sub	x28, x8, x12
	mov	sp, x28
	mov	x25, #0
	dup.4s	v2, w26
	add	w8, w26, #4
	dup.4s	v3, w8
	add	w8, w26, #8
	dup.4s	v4, w8
	stur	x26, [x29, #-176]               ; 8-byte Folded Spill
	add	w8, w26, #12
	stur	x17, [x29, #-152]               ; 8-byte Folded Spill
	add	x9, x17, #32
	sub	x10, x29, #56
	stur	x9, [x10, #-256]                ; 8-byte Folded Spill
	dup.4s	v5, w8
	mov	x27, x15
	add	x8, x15, #32
	sub	x9, x29, #88
	stur	x8, [x9, #-256]                 ; 8-byte Folded Spill
	add	x8, x28, #4
	sub	x9, x29, #96
	stur	x8, [x9, #-256]                 ; 8-byte Folded Spill
	sub	x8, x29, #160
	stur	x24, [x8, #-256]                ; 8-byte Folded Spill
	sub	x8, x29, #152
	stur	x14, [x8, #-256]                ; 8-byte Folded Spill
	mov	x26, x14
	sub	x8, x29, #16
	stur	q2, [x8, #-256]                 ; 16-byte Folded Spill
	sub	x8, x29, #32
	stur	q3, [x8, #-256]                 ; 16-byte Folded Spill
	sub	x8, x29, #48
	stur	q4, [x8, #-256]                 ; 16-byte Folded Spill
	sub	x8, x29, #80
	stur	q5, [x8, #-256]                 ; 16-byte Folded Spill
	b	LBB10_7
LBB10_6:                                ;   in Loop: Header=BB10_7 Depth=2
	add	x25, x25, #1
	add	x26, x26, #4
	add	x24, x24, #4
	ldur	x8, [x29, #-160]                ; 8-byte Folded Reload
	cmp	x25, x8
	b.eq	LBB10_4
LBB10_7:                                ;   Parent Loop BB10_5 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB10_10 Depth 3
                                        ;       Child Loop BB10_13 Depth 3
                                        ;       Child Loop BB10_17 Depth 3
                                        ;       Child Loop BB10_20 Depth 3
	scvtf	s0, w25
	fmadd	s0, s0, s9, s8
	stur	q0, [x29, #-144]                ; 16-byte Folded Spill
	mov	x0, x28
	ldur	x1, [x29, #-184]                ; 8-byte Folded Reload
	bl	_bzero
	ldur	w8, [x29, #-116]                ; 4-byte Folded Reload
	cmp	w8, #16
	b.hs	LBB10_9
; %bb.8:                                ;   in Loop: Header=BB10_7 Depth=2
	mov	x11, #0
	ldp	q16, q7, [x29, #-224]           ; 32-byte Folded Reload
	ldur	x12, [x29, #-168]               ; 8-byte Folded Reload
	ldur	q24, [x29, #-144]               ; 16-byte Folded Reload
	b	LBB10_12
LBB10_9:                                ;   in Loop: Header=BB10_7 Depth=2
	ldur	q24, [x29, #-144]               ; 16-byte Folded Reload
	dup.4s	v0, v24[0]
	sub	x8, x29, #88
	ldur	x8, [x8, #-256]                 ; 8-byte Folded Reload
	sub	x9, x29, #56
	ldur	x9, [x9, #-256]                 ; 8-byte Folded Reload
	ldur	x10, [x29, #-232]               ; 8-byte Folded Reload
	sub	x11, x29, #144
	ldur	q1, [x11, #-256]                ; 16-byte Folded Reload
	ldp	q16, q7, [x29, #-224]           ; 32-byte Folded Reload
	ldur	x12, [x29, #-168]               ; 8-byte Folded Reload
	sub	x11, x29, #112
	ldur	q17, [x11, #-256]               ; 16-byte Folded Reload
	sub	x11, x29, #128
	ldur	q18, [x11, #-256]               ; 16-byte Folded Reload
	movi.4s	v19, #16
	sub	x11, x29, #16
	ldur	q20, [x11, #-256]               ; 16-byte Folded Reload
	sub	x11, x29, #32
	ldur	q21, [x11, #-256]               ; 16-byte Folded Reload
	sub	x11, x29, #48
	ldur	q22, [x11, #-256]               ; 16-byte Folded Reload
	sub	x11, x29, #80
	ldur	q23, [x11, #-256]               ; 16-byte Folded Reload
LBB10_10:                               ;   Parent Loop BB10_5 Depth=1
                                        ;     Parent Loop BB10_7 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	stp	q0, q0, [x8, #-32]
	stp	q0, q0, [x8], #64
	add.4s	v2, v1, v20
	add.4s	v3, v21, v1
	add.4s	v4, v22, v1
	add.4s	v5, v23, v1
	scvtf.4s	v2, v2
	scvtf.4s	v3, v3
	scvtf.4s	v4, v4
	scvtf.4s	v5, v5
	mov.16b	v6, v18
	fmla.4s	v6, v17, v2
	mov.16b	v2, v18
	fmla.4s	v2, v17, v3
	mov.16b	v3, v18
	fmla.4s	v3, v17, v4
	mov.16b	v4, v18
	fmla.4s	v4, v17, v5
	stp	q6, q2, [x9, #-32]
	stp	q3, q4, [x9], #64
	add.4s	v1, v1, v19
	subs	x10, x10, #16
	b.ne	LBB10_10
; %bb.11:                               ;   in Loop: Header=BB10_7 Depth=2
	ldur	x8, [x29, #-232]                ; 8-byte Folded Reload
	mov	x11, x8
	cmp	x8, x21
	b.eq	LBB10_14
LBB10_12:                               ;   in Loop: Header=BB10_7 Depth=2
	ldur	x8, [x29, #-176]                ; 8-byte Folded Reload
	add	w8, w8, w11
	lsl	x10, x11, #2
	add	x9, x27, x10
	ldur	x13, [x29, #-152]               ; 8-byte Folded Reload
	add	x10, x13, x10
	sub	x11, x21, x11
LBB10_13:                               ;   Parent Loop BB10_5 Depth=1
                                        ;     Parent Loop BB10_7 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	str	s24, [x9], #4
	scvtf	s0, w8
	fmadd	s0, s0, s16, s7
	str	s0, [x10], #4
	add	w8, w8, #1
	subs	x11, x11, #1
	b.ne	LBB10_13
LBB10_14:                               ;   in Loop: Header=BB10_7 Depth=2
	mov	x0, x27
	ldur	x1, [x29, #-152]                ; 8-byte Folded Reload
	mov	x2, x20
	mov	x3, x28
	blr	x12
	ldur	w8, [x29, #-116]                ; 4-byte Folded Reload
	cmp	w8, #1
	b.ne	LBB10_16
; %bb.15:                               ;   in Loop: Header=BB10_7 Depth=2
	mov	x10, #0
	b	LBB10_19
LBB10_16:                               ;   in Loop: Header=BB10_7 Depth=2
	mov	x9, #0
	mov	x8, #0
	ldur	x10, [x29, #-240]               ; 8-byte Folded Reload
	sub	x11, x29, #96
	ldur	x11, [x11, #-256]               ; 8-byte Folded Reload
LBB10_17:                               ;   Parent Loop BB10_5 Depth=1
                                        ;     Parent Loop BB10_7 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldpsw	x12, x13, [x11, #-4]
	str	w12, [x24, x9]
	str	w13, [x26, x9]
	add	x22, x22, x12
	add	x8, x8, x13
	add	x9, x9, x19
	add	x11, x11, #8
	subs	x10, x10, #2
	b.ne	LBB10_17
; %bb.18:                               ;   in Loop: Header=BB10_7 Depth=2
	add	x22, x8, x22
	ldur	x8, [x29, #-240]                ; 8-byte Folded Reload
	mov	x10, x8
	cmp	x8, x21
	b.eq	LBB10_6
LBB10_19:                               ;   in Loop: Header=BB10_7 Depth=2
	add	x8, x28, x10, lsl #2
	ldur	x9, [x29, #-176]                ; 8-byte Folded Reload
	add	x9, x9, x10
	ldur	x11, [x29, #-160]               ; 8-byte Folded Reload
	madd	x9, x11, x9, x25
	ldur	x11, [x29, #-248]               ; 8-byte Folded Reload
	add	x9, x11, x9, lsl #2
	sub	x10, x21, x10
LBB10_20:                               ;   Parent Loop BB10_5 Depth=1
                                        ;     Parent Loop BB10_7 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldrsw	x11, [x8], #4
	str	w11, [x9]
	add	x22, x22, x11
	add	x9, x9, x23
	subs	x10, x10, #1
	b.ne	LBB10_20
	b	LBB10_6
LBB10_21:                               ; =>This Inner Loop Header: Depth=1
	add	w26, w26, w1
	cmp	w26, w19
	b.le	LBB10_21
LBB10_22:
	mov	x22, #0
	cmp	w26, w10
	b.lt	LBB10_29
	b	LBB10_40
LBB10_23:
                                        ; kill: def $w26 killed $w26 killed $x26 def $x26
	ldur	x24, [x29, #-248]               ; 8-byte Folded Reload
	ldp	q16, q1, [x29, #-224]           ; 32-byte Folded Reload
	sub	x8, x29, #188
	ldur	w2, [x8, #-256]                 ; 4-byte Folded Reload
	sub	x8, x29, #200
	ldur	x10, [x8, #-256]                ; 8-byte Folded Reload
	cmp	w26, w10
	b.lt	LBB10_29
	b	LBB10_40
LBB10_24:
	lsl	x22, x21, #2
LBB10_25:                               ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB10_26 Depth 2
	mov	x21, sp
	mov	x9, x22
Lloh11:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh12:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	mov	x8, sp
	add	x9, x22, #15
	and	x12, x9, #0x7fffffff0
	sub	x25, x8, x12
	mov	sp, x25
	mov	x9, x22
Lloh13:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh14:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	mov	x8, sp
	sub	x27, x8, x12
	mov	sp, x27
	mov	x9, x22
Lloh15:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh16:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	mov	x8, sp
	sub	x28, x8, x12
	mov	sp, x28
	mov	x23, x2
LBB10_26:                               ;   Parent Loop BB10_25 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	mov	x0, x25
	mov	x1, x27
	mov	x2, x20
	mov	x3, x28
	ldur	x8, [x29, #-168]                ; 8-byte Folded Reload
	blr	x8
	subs	w23, w23, #1
	b.ne	LBB10_26
; %bb.27:                               ;   in Loop: Header=BB10_25 Depth=1
	mov	sp, x21
	ldur	w8, [x29, #-116]                ; 4-byte Folded Reload
	add	w26, w26, w8
	cmp	w26, w19
	sub	x8, x29, #188
	ldur	w2, [x8, #-256]                 ; 4-byte Folded Reload
	b.le	LBB10_25
; %bb.28:
	mov	x22, #0
	ldp	q16, q1, [x29, #-224]           ; 32-byte Folded Reload
	sub	x8, x29, #200
	ldur	x10, [x8, #-256]                ; 8-byte Folded Reload
	cmp	w26, w10
	b.ge	LBB10_40
LBB10_29:
	cmp	w2, #1
	b.lt	LBB10_40
; %bb.30:
	cmp	w20, #0
	b.le	LBB10_39
; %bb.31:
                                        ; kill: def $w26 killed $w26 killed $x26 def $x26
	sxtw	x8, w26
	mov	w9, w2
	sxtw	x10, w10
	fmov	s0, #4.00000000
	b	LBB10_33
LBB10_32:                               ;   in Loop: Header=BB10_33 Depth=1
	add	x8, x8, #1
	cmp	x8, x10
	b.eq	LBB10_40
LBB10_33:                               ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB10_35 Depth 2
                                        ;       Child Loop BB10_36 Depth 3
	mov	x11, #0
	scvtf	s2, w8
	fmadd	s7, s2, s16, s1
	mul	x12, x8, x9
	b	LBB10_35
LBB10_34:                               ;   in Loop: Header=BB10_35 Depth=2
	add	x22, x22, w13, sxtw
	str	w13, [x24, x14, lsl #2]
	add	x11, x11, #1
	cmp	x11, x9
	b.eq	LBB10_32
LBB10_35:                               ;   Parent Loop BB10_33 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB10_36 Depth 3
	mov	w13, #0
	scvtf	s2, w11
	fmadd	s2, s2, s9, s8
	add	x14, x11, x12
	fmov	s3, s2
	fmov	s4, s7
LBB10_36:                               ;   Parent Loop BB10_33 Depth=1
                                        ;     Parent Loop BB10_35 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	fmul	s5, s4, s4
	fmadd	s6, s3, s3, s5
	fcmp	s6, s0
	b.gt	LBB10_34
; %bb.37:                               ;   in Loop: Header=BB10_36 Depth=3
	fnmsub	s5, s3, s3, s5
	fadd	s3, s3, s3
	fmul	s4, s4, s3
	fadd	s3, s2, s5
	fadd	s4, s7, s4
	add	w13, w13, #1
	cmp	w20, w13
	b.ne	LBB10_36
; %bb.38:                               ;   in Loop: Header=BB10_35 Depth=2
	mov	x13, x20
	b	LBB10_34
LBB10_39:
	mul	w8, w26, w2
	mov	w9, w2
	add	x0, x24, w8, sxtw #2
	mvn	w8, w26
	add	w8, w10, w8
	umaddl	x8, w2, w8, x9
	lsl	x1, x8, #2
	bl	_bzero
LBB10_40:
	ldur	x8, [x29, #-112]
Lloh17:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh18:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh19:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB10_42
; %bb.41:
	mov	x0, x22
	sub	sp, x29, #96
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #16]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp], #112              ; 16-byte Folded Reload
	ret
LBB10_42:
	bl	___stack_chk_fail
	.loh AdrpLdrGotLdr	Lloh0, Lloh1, Lloh2
	.loh AdrpLdr	Lloh3, Lloh4
	.loh AdrpLdrGot	Lloh9, Lloh10
	.loh AdrpLdrGot	Lloh7, Lloh8
	.loh AdrpLdrGot	Lloh5, Lloh6
	.loh AdrpLdrGot	Lloh15, Lloh16
	.loh AdrpLdrGot	Lloh13, Lloh14
	.loh AdrpLdrGot	Lloh11, Lloh12
	.loh AdrpLdrGotLdr	Lloh17, Lloh18, Lloh19
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"ILP parallelism x1"

l_.str.1:                               ; @.str.1
	.asciz	"ILP parallelism x2"

l_.str.2:                               ; @.str.2
	.asciz	"ILP parallelism x3"

l_.str.3:                               ; @.str.3
	.asciz	"ILP parallelism x4"

l_.str.4:                               ; @.str.4
	.asciz	"ILP parallelism x5"

l_.str.5:                               ; @.str.5
	.asciz	"ILP parallelism x6"

l_.str.6:                               ; @.str.6
	.asciz	"ILP parallelism x7"

l_.str.7:                               ; @.str.7
	.asciz	"ILP parallelism x8"

l_.str.8:                               ; @.str.8
	.space	1

	.section	__DATA,__data
	.globl	_par_funs                       ; @par_funs
	.p2align	3
_par_funs:
	.long	1                               ; 0x1
	.space	4
	.quad	__Z10mandel_parILi1EEvPfS0_iPi
	.quad	l_.str
	.long	2                               ; 0x2
	.space	4
	.quad	__Z10mandel_parILi2EEvPfS0_iPi
	.quad	l_.str.1
	.long	3                               ; 0x3
	.space	4
	.quad	__Z10mandel_parILi3EEvPfS0_iPi
	.quad	l_.str.2
	.long	4                               ; 0x4
	.space	4
	.quad	__Z10mandel_parILi4EEvPfS0_iPi
	.quad	l_.str.3
	.long	5                               ; 0x5
	.space	4
	.quad	__Z10mandel_parILi5EEvPfS0_iPi
	.quad	l_.str.4
	.long	6                               ; 0x6
	.space	4
	.quad	__Z10mandel_parILi6EEvPfS0_iPi
	.quad	l_.str.5
	.long	7                               ; 0x7
	.space	4
	.quad	__Z10mandel_parILi7EEvPfS0_iPi
	.quad	l_.str.6
	.long	8                               ; 0x8
	.space	4
	.quad	__Z10mandel_parILi8EEvPfS0_iPi
	.quad	l_.str.7
	.long	0                               ; 0x0
	.space	4
	.quad	0
	.quad	l_.str.8

.subsections_via_symbols
