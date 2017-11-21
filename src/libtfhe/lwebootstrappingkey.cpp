#include <cstdlib>
#include <iostream>
#include "tfhe_core.h"
#include "lwebootstrappingkey.h"
#include "lwekeyswitch.h"
#include "lwe-functions.h"
#include "tgsw.h"
#include "tgsw_functions.h"


using namespace std;

LweBootstrappingKey::LweBootstrappingKey(const LweParams* in_out_params, 
    const TGswParams* bk_params,
    const TLweParams* accum_params,
    const LweParams* extract_params,
    TGswSample* bk,
    LweKeySwitchKey* ks): in_out_params(in_out_params), 
    bk_params(bk_params),
    accum_params(accum_params),
    extract_params(extract_params),
    bk(bk), ks(ks) {}

LweBootstrappingKey::~LweBootstrappingKey() {}




/*
LweBootstrappingKeyFFT::LweBootstrappingKeyFFT(const LweParams* in_out_params, const TGswParams* bk_params) {
    this->in_out_params = in_out_params;
    this->bk_params= bk_params;
    this->accum_params= bk_params->tlwe_params;
    this->extract_params=&accum_params->extracted_lweparams;
    const int32_t n = in_out_params->n;
    const int32_t N = extract_params->n;

    this->bk=new_TGswSampleFFT_array(n,this->bk_params);
    this->ks=new_LweKeySwitchKey(N, kslength, basebit, in_out_params);
}

LweBootstrappingKeyFFT::~LweBootstrappingKeyFFT() {
    delete_LweKeySwitchKey(ks);
    delete_TGswSampleFFT_array(in_out_params->n,bk);
}
*/






/*
 * LweBootstrappingKey is converted to a BootstrappingKeyFFT
 */
LweBootstrappingKeyFFT::LweBootstrappingKeyFFT(const LweParams* in_out_params, 
    const TGswParams* bk_params,
    const TLweParams* accum_params,
    const LweParams* extract_params, 
    const TGswSampleFFT* bkFFT,
    const LweKeySwitchKey* ks): in_out_params(in_out_params), 
    bk_params(bk_params), 
    accum_params(accum_params), 
    extract_params(extract_params),
    bkFFT(bkFFT), ks(ks) {}


LweBootstrappingKeyFFT::~LweBootstrappingKeyFFT() {}
 






