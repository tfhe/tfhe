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
//allocate memory space for a LweBootstrappingKey
EXPORT LweBootstrappingKey* alloc_LweBootstrappingKey();
EXPORT LweBootstrappingKey* alloc_LweBootstrappingKey_array(int nbelts);

//free memory space for a LweBootstrappingKey
EXPORT void free_LweBootstrappingKey(LweBootstrappingKey* ptr);
EXPORT void free_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* ptr);

//initialize the LweBootstrappingKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LweBootstrappingKey(LweBootstrappingKey* obj, const LweParams* in_out_params, const TGswParams* bk_params);
EXPORT void init_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* obj, const LweParams* in_out_params, const TGswParams* bk_params);

//destroys the LweBootstrappingKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweBootstrappingKey(LweBootstrappingKey* obj);
EXPORT void destroy_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* obj);
 
//allocates and initialize the LweBootstrappingKey structure
//(equivalent of the C++ new)
EXPORT LweBootstrappingKey* new_LweBootstrappingKey(const LweParams* in_out_params, const TGswParams* bk_params);
EXPORT LweBootstrappingKey* new_LweBootstrappingKey_array(int nbelts, const LweParams* in_out_params, const TGswParams* bk_params);

//destroys and frees the LweBootstrappingKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweBootstrappingKey(LweBootstrappingKey* obj);
EXPORT void delete_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey* obj);
//allocate memory space for a LweBootstrappingKeyFFT
EXPORT LweBootstrappingKeyFFT* alloc_LweBootstrappingKeyFFT();
EXPORT LweBootstrappingKeyFFT* alloc_LweBootstrappingKeyFFT_array(int nbelts);

//free memory space for a LweBootstrappingKeyFFT
EXPORT void free_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* ptr);
EXPORT void free_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* ptr);

//initialize the LweBootstrappingKeyFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* obj, const LweParams* in_out_params, const TGswParams* bk_params);
EXPORT void init_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* obj, const LweParams* in_out_params, const TGswParams* bk_params);

//destroys the LweBootstrappingKeyFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* obj);
EXPORT void destroy_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* obj);
 
//allocates and initialize the LweBootstrappingKeyFFT structure
//(equivalent of the C++ new)
EXPORT LweBootstrappingKeyFFT* new_LweBootstrappingKeyFFT(const LweParams* in_out_params, const TGswParams* bk_params);
EXPORT LweBootstrappingKeyFFT* new_LweBootstrappingKeyFFT_array(int nbelts, const LweParams* in_out_params, const TGswParams* bk_params);

//destroys and frees the LweBootstrappingKeyFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT* obj);
EXPORT void delete_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT* obj);
//allocate memory space for a LweKey
EXPORT LweKey* alloc_LweKey();
EXPORT LweKey* alloc_LweKey_array(int nbelts);

//free memory space for a LweKey
EXPORT void free_LweKey(LweKey* ptr);
EXPORT void free_LweKey_array(int nbelts, LweKey* ptr);

//initialize the LweKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LweKey(LweKey* obj, const LweParams* params);
EXPORT void init_LweKey_array(int nbelts, LweKey* obj, const LweParams* params);

//destroys the LweKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweKey(LweKey* obj);
EXPORT void destroy_LweKey_array(int nbelts, LweKey* obj);
 
//allocates and initialize the LweKey structure
//(equivalent of the C++ new)
EXPORT LweKey* new_LweKey(const LweParams* params);
EXPORT LweKey* new_LweKey_array(int nbelts, const LweParams* params);

//destroys and frees the LweKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweKey(LweKey* obj);
EXPORT void delete_LweKey_array(int nbelts, LweKey* obj);
//allocate memory space for a LweKeySwitchKey
EXPORT LweKeySwitchKey* alloc_LweKeySwitchKey();
EXPORT LweKeySwitchKey* alloc_LweKeySwitchKey_array(int nbelts);

//free memory space for a LweKeySwitchKey
EXPORT void free_LweKeySwitchKey(LweKeySwitchKey* ptr);
EXPORT void free_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* ptr);

