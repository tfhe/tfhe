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

using namespace std;




/*
 * Homomorphic NAND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs an LWE sample (with message space [0,1/2]*vs[?,?], noise<1/4)
*/
void homNAND(LweSample* result, LweSample* c1, LweSample* c2, LweParams* params) {
	static const Torus32 NandConst=dtot32(5./8);
	lweNoiselessTrivial(result, NandConst, params);
	lweSubTo(result, c1, params);
	lweSubTo(result, c2, params);
}


/*
 * Homomorphic bootstrapped NAND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]*vs[-1/8,1/8], noise<1/16)
 * Outputs an LWE bootstrapped sample (with message space [0,1/4]*vs[?,?], noise<1/16)
*/
void homNAND(LweSample* result, LweSample* c1, LweSample* c2, LweBootstrappingKey* BK) {
	static const Torus32 mu0=dtot32(0.);
	static const Torus32 mu1=dtot32(1./4);

	LweSample* temp_result = new_LweSample(BK->LweParams); // attention
	homNAND(temp_result, c1, c2, params);
	lweToLweBootstrap(result, BK, mu1, mu0, temp_result);

	delete_LweSample(temp_result);
}
