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


EXPORT void ringLweSymEncrypt(RingLWESample* result, TorusPolynomial* message, double alpha, const RingLWEKey* key){
    const int N = key->params->N;
    const int k = key->params->k;
    
    for (int j = 0; j < N; ++j)
        result->b->coefsT[j] = gaussian32(0, alpha) + message->coefsT[j];   
    
    for (int i = 0; i < k; ++i)
    {
	torusPolynomialUniform(&result->a[i]);
	addMulRToTorusPolynomial(result->b, &key->key[i], &result->a[i]);
    }

    result->current_variance = alpha*alpha;
}

/**
 * encrypts a constant message
 */
EXPORT void ringLweSymEncryptT(RingLWESample* result, Torus32 message, double alpha, const RingLWEKey* key){
    const int N = key->params->N;
    const int k = key->params->k;
    
    for (int j = 0; j < N; ++j)
        result->b->coefsT[j] = gaussian32(0, alpha);
    result->b->coefsT[0]+=message;

    
    for (int i = 0; i < k; ++i)
    {
	torusPolynomialUniform(&result->a[i]);
	addMulRToTorusPolynomial(result->b, &key->key[i], &result->a[i]);
    }

    result->current_variance = alpha*alpha;
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
/** meme chose que RingLWE */
EXPORT void ringGswKeyGen(RingGSWKey* result){
    const int N = result->params->ringlwe_params->N;
    const int k = result->params->ringlwe_params->k;
    uniform_int_distribution<int> distribution(0,1);

    for (int i = 0; i < k; ++i)
        for (int j = 0; j < N; ++j)
            result->key[i].coefs[j] = distribution(generator);
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
    const int kpl = params->kpl;
    const int l = params->l;
    int q; // quotient of p/l
    const Torus32* h = params->h;

    // compute result += H
    for (int p = 0; p < kpl; ++p)
    {
        q = (int) (((double) p)/((double) l)); 
        result->all_sample[p].a[q].coefsT[0] += h[p%l]; // when q=k adds to a[k] = b
    }
}

// Result += mu*H
EXPORT void ringGSWAddMuH(RingGSWSample* result, const IntPolynomial* message, const RingGSWParams* params){
    const int N = params->ringlwe_params->N;
    const int kpl = params->kpl;
    const int l = params->l;
    TorusPolynomial* message_h = new_TorusPolynomial_array(l, N); 
    int q; // quotient of p/l
    const Torus32* h = params->h;
    //const int* message = message->coefs;

    // compute mu*h = mu*[1/Bg,...,1/Bg^l]
    for (int i = 0; i < l; ++i)
        for (int j = 0; j < N; ++j)
            message_h[i].coefsT[j] = message->coefs[j]*h[i]; 

    // compute result += mu*H
    for (int p = 0; p < kpl; ++p)
    {
        q = (int) (((double) p)/((double) l)); 
        torusPolynomialAddTo(&result->all_sample[p].a[q], &message_h[p%l]); // when q=k adds to a[k] = b
    }

    delete_TorusPolynomial_array(l, message_h);
}

// Result += mu*H, mu integer
EXPORT void ringGSWAddMuIntH(RingGSWSample* result, const int message, const RingGSWParams* params){
    const int kpl = params->kpl;
    const int l = params->l;
    int q; // quotient of p/l
    const Torus32* h = params->h;

    // compute result += mu*H
    for (int p = 0; p < kpl; ++p)
    {
        q = (int) (((double) p)/((double) l)); 
        result->all_sample[p].a[q].coefsT[0] += message*h[p%l]; // when q=k adds to a[k] = b
    }
}

// Result = ringGsw(0)
EXPORT void ringGSWEncryptZero(RingGSWSample* result, double alpha, const RingGSWKey* key){
    const int N = key->params->ringlwe_params->N;
    const int k = key->params->ringlwe_params->k;
    const int kpl = key->params->kpl;
        
    for (int p = 0; p < kpl; ++p) // each line of the RingGSWSample is a RingLWESample (on aurait pu appeler la fonction ringLweSymEncrypt)
    {
        for (int j = 0; j < N; ++j)
            result->all_sample[p].b->coefsT[j] = gaussian32(0, alpha);   
    
        for (int i = 0; i < k; ++i)
        {
            torusPolynomialUniform(&result->all_sample[p].a[i]);
            addMulRToTorusPolynomial(result->all_sample[p].b, &key->key[i], &result->all_sample[p].a[i]);
        }
    }
}



void TorusPolynomialMulByXaiMinusOne(TorusPolynomial* result, int a, const TorusPolynomial* bk){
const int N=bk->N;
Torus32* out=result->coefsT;
Torus32* in =bk->coefsT; 

for (int i=0;i<a;i++)//sur que i-a<0
    out[i]= -in[i-a+N]-in[i];

for (int i=a;i<N;i++)//sur que N>i-a>=0
    out[i]= in[i-a]-in[i];
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
}

//crée la clé de KeySwitching
EXPORT void lweCreateKeySwitchKey(LWEKeySwitchKey* result, const LWEKey* in_key, const LWEKey* out_key){
    const int n=result->n;
    //const int B=result->base;
    const int l=result->l;
    const int Bl=result->basebit;

    for(int i=0;i<n;i++){
    	for(int j=0;j<l;j++){
    	    for(int k=0;i<Bl;i++){
        		Torus32 x=(in_key->key[i]*k)*(1<<(32-j*Bl));
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






EXPORT void ringGswSymDecrypt(IntPolynomial* result, const RingGSWSample* sample, const RingGSWKey* key); 
EXPORT int ringGswSymDecryptInt(const RingGSWSample* sample, const RingGSWKey* key); 
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
            uint32_t temp1 = (temp0 >> ((32-p-1)*Bgbit)) &maskMod; // doute
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
    
    for (int i = 0; i < kpl; i++) 
        ringLweAddMulRTo(result, &dec[i], &a->all_sample[i], parlwe);
}






//TODO: mettre les mêmes fonctions arithmétiques que pour LWE
//      pour les opérations externes, prévoir int et intPolynomial

//extractions RingLWE -> LWE
EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void ringLweExtractSample(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> SemiRingGSW
EXPORT void ringGswExtractKey(SemiRingGSWSample* result, const RingGSWKey* key);
EXPORT void ringGswExtractSample(RingLWESample* result, const RingGSWSample* x);


//calcule l'arrondi inférieur d'un élément Torus32
int bar(uint64_t b, uint64_t Nx2){
uint64_t xx=b*Nx2+(1l<<31);
return (xx>>32)%Nx2;
}



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

//LWE to LWE Single gate bootstrapping
//TODO: Malika
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x){
    Torus32 aa=(mu1-mu0)/2;
    const int N=bk->accum_params->N;
    
    const int Ns2=N/2;
    const uint64_t Nx2= 2*N;
    int barb=bar(x->b,Nx2);
    
    RingLWEParams* accum_par=bk->accum_params;
    TorusPolynomial* testvect=new_TorusPolynomial(N);//je definis le test vector (multiplié par a inclus !
    TorusPolynomial* testvectbis=new_TorusPolynomial(N);
    for (int i=0;i<Ns2;i++){
	   testvect->coefsT[i]=aa;
    }    
    for (int i=Ns2;i<N;i++){
	   testvect->coefsT[i]=-aa;
    }
    for (int i=0;i< barb;i++)
	   testvectbis->coefsT[i]=-testvect->coefsT[i-barb+N];
    for (int i=barb;i<N;i++)
	   testvectbis->coefsT[i]=testvect->coefsT[i-barb];

    //RingLWESample* result, const TorusPolynomial* mu, const RingLWEParams* params
    RingLWESample* acc;
    acc=new_RingLWESample(accum_par);
    ringLweNoiselessTrivial(acc,testvect,accum_par);
    for (int i=0;i<N;i++){
int bara=bar(x->a[i],Nx2);
RingGSWSample* temp;
ringGSWMulByXaiMinusOne(temp,bara,bk->bk,bk->bk_params);
ringGSWAddH(temp,bk->bk_params);
ringGSWExternProduct(acc,temp,acc,bk->bk_params);
}
LWESample* u;//=LWESample(bk->params);
sampleExtract(result,acc,bk->params,accum_par);
lweNoiselessTrivial(u,aa,bk->params);
for (int i=0;i<bk->params->n;i++){
result->a[i]=u->a[i]+result->a[i];
  }
result->b=u->b+result->b;
lweKeySwitch(result,bk->ks,result);
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
EXPORT void keyExtract(LWEKey* result, const RingLWEKey* key) //sans doute un param supplémentaire
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
