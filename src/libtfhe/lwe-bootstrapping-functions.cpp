#ifndef TFHE_TEST_ENVIRONMENT
#include <iostream>
#include <cassert>
#include "tfhe.h"
using namespace std;
#define INCLUDE_ALL
#else
#undef EXPORT
#define EXPORT
#endif


/*
// ILA : créér à chaque fois un nouveau sample TLWE ce n'est pas un bon choix
void tfhe_cMux(TLweSample* accum, const TGswSample* bki, const int barai, const TGswParams* bk_params) {
    TLweSample* temp = new_TLweSample(bk_params->tlwe_params);

    // ACC = BKi*[(X^barai-1)*ACC]+ACC
    // temp = (X^barai-1)*ACC
    tLweMulByXaiMinusOne(temp, barai, accum, bk_params->tlwe_params);
    // temp *= BKi
    tGswExternMulToTLwe(temp, bki, bk_params);
    // ACC += temp
    tLweAddTo(accum, temp, bk_params->tlwe_params);

    delete_TLweSample(temp);
}
*/




#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE
#undef INCLUDE_TFHE_BLIND_ROTATE
/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the TLWE sample to multiply
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_blindRotate(TLweSample* accum, 
	const TGswSample* bk, 
	const int* bara,
	const int n,
	const TGswParams* bk_params) {
    //TGswSample* temp = new_TGswSample(bk_params);
    TLweSample* temp = new_TLweSample(bk_params->tlwe_params);

    for (int i=0; i<n; i++) {
	const int barai=bara[i];
	if (barai==0) continue; //indeed, this is an easy case!

    // ACC = BKi*[(X^barai-1)*ACC]+ACC
    // temp = (X^barai-1)*ACC
    tLweMulByXaiMinusOne(temp, barai, accum, bk_params->tlwe_params);
    // temp *= BKi
    tGswExternMulToTLwe(temp, bk+i, bk_params);
    // ACC += temp
    tLweAddTo(accum, temp, bk_params->tlwe_params);

    //tfhe_cMux(accum, bk+i, barai, bk_params);
    /*
	tGswMulByXaiMinusOne(temp, barai, bk+i, bk_params);
	tGswAddH(temp, bk_params);
	tGswExternMulToTLwe(accum, temp, bk_params);
    */
    }
    delete_TLweSample(temp);
    
    //delete_TGswSample(temp);
}
#endif 

#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT
#undef INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT
/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_blindRotateAndExtract(LweSample* result, 
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

    TorusPolynomial* testvectbis=new_TorusPolynomial(N);
    TLweSample* acc = new_TLweSample(accum_params);

    if (barb!=0)
	torusPolynomialMulByXai(testvectbis, _2N-barb, v);
    tLweNoiselessTrivial(acc, testvectbis, accum_params);
    tfhe_blindRotate(acc, bk, bara, n, bk_params);
    tLweExtractLweSample(result, acc, extract_params, accum_params);

    delete_TLweSample(acc);
    delete_TorusPolynomial(testvectbis);
}
#endif 
   

#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP
#undef INCLUDE_TFHE_BOOTSTRAP
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
EXPORT void tfhe_bootstrap(LweSample* result, 
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

    tfhe_blindRotateAndExtract(u, testvect, bk->bk, barb, bara, n, bk_params);

    lweKeySwitch(result, bk->ks, u);

    delete_LweSample(u);
    delete[] bara;
    delete_TorusPolynomial(testvect);
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
#undef INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
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
    //const int kpl = bk_params->kpl;
    //const int k = accum_params->k;
    //const int N = accum_params->N;
    //cout << "create the bootstrapping key bk ("  << "  " << n*kpl*(k+1)*N*4 << " bytes)" << endl;
    //cout << "  with noise_stdev: " << alpha << endl;
    for (int i=0; i<n; i++) {
	tGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
    }
}
#endif


#undef INCLUDE_ALL
