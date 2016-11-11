#ifndef FAKE_LWE_BOOTSTRAPPING_H
#define FAKE_LWE_BOOTSTRAPPING_H

#include <tfhe.h>
#include "./lwe-keyswitch.h"
struct FakeLweBootstrappingKey{
    const LweParams* in_out_params; ///< paramètre de l'input et de l'output. key: s
    const TGswParams* bk_params; ///< params of the Gsw elems in bk. key: s"
    const TLweParams* accum_params; ///< params of the accum variable key: s"
    const LweParams* extract_params; ///< params after extraction: key: s' 
    TGswSample* bk; ///< the bootstrapping key (s->s")
    LweKeySwitchKey* ks; ///< the keyswitch key (s'->s)

    FakeLweBootstrappingKey(const LweParams* in_out_params, const TGswParams* bk_params) {
	this->in_out_params = in_out_params;
	this->bk_params= bk_params;
	this->accum_params= bk_params->tlwe_params;
	this->extract_params=&accum_params->extracted_lweparams;
	const int n = in_out_params->n;
	const int N = extract_params->n;
	const int kslength = 15;
	const int basebit = 2;

	this->bk=fake_new_TGswSample_array(n,this->bk_params);
	this->ks=new_LweKeySwitchKey(N, kslength, basebit, in_out_params);
    }
    ~FakeLweBootstrappingKey() {
	delete_LweKeySwitchKey(ks);
	fake_delete_TGswSample_array(in_out_params->n,bk);
    }
    FakeLweBootstrappingKey(const LweBootstrappingKey&) = delete;
    void operator=(const FakeLweBootstrappingKey&) = delete;  
};

FakeLweBootstrappingKey* fake(LweBootstrappingKey* key) {
    FakeLweBootstrappingKey* fkey = (FakeLweBootstrappingKey*) key;
    fake(fkey->bk); //test if bk is a fake
    return fkey;
}

const FakeLweBootstrappingKey* fake(const LweBootstrappingKey* key) {
    const FakeLweBootstrappingKey* fkey = (const FakeLweBootstrappingKey*) key;
    fake(fkey->bk); //test if bk is a fake
    return fkey;
}

inline LweBootstrappingKey* fake_new_LweBootstrappingKey(const LweParams* in_out_params, const TGswParams* bk_params) {
    return (LweBootstrappingKey*) new FakeLweBootstrappingKey(in_out_params, bk_params);
}

inline void fake_delete_LweBootstrappingKey(LweBootstrappingKey* bk) {
    FakeLweBootstrappingKey* fbk = fake(bk);
    delete fbk;
}

inline void fake_tfhe_createLweBootstrappingKey(
	LweBootstrappingKey* bk, 
	const LweKey* key_in, 
	const TGswKey* rgsw_key) {
    FakeLweBootstrappingKey* fbk = fake(bk);
    assert(fbk->bk_params==rgsw_key->params);
    assert(fbk->in_out_params==key_in->params);

    const LweParams* in_out_params = fbk->in_out_params; 
    const TGswParams* bk_params = fbk->bk_params;
    const TLweParams* accum_params = bk_params->tlwe_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;

    //LweKeySwitchKey* ks; ///< the keyswitch key (s'->s)
    const TLweKey* accum_key = &rgsw_key->tlwe_key;
    LweKey* extracted_key = new_LweKey(extract_params);
    tLweExtractKey(extracted_key, accum_key);
    fake_lweCreateKeySwitchKey(bk->ks, extracted_key, key_in); //TODO: fake
    delete_LweKey(extracted_key);

    //TGswSample* bk; ///< the bootstrapping key (s->s")
    int* kin = key_in->key;
    const double alpha = accum_params->alpha_min;
    const int n = in_out_params->n;
    //const int kpl = bk_params->kpl;
    //const int k = accum_params->k;
    //const int N = accum_params->N;
    for (int i=0; i<n; i++) {
	fake_tGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
    }
}

/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the (fake) TLWE sample to multiply
 * @param bk An array of n (fakes) TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
inline void fake_tfhe_bootstrapRotate(TLweSample* accum, 
	const TGswSample* bk, 
	const int* bara,
	const int n,
	const TGswParams* bk_params) {
    int offset = 0;
    for (int i=0; i<n; i++) {
	const int si = fake(bk+i)->message->coefs[0];
	const int barai=bara[i];
	if (barai==0 || si==0) continue; //indeed, this is an easy case!
	offset = (offset + barai*si) % (2*bk_params->tlwe_params->N);
    }
    //TODO: make a fake function for that
    torusPolynomialCopy(accum->a,accum->b);
    torusPolynomialMulByXai(accum->b,offset,accum->a);
    torusPolynomialClear(accum->a);
    //TODO update the variance
}

