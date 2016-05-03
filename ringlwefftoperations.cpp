#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "ringlwe.h"
#include "ringgsw.h"
#include "polynomials.h"

using namespace std;


EXPORT void ringLweToFFTConvert(RingLWESampleFFT* result, const RingLWESample* source, const RingLWEParams* params){
    const int k = params->k;
    
    for (int i = 0; i <= k; ++i)
	TorusPolynomial_fft(result->a+i,source->a+i);
}

EXPORT void ringLweFromFFTConvert(RingLWESample* result, const RingLWESampleFFT* source, const RingLWEParams* params){
    const int k = params->k;
    
    for (int i = 0; i <= k; ++i)
	TorusPolynomial_ifft(result->a+i,source->a+i);
}



//Arithmetic operations on RingLWE samples
/** result = (0,0) */
EXPORT void ringLweFFTClear(RingLWESampleFFT* result, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i <= k; ++i) clearLagrangeHalfCPolynomial(&result->a[i]);
    result->current_variance = 0.;
}

/** result = (0,mu) */
EXPORT void ringLweFFTNoiselessTrivial(RingLWESampleFFT* result, const TorusPolynomial* mu, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i <= k; ++i) clearLagrangeHalfCPolynomial(&result->a[i]);
    result->current_variance = 0.;
}

/** result = (0,mu) where mu is constant*/
EXPORT void ringLweFFTNoiselessTrivialT(RingLWESampleFFT* result, const Torus32 mu, const RingLWEParams* params){
    const int k = params->k;
    const int Ns2 = params->N/2;
    cplx* b = result->b->coefsC;
    const cplx muc = t32tod(mu);
    

    for (int i = 0; i < k; ++i) 
	clearLagrangeHalfCPolynomial(&result->a[i]);
    for (int j=0; j<Ns2; j++)
	b[j]=muc;
    result->current_variance = 0.;
}

/** result = result + sample */
EXPORT void ringLweFFTAddTo(RingLWESampleFFT* result, const RingLWESampleFFT* sample, const RingLWEParams* params);
//Let's postpone this to make sure we actually need it
//{
//    int k = params->k;
//
//    for (int i = 0; i < k; ++i) 
//	AddToLagrangeHalfCPolynomial(&result->a[i], &sample->a[i]);
//    AddToLagrangeHalfCPolynomial(result->b, sample->b);
//    result->current_variance += sample->current_variance; //à revoir//OK si c'est la variance
//}

/** result = result - sample */
EXPORT void ringLweFFTSubTo(RingLWESampleFFT* result, const RingLWESampleFFT* sample, const RingLWEParams* params);

/** result = result + p.sample */
EXPORT void ringLweFFTAddMulZTo(RingLWESampleFFT* result, int p, const RingLWESampleFFT* sample, const RingLWEParams* params);
//Let's postpone this to make sure we actually need it
//{
//    int k = params->k;
//
//    for (int i = 0; i < k; ++i) 
//	torusPolynomialAddMulZTo(&result->a[i], p, &sample->a[i]);
//    torusPolynomialAddMulZTo(result->b, p, sample->b);
//    result->current_variance += (p*p)*sample->current_variance;
//}

/** result = result - p.sample */
EXPORT void ringLweFFTSubMulZTo(RingLWESampleFFT* result, int p, const RingLWESampleFFT* sample, const RingLWEParams* params);


EXPORT void ringLweFFTAddMulRTo(RingLWESampleFFT* result, const LagrangeHalfCPolynomial* p, const RingLWESampleFFT* sample, const RingLWEParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
	LagrangeHalfCPolynomial_addmul(result->a+i,p,sample->a+i);
}

EXPORT void ringLweFFTMulR(RingLWESampleFFT* result, const LagrangeHalfCPolynomial* p, const RingLWESampleFFT* sample, const RingLWEParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
	LagrangeHalfCPolynomial_mul(result->a+i,p,sample->a+i);
}

EXPORT void ringLweFFTSubMulRTo(RingLWESampleFFT* result, const LagrangeHalfCPolynomial* p, const RingLWESampleFFT* sample, const RingLWEParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
	LagrangeHalfCPolynomial_submul(result->a+i,p,sample->a+i);
}

    
EXPORT void ringGswToFFTConvert(RingGSWSampleFFT* result, const RingGSWSample* source, const RingGSWParams* params) {
    const int kpl = params->kpl;
    
    for (int p=0; p<kpl; p++)
	ringLweToFFTConvert(result->all_samples+p, source->all_sample+p, params->ringlwe_params);
}

