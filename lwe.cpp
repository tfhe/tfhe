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
#include "lwekeyswitch.h"
#include "lwebootstrappingkey.h"

using namespace std;






// RingLWE
EXPORT void ringLweKeyGen(RingLWEKey* result){
    const int N = result->params->N;
    const int k = result->params->k;
    uniform_int_distribution<int> distribution(0,1);

    for (int i = 0; i < k; ++i)
        for (int j = 0; j < N; ++j)
            result->key[i].coefs[j] = distribution(generator);
}


EXPORT void ringLweSymEncryptZero(RingLWESample* result, double alpha, const RingLWEKey* key){
    const int N = key->params->N;
    const int k = key->params->k;
    
    for (int j = 0; j < N; ++j)
        result->b->coefsT[j] = gaussian32(0, alpha);   
    
    for (int i = 0; i < k; ++i)
    {
	torusPolynomialUniform(&result->a[i]);
	addMulRToTorusPolynomial(result->b, &key->key[i], &result->a[i]);
    }

    result->current_variance = alpha*alpha;
}

EXPORT void ringLweSymEncrypt(RingLWESample* result, TorusPolynomial* message, double alpha, const RingLWEKey* key){
    const int N = key->params->N;
    
    ringLweSymEncryptZero(result, alpha, key);

    for (int j = 0; j < N; ++j)
        result->b->coefsT[j] += message->coefsT[j];   
}

/**
 * encrypts a constant message
 */
EXPORT void ringLweSymEncryptT(RingLWESample* result, Torus32 message, double alpha, const RingLWEKey* key){
    ringLweSymEncryptZero(result, alpha, key);

    result->b->coefsT[0] += message;
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT void ringLwePhase(TorusPolynomial* phase, const RingLWESample* sample, const RingLWEKey* key){
    const int k = key->params->k;

    torusPolynomialCopy(phase, sample->b); // phi = b

    for (int i = 0; i < k; ++i)
        subMulRToTorusPolynomial(phase, &key->key[i], &sample->a[i]);
}


/**
 * This function computes the approximation of the phase 
 * à revoir, surtout le Msize
 */
EXPORT void ringLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int Msize, int N){
    for (int i = 0; i < N; ++i) message->coefsT[i] = approxPhase(phase->coefsT[i], Msize);
}




EXPORT void ringLweSymDecrypt(TorusPolynomial* result, const RingLWESample* sample, const RingLWEKey* key, int Msize){
    ringLwePhase(result, sample, key);
    ringLweApproxPhase(result, result, Msize, key->params->N);
}


EXPORT Torus32 ringLweSymDecryptT(const RingLWESample* sample, const RingLWEKey* key, int Msize){
    TorusPolynomial* phase = new_TorusPolynomial(key->params->N);

    ringLwePhase(phase, sample, key);
    Torus32 result = approxPhase(phase->coefsT[0], Msize);

    delete_TorusPolynomial(phase);
    return result;
}




//Arithmetic operations on RingLWE samples
/** result = (0,0) */
EXPORT void ringLweClear(RingLWESample* result, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
    torusPolynomialClear(result->b);
    result->current_variance = 0.;
}


/** result = sample */
EXPORT void ringLweCopy(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params){
    const int k = params->k;
    const int N = params->N;

    for (int i = 0; i <= k; ++i) 
        for (int j = 0; j < N; ++j)
            result->a[i].coefsT[j] = sample->a[i].coefsT[j];
    
    result->current_variance = sample->current_variance;
}



/** result = (0,mu) */
EXPORT void ringLweNoiselessTrivial(RingLWESample* result, const TorusPolynomial* mu, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
    torusPolynomialCopy(result->b, mu);
    result->current_variance = 0.;
}

/** result = (0,mu) where mu is constant*/
EXPORT void ringLweNoiselessTrivialT(RingLWESample* result, const Torus32 mu, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
    torusPolynomialClear(result->b);
    result->b->coefsT[0]=mu;
    result->current_variance = 0.;
}

/** result = result + sample */
EXPORT void ringLweAddTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) 
	torusPolynomialAddTo(&result->a[i], &sample->a[i]);
    torusPolynomialAddTo(result->b, sample->b);
    result->current_variance += sample->current_variance;
}

