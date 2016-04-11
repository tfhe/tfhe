//THIS  FILE IS AUTOMATICALLY GENERATED
//DO NOT EDIT BY HANDS
#include "lwe.h"
#include <cstdlib>
#include <new>
using namespace std;
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
