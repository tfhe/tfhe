//THIS  FILE IS AUTOMATICALLY GENERATED
//DO NOT EDIT BY HANDS
#include "lwe.h"
#include <cstdlib>
#include <new>
using namespace std;
#include "multiplication.h" 
//allocate memory space for a IntPolynomial

EXPORT IntPolynomial* alloc_IntPolynomial() {
    return (IntPolynomial*) malloc(sizeof(IntPolynomial));
}
EXPORT IntPolynomial* alloc_IntPolynomial_array(int nbelts) {
    return (IntPolynomial*) malloc(nbelts*sizeof(IntPolynomial));
}

//free memory space for a LWEKey
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
#include "lwekey.h" 
//allocate memory space for a LWEKey

EXPORT LWEKey* alloc_LWEKey() {
    return (LWEKey*) malloc(sizeof(LWEKey));
}
EXPORT LWEKey* alloc_LWEKey_array(int nbelts) {
    return (LWEKey*) malloc(nbelts*sizeof(LWEKey));
}

//free memory space for a LWEKey
EXPORT void free_LWEKey(LWEKey* ptr) {
    free(ptr);
}
EXPORT void free_LWEKey_array(int nbelts, LWEKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEKey(LWEKey* obj, const LWEParams* params) {
    new(obj) LWEKey(params);
}
EXPORT void init_LWEKey_array(int nbelts, LWEKey* obj, const LWEParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LWEKey(params);
    }
}

//destroys the LWEKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEKey(LWEKey* obj) {
    obj->~LWEKey();
}
EXPORT void destroy_LWEKey_array(int nbelts, LWEKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LWEKey();
    }
}
 
//allocates and initialize the LWEKey structure
//(equivalent of the C++ new)
EXPORT LWEKey* new_LWEKey(const LWEParams* params) {
    return new LWEKey(params);
}
EXPORT LWEKey* new_LWEKey_array(int nbelts, const LWEParams* params) {
    LWEKey* obj = alloc_LWEKey_array(nbelts);
    init_LWEKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the LWEKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEKey(LWEKey* obj) {
    delete obj;
}
EXPORT void delete_LWEKey_array(int nbelts, LWEKey* obj) {
    destroy_LWEKey_array(nbelts,obj);
    free_LWEKey_array(nbelts,obj);
}
#include "lwekeyswitch.h" 
//allocate memory space for a LWEKeySwitchKey

EXPORT LWEKeySwitchKey* alloc_LWEKeySwitchKey() {
    return (LWEKeySwitchKey*) malloc(sizeof(LWEKeySwitchKey));
}
EXPORT LWEKeySwitchKey* alloc_LWEKeySwitchKey_array(int nbelts) {
    return (LWEKeySwitchKey*) malloc(nbelts*sizeof(LWEKeySwitchKey));
}

//free memory space for a LWEKey
EXPORT void free_LWEKeySwitchKey(LWEKeySwitchKey* ptr) {
    free(ptr);
}
EXPORT void free_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEKeySwitchKey(LWEKeySwitchKey* obj, int base, LWEParams* in_params, LWEParams* out_params) {
    new(obj) LWEKeySwitchKey(base,in_params,out_params);
}
EXPORT void init_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* obj, int base, LWEParams* in_params, LWEParams* out_params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LWEKeySwitchKey(base,in_params,out_params);
    }
}

//destroys the LWEKeySwitchKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEKeySwitchKey(LWEKeySwitchKey* obj) {
    obj->~LWEKeySwitchKey();
}
EXPORT void destroy_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LWEKeySwitchKey();
    }
}
 
//allocates and initialize the LWEKeySwitchKey structure
//(equivalent of the C++ new)
EXPORT LWEKeySwitchKey* new_LWEKeySwitchKey(int base, LWEParams* in_params, LWEParams* out_params) {
    return new LWEKeySwitchKey(base,in_params,out_params);
}
EXPORT LWEKeySwitchKey* new_LWEKeySwitchKey_array(int nbelts, int base, LWEParams* in_params, LWEParams* out_params) {
    LWEKeySwitchKey* obj = alloc_LWEKeySwitchKey_array(nbelts);
    init_LWEKeySwitchKey_array(nbelts,obj,base,in_params,out_params);
    return obj;
}