//initialize the LweKeySwitchKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LweKeySwitchKey(LweKeySwitchKey* obj, int basebit, int kslength, const LweParams* in_params, const LweParams* out_params);
EXPORT void init_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* obj, int basebit, int kslength, const LweParams* in_params, const LweParams* out_params);

//destroys the LweKeySwitchKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweKeySwitchKey(LweKeySwitchKey* obj);
EXPORT void destroy_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* obj);
 
//allocates and initialize the LweKeySwitchKey structure
//(equivalent of the C++ new)
EXPORT LweKeySwitchKey* new_LweKeySwitchKey(int basebit, int kslength, const LweParams* in_params, const LweParams* out_params);
EXPORT LweKeySwitchKey* new_LweKeySwitchKey_array(int nbelts, int basebit, int kslength, const LweParams* in_params, const LweParams* out_params);

//destroys and frees the LweKeySwitchKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweKeySwitchKey(LweKeySwitchKey* obj);
EXPORT void delete_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey* obj);
//allocate memory space for a LweParams
EXPORT LweParams* alloc_LweParams();
EXPORT LweParams* alloc_LweParams_array(int nbelts);

//free memory space for a LweParams
EXPORT void free_LweParams(LweParams* ptr);
EXPORT void free_LweParams_array(int nbelts, LweParams* ptr);

//initialize the LweParams structure
//(equivalent of the C++ constructor)
EXPORT void init_LweParams(LweParams* obj, int n, double alpha_min, double alpha_max);
EXPORT void init_LweParams_array(int nbelts, LweParams* obj, int n, double alpha_min, double alpha_max);

//destroys the LweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweParams(LweParams* obj);
EXPORT void destroy_LweParams_array(int nbelts, LweParams* obj);
 
//allocates and initialize the LweParams structure
//(equivalent of the C++ new)
EXPORT LweParams* new_LweParams(int n, double alpha_min, double alpha_max);
EXPORT LweParams* new_LweParams_array(int nbelts, int n, double alpha_min, double alpha_max);

//destroys and frees the LweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_LweParams(LweParams* obj);
EXPORT void delete_LweParams_array(int nbelts, LweParams* obj);
//allocate memory space for a LweSample
EXPORT LweSample* alloc_LweSample();
EXPORT LweSample* alloc_LweSample_array(int nbelts);

//free memory space for a LweSample
EXPORT void free_LweSample(LweSample* ptr);
EXPORT void free_LweSample_array(int nbelts, LweSample* ptr);

//initialize the LweSample structure
//(equivalent of the C++ constructor)
EXPORT void init_LweSample(LweSample* obj, const LweParams* params);
EXPORT void init_LweSample_array(int nbelts, LweSample* obj, const LweParams* params);

//destroys the LweSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweSample(LweSample* obj);
EXPORT void destroy_LweSample_array(int nbelts, LweSample* obj);
 
//allocates and initialize the LweSample structure
//(equivalent of the C++ new)
EXPORT LweSample* new_LweSample(const LweParams* params);
EXPORT LweSample* new_LweSample_array(int nbelts, const LweParams* params);

//destroys and frees the LweSample structure
//(equivalent of the C++ delete)
EXPORT void delete_LweSample(LweSample* obj);
EXPORT void delete_LweSample_array(int nbelts, LweSample* obj);

//allocate memory space for a TGswKey
EXPORT TGswKey* alloc_TGswKey();
EXPORT TGswKey* alloc_TGswKey_array(int nbelts);

//free memory space for a TGswKey
EXPORT void free_TGswKey(TGswKey* ptr);
EXPORT void free_TGswKey_array(int nbelts, TGswKey* ptr);

//initialize the TGswKey structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswKey(TGswKey* obj, const TGswParams* params);
EXPORT void init_TGswKey_array(int nbelts, TGswKey* obj, const TGswParams* params);

//destroys the TGswKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswKey(TGswKey* obj);
EXPORT void destroy_TGswKey_array(int nbelts, TGswKey* obj);
 
//allocates and initialize the TGswKey structure
//(equivalent of the C++ new)
EXPORT TGswKey* new_TGswKey(const TGswParams* params);
EXPORT TGswKey* new_TGswKey_array(int nbelts, const TGswParams* params);

