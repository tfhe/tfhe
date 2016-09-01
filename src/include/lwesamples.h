#ifndef LweSAMPLES_H
#define LweSAMPLES_H

#include "lwe.h"

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

#endif //LweSAMPLES_H

