#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <tfhe.h>
#include <time.h>

// **********************************************************************************
//            This is just an example file to verify that the library is 
//            usable from a C code
// **********************************************************************************


void dieDramatically(const char *message) {
    fputs(message, stderr);
    abort();
}


#ifndef NDEBUG
extern const TLweKey *debug_accum_key;
extern const LweKey *debug_extract_key;
extern const LweKey *debug_in_key;
#endif

int32_t main(int32_t argc, char **argv) {
#ifndef NDEBUG
    printf("DEBUG MODE!\n");
#endif

    const int32_t N = 1024;
    const int32_t k = 1;
    const int32_t n = 500;
    const int32_t l_bk = 3;
    const int32_t Bgbit_bk = 10;
    const int32_t ks_t = 15;
    const int32_t ks_basebit = 1;
    const double alpha_in = 5e-4;
    const double alpha_bk = 9e-9;
    //const int32_t alpha_ks = 1e-6;

    LweParams *params_in = new_LweParams(n, alpha_in, 1. / 16.);
    TLweParams *params_accum = new_TLweParams(N, k, alpha_bk, 1. / 16.);
    TGswParams *params_bk = new_TGswParams(l_bk, Bgbit_bk, params_accum);

    LweKey *key = new_LweKey(params_in);
    lweKeyGen(key);

    TGswKey *key_bk = new_TGswKey(params_bk);
    tGswKeyGen(key_bk);

    LweBootstrappingKey *bk = new_LweBootstrappingKey(ks_t, ks_basebit, params_in, params_bk);
    tfhe_createLweBootstrappingKey(bk, key, key_bk);

    LweSample *test = new_LweSample(params_in);
    LweSample *test_out = new_LweSample(params_in);

    const Torus32 mu = modSwitchToTorus32(1, 4);

    Torus32 mu_in = modSwitchToTorus32(-1, 4);
    lweSymEncrypt(test, mu_in, alpha_in, key);
    printf("in_message: %d\n", mu_in);

#ifndef NDEBUG
    debug_accum_key = &key_bk->tlwe_key;
    LweKey *debug_extract_key2 = new_LweKey(&params_accum->extracted_lweparams);
    tLweExtractKey(debug_extract_key2, debug_accum_key);
    debug_extract_key = debug_extract_key2;
    debug_in_key = key;
#endif

    printf("starting bootstrapping...\n");

    int32_t nbtrials = 50;
    clock_t begin = clock();
    for (int32_t i = 0; i < nbtrials; i++)
        tfhe_bootstrap(test_out, bk, mu, test);
    clock_t end = clock();
    printf("finished bootstrapping in (microsecs)... %lf\n", (double) (end - begin) / (double) (nbtrials));
    Torus32 mu_out = lweSymDecrypt(test_out, key, 4);
    Torus32 phase_out = lwePhase(test_out, key);
    printf("end_variance: %lf\n", test_out->current_variance);
    printf("end_phase: %d\n ", phase_out);
    printf("end_message: %d\n", mu_out);

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