//destroys and frees the LWEKeySwitchKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEKeySwitchKey(LWEKeySwitchKey* obj) {
    delete obj;
}
EXPORT void delete_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* obj) {
    destroy_LWEKeySwitchKey_array(nbelts,obj);
    free_LWEKeySwitchKey_array(nbelts,obj);
}
#include "lweparams.h" 
//allocate memory space for a LWEParams

EXPORT LWEParams* alloc_LWEParams() {
    return (LWEParams*) malloc(sizeof(LWEParams));
}
EXPORT LWEParams* alloc_LWEParams_array(int nbelts) {
    return (LWEParams*) malloc(nbelts*sizeof(LWEParams));
}

//free memory space for a LWEKey
EXPORT void free_LWEParams(LWEParams* ptr) {
    free(ptr);
}
EXPORT void free_LWEParams_array(int nbelts, LWEParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEParams(LWEParams* obj, int n, double alpha_min, double alpha_max) {
    new(obj) LWEParams(n,alpha_min,alpha_max);
}
EXPORT void init_LWEParams_array(int nbelts, LWEParams* obj, int n, double alpha_min, double alpha_max) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LWEParams(n,alpha_min,alpha_max);
    }
}

//destroys the LWEParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEParams(LWEParams* obj) {
    obj->~LWEParams();
}
EXPORT void destroy_LWEParams_array(int nbelts, LWEParams* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LWEParams();
    }
}
 
//allocates and initialize the LWEParams structure
//(equivalent of the C++ new)
EXPORT LWEParams* new_LWEParams(int n, double alpha_min, double alpha_max) {
    return new LWEParams(n,alpha_min,alpha_max);
}
EXPORT LWEParams* new_LWEParams_array(int nbelts, int n, double alpha_min, double alpha_max) {
    LWEParams* obj = alloc_LWEParams_array(nbelts);
    init_LWEParams_array(nbelts,obj,n,alpha_min,alpha_max);
    return obj;
}

//destroys and frees the LWEParams structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEParams(LWEParams* obj) {
    delete obj;
}
EXPORT void delete_LWEParams_array(int nbelts, LWEParams* obj) {
    destroy_LWEParams_array(nbelts,obj);
    free_LWEParams_array(nbelts,obj);
}
#include "lwesamples.h" 
//allocate memory space for a LWESample

EXPORT LWESample* alloc_LWESample() {
    return (LWESample*) malloc(sizeof(LWESample));
}
EXPORT LWESample* alloc_LWESample_array(int nbelts) {
    return (LWESample*) malloc(nbelts*sizeof(LWESample));
}

//free memory space for a LWEKey
EXPORT void free_LWESample(LWESample* ptr) {
    free(ptr);
}
EXPORT void free_LWESample_array(int nbelts, LWESample* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LWESample(LWESample* obj, const LWEParams* params) {
    new(obj) LWESample(params);
}
EXPORT void init_LWESample_array(int nbelts, LWESample* obj, const LWEParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LWESample(params);
    }
}

//destroys the LWESample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWESample(LWESample* obj) {
    obj->~LWESample();
}
EXPORT void destroy_LWESample_array(int nbelts, LWESample* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LWESample();
    }
}
 