/** result = result - sample */
EXPORT void ringLweSubTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) 
	torusPolynomialSubTo(&result->a[i], &sample->a[i]);
    torusPolynomialSubTo(result->b, sample->b);
    result->current_variance += sample->current_variance; 
}

/** result = result + p.sample */
EXPORT void ringLweAddMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) 
	torusPolynomialAddMulZTo(&result->a[i], p, &sample->a[i]);
    torusPolynomialAddMulZTo(result->b, p, sample->b);
    result->current_variance += (p*p)*sample->current_variance;
}

/** result = result - p.sample */
EXPORT void ringLweSubMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params){
    const int k = params->k;

    for (int i = 0; i < k; ++i) 
	torusPolynomialSubMulZTo(&result->a[i], p, &sample->a[i]);
    torusPolynomialSubMulZTo(result->b, p, sample->b);
    result->current_variance += (p*p)*sample->current_variance; 
}


/** result = result + p.sample */
EXPORT void ringLweAddMulRTo(RingLWESample* result, const IntPolynomial* p, const RingLWESample* sample, const RingLWEParams* params){
    const int k = params->k;
    
    for (int i = 0; i <= k; ++i)
       addMulRToTorusPolynomial(result->a+i, p, sample->a+i);	
    result->current_variance += intPolynomialNormSq2(p)*sample->current_variance; 
}











// RingGSW
/** generate a ringgsw key (in fact, a ringlwe key) */
EXPORT void ringGswKeyGen(RingGSWKey* result){
    ringLweKeyGen(&result->ringlwe_key);
}



// support Functions for RingGSW
// Result = 0
EXPORT void ringGSWClear(RingGSWSample* result, const RingGSWParams* params){
    const int kpl = params->kpl;

    for (int p = 0; p < kpl; ++p)
        ringLweClear(&result->all_sample[p], params->ringlwe_params);
}

// Result += H
EXPORT void ringGSWAddH(RingGSWSample* result, const RingGSWParams* params){
    const int k = params->ringlwe_params->k;
    const int l = params->l;
    const Torus32* h = params->h;

    // compute result += H
    for (int bloc = 0; bloc <= k; ++bloc)
	for (int i=0; i<l; i++) 
	    result->bloc_sample[bloc][i].a[bloc].coefsT[0]+=h[i];
}

