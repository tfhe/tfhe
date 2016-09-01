	.file	"lagrangehalfc_impl_asm.cpp"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	LagrangeHalfCPolynomialMul
	.type	LagrangeHalfCPolynomialMul, @function
LagrangeHalfCPolynomialMul:
.LFB7134:
	.cfi_startproc
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* LagrangeHalfCPolynomialMul(HCP* r, HCP* a, HCP* b) */
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
	vmulpd	%ymm0,%ymm2,%ymm4 /* are*bre */
	vmulpd	%ymm1,%ymm3,%ymm5 /* aim*bim */
	vmulpd	%ymm0,%ymm3,%ymm6 /* are*bim */
	vmulpd	%ymm1,%ymm2,%ymm7 /* aim*bre */
	vsubpd	%ymm5,%ymm4,%ymm2
	vaddpd	%ymm7,%ymm6,%ymm3
	vmovupd	%ymm2,(%r8)
	vmovupd	%ymm3,(%r9)
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
	.size	LagrangeHalfCPolynomialMul, .-LagrangeHalfCPolynomialMul
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.section	.text.unlikely
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4,,15
	.globl	LagrangeHalfCPolynomialAddMul
	.type	LagrangeHalfCPolynomialAddMul, @function
LagrangeHalfCPolynomialAddMul:
.LFB7135:
	.cfi_startproc
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* LagrangeHalfCPolynomialAddMul(HCP* r, HCP* a, HCP* b) */
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
	vmovupd (%r8),%ymm8
	vmovupd (%r9),%ymm9
	vmovupd (%r10),%ymm0
	vmovupd (%r11),%ymm1
	vmovupd (%r12),%ymm2
	vmovupd (%r13),%ymm3
	vmulpd	%ymm0,%ymm2,%ymm4
	vmulpd	%ymm1,%ymm3,%ymm5
	vmulpd	%ymm0,%ymm3,%ymm6
	vmulpd	%ymm1,%ymm2,%ymm7
	vsubpd	%ymm5,%ymm4,%ymm2
	vaddpd	%ymm7,%ymm6,%ymm3
	vaddpd	%ymm8,%ymm2,%ymm8
	vaddpd	%ymm9,%ymm3,%ymm9
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
	.size	LagrangeHalfCPolynomialAddMul, .-LagrangeHalfCPolynomialAddMul
	.section	.text.unlikely
.LCOLDE1:
	.text
.LHOTE1:
	.section	.text.unlikely
.LCOLDB2:
	.text
.LHOTB2:
	.p2align 4,,15
	.globl	LagrangeHalfCPolynomialSubMul
	.type	LagrangeHalfCPolynomialSubMul, @function
LagrangeHalfCPolynomialSubMul:
.LFB7136:
	.cfi_startproc
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* LagrangeHalfCPolynomialSubMul(HCP* r, HCP* a, HCP* b) */
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
	vmulpd	%ymm0,%ymm2,%ymm4
	vmulpd	%ymm1,%ymm3,%ymm5
	vmulpd	%ymm0,%ymm3,%ymm6
	vmulpd	%ymm1,%ymm2,%ymm7
	vsubpd	%ymm5,%ymm4,%ymm2
	vaddpd	%ymm7,%ymm6,%ymm3
	vsubpd	%ymm2,%ymm8,%ymm8
	vsubpd	%ymm3,%ymm8,%ymm9
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
	.size	LagrangeHalfCPolynomialSubMul, .-LagrangeHalfCPolynomialSubMul
	.section	.text.unlikely
.LCOLDE2:
	.text
.LHOTE2:
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