//allocates and initialize the LWESample structure
//(equivalent of the C++ new)
EXPORT LWESample* new_LWESample(const LWEParams* params) {
    return new LWESample(params);
}
EXPORT LWESample* new_LWESample_array(int nbelts, const LWEParams* params) {
    LWESample* obj = alloc_LWESample_array(nbelts);
    init_LWESample_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the LWESample structure
//(equivalent of the C++ delete)
EXPORT void delete_LWESample(LWESample* obj) {
    delete obj;
}
EXPORT void delete_LWESample_array(int nbelts, LWESample* obj) {
    destroy_LWESample_array(nbelts,obj);
    free_LWESample_array(nbelts,obj);
}
#include "polynomials.h" 
//allocate memory space for a LagrangeHalfCPolynomial

EXPORT LagrangeHalfCPolynomial* alloc_LagrangeHalfCPolynomial() {
    return (LagrangeHalfCPolynomial*) malloc(sizeof(LagrangeHalfCPolynomial));
}
EXPORT LagrangeHalfCPolynomial* alloc_LagrangeHalfCPolynomial_array(int nbelts) {
    return (LagrangeHalfCPolynomial*) malloc(nbelts*sizeof(LagrangeHalfCPolynomial));
}

//free memory space for a LWEKey
EXPORT void free_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* ptr) {
    free(ptr);
}
EXPORT void free_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj, const int N) {
    new(obj) LagrangeHalfCPolynomial(N);
}
EXPORT void init_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj, const int N) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LagrangeHalfCPolynomial(N);
    }
}

//destroys the LagrangeHalfCPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj) {
    obj->~LagrangeHalfCPolynomial();
}
EXPORT void destroy_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~LagrangeHalfCPolynomial();
    }
}
 
//allocates and initialize the LagrangeHalfCPolynomial structure
//(equivalent of the C++ new)
EXPORT LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial(const int N) {
    return new LagrangeHalfCPolynomial(N);
}
EXPORT LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial_array(int nbelts, const int N) {
    LagrangeHalfCPolynomial* obj = alloc_LagrangeHalfCPolynomial_array(nbelts);
    init_LagrangeHalfCPolynomial_array(nbelts,obj,N);
    return obj;
}

//destroys and frees the LagrangeHalfCPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj) {
    delete obj;
}
EXPORT void delete_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj) {
    destroy_LagrangeHalfCPolynomial_array(nbelts,obj);
    free_LagrangeHalfCPolynomial_array(nbelts,obj);
}
#include "ringgsw.h" 
//allocate memory space for a RingGSWKey

EXPORT RingGSWKey* alloc_RingGSWKey() {
    return (RingGSWKey*) malloc(sizeof(RingGSWKey));
}
EXPORT RingGSWKey* alloc_RingGSWKey_array(int nbelts) {
    return (RingGSWKey*) malloc(nbelts*sizeof(RingGSWKey));
}

//free memory space for a LWEKey
EXPORT void free_RingGSWKey(RingGSWKey* ptr) {
    free(ptr);
}
EXPORT void free_RingGSWKey_array(int nbelts, RingGSWKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWKey(RingGSWKey* obj, const RingGSWParams* params) {
    new(obj) RingGSWKey(params);
}
EXPORT void init_RingGSWKey_array(int nbelts, RingGSWKey* obj, const RingGSWParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingGSWKey(params);
    }
}

//destroys the RingGSWKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWKey(RingGSWKey* obj) {
    obj->~RingGSWKey();
}
EXPORT void destroy_RingGSWKey_array(int nbelts, RingGSWKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingGSWKey();
    }
}
 
//allocates and initialize the RingGSWKey structure
//(equivalent of the C++ new)
EXPORT RingGSWKey* new_RingGSWKey(const RingGSWParams* params) {
    return new RingGSWKey(params);
}
EXPORT RingGSWKey* new_RingGSWKey_array(int nbelts, const RingGSWParams* params) {
    RingGSWKey* obj = alloc_RingGSWKey_array(nbelts);
    init_RingGSWKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the RingGSWKey structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWKey(RingGSWKey* obj) {
    delete obj;
}
EXPORT void delete_RingGSWKey_array(int nbelts, RingGSWKey* obj) {
    destroy_RingGSWKey_array(nbelts,obj);
    free_RingGSWKey_array(nbelts,obj);
}
#include "ringgsw.h" 
//allocate memory space for a RingGSWParams

EXPORT RingGSWParams* alloc_RingGSWParams() {
    return (RingGSWParams*) malloc(sizeof(RingGSWParams));
}
EXPORT RingGSWParams* alloc_RingGSWParams_array(int nbelts) {
    return (RingGSWParams*) malloc(nbelts*sizeof(RingGSWParams));
}

//free memory space for a LWEKey
EXPORT void free_RingGSWParams(RingGSWParams* ptr) {
    free(ptr);
}
EXPORT void free_RingGSWParams_array(int nbelts, RingGSWParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWParams(RingGSWParams* obj, int l, int Bgbit, RingLWEParams* ringlwe_params) {
    new(obj) RingGSWParams(l,Bgbit,ringlwe_params);
}
EXPORT void init_RingGSWParams_array(int nbelts, RingGSWParams* obj, int l, int Bgbit, RingLWEParams* ringlwe_params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingGSWParams(l,Bgbit,ringlwe_params);
    }
}

//destroys the RingGSWParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWParams(RingGSWParams* obj) {
    obj->~RingGSWParams();
}
EXPORT void destroy_RingGSWParams_array(int nbelts, RingGSWParams* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingGSWParams();
    }
}
 
