#include "lwe.h"
#include "ringlwe.h"
#include "ringgsw.h"
#include "multiplication.h"



RingGSWParams::RingGSWParams(int l, RingLWEParams* ringlwe_params):
    l(l),ringlwe_params(ringlwe_params) {}

RingGSWParams::~RingGSWParams() {}



// same key as in RingLWE
RingGSWKey::RingGSWKey(const RingGSWParams* params):
    params(params)
{
    key = new_IntPolynomial_array(params->ringlwe_params->k,params->ringlwe_params->N);   
}

RingGSWKey::~RingGSWKey() {
    delete_IntPolynomial_array(params->ringlwe_params->k,key);
}



RingGSWSample::RingGSWSample(const RingGSWParams* params): k(params->ringlwe_params->k), l(params->l) {
    sample_raw = new_RingLWESample_array((k+1)*l,params->ringlwe_params);
    sample = new RingLWESample*[(k+1)*l];

    for (int p = 0; p < (k+1)*l; ++p)
	    sample[p] = sample_raw + p;

	current_alpha = 0;
}

RingGSWSample::~RingGSWSample() {
    delete_RingLWESample_array((k+1)*l,sample_raw);
    delete[] sample;
}

