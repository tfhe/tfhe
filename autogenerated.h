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
//allocate memory space for a LWEBootstrappingKey
EXPORT LWEBootstrappingKey* alloc_LWEBootstrappingKey();
EXPORT LWEBootstrappingKey* alloc_LWEBootstrappingKey_array(int nbelts);

//free memory space for a LWEBootstrappingKey
EXPORT void free_LWEBootstrappingKey(LWEBootstrappingKey* ptr);
EXPORT void free_LWEBootstrappingKey_array(int nbelts, LWEBootstrappingKey* ptr);

//initialize the LWEBootstrappingKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEBootstrappingKey(LWEBootstrappingKey* obj, const LWEParams* in_out_params, const RingGSWParams* bk_params);
EXPORT void init_LWEBootstrappingKey_array(int nbelts, LWEBootstrappingKey* obj, const LWEParams* in_out_params, const RingGSWParams* bk_params);

//destroys the LWEBootstrappingKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEBootstrappingKey(LWEBootstrappingKey* obj);
EXPORT void destroy_LWEBootstrappingKey_array(int nbelts, LWEBootstrappingKey* obj);
 
//allocates and initialize the LWEBootstrappingKey structure
//(equivalent of the C++ new)
EXPORT LWEBootstrappingKey* new_LWEBootstrappingKey(const LWEParams* in_out_params, const RingGSWParams* bk_params);
EXPORT LWEBootstrappingKey* new_LWEBootstrappingKey_array(int nbelts, const LWEParams* in_out_params, const RingGSWParams* bk_params);

//destroys and frees the LWEBootstrappingKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEBootstrappingKey(LWEBootstrappingKey* obj);
EXPORT void delete_LWEBootstrappingKey_array(int nbelts, LWEBootstrappingKey* obj);
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
//allocate memory space for a LWEKeySwitchKey
EXPORT LWEKeySwitchKey* alloc_LWEKeySwitchKey();
EXPORT LWEKeySwitchKey* alloc_LWEKeySwitchKey_array(int nbelts);

//free memory space for a LWEKeySwitchKey
EXPORT void free_LWEKeySwitchKey(LWEKeySwitchKey* ptr);
EXPORT void free_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* ptr);

//initialize the LWEKeySwitchKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LWEKeySwitchKey(LWEKeySwitchKey* obj, int basebit, int kslength, const LWEParams* in_params, const LWEParams* out_params);
EXPORT void init_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* obj, int basebit, int kslength, const LWEParams* in_params, const LWEParams* out_params);

//destroys the LWEKeySwitchKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LWEKeySwitchKey(LWEKeySwitchKey* obj);
EXPORT void destroy_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* obj);
 
//allocates and initialize the LWEKeySwitchKey structure
//(equivalent of the C++ new)
EXPORT LWEKeySwitchKey* new_LWEKeySwitchKey(int basebit, int kslength, const LWEParams* in_params, const LWEParams* out_params);
EXPORT LWEKeySwitchKey* new_LWEKeySwitchKey_array(int nbelts, int basebit, int kslength, const LWEParams* in_params, const LWEParams* out_params);

//destroys and frees the LWEKeySwitchKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LWEKeySwitchKey(LWEKeySwitchKey* obj);
EXPORT void delete_LWEKeySwitchKey_array(int nbelts, LWEKeySwitchKey* obj);
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
//allocate memory space for a LagrangeHalfCPolynomial
EXPORT LagrangeHalfCPolynomial* alloc_LagrangeHalfCPolynomial();
EXPORT LagrangeHalfCPolynomial* alloc_LagrangeHalfCPolynomial_array(int nbelts);

//free memory space for a LagrangeHalfCPolynomial
EXPORT void free_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* ptr);
EXPORT void free_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* ptr);

//initialize the LagrangeHalfCPolynomial structure
//(equivalent of the C++ constructor)
EXPORT void init_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj, const int N);
EXPORT void init_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj, const int N);

//destroys the LagrangeHalfCPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj);
EXPORT void destroy_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj);
 
//allocates and initialize the LagrangeHalfCPolynomial structure
//(equivalent of the C++ new)
EXPORT LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial(const int N);
EXPORT LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial_array(int nbelts, const int N);

//destroys and frees the LagrangeHalfCPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj);
EXPORT void delete_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj);
//allocate memory space for a RingGSWKey
EXPORT RingGSWKey* alloc_RingGSWKey();
EXPORT RingGSWKey* alloc_RingGSWKey_array(int nbelts);

//free memory space for a RingGSWKey
EXPORT void free_RingGSWKey(RingGSWKey* ptr);
EXPORT void free_RingGSWKey_array(int nbelts, RingGSWKey* ptr);

//initialize the RingGSWKey structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWKey(RingGSWKey* obj, const RingGSWParams* params);
EXPORT void init_RingGSWKey_array(int nbelts, RingGSWKey* obj, const RingGSWParams* params);

