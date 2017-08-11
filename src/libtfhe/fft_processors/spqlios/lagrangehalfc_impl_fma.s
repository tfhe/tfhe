	.file	"lagrangehalfc_impl_asm.cpp"
#if __APPLE__
	.text
#else
	.section	.text.unlikely,"ax",@progbits
#endif
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	_LagrangeHalfCPolynomialMul
#if !__APPLE__
	.type	_LagrangeHalfCPolynomialMul, @function
#endif
_LagrangeHalfCPolynomialMul:
.LFB7134:
	.cfi_startproc
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* _LagrangeHalfCPolynomialMul(HCP* r, HCP* a, HCP* b) */
	movq	8(%rdi), %rax /* rax: proc */
	movq	$0,%rcx
	movl	8(%rax), %ecx /* ecx: Ns2 */
	movq	(%rdi), %r8  /* r8: base of rre */
	movq	(%rsi), %r10 /* r10: base of are */
	movq	(%rdx), %r12 /* r12: base of bre */
	leaq	(%r8,%rcx,8),%r9 /* r9: base of rim */ 
	leaq	(%r10,%rcx,8),%r11 /* r11: base of aim */ 
	leaq	(%r12,%rcx,8),%r13 /* r13: base of bim */
	movq	%r9,%r14           /* r14: end of loop */
.loopLHCmul:
	vmovupd (%r10),%ymm0 /* are */
	vmovupd (%r11),%ymm1 /* aim */
	vmovupd (%r12),%ymm2 /* bre */
	vmovupd (%r13),%ymm3 /* bim */
	vmulpd	%ymm1,%ymm3,%ymm4 /* aim*bim */
	vmulpd	%ymm0,%ymm3,%ymm6 /* are*bim */
	vfmadd231pd	%ymm2,%ymm1,%ymm6
	vfmsub231pd	%ymm0,%ymm2,%ymm4
	vmovupd	%ymm4,(%r8)
	vmovupd	%ymm6,(%r9)
	/* end of loop */
	addq	$32,%r8
	addq	$32,%r9
	addq	$32,%r10
	addq	$32,%r11
	addq	$32,%r12
	addq	$32,%r13
	cmpq	%r14,%r8
	jb .loopLHCmul

	vzeroall
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
.L1:
	rep ret
	.p2align 4,,10
	.p2align 3
.L22:
	rep ret
	.p2align 4,,10
	.p2align 3
.L21:
	rep ret
	.cfi_endproc
.LFE7134:
#if __APPLE__
	.text
#else
	.size	_LagrangeHalfCPolynomialMul, .-_LagrangeHalfCPolynomialMul
	.section	.text.unlikely
#endif
.LCOLDE0:
	.text
.LHOTE0:
#if __APPLE__
	.text
#else
	.section	.text.unlikely
#endif
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4,,15
	.globl	_LagrangeHalfCPolynomialAddMul
#if !__APPLE__
	.type	_LagrangeHalfCPolynomialAddMul, @function
#endif
_LagrangeHalfCPolynomialAddMul:
.LFB7135:
	.cfi_startproc
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* _LagrangeHalfCPolynomialAddMul(HCP* r, HCP* a, HCP* b) */
	movq	8(%rdi), %rax /* rax: proc */
	movq	$0,%rcx
	movl	8(%rax), %ecx /* ecx: Ns2 */
	movq	(%rdi), %r8  /* r8: base of rre */
	movq	(%rsi), %r10 /* r10: base of are */
	movq	(%rdx), %r12 /* r12: base of bre */
	leaq	(%r8,%rcx,8),%r9 /* r9: base of rim */ 
	leaq	(%r10,%rcx,8),%r11 /* r11: base of aim */ 
	leaq	(%r12,%rcx,8),%r13 /* r13: base of bim */
	movq	%r9,%r14           /* r14: end of loop */
