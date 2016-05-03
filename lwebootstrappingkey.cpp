#include <cstdlib>
#include <iostream>
#include "lwe.h"
#include "lwebootstrapping.h"


using namespace std;

LWEBootstrappinKey::LWEBootstrappingKey(const LWEParams* params, const RingLWEparams* accpar, const RingGSWparams* bkpar,const LWEParams* extpar ) {
    this->params = params;
this->accum_params= accpar;
this->bk_params= bkpar;
this->extract_params=extpar;
this->bk=new_RingGSWSample(this->bk_params);
this->ks=new_LWEKeySwitchKey(this->bk->params->Bg,params,extparams);
}

LWEBootstrappingKey::~LWEBootstrappingKey() {
    delete[] ;
}




 






