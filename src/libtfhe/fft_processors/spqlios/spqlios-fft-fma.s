	.file	"spqlios-fft-avx.s"
	.text
	.p2align 4
#if !__APPLE__
	.globl	fft
	.type	fft, @function
fft:
#else
	.globl	_fft
_fft:
#endif
//c has size n/2
//void fft(const void* tables, double* c) {

//    FFT_PRECOMP* fft_tables = (FFT_PRECOMP*) tables;
//    const int32_t n = fft_tables->n;
//    const double* trig_tables = fft_tables->trig_tables;

	/* Save registers */
	pushq       %r10
	pushq       %r11
	pushq       %r12
	pushq       %r13
	pushq       %r14
	pushq       %rbx
	
	/* Permute registers for better variable names */
	movq        %rdi, %rax
	movq        %rsi, %rdi      /* rdi: base of the real data CONSTANT */
	
	/* Load struct FftTables fields */
	movq         0(%rax), %rdx  /* rdx: n (logical Size of _fft  = a power of 2, must be at least 4) */
	movq         8(%rax), %r8   /* r8: Base address of trigonometric tables array (CONSTANT) */
	
//    int32_t ns4 = n/4;
//    double* pre = c;     //size n/4
//    double* pim = c+ns4; //size n/4
	movq	%rdx, %r9
	shr	$2,%r9              /* r9: ns4 CONSTANT */
	leaq	(%rdi,%r9,8),%rsi   /* rsi: base of imaginary data CONSTANT */

//    //size 2
//    {
//	//[1  1]
//	//[1 -1]
//	//     [1  1]
//	//     [1 -1]
//	for (int32_t block=0; block<ns4; block+=4) {
//	    double* d0 = pre+block;
//	    double* d1 = pim+block;
//	    tmp0[0]=d0[0];
//	    tmp0[1]=d0[0];
//	    tmp0[2]=d0[2];
//	    tmp0[3]=d0[2];
//	    tmp1[0]=d0[1];
//	    tmp1[1]=-d0[1];
//	    tmp1[2]=d0[3];
//	    tmp1[3]=-d0[3];
//	    add4(d0,tmp0,tmp1);
//	    tmp0[0]=d1[0];
//	    tmp0[1]=d1[0];
//	    tmp0[2]=d1[2];
//	    tmp0[3]=d1[2];
//	    tmp1[0]=d1[1];
//	    tmp1[1]=-d1[1];
//	    tmp1[2]=d1[3];
//	    tmp1[3]=-d1[3];
//	    add4(d1,tmp0,tmp1);
//	}
//    }
	vmovapd     size4negation0(%rip), %ymm15
	vmovapd     size4negation1(%rip), %ymm14
	vmovapd     size4negation2(%rip), %ymm13
	vmovapd     size4negation3(%rip), %ymm12
	
	movq	$0,%rax	/* rax: block */
	movq	%rdi,%r10
	movq	%rsi,%r11
fftsize2loop:
	vmovapd (%r10),%ymm0 /* r0 r1 r2 r3 */
	vmovapd (%r11),%ymm1 /* i0 i1 i2 i3 */
	vshufpd $0,%ymm0,%ymm0,%ymm2  /* r0 r0 r2 r2 */
	vshufpd $15,%ymm0,%ymm0,%ymm3 /* r1 r1 r3 r3 */
	vshufpd $0,%ymm1,%ymm1,%ymm4  /* i0 i0 i2 i2 */
	vshufpd $15,%ymm1,%ymm1,%ymm5 /* i1 i1 i3 i3 */
	vfmadd231pd %ymm3,%ymm12,%ymm2 /* (r0 r0 r2 r2) + (r1 -r1 r3 -r3) */
	vfmadd231pd %ymm5,%ymm12,%ymm4 /* (i0 i0 i2 i2) + (i1 -i1 i3 -i3) */
	vmovapd %ymm2,(%r10)
	vmovapd %ymm4,(%r11)
	/* end of loop */
        leaq 32(%r10),%r10
        leaq 32(%r11),%r11
        addq $4,%rax
	cmpq %r9,%rax
	jb fftsize2loop
	
