#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <ccomplex>
#include <sys/time.h>
#include "lwe.h"
#include "multiplication.h"
#include "polynomials.h"
#include "lwesamples.h"
#include "lwekey.h"
#include "lweparams.h"
#include "ringlwe.h"
#include "ringgsw.h"
#include "lwebootstrappingkey.h"



using namespace std;








void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}


EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey* key); //TODO: change the name and put in a .h
EXPORT void createBootstrappingKeyFFT(
	LWEBootstrappingKeyFFT* bk, 
	const LWEKey* key_in, 
	const RingGSWKey* rgsw_key);
EXPORT void bootstrapFFT(LWESample* result, const LWEBootstrappingKeyFFT* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);


EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey* key); //TODO: change the name and put in a .h
EXPORT void createBootstrappingKey(
    LWEBootstrappingKey* bk, 
    const LWEKey* key_in, 
    const RingGSWKey* rgsw_key);
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);




EXPORT void ringLweToFFTConvert(RingLWESampleFFT* result, const RingLWESample* source, const RingLWEParams* params);
EXPORT void ringLweFromFFTConvert(RingLWESample* result, const RingLWESampleFFT* source, const RingLWEParams* params);
EXPORT void ringGswToFFTConvert(RingGSWSampleFFT* result, const RingGSWSample* source, const RingGSWParams* params);
EXPORT void ringGswFromFFTConvert(RingGSWSample* result, const RingGSWSampleFFT* source, const RingGSWParams* params);
EXPORT void ringGSWFFTMulByXaiMinusOne(RingGSWSampleFFT* result, const int ai, const RingGSWSampleFFT* bki, const RingGSWParams* params);
EXPORT void ringGswFFTAddH(RingGSWSampleFFT* result, const RingGSWParams* params);
EXPORT void ringLweFFTExternMulGSWTo(RingLWESampleFFT* accum, RingGSWSampleFFT* gsw, const RingGSWParams* params);












// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************



#ifndef NDEBUG
extern const RingLWEKey* debug_accum_key;
extern const LWEKey* debug_extract_key;
extern const LWEKey* debug_in_key;
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
    
    LWEParams* in_params = new_LWEParams(n, alpha_in, 1./16.);
    RingLWEParams* accum_params = new_RingLWEParams(N, k, alpha_bk, 1./16.);
    RingGSWParams* bk_params = new_RingGSWParams(l_bk, Bgbit_bk, accum_params);
    
    LWEKey* key = new_LWEKey(in_params);
    lweKeyGen(key);

    RingGSWKey* key_bk = new_RingGSWKey(bk_params);
    ringGswKeyGen(key_bk);

    // bk used for bootstrapFFT, bk1 used for bootstrap
    LWEBootstrappingKeyFFT* bk = new_LWEBootstrappingKeyFFT(in_params, bk_params);
    createBootstrappingKeyFFT(bk, key, key_bk);
    LWEBootstrappingKey* bk1 = new_LWEBootstrappingKey(in_params, bk_params);
    for (int i=0; i<n; i++) {
        ringGswFromFFTConvert(&bk1->bk[i], &bk->bk[i], bk_params);
    }

    
    // test omega
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



    // LWESample* test = new_LWESample(in_params);
    // LWESample* test_out = new_LWESample(in_params);
    LWESample* x = new_LWESample(in_params);
    LWESample* result = new_LWESample(in_params);
    LWESample* result1 = new_LWESample(in_params);
    
    const Torus32 mu1 = modSwitchToTorus32(1,2);
    const Torus32 mu0 = modSwitchToTorus32(0,2);

    Torus32 mu_in = modSwitchToTorus32(1,2);
    lweSymEncrypt(x, mu_in, alpha_in, key);


#ifndef NDEBUG
    debug_accum_key=&key_bk->ringlwe_key;
    LWEKey* debug_extract_key2=new_LWEKey(&accum_params->extracted_lweparams);
    ringLweExtractKey(debug_extract_key2, debug_accum_key);
    debug_extract_key=debug_extract_key2;
    debug_in_key=key;