//allocates and initialize the RingGSWParams structure
//(equivalent of the C++ new)
EXPORT RingGSWParams* new_RingGSWParams(int l, int Bgbit, RingLWEParams* ringlwe_params) {
    return new RingGSWParams(l,Bgbit,ringlwe_params);
}
EXPORT RingGSWParams* new_RingGSWParams_array(int nbelts, int l, int Bgbit, RingLWEParams* ringlwe_params) {
    RingGSWParams* obj = alloc_RingGSWParams_array(nbelts);
    init_RingGSWParams_array(nbelts,obj,l,Bgbit,ringlwe_params);
    return obj;
}

//destroys and frees the RingGSWParams structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWParams(RingGSWParams* obj) {
    delete obj;
}
EXPORT void delete_RingGSWParams_array(int nbelts, RingGSWParams* obj) {
    destroy_RingGSWParams_array(nbelts,obj);
    free_RingGSWParams_array(nbelts,obj);
}
#include "ringgsw.h" 
//allocate memory space for a RingGSWSample

EXPORT RingGSWSample* alloc_RingGSWSample() {
    return (RingGSWSample*) malloc(sizeof(RingGSWSample));
}
EXPORT RingGSWSample* alloc_RingGSWSample_array(int nbelts) {
    return (RingGSWSample*) malloc(nbelts*sizeof(RingGSWSample));
}

//free memory space for a LWEKey
EXPORT void free_RingGSWSample(RingGSWSample* ptr) {
    free(ptr);
}
EXPORT void free_RingGSWSample_array(int nbelts, RingGSWSample* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWSample(RingGSWSample* obj, const RingGSWParams* params) {
    new(obj) RingGSWSample(params);
}
EXPORT void init_RingGSWSample_array(int nbelts, RingGSWSample* obj, const RingGSWParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingGSWSample(params);
    }
}

//destroys the RingGSWSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWSample(RingGSWSample* obj) {
    obj->~RingGSWSample();
}
EXPORT void destroy_RingGSWSample_array(int nbelts, RingGSWSample* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingGSWSample();
    }
}
 
//allocates and initialize the RingGSWSample structure
//(equivalent of the C++ new)
EXPORT RingGSWSample* new_RingGSWSample(const RingGSWParams* params) {
    return new RingGSWSample(params);
}
EXPORT RingGSWSample* new_RingGSWSample_array(int nbelts, const RingGSWParams* params) {
    RingGSWSample* obj = alloc_RingGSWSample_array(nbelts);
    init_RingGSWSample_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the RingGSWSample structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWSample(RingGSWSample* obj) {
    delete obj;
}
EXPORT void delete_RingGSWSample_array(int nbelts, RingGSWSample* obj) {
    destroy_RingGSWSample_array(nbelts,obj);
    free_RingGSWSample_array(nbelts,obj);
}
#include "ringlwe.h" 
//allocate memory space for a RingLWEKey

EXPORT RingLWEKey* alloc_RingLWEKey() {
    return (RingLWEKey*) malloc(sizeof(RingLWEKey));
}
EXPORT RingLWEKey* alloc_RingLWEKey_array(int nbelts) {
    return (RingLWEKey*) malloc(nbelts*sizeof(RingLWEKey));
}

//free memory space for a LWEKey
EXPORT void free_RingLWEKey(RingLWEKey* ptr) {
    free(ptr);
}
EXPORT void free_RingLWEKey_array(int nbelts, RingLWEKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWEKey(RingLWEKey* obj, const RingLWEParams* params) {
    new(obj) RingLWEKey(params);
}
EXPORT void init_RingLWEKey_array(int nbelts, RingLWEKey* obj, const RingLWEParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingLWEKey(params);
    }
}

//destroys the RingLWEKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWEKey(RingLWEKey* obj) {
    obj->~RingLWEKey();
}
EXPORT void destroy_RingLWEKey_array(int nbelts, RingLWEKey* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingLWEKey();
    }
}
 