//destroys the RingGSWKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWKey(RingGSWKey* obj);
EXPORT void destroy_RingGSWKey_array(int nbelts, RingGSWKey* obj);
 
//allocates and initialize the RingGSWKey structure
//(equivalent of the C++ new)
EXPORT RingGSWKey* new_RingGSWKey(const RingGSWParams* params);
EXPORT RingGSWKey* new_RingGSWKey_array(int nbelts, const RingGSWParams* params);

//destroys and frees the RingGSWKey structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWKey(RingGSWKey* obj);
EXPORT void delete_RingGSWKey_array(int nbelts, RingGSWKey* obj);
//allocate memory space for a RingGSWParams
EXPORT RingGSWParams* alloc_RingGSWParams();
EXPORT RingGSWParams* alloc_RingGSWParams_array(int nbelts);

//free memory space for a RingGSWParams
EXPORT void free_RingGSWParams(RingGSWParams* ptr);
EXPORT void free_RingGSWParams_array(int nbelts, RingGSWParams* ptr);

//initialize the RingGSWParams structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWParams(RingGSWParams* obj, int l, int Bgbit, RingLWEParams* ringlwe_params);
EXPORT void init_RingGSWParams_array(int nbelts, RingGSWParams* obj, int l, int Bgbit, RingLWEParams* ringlwe_params);

//destroys the RingGSWParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWParams(RingGSWParams* obj);
EXPORT void destroy_RingGSWParams_array(int nbelts, RingGSWParams* obj);
 
//allocates and initialize the RingGSWParams structure
//(equivalent of the C++ new)
EXPORT RingGSWParams* new_RingGSWParams(int l, int Bgbit, RingLWEParams* ringlwe_params);
EXPORT RingGSWParams* new_RingGSWParams_array(int nbelts, int l, int Bgbit, RingLWEParams* ringlwe_params);

//destroys and frees the RingGSWParams structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWParams(RingGSWParams* obj);
EXPORT void delete_RingGSWParams_array(int nbelts, RingGSWParams* obj);
//allocate memory space for a RingGSWSample
EXPORT RingGSWSample* alloc_RingGSWSample();
EXPORT RingGSWSample* alloc_RingGSWSample_array(int nbelts);

//free memory space for a RingGSWSample
EXPORT void free_RingGSWSample(RingGSWSample* ptr);
EXPORT void free_RingGSWSample_array(int nbelts, RingGSWSample* ptr);

//initialize the RingGSWSample structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWSample(RingGSWSample* obj, const RingGSWParams* params);
EXPORT void init_RingGSWSample_array(int nbelts, RingGSWSample* obj, const RingGSWParams* params);

//destroys the RingGSWSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWSample(RingGSWSample* obj);
EXPORT void destroy_RingGSWSample_array(int nbelts, RingGSWSample* obj);
 
//allocates and initialize the RingGSWSample structure
//(equivalent of the C++ new)
EXPORT RingGSWSample* new_RingGSWSample(const RingGSWParams* params);
EXPORT RingGSWSample* new_RingGSWSample_array(int nbelts, const RingGSWParams* params);

//destroys and frees the RingGSWSample structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWSample(RingGSWSample* obj);
EXPORT void delete_RingGSWSample_array(int nbelts, RingGSWSample* obj);
//allocate memory space for a RingGSWSampleFFT
EXPORT RingGSWSampleFFT* alloc_RingGSWSampleFFT();
EXPORT RingGSWSampleFFT* alloc_RingGSWSampleFFT_array(int nbelts);

//free memory space for a RingGSWSampleFFT
EXPORT void free_RingGSWSampleFFT(RingGSWSampleFFT* ptr);
EXPORT void free_RingGSWSampleFFT_array(int nbelts, RingGSWSampleFFT* ptr);

//initialize the RingGSWSampleFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_RingGSWSampleFFT(RingGSWSampleFFT* obj, const RingGSWParams* params);
EXPORT void init_RingGSWSampleFFT_array(int nbelts, RingGSWSampleFFT* obj, const RingGSWParams* params);

//destroys the RingGSWSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingGSWSampleFFT(RingGSWSampleFFT* obj);
EXPORT void destroy_RingGSWSampleFFT_array(int nbelts, RingGSWSampleFFT* obj);
 
//allocates and initialize the RingGSWSampleFFT structure
//(equivalent of the C++ new)
EXPORT RingGSWSampleFFT* new_RingGSWSampleFFT(const RingGSWParams* params);
EXPORT RingGSWSampleFFT* new_RingGSWSampleFFT_array(int nbelts, const RingGSWParams* params);

//destroys and frees the RingGSWSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_RingGSWSampleFFT(RingGSWSampleFFT* obj);
EXPORT void delete_RingGSWSampleFFT_array(int nbelts, RingGSWSampleFFT* obj);
//allocate memory space for a RingLWEKey
EXPORT RingLWEKey* alloc_RingLWEKey();
EXPORT RingLWEKey* alloc_RingLWEKey_array(int nbelts);

