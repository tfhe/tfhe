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
EXPORT IntPolynomial* alloc_IntPolynomial_array(int32_t nbelts) {
    return (IntPolynomial*) malloc(nbelts*sizeof(IntPolynomial));
}

//free memory space for a LweKey
EXPORT void free_IntPolynomial(IntPolynomial* ptr) {
    free(ptr);
}
EXPORT void free_IntPolynomial_array(int32_t nbelts, IntPolynomial* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_IntPolynomial(IntPolynomial* obj, const int32_t N) {
    new(obj) IntPolynomial(N);
}
EXPORT void init_IntPolynomial_array(int32_t nbelts, IntPolynomial* obj, const int32_t N) {
    for (int32_t i=0; i<nbelts; i++) {
	new(obj+i) IntPolynomial(N);
    }
}

//destroys the IntPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_IntPolynomial(IntPolynomial* obj) {
    obj->~IntPolynomial();
}
EXPORT void destroy_IntPolynomial_array(int32_t nbelts, IntPolynomial* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	(obj+i)->~IntPolynomial();
    }
}
 
//allocates and initialize the IntPolynomial structure
//(equivalent of the C++ new)
EXPORT IntPolynomial* new_IntPolynomial(const int32_t N) {
    return new IntPolynomial(N);
}
EXPORT IntPolynomial* new_IntPolynomial_array(int32_t nbelts, const int32_t N) {
    IntPolynomial* obj = alloc_IntPolynomial_array(nbelts);
    init_IntPolynomial_array(nbelts,obj,N);
    return obj;
}

//destroys and frees the IntPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_IntPolynomial(IntPolynomial* obj) {
    delete obj;
}
EXPORT void delete_IntPolynomial_array(int32_t nbelts, IntPolynomial* obj) {
    destroy_IntPolynomial_array(nbelts,obj);
    free_IntPolynomial_array(nbelts,obj);
}

#include "lwebootstrappingkey.h" 





#include "lweparams.h" 
//allocate memory space for a LweParams

EXPORT LweParams* alloc_LweParams() {
    return (LweParams*) malloc(sizeof(LweParams));
}
EXPORT LweParams* alloc_LweParams_array(int32_t nbelts) {
    return (LweParams*) malloc(nbelts*sizeof(LweParams));
}

//free memory space for a LweKey
EXPORT void free_LweParams(LweParams* ptr) {
    free(ptr);
}
EXPORT void free_LweParams_array(int32_t nbelts, LweParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweParams(LweParams* obj, int32_t n, double alpha_min, double alpha_max) {
    new(obj) LweParams(n,alpha_min,alpha_max);
}
EXPORT void init_LweParams_array(int32_t nbelts, LweParams* obj, int32_t n, double alpha_min, double alpha_max) {
    for (int32_t i=0; i<nbelts; i++) {
	new(obj+i) LweParams(n,alpha_min,alpha_max);
    }
}

//destroys the LweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweParams(LweParams* obj) {
    obj->~LweParams();
}
EXPORT void destroy_LweParams_array(int32_t nbelts, LweParams* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	(obj+i)->~LweParams();
    }
}
 
//allocates and initialize the LweParams structure
//(equivalent of the C++ new)
EXPORT LweParams* new_LweParams(int32_t n, double alpha_min, double alpha_max) {
    return new LweParams(n,alpha_min,alpha_max);
}
EXPORT LweParams* new_LweParams_array(int32_t nbelts, int32_t n, double alpha_min, double alpha_max) {
    LweParams* obj = alloc_LweParams_array(nbelts);
    init_LweParams_array(nbelts,obj,n,alpha_min,alpha_max);
    return obj;
}

//destroys and frees the LweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_LweParams(LweParams* obj) {
    delete obj;
}
EXPORT void delete_LweParams_array(int32_t nbelts, LweParams* obj) {
    destroy_LweParams_array(nbelts,obj);
    free_LweParams_array(nbelts,obj);
}

#include "tgsw.h" 
//allocate memory space for a TGswKey