//
//    //size 4
//    //[1  0  1  0]
//    //[0  1  0 -i]
//    //[1  0 -1  0]
//    //[0  1  0  i]
//    // r0 + r2    i0 + i2
//    // r1 + i3    i1 - r3
//    // r0 - r2    i0 - i2
//    // r1 - i3    i1 + r3
//    {
//	for (int32_t block=0; block<ns4; block+=4) {
//	    double* re = pre+block;
//	    double* im = pim+block;
//	    tmp0[0]=re[0];
//	    tmp0[1]=re[1];
//	    tmp0[2]=re[0];
//	    tmp0[3]=re[1];
//	    tmp1[0]=re[2];
//	    tmp1[1]=im[3];
//	    tmp1[2]=-re[2];
//	    tmp1[3]=-im[3];
//	    tmp2[0]=im[0];
//	    tmp2[1]=im[1];
//	    tmp2[2]=im[0];
//	    tmp2[3]=im[1];
//	    tmp3[0]=im[2];
//	    tmp3[1]=-re[3];
//	    tmp3[2]=-im[2];
//	    tmp3[3]=re[3];
//	    add4(re,tmp0,tmp1);
//	    add4(im,tmp2,tmp3);
//	}
//    }
	movq	$0, %rax
	movq	%rdi,%r10
	movq	%rsi,%r11
fftsize4loop:
	vmovapd (%r10),%ymm0 /* r0 r1 r2 r3 */
	vmovapd (%r11),%ymm1 /* i0 i1 i2 i3 */
	vperm2f128 $32,%ymm0,%ymm0,%ymm4 /* r0 r1 r0 r1 */
	vperm2f128 $32,%ymm1,%ymm1,%ymm5 /* i0 i1 i0 i1 */
	vperm2f128 $49,%ymm0,%ymm0,%ymm6 /* r2 r3 r2 r3 */
	vperm2f128 $49,%ymm1,%ymm1,%ymm7 /* i2 i3 i2 i3 */
	vshufpd $10,%ymm7,%ymm6,%ymm8    /* r2 i3 r2 i3 */
	vshufpd $10,%ymm6,%ymm7,%ymm9    /* i2 r3 i2 r3 */
	vfmadd231pd %ymm8,%ymm13,%ymm4   /* (r0 r1 r0 r1) + (r2 i3 -r2 -i3) */
	vfmadd231pd %ymm9,%ymm14,%ymm5   /* (i0 i1 i0 i1) + (i2 -r3 -i2 r3) */
	vmovapd %ymm4,(%r10)
  	vmovapd %ymm5,(%r11)
        /* end of loop */
        leaq 32(%r10),%r10
        leaq 32(%r11),%r11
        addq $4,%rax
	cmpq %r9,%rax
	jb fftsize4loop


//    
//    //general loop
//    const double* cur_tt = trig_tables;
//    for (int32_t halfnn=4; halfnn<ns4; halfnn*=2) {
//	int32_t nn = 2*halfnn;
//	for (int32_t block=0; block<ns4; block+=nn) {
//	    for (int32_t off=0; off<halfnn; off+=4) {
//		double* re0 = pre + block + off;
//		double* im0 = pim + block + off;
//		double* re1 = pre + block + halfnn + off;
//		double* im1 = pim + block + halfnn + off;
//		const double* tcs = cur_tt+2*off;
//		const double* tsn = tcs+4;
//		dotp4(tmp0,re1,tcs); // re*cos
//		dotp4(tmp1,re1,tsn); // re*sin
//		dotp4(tmp2,im1,tcs); // im*cos
//		dotp4(tmp3,im1,tsn); // im*sin
//		sub4(tmp0,tmp0,tmp3); // re2
//		add4(tmp1,tmp1,tmp2); // im2
//		add4(tmp2,re0,tmp0); // re + re
//		add4(tmp3,im0,tmp1); // im + im
//		sub4(tmp0,re0,tmp0); // re - re
//		sub4(tmp1,im0,tmp1); // im - im
//		copy4(re0,tmp2);
//		copy4(im0,tmp3);
//		copy4(re1,tmp0);
//		copy4(im1,tmp1);
//	    }
//	}
//	cur_tt += nn;
//    }

	movq %r8,%rdx /* rdx: cur_tt */
	movq $4,%rax /* rax: halfnn */
ffthalfnnloop:
	movq $0,%rbx /* rbx: block */
fftblockloop:
	leaq (%rdi,%rbx,8),%r10 /* re0 pointer */
	leaq (%rsi,%rbx,8),%r11 /* im0 pointer */
	leaq (%r10,%rax,8),%r12 /* re1 pointer */
	leaq (%r11,%rax,8),%r13 /* im1 pointer */
	movq %rdx,%r14          /* tcs pointer */
	movq $0,%rcx /* rcx: off */
