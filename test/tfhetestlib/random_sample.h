#ifndef TFHE_TEST_RANDOM_SAMPLE_H
#define TFHE_TEST_RANDOM_SAMPLE_H

#include <core/arithmetic/big_real.h>
#include <core/arithmetic/big_int.h>

namespace tfhe_test {
    /**
     * @brief Sample randomly big real limbs
     */
    void randomBigReal(BigReal *res, const RealParams<BigTorus> *params);

    /**
     * @brief Sample randomly big torus
     */
    void randomBigTorus(BigTorus *res, const ZModuleParams<BigTorus> *params);

    /**
     * @brief Sample randomly big int
     */
    void randomBigInt(BigInt *res, const ZModuleParams<BigTorus> *params);

};

#endif //TFHE_TEST_RANDOM_SAMPLE_H
