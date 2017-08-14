/* 
 * Fast Fourier transform (x86-64 AVX)
 * 
 * Copyright (c) 2015 Project Nayuki
 * https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly
 * 
 * (MIT License)
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */


/* 
 * Storage usage:
 *    Bytes  Location  Description
 *        8  rcx       Loop counter
 *        8  rdx       Size of FFT (i.e. number of elements in the vector) (a power of 2), constant
 *        8  rdi       Base of real components array, constant (64-bit floats)
 *        8  rsi       Base of imaginary components array, constant (64-bit floats)
 *        8  r8        Base of trigonometric tables array (64-bit floats)
 *        8  r9        Base of bit reversal array (64-bit ints), loop counter
 *        8  rax       Temporary, loop counter
 *        8  r10       Temporary
 *        8  r11       Temporary
 *        8  r12       Temporary
 *        8  r13       Temporary
 *        8  rsp       x86-64 stack pointer
 *      320  ymm0-9    Temporary (64-bit float vectors)
 *       64  ymm14-15  Multiplication constants (64-bit float vectors)
 *        8  [rsp+ 0]  Caller's value of r13
 *        8  [rsp+ 8]  Caller's value of r12
 *        8  [rsp+16]  Caller's value of r11
 *        8  [rsp+24]  Caller's value of r10
 */
	.file	"fft-x8664-avx.s"
	.text
	.p2align 4
#if !__APPLE__
	.globl	fft_transform
	.type	fft_transform, @function
fft_transform:
#else
	.globl	_fft_transform
_fft_transform:
#endif
/* void fft_transform(const void *tables, double *real, double *imag) */
	/* Save registers */
	pushq       %r10
	pushq       %r11
	pushq       %r12
	pushq       %r13
	
	/* Permute registers for better variable names */
	movq        %rdi, %rax
	movq        %rsi, %rdi
	movq        %rdx, %rsi
	
	/* Load struct FftTables fields */
	movq         0(%rax), %rdx  /* rdx: Size of FFT (a power of 2, must be at least 4) */
	movq         8(%rax), %r9   /* r9: Base address of bit-reversed indexing array */
	movq        16(%rax), %r8   /* r8: Base address of trigonometric tables array */
	
	/* Bit-reversed addressing permutation */
	movq        $0, %rcx  /* Loop counter: Range [0, rdx), step size 1 */
bitrevloop:
	movq        (%r9,%rcx,8), %rax  /* rax: Bit reversal of rcx, with bit width log2(rdx) */
	cmpq        %rcx, %rax
	jae         bitrevskip
	movq        (%rdi,%rcx,8), %r10
	movq        (%rsi,%rcx,8), %r11
	movq        (%rdi,%rax,8), %r12
	movq        (%rsi,%rax,8), %r13
	movq        %r10, (%rdi,%rax,8)
	movq        %r11, (%rsi,%rax,8)
	movq        %r12, (%rdi,%rcx,8)
	movq        %r13, (%rsi,%rcx,8)
bitrevskip:
	incq        %rcx
	cmpq        %rdx, %rcx
	jb          bitrevloop
	
	/* Size 2 merge (special) */
	movq        $0, %rcx  /* Loop counter: Range [0, rdx), step size 4 */
size2loop:
	vmovupd     (%rdi,%rcx,8), %ymm0
	vmovupd     (%rsi,%rcx,8), %ymm2
	vshufpd     $15, %ymm0, %ymm0, %ymm1
	vshufpd     $0, %ymm0, %ymm0, %ymm0
	vshufpd     $15, %ymm2, %ymm2, %ymm3
	vshufpd     $0, %ymm2, %ymm2, %ymm2
	vaddsubpd   %ymm1, %ymm0, %ymm0
	vaddsubpd   %ymm3, %ymm2, %ymm2
	vshufpd     $5, %ymm0, %ymm0, %ymm0
	vshufpd     $5, %ymm2, %ymm2, %ymm2
	vmovupd     %ymm0, (%rdi,%rcx,8)
	vmovupd     %ymm2, (%rsi,%rcx,8)
	addq        $4, %rcx
	cmpq        %rdx, %rcx
	jb          size2loop
	
	/* Size 4 merge (special) */
	vmovapd     size4negation0(%rip), %ymm14
	vmovapd     size4negation1(%rip), %ymm15
	movq        $0, %rcx  /* Loop counter: Range [0, rdx), step size 4 */
