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

static default_random_engine generator;
static const int64_t _two31 = INT64_C(1) << 31; // 2^31
static const int64_t _two32 = INT64_C(1) << 32; // 2^32
static const double _two32_double = _two32;
static const double _two31_double = _two31;

static uniform_int_distribution<Torus32> uniformTorus32_distrib(INT32_MIN, INT32_MAX);


EXPORT void ringLweToFFTConvert(RingLWESampleFFT* result, const RingLWESample* source, const RingLWEParams* params){
    const int k = params->k;
    
    for (int i = 0; i < k; ++i)
	TorusPolynomial_fft(result->a+i,source->a+i);
    TorusPolynomial_fft(result->b, source->b);
}

EXPORT void ringLweFromFFTConvert(RingLWESample* result, const RingLWESampleFFT* source, const RingLWEParams* params){
    const int k = params->k;
    
    for (int i = 0; i < k; ++i)
	TorusPolynomial_ifft(result->a+i,source->a+i);
    TorusPolynomial_ifft(result->b, source->b);
}



//Arithmetic operations on RingLWE samples
/** result = (0,0) */
EXPORT void RingLweFFTClear(RingLWESampleFFT* result, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) clearLagrangeHalfCPolynomial(&result->a[i]);
    clearLagrangeHalfCPolynomial(result->b);
    result->current_variance = 0.;
}

/** result = (0,mu) */
EXPORT void RingLweFFTNoiselessTrivial(RingLWESampleFFT* result, const TorusPolynomial* mu, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) clearLagrangeHalfCPolynomial(&result->a[i]);
    TorusPolynomial_fft(result->b, mu);
    result->current_variance = 0.;
}

/** result = (0,mu) where mu is constant*/
EXPORT void RingLweFFTNoiselessTrivialT(RingLWESampleFFT* result, const Torus32 mu, const RingLWEParams* params){
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
EXPORT void RingLweFFTAddTo(RingLWESampleFFT* result, const RingLWESampleFFT* sample, const RingLWEParams* params);
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
EXPORT void RingLweFFTSubTo(RingLWESampleFFT* result, const RingLWESampleFFT* sample, const RingLWEParams* params);

/** result = result + p.sample */
EXPORT void RingLweFFTAddMulZTo(RingLWESampleFFT* result, int p, const RingLWESampleFFT* sample, const RingLWEParams* params);
//Let's postpone this to make sure we actually need it
//{
//    int k = params->k;
//
//    for (int i = 0; i < k; ++i) 
//	AddMulZToTorusPolynomial(&result->a[i], p, &sample->a[i]);
//    AddMulZToTorusPolynomial(result->b, p, sample->b);
//    result->current_variance += (p*p)*sample->current_variance;
//}

/** result = result - p.sample */
EXPORT void RingLweFFTSubMulZTo(RingLWESampleFFT* result, int p, const RingLWESampleFFT* sample, const RingLWEParams* params);


EXPORT void RingLweFFTAddMulRTo(RingLWESampleFFT* result, const LagrangeHalfCPolynomial* p, const RingLWESampleFFT* sample, const RingLWEParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
	LagrangeHalfCPolynomial_addmul(result->a+i,p,sample->a+i);
}

EXPORT void RingLweFFTMulR(RingLWESampleFFT* result, const LagrangeHalfCPolynomial* p, const RingLWESampleFFT* sample, const RingLWEParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
	LagrangeHalfCPolynomial_mul(result->a+i,p,sample->a+i);
}

EXPORT void RingLweFFTSubMulRTo(RingLWESampleFFT* result, const LagrangeHalfCPolynomial* p, const RingLWESampleFFT* sample, const RingLWEParams* params) {
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
	RingLweFFTMulR(result->all_samples+p, xaim1, sample->all_samples+p, params->ringlwe_params);
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
	RingLweFFTClear(result->all_samples+p, params->ringlwe_params);
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
    RingLweFFTClear(accum, ringlwe_params);
    for (int p=0; p<kpl; p++)
	RingLweFFTAddMulRTo(accum, decomps+p, gsw->all_samples+p, ringlwe_params);
}