fftoffloop:
	vmovapd (%r10),%ymm0 /* re0 */
	vmovapd (%r11),%ymm1 /* im0 */
	vmovapd (%r12),%ymm2 /* re1 */
	vmovapd (%r13),%ymm3 /* im1 */
	vmovapd (%r14),%ymm4 /* cos */
	vmovapd 32(%r14),%ymm5 /* sin */
	vmulpd	%ymm2,%ymm4,%ymm6 /* re1.cos */
	vmulpd	%ymm2,%ymm5,%ymm7 /* re1.sin */
        vfnmadd231pd %ymm3,%ymm5,%ymm6 /* re2 = re1.cos - im1.sin */ 
        vfmadd231pd %ymm3,%ymm4,%ymm7  /* im2 = re1.sin + im1.cos */ 
	vsubpd	%ymm6,%ymm0,%ymm2 /* re0 - re2 */
	vsubpd	%ymm7,%ymm1,%ymm3 /* im0 - im2 */
	vaddpd	%ymm6,%ymm0,%ymm0 /* re0 + re2 */
	vaddpd	%ymm7,%ymm1,%ymm1 /* im0 + im2 */
	vmovapd %ymm0,(%r10)
	vmovapd %ymm1,(%r11)
	vmovapd %ymm2,(%r12)
	vmovapd %ymm3,(%r13)
        /* end of off loop */
    	leaq 	32(%r10),%r10
    	leaq	32(%r11),%r11
    	leaq 	32(%r12),%r12
    	leaq 	32(%r13),%r13
	leaq 	64(%r14),%r14
	addq 	$4,%rcx
	cmpq	%rax,%rcx
	jb 	fftoffloop
	/* end of block loop */
	leaq	(%rbx,%rax,2),%rbx
	cmpq	%r9,%rbx
	jb 	fftblockloop
	/* end of halfnn loop */
	shlq	$1,%rax
	leaq	(%rdx,%rax,8),%rdx
	cmpq	%r9,%rax
	jb ffthalfnnloop

//    //multiply by omb^j
//    for (int32_t j=0; j<ns4; j+=4) {
//	const double* r0 = cur_tt+2*j;
//	const double* r1 = r0+4;
//	//(re*cos-im*sin) + i (im*cos+re*sin)
//	double* d0 = pre+j;
//	double* d1 = pim+j;
//	dotp4(tmp0,d0,r0); //re*cos
//	dotp4(tmp1,d1,r0); //im*cos
//	dotp4(tmp2,d0,r1); //re*sin
//	dotp4(tmp3,d1,r1); //im*sin
//	sub4(d0,tmp0,tmp3);
//	add4(d1,tmp1,tmp2);
//    }
//}
	/* cur_tt is at rdx */
	movq $0,%rax /* j */
	movq %rdi,%r10
	movq %rsi,%r11
fftfinalloop:
	vmovapd	(%r10),%ymm0 /* re */
	vmovapd	(%r11),%ymm1 /* im */
	vmovapd (%rdx),%ymm2 /* cos */
	vmovapd 32(%rdx),%ymm3 /* sin */
	vmulpd %ymm0,%ymm2,%ymm4
	vmulpd %ymm0,%ymm3,%ymm5
	vmulpd %ymm1,%ymm2,%ymm6
	vmulpd %ymm1,%ymm3,%ymm7
	vsubpd %ymm7,%ymm4,%ymm0
	vaddpd %ymm6,%ymm5,%ymm1
	vmovapd %ymm0,(%r10)
	vmovapd %ymm1,(%r11)
    	/* end of final loop */
    	leaq	32(%r10),%r10
    	leaq	32(%r11),%r11
    	leaq	64(%rdx),%rdx
	addq	$4,%rax
	cmpq	%r9,%rax
	jb fftfinalloop

	/* Restore registers */
fftend:
	vzeroall
	popq        %rbx
	popq        %r14
	popq        %r13
	popq        %r12
	popq        %r11
	popq        %r10
	retq


/* Constants for YMM */
.balign 32
size4negation0: .double +1.0, +1.0, +1.0, -1.0 /* ymm15 */
size4negation1: .double +1.0, -1.0, -1.0, +1.0 /* ymm14 */
size4negation2: .double +1.0, +1.0, -1.0, -1.0 /* ymm13 */
size4negation3: .double +1.0, -1.0, +1.0, -1.0 /* ymm12 */

#if !__APPLE__
	.size	fft, .-fft
#endif
