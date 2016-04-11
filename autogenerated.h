//THIS  FILE IS AUTOMATICALLY GENERATED
//DO NOT EDIT BY HANDS
//allocate memory space for a IntPolynomial
EXPORT IntPolynomial* alloc_IntPolynomial();
EXPORT IntPolynomial* alloc_IntPolynomial_array(int nbelts);

//free memory space for a IntPolynomial
EXPORT void free_IntPolynomial(IntPolynomial* ptr);
EXPORT void free_IntPolynomial_array(int nbelts, IntPolynomial* ptr);

//initialize the IntPolynomial structure
//(equivalent of the C++ constructor)
EXPORT void init_IntPolynomial(IntPolynomial* obj, const int N);
EXPORT void init_IntPolynomial_array(int nbelts, IntPolynomial* obj, const int N);

//destroys the IntPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_IntPolynomial(IntPolynomial* obj);
EXPORT void destroy_IntPolynomial_array(int nbelts, IntPolynomial* obj);
 
//allocates and initialize the IntPolynomial structure
//(equivalent of the C++ new)
EXPORT IntPolynomial* new_IntPolynomial(const int N);
EXPORT IntPolynomial* new_IntPolynomial_array(int nbelts, const int N);

//destroys and frees the IntPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_IntPolynomial(IntPolynomial* obj);
EXPORT void delete_IntPolynomial_array(int nbelts, IntPolynomial* obj);
//allocate memory space for a LWEKey
EXPORT LWEKey* alloc_LWEKey();
EXPORT LWEKey* alloc_LWEKey_array(int nbelts);

//free memory space for a LWEKey
EXPORT void free_LWEKey(LWEKey* ptr);
EXPORT void free_LWEKey_array(int nbelts, LWEKey* ptr);

//initialize the LWEKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEKey(LWEKey* obj, const LWEParams* params);
EXPORT void init_LWEKey_array(int nbelts, LWEKey* obj, const LWEParams* params);

//destroys the LWEKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEKey(LWEKey* obj);
EXPORT void destroy_LWEKey_array(int nbelts, LWEKey* obj);
 
//allocates and initialize the LWEKey structure
//(equivalent of the C++ new)
EXPORT LWEKey* new_LWEKey(const LWEParams* params);
EXPORT LWEKey* new_LWEKey_array(int nbelts, const LWEParams* params);

//destroys and frees the LWEKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEKey(LWEKey* obj);
EXPORT void delete_LWEKey_array(int nbelts, LWEKey* obj);
//allocate memory space for a LWEParams
EXPORT LWEParams* alloc_LWEParams();
EXPORT LWEParams* alloc_LWEParams_array(int nbelts);

//free memory space for a LWEParams
EXPORT void free_LWEParams(LWEParams* ptr);
EXPORT void free_LWEParams_array(int nbelts, LWEParams* ptr);

//initialize the LWEParams structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEParams(LWEParams* obj, int n, double alpha_min, double alpha_max);
EXPORT void init_LWEParams_array(int nbelts, LWEParams* obj, int n, double alpha_min, double alpha_max);

//destroys the LWEParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEParams(LWEParams* obj);
EXPORT void destroy_LWEParams_array(int nbelts, LWEParams* obj);
 
//allocates and initialize the LWEParams structure
//(equivalent of the C++ new)
EXPORT LWEParams* new_LWEParams(int n, double alpha_min, double alpha_max);
EXPORT LWEParams* new_LWEParams_array(int nbelts, int n, double alpha_min, double alpha_max);

//destroys and frees the LWEParams structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEParams(LWEParams* obj);
EXPORT void delete_LWEParams_array(int nbelts, LWEParams* obj);
//allocate memory space for a LWESample
EXPORT LWESample* alloc_LWESample();
EXPORT LWESample* alloc_LWESample_array(int nbelts);

//free memory space for a LWESample
EXPORT void free_LWESample(LWESample* ptr);
EXPORT void free_LWESample_array(int nbelts, LWESample* ptr);

//initialize the LWESample structure
//(equivalent of the C++ constructor)
EXPORT void init_LWESample(LWESample* obj, const LWEParams* params);
EXPORT void init_LWESample_array(int nbelts, LWESample* obj, const LWEParams* params);

//destroys the LWESample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWESample(LWESample* obj);
EXPORT void destroy_LWESample_array(int nbelts, LWESample* obj);
 
//allocates and initialize the LWESample structure
//(equivalent of the C++ new)
EXPORT LWESample* new_LWESample(const LWEParams* params);
EXPORT LWESample* new_LWESample_array(int nbelts, const LWEParams* params);

//destroys and frees the LWESample structure
//(equivalent of the C++ delete)
EXPORT void delete_LWESample(LWESample* obj);
EXPORT void delete_LWESample_array(int nbelts, LWESample* obj);
//allocate memory space for a TorusPolynomial
EXPORT TorusPolynomial* alloc_TorusPolynomial();
EXPORT TorusPolynomial* alloc_TorusPolynomial_array(int nbelts);

//free memory space for a TorusPolynomial
EXPORT void free_TorusPolynomial(TorusPolynomial* ptr);
EXPORT void free_TorusPolynomial_array(int nbelts, TorusPolynomial* ptr);

//initialize the TorusPolynomial structure
//(equivalent of the C++ constructor)
EXPORT void init_TorusPolynomial(TorusPolynomial* obj, const int N);
EXPORT void init_TorusPolynomial_array(int nbelts, TorusPolynomial* obj, const int N);

//destroys the TorusPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TorusPolynomial(TorusPolynomial* obj);
EXPORT void destroy_TorusPolynomial_array(int nbelts, TorusPolynomial* obj);
 
//allocates and initialize the TorusPolynomial structure
//(equivalent of the C++ new)
EXPORT TorusPolynomial* new_TorusPolynomial(const int N);
EXPORT TorusPolynomial* new_TorusPolynomial_array(int nbelts, const int N);

//destroys and frees the TorusPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_TorusPolynomial(TorusPolynomial* obj);
EXPORT void delete_TorusPolynomial_array(int nbelts, TorusPolynomial* obj);