#endif




    cout << "starting bootstrapping..." << endl;

    const Torus32 ab=(mu1+mu0)/2;
    const Torus32 aa = mu0-ab; // aa=(mu1-mu0)/2;
    const LWEParams* extract_params = &accum_params->extracted_lweparams;
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
    TorusPolynomialMulByXai(testvectbis, barb, testvect);

    // Accumulateur 
    RingLWESample* acc = new_RingLWESample(accum_params);
    RingLWESample* acc1 = new_RingLWESample(accum_params);
    RingLWESampleFFT* accFFT = new_RingLWESampleFFT(accum_params);

    // acc and accFFt will be used for bootstrapFFT, acc1=acc will be used for bootstrap
    ringLweNoiselessTrivial(acc, testvectbis, accum_params);
    for (int i = 0; i < k+1; ++i)
        for (int j = 0; j < N; ++j)
                acc1->a[i].coefsT[j] = acc->a[i].coefsT[j];
    ringLweToFFTConvert(accFFT, acc, accum_params);


    RingGSWSample* temp = new_RingGSWSample(bk_params);
    RingGSWSampleFFT* tempFFT = new_RingGSWSampleFFT(bk_params);

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
            ringGSWFFTMulByXaiMinusOne(tempFFT, bara, bk->bk+i, bk_params);
            ringGswFFTAddH(tempFFT, bk_params);
            ringLweFFTExternMulGSWTo(accFFT, tempFFT, bk_params);
            ringLweFromFFTConvert(acc, accFFT, accum_params);
        }

        if (bara1!=0) {
            ringGSWMulByXaiMinusOne(temp, bara1, bk1->bk+i, bk_params);
            ringGSWAddH(temp, bk_params);
            ringLWEExternMulRGSWTo(acc1, temp, bk_params);
        }

//NICOLAS: et surtout, j'ai ajouté celui-ci!
#ifndef NDEBUG
        //EXPORT void ringLwePhase(TorusPolynomial* phase, const RingLWESample* sample, const RingLWEKey* key);
        ringLwePhase(phase,acc,debug_accum_key);  //celui-ci, c'est la phase de acc (FFT)
	ringLwePhase(phase1,acc1,debug_accum_key); //celui-ci, c'est la phase de acc1 (Normal)
	if (debug_in_key->key[i]==1) correctOffset = (correctOffset+bara1)%Nx2; 
        TorusPolynomialMulByXai(testvectbis, correctOffset, testvect); //celui-ci, c'est la phase idéale (calculée sans bruit avec la clé privée)
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
        //ringLweToFFTConvert(accFFT, acc, accum_params); 
    }
    ringLweFromFFTConvert(acc, accFFT, accum_params);




    LWESample* u = new_LWESample(extract_params);
    LWESample* u1 = new_LWESample(extract_params);
    sampleExtract(u, acc, extract_params, accum_params);
    sampleExtract(u1, acc1, extract_params, accum_params);
    u->b += ab;
    u1->b += ab;
    
    lweKeySwitch(result, bk->ks, u);
    lweKeySwitch(result1, bk->ks, u1);
    
    
    

    Torus32 mu_out = lweSymDecrypt(result, key, 2);
    Torus32 mu_out1 = lweSymDecrypt(result1, key, 2);
    cout << "end_variance FFT: " << result->current_variance << endl;
    cout << "end_variance: " << result1->current_variance << endl;
    if (mu_in != mu_out) cout << "Erreur bootstrapFFT!" << endl;
    if (mu_in != mu_out1) cout << "Erreur bootstrap!" << endl;



    delete_LWESample(u1);
    delete_LWESample(u);
    delete_RingGSWSampleFFT(tempFFT); 
    delete_RingGSWSample(temp);
    delete_RingLWESampleFFT(accFFT);
    delete_RingLWESample(acc1);
    delete_RingLWESample(acc);
    delete_TorusPolynomial(testvectbis);
    delete_TorusPolynomial(testvect);

    // delete_LWESample(test_out);
    // delete_LWESample(test);
    delete_LWESample(result1);
    delete_LWESample(result);
    delete_LWESample(x);
    delete_LWEBootstrappingKey(bk1);
    delete_LWEBootstrappingKeyFFT(bk);
    delete_RingGSWKey(key_bk);
    delete_LWEKey(key);
    delete_RingGSWParams(bk_params);
    delete_RingLWEParams(accum_params);
    delete_LWEParams(in_params);

    return 0;
}
