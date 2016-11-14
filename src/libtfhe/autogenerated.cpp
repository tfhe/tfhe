//THIS  FILE IS AUTOMATICALLY GENERATED
//DO NOT EDIT BY HANDS
#include "tfhe_core.h"
#include <cstdlib>
#include <new>
using namespace std;
#include "polynomials.h" 
//allocate memory space for a IntPolynomial

EXPORT IntPolynomial* alloc_IntPolynomial() {
    return (IntPolynomial*) malloc(sizeof(IntPolynomial));
}
EXPORT IntPolynomial* alloc_IntPolynomial_array(int nbelts) {
    return (IntPolynomial*) malloc(nbelts*sizeof(IntPolynomial));
}

//free memory space for a LweKey
EXPORT void free_IntPolynomial(IntPolynomial* ptr) {
    free(ptr);
}
EXPORT void free_IntPolynomial_array(int nbelts, IntPolynomial* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_IntPolynomial(IntPolynomial* obj, const int N) {
    new(obj) IntPolynomial(N);
}
EXPORT void init_IntPolynomial_array(int nbelts, IntPolynomial* obj, const int N) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) IntPolynomial(N);
    }
}

//destroys the IntPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_IntPolynomial(IntPolynomial* obj) {
    obj->~IntPolynomial();
}
EXPORT void destroy_IntPolynomial_array(int nbelts, IntPolynomial* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~IntPolynomial();
    }
}
 
//allocates and initialize the IntPolynomial structure
//(equivalent of the C++ new)
EXPORT IntPolynomial* new_IntPolynomial(const int N) {
    return new IntPolynomial(N);
}
EXPORT IntPolynomial* new_IntPolynomial_array(int nbelts, const int N) {
    IntPolynomial* obj = alloc_IntPolynomial_array(nbelts);
    init_IntPolynomial_array(nbelts,obj,N);
    return obj;
}

//destroys and frees the IntPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_IntPolynomial(IntPolynomial* obj) {
    delete obj;
}
EXPORT void delete_IntPolynomial_array(int nbelts, IntPolynomial* obj) {
    destroy_IntPolynomial_array(nbelts,obj);
    free_IntPolynomial_array(nbelts,obj);
}
#include "lwebootstrappingkey.h" 
//allocate memory space for a LweBootstrappingKey

EXPORT LweBootstrappingKey* alloc_LweBootstrappingKey() {
    return (LweBootstrappingKey*) malloc(sizeof(LweBootstrappingKey));
}
EXPORT LweBootstrappingKey* alloc_LweBootstrappingKey_array(int nbelts) {
    return (LweBootstrappingKey*) malloc(nbelts*sizeof(LweBootstrappingKey));
}

//free memory space for a LweKey
EXPORT void free_LweBootstrappingKey(LweBootstrappingKey* ptr) {
    free(ptr);
}
EXPORT void free_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweBootstrappingKey(LweBootstrappingKey* obj, const LweParams* in_out_params, const TGswParams* bk_params) {
    new(obj) LweBootstrappingKey(in_out_params,bk_params);
}
EXPORT void init_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* obj, const LweParams* in_out_params, const TGswParams* bk_params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LweBootstrappingKey(in_out_params,bk_params);
    }
}

//destroys the LweBootstrappingKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweBootstrappingKey(LweBootstrappingKey* obj) {
    obj->~LweBootstrappingKey();
}
EXPORT void destroy_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LweBootstrappingKey();
    }
}
 
//allocates and initialize the LweBootstrappingKey structure
//(equivalent of the C++ new)
EXPORT LweBootstrappingKey* new_LweBootstrappingKey(const LweParams* in_out_params, const TGswParams* bk_params) {
    return new LweBootstrappingKey(in_out_params,bk_params);
}
EXPORT LweBootstrappingKey* new_LweBootstrappingKey_array(int nbelts, const LweParams* in_out_params, const TGswParams* bk_params) {
    LweBootstrappingKey* obj = alloc_LweBootstrappingKey_array(nbelts);
    init_LweBootstrappingKey_array(nbelts,obj,in_out_params,bk_params);
    return obj;
}

//destroys and frees the LweBootstrappingKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweBootstrappingKey(LweBootstrappingKey* obj) {
    delete obj;
}
EXPORT void delete_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* obj) {
    destroy_LweBootstrappingKey_array(nbelts,obj);
    free_LweBootstrappingKey_array(nbelts,obj);
}
#include "lwebootstrappingkey.h" 
//allocate memory space for a LweBootstrappingKeyFFT