#define USE_FAKE_tfhe_bootstrapRotate \
    inline void tfhe_bootstrapRotate(TLweSample* accum, \
	    const TGswSample* bk, \
	    const int* bara, \
	    const int n, \
	    const TGswParams* bk_params) { \
	fake_tfhe_bootstrapRotate(accum,bk,bara,n,bk_params); \
    }

/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
inline void fake_tfhe_bootstrapRotateExtract(LweSample* result, 
	const TorusPolynomial* v,
	const TGswSample* bk, 
	const int barb,
	const int* bara,
	const int n,
	const TGswParams* bk_params) {
    const TLweParams* accum_params = bk_params->tlwe_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;
    const int N = accum_params->N;
    const int _2N = 2*N;

    int offset = barb;
    for (int i=0; i<n; i++) {
	int si = fake(bk+i)->message->coefs[0];
	offset = (offset + _2N - si*bara[i])%(_2N);
    }
    Torus32 message = offset<N?v->coefsT[offset]:-v->coefsT[offset-N];
    lweNoiselessTrivial(result,message,extract_params);
    result->current_variance=0; //TODO variance
}

#define USE_FAKE_tfhe_bootstrapRotateExtract \
    inline void tfhe_bootstrapRotateExtract(LweSample* result, \
	    const TorusPolynomial* v, \
	    const TGswSample* bk, \
	    const int barb, \
	    const int* bara, \
	    const int n, \
	    const TGswParams* bk_params) { \
	fake_tfhe_bootstrapRotateExtract(result,v,bk,barb,bara,n,bk_params); \
    }


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
inline void fake_tfhe_bootstrap(LweSample* result, 
	const LweBootstrappingKey* bk, 
	Torus32 mu, const LweSample* x){
    const TGswParams* bk_params = bk->bk_params;
    const TLweParams* accum_params = bk->accum_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;
    const LweParams* in_params = bk->in_out_params;
    const int N=accum_params->N;
    const int Nx2= 2*N;
    const int n = in_params->n;

    TorusPolynomial* testvect=new_TorusPolynomial(N);
    int* bara = new int[N];
    LweSample* u = new_LweSample(extract_params);

    int barb=modSwitchFromTorus32(x->b,Nx2);
    for (int i=0; i<n; i++) {
	bara[i]=modSwitchFromTorus32(x->a[i],Nx2);
    }

    //the initial testvec = [mu,mu,mu,...,mu]
    for (int i=0;i<N;i++) testvect->coefsT[i]=mu;

    fake_tfhe_bootstrapRotateExtract(u, testvect, bk->bk, barb, bara, n, bk_params);

    lweKeySwitch(result, bk->ks, u);

    delete_LweSample(u);
    delete[] bara;
    delete_TorusPolynomial(testvect);
}

#if defined INCLUDE_ALL || defined INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
EXPORT void tfhe_createLweBootstrappingKey(
	LweBootstrappingKey* bk, 
	const LweKey* key_in, 
	const TGswKey* rgsw_key) {
    assert(bk->bk_params==rgsw_key->params);
    assert(bk->in_out_params==key_in->params);

    const LweParams* in_out_params = bk->in_out_params; 
    const TGswParams* bk_params = bk->bk_params;
    const TLweParams* accum_params = bk_params->tlwe_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;

    //LweKeySwitchKey* ks; ///< the keyswitch key (s'->s)
    const TLweKey* accum_key = &rgsw_key->tlwe_key;
    LweKey* extracted_key = new_LweKey(extract_params);
    tLweExtractKey(extracted_key, accum_key);
    lweCreateKeySwitchKey(bk->ks, extracted_key, key_in);
    delete_LweKey(extracted_key);

    //TGswSample* bk; ///< the bootstrapping key (s->s")
    int* kin = key_in->key;
    const double alpha = accum_params->alpha_min;
    const int n = in_out_params->n;
    const int kpl = bk_params->kpl;
    const int k = accum_params->k;
    const int N = accum_params->N;
    cout << "create the bootstrapping key bk ("  << "  " << n*kpl*(k+1)*N*4 << " bytes)" << endl;
    cout << "  with noise_stdev: " << alpha << endl;
    for (int i=0; i<n; i++) {
	tGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
    }
}
#endif

#endif // FAKE_LWE_BOOTSTRAPPING_H