//destroys and frees the TGswKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswKey(TGswKey* obj);
EXPORT void delete_TGswKey_array(int nbelts, TGswKey* obj);
//allocate memory space for a TGswParams
EXPORT TGswParams* alloc_TGswParams();
EXPORT TGswParams* alloc_TGswParams_array(int nbelts);

//free memory space for a TGswParams
EXPORT void free_TGswParams(TGswParams* ptr);
EXPORT void free_TGswParams_array(int nbelts, TGswParams* ptr);

//initialize the TGswParams structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswParams(TGswParams* obj, int l, int Bgbit, TLweParams* tlwe_params);
EXPORT void init_TGswParams_array(int nbelts, TGswParams* obj, int l, int Bgbit, TLweParams* tlwe_params);

//destroys the TGswParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswParams(TGswParams* obj);
EXPORT void destroy_TGswParams_array(int nbelts, TGswParams* obj);
 
//allocates and initialize the TGswParams structure
//(equivalent of the C++ new)
EXPORT TGswParams* new_TGswParams(int l, int Bgbit, TLweParams* tlwe_params);
EXPORT TGswParams* new_TGswParams_array(int nbelts, int l, int Bgbit, TLweParams* tlwe_params);

//destroys and frees the TGswParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswParams(TGswParams* obj);
EXPORT void delete_TGswParams_array(int nbelts, TGswParams* obj);
//allocate memory space for a TGswSample
EXPORT TGswSample* alloc_TGswSample();
EXPORT TGswSample* alloc_TGswSample_array(int nbelts);

//free memory space for a TGswSample
EXPORT void free_TGswSample(TGswSample* ptr);
EXPORT void free_TGswSample_array(int nbelts, TGswSample* ptr);

//initialize the TGswSample structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSample(TGswSample* obj, const TGswParams* params);
EXPORT void init_TGswSample_array(int nbelts, TGswSample* obj, const TGswParams* params);

//destroys the TGswSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSample(TGswSample* obj);
EXPORT void destroy_TGswSample_array(int nbelts, TGswSample* obj);
 
//allocates and initialize the TGswSample structure
//(equivalent of the C++ new)
EXPORT TGswSample* new_TGswSample(const TGswParams* params);
EXPORT TGswSample* new_TGswSample_array(int nbelts, const TGswParams* params);

//destroys and frees the TGswSample structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswSample(TGswSample* obj);
EXPORT void delete_TGswSample_array(int nbelts, TGswSample* obj);
//allocate memory space for a TGswSampleFFT
EXPORT TGswSampleFFT* alloc_TGswSampleFFT();
EXPORT TGswSampleFFT* alloc_TGswSampleFFT_array(int nbelts);

//free memory space for a TGswSampleFFT
EXPORT void free_TGswSampleFFT(TGswSampleFFT* ptr);
EXPORT void free_TGswSampleFFT_array(int nbelts, TGswSampleFFT* ptr);

//initialize the TGswSampleFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSampleFFT(TGswSampleFFT* obj, const TGswParams* params);
EXPORT void init_TGswSampleFFT_array(int nbelts, TGswSampleFFT* obj, const TGswParams* params);

//destroys the TGswSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSampleFFT(TGswSampleFFT* obj);
EXPORT void destroy_TGswSampleFFT_array(int nbelts, TGswSampleFFT* obj);
 
//allocates and initialize the TGswSampleFFT structure
//(equivalent of the C++ new)
EXPORT TGswSampleFFT* new_TGswSampleFFT(const TGswParams* params);
EXPORT TGswSampleFFT* new_TGswSampleFFT_array(int nbelts, const TGswParams* params);

//destroys and frees the TGswSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswSampleFFT(TGswSampleFFT* obj);
EXPORT void delete_TGswSampleFFT_array(int nbelts, TGswSampleFFT* obj);
//allocate memory space for a TLweKey
EXPORT TLweKey* alloc_TLweKey();
EXPORT TLweKey* alloc_TLweKey_array(int nbelts);

//free memory space for a TLweKey
EXPORT void free_TLweKey(TLweKey* ptr);
EXPORT void free_TLweKey_array(int nbelts, TLweKey* ptr);