EXPORT LweBootstrappingKeyFFT* alloc_LweBootstrappingKeyFFT() {
    return (LweBootstrappingKeyFFT*) malloc(sizeof(LweBootstrappingKeyFFT));
}
EXPORT LweBootstrappingKeyFFT* alloc_LweBootstrappingKeyFFT_array(int nbelts) {
    return (LweBootstrappingKeyFFT*) malloc(nbelts*sizeof(LweBootstrappingKeyFFT));
}

//free memory space for a LweKey
EXPORT void free_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* ptr) {
    free(ptr);
}
EXPORT void free_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* obj, const LweParams* in_out_params, const TGswParams* bk_params) {
    new(obj) LweBootstrappingKeyFFT(in_out_params,bk_params);
}
EXPORT void init_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* obj, const LweParams* in_out_params, const TGswParams* bk_params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LweBootstrappingKeyFFT(in_out_params,bk_params);
    }
}

//destroys the LweBootstrappingKeyFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* obj) {
    obj->~LweBootstrappingKeyFFT();
}
EXPORT void destroy_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LweBootstrappingKeyFFT();
    }
}
 
//allocates and initialize the LweBootstrappingKeyFFT structure
//(equivalent of the C++ new)
EXPORT LweBootstrappingKeyFFT* new_LweBootstrappingKeyFFT(const LweParams* in_out_params, const TGswParams* bk_params) {
    return new LweBootstrappingKeyFFT(in_out_params,bk_params);
}
EXPORT LweBootstrappingKeyFFT* new_LweBootstrappingKeyFFT_array(int nbelts, const LweParams* in_out_params, const TGswParams* bk_params) {
    LweBootstrappingKeyFFT* obj = alloc_LweBootstrappingKeyFFT_array(nbelts);
    init_LweBootstrappingKeyFFT_array(nbelts,obj,in_out_params,bk_params);
    return obj;
}

//destroys and frees the LweBootstrappingKeyFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* obj) {
    delete obj;
}
EXPORT void delete_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* obj) {
    destroy_LweBootstrappingKeyFFT_array(nbelts,obj);
    free_LweBootstrappingKeyFFT_array(nbelts,obj);
}
#include "lwekey.h" 
//allocate memory space for a LweKey

EXPORT LweKey* alloc_LweKey() {
    return (LweKey*) malloc(sizeof(LweKey));
}
EXPORT LweKey* alloc_LweKey_array(int nbelts) {
    return (LweKey*) malloc(nbelts*sizeof(LweKey));
}

//free memory space for a LweKey
EXPORT void free_LweKey(LweKey* ptr) {
    free(ptr);
}
EXPORT void free_LweKey_array(int nbelts, LweKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweKey(LweKey* obj, const LweParams* params) {
    new(obj) LweKey(params);
}
EXPORT void init_LweKey_array(int nbelts, LweKey* obj, const LweParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LweKey(params);
    }
}

//destroys the LweKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweKey(LweKey* obj) {
    obj->~LweKey();
}
EXPORT void destroy_LweKey_array(int nbelts, LweKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LweKey();
    }
}
 
//allocates and initialize the LweKey structure
//(equivalent of the C++ new)
EXPORT LweKey* new_LweKey(const LweParams* params) {
    return new LweKey(params);
}
EXPORT LweKey* new_LweKey_array(int nbelts, const LweParams* params) {
    LweKey* obj = alloc_LweKey_array(nbelts);
    init_LweKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the LweKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweKey(LweKey* obj) {
    delete obj;
}
EXPORT void delete_LweKey_array(int nbelts, LweKey* obj) {
    destroy_LweKey_array(nbelts,obj);
    free_LweKey_array(nbelts,obj);
}
#include "lwekeyswitch.h" 
//allocate memory space for a LweKeySwitchKey

EXPORT LweKeySwitchKey* alloc_LweKeySwitchKey() {
    return (LweKeySwitchKey*) malloc(sizeof(LweKeySwitchKey));
}
EXPORT LweKeySwitchKey* alloc_LweKeySwitchKey_array(int nbelts) {
    return (LweKeySwitchKey*) malloc(nbelts*sizeof(LweKeySwitchKey));
}

//free memory space for a LweKey
EXPORT void free_LweKeySwitchKey(LweKeySwitchKey* ptr) {
    free(ptr);
}
EXPORT void free_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweKeySwitchKey(LweKeySwitchKey* obj, int n, int t, int basebit, const LweParams* out_params) {
    new(obj) LweKeySwitchKey(n,t,basebit,out_params);
}
EXPORT void init_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* obj, int n, int t, int basebit, const LweParams* out_params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LweKeySwitchKey(n,t,basebit,out_params);
    }
}

