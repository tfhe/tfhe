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


/*
 * Homomorphic bootstrapped OR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,1/8) + ca + cb
	static const Torus32 OrConst=modSwitchToTorus32(1,8);
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
EXPORT void bootsAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,-1/8) + ca + cb
	static const Torus32 AndConst=modSwitchToTorus32(-1,8);
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
EXPORT void bootsXOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,1/4) + 2*(ca + cb)
	static const Torus32 XorConst=modSwitchToTorus32(1,4);
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
EXPORT void bootsXNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,-1/4) + 2*(-ca-cb)
	static const Torus32 XnorConst=modSwitchToTorus32(-1,4);
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
EXPORT void bootsNOT(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk) { 
	const LweParams* in_out_params = bk->params->in_out_params;
	const int n = in_out_params->n;

    //compute: -ca
	for (int i = 0; i < n; ++i) result->a[i] = -ca->a[i];
    result->b = -ca->b;
    result->current_variance = ca->current_variance; 
}


/*
 * Homomorphic bootstrapped COPY gate (doesn't need to be bootstrapped)
 * Takes in input 1 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsCOPY(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk) { 
	const LweParams* in_out_params = bk->params->in_out_params;
	const int n = in_out_params->n;

    //compute: result = ca
	for (int i = 0; i < n; ++i) result->a[i] = ca->a[i];
    result->b = ca->b;
    result->current_variance = ca->current_variance;
}


/*
 * Homomorphic bootstrapped NOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
EXPORT void bootsNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,-1/8) - ca - cb
	static const Torus32 NorConst=modSwitchToTorus32(-1,8);
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
EXPORT void bootsANDNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,-1/8) - ca + cb
	static const Torus32 AndNYConst=modSwitchToTorus32(-1,8);
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
EXPORT void bootsANDYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,-1/8) + ca - cb
	static const Torus32 AndYNConst=modSwitchToTorus32(-1,8);
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
EXPORT void bootsORNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,1/8) - ca + cb
	static const Torus32 OrNYConst=modSwitchToTorus32(1,8);
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
EXPORT void bootsORYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk) { 
	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result = new_LweSample(in_out_params); 

	//compute: (0,1/8) + ca - cb
	static const Torus32 OrYNConst=modSwitchToTorus32(1,8);
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
EXPORT void bootsMUX(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* c, const TFheGateBootstrappingCloudKeySet* bk) { 
	const TGswParams* bk_params = bk->bkFFT->bk_params;
    const TLweParams* accum_params = bk->bkFFT->accum_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;
    const LweParams* in_params = bk->bkFFT->in_out_params;
    const int N=accum_params->N;
    const int Nx2= 2*N;
    const int n = in_params->n;

	static const Torus32 MU = modSwitchToTorus32(1,8);
	const LweParams* in_out_params = bk->params->in_out_params;

	LweSample* temp_result1 = new_LweSample(in_out_params); 
	LweSample* temp_result2 = new_LweSample(in_out_params); 


	//compute "AND(a,b)": (0,-1/8) + a + b
	static const Torus32 AndConst=modSwitchToTorus32(-1,8);
	lweNoiselessTrivial(temp_result1, AndConst, in_out_params); 
	lweAddTo(temp_result1, a, in_out_params);
	lweAddTo(temp_result1, b, in_out_params);

	TorusPolynomial* testvect1 = new_TorusPolynomial(N);
    int* bara1 = new int[N];
    LweSample* u1 = new_LweSample(extract_params);
	// Modulus switching
    int barb1 = modSwitchFromTorus32(temp_result1->b,Nx2);
    for (int i=0; i<n; i++) {
        bara1[i]=modSwitchFromTorus32(temp_result1->a[i],Nx2);
    }
    // the initial testvec = [mu,mu,mu,...,mu]
    for (int i=0;i<N;i++) testvect1->coefsT[i]=MU;
    // Bootstrapping rotation and extraction
    tfhe_blindRotateAndExtract_FFT(u1, testvect1, bk->bkFFT->bkFFT, barb1, bara1, n, bk_params);
    

	//compute "AND(not(a),c)": (0,-1/8) - a + c
	lweNoiselessTrivial(temp_result2, AndConst, in_out_params); 
	lweSubTo(temp_result2, a, in_out_params);
	lweAddTo(temp_result2, c, in_out_params);

	TorusPolynomial* testvect2 = new_TorusPolynomial(N);
    int* bara2 = new int[N];
    LweSample* u2 = new_LweSample(extract_params);
	// Modulus switching
    int barb2 = modSwitchFromTorus32(temp_result2->b,Nx2);
    for (int i=0; i<n; i++) {
        bara2[i]=modSwitchFromTorus32(temp_result2->a[i],Nx2);
    }
    // the initial testvec = [mu,mu,mu,...,mu]
    for (int i=0;i<N;i++) testvect2->coefsT[i]=MU;
    // Bootstrapping rotation and extraction
    tfhe_blindRotateAndExtract_FFT(u2, testvect2, bk->bkFFT->bkFFT, barb2, bara2, n, bk_params);


	// Add u1=u1+u2
	lweAddTo(u1, u2, in_out_params);
	// Key switching
    lweKeySwitch(result, bk->bkFFT->ks, u1);


    delete_LweSample(u2);
    delete[] bara2;
    delete_TorusPolynomial(testvect2);
    delete_LweSample(u1);
    delete[] bara1;
    delete_TorusPolynomial(testvect1);
	delete_LweSample(temp_result2);
	delete_LweSample(temp_result1);
}






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
