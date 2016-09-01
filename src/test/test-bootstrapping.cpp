#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "lwe.h"
#include "multiplication.h"
#include "polynomials.h"
#include "lwesamples.h"
#include "lwekey.h"
#include "lweparams.h"
#include "tlwe.h"
#include "tgsw.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}

EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
EXPORT void createBootstrappingKey(
	LweBootstrappingKey* bk, 
	const LweKey* key_in, 
	const TGswKey* rgsw_key);
EXPORT void bootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


#ifndef NDEBUG
extern const TLweKey* debug_accum_key;
extern const LweKey* debug_extract_key;
extern const LweKey* debug_in_key;
#endif

int main(int argc, char** argv) {
#ifndef NDEBUG
    cout << "DEBUG MODE!" << endl;
#endif
    //const int count = 1000; //number of tests to compare the 3 types of multiplication

    const int N = 1024;
    const int k = 1;
    const int n = 500;
    const int l_bk = 3;
    const int Bgbit_bk = 10;
    //const int ks_basebit = 4;
    const double alpha_in = 5e-4;
    const double alpha_bk = 1e-8;
    //const int alpha_ks = 1e-6;

    LweParams* params_in = new_LweParams(n, alpha_in, 1./16.);
    TLweParams* params_accum = new_TLweParams(N, k, alpha_bk, 1./16.);
    TGswParams* params_bk = new_TGswParams(l_bk, Bgbit_bk, params_accum);

    LweKey* key = new_LweKey(params_in);
    lweKeyGen(key);

    TGswKey* key_bk = new_TGswKey(params_bk);
    tGswKeyGen(key_bk);

    LweBootstrappingKey* bk = new_LweBootstrappingKey(params_in, params_bk);
    createBootstrappingKey(bk, key, key_bk);

    LweSample* test = new_LweSample(params_in);
    LweSample* test_out = new_LweSample(params_in);
    
    const Torus32 mu1 = modSwitchToTorus32(1,2);
    const Torus32 mu0 = modSwitchToTorus32(0,2);

    Torus32 mu_in = modSwitchToTorus32(1,2);
    lweSymEncrypt(test, mu_in, alpha_in, key);
    
#ifndef NDEBUG
    debug_accum_key=&key_bk->tlwe_key;
    LweKey* debug_extract_key2=new_LweKey(&params_accum->extracted_lweparams);
    tLweExtractKey(debug_extract_key2, debug_accum_key);
    debug_extract_key=debug_extract_key2;
    debug_in_key=key;
#endif

    cout << "starting bootstrapping..." << endl;

    int nbtrials = 50;
    clock_t begin = clock();
    for (int i=0; i<nbtrials; i++)
	bootstrap(test_out, bk, mu1, mu0, test);
    clock_t end = clock();
    cout << "finished bootstrapping in (microsecs)... " << (end-begin)/double(nbtrials) << endl;
    Torus32 mu_out = lweSymDecrypt(test_out, key, 2);
    cout << "end_variance: " << test_out->current_variance << endl;

    if (mu_in != mu_out) 
	dieDramatically("et Zut!");


    delete_LweSample(test_out);
    delete_LweSample(test);
    delete_LweBootstrappingKey(bk);
    delete_TGswKey(key_bk);
    delete_LweKey(key);
    delete_TGswParams(params_bk);
    delete_TLweParams(params_accum);
    delete_LweParams(params_in);
    return 0;
}
