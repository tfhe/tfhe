#include "lwekeyswitch.h"


LweKeySwitchKey::LweKeySwitchKey(int n, int t, int basebit, const LweParams* out_params){
    this->basebit=basebit;
    this->out_params=out_params; 
    this->n=n;
    this->t=t;
    this->base=1<<basebit;
    ks0_raw = new_LweSample_array(n*t*base,out_params);
    ks1_raw = new LweSample*[n*t];
    ks = new LweSample**[n];

   
    for (int p = 0; p < n*t; ++p)
	    ks1_raw[p] = ks0_raw + base*p;
	for (int p = 0; p < n; ++p)
	    ks[p] = ks1_raw + t*p;
}

LweKeySwitchKey::~LweKeySwitchKey() {
	delete_LweSample_array(n*t*base,ks0_raw);
    delete[] ks1_raw;
    delete[] ks;
}

