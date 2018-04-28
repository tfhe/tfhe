#include "random_sample.h"

using namespace tfhe_backend;

void tfhe_test::randomBigReal(BigReal *res, const RealParams<BigTorus> *params) {
    for (int i = 0; i < params->nbLimbs; i++) {
        res->data[i] = ((uint64_t) rand() << 32) + rand();
    }
}

//quick and dirty function to fill a bigtorus with random
void tfhe_test::randomBigTorus(BigTorus *res, const ZModuleParams<BigTorus> *params) {
    for (int i = 0; i < params->max_nbLimbs; i++) {
        res->data[i] = ((uint64_t) rand() << 32) + rand();
    }
}

//quick and dirty function to fill a bigtorus with random
void tfhe_test::randomBigInt(BigInt *res, const ZModuleParams<BigTorus> *params) {
    setvalue(res, rand() - RAND_MAX / 2, params);
    for (int i = 1; i < 2 * params->max_nbLimbs; i++) {
        mul(res, res, rand(), params);
    }
}
