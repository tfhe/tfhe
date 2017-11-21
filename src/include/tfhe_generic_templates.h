#ifndef TFHE_GENERIC_TEMPLATES
#define TFHE_GENERIC_TEMPLATES

///@file
///@brief macros for generic constructors/destructors

#ifdef __cplusplus

namespace tfhe {

    //this macro generates all default function implementations
#define USE_DEFAULT_CONSTRUCTOR_DESTRUCTOR_IMPLEMENTATIONS1(TFHE_TYPE, CONSTR_ARG_TYPE) \
    /* alloc memory */ \
    EXPORT TFHE_TYPE* alloc_ ## TFHE_TYPE() { \
    return (TFHE_TYPE*) malloc(sizeof(TFHE_TYPE)); \
    } \
    \
    EXPORT TFHE_TYPE* alloc_ ## TFHE_TYPE ## _array(int32_t nbelts) { \
    return (TFHE_TYPE*) malloc(nbelts*sizeof(TFHE_TYPE)); \
    } \
    /*free memory */ \
    EXPORT void free_ ## TFHE_TYPE(TFHE_TYPE* ptr) { \
    free(ptr); \
    } \
    EXPORT void free_ ## TFHE_TYPE ## _array(int32_t nbelts, TFHE_TYPE* ptr) { \
    free(ptr); \
    } \
    /* init array */ \
    EXPORT void init_ ## TFHE_TYPE ## _array(int32_t nbelts, TFHE_TYPE* obj, const CONSTR_ARG_TYPE* params) { \
    for (int32_t ii=0; ii<nbelts; ++ii) init_ ## TFHE_TYPE(obj+ii,params); \
    } \
    /* destroy array */ \
    EXPORT void destroy_ ## TFHE_TYPE ## _array(int32_t nbelts, TFHE_TYPE* obj) { \
    for (int32_t ii=0; ii<nbelts; ++ii) destroy_ ## TFHE_TYPE(obj+ii); \
    } \
    /* alloc+init */ \
    EXPORT TFHE_TYPE* new_ ## TFHE_TYPE(const CONSTR_ARG_TYPE* params) { \
    TFHE_TYPE* reps = alloc_ ## TFHE_TYPE(); \
    init_ ## TFHE_TYPE(reps, params); \
    return reps; \
    } \
    EXPORT TFHE_TYPE* new_ ## TFHE_TYPE ## _array(int32_t nbelts, const CONSTR_ARG_TYPE* params) { \
    TFHE_TYPE* reps = alloc_ ## TFHE_TYPE ## _array(nbelts); \
    init_ ## TFHE_TYPE ## _array(nbelts, reps, params); \
    return reps; \
    } \
    /* destroy+free */ \
    EXPORT void delete_ ## TFHE_TYPE(TFHE_TYPE* obj) { \
    destroy_ ## TFHE_TYPE(obj); \
    free_ ## TFHE_TYPE(obj); \
    } \
    EXPORT void delete_ ## TFHE_TYPE ##_array(int32_t nbelts, TFHE_TYPE* obj) { \
    destroy_ ## TFHE_TYPE ## _array(nbelts, obj); \
    free_ ## TFHE_TYPE ## _array(nbelts, obj); \
    }

}

#endif //__cplusplus

#endif // TFHE_GENERIC_TEMPLATES
