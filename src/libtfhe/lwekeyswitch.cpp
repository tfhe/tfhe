#include "lwekeyswitch.h"

LweKeySwitchKey::LweKeySwitchKey(int32_t n, int32_t t, int32_t basebit, const LweParams* out_params, LweSample* ks0_raw){
    this->basebit=basebit;
    this->out_params=out_params; 
    this->n=n;
    this->t=t;
    this->base=1<<basebit;
    this->ks0_raw = ks0_raw;
    ks1_raw = new LweSample*[n*t];
    ks = new LweSample**[n];

   
    for (int32_t p = 0; p < n*t; ++p)
	    ks1_raw[p] = ks0_raw + base*p;
	for (int32_t p = 0; p < n; ++p)
	    ks[p] = ks1_raw + t*p;
}

LweKeySwitchKey::~LweKeySwitchKey() {
    delete[] ks1_raw;
    delete[] ks;
}

