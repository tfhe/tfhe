#include <cstdio>
#include <iostream>
#include "tfhe.h"
#include "tfhe_garbage_collector.h"

using namespace std;

struct Exception42 {};

EXPORT void die_dramatically(const char* message) {
    cerr << message << endl;
    abort(); throw Exception42();
}

/** 
 * multiplies by sqrt(2/pi)  to convert a "litterature" gaussian
 * parameter into a standard deviation
 */
constexpr inline double mulBySqrtTwoOverPi(double x) { return x*sqrt(2./M_PI); }

/** generate default gate bootstrapping parameters */
EXPORT TFheGateBootstrappingParameterSet* new_default_gate_bootstrapping_parameters(int minimum_lambda) {
    if (minimum_lambda>128) 
	die_dramatically("Sorry, for now, the parameters are only implemented for about 128bit of security!");
    
    static const int N = 1024;
    static const int k = 1;
    static const int n = 500;
    static const int bk_l = 3;
    static const int bk_Bgbit = 10;
    static const int ks_basebit = 1;
    static const int ks_length = 15;
    static const double ks_stdev = mulBySqrtTwoOverPi(pow(2.,-15));   //standard deviation
    static const double bk_stdev = mulBySqrtTwoOverPi(9.e-9);          //standard deviation
    static const double max_stdev = mulBySqrtTwoOverPi(pow(2.,-4)/4.); //max standard deviation for a 1/4 msg space
    
    LweParams* params_in = new_LweParams(n, ks_stdev, max_stdev);
    TLweParams* params_accum = new_TLweParams(N, k, bk_stdev, max_stdev);
    TGswParams* params_bk = new_TGswParams(bk_l, bk_Bgbit, params_accum);

    TfheGarbageCollector::register_param(params_in);
    TfheGarbageCollector::register_param(params_accum);
    TfheGarbageCollector::register_param(params_bk);

    return new TFheGateBootstrappingParameterSet(ks_length, ks_basebit, params_in, params_bk);
}

/** deletes gate bootstrapping parameters */
EXPORT void delete_gate_bootstrapping_parameters(TFheGateBootstrappingParameterSet* params) {
    delete params;
}

/** generate a gate bootstrapping secret key */
EXPORT TFheGateBootstrappingSecretKeySet* new_random_gate_bootstrapping_secret_keyset(const TFheGateBootstrappingParameterSet* params) {
    LweKey* lwe_key = new_LweKey(params->in_out_params);
    lweKeyGen(lwe_key);
    TGswKey* tgsw_key = new_TGswKey(params->tgsw_params);
    tGswKeyGen(tgsw_key);
    LweBootstrappingKey* bk = new_LweBootstrappingKey(params->ks_t, params->ks_basebit, params->in_out_params, params->tgsw_params);
    tfhe_createLweBootstrappingKey(bk, lwe_key, tgsw_key);
    LweBootstrappingKeyFFT* bkFFT = new_LweBootstrappingKeyFFT(bk);
    return new TFheGateBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key);
}

/** deletes a gate bootstrapping secret key */
EXPORT void delete_gate_bootstrapping_secret_keyset(TFheGateBootstrappingSecretKeySet* keyset) {
    LweKey* lwe_key = (LweKey*) keyset->lwe_key;
    TGswKey* tgsw_key = (TGswKey*) keyset->tgsw_key;
    LweBootstrappingKey* bk = (LweBootstrappingKey*) keyset->cloud.bk;
    LweBootstrappingKeyFFT* bkFFT = (LweBootstrappingKeyFFT*) keyset->cloud.bkFFT;
    if (bkFFT) delete_LweBootstrappingKeyFFT(bkFFT);    
    if (bk) delete_LweBootstrappingKey(bk);    
    delete_TGswKey(tgsw_key);
    delete_LweKey(lwe_key); 
    delete keyset;
}

/** deletes a gate bootstrapping cloud key */
EXPORT void delete_gate_bootstrapping_cloud_keyset(TFheGateBootstrappingCloudKeySet* keyset) {
    LweBootstrappingKey* bk = (LweBootstrappingKey*) keyset->bk;
    LweBootstrappingKeyFFT* bkFFT = (LweBootstrappingKeyFFT*) keyset->bkFFT;
    if (bkFFT) delete_LweBootstrappingKeyFFT(bkFFT);    
    if (bk) delete_LweBootstrappingKey(bk);    
    delete keyset;
}

/** generate a new unititialized ciphertext */
EXPORT LweSample* new_gate_bootstrapping_ciphertext(const TFheGateBootstrappingParameterSet* params) {
    return new_LweSample(params->in_out_params);
}

/** generate a new unititialized ciphertext array of length nbelems */
EXPORT LweSample* new_gate_bootstrapping_ciphertext_array(int nbelems, const TFheGateBootstrappingParameterSet* params) {
    return new_LweSample_array(nbelems, params->in_out_params);
}

/** deletes a ciphertext */
EXPORT void delete_gate_bootstrapping_ciphertext(LweSample* sample) {
    delete_LweSample(sample);
}

/** deletes a ciphertext array of length nbelems */
EXPORT void delete_gate_bootstrapping_ciphertext_array(int nbelems, LweSample* samples) {
    delete_LweSample_array(nbelems, samples);
}

/** encrypts a boolean */
EXPORT void bootsSymEncrypt(LweSample* result, int message, const TFheGateBootstrappingSecretKeySet* key) {
    Torus32 _1s8 = modSwitchToTorus32(1,8);
    Torus32 mu = message?_1s8:-_1s8;
    double alpha = key->params->in_out_params->alpha_min; //TODO: specify noise
    lweSymEncrypt(result, mu, alpha, key->lwe_key);
}

/** decrypts a boolean */
EXPORT int bootsSymDecrypt(const LweSample* sample, const TFheGateBootstrappingSecretKeySet* key) {
    Torus32 mu = lwePhase(sample, key->lwe_key);
    return (mu>0?1:0); //we have to do that because of the C binding
}