//allocates and initialize the RingLWEKey structure
//(equivalent of the C++ new)
EXPORT RingLWEKey* new_RingLWEKey(const RingLWEParams* params) {
    return new RingLWEKey(params);
}
EXPORT RingLWEKey* new_RingLWEKey_array(int nbelts, const RingLWEParams* params) {
    RingLWEKey* obj = alloc_RingLWEKey_array(nbelts);
    init_RingLWEKey_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the RingLWEKey structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWEKey(RingLWEKey* obj) {
    delete obj;
}
EXPORT void delete_RingLWEKey_array(int nbelts, RingLWEKey* obj) {
    destroy_RingLWEKey_array(nbelts,obj);
    free_RingLWEKey_array(nbelts,obj);
}
#include "ringlwe.h" 
//allocate memory space for a RingLWEParams

EXPORT RingLWEParams* alloc_RingLWEParams() {
    return (RingLWEParams*) malloc(sizeof(RingLWEParams));
}
EXPORT RingLWEParams* alloc_RingLWEParams_array(int nbelts) {
    return (RingLWEParams*) malloc(nbelts*sizeof(RingLWEParams));
}

//free memory space for a LWEKey
EXPORT void free_RingLWEParams(RingLWEParams* ptr) {
    free(ptr);
}
EXPORT void free_RingLWEParams_array(int nbelts, RingLWEParams* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWEParams(RingLWEParams* obj, int N, int k, double alpha_min, double alpha_max) {
    new(obj) RingLWEParams(N,k,alpha_min,alpha_max);
}
EXPORT void init_RingLWEParams_array(int nbelts, RingLWEParams* obj, int N, int k, double alpha_min, double alpha_max) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingLWEParams(N,k,alpha_min,alpha_max);
    }
}

//destroys the RingLWEParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWEParams(RingLWEParams* obj) {
    obj->~RingLWEParams();
}
EXPORT void destroy_RingLWEParams_array(int nbelts, RingLWEParams* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingLWEParams();
    }
}
 
//allocates and initialize the RingLWEParams structure
//(equivalent of the C++ new)
EXPORT RingLWEParams* new_RingLWEParams(int N, int k, double alpha_min, double alpha_max) {
    return new RingLWEParams(N,k,alpha_min,alpha_max);
}
EXPORT RingLWEParams* new_RingLWEParams_array(int nbelts, int N, int k, double alpha_min, double alpha_max) {
    RingLWEParams* obj = alloc_RingLWEParams_array(nbelts);
    init_RingLWEParams_array(nbelts,obj,N,k,alpha_min,alpha_max);
    return obj;
}

//destroys and frees the RingLWEParams structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWEParams(RingLWEParams* obj) {
    delete obj;
}
EXPORT void delete_RingLWEParams_array(int nbelts, RingLWEParams* obj) {
    destroy_RingLWEParams_array(nbelts,obj);
    free_RingLWEParams_array(nbelts,obj);
}
#include "ringlwe.h" 
//allocate memory space for a RingLWESample

EXPORT RingLWESample* alloc_RingLWESample() {
    return (RingLWESample*) malloc(sizeof(RingLWESample));
}
EXPORT RingLWESample* alloc_RingLWESample_array(int nbelts) {
    return (RingLWESample*) malloc(nbelts*sizeof(RingLWESample));
}

