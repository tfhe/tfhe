	.file	"lagrangehalfc_impl_fma.s"
	.text
	.p2align 4
#if !__APPLE__
	.globl	LagrangeHalfCPolynomialMul
	.type	LagrangeHalfCPolynomialMul, @function
LagrangeHalfCPolynomialMul:
#else
	.globl	_LagrangeHalfCPolynomialMul
_LagrangeHalfCPolynomialMul:
#endif

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
	retq

#if !__APPLE__
	.size	LagrangeHalfCPolynomialMul, .-LagrangeHalfCPolynomialMul
#endif


//	.text
	.p2align 4
#if !__APPLE__
	.globl	LagrangeHalfCPolynomialAddMul
	.type	LagrangeHalfCPolynomialAddMul, @function
LagrangeHalfCPolynomialAddMul:
#else
	.globl	_LagrangeHalfCPolynomialAddMul
_LagrangeHalfCPolynomialAddMul:
#endif

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
	retq
#if !__APPLE__
	.size	LagrangeHalfCPolynomialAddMul, .-LagrangeHalfCPolynomialAddMul
#endif

//	.text
	.p2align 4
#if !__APPLE__
	.globl	LagrangeHalfCPolynomialSubMul
	.type	LagrangeHalfCPolynomialSubMul, @function
LagrangeHalfCPolynomialSubMul:
#else
	.globl	_LagrangeHalfCPolynomialSubMul
_LagrangeHalfCPolynomialSubMul:
#endif
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
	retq
#if !__APPLE__
	.size	LagrangeHalfCPolynomialSubMul, .-LagrangeHalfCPolynomialSubMul
#endif
