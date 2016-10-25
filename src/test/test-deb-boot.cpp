#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <ccomplex>
#include <sys/time.h>
#include "tfhe.h"
#include "polynomials.h"
#include "lwesamples.h"
#include "lwekey.h"
#include "lweparams.h"
#include "tlwe.h"
#include "tgsw.h"
#include "lwebootstrappingkey.h"



using namespace std;








void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}


EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
EXPORT void tfhe_createLweBootstrappingKeyFFT(
	LweBootstrappingKeyFFT* bk, 
	const LweKey* key_in, 
	const TGswKey* rgsw_key);
EXPORT void tfhe_bootstrapFFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
EXPORT void tfhe_createLweBootstrappingKey(
    LweBootstrappingKey* bk, 
    const LweKey* key_in, 
    const TGswKey* rgsw_key);




EXPORT void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params);
EXPORT void tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params);
EXPORT void tGswToFFTConvert(TGswSampleFFT* result, const TGswSample* source, const TGswParams* params);
EXPORT void tGswFromFFTConvert(TGswSample* result, const TGswSampleFFT* source, const TGswParams* params);
EXPORT void tGswFFTMulByXaiMinusOne(TGswSampleFFT* result, const int ai, const TGswSampleFFT* bki, const TGswParams* params);
EXPORT void tGswFFTAddH(TGswSampleFFT* result, const TGswParams* params);
EXPORT void tGswFFTExternMulToTLwe(TLweSampleFFT* accum, TGswSampleFFT* gsw, const TGswParams* params);












// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************



#ifndef NDEBUG
extern const TLweKey* debug_accum_key;
extern const LweKey* debug_extract_key;
extern const LweKey* debug_in_key;
#endif


