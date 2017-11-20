#ifndef LWEPARAMS_H
#define LWEPARAMS_H

///@file
///@brief This file contains the declaration of lwe parameters structures

#include "tfhe_core.h"

//this structure contains Lwe parameters
//this structure is constant (cannot be modified once initialized): 
//the pointer to the param can be passed directly
//to all the Lwe keys that use these params.
struct LweParams {
	const int32_t n;
	const double alpha_min;//le plus petit bruit tq sur
	const double alpha_max;//le plus gd bruit qui permet le déchiffrement



//since all members are declared constant, a constructor is 
//required in the structure.
#ifdef __cplusplus
	LweParams(int32_t n, double alpha_min, double alpha_max);
	~LweParams();
	LweParams(const LweParams&) = delete; //forbidden
	LweParams& operator=(const LweParams& ) = delete; //forbidden
#endif
};

//allocate memory space for a LweParams
EXPORT LweParams* alloc_LweParams();
EXPORT LweParams* alloc_LweParams_array(int32_t nbelts);

//free memory space for a LweParams
EXPORT void free_LweParams(LweParams* ptr);
EXPORT void free_LweParams_array(int32_t nbelts, LweParams* ptr);

//initialize the LweParams structure
//(equivalent of the C++ constructor)
EXPORT void init_LweParams(LweParams* obj, int32_t n, double alpha_min, double alpha_max);
EXPORT void init_LweParams_array(int32_t nbelts, LweParams* obj, int32_t n, double alpha_min, double alpha_max);

//destroys the LweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweParams(LweParams* obj);
EXPORT void destroy_LweParams_array(int32_t nbelts, LweParams* obj);
 
//allocates and initialize the LweParams structure
//(equivalent of the C++ new)
EXPORT LweParams* new_LweParams(int32_t n, double alpha_min, double alpha_max);
EXPORT LweParams* new_LweParams_array(int32_t nbelts, int32_t n, double alpha_min, double alpha_max);

//destroys and frees the LweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_LweParams(LweParams* obj);
EXPORT void delete_LweParams_array(int32_t nbelts, LweParams* obj);

#endif //LWEPARAMS_H