//destroys the LweKeySwitchKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweKeySwitchKey(LweKeySwitchKey* obj) {
    obj->~LweKeySwitchKey();
}
EXPORT void destroy_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LweKeySwitchKey();
    }
}
 
//allocates and initialize the LweKeySwitchKey structure
//(equivalent of the C++ new)
EXPORT LweKeySwitchKey* new_LweKeySwitchKey(int n, int t, int basebit, const LweParams* out_params) {
    return new LweKeySwitchKey(n,t,basebit,out_params);
}
EXPORT LweKeySwitchKey* new_LweKeySwitchKey_array(int nbelts, int n, int t, int basebit, const LweParams* out_params) {
    LweKeySwitchKey* obj = alloc_LweKeySwitchKey_array(nbelts);
    init_LweKeySwitchKey_array(nbelts, obj, n,t,basebit,out_params);
    return obj;
}

//destroys and frees the LweKeySwitchKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweKeySwitchKey(LweKeySwitchKey* obj) {
    delete obj;
}
EXPORT void delete_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* obj) {
    destroy_LweKeySwitchKey_array(nbelts,obj);
    free_LweKeySwitchKey_array(nbelts,obj);
}
#include "lweparams.h" 
//allocate memory space for a LweParams

EXPORT LweParams* alloc_LweParams() {
    return (LweParams*) malloc(sizeof(LweParams));
}
EXPORT LweParams* alloc_LweParams_array(int nbelts) {
    return (LweParams*) malloc(nbelts*sizeof(LweParams));
}

//free memory space for a LweKey
EXPORT void free_LweParams(LweParams* ptr) {
    free(ptr);
}
EXPORT void free_LweParams_array(int nbelts, LweParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweParams(LweParams* obj, int n, double alpha_min, double alpha_max) {
    new(obj) LweParams(n,alpha_min,alpha_max);
}
EXPORT void init_LweParams_array(int nbelts, LweParams* obj, int n, double alpha_min, double alpha_max) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LweParams(n,alpha_min,alpha_max);
    }
}

//destroys the LweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweParams(LweParams* obj) {
    obj->~LweParams();
}
EXPORT void destroy_LweParams_array(int nbelts, LweParams* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LweParams();
    }
}
 
//allocates and initialize the LweParams structure
//(equivalent of the C++ new)
EXPORT LweParams* new_LweParams(int n, double alpha_min, double alpha_max) {
    return new LweParams(n,alpha_min,alpha_max);
}
EXPORT LweParams* new_LweParams_array(int nbelts, int n, double alpha_min, double alpha_max) {
    LweParams* obj = alloc_LweParams_array(nbelts);
    init_LweParams_array(nbelts,obj,n,alpha_min,alpha_max);
    return obj;
}

//destroys and frees the LweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_LweParams(LweParams* obj) {
    delete obj;
}
EXPORT void delete_LweParams_array(int nbelts, LweParams* obj) {
    destroy_LweParams_array(nbelts,obj);
    free_LweParams_array(nbelts,obj);
}
#include "lwesamples.h" 
//allocate memory space for a LweSample

EXPORT LweSample* alloc_LweSample() {
    return (LweSample*) malloc(sizeof(LweSample));
}
EXPORT LweSample* alloc_LweSample_array(int nbelts) {
    return (LweSample*) malloc(nbelts*sizeof(LweSample));
}

//free memory space for a LweKey
EXPORT void free_LweSample(LweSample* ptr) {
    free(ptr);
}
EXPORT void free_LweSample_array(int nbelts, LweSample* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweSample(LweSample* obj, const LweParams* params) {
    new(obj) LweSample(params);
}
EXPORT void init_LweSample_array(int nbelts, LweSample* obj, const LweParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LweSample(params);
    }
}