//free memory space for a LWEKey
EXPORT void free_RingLWESample(RingLWESample* ptr) {
    free(ptr);
}
EXPORT void free_RingLWESample_array(int nbelts, RingLWESample* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWESample(RingLWESample* obj, const RingLWEParams* params) {
    new(obj) RingLWESample(params);
}
EXPORT void init_RingLWESample_array(int nbelts, RingLWESample* obj, const RingLWEParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingLWESample(params);
    }
}

//destroys the RingLWESample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWESample(RingLWESample* obj) {
    obj->~RingLWESample();
}
EXPORT void destroy_RingLWESample_array(int nbelts, RingLWESample* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingLWESample();
    }
}
 
//allocates and initialize the RingLWESample structure
//(equivalent of the C++ new)
EXPORT RingLWESample* new_RingLWESample(const RingLWEParams* params) {
    return new RingLWESample(params);
}
EXPORT RingLWESample* new_RingLWESample_array(int nbelts, const RingLWEParams* params) {
    RingLWESample* obj = alloc_RingLWESample_array(nbelts);
    init_RingLWESample_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the RingLWESample structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWESample(RingLWESample* obj) {
    delete obj;
}
EXPORT void delete_RingLWESample_array(int nbelts, RingLWESample* obj) {
    destroy_RingLWESample_array(nbelts,obj);
    free_RingLWESample_array(nbelts,obj);
}
#include "ringlwe.h" 
//allocate memory space for a RingLWESampleFFT

EXPORT RingLWESampleFFT* alloc_RingLWESampleFFT() {
    return (RingLWESampleFFT*) malloc(sizeof(RingLWESampleFFT));
}
EXPORT RingLWESampleFFT* alloc_RingLWESampleFFT_array(int nbelts) {
    return (RingLWESampleFFT*) malloc(nbelts*sizeof(RingLWESampleFFT));
}

//free memory space for a LWEKey
EXPORT void free_RingLWESampleFFT(RingLWESampleFFT* ptr) {
    free(ptr);
}
EXPORT void free_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWESampleFFT(RingLWESampleFFT* obj, const RingLWEParams* params) {
    new(obj) RingLWESampleFFT(params);
}
EXPORT void init_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* obj, const RingLWEParams* params) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) RingLWESampleFFT(params);
    }
}

//destroys the RingLWESampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWESampleFFT(RingLWESampleFFT* obj) {
    obj->~RingLWESampleFFT();
}
EXPORT void destroy_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* obj) {
    for (int i=0; i<nbelts; i++) {
	(obj+i)->~RingLWESampleFFT();
    }
}
 
//allocates and initialize the RingLWESampleFFT structure
//(equivalent of the C++ new)
EXPORT RingLWESampleFFT* new_RingLWESampleFFT(const RingLWEParams* params) {
    return new RingLWESampleFFT(params);
}
EXPORT RingLWESampleFFT* new_RingLWESampleFFT_array(int nbelts, const RingLWEParams* params) {
    RingLWESampleFFT* obj = alloc_RingLWESampleFFT_array(nbelts);
    init_RingLWESampleFFT_array(nbelts,obj,params);
    return obj;
}

//destroys and frees the RingLWESampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWESampleFFT(RingLWESampleFFT* obj) {
    delete obj;
}
EXPORT void delete_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* obj) {
    destroy_RingLWESampleFFT_array(nbelts,obj);
    free_RingLWESampleFFT_array(nbelts,obj);
}
#include "multiplication.h" 
//allocate memory space for a TorusPolynomial

EXPORT TorusPolynomial* alloc_TorusPolynomial() {
    return (TorusPolynomial*) malloc(sizeof(TorusPolynomial));
}
EXPORT TorusPolynomial* alloc_TorusPolynomial_array(int nbelts) {
    return (TorusPolynomial*) malloc(nbelts*sizeof(TorusPolynomial));
}

//free memory space for a LWEKey
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
