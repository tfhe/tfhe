#ifndef TFHE_TEST_ENVIRONMENT

#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwekeyswitch.h"
#include "lwe-functions.h"
#include "lwebootstrappingkey.h"
#include "tfhe.h"

using namespace std;
#else
#undef EXPORT
#define EXPORT static
#endif


//*//*****************************************
// zones on the torus -> to see
//*//*****************************************


/*
 * Homomorphic bootstrapped NAND gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsNAND(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,1/8) - ca - cb
    static const Torus32 NandConst = modSwitchToTorus32(1, 8);
    lweNoiselessTrivial(temp_result, NandConst, in_out_params);
    lweSubTo(temp_result, ca, in_out_params);
    lweSubTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped OR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsOR(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,1/8) + ca + cb
    static const Torus32 OrConst = modSwitchToTorus32(1, 8);
    lweNoiselessTrivial(temp_result, OrConst, in_out_params);
    lweAddTo(temp_result, ca, in_out_params);
    lweAddTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped AND gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsAND(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,-1/8) + ca + cb
    static const Torus32 AndConst = modSwitchToTorus32(-1, 8);
    lweNoiselessTrivial(temp_result, AndConst, in_out_params);
    lweAddTo(temp_result, ca, in_out_params);
    lweAddTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped XOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsXOR(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,1/4) + 2*(ca + cb)
    static const Torus32 XorConst = modSwitchToTorus32(1, 4);
    lweNoiselessTrivial(temp_result, XorConst, in_out_params);
    lweAddMulTo(temp_result, 2, ca, in_out_params);
    lweAddMulTo(temp_result, 2, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped XNOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsXNOR(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,-1/4) + 2*(-ca-cb)
    static const Torus32 XnorConst = modSwitchToTorus32(-1, 4);
    lweNoiselessTrivial(temp_result, XnorConst, in_out_params);
    lweSubMulTo(temp_result, 2, ca, in_out_params);
    lweSubMulTo(temp_result, 2, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped NOT gate (doesn't need to be bootstrapped)
 * Takes in input 1 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsNOT(LweSample *result, const LweSample *ca, const TFheGateBootstrappingCloudKeySet *bk) {
    const LweParams *in_out_params = bk->params->in_out_params;
    lweNegate(result, ca, in_out_params);
}


/*
 * Homomorphic bootstrapped COPY gate (doesn't need to be bootstrapped)
 * Takes in input 1 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsCOPY(LweSample *result, const LweSample *ca, const TFheGateBootstrappingCloudKeySet *bk) {
    const LweParams *in_out_params = bk->params->in_out_params;
    lweCopy(result, ca, in_out_params);
}

/*
 * Homomorphic Trivial Constant gate (doesn't need to be bootstrapped)
 * Takes a boolean value)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsCONSTANT(LweSample *result, int32_t value, const TFheGateBootstrappingCloudKeySet *bk) {
    const LweParams *in_out_params = bk->params->in_out_params;
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    lweNoiselessTrivial(result, value ? MU : -MU, in_out_params);
}


/*
 * Homomorphic bootstrapped NOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsNOR(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,-1/8) - ca - cb
    static const Torus32 NorConst = modSwitchToTorus32(-1, 8);
    lweNoiselessTrivial(temp_result, NorConst, in_out_params);
    lweSubTo(temp_result, ca, in_out_params);
    lweSubTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped AndNY Gate: not(a) and b
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsANDNY(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,-1/8) - ca + cb
    static const Torus32 AndNYConst = modSwitchToTorus32(-1, 8);
    lweNoiselessTrivial(temp_result, AndNYConst, in_out_params);
    lweSubTo(temp_result, ca, in_out_params);
    lweAddTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped AndYN Gate: a and not(b)
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsANDYN(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,-1/8) + ca - cb
    static const Torus32 AndYNConst = modSwitchToTorus32(-1, 8);
    lweNoiselessTrivial(temp_result, AndYNConst, in_out_params);
    lweAddTo(temp_result, ca, in_out_params);
    lweSubTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped OrNY Gate: not(a) or b
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsORNY(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,1/8) - ca + cb
    static const Torus32 OrNYConst = modSwitchToTorus32(1, 8);
    lweNoiselessTrivial(temp_result, OrNYConst, in_out_params);
    lweSubTo(temp_result, ca, in_out_params);
    lweAddTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped OrYN Gate: a or not(b)
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void
bootsORYN(LweSample *result, const LweSample *ca, const LweSample *cb, const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;

    LweSample *temp_result = new_LweSample(in_out_params);

    //compute: (0,1/8) + ca - cb
    static const Torus32 OrYNConst = modSwitchToTorus32(1, 8);
    lweNoiselessTrivial(temp_result, OrYNConst, in_out_params);
    lweAddTo(temp_result, ca, in_out_params);
    lweSubTo(temp_result, cb, in_out_params);

    //if the phase is positive, the result is 1/8
    //if the phase is positive, else the result is -1/8
    tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

    delete_LweSample(temp_result);
}




/*
 * Homomorphic bootstrapped Mux(a,b,c) = a?b:c = a*b + not(a)*c
 * Takes in input 3 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsMUX(LweSample *result, const LweSample *a, const LweSample *b, const LweSample *c,
                     const TFheGateBootstrappingCloudKeySet *bk) {
    static const Torus32 MU = modSwitchToTorus32(1, 8);
    const LweParams *in_out_params = bk->params->in_out_params;
    const LweParams *extracted_params = &bk->params->tgsw_params->tlwe_params->extracted_lweparams;

    LweSample *temp_result = new_LweSample(in_out_params);
    LweSample *temp_result1 = new_LweSample(extracted_params);
    LweSample *u1 = new_LweSample(extracted_params);
    LweSample *u2 = new_LweSample(extracted_params);


    //compute "AND(a,b)": (0,-1/8) + a + b
    static const Torus32 AndConst = modSwitchToTorus32(-1, 8);
    lweNoiselessTrivial(temp_result, AndConst, in_out_params);
    lweAddTo(temp_result, a, in_out_params);
    lweAddTo(temp_result, b, in_out_params);
    // Bootstrap without KeySwitch
    tfhe_bootstrap_woKS_FFT(u1, bk->bkFFT, MU, temp_result);


    //compute "AND(not(a),c)": (0,-1/8) - a + c
    lweNoiselessTrivial(temp_result, AndConst, in_out_params);
    lweSubTo(temp_result, a, in_out_params);
    lweAddTo(temp_result, c, in_out_params);
    // Bootstrap without KeySwitch
    tfhe_bootstrap_woKS_FFT(u2, bk->bkFFT, MU, temp_result);

    // Add u1=u1+u2
    static const Torus32 MuxConst = modSwitchToTorus32(1, 8);
    lweNoiselessTrivial(temp_result1, MuxConst, extracted_params);
    lweAddTo(temp_result1, u1, extracted_params);
    lweAddTo(temp_result1, u2, extracted_params);
    // Key switching
    lweKeySwitch(result, bk->bkFFT->ks, temp_result1);


    delete_LweSample(u2);
    delete_LweSample(u1);
    delete_LweSample(temp_result1);
    delete_LweSample(temp_result);
}