//destroys the LweSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweSample(LweSample* obj) {
    obj->~LweSample();
}
EXPORT void destroy_LweSample_array(int nbelts, LweSample* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LweSample();
    }
}
 
//allocates and initialize the LweSample structure
//(equivalent of the C++ new)
EXPORT LweSample* new_LweSample(const LweParams* params) {
    return new LweSample(params);
}
EXPORT LweSample* new_LweSample_array(int nbelts, const LweParams* params) {
    LweSample* obj = alloc_LweSample_array(nbelts);
    init_LweSample_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the LweSample structure
//(equivalent of the C++ delete)
EXPORT void delete_LweSample(LweSample* obj) {
    delete obj;
}
EXPORT void delete_LweSample_array(int nbelts, LweSample* obj) {
    destroy_LweSample_array(nbelts,obj);
    free_LweSample_array(nbelts,obj);
}

#include "tgsw.h" 
//allocate memory space for a TGswKey

EXPORT TGswKey* alloc_TGswKey() {
    return (TGswKey*) malloc(sizeof(TGswKey));
}
EXPORT TGswKey* alloc_TGswKey_array(int nbelts) {
    return (TGswKey*) malloc(nbelts*sizeof(TGswKey));
}

//free memory space for a LweKey
EXPORT void free_TGswKey(TGswKey* ptr) {
    free(ptr);
}
EXPORT void free_TGswKey_array(int nbelts, TGswKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswKey(TGswKey* obj, const TGswParams* params) {
    new(obj) TGswKey(params);
}
EXPORT void init_TGswKey_array(int nbelts, TGswKey* obj, const TGswParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TGswKey(params);
    }
}

//destroys the TGswKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswKey(TGswKey* obj) {
    obj->~TGswKey();
}
EXPORT void destroy_TGswKey_array(int nbelts, TGswKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TGswKey();
    }
}
 
//allocates and initialize the TGswKey structure
//(equivalent of the C++ new)
EXPORT TGswKey* new_TGswKey(const TGswParams* params) {
    return new TGswKey(params);
}
EXPORT TGswKey* new_TGswKey_array(int nbelts, const TGswParams* params) {
    TGswKey* obj = alloc_TGswKey_array(nbelts);
    init_TGswKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TGswKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswKey(TGswKey* obj) {
    delete obj;
}
EXPORT void delete_TGswKey_array(int nbelts, TGswKey* obj) {
    destroy_TGswKey_array(nbelts,obj);
    free_TGswKey_array(nbelts,obj);
}
#include "tgsw.h" 
//allocate memory space for a TGswParams

EXPORT TGswParams* alloc_TGswParams() {
    return (TGswParams*) malloc(sizeof(TGswParams));
}
EXPORT TGswParams* alloc_TGswParams_array(int nbelts) {
    return (TGswParams*) malloc(nbelts*sizeof(TGswParams));
}

//free memory space for a LweKey
EXPORT void free_TGswParams(TGswParams* ptr) {
    free(ptr);
}
EXPORT void free_TGswParams_array(int nbelts, TGswParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswParams(TGswParams* obj, int l, int Bgbit, const TLweParams* tlwe_params) {
    new(obj) TGswParams(l,Bgbit,tlwe_params);
}
EXPORT void init_TGswParams_array(int nbelts, TGswParams* obj, int l, int Bgbit, const TLweParams* tlwe_params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TGswParams(l,Bgbit,tlwe_params);
    }
}

//destroys the TGswParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswParams(TGswParams* obj) {
    obj->~TGswParams();
}
EXPORT void destroy_TGswParams_array(int nbelts, TGswParams* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TGswParams();
    }
}
 
//allocates and initialize the TGswParams structure
//(equivalent of the C++ new)
EXPORT TGswParams* new_TGswParams(int l, int Bgbit, const TLweParams* tlwe_params) {
    return new TGswParams(l,Bgbit,tlwe_params);
}
EXPORT TGswParams* new_TGswParams_array(int nbelts, int l, int Bgbit, const TLweParams* tlwe_params) {
    TGswParams* obj = alloc_TGswParams_array(nbelts);
    init_TGswParams_array(nbelts,obj,l,Bgbit,tlwe_params);
    return obj;
}