int main(int argc, char** argv) {

#ifndef NDEBUG
    cout << "DEBUG MODE!" << endl;
#endif

    const int N = 1024;

    const int k = 1;
    const int n = 500;
    const int l_bk = 3;
    const int Bgbit_bk = 10;
    const double alpha_in = 5e-4;
    const double alpha_bk = 1e-8;
    
    LweParams* in_params = new_LweParams(n, alpha_in, 1./16.);
    TLweParams* accum_params = new_TLweParams(N, k, alpha_bk, 1./16.);
    TGswParams* bk_params = new_TGswParams(l_bk, Bgbit_bk, accum_params);
    
    LweKey* key = new_LweKey(in_params);
    lweKeyGen(key);

    TGswKey* key_bk = new_TGswKey(bk_params);
    tGswKeyGen(key_bk);

    // bk used for tfhe_bootstrapFFT, bk1 used for tfhe_bootstrap
    LweBootstrappingKeyFFT* bk = new_LweBootstrappingKeyFFT(in_params, bk_params);
    tfhe_createLweBootstrappingKeyFFT(bk, key, key_bk);
    LweBootstrappingKey* bk1 = new_LweBootstrappingKey(in_params, bk_params);
    for (int i=0; i<n; i++) {
        tGswFromFFTConvert(&bk1->bk[i], &bk->bk[i], bk_params);
    }

    
    // test omega
    /*
    IntPolynomial* X = new_IntPolynomial(N); 
    LagrangeHalfCPolynomial* resultFFT = new_LagrangeHalfCPolynomial(N);
    cplx rFFT;
    for (int i = 0; i < N; ++i) X->coefs[i] = 0;
    X->coefs[1] = 1;

    IntPolynomial_fft(resultFFT, X);
    rFFT = resultFFT->coefsC[0];
    printf("%.20f + i%.20f\n", creal(rFFT), cimag(rFFT));

    double omega_re, omega_im;
    omega_re = cos(M_PI/1024);
    omega_im = sin(M_PI/1024);
    printf("%.20f + i%.20f\n", omega_re, omega_im);

    delete_IntPolynomial(X);
    delete_LagrangeHalfCPolynomial(resultFFT);
    */



    // LweSample* test = new_LweSample(in_params);
    // LweSample* test_out = new_LweSample(in_params);
    LweSample* x = new_LweSample(in_params);
    LweSample* result = new_LweSample(in_params);
    LweSample* result1 = new_LweSample(in_params);
    
    const Torus32 mu1 = modSwitchToTorus32(1,2);
    const Torus32 mu0 = modSwitchToTorus32(0,2);

    Torus32 mu_in = modSwitchToTorus32(1,2);
    lweSymEncrypt(x, mu_in, alpha_in, key);


#ifndef NDEBUG
    debug_accum_key=&key_bk->tlwe_key;
    LweKey* debug_extract_key2=new_LweKey(&accum_params->extracted_lweparams);
    tLweExtractKey(debug_extract_key2, debug_accum_key);
    debug_extract_key=debug_extract_key2;
    debug_in_key=key;
#endif




    cout << "starting bootstrapping..." << endl;

    const Torus32 ab=(mu1+mu0)/2;
    const Torus32 aa = mu0-ab; // aa=(mu1-mu0)/2;
    const LweParams* extract_params = &accum_params->extracted_lweparams;
    const int Ns2=N/2;
    const int Nx2= 2*N;
    
    
    TorusPolynomial* testvect=new_TorusPolynomial(N);
    TorusPolynomial* testvectbis=new_TorusPolynomial(N);


    int barb=modSwitchFromTorus32(x->b,Nx2);
    //je definis le test vector (multiplié par a inclus !
    for (int i=0;i<Ns2;i++)
       testvect->coefsT[i]=aa;
    for (int i=Ns2;i<N;i++)
       testvect->coefsT[i]=-aa;
    torusPolynomialMulByXai(testvectbis, barb, testvect);

    // Accumulateur 
    TLweSample* acc = new_TLweSample(accum_params);
    TLweSample* acc1 = new_TLweSample(accum_params);
    TLweSampleFFT* accFFT = new_TLweSampleFFT(accum_params);

    // acc and accFFt will be used for tfhe_bootstrapFFT, acc1=acc will be used for tfhe_bootstrap
    tLweNoiselessTrivial(acc, testvectbis, accum_params);
    for (int i = 0; i < k+1; ++i)
        for (int j = 0; j < N; ++j)
                acc1->a[i].coefsT[j] = acc->a[i].coefsT[j];
    tLweToFFTConvert(accFFT, acc, accum_params);
    //tLweFromFFTConvert(acc, accFFT, accum_params);
    //for (int j=0; j<N; j++) {
    //	printf("Before start: Acc index %d: %d vs expected %d\n",j,acc->b->coefsT[j],acc->b->coefsT[j]);
    //}

    //verify multiply by


    TGswSample* temp = new_TGswSample(bk_params);
    TGswSampleFFT* tempFFT = new_TGswSampleFFT(bk_params);

//NICOLAS: j'ai ajouté ce bloc
#ifndef NDEBUG
    TorusPolynomial* phase = new_TorusPolynomial(N);
    TorusPolynomial* phase1 = new_TorusPolynomial(N);
    int correctOffset = barb;
#endif

    cout << "starting the test..." << endl;
    // the index 1 is given when we don't use the fft
    for (int i=0; i<n; i++) {
        int bara=modSwitchFromTorus32(-x->a[i],Nx2);
        int bara1=modSwitchFromTorus32(-x->a[i],Nx2);
        
        if (bara!=0) {
            tGswFFTMulByXaiMinusOne(tempFFT, bara, bk->bk+i, bk_params);
            tGswFFTAddH(tempFFT, bk_params);
            tGswFFTExternMulToTLwe(accFFT, tempFFT, bk_params);
            tLweFromFFTConvert(acc, accFFT, accum_params);
        }

        if (bara1!=0) {
            tGswMulByXaiMinusOne(temp, bara1, bk1->bk+i, bk_params);
            tGswAddH(temp, bk_params);
            tGswExternMulToTLwe(acc1, temp, bk_params);
        }

//NICOLAS: et surtout, j'ai ajouté celui-ci!
#ifndef NDEBUG
        //EXPORT void tLwePhase(TorusPolynomial* phase, const TLweSample* sample, const TLweKey* key);
        tLwePhase(phase,acc,debug_accum_key);  //celui-ci, c'est la phase de acc (FFT)
	tLwePhase(phase1,acc1,debug_accum_key); //celui-ci, c'est la phase de acc1 (Normal)
	if (debug_in_key->key[i]==1) correctOffset = (correctOffset+bara1)%Nx2; 
        torusPolynomialMulByXai(testvectbis, correctOffset, testvect); //celui-ci, c'est la phase idéale (calculée sans bruit avec la clé privée)
	for (int j=0; j<N; j++) {
	       printf("Iteration %d, index %d: phase %d vs expected %d vs noiseless %d\n",i,j,phase->coefsT[j],phase1->coefsT[j], testvectbis->coefsT[j]);
	}
#endif

        /*
        for (int j = 0; j < 5; ++j) { //for (int j = 0; j < N; ++j) {
            if (acc1->a[i].coefsT[j] != acc->a[i].coefsT[j])
                cout << i << "," << j << " - " << (acc1->a[i].coefsT[j] - acc->a[i].coefsT[j]) << endl;
        }
        */
        //tLweToFFTConvert(accFFT, acc, accum_params); 
    }
    tLweFromFFTConvert(acc, accFFT, accum_params);




    LweSample* u = new_LweSample(extract_params);
    LweSample* u1 = new_LweSample(extract_params);
    tLweExtractLweSample(u, acc, extract_params, accum_params);
    tLweExtractLweSample(u1, acc1, extract_params, accum_params);
    u->b += ab;
    u1->b += ab;
    
    lweKeySwitch(result, bk->ks, u);
    lweKeySwitch(result1, bk->ks, u1);
    
    
    

    Torus32 mu_out = lweSymDecrypt(result, key, 2);
    Torus32 mu_out1 = lweSymDecrypt(result1, key, 2);
    cout << "end_variance FFT: " << result->current_variance << endl;
    cout << "end_variance: " << result1->current_variance << endl;
    if (mu_in != mu_out) cout << "Erreur tfhe_bootstrapFFT!" << endl;
    if (mu_in != mu_out1) cout << "Erreur tfhe_bootstrap!" << endl;



    delete_LweSample(u1);
    delete_LweSample(u);
    delete_TGswSampleFFT(tempFFT); 
    delete_TGswSample(temp);
    delete_TLweSampleFFT(accFFT);
    delete_TLweSample(acc1);
    delete_TLweSample(acc);
    delete_TorusPolynomial(testvectbis);
    delete_TorusPolynomial(testvect);

    // delete_LweSample(test_out);
    // delete_LweSample(test);
    delete_LweSample(result1);
    delete_LweSample(result);
    delete_LweSample(x);
    delete_LweBootstrappingKey(bk1);
    delete_LweBootstrappingKeyFFT(bk);
    delete_TGswKey(key_bk);
    delete_LweKey(key);
    delete_TGswParams(bk_params);
    delete_TLweParams(accum_params);
    delete_LweParams(in_params);

    return 0;
}
