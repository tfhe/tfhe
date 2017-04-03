#ifndef TFHE_H
#define TFHE_H

///@file
///@brief This file declares almost everything

#include "tfhe_core.h"

#include "numeric_functions.h"

#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"

#include "lwe-functions.h"

#include "tlwe_functions.h"

#include "tgsw_functions.h"

#include "lwekeyswitch.h"

#include "lwebootstrappingkey.h"

//////////////////////////////////////////
// Gate bootstrapping public interface
//////////////////////////////////////////

typedef struct TFheGateBootstrappingParameterSet {
    const int ks_t;
    const int ks_basebit;
    const LweParams* const in_out_params;
    const TGswParams* const tgsw_params;
#ifdef __cplusplus
    TFheGateBootstrappingParameterSet(const int ks_t, const int ks_basebit, const LweParams* const in_out_params, const TGswParams* const tgsw_params):
	ks_t(ks_t),
	ks_basebit(ks_basebit),
	in_out_params(in_out_params),
	tgsw_params(tgsw_params) {}
#endif
} TFheGateBootstrappingParameterSet;

typedef struct TFheGateBootstrappingCloudKeySet {
    const TFheGateBootstrappingParameterSet* const params;
    const LweBootstrappingKey* const bk;
    const LweBootstrappingKeyFFT* const bkFFT;
#ifdef __cplusplus
    TFheGateBootstrappingCloudKeySet(
	    const TFheGateBootstrappingParameterSet* const params, 
	    const LweBootstrappingKey* const bk,
	    const LweBootstrappingKeyFFT* const bkFFT):
	params(params),bk(bk),bkFFT(bkFFT) {}
#endif
} TFheGateBootstrappingCloudKeySet;

typedef struct TFheGateBootstrappingSecretKeySet {
    const TFheGateBootstrappingParameterSet* params;
    const LweKey* lwe_key;
    const TGswKey* tgsw_key;
    const TFheGateBootstrappingCloudKeySet cloud;
#ifdef __cplusplus
    TFheGateBootstrappingSecretKeySet(
	    const TFheGateBootstrappingParameterSet* const params, 
	    const LweBootstrappingKey* const bk,
	    const LweBootstrappingKeyFFT* const bkFFT,
	    const LweKey* lwe_key,
	    const TGswKey* tgsw_key):
	params(params),
	lwe_key(lwe_key),
	tgsw_key(tgsw_key),
	cloud(params,bk,bkFFT) {}
#endif
} TFheGateBootstrappingSecretKeySet;

/** generate default gate bootstrapping parameters */
EXPORT TFheGateBootstrappingParameterSet* new_default_gate_bootstrapping_parameters(int minimum_lambda);

/** generate a random gate bootstrapping secret key */
EXPORT TFheGateBootstrappingSecretKeySet* new_random_gate_bootstrapping_secret_keyset(const TFheGateBootstrappingParameterSet* params);

/** deletes gate bootstrapping parameters */
EXPORT void delete_gate_bootstrapping_parameters(TFheGateBootstrappingParameterSet* params);

/** deletes a gate bootstrapping secret key */
EXPORT void delete_gate_bootstrapping_secret_keyset(TFheGateBootstrappingSecretKeySet* keyset);

/** deletes a gate bootstrapping secret key */
EXPORT void delete_gate_bootstrapping_cloud_keyset(TFheGateBootstrappingCloudKeySet* keyset);

/** generate a new unititialized ciphertext */
EXPORT LweSample* new_gate_bootstrapping_ciphertext(const TFheGateBootstrappingParameterSet* params);

/** generate a new unititialized ciphertext array of length nbelems */
EXPORT LweSample* new_gate_bootstrapping_ciphertext_array(int nbelems, const TFheGateBootstrappingParameterSet* params);

/** deletes a ciphertext */
EXPORT void delete_gate_bootstrapping_ciphertext(LweSample* sample);

/** deletes a ciphertext array of length nbelems */
EXPORT void delete_gate_bootstrapping_ciphertext_array(int nbelems, LweSample* samples);

/** encrypts a boolean */
EXPORT void bootsSymEncrypt(LweSample* result, bool message, const TFheGateBootstrappingSecretKeySet* params);

/** decrypts a boolean */
EXPORT bool bootsSymDecrypt(const LweSample* sample, const TFheGateBootstrappingSecretKeySet* params);

/** bootstrapped Nand Gate */ 
EXPORT void bootsNAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Or Gate:  */ 
EXPORT void bootsOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped And Gate: result = a and b */ 
EXPORT void bootsAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Xor Gate: result = a xor b */ 
EXPORT void bootsXOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Xnor Gate: result = (a==b) */ 
EXPORT void bootsXNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Not Gate: result = not(a) */ 
EXPORT void bootsNOT(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Copy Gate: result = a */ 
EXPORT void bootsCOPY(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Nor Gate: result = not(a or b) */ 
EXPORT void bootsNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped AndNY Gate: not(a) and b */ 
EXPORT void bootsANDNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped AndYN Gate: a and not(b) */ 
EXPORT void bootsANDYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped OrNY Gate: not(a) or b */ 
EXPORT void bootsORNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped OrYN Gate: a or not(b) */ 
EXPORT void bootsORYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);

/** bootstrapped Mux(a,b,c) = a?b:c */ 
EXPORT void bootsMUX(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* c, const TFheGateBootstrappingCloudKeySet* bk);

///////////////////////////////////////////////////
//  TFHE bootstrapping internal functions
//////////////////////////////////////////////////


EXPORT void tfhe_blindRotate(TLweSample* accum, const TGswSample* bk, const int* bara, const int n, const TGswParams* bk_params);
EXPORT void tfhe_blindRotateAndExtract(LweSample* result, const TorusPolynomial* v, const TGswSample* bk, const int barb, const int* bara, const int n, const TGswParams* bk_params);
EXPORT void tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu, const LweSample* x);
EXPORT void tfhe_createLweBootstrappingKey(LweBootstrappingKey* bk, const LweKey* key_in, const TGswKey* rgsw_key);

EXPORT void tfhe_blindRotate_FFT(TLweSample* accum, const TGswSampleFFT* bk, const int* bara, const int n, const TGswParams* bk_params);
EXPORT void tfhe_blindRotateAndExtract_FFT(LweSample* result, const TorusPolynomial* v, const TGswSampleFFT* bk, const int barb, const int* bara, const int n, const TGswParams* bk_params);
EXPORT void tfhe_bootstrap_FFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu, const LweSample* x);


#endif //TFHE_H