//destroys and frees the TGswParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswParams(TGswParams* obj) {
    delete obj;
}
EXPORT void delete_TGswParams_array(int nbelts, TGswParams* obj) {
    destroy_TGswParams_array(nbelts,obj);
    free_TGswParams_array(nbelts,obj);
}
#include "tgsw.h" 
//allocate memory space for a TGswSample

EXPORT TGswSample* alloc_TGswSample() {
    return (TGswSample*) malloc(sizeof(TGswSample));
}
EXPORT TGswSample* alloc_TGswSample_array(int nbelts) {
    return (TGswSample*) malloc(nbelts*sizeof(TGswSample));
}

//free memory space for a LweKey
EXPORT void free_TGswSample(TGswSample* ptr) {
    free(ptr);
}
EXPORT void free_TGswSample_array(int nbelts, TGswSample* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSample(TGswSample* obj, const TGswParams* params) {
    new(obj) TGswSample(params);
}
EXPORT void init_TGswSample_array(int nbelts, TGswSample* obj, const TGswParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TGswSample(params);
    }
}

//destroys the TGswSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSample(TGswSample* obj) {
    obj->~TGswSample();
}
EXPORT void destroy_TGswSample_array(int nbelts, TGswSample* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TGswSample();
    }
}
 
//allocates and initialize the TGswSample structure
//(equivalent of the C++ new)
EXPORT TGswSample* new_TGswSample(const TGswParams* params) {
    return new TGswSample(params);
}
EXPORT TGswSample* new_TGswSample_array(int nbelts, const TGswParams* params) {
    TGswSample* obj = alloc_TGswSample_array(nbelts);
    init_TGswSample_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TGswSample structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswSample(TGswSample* obj) {
    delete obj;
}
EXPORT void delete_TGswSample_array(int nbelts, TGswSample* obj) {
    destroy_TGswSample_array(nbelts,obj);
    free_TGswSample_array(nbelts,obj);
}
#include "tgsw.h" 
//allocate memory space for a TGswSampleFFT

EXPORT TGswSampleFFT* alloc_TGswSampleFFT() {
    return (TGswSampleFFT*) malloc(sizeof(TGswSampleFFT));
}
EXPORT TGswSampleFFT* alloc_TGswSampleFFT_array(int nbelts) {
    return (TGswSampleFFT*) malloc(nbelts*sizeof(TGswSampleFFT));
}

//free memory space for a LweKey
EXPORT void free_TGswSampleFFT(TGswSampleFFT* ptr) {
    free(ptr);
}
EXPORT void free_TGswSampleFFT_array(int nbelts, TGswSampleFFT* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSampleFFT(TGswSampleFFT* obj, const TGswParams* params) {
    new(obj) TGswSampleFFT(params);
}
EXPORT void init_TGswSampleFFT_array(int nbelts, TGswSampleFFT* obj, const TGswParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TGswSampleFFT(params);
    }
}

//destroys the TGswSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSampleFFT(TGswSampleFFT* obj) {
    obj->~TGswSampleFFT();
}
EXPORT void destroy_TGswSampleFFT_array(int nbelts, TGswSampleFFT* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TGswSampleFFT();
    }
}
 
//allocates and initialize the TGswSampleFFT structure
//(equivalent of the C++ new)
EXPORT TGswSampleFFT* new_TGswSampleFFT(const TGswParams* params) {
    return new TGswSampleFFT(params);
}
EXPORT TGswSampleFFT* new_TGswSampleFFT_array(int nbelts, const TGswParams* params) {
    TGswSampleFFT* obj = alloc_TGswSampleFFT_array(nbelts);
    init_TGswSampleFFT_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TGswSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswSampleFFT(TGswSampleFFT* obj) {
    delete obj;
}
EXPORT void delete_TGswSampleFFT_array(int nbelts, TGswSampleFFT* obj) {
    destroy_TGswSampleFFT_array(nbelts,obj);
    free_TGswSampleFFT_array(nbelts,obj);
}
#include "tlwe.h" 
//allocate memory space for a TLweKey

EXPORT TLweKey* alloc_TLweKey() {
    return (TLweKey*) malloc(sizeof(TLweKey));
}
EXPORT TLweKey* alloc_TLweKey_array(int nbelts) {
    return (TLweKey*) malloc(nbelts*sizeof(TLweKey));
}

//free memory space for a LweKey
EXPORT void free_TLweKey(TLweKey* ptr) {
    free(ptr);
}
EXPORT void free_TLweKey_array(int nbelts, TLweKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweKey(TLweKey* obj, const TLweParams* params) {
    new(obj) TLweKey(params);
}
EXPORT void init_TLweKey_array(int nbelts, TLweKey* obj, const TLweParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TLweKey(params);
    }
}

//destroys the TLweKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweKey(TLweKey* obj) {
    obj->~TLweKey();
}
EXPORT void destroy_TLweKey_array(int nbelts, TLweKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TLweKey();
    }
}
 
