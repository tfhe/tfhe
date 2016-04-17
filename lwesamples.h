#ifndef LWESAMPLES_H
#define LWESAMPLES_H

#include "lwe.h"

struct LWESample {
	Torus32* a; //-- the n coefs of the mask
    Torus32 b;  //
   	double alpha; //-- average noise of the sample

#ifdef __cplusplus
   LWESample(const LWEParams* params);
   ~LWESample();
   LWESample(const LWESample&)=delete;
   LWESample& operator=(const LWESample&)=delete;
#endif
};

#endif //LWESAMPLES_H

