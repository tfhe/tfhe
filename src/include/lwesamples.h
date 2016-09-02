#ifndef LweSAMPLES_H
#define LweSAMPLES_H

///@file
///@brief This file contains the declaration of lwesamples structures

#include "tfhe_core.h"

struct LweSample {
	Torus32* a; //-- the n coefs of the mask
    Torus32 b;  //
   	double current_variance; //-- average noise of the sample

#ifdef __cplusplus
   LweSample(const LweParams* params);
   ~LweSample();
   LweSample(const LweSample&)=delete;
   LweSample& operator=(const LweSample&)=delete;
#endif
};

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

#endif //LweSAMPLES_H

