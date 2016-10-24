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


//*//*****************************************
// zones on the torus -> to see
//*//*****************************************


/*
 * Homomorphic NAND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [0,1/2]*vs[?,?], noise<1/4)
*/
void homNAND(LweSample* result, const LweSample* c1, const LweSample* c2, const LweParams* params) {
	static const Torus32 NandConst=dtot32(5./8);
	lweNoiselessTrivial(result, NandConst, params); 
	lweSubTo(result, c1, params);
	lweSubTo(result, c2, params);
}


/*
 * Homomorphic bootstrapped NAND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [0,1/4]*vs[?,?], noise<1/16)
*/
EXPORT void bootsNAND(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK) {
	static const Torus32 mu0=dtot32(0.);
	static const Torus32 mu1=dtot32(1./4);

	LweSample* temp_result = new_LweSample(BK->in_out_params); 
	homNAND(temp_result, c1, c2, BK->in_out_params);
	lweToLweBootstrap(result, BK, mu1, mu0, temp_result);

	delete_LweSample(temp_result);
}










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
