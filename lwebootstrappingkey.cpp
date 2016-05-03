#include <cstdlib>
#include <iostream>
#include "lwe.h"
#include "lwebootstrappingkey.h"
#include "ringgsw.h"


using namespace std;

LWEBootstrappingKey::LWEBootstrappingKey(const LWEParams* in_out_params, const RingGSWParams* bk_params) {
    this->in_out_params = in_out_params;
    this->bk_params= bk_params;
    this->accum_params= bk_params->ringlwe_params;
    this->extract_params=&accum_params->extracted_lweparams;
    const int n = in_out_params->n;

    this->bk=new_RingGSWSample_array(n,this->bk_params);
    const int basebit = 8;
    const int kslength = 4;
    this->ks=new_LWEKeySwitchKey(basebit, kslength, extract_params, in_out_params);
}

LWEBootstrappingKey::~LWEBootstrappingKey() {
    delete_LWEKeySwitchKey(ks);
    delete_RingGSWSample_array(in_out_params->n,bk);
}




 