//free memory space for a RingLWEKey
EXPORT void free_RingLWEKey(RingLWEKey* ptr);
EXPORT void free_RingLWEKey_array(int nbelts, RingLWEKey* ptr);

//initialize the RingLWEKey structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWEKey(RingLWEKey* obj, const RingLWEParams* params);
EXPORT void init_RingLWEKey_array(int nbelts, RingLWEKey* obj, const RingLWEParams* params);

//destroys the RingLWEKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWEKey(RingLWEKey* obj);
EXPORT void destroy_RingLWEKey_array(int nbelts, RingLWEKey* obj);
 
//allocates and initialize the RingLWEKey structure
//(equivalent of the C++ new)
EXPORT RingLWEKey* new_RingLWEKey(const RingLWEParams* params);
EXPORT RingLWEKey* new_RingLWEKey_array(int nbelts, const RingLWEParams* params);

//destroys and frees the RingLWEKey structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWEKey(RingLWEKey* obj);
EXPORT void delete_RingLWEKey_array(int nbelts, RingLWEKey* obj);
//allocate memory space for a RingLWEParams
EXPORT RingLWEParams* alloc_RingLWEParams();
EXPORT RingLWEParams* alloc_RingLWEParams_array(int nbelts);

//free memory space for a RingLWEParams
EXPORT void free_RingLWEParams(RingLWEParams* ptr);
EXPORT void free_RingLWEParams_array(int nbelts, RingLWEParams* ptr);

//initialize the RingLWEParams structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWEParams(RingLWEParams* obj, int N, int k, double alpha_min, double alpha_max);
EXPORT void init_RingLWEParams_array(int nbelts, RingLWEParams* obj, int N, int k, double alpha_min, double alpha_max);

//destroys the RingLWEParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWEParams(RingLWEParams* obj);
EXPORT void destroy_RingLWEParams_array(int nbelts, RingLWEParams* obj);
 
//allocates and initialize the RingLWEParams structure
//(equivalent of the C++ new)
EXPORT RingLWEParams* new_RingLWEParams(int N, int k, double alpha_min, double alpha_max);
EXPORT RingLWEParams* new_RingLWEParams_array(int nbelts, int N, int k, double alpha_min, double alpha_max);

//destroys and frees the RingLWEParams structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWEParams(RingLWEParams* obj);
EXPORT void delete_RingLWEParams_array(int nbelts, RingLWEParams* obj);
//allocate memory space for a RingLWESample
EXPORT RingLWESample* alloc_RingLWESample();
EXPORT RingLWESample* alloc_RingLWESample_array(int nbelts);

//free memory space for a RingLWESample
EXPORT void free_RingLWESample(RingLWESample* ptr);
EXPORT void free_RingLWESample_array(int nbelts, RingLWESample* ptr);

//initialize the RingLWESample structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWESample(RingLWESample* obj, const RingLWEParams* params);
EXPORT void init_RingLWESample_array(int nbelts, RingLWESample* obj, const RingLWEParams* params);

//destroys the RingLWESample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWESample(RingLWESample* obj);
EXPORT void destroy_RingLWESample_array(int nbelts, RingLWESample* obj);
 
//allocates and initialize the RingLWESample structure
//(equivalent of the C++ new)
EXPORT RingLWESample* new_RingLWESample(const RingLWEParams* params);
EXPORT RingLWESample* new_RingLWESample_array(int nbelts, const RingLWEParams* params);

//destroys and frees the RingLWESample structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWESample(RingLWESample* obj);
EXPORT void delete_RingLWESample_array(int nbelts, RingLWESample* obj);
//allocate memory space for a RingLWESampleFFT
EXPORT RingLWESampleFFT* alloc_RingLWESampleFFT();
EXPORT RingLWESampleFFT* alloc_RingLWESampleFFT_array(int nbelts);

//free memory space for a RingLWESampleFFT
EXPORT void free_RingLWESampleFFT(RingLWESampleFFT* ptr);
EXPORT void free_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* ptr);

//initialize the RingLWESampleFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_RingLWESampleFFT(RingLWESampleFFT* obj, const RingLWEParams* params);
EXPORT void init_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* obj, const RingLWEParams* params);

//destroys the RingLWESampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_RingLWESampleFFT(RingLWESampleFFT* obj);
EXPORT void destroy_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* obj);
 
//allocates and initialize the RingLWESampleFFT structure
//(equivalent of the C++ new)
EXPORT RingLWESampleFFT* new_RingLWESampleFFT(const RingLWEParams* params);
EXPORT RingLWESampleFFT* new_RingLWESampleFFT_array(int nbelts, const RingLWEParams* params);

//destroys and frees the RingLWESampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_RingLWESampleFFT(RingLWESampleFFT* obj);
EXPORT void delete_RingLWESampleFFT_array(int nbelts, RingLWESampleFFT* obj);
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