size4loop:
	vmovupd     (%rdi,%rcx,8), %ymm0
	vmovupd     (%rsi,%rcx,8), %ymm1
	vperm2f128  $0, %ymm0, %ymm0, %ymm2
	vperm2f128  $0, %ymm1, %ymm1, %ymm3
	vshufpd     $10, %ymm1, %ymm0, %ymm4
	vshufpd     $10, %ymm0, %ymm1, %ymm5
	vperm2f128  $17, %ymm4, %ymm4, %ymm4
	vperm2f128  $17, %ymm5, %ymm5, %ymm5
	vmulpd      %ymm4, %ymm14, %ymm4
	vmulpd      %ymm5, %ymm15, %ymm5
	vaddpd      %ymm2, %ymm4, %ymm0
	vaddpd      %ymm3, %ymm5, %ymm1
	vmovupd     %ymm0, (%rdi,%rcx,8)
	vmovupd     %ymm1, (%rsi,%rcx,8)
	addq        $4, %rcx
	cmpq        %rdx, %rcx
	jb          size4loop
	
	/* Size 8 and larger merges (general) */
	movq        $4, %rcx  /* rcx: halfsize */
	cmpq        %rdx, %rcx
	jae         end
outerloop:   /* For each merge size: From 8, 16, 32, ..., to rdx (inclusive). rcx is half the merge size. */
	movq        $0, %rax
middleloop:  /* For each block of the current size: From 0, 2*rcx, 4*rcx, 6*rcx, ..., to rdx (exclusive). rax is the block start. */
	movq        $0, %r9
innerloop:   /* For each 4 elements up to halfsize. r9 is the vector start. */
	leaq        (%rax,%r9 ), %r10
	leaq        (%r10,%rcx), %r11
	vmovupd     (%rdi,%r10,8), %ymm0
	vmovupd     (%rsi,%r10,8), %ymm1
	vmovupd     (%rdi,%r11,8), %ymm2
	vmovupd     (%rsi,%r11,8), %ymm3
	leaq        (,%r9,4), %r12
	leaq        (%r8,%r12,4), %r12  /* r12 = r9*16 + r8 */
	vmovupd      0(%r12), %ymm4  /* Cosine table values */
	vmovupd     32(%r12), %ymm5  /* Sine table values */
	vmulpd      %ymm2, %ymm4, %ymm6
	vmulpd      %ymm3, %ymm5, %ymm7
	vmulpd      %ymm3, %ymm4, %ymm8
	vmulpd      %ymm2, %ymm5, %ymm9
	vaddpd      %ymm6, %ymm7, %ymm2
	vsubpd      %ymm9, %ymm8, %ymm3
	vaddpd      %ymm0, %ymm2, %ymm4
	vaddpd      %ymm1, %ymm3, %ymm5
	vsubpd      %ymm2, %ymm0, %ymm6
	vsubpd      %ymm3, %ymm1, %ymm7
	vmovupd     %ymm4, (%rdi,%r10,8)
	vmovupd     %ymm5, (%rsi,%r10,8)
	vmovupd     %ymm6, (%rdi,%r11,8)
	vmovupd     %ymm7, (%rsi,%r11,8)
	addq        $4, %r9
	cmpq        %rcx, %r9
	jb          innerloop
	/* End inner loop */
	leaq        (%rax,%rcx,2), %rax  /* Increment rax by current size */
	cmpq        %rdx, %rax
	jb          middleloop
	/* End middle loop */
	shlq        $1, %rcx
	leaq        (%r8,%rcx,8), %r8  /* Advance the trigonometric tables */
	cmpq        %rdx, %rcx
	jb          outerloop
	
	/* Restore registers */
end:
	vzeroall
	popq        %r13
	popq        %r12
	popq        %r11
	popq        %r10
	retq


/* Constants for YMM */
.balign 32
size4negation0: .double +1.0, +1.0, -1.0, -1.0
size4negation1: .double +1.0, -1.0, -1.0, +1.0

#if !__APPLE__
	.size	fft_transform, .-fft_transform
#endif