//allocates and initialize the TLweKey structure
//(equivalent of the C++ new)
EXPORT TLweKey* new_TLweKey(const TLweParams* params) {
    return new TLweKey(params);
}
EXPORT TLweKey* new_TLweKey_array(int nbelts, const TLweParams* params) {
    TLweKey* obj = alloc_TLweKey_array(nbelts);
    init_TLweKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TLweKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweKey(TLweKey* obj) {
    delete obj;
}
EXPORT void delete_TLweKey_array(int nbelts, TLweKey* obj) {
    destroy_TLweKey_array(nbelts,obj);
    free_TLweKey_array(nbelts,obj);
}
#include "tlwe.h" 
//allocate memory space for a TLweParams

EXPORT TLweParams* alloc_TLweParams() {
    return (TLweParams*) malloc(sizeof(TLweParams));
}
EXPORT TLweParams* alloc_TLweParams_array(int nbelts) {
    return (TLweParams*) malloc(nbelts*sizeof(TLweParams));
}

//free memory space for a LweKey
EXPORT void free_TLweParams(TLweParams* ptr) {
    free(ptr);
}
EXPORT void free_TLweParams_array(int nbelts, TLweParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweParams(TLweParams* obj, int N, int k, double alpha_min, double alpha_max) {
    new(obj) TLweParams(N,k,alpha_min,alpha_max);
}
EXPORT void init_TLweParams_array(int nbelts, TLweParams* obj, int N, int k, double alpha_min, double alpha_max) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TLweParams(N,k,alpha_min,alpha_max);
    }
}

//destroys the TLweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweParams(TLweParams* obj) {
    obj->~TLweParams();
}
EXPORT void destroy_TLweParams_array(int nbelts, TLweParams* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TLweParams();
    }
}
 
//allocates and initialize the TLweParams structure
//(equivalent of the C++ new)
EXPORT TLweParams* new_TLweParams(int N, int k, double alpha_min, double alpha_max) {
    return new TLweParams(N,k,alpha_min,alpha_max);
}
EXPORT TLweParams* new_TLweParams_array(int nbelts, int N, int k, double alpha_min, double alpha_max) {
    TLweParams* obj = alloc_TLweParams_array(nbelts);
    init_TLweParams_array(nbelts,obj,N,k,alpha_min,alpha_max);
    return obj;
}

//destroys and frees the TLweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweParams(TLweParams* obj) {
    delete obj;
}
EXPORT void delete_TLweParams_array(int nbelts, TLweParams* obj) {
    destroy_TLweParams_array(nbelts,obj);
    free_TLweParams_array(nbelts,obj);
}
#include "tlwe.h" 
//allocate memory space for a TLweSample

EXPORT TLweSample* alloc_TLweSample() {
    return (TLweSample*) malloc(sizeof(TLweSample));
}
EXPORT TLweSample* alloc_TLweSample_array(int nbelts) {
    return (TLweSample*) malloc(nbelts*sizeof(TLweSample));
}

//free memory space for a LweKey
EXPORT void free_TLweSample(TLweSample* ptr) {
    free(ptr);
}
EXPORT void free_TLweSample_array(int nbelts, TLweSample* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweSample(TLweSample* obj, const TLweParams* params) {
    new(obj) TLweSample(params);
}
EXPORT void init_TLweSample_array(int nbelts, TLweSample* obj, const TLweParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TLweSample(params);
    }
}

//destroys the TLweSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweSample(TLweSample* obj) {
    obj->~TLweSample();
}
EXPORT void destroy_TLweSample_array(int nbelts, TLweSample* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TLweSample();
    }
}
 
