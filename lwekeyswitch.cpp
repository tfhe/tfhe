#include "lwekeyswitch.h"


LWEKeySwitchKey::LWEKeySwitchKey(int basebit, LWEParams* in_params, LWEParams* out_params){
    this->basebit=basebit;
 this->base=1<<basebit;   
    this->in_params=in_params; 
    this->out_params=out_params; 
    int n=in_params->n;
    int l=(32+basebit-1)/basebit; // \ell=partie entiere superieure 32/basebit 
//garantit qu'on a au moins 32 bits de precision
    ks0_raw = new_LWESample_array(n*l*base,out_params);
    ks1_raw = new LWESample*[n*l];
    ks = new LWESample**[n];

   
    for (int p = 0; p < n*l; ++p)
	    ks1_raw[p] = ks0_raw + base*p;
	 for (int p = 0; p < n; ++p)
	    ks[p] = ks1_raw + l*p;
}

LWEKeySwitchKey::~LWEKeySwitchKey() {
	delete_LWESample_array(in_params->n*l*base,ks0_raw);
    delete[] ks1_raw;
    delete[] ks2_raw;
    delete[] ks;
}

