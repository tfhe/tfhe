#include <gtest/gtest.h>
#define TFHE_TEST_ENVIRONMENT
#include "tfhe.h"
#include "fakes/lwe.h"
#include "fakes/lwe-bootstrapping-fft.h"


using namespace std;
using namespace ::testing;

namespace {

    /** generate a gate bootstrapping secret key */
    EXPORT TFheGateBootstrappingSecretKeySet* new_zero_keyset(const TFheGateBootstrappingParameterSet* params) {
	LweKey* lwe_key = 0x0; // new_LweKey(params->in_out_params);
	TGswKey* tgsw_key = 0x0; //new_TGswKey(params->tgsw_params);
	LweBootstrappingKey* bk = 0x0; //new_LweBootstrappingKey(params->in_out_params, params->tgsw_params);
	//tfhe_createLweBootstrappingKey(bk, lwe_key, tgsw_key);
	LweBootstrappingKeyFFT* bkFFT = 0x0; // new_LweBootstrappingKeyFFT(bk);
	return new TFheGateBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key);
    }




    const TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(100);
    const TFheGateBootstrappingSecretKeySet* SECRET_KEY = new_zero_keyset(params);
    const TFheGateBootstrappingCloudKeySet* CLOUD_KEY = &SECRET_KEY->cloud;
    const LweParams* LWE_PARAMS = 0x0;
    const Torus32 ENC_TRUE=modSwitchToTorus32(1,8);
    const Torus32 ENC_FALSE=modSwitchToTorus32(-1,8);


    class BootsGateTest: public ::testing::Test {
	public:

	    USE_FAKE_new_LweSample;
	    USE_FAKE_delete_LweSample;
	    USE_FAKE_lweSubTo;
	    //USE_FAKE_lweAddTo;
	    USE_FAKE_lweNoiselessTrivial;
	    //USE_FAKE_tfhe_bootstrap_FFT;

	    inline void tfhe_bootstrap_FFT(LweSample* result,  const LweBootstrappingKeyFFT* bkFFT,  Torus32 mu, const LweSample* x){
		FakeLwe* fres = fake(result);
		const FakeLwe* fx = fake(x);
		if (fx->message>=0)
		    fres->message=mu;
		else
		    fres->message=-mu;
	    }


#include "../libtfhe/boot-gates.cpp"

    };


    TEST_F(BootsGateTest, NandTest) {

	LweSample* a = fake_new_LweSample(LWE_PARAMS);
	LweSample* b = fake_new_LweSample(LWE_PARAMS);
	LweSample* c = fake_new_LweSample(LWE_PARAMS);

	FakeLwe* fa = fake(a);
	FakeLwe* fb = fake(b);
	FakeLwe* fc = fake(c);

	for (int i=0; i<4; i++) {
	    bool ba = i%2;
	    bool bb = i/2;

	    fa->message=ba?ENC_TRUE:ENC_FALSE;
	    fb->message=bb?ENC_TRUE:ENC_FALSE;
	    fa->current_variance=0.01;
	    fb->current_variance=0.01;

	    bootsNAND(c, a, b, CLOUD_KEY);
	    bool bc = !(ba && bb);  //nand

	    ASSERT_EQ(fc->message, bc?ENC_TRUE:ENC_FALSE);
	}
    }
}
