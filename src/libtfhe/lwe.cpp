#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "lwekeyswitch.h"
#include "lwebootstrappingkey.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"

using namespace std;


#ifndef NDEBUG
const TLweKey* debug_accum_key;
const LweKey* debug_extract_key;
const LweKey* debug_in_key;
#endif

// TGsw
/** generate a tgsw key (in fact, a tlwe key) */
EXPORT void tGswKeyGen(TGswKey* result){
    tLweKeyGen(&result->tlwe_key);
}



// support Functions for TGsw
// Result = 0
EXPORT void tGswClear(TGswSample* result, const TGswParams* params){
    const int kpl = params->kpl;

    for (int p = 0; p < kpl; ++p)
        tLweClear(&result->all_sample[p], params->tlwe_params);
}

// Result += H
EXPORT void tGswAddH(TGswSample* result, const TGswParams* params){
    const int k = params->tlwe_params->k;
    const int l = params->l;
    const Torus32* h = params->h;

    // compute result += H
    for (int bloc = 0; bloc <= k; ++bloc)
    	for (int i=0; i<l; i++) 
    	    result->bloc_sample[bloc][i].a[bloc].coefsT[0]+=h[i];
}

// Result += mu*H
EXPORT void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params) {
    const int k = params->tlwe_params->k;
    const int N = params->tlwe_params->N;
    const int l = params->l;
    const Torus32* h = params->h;
    const int* mu = message->coefs;

    // compute result += H
    for (int bloc = 0; bloc <= k; ++bloc)
	for (int i=0; i<l; i++) {
	    Torus32* target = 
		result->bloc_sample[bloc][i].a[bloc].coefsT;
	    const Torus32 hi=h[i];
	    for (int j=0; j<N; j++) {
		target[j]+=mu[j]*hi;
	    }
	}
}

// Result += mu*H, mu integer
EXPORT void tGswAddMuIntH(TGswSample* result, const int message, const TGswParams* params)
{
    const int k = params->tlwe_params->k;
    const int l = params->l;
    const Torus32* h = params->h;

    // compute result += H
    for (int bloc = 0; bloc <= k; ++bloc)
	for (int i=0; i<l; i++) 
	    result->bloc_sample[bloc][i].a[bloc].coefsT[0]+=message*h[i];
}

// Result = tGsw(0)
EXPORT void tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key){
    const TLweKey* rlkey = &key->tlwe_key;
    const int kpl = key->params->kpl;
        
    for (int p = 0; p < kpl; ++p) // each line of the TGswSample is a TLweSample (on aurait pu appeler la fonction tLweSymEncrypt)
	//Indeed!!
    {
	tLweSymEncryptZero(&result->all_sample[p], alpha, rlkey);
    }
}






//mult externe de X^{a_i} par bki
EXPORT void tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params){
    const TLweParams* par=params->tlwe_params;
    const int kpl=params->kpl;
    for (int i=0;i<kpl;i++)
        tLweMulByXaiMinusOne(&result->all_sample[i],ai,&bk->all_sample[i],par);
}

//Update l'accumulateur ligne 5 de l'algo toujours
//void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	
EXPORT void tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample,const TGswParams* params){
    const TLweParams* par=params->tlwe_params;
    const int N=par->N;
    const int kpl=params->kpl;
    IntPolynomial* dec =new_IntPolynomial_array(kpl,N);

    tGswTLweDecompH(dec,accum,params);
    tLweClear(accum,par);
    for (int i=0; i<kpl;i++) 
        tLweAddMulRTo(accum,&dec[i],&sample->all_sample[i],par);

    delete_IntPolynomial_array(kpl, dec);
}


/**
 * encrypts a poly message
 */
EXPORT void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key){
    tGswEncryptZero(result, alpha, key);
    tGswAddMuH(result, message, key->params);
}


/**
 * encrypts a constant message
 */
EXPORT void tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key){
    tGswEncryptZero(result, alpha, key);
    tGswAddMuIntH(result, message, key->params);
}


/**
 * encrypts a message = 0 ou 1
 */
EXPORT void tGswEncryptB(TGswSample* result, const int message, double alpha, const TGswKey* key){
    tGswEncryptZero(result, alpha, key);
    if (message == 1)
        tGswAddH(result, key->params);    
}




/*
// à revoir
EXPORT void tGswPhase(TorusPolynomial* phase, const TGswSample* sample, const TGswKey* key){
    const int k = key->params->tlwe_params->k;

    torusPolynomialCopy(phase, sample[0]->b); // phi = b

    for (int i = 0; i < k; ++i)
        torusPolynomialSubMulR(phase, &key->key[i], &sample[0]->a[i]);
}

// à revoir
EXPORT void tGswApproxPhase(IntPolynomial* message, const TorusPolynomial* phase, int Msize, int N){
    for (int i = 0; i < N; ++i) message->coefsT[i] = modSwitchFromTorus32(phase->coefsT[i], Msize);
}
*/