EXPORT TGswKey* alloc_TGswKey() {
    return (TGswKey*) malloc(sizeof(TGswKey));
}
EXPORT TGswKey* alloc_TGswKey_array(int32_t nbelts) {
    return (TGswKey*) malloc(nbelts*sizeof(TGswKey));
}

//free memory space for a LweKey
EXPORT void free_TGswKey(TGswKey* ptr) {
    free(ptr);
}
EXPORT void free_TGswKey_array(int32_t nbelts, TGswKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswKey(TGswKey* obj, const TGswParams* params) {
    new(obj) TGswKey(params);
}
EXPORT void init_TGswKey_array(int32_t nbelts, TGswKey* obj, const TGswParams* params) {
    for (int32_t i=0; i<nbelts; i++) {
	new(obj+i) TGswKey(params);
    }
}

//destroys the TGswKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswKey(TGswKey* obj) {
    obj->~TGswKey();
}
EXPORT void destroy_TGswKey_array(int32_t nbelts, TGswKey* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	(obj+i)->~TGswKey();
    }
}
 
//allocates and initialize the TGswKey structure
//(equivalent of the C++ new)
EXPORT TGswKey* new_TGswKey(const TGswParams* params) {
    return new TGswKey(params);
}
EXPORT TGswKey* new_TGswKey_array(int32_t nbelts, const TGswParams* params) {
    TGswKey* obj = alloc_TGswKey_array(nbelts);
    init_TGswKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the TGswKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswKey(TGswKey* obj) {
    delete obj;
}
EXPORT void delete_TGswKey_array(int32_t nbelts, TGswKey* obj) {
    destroy_TGswKey_array(nbelts,obj);
    free_TGswKey_array(nbelts,obj);
}
#include "tgsw.h" 
//allocate memory space for a TGswParams

EXPORT TGswParams* alloc_TGswParams() {
    return (TGswParams*) malloc(sizeof(TGswParams));
}
EXPORT TGswParams* alloc_TGswParams_array(int32_t nbelts) {
    return (TGswParams*) malloc(nbelts*sizeof(TGswParams));
}

//free memory space for a LweKey
EXPORT void free_TGswParams(TGswParams* ptr) {
    free(ptr);
}
EXPORT void free_TGswParams_array(int32_t nbelts, TGswParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswParams(TGswParams* obj, int32_t l, int32_t Bgbit, const TLweParams* tlwe_params) {
    new(obj) TGswParams(l,Bgbit,tlwe_params);
}
EXPORT void init_TGswParams_array(int32_t nbelts, TGswParams* obj, int32_t l, int32_t Bgbit, const TLweParams* tlwe_params) {
    for (int32_t i=0; i<nbelts; i++) {
	new(obj+i) TGswParams(l,Bgbit,tlwe_params);
    }
}

//destroys the TGswParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswParams(TGswParams* obj) {
    obj->~TGswParams();
}
EXPORT void destroy_TGswParams_array(int32_t nbelts, TGswParams* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	(obj+i)->~TGswParams();
    }
}
 
//allocates and initialize the TGswParams structure
//(equivalent of the C++ new)
EXPORT TGswParams* new_TGswParams(int32_t l, int32_t Bgbit, const TLweParams* tlwe_params) {
    return new TGswParams(l,Bgbit,tlwe_params);
}
EXPORT TGswParams* new_TGswParams_array(int32_t nbelts, int32_t l, int32_t Bgbit, const TLweParams* tlwe_params) {
    TGswParams* obj = alloc_TGswParams_array(nbelts);
    init_TGswParams_array(nbelts,obj,l,Bgbit,tlwe_params);
    return obj;
}

//destroys and frees the TGswParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswParams(TGswParams* obj) {
    delete obj;
}
EXPORT void delete_TGswParams_array(int32_t nbelts, TGswParams* obj) {
    destroy_TGswParams_array(nbelts,obj);
    free_TGswParams_array(nbelts,obj);
}
#include "tgsw.h" 
#include "tlwe.h" 
//allocate memory space for a TLweParams

EXPORT TLweParams* alloc_TLweParams() {
    return (TLweParams*) malloc(sizeof(TLweParams));
}
EXPORT TLweParams* alloc_TLweParams_array(int32_t nbelts) {
    return (TLweParams*) malloc(nbelts*sizeof(TLweParams));
}

