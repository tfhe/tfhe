#include "lwekeyswitch.h"


LweKeySwitchKey::LweKeySwitchKey(int basebit, int kslength, const LweParams* in_params, const LweParams* out_params){
    this->basebit=basebit;
    this->base=1<<basebit;   
    this->in_params=in_params; 
    this->out_params=out_params; 
    this->mask= base-1;
    this->n=in_params->n;
    this->l=kslength;
    ks0_raw = new_LweSample_array(n*l*base,out_params);
    ks1_raw = new LweSample*[n*l];
    ks = new LweSample**[n];

   
    for (int p = 0; p < n*l; ++p)
	    ks1_raw[p] = ks0_raw + base*p;
	for (int p = 0; p < n; ++p)
	    ks[p] = ks1_raw + l*p;
}

LweKeySwitchKey::~LweKeySwitchKey() {
	delete_LweSample_array(in_params->n*l*base,ks0_raw);
    delete[] ks1_raw;
    delete[] ks;
}

