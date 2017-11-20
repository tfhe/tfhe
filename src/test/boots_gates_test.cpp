#include <gtest/gtest.h>

#define TFHE_TEST_ENVIRONMENT

#include "tfhe.h"
#include "fakes/lwe.h"
#include "fakes/lwe-bootstrapping-fft.h"


using namespace std;
using namespace ::testing;

namespace {

    /** generate a gate bootstrapping total fake secret key */
    EXPORT TFheGateBootstrappingSecretKeySet *new_zero_keyset(const TFheGateBootstrappingParameterSet *params) {
        LweKey *lwe_key = 0x0; // new_LweKey(params->in_out_params);
        TGswKey *tgsw_key = 0x0; //new_TGswKey(params->tgsw_params);
        LweBootstrappingKey *bk = 0x0; //new_LweBootstrappingKey(params->in_out_params, params->tgsw_params);
        //tfhe_createLweBootstrappingKey(bk, lwe_key, tgsw_key);
        //LweBootstrappingKeyFFT* bkFFT = 0x0; // new_LweBootstrappingKeyFFT(bk);
        LweKeySwitchKey *ks = (LweKeySwitchKey *) new FakeLweKeySwitchKey(1024, 15, 1);
        LweBootstrappingKeyFFT *bkFFT = new LweBootstrappingKeyFFT(0, 0, 0, 0, 0, ks);
        return new TFheGateBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key);
    }


    const TFheGateBootstrappingParameterSet *params = new_default_gate_bootstrapping_parameters(100);
    const TFheGateBootstrappingSecretKeySet *SECRET_KEY = new_zero_keyset(params);
    const TFheGateBootstrappingCloudKeySet *CLOUD_KEY = &SECRET_KEY->cloud;
    const LweParams *LWE_PARAMS = 0x0;
    const Torus32 ENC_TRUE = modSwitchToTorus32(1, 8);
    const Torus32 ENC_FALSE = modSwitchToTorus32(-1, 8);


    class BootsGateTest : public ::testing::Test {
    public:

        USE_FAKE_new_LweSample;
        USE_FAKE_delete_LweSample;
        USE_FAKE_lweSubTo;
        USE_FAKE_lweAddTo;
        USE_FAKE_lweAddMulTo;
        USE_FAKE_lweSubMulTo;
        USE_FAKE_lweCopy;
        USE_FAKE_lweNegate;
        USE_FAKE_lweNoiselessTrivial;
        USE_FAKE_lweKeySwitch;
        USE_FAKE_tfhe_bootstrap_woKS_FFT;
        USE_FAKE_tfhe_bootstrap_FFT;

#include "../libtfhe/boot-gates.cpp"


        /**
         * test template for a binary gate: test the whole truth
         * table
         */
        void binary_gate_test(
                bool (*model_gate)(bool, bool), //the ideal gate
                void (*boots_gate)(LweSample *, const LweSample *, const LweSample *,
                                   const TFheGateBootstrappingCloudKeySet *)
        ) {
            LweSample *a = fake_new_LweSample(LWE_PARAMS);
            LweSample *b = fake_new_LweSample(LWE_PARAMS);
            LweSample *c = fake_new_LweSample(LWE_PARAMS);

            FakeLwe *fa = fake(a);
            FakeLwe *fb = fake(b);
            FakeLwe *fc = fake(c);

            for (int32_t i = 0; i < 4; i++) {
                bool ba = i % 2;
                bool bb = i / 2;

                fa->message = ba ? ENC_TRUE : ENC_FALSE;
                fb->message = bb ? ENC_TRUE : ENC_FALSE;
                fa->current_variance = 0.01;
                fb->current_variance = 0.01;

                boots_gate(c, a, b, CLOUD_KEY); //bootstrapped
                bool bc = model_gate(ba, bb);  //model

                ASSERT_EQ(fc->message, bc ? ENC_TRUE : ENC_FALSE);
                ASSERT_LE(fc->current_variance, 1. / 1024.);
            }

            fake_delete_LweSample(a);
            fake_delete_LweSample(b);
            fake_delete_LweSample(c);
        }

        void unary_gate_test(
                bool (*model_gate)(bool), //the ideal gate
                void (*boots_gate)(LweSample *, const LweSample *, const TFheGateBootstrappingCloudKeySet *)
        ) {
            LweSample *a = fake_new_LweSample(LWE_PARAMS);
            LweSample *c = fake_new_LweSample(LWE_PARAMS);

            FakeLwe *fa = fake(a);
            FakeLwe *fc = fake(c);

            for (int32_t i = 0; i < 2; i++) {
                bool ba = i;

                fa->message = ba ? ENC_TRUE : ENC_FALSE;
                fa->current_variance = 0.01;

                boots_gate(c, a, CLOUD_KEY); //bootstrapped
                bool bc = model_gate(ba);  //model

                ASSERT_EQ(fc->message, bc ? ENC_TRUE : ENC_FALSE);
                ASSERT_LE(fc->current_variance, fa->current_variance);
            }

            fake_delete_LweSample(a);
            fake_delete_LweSample(c);
        }

        /**
         * test template for a binary gate: test the whole truth
         * table
         */
        void ternary_gate_test(
                bool (*model_gate)(bool, bool, bool), //the ideal gate
                void (*boots_gate)(LweSample *, const LweSample *, const LweSample *, const LweSample *,
                                   const TFheGateBootstrappingCloudKeySet *)
        ) {
            LweSample *res = fake_new_LweSample(LWE_PARAMS);
            LweSample *a = fake_new_LweSample(LWE_PARAMS);
            LweSample *b = fake_new_LweSample(LWE_PARAMS);
            LweSample *c = fake_new_LweSample(LWE_PARAMS);

            FakeLwe *fres = fake(res);
            FakeLwe *fa = fake(a);
            FakeLwe *fb = fake(b);
            FakeLwe *fc = fake(c);

            for (int32_t i = 0; i < 8; i++) {
                bool ba = (i >> 0) & 1;
                bool bb = (i >> 1) & 1;
                bool bc = (i >> 2) & 1;

                fa->message = ba ? ENC_TRUE : ENC_FALSE;
                fb->message = bb ? ENC_TRUE : ENC_FALSE;
                fc->message = bc ? ENC_TRUE : ENC_FALSE;
                fa->current_variance = 0.01;
                fb->current_variance = 0.01;
                fc->current_variance = 0.01;

                boots_gate(res, a, b, c, CLOUD_KEY); //bootstrapped
                bool bres = model_gate(ba, bb, bc);  //model

                ASSERT_EQ(fres->message, bres ? ENC_TRUE : ENC_FALSE);
                ASSERT_LE(fres->current_variance, 1. / 1024.);
            }

            fake_delete_LweSample(a);
            fake_delete_LweSample(b);
            fake_delete_LweSample(c);
            fake_delete_LweSample(res);
        }


    };

    bool bool_nand(bool a, bool b) { return !(a && b); }

    bool bool_and(bool a, bool b) { return a && b; }

    bool bool_andny(bool a, bool b) { return (!a) && b; }

    bool bool_andyn(bool a, bool b) { return a && (!b); }

    bool bool_nor(bool a, bool b) { return !(a || b); }

    bool bool_or(bool a, bool b) { return a || b; }

    bool bool_orny(bool a, bool b) { return (!a) || b; }

    bool bool_oryn(bool a, bool b) { return a || (!b); }

    bool bool_xor(bool a, bool b) { return a ^ b; }

    bool bool_xnor(bool a, bool b) { return a == b; }

    bool bool_not(bool a) { return !a; }

    bool bool_copy(bool a) { return a; }

    bool bool_mux(bool a, bool b, bool c) { return a ? b : c; }

    TEST_F(BootsGateTest, NandTest) { binary_gate_test(bool_nand, bootsNAND); }

    TEST_F(BootsGateTest, AndTest) { binary_gate_test(bool_and, bootsAND); }

    TEST_F(BootsGateTest, AndNYTest) { binary_gate_test(bool_andny, bootsANDNY); }

    TEST_F(BootsGateTest, AndYNTest) { binary_gate_test(bool_andyn, bootsANDYN); }

    TEST_F(BootsGateTest, NorTest) { binary_gate_test(bool_nor, bootsNOR); }

    TEST_F(BootsGateTest, OrTest) { binary_gate_test(bool_or, bootsOR); }

    TEST_F(BootsGateTest, OrNYTest) { binary_gate_test(bool_orny, bootsORNY); }

    TEST_F(BootsGateTest, OrYNTest) { binary_gate_test(bool_oryn, bootsORYN); }

    TEST_F(BootsGateTest, XorTest) { binary_gate_test(bool_xor, bootsXOR); }

    TEST_F(BootsGateTest, XnorTest) { binary_gate_test(bool_xnor, bootsXNOR); }

    TEST_F(BootsGateTest, NotTest) { unary_gate_test(bool_not, bootsNOT); }

    TEST_F(BootsGateTest, CopyTest) { unary_gate_test(bool_copy, bootsCOPY); }

    TEST_F(BootsGateTest, MuxTest) { ternary_gate_test(bool_mux, bootsMUX); }
}