//allocates and initialize the TLweSample structure
//(equivalent of the C++ new)
EXPORT TLweSample* new_TLweSample(const TLweParams* params) {
    return new TLweSample(params);
}
EXPORT TLweSample* new_TLweSample_array(int nbelts, const TLweParams* params) {
    TLweSample* obj = alloc_TLweSample_array(nbelts);
    init_TLweSample_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TLweSample structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweSample(TLweSample* obj) {
    delete obj;
}
EXPORT void delete_TLweSample_array(int nbelts, TLweSample* obj) {
    destroy_TLweSample_array(nbelts,obj);
    free_TLweSample_array(nbelts,obj);
}
#include "tlwe.h" 
//allocate memory space for a TLweSampleFFT

EXPORT TLweSampleFFT* alloc_TLweSampleFFT() {
    return (TLweSampleFFT*) malloc(sizeof(TLweSampleFFT));
}
EXPORT TLweSampleFFT* alloc_TLweSampleFFT_array(int nbelts) {
    return (TLweSampleFFT*) malloc(nbelts*sizeof(TLweSampleFFT));
}

//free memory space for a LweKey
EXPORT void free_TLweSampleFFT(TLweSampleFFT* ptr) {
    free(ptr);
}
EXPORT void free_TLweSampleFFT_array(int nbelts, TLweSampleFFT* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweSampleFFT(TLweSampleFFT* obj, const TLweParams* params) {
    new(obj) TLweSampleFFT(params);
}
EXPORT void init_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj, const TLweParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TLweSampleFFT(params);
    }
}

//destroys the TLweSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweSampleFFT(TLweSampleFFT* obj) {
    obj->~TLweSampleFFT();
}
EXPORT void destroy_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TLweSampleFFT();
    }
}
 
//allocates and initialize the TLweSampleFFT structure
//(equivalent of the C++ new)
EXPORT TLweSampleFFT* new_TLweSampleFFT(const TLweParams* params) {
    return new TLweSampleFFT(params);
}
EXPORT TLweSampleFFT* new_TLweSampleFFT_array(int nbelts, const TLweParams* params) {
    TLweSampleFFT* obj = alloc_TLweSampleFFT_array(nbelts);
    init_TLweSampleFFT_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TLweSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweSampleFFT(TLweSampleFFT* obj) {
    delete obj;
}
EXPORT void delete_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj) {
    destroy_TLweSampleFFT_array(nbelts,obj);
    free_TLweSampleFFT_array(nbelts,obj);
}
#include "polynomials.h" 
//allocate memory space for a TorusPolynomial

EXPORT TorusPolynomial* alloc_TorusPolynomial() {
    return (TorusPolynomial*) malloc(sizeof(TorusPolynomial));
}
EXPORT TorusPolynomial* alloc_TorusPolynomial_array(int nbelts) {
    return (TorusPolynomial*) malloc(nbelts*sizeof(TorusPolynomial));
}

//free memory space for a LweKey
EXPORT void free_TorusPolynomial(TorusPolynomial* ptr) {
    free(ptr);
}
EXPORT void free_TorusPolynomial_array(int nbelts, TorusPolynomial* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TorusPolynomial(TorusPolynomial* obj, const int N) {
    new(obj) TorusPolynomial(N);
}
EXPORT void init_TorusPolynomial_array(int nbelts, TorusPolynomial* obj, const int N) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) TorusPolynomial(N);
    }
}

//destroys the TorusPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TorusPolynomial(TorusPolynomial* obj) {
    obj->~TorusPolynomial();
}
EXPORT void destroy_TorusPolynomial_array(int nbelts, TorusPolynomial* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~TorusPolynomial();
    }
}
 
//allocates and initialize the TorusPolynomial structure
//(equivalent of the C++ new)
EXPORT TorusPolynomial* new_TorusPolynomial(const int N) {
    return new TorusPolynomial(N);
}
EXPORT TorusPolynomial* new_TorusPolynomial_array(int nbelts, const int N) {
    TorusPolynomial* obj = alloc_TorusPolynomial_array(nbelts);
    init_TorusPolynomial_array(nbelts,obj,N);
    return obj;
}

//destroys and frees the TorusPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_TorusPolynomial(TorusPolynomial* obj) {
    delete obj;
}
EXPORT void delete_TorusPolynomial_array(int nbelts, TorusPolynomial* obj) {
    destroy_TorusPolynomial_array(nbelts,obj);
    free_TorusPolynomial_array(nbelts,obj);
}