// Result += mu*H
EXPORT void ringGSWAddMuH(RingGSWSample* result, const IntPolynomial* message, const RingGSWParams* params) {
    const int k = params->ringlwe_params->k;
    const int N = params->ringlwe_params->N;
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
EXPORT void ringGSWAddMuIntH(RingGSWSample* result, const int message, const RingGSWParams* params)
{
    const int k = params->ringlwe_params->k;
    const int l = params->l;
    const Torus32* h = params->h;

    // compute result += H
    for (int bloc = 0; bloc <= k; ++bloc)
	for (int i=0; i<l; i++) 
	    result->bloc_sample[bloc][i].a[bloc].coefsT[0]+=message*h[i];
}

// Result = ringGsw(0)
EXPORT void ringGSWEncryptZero(RingGSWSample* result, double alpha, const RingGSWKey* key){
    const RingLWEKey* rlkey = &key->ringlwe_key;
    const int kpl = key->params->kpl;
        
    for (int p = 0; p < kpl; ++p) // each line of the RingGSWSample is a RingLWESample (on aurait pu appeler la fonction ringLweSymEncrypt)
	//Indeed!!
    {
	ringLweSymEncryptZero(&result->all_sample[p], alpha, rlkey);
    }
}



void TorusPolynomialMulByXaiMinusOne(TorusPolynomial* result, int a, const TorusPolynomial* bk){
    const int N=bk->N;
    Torus32* out=result->coefsT;
    Torus32* in =bk->coefsT; 

    assert(a>=0 && a<2*N);

    if (a<N) {
	for (int i=0;i<a;i++)//sur que i-a<0
	    out[i]= -in[i-a+N]-in[i];
	for (int i=a;i<N;i++)//sur que N>i-a>=0
	    out[i]= in[i-a]-in[i];
    } else {
	const int aa=a-N;
	for (int i=0;i<aa;i++)//sur que i-a<0
	    out[i]= in[i-aa+N]-in[i];
	for (int i=aa;i<N;i++)//sur que N>i-a>=0
	    out[i]= -in[i-aa]-in[i];
    }
}

void TorusPolynomialMulByXai(TorusPolynomial* result, int a, const TorusPolynomial* bk){
    const int N=bk->N;
    Torus32* out=result->coefsT;
    Torus32* in =bk->coefsT; 

    assert(a>=0 && a<2*N);

    if (a<N) {
	for (int i=0;i<a;i++)//sur que i-a<0
	    out[i]= -in[i-a+N];
	for (int i=a;i<N;i++)//sur que N>i-a>=0
	    out[i]= in[i-a];
    } else {
	const int aa=a-N;
	for (int i=0;i<aa;i++)//sur que i-a<0
	    out[i]= in[i-aa+N];
	for (int i=aa;i<N;i++)//sur que N>i-a>=0
	    out[i]= -in[i-aa];
    }
}




//mult externe de X^ai-1 par bki
EXPORT void ringLWEMulByXaiMinusOne(RingLWESample* result, int ai, const RingLWESample* bk, const RingLWEParams* params){
    const int k=params->k;
    for(int i=0;i<=k;i++)
        TorusPolynomialMulByXaiMinusOne(&result->a[i],ai,&bk->a[i]);
}

//mult externe de X^{a_i} par bki
EXPORT void ringGSWMulByXaiMinusOne(RingGSWSample* result, int ai, const RingGSWSample* bk, const RingGSWParams* params){
    const RingLWEParams* par=params->ringlwe_params;
    const int kpl=params->kpl;
    for (int i=0;i<kpl;i++)
        ringLWEMulByXaiMinusOne(&result->all_sample[i],ai,&bk->all_sample[i],par);
}

//Update l'accumulateur ligne 5 de l'algo toujours
//void ringLWEDecompH(IntPolynomial* result, const RingLWESample* sample,const RingGSWParams* params);	
EXPORT void ringLWEExternMulRGSWTo(RingLWESample* accum, const RingGSWSample* sample,const RingGSWParams* params){
    const RingLWEParams* par=params->ringlwe_params;
    const int N=par->N;
    const int kpl=params->kpl;
    IntPolynomial* dec =new_IntPolynomial_array(kpl,N);

    ringLWEDecompH(dec,accum,params);
    ringLweClear(accum,par);
    for (int i=0; i<kpl;i++) 
        ringLweAddMulRTo(accum,&dec[i],&sample->all_sample[i],par);

    delete_IntPolynomial_array(kpl, dec);
}

//crée la clé de KeySwitching
EXPORT void lweCreateKeySwitchKey(LWEKeySwitchKey* result, const LWEKey* in_key, const LWEKey* out_key){
    const int n=result->n;
    const int basebit=result->basebit;
    const int l=result->l;
    const int base=result->base;

    cerr << "create_keyswitch: " << n << " " << l << " " << base << " ("<< n*l*base*out_key->params->n*4 << "bytes)" << endl;
    cerr << "  with noise stdev: " << out_key->params->alpha_min << endl;
    for(int i=0;i<n;i++){
    	for(int j=0;j<l;j++){
    	    for(int k=0;k<base;k++){
		Torus32 x=(in_key->key[i]*k)*(1<<(32-(j+1)*basebit));
		lweSymEncrypt(&result->ks[i][j][k],x,out_key->params->alpha_min,out_key);
    	    }
    	}
    }
}

//sample=(a',b')
EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample* sample){
    const LWEParams* par=ks->out_params;
    const int n=ks->in_params->n;
    const int Bl=ks->basebit;
    const int l=ks->l;
    const uint32_t mask=ks->mask;

    lweNoiselessTrivial(result,sample->b,par);
    for (int i=0;i<n;i++){
    	uint32_t ai=sample->a[i];
    	for (int j=0;j<l;j++){
    	    uint32_t aij=(ai>>(32-(j+1)*Bl))& mask;
    	    lweSubTo(result,&ks->ks[i][j][aij],par);	
    	}
    }
}