// à revoir
EXPORT void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize){
    const TGswParams* params = key->params;
    const TLweParams* rlwe_params = params->tlwe_params;
    const int N = rlwe_params->N;
    const int l = params->l;
    const int k = rlwe_params->k;
    TorusPolynomial* testvec = new_TorusPolynomial(N);
    TorusPolynomial* tmp = new_TorusPolynomial(N);
    IntPolynomial* decomp = new_IntPolynomial_array(l,N);
    
    const Torus32 indic = modSwitchToTorus32(1, Msize);
    torusPolynomialClear(testvec);
    testvec->coefsT[0]=indic;
    tGswTorus32PolynomialDecompH(decomp, testvec, params);

    torusPolynomialClear(testvec);
    for (int i=0; i<l; i++) {
	for (int j=1; j<N; j++) assert(decomp[i].coefs[j]==0);
	tLwePhase(tmp, &sample->bloc_sample[k][i], &key->tlwe_key);
	torusPolynomialAddMulR(testvec, decomp+i, tmp);
    }
    for (int i=0; i<N; i++)
	result->coefs[i]=modSwitchFromTorus32(testvec->coefsT[i], Msize);
    
    delete_TorusPolynomial(testvec);
    delete_TorusPolynomial(tmp);
    delete_IntPolynomial_array(l,decomp);
}

/*
// à revoir
EXPORT int tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key){
    TorusPolynomial* phase = new_TorusPolynomial(key->params->tlwe_params->N);

    tGswPhase(phase, sample, key);
    int result = modSwitchFromTorus32(phase->coefsT[0], Msize);

    delete_TorusPolynomial(phase);
    return result;
}
*/
//do we really decrypt Gsw samples?
// EXPORT void tGswMulByXaiMinusOne(Gsw* result, int ai, const Gsw* bk);
// EXPORT void tLweExternMulRLweTo(RLwe* accum, Gsw* a); //  accum = a \odot accum


//fonction de decomposition
EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample, const TGswParams* params){
    const int k = params->tlwe_params->k;
    const int l = params->l;

    for (int i = 0; i <= k; ++i) // b=a[k]
        tGswTorus32PolynomialDecompH(result+(i*l), &sample->a[i], params);
}


EXPORT void Torus32PolynomialDecompH_old(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params){
    const int N = params->tlwe_params->N;
    const int l = params->l;
    const int Bgbit = params->Bgbit;
    const uint32_t maskMod = params->maskMod;
    const int32_t halfBg = params->halfBg;
    const uint32_t offset = params->offset;
    
    for (int j = 0; j < N; ++j)
    {
        uint32_t temp0 = sample->coefsT[j] + offset;
        for (int p = 0; p < l; ++p)
        {
            uint32_t temp1 = (temp0 >> (32-(p+1)*Bgbit)) &maskMod; // doute
            result[p].coefs[j] = temp1 - halfBg;
        }   
    }
}
EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params){
    const int N = params->tlwe_params->N;
    const int l = params->l;
    const int Bgbit = params->Bgbit;
    uint32_t* buf = (uint32_t*) sample->coefsT;
//#define __AVX2__ //(to test)
#ifndef __AVX2__
    const uint32_t maskMod = params->maskMod;
    const int32_t halfBg = params->halfBg;
    const uint32_t offset = params->offset;
#else
    const uint32_t* maskMod_addr = &params->maskMod;
    const int32_t* halfBg_addr = &params->halfBg;
    const uint32_t* offset_addr = &params->offset;
    //const uint32_t offset = params->offset;
    //const uint32_t maskMod = params->maskMod;
    //const int32_t halfBg = params->halfBg;
#endif	

    //First, add offset to everyone
#ifndef __AVX2__
    for (int j = 0; j < N; ++j) buf[j]+=offset;
#else
    {
	const uint32_t* sit = buf;
	const uint32_t* send = buf+N;
	__asm__ __volatile__ (
		"vpbroadcastd (%2),%%ymm0\n"
		"0:\n"
		"vmovdqu (%0),%%ymm3\n"
		"vpaddd %%ymm0,%%ymm3,%%ymm3\n" // add offset
		"vmovdqu %%ymm3,(%0)\n"
		"addq $32,%0\n"
		"cmpq %1,%0\n"
		"jb 0b\n"
		: "=r"(sit),"=r"(send),"=r"(offset_addr)
		:  "0"(sit), "1"(send), "2"(offset_addr)
		: "%ymm0","%ymm3","memory"
		);
    }
#endif	
    
    //then, do the decomposition (in parallel)
    for (int p = 0; p < l; ++p)
    {
	const int decal = (32-(p+1)*Bgbit);
#ifndef __AVX2__
	int32_t* res_p = result[p].coefs;
	for (int j = 0; j < N; ++j)
	{
	    uint32_t temp1 = (buf[j] >> decal) & maskMod; 
	    res_p[j] = temp1 - halfBg;
	}
#else
	int32_t* dst = result[p].coefs;
	const uint32_t* sit = buf;
	const uint32_t* send = buf+N;
	const int32_t* decal_addr = &decal;
	__asm__ __volatile__ (
		"vpbroadcastd (%4),%%ymm0\n"
		"vpbroadcastd (%5),%%ymm1\n"
		"vmovd (%3),%%xmm2\n"
		"1:\n"
		"vmovdqu (%1),%%ymm3\n"
		"VPSRLD %%xmm2,%%ymm3,%%ymm3\n" // shift by decal
		"VPAND %%ymm1,%%ymm3,%%ymm3\n"  // and maskMod
		"VPSUBD %%ymm0,%%ymm3,%%ymm3\n" // sub halfBg
		"vmovdqu %%ymm3,(%0)\n"
		"addq $32,%0\n"
		"addq $32,%1\n"
		"cmpq %2,%1\n"
		"jb 1b\n"
		: "=r"(dst),"=r"(sit),"=r"(send),"=r"(decal_addr),"=r"(halfBg_addr),"=r"(maskMod_addr)
		:  "0"(dst), "1"(sit), "2"(send), "3"(decal_addr), "4"(halfBg_addr) ,"5"(maskMod_addr)
		: "%ymm0","%ymm1","%ymm2","%ymm3","memory"
		);
	/* // verify that the assembly block was ok
	int32_t* res_p = result[p].coefs;
	for (int j = 0; j < N; ++j)
	{
	    uint32_t temp1 = (buf[j] >> decal) & maskMod; 
	    if (res_p[j] != int32_t(temp1 - halfBg)) {
		fprintf(stderr, "j=%d,buf[j]=%u,decal=%u,mask=%u,halfbg=%d,res_p[j]=%d\n",j,buf[j],decal,maskMod,halfBg,res_p[j]);
		abort();
	    }
	}*/

#endif	
    }

    //finally, remove offset to everyone
#ifndef __AVX2__
    for (int j = 0; j < N; ++j) buf[j]-=offset;
#else
    {
	const uint32_t* sit = buf;
	const uint32_t* send = buf+N;
	__asm__ __volatile__ (
		"vpbroadcastd (%2),%%ymm0\n"
		"2:\n"
		"vmovdqu (%0),%%ymm3\n"
		"vpsubd %%ymm0,%%ymm3,%%ymm3\n" // add offset
		"vmovdqu %%ymm3,(%0)\n"
		"addq $32,%0\n"
		"cmpq %1,%0\n"
		"jb 2b\n"
		"vzeroall\n"
		: "=r"(sit),"=r"(send),"=r"(offset_addr)
		:  "0"(sit), "1"(send), "2"(offset_addr)
		: "%ymm0","%ymm3","memory"
		);
    }
#endif	
}


