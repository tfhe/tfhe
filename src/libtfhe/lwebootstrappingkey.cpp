#include <cstdlib>
#include <iostream>
#include "tfhe_core.h"
#include "lwebootstrappingkey.h"
#include "lwekeyswitch.h"
#include "tgsw.h"

const int basebit = 1;
const int kslength = 15;

using namespace std;

LweBootstrappingKey::LweBootstrappingKey(const LweParams* in_out_params, const TGswParams* bk_params) {
    this->in_out_params = in_out_params;
    this->bk_params= bk_params;
    this->accum_params= bk_params->tlwe_params;
    this->extract_params=&accum_params->extracted_lweparams;
    const int n = in_out_params->n;
    const int N = extract_params->n;

    this->bk=new_TGswSample_array(n,this->bk_params);
    this->ks=new_LweKeySwitchKey(N, kslength, basebit, in_out_params);
}

LweBootstrappingKey::~LweBootstrappingKey() {
    delete_LweKeySwitchKey(ks);
    delete_TGswSample_array(in_out_params->n,bk);
}



LweBootstrappingKeyFFT::LweBootstrappingKeyFFT(const LweParams* in_out_params, const TGswParams* bk_params) {
    this->in_out_params = in_out_params;
    this->bk_params= bk_params;
    this->accum_params= bk_params->tlwe_params;
    this->extract_params=&accum_params->extracted_lweparams;
    const int n = in_out_params->n;
    const int N = extract_params->n;

    this->bk=new_TGswSampleFFT_array(n,this->bk_params);
    this->ks=new_LweKeySwitchKey(N, kslength, basebit, in_out_params);
}

LweBootstrappingKeyFFT::~LweBootstrappingKeyFFT() {
    delete_LweKeySwitchKey(ks);
    delete_TGswSampleFFT_array(in_out_params->n,bk);
}



 