/**
 * encrypts a poly message
 */
EXPORT void ringGswSymEncrypt(RingGSWSample* result, const IntPolynomial* message, double alpha, const RingGSWKey* key){
    ringGSWEncryptZero(result, alpha, key);
    ringGSWAddMuH(result, message, key->params);
}


/**
 * encrypts a constant message
 */
EXPORT void ringGswSymEncryptInt(RingGSWSample* result, const int message, double alpha, const RingGSWKey* key){
    ringGSWEncryptZero(result, alpha, key);
    ringGSWAddMuIntH(result, message, key->params);
}


/**
 * encrypts a message = 0 ou 1
 */
EXPORT void ringGSWEncryptB(RingGSWSample* result, const int message, double alpha, const RingGSWKey* key){
    ringGSWEncryptZero(result, alpha, key);
    if (message == 1)
        ringGSWAddH(result, key->params);    
}




/*
// à revoir
EXPORT void ringGswPhase(TorusPolynomial* phase, const RingGSWSample* sample, const RingGSWKey* key){
    const int k = key->params->ringlwe_params->k;

    torusPolynomialCopy(phase, sample[0]->b); // phi = b

    for (int i = 0; i < k; ++i)
        subMulRToTorusPolynomial(phase, &key->key[i], &sample[0]->a[i]);
}

// à revoir
EXPORT void ringGswApproxPhase(IntPolynomial* message, const TorusPolynomial* phase, int Msize, int N){
    for (int i = 0; i < N; ++i) message->coefsT[i] = modSwitchFromTorus32(phase->coefsT[i], Msize);
}
*/

// à revoir
EXPORT void ringGswSymDecrypt(IntPolynomial* result, const RingGSWSample* sample, const RingGSWKey* key, const int Msize){
    const RingGSWParams* params = key->params;
    const RingLWEParams* rlwe_params = params->ringlwe_params;
    const int N = rlwe_params->N;
    const int l = params->l;
    const int k = rlwe_params->k;
    TorusPolynomial* testvec = new_TorusPolynomial(N);
    TorusPolynomial* tmp = new_TorusPolynomial(N);
    IntPolynomial* decomp = new_IntPolynomial_array(l,N);
    
    const Torus32 indic = modSwitchToTorus32(1, Msize);
    torusPolynomialClear(testvec);
    testvec->coefsT[0]=indic;
    Torus32PolynomialDecompH(decomp, testvec, params);

    torusPolynomialClear(testvec);
    for (int i=0; i<l; i++) {
	for (int j=1; j<N; j++) assert(decomp[i].coefs[j]==0);
	ringLwePhase(tmp, &sample->bloc_sample[k][i], &key->ringlwe_key);
	addMulRToTorusPolynomial(testvec, decomp+i, tmp);
    }
    for (int i=0; i<N; i++)
	result->coefs[i]=modSwitchFromTorus32(testvec->coefsT[i], Msize);
    
    delete_TorusPolynomial(testvec);
    delete_TorusPolynomial(tmp);
    delete_IntPolynomial_array(l,decomp);
}

/*
// à revoir
EXPORT int ringGswSymDecryptInt(const RingGSWSample* sample, const RingGSWKey* key){
    TorusPolynomial* phase = new_TorusPolynomial(key->params->ringlwe_params->N);

    ringGswPhase(phase, sample, key);
    int result = modSwitchFromTorus32(phase->coefsT[0], Msize);

    delete_TorusPolynomial(phase);
    return result;
}
*/
//do we really decrypt GSW samples?
// EXPORT void ringGSWMulByXaiMinusOne(GSW* result, int ai, const GSW* bk);
// EXPORT void ringLWEExternMulRLWETo(RLWE* accum, GSW* a); //  accum = a \odot accum


//fonction de decomposition
EXPORT void ringLWEDecompH(IntPolynomial* result, const RingLWESample* sample, const RingGSWParams* params){
    const int k = params->ringlwe_params->k;
    const int l = params->l;

    for (int i = 0; i <= k; ++i) // b=a[k]
        Torus32PolynomialDecompH(result+(i*l), &sample->a[i], params);
}