EXPORT void ringGswFromFFTConvert(RingGSWSample* result, const RingGSWSampleFFT* source, const RingGSWParams* params){
    const int kpl = params->kpl;
    
    for (int p=0; p<kpl; p++)
	ringLweFromFFTConvert(result->all_sample+p, source->all_samples+p, params->ringlwe_params);
}

struct GlobalsVars {
    LagrangeHalfCPolynomial* XaiMinusOneFFT;
    LagrangeHalfCPolynomial* hlsPolFFT;
    uint32_t* decompOffset1;
    uint32_t* decompOffset2;
    uint32_t* decompModMask;
};
GlobalsVars globals;



EXPORT void ringGswFFTMulXaiMinusOne(RingGSWSampleFFT* result, const int ai, const RingGSWSampleFFT* sample, const RingGSWParams* params) {
    const LagrangeHalfCPolynomial* xaim1 = globals.XaiMinusOneFFT+ai;
    const int kpl = params->kpl;

    for (int p=0; p<kpl; p++)
	ringLweFFTMulR(result->all_samples+p, xaim1, sample->all_samples+p, params->ringlwe_params);
}

EXPORT void ringGswFFTAddH(RingGSWSampleFFT* result, const RingGSWParams* params) {
    const LagrangeHalfCPolynomial* hls = globals.hlsPolFFT;
    const int k = params->ringlwe_params->k;
    const int l = params->l;

    for (int i=0; i<=k; i++)
	for (int j=0; j<l; j++)
	    LagrangeHalfCPolynomial_addto(result->sample[i][j].a+i, hls+j);
}

EXPORT void ringGswFFTClear(RingGSWSampleFFT* result, const RingGSWParams* params) {
    const int kpl = params->kpl;

    for (int p=0; p<kpl; p++)
	ringLweFFTClear(result->all_samples+p, params->ringlwe_params);
}    

EXPORT void LagrangeHalfCPolynomial_decompH(LagrangeHalfCPolynomial* reps, const LagrangeHalfCPolynomial* pol, const int BgBits, const int l) {
    static uint32_t offset1 = globals.decompOffset1[BgBits];
    static int32_t offset2 = globals.decompOffset2[BgBits];
    static uint32_t mask = globals.decompModMask[BgBits];
    const int N = pol->N;
    //TODO attention, this prevents parallelization...
    static TorusPolynomial* a = new_TorusPolynomial(N);
    static IntPolynomial* deca = new_IntPolynomial_array(l,N);

    TorusPolynomial_ifft(a,pol);
    for (int i=0; i<N; i++) {
	const uint32_t ai = offset1 + a->coefsT[i];
	for (int j=1; j<l; j++) {
	    uint32_t aid = ai >> (32-j*BgBits);
	    int32_t aidm = aid & mask;
	    aidm -= offset2;
	    deca[j-1].coefs[i]=aidm;
	}
	//Attention, le dernier est un left shift car l*BgBits>=32!
	uint32_t aid = ai << (l*BgBits-32);
	int32_t aidm = aid & mask;
	aidm -= offset2;
	deca[l-1].coefs[i]=aidm;
    }
    for (int j=0; j<l; j++) {
	IntPolynomial_fft(reps+j,deca+j);
    }
}

EXPORT void ringLweFFTExternMulGSWTo(RingLWESampleFFT* accum, RingGSWSampleFFT* gsw, const RingGSWParams* params) {
    const RingLWEParams* ringlwe_params=params->ringlwe_params;
    const int k = ringlwe_params->k;
    const int l = params->l;
    const int kpl = params->kpl;
    const int BgBits = params->Bgbit;
    const int N = ringlwe_params->N;
    //TODO attention, this prevents parallelization...
    LagrangeHalfCPolynomial* decomps=new_LagrangeHalfCPolynomial_array(kpl,N);

    for (int i=0; i<=k; i++)
	LagrangeHalfCPolynomial_decompH(decomps+i*l,accum->a+i, BgBits, l);
    ringLweFFTClear(accum, ringlwe_params);
    for (int p=0; p<kpl; p++)
	ringLweFFTAddMulRTo(accum, decomps+p, gsw->all_samples+p, ringlwe_params);
}


