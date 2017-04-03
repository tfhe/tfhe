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
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [0,1/4]*vs[?,?], noise<1/16)
*/
EXPORT void bootsNAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) {
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,1/8) - ca - cb
	static const Torus32 NandConst=modSwitchToTorus32(1,8);
	lweNoiselessTrivial(temp_result, NandConst, in_out_params); 
	lweSubTo(temp_result, ca, in_out_params);
	lweSubTo(temp_result, cb, in_out_params);

	//if the phase is positive, the result is 1/8
	//if the phase is positive, else the result is -1/8
	tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

	delete_LweSample(temp_result);
}


/** bootstrapped Or Gate:  */ 
EXPORT void bootsOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped And Gate: result = a and b */ 
EXPORT void bootsAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped Xor Gate: result = a xor b */ 
EXPORT void bootsXOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped Xnor Gate: result = (a==b) */ 
EXPORT void bootsXNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped Not Gate: result = not(a) */ 
EXPORT void bootsNOT(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped Copy Gate: result = a */ 
EXPORT void bootsCOPY(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped Nor Gate: result = not(a or b) */ 
EXPORT void bootsNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped AndNY Gate: not(a) and b */ 
EXPORT void bootsANDNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped AndYN Gate: a and not(b) */ 
EXPORT void bootsANDYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped OrNY Gate: not(a) or b */ 
EXPORT void bootsORNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }
/** bootstrapped OrYN Gate: a or not(b) */ 
EXPORT void bootsORYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }

/** bootstrapped Mux(a,b,c) = a?b:c */ 
EXPORT void bootsMUX(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* c, const TFheGateBootstrappingCloudKeySet* bk) { die_dramatically("Not implemented"); }


#if 0


/*
 * Homomorphic AND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [0,1/2]*vs[?,?], noise<1/4)
*/
void homAND(LweSample* result, const LweSample* c1, const LweSample* c2, const LweParams* params) {
	static const Torus32 AndConst=dtot32(-1./8);
	lweNoiselessTrivial(result, AndConst, params); 
	lweAddTo(result, c1, params);
	lweAddTo(result, c2, params);
}


/*
 * Homomorphic bootstrapped AND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [0,1/4]*vs[?,?], noise<1/16)
*/
EXPORT void bootsAND(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK) {
	static const Torus32 mu0=dtot32(0.);
	static const Torus32 mu1=dtot32(1./4);

	LweSample* temp_result = new_LweSample(BK->in_out_params); 
	homAND(temp_result, c1, c2, BK->in_out_params);
	lweToLweBootstrap(result, BK, mu1, mu0, temp_result);

	delete_LweSample(temp_result);
}









/*
 * Homomorphic OR gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [0,1/2]*vs[?,?], noise<1/4)
*/
void homOR(LweSample* result, const LweSample* c1, const LweSample* c2, const LweParams* params) {
	static const Torus32 OrConst=dtot32(1./8);
	lweNoiselessTrivial(result, OrConst, params); 
	lweAddTo(result, c1, params);
	lweAddTo(result, c2, params);
}


/*
 * Homomorphic bootstrapped OR gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [0,1/4]*vs[?,?], noise<1/16)
*/
EXPORT void bootsOR(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK) {
	static const Torus32 mu0=dtot32(0.);
	static const Torus32 mu1=dtot32(1./4);

	LweSample* temp_result = new_LweSample(BK->in_out_params); 
	homOR(temp_result, c1, c2, BK->in_out_params);
	lweToLweBootstrap(result, BK, mu1, mu0, temp_result);

	delete_LweSample(temp_result);
}









/*
 * Homomorphic XOR gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [0,1/2]*vs[?,?], noise<1/4)
*/
void homXOR(LweSample* result, const LweSample* c1, const LweSample* c2, const LweParams* params) {
	lweClear(result, params); 
	lweAddTo(result, c1, params);
	lweAddTo(result, c2, params);
	lweAddTo(result, result, params); //2*(c1+c2)
}


/*
 * Homomorphic bootstrapped XOR gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [0,1/4]*vs[?,?], noise<1/16)
*/
EXPORT void bootsXOR(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK) {
	static const Torus32 mu0=dtot32(0.);
	static const Torus32 mu1=dtot32(1./4);

	LweSample* temp_result = new_LweSample(BK->in_out_params); 
	homXOR(temp_result, c1, c2, BK->in_out_params);
	lweToLweBootstrap(result, BK, mu1, mu0, temp_result);

	delete_LweSample(temp_result);
}









/*
 * Homomorphic NOT gate
 * Takes in input a LWE sample (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * No bootstrapping is needed
*/
EXPORT void homNOT(LweSample* result, const LweSample* c1, const LweParams* params) {
	static const Torus32 NotConst=dtot32(1./4);
	lweNoiselessTrivial(result, NotConst, params); 
	lweSubTo(result, c1, params);
}
#endif