//free memory space for a LweKey
EXPORT void free_TLweParams(TLweParams* ptr) {
    free(ptr);
}
EXPORT void free_TLweParams_array(int32_t nbelts, TLweParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweParams(TLweParams* obj, int32_t N, int32_t k, double alpha_min, double alpha_max) {
    new(obj) TLweParams(N,k,alpha_min,alpha_max);
}
EXPORT void init_TLweParams_array(int32_t nbelts, TLweParams* obj, int32_t N, int32_t k, double alpha_min, double alpha_max) {
    for (int32_t i=0; i<nbelts; i++) {
	new(obj+i) TLweParams(N,k,alpha_min,alpha_max);
    }
}

//destroys the TLweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweParams(TLweParams* obj) {
    obj->~TLweParams();
}
EXPORT void destroy_TLweParams_array(int32_t nbelts, TLweParams* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	(obj+i)->~TLweParams();
    }
}
 
//allocates and initialize the TLweParams structure
//(equivalent of the C++ new)
EXPORT TLweParams* new_TLweParams(int32_t N, int32_t k, double alpha_min, double alpha_max) {
    return new TLweParams(N,k,alpha_min,alpha_max);
}
EXPORT TLweParams* new_TLweParams_array(int32_t nbelts, int32_t N, int32_t k, double alpha_min, double alpha_max) {
    TLweParams* obj = alloc_TLweParams_array(nbelts);
    init_TLweParams_array(nbelts,obj,N,k,alpha_min,alpha_max);
    return obj;
}

//destroys and frees the TLweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweParams(TLweParams* obj) {
    delete obj;
}
EXPORT void delete_TLweParams_array(int32_t nbelts, TLweParams* obj) {
    destroy_TLweParams_array(nbelts,obj);
    free_TLweParams_array(nbelts,obj);
}
#include "polynomials.h" 
//allocate memory space for a TorusPolynomial

EXPORT TorusPolynomial* alloc_TorusPolynomial() {
    return (TorusPolynomial*) malloc(sizeof(TorusPolynomial));
}
EXPORT TorusPolynomial* alloc_TorusPolynomial_array(int32_t nbelts) {
    return (TorusPolynomial*) malloc(nbelts*sizeof(TorusPolynomial));
}

//free memory space for a LweKey
EXPORT void free_TorusPolynomial(TorusPolynomial* ptr) {
    free(ptr);
}
EXPORT void free_TorusPolynomial_array(int32_t nbelts, TorusPolynomial* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_TorusPolynomial(TorusPolynomial* obj, const int32_t N) {
    new(obj) TorusPolynomial(N);
}
EXPORT void init_TorusPolynomial_array(int32_t nbelts, TorusPolynomial* obj, const int32_t N) {
    for (int32_t i=0; i<nbelts; i++) {
	new(obj+i) TorusPolynomial(N);
    }
}

//destroys the TorusPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TorusPolynomial(TorusPolynomial* obj) {
    obj->~TorusPolynomial();
}
EXPORT void destroy_TorusPolynomial_array(int32_t nbelts, TorusPolynomial* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	(obj+i)->~TorusPolynomial();
    }
}
 
//allocates and initialize the TorusPolynomial structure
//(equivalent of the C++ new)
EXPORT TorusPolynomial* new_TorusPolynomial(const int32_t N) {
    return new TorusPolynomial(N);
}
EXPORT TorusPolynomial* new_TorusPolynomial_array(int32_t nbelts, const int32_t N) {
    TorusPolynomial* obj = alloc_TorusPolynomial_array(nbelts);
    init_TorusPolynomial_array(nbelts,obj,N);
    return obj;
}

//destroys and frees the TorusPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_TorusPolynomial(TorusPolynomial* obj) {
    delete obj;
}
EXPORT void delete_TorusPolynomial_array(int32_t nbelts, TorusPolynomial* obj) {
    destroy_TorusPolynomial_array(nbelts,obj);
    free_TorusPolynomial_array(nbelts,obj);
}