.loopLHCaddmul:
	vmovupd (%r8),%ymm8 /* rre */
	vmovupd (%r9),%ymm9 /* rim */
	vmovupd (%r10),%ymm0
	vmovupd (%r11),%ymm1
	vmovupd (%r12),%ymm2
	vmovupd (%r13),%ymm3
        vfmsub231pd	%ymm3,%ymm1,%ymm8 /* rre  = -rre + aim.bim */
        vfmsub231pd	%ymm2,%ymm0,%ymm8 /* rre  = -rre + are.bre */
	vfmadd231pd	%ymm3,%ymm0,%ymm9 /* rim  += are.bim */
	vfmadd231pd	%ymm2,%ymm1,%ymm9 /* rim  += aim.bre */
	vmovupd	%ymm8,(%r8)
	vmovupd	%ymm9,(%r9)
	/* end of loop */
	addq	$32,%r8
	addq	$32,%r9
	addq	$32,%r10
	addq	$32,%r11
	addq	$32,%r12
	addq	$32,%r13
	cmpq	%r14,%r8
	jb .loopLHCaddmul

	vzeroall
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
.L23:
	rep ret
	.p2align 4,,10
	.p2align 3
.L43:
	rep ret
	.p2align 4,,10
	.p2align 3
.L42:
	rep ret
	.cfi_endproc
.LFE7135:
#if __APPLE__
	.text
#else
	.size	_LagrangeHalfCPolynomialAddMul, .-_LagrangeHalfCPolynomialAddMul
	.section	.text.unlikely
#endif
.LCOLDE1:
	.text
.LHOTE1:
#if __APPLE__
	.text
#else
	.section	.text.unlikely
#endif
.LCOLDB2:
	.text
.LHOTB2:
	.p2align 4,,15
	.globl	_LagrangeHalfCPolynomialSubMul
#if !__APPLE__
	.type	_LagrangeHalfCPolynomialSubMul, @function
#endif
_LagrangeHalfCPolynomialSubMul:
.LFB7136:
	.cfi_startproc
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* _LagrangeHalfCPolynomialSubMul(HCP* r, HCP* a, HCP* b) */
	movq	8(%rdi), %rax /* rax: proc */
	movq	$0,%rcx
	movl	8(%rax), %ecx /* ecx: Ns2 */
	movq	(%rdi), %r8  /* r8: base of rre */
	movq	(%rsi), %r10 /* r10: base of are */
	movq	(%rdx), %r12 /* r12: base of bre */
	leaq	(%r8,%rcx,8),%r9 /* r9: base of rim */ 
	leaq	(%r10,%rcx,8),%r11 /* r11: base of aim */ 
	leaq	(%r12,%rcx,8),%r13 /* r13: base of bim */
	movq	%r9,%r14           /* r14: end of loop */
.loopLHCsubmul:
	vmovupd (%r8),%ymm8
	vmovupd (%r9),%ymm9
	vmovupd (%r10),%ymm0
	vmovupd (%r11),%ymm1
	vmovupd (%r12),%ymm2
	vmovupd (%r13),%ymm3
        vfnmsub231pd	%ymm3,%ymm1,%ymm8 /* x  = -rre - aim.bim */
        vfnmsub231pd	%ymm2,%ymm0,%ymm8 /* rre  = -x - are.bre */
	vfmsub231pd	%ymm3,%ymm0,%ymm9 /* x  = are.bim - rim */
	vfnmsub231pd	%ymm2,%ymm1,%ymm9 /* rim  = -aim.bre - x*/
	vmovupd	%ymm8,(%r8)
	vmovupd	%ymm9,(%r9)
	/* end of loop */
	addq	$32,%r8
	addq	$32,%r9
	addq	$32,%r10
	addq	$32,%r11
	addq	$32,%r12
	addq	$32,%r13
	cmpq	%r14,%r8
	jb .loopLHCsubmul

	vzeroall
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
.L44:
	rep ret
	.p2align 4,,10
	.p2align 3
.L64:
	rep ret
	.p2align 4,,10
	.p2align 3
.L63:
	rep ret
	.cfi_endproc
.LFE7136:
#if __APPLE__
	.text
#else
	.size	_LagrangeHalfCPolynomialSubMul, .-_LagrangeHalfCPolynomialSubMul
	.section	.text.unlikely
#endif
.LCOLDE2:
	.text
.LHOTE2:
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
#if !__APPLE__
	.section	.note.GNU-stack,"",@progbits
#endif