//TODO: Ilaria.
EXPORT void tGswExternProduct(TLweSample* result, const TGswSample* a, const TLweSample* b, const TGswParams* params){
    const TLweParams* parlwe = params->tlwe_params;
    const int N = parlwe->N;
    const int kpl = params->kpl;
    IntPolynomial* dec = new_IntPolynomial_array(kpl,N);

    tGswTLweDecompH(dec, b, params);

    tLweClear(result, parlwe);
    for (int i = 0; i < kpl; i++) 
	tLweAddMulRTo(result, &dec[i], &a->all_sample[i], parlwe);

    result->current_variance += b->current_variance; //todo + the error term?

    delete_IntPolynomial_array(kpl, dec);
}






//TODO: mettre les mêmes fonctions arithmétiques que pour Lwe
//      pour les opérations externes, prévoir int et intPolynomial

//extractions TLwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
EXPORT void tLweExtractSample(LweSample* result, const TLweSample* x);

//extraction TGsw -> SemiTGsw
EXPORT void tGswExtractKey(SemiTGswSample* result, const TGswKey* key);
EXPORT void tGswExtractSample(TLweSample* result, const TGswSample* x);


/*//calcule l'arrondi inférieur d'un élément Torus32
  int bar(uint64_t b, uint64_t Nx2){
  uint64_t xx=b*Nx2+(1l<<31);
  return (xx>>32)%Nx2;
  }*/



EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams) {
    const int N = rparams->N;
    const int k = rparams->k;
    assert(params->n == k*N);
    for (int i=0; i<k; i++) {
	result->a[i*N]=x->a[i].coefsT[0];
	for (int j=1; j<N; j++)
	    result->a[i*N+j]=-x->a[i].coefsT[N-j];
    }
    result->b=x->b->coefsT[0];
}

//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
EXPORT void lweNand(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweOr(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweAnd(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweXor(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b, const LweSample* c);
EXPORT void lweNot(LweSample* result, LweSample* a);


//leveled functions

//Lwe to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);



// EXPORT void tGswPolyCombination(LweSample* result, const int* combi, const LweSample* samples, const LweParams* params);

//extractions Ring Lwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key) //sans doute un param supplémentaire
{
    const int N = key->params->N;
    const int k = key->params->k;
    assert(result->params->n == k*N);
    for (int i=0; i<k; i++) {
	for (int j=0; j<N; j++)
	    result->key[i*N+j]=key->key[i].coefs[j];
    }
}



//extraction TGsw -> Gsw
// EXPORT void gswKeyExtract(TLweKey* result, const TGswKey* key); //sans doute un param supplémentaire
// EXPORT void gswSampleExtract(TLweSample* result, const TGswSample* x);

//bootstrapping
// EXPORT void tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, double mu1, double mu0, const LweSample* x);