//initialize the TLweKey structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweKey(TLweKey* obj, const TLweParams* params);
EXPORT void init_TLweKey_array(int nbelts, TLweKey* obj, const TLweParams* params);

//destroys the TLweKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweKey(TLweKey* obj);
EXPORT void destroy_TLweKey_array(int nbelts, TLweKey* obj);
 
//allocates and initialize the TLweKey structure
//(equivalent of the C++ new)
EXPORT TLweKey* new_TLweKey(const TLweParams* params);
EXPORT TLweKey* new_TLweKey_array(int nbelts, const TLweParams* params);

//destroys and frees the TLweKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweKey(TLweKey* obj);
EXPORT void delete_TLweKey_array(int nbelts, TLweKey* obj);
//allocate memory space for a TLweParams
EXPORT TLweParams* alloc_TLweParams();
EXPORT TLweParams* alloc_TLweParams_array(int nbelts);

//free memory space for a TLweParams
EXPORT void free_TLweParams(TLweParams* ptr);
EXPORT void free_TLweParams_array(int nbelts, TLweParams* ptr);

//initialize the TLweParams structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweParams(TLweParams* obj, int N, int k, double alpha_min, double alpha_max);
EXPORT void init_TLweParams_array(int nbelts, TLweParams* obj, int N, int k, double alpha_min, double alpha_max);

//destroys the TLweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweParams(TLweParams* obj);
EXPORT void destroy_TLweParams_array(int nbelts, TLweParams* obj);
 
//allocates and initialize the TLweParams structure
//(equivalent of the C++ new)
EXPORT TLweParams* new_TLweParams(int N, int k, double alpha_min, double alpha_max);
EXPORT TLweParams* new_TLweParams_array(int nbelts, int N, int k, double alpha_min, double alpha_max);

//destroys and frees the TLweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweParams(TLweParams* obj);
EXPORT void delete_TLweParams_array(int nbelts, TLweParams* obj);
//allocate memory space for a TLweSample
EXPORT TLweSample* alloc_TLweSample();
EXPORT TLweSample* alloc_TLweSample_array(int nbelts);

//free memory space for a TLweSample
EXPORT void free_TLweSample(TLweSample* ptr);
EXPORT void free_TLweSample_array(int nbelts, TLweSample* ptr);

//initialize the TLweSample structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweSample(TLweSample* obj, const TLweParams* params);
EXPORT void init_TLweSample_array(int nbelts, TLweSample* obj, const TLweParams* params);

//destroys the TLweSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweSample(TLweSample* obj);
EXPORT void destroy_TLweSample_array(int nbelts, TLweSample* obj);
 
//allocates and initialize the TLweSample structure
//(equivalent of the C++ new)
EXPORT TLweSample* new_TLweSample(const TLweParams* params);
EXPORT TLweSample* new_TLweSample_array(int nbelts, const TLweParams* params);

//destroys and frees the TLweSample structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweSample(TLweSample* obj);
EXPORT void delete_TLweSample_array(int nbelts, TLweSample* obj);
//allocate memory space for a TLweSampleFFT
EXPORT TLweSampleFFT* alloc_TLweSampleFFT();
EXPORT TLweSampleFFT* alloc_TLweSampleFFT_array(int nbelts);

//free memory space for a TLweSampleFFT
EXPORT void free_TLweSampleFFT(TLweSampleFFT* ptr);
EXPORT void free_TLweSampleFFT_array(int nbelts, TLweSampleFFT* ptr);

//initialize the TLweSampleFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweSampleFFT(TLweSampleFFT* obj, const TLweParams* params);
EXPORT void init_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj, const TLweParams* params);

//destroys the TLweSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweSampleFFT(TLweSampleFFT* obj);
EXPORT void destroy_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj);
 
//allocates and initialize the TLweSampleFFT structure
//(equivalent of the C++ new)
EXPORT TLweSampleFFT* new_TLweSampleFFT(const TLweParams* params);
EXPORT TLweSampleFFT* new_TLweSampleFFT_array(int nbelts, const TLweParams* params);

//destroys and frees the TLweSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweSampleFFT(TLweSampleFFT* obj);
EXPORT void delete_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj);
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