EXPORT void Torus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const RingGSWParams* params){
    const int N = params->ringlwe_params->N;
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
    

//TODO: Ilaria.
EXPORT void ringGSWExternProduct(RingLWESample* result, const RingGSWSample* a, const RingLWESample* b, const RingGSWParams* params){
    const RingLWEParams* parlwe = params->ringlwe_params;
    const int N = parlwe->N;
    const int kpl = params->kpl;
    IntPolynomial* dec = new_IntPolynomial_array(kpl,N);
    
    ringLWEDecompH(dec, b, params);
    
    ringLweClear(result, parlwe);
    for (int i = 0; i < kpl; i++) 
        ringLweAddMulRTo(result, &dec[i], &a->all_sample[i], parlwe);

    result->current_variance += b->current_variance; //todo + the error term?

    delete_IntPolynomial_array(kpl, dec);
}






//TODO: mettre les mêmes fonctions arithmétiques que pour LWE
//      pour les opérations externes, prévoir int et intPolynomial

//extractions RingLWE -> LWE
EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void ringLweExtractSample(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> SemiRingGSW
EXPORT void ringGswExtractKey(SemiRingGSWSample* result, const RingGSWKey* key);
EXPORT void ringGswExtractSample(RingLWESample* result, const RingGSWSample* x);


/*//calcule l'arrondi inférieur d'un élément Torus32
int bar(uint64_t b, uint64_t Nx2){
uint64_t xx=b*Nx2+(1l<<31);
return (xx>>32)%Nx2;
}*/



EXPORT void sampleExtract(LWESample* result, const RingLWESample* x, const LWEParams* params,  const RingLWEParams* rparams) {
    const int N = rparams->N;
    const int k = rparams->k;
    const int n = params->n;
    assert(n == k*N);
    for (int i=0; i<k; i++) {
    	for (int j=0; j<N; j++)
    	    result->a[i*N+j]=x->a[i].coefsT[j];
    }
    result->b=x->b->coefsT[0];
}

#ifndef NDEBUG
const RingLWEKey* debug_accum_key;
const LWEKey* debug_extract_key;
const LWEKey* debug_in_key;
#endif

//LWE to LWE Single gate bootstrapping
//TODO: Malika
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x){
    const Torus32 ab=(mu0+mu1)/2;
    const Torus32 aa=mu0-ab; //it is important to define it like this and not (mu0-mu1)/2!
    const RingGSWParams* bk_params = bk->bk_params;
    const RingLWEParams* accum_params = bk->accum_params;
    const LWEParams* extract_params = &accum_params->extracted_lweparams;
    const LWEParams* in_params = bk->in_out_params;
    const int N=accum_params->N;
    const int Ns2=N/2;
    const int Nx2= 2*N;
    const int n = in_params->n;
    
    TorusPolynomial* testvect=new_TorusPolynomial(N);
    TorusPolynomial* testvectbis=new_TorusPolynomial(N);
#ifndef NDEBUG
    Torus32 ph = lwePhase(x, debug_in_key);
    printf("Phase before loop: %d\n",ph);
    printf("aa (enc 0) before loop: %d\n",aa);
#endif

    int barb=modSwitchFromTorus32(x->b,Nx2);
    //je definis le test vector (multiplié par a inclus !
    for (int i=0;i<Ns2;i++)
	   testvect->coefsT[i]=aa;
    for (int i=Ns2;i<N;i++)
	   testvect->coefsT[i]=-aa;
    TorusPolynomialMulByXai(testvectbis, barb, testvect);

    RingLWESample* acc = new_RingLWESample(accum_params);
    ringLweNoiselessTrivial(acc, testvectbis, accum_params);
//#ifndef NDEBUG
//    ringLweSymDecrypt(testvectbis, acc, accum_params);
//
//    int accum
//    Torus32 ph = lwePhase(x, debug_in_key);
//    printf("Phase before loop: %d\n",ph);
//    printf("aa (enc 0) before loop: %d\n",aa);
//#endif
    RingGSWSample* temp = new_RingGSWSample(bk_params);
    for (int i=0; i<n; i++) {
	int bara=modSwitchFromTorus32(-x->a[i],Nx2);
	if (bara==0) continue; //indeed, this is an easy case!
	ringGSWMulByXaiMinusOne(temp, bara, bk->bk+i, bk_params);
	ringGSWAddH(temp, bk->bk_params);
	ringLWEExternMulRGSWTo(acc, temp, bk_params);
    }
#ifndef NDEBUG
    ringLwePhase(testvectbis, acc, debug_accum_key);
    printf("Phase after loop: %d\n",testvectbis->coefsT[0]);
#endif
    LWESample* u = new_LWESample(extract_params);
    sampleExtract(u, acc, extract_params, accum_params);
#ifndef NDEBUG
    ph = lwePhase(u, debug_extract_key);
    printf("Phase after extract: %d\n",ph);
#endif
    u->b += ab;
#ifndef NDEBUG
    ph = lwePhase(u, debug_extract_key);
    printf("Phase after add average: %d\n",ph);
#endif
    lweKeySwitch(result, bk->ks, u);
#ifndef NDEBUG
    ph = lwePhase(u, debug_in_key);
    printf("Phase after keySwitch: %d\n",ph);
#endif

    delete_LWESample(u);
    delete_RingGSWSample(temp);
    delete_RingLWESample(acc);
    delete_TorusPolynomial(testvectbis);
    delete_TorusPolynomial(testvect);
}

EXPORT void createBootstrappingKey(
	LWEBootstrappingKey* bk, 
	const LWEKey* key_in, 
	const RingGSWKey* rgsw_key) {
    assert(bk->bk_params==rgsw_key->params);
    assert(bk->in_out_params==key_in->params);

    const LWEParams* in_out_params = bk->in_out_params; 
    const RingGSWParams* bk_params = bk->bk_params;
    const RingLWEParams* accum_params = bk_params->ringlwe_params;
    const LWEParams* extract_params = &accum_params->extracted_lweparams;
    
    //LWEKeySwitchKey* ks; ///< the keyswitch key (s'->s)
    const RingLWEKey* accum_key = &rgsw_key->ringlwe_key;
    LWEKey* extracted_key = new_LWEKey(extract_params);
    ringLweExtractKey(extracted_key, accum_key);
    lweCreateKeySwitchKey(bk->ks, extracted_key, key_in);
    delete_LWEKey(extracted_key);
    
    //RingGSWSample* bk; ///< the bootstrapping key (s->s")
    int* kin = key_in->key;
    const double alpha = accum_params->alpha_min;
    const int n = in_out_params->n;
    const int kpl = bk_params->kpl;
    const int k = accum_params->k;
    const int N = accum_params->N;
    cout << "create the bootstrapping key bk ("  << "  " << n*kpl*(k+1)*N*4 << " bytes)" << endl;
    cout << "  with noise_stdev: " << alpha << endl;
    for (int i=0; i<n; i++) {
	ringGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
    }
}
//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
EXPORT void lweNand(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweOr(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweAnd(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweXor(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b, const LWESample* c);
EXPORT void lweNot(LWESample* result, LWESample* a);


//leveled functions

//LWE to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);



// EXPORT void ringGswPolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

//extractions Ring LWE -> LWE
EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey* key) //sans doute un param supplémentaire
{
    const int N = key->params->N;
    const int k = key->params->k;
    const int n = result->params->n;
    assert(n == k*N);
    for (int i=0; i<k; i++) {
	    result->key[i*N]=key->key[i].coefs[0];
    	for (int j=1; j<N; j++)
    	    result->key[i*N+j]=-key->key[i].coefs[N-j];
    }
}
    


//extraction RingGSW -> GSW
// EXPORT void gswKeyExtract(RingLWEKey* result, const RingGSWKey* key); //sans doute un param supplémentaire
// EXPORT void gswSampleExtract(RingLWESample* result, const RingGSWSample* x);

//bootstrapping
// EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, double mu1, double mu0, const LWESample* x);
