#include <gtest/gtest.h>
#include <tlwe_functions.h>
#include <numeric_functions.h>

using namespace std;

namespace {
	// N=512,1024,2048 et k=1,2
	// creer un set des parametres avec tous les params à l'interieur
	// utiliser le set dans les tests
	/*
	const TLweParams* params512_1 = new_TLweParams(500,1,0.,1.);
	const TLweParams* params512_2 = new_TLweParams(500,1,0.,1.);
	params750 = new_TLweParams(750,1,0.,1.);
	params1024 = new_TLweParams(1024,1,0.,1.);
	key500 = new_TLweKey(params500);
	key750 = new_TLweKey(params750);
	key1024 = new_TLweKey(params1024);

	
    TLweParams* params750;
    TLweParams* params1024;
    TLweKey* key500;
    TLweKey* key750;
    TLweKey* key1024;
    */

    class TLweTest: public ::testing::Test {
    };

    // | frac(x) |
    double absfrac(double d) {return abs(d-rint(d));}

    // fills a TLweSample with random Torus32
	    void fillRandom(TLweSample* result, const TLweParams* params) {
		const int N = params->N;
		const int k = params->k;
		for (int i = 0; i <= k; ++i)
			for (int j = 0; j < N; ++j) 
				result->a[i].coefsT[j]=uniformTorus32_distrib(generator);
		result->current_variance=0.2;
    }

    // copy a TLweSample
    void copySample(TLweSample* result, const TLweSample* sample, const TLweParams* params) {
		const int N = params->N;
		const int k = params->k;
		
		for (int i = 0; i <= k; ++i) 
	        for (int j = 0; j < N; ++j)
	            result->a[i].coefsT[j] = sample->a[i].coefsT[j];
		result->current_variance=sample->current_variance;
    }



    // This function generates a random TLwe key for the given parameters.
    // The TLwe key for the result must be allocated and initialized
    // (this means that the parameters are already in the result)
    TEST_F(TLweTest, tLweKeyGen) {
		vector<TLweParams*> all_params = {params500, params750, params1024};
		for (TLweParams* params: all_params) {
		    TLweKey* key = new_TLweKey(params);
		    tLweKeyGen(key);
		    ASSERT_EQ(params,key->params);
			int N = key->params->N;
			int k = key->params->k;
			int* s = key->key;
		    //verify that the key is binary and kind-of random
		    int count = 0;
		    for (int i = 0; i < k; ++i) {
		    	for (int j = 0; j < N; ++j) {
					ASSERT_TRUE(s[i].coefs[j]==0 || s[i].coefs[j]==1);
					count += s[i].coefs[j];
		    	}
			}
			
			ASSERT_LE(count,k*N-20); // <=
		    ASSERT_GE(count,20); // >=

		    delete_TLweKey(key);
		}
    }



	/* ILA
    not tested --> à faire
	EXPORT void tLweSymEncryptT(TLweSample* result, Torus32 message, double alpha, const TLweKey* key);
	EXPORT void tLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int Msize, int N);
	EXPORT Torus32 tLweSymDecryptT(const TLweSample* sample, const TLweKey* key, int Msize);
  	*/
    // This function encrypts message by using key, with stdev alpha
    // The TLwe sample for the result must be allocated and initialized
    // (this means that the parameters are already in the result)
    TEST_F (TLweTest,tLweSymEncryptPhaseDecrypt) {
		static const int NB_SAMPLES=10;
		static const int M = 8;
		static const double alpha = 1./(10.*M);
		vector<TLweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TLweParams* params = key->params;
		    TLweSample* samples = new_TLweSample_array(NB_SAMPLES,params);
		    TorusPolynomial* message = new_TorusPolynomial(N);
		    TorusPolynomial* phase = new_TorusPolynomial(N);
		    TorusPolynomial* decrypt = new_TorusPolynomial(N);
		    //verify correctness of the decryption
		    for (int trial=0; trial<NB_SAMPLES; trial++) {
		    	for (int j = 0; j < N; ++j)
		    		message.coefsT[j] = modSwitchToTorus32(trial,M);
				
				tLweSymEncrypt(&samples[trial],message,alpha,key);
				tLwePhase(phase,&samples[trial],key);
				tLweSymDecrypt(decrypt,&samples[trial],key,M);

				// ILA???
				double dmessage = t32tod(message);
				double dphase = t32tod(phase);
				ASSERT_EQ(message,decrypt);
				ASSERT_LE(absfrac(dmessage-dphase),10.*alpha);
				ASSERT_EQ(alpha*alpha,samples[trial].current_variance);
		    }
		    //verify that samples are random enough (all coordinates different)
		    const int n = params->n;
		    for (int i=0; i<n; i++) {
			set<Torus32> testset;
			for (int trial=0; trial<NB_SAMPLES; trial++) {
			    testset.insert(samples[trial].a[i]);
			}
			ASSERT_GE(testset.size(),0.9*NB_SAMPLES);
		    }
		    // ILA???

		    delete_TorusPolynomial(decrypt);		    
		    delete_TorusPolynomial(phase);		    
		    delete_TorusPolynomial(message);		    
		    delete_TLweSample_array(NB_SAMPLES,samples);
		}
	}









    //Arithmetic operations on TLwe samples


    // result = (0,0)
    TEST_F(TLweTest, tLweClear) {
		vector<TLweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TLweParams* params = key->params;
		    const int N = params->N;
			const int k = params->k;
			TLweSample* sample = new_TLweSample(params);
		    fillRandom(sample,params);
		    tLweClear(sample,params);
		    for (int i = 0; i <= k; ++i)
		    	for (int j = 0; j < N; ++j)
		    		ASSERT_EQ(0,sample->a[i].coefsT[j]);
		    ASSERT_EQ(0.,sample->current_variance);
		    delete_TLweSample(sample);
		}
    }



/* ILA
not tested
EXPORT void tLweCopy(TLweSample* result, const TLweSample* sample, const TLweParams* params);
*/
    // result = (0,mu)
    TEST_F(TLweTest, tLweNoiselessTrivial) {
		vector<TLweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TorusPolynomial* message = new_TorusPolynomial(N);
			for (int j = 0; j < N; ++j)
				message.coefsT[j] = uniformTorus32_distrib(generator);
		    const LweParams* params = key->params;
		    const int N = params->N;
			const int k = params->k;
		    TLweSample* sample = new_TLweSample(params);
		    fillRandom(sample,params);
		    tLweNoiselessTrivial(sample,message,params);
		    
		    for (int i = 0; i < k; ++i)
		    	for (int j = 0; j < N; ++j)
		    		ASSERT_EQ(0,sample->a[i].coefsT[j]);
		    for (int j = 0; j < N; ++j)
		    	ASSERT_EQ(message.coefsT[j],sample->b.coefsT[j]);
		    ASSERT_EQ(0.,sample->current_variance);

		    delete_TorusPolynomial(message);
		    delete_TLweSample(sample);
		}
    }





    // result = result + sample */
    TEST_F(TLweTest, tLweAddTo) {
		vector<TLweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TLweParams* params = key->params;
		    const int N = params->N;
			const int k = params->k;
		    TLweSample* sample1 = new_TLweSample(params);
		    TLweSample* sample2 = new_TLweSample(params);
		    TLweSample* sample1copy = new_TLweSample(params);
		    fillRandom(sample1,params);
		    fillRandom(sample2,params);
		    copySample(sample1copy,sample1,params);
		    tLweAddTo(sample1,sample2,params);

		    for (int i = 0; i <= k; ++i)
		    	for (int j = 0; j < N; ++j)
		    		ASSERT_EQ(sample1copy->a[i].coefsT[j] + sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
		    ASSERT_EQ(sample1copy->current_variance + sample2->current_variance, sample1->current_variance);

		    delete_LweSample(sample1);
		    delete_LweSample(sample2);
		    delete_LweSample(sample1copy);
		}
    }





    // result = result - sample
    TEST_F(TLweTest, tLweSubTo) {
		vector<LweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TLweParams* params = key->params;
		    const int N = params->N;
			const int k = params->k;
		    TLweSample* sample1 = new_TLweSample(params);
		    TLweSample* sample2 = new_TLweSample(params);
		    TLweSample* sample1copy = new_TLweSample(params);
		    fillRandom(sample1,params);
		    fillRandom(sample2,params);
		    copySample(sample1copy,sample1,params);
		    tLweSubTo(sample1,sample2,params);

		    for (int i = 0; i <= k; ++i)
		    	for (int j = 0; j < N; ++j)
		    		ASSERT_EQ(sample1copy->a[i].coefsT[j] - sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
		    ASSERT_EQ(sample1copy->current_variance + sample2->current_variance, sample1->current_variance);

		    delete_LweSample(sample1);
		    delete_LweSample(sample2);
		    delete_LweSample(sample1copy);
		}
    }





    // result = result + p.sample
    TEST_F(TLweTest, tLweAddMulTo) {
		const int p = 3;
		vector<TLweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TLweParams* params = key->params;
		    const int N = params->N;
			const int k = params->k;
		    TLweSample* sample1 = new_TLweSample(params);
		    TLweSample* sample2 = new_TLweSample(params);
		    TLweSample* sample1copy = new_TLweSample(params);
		    fillRandom(sample1,params);
		    fillRandom(sample2,params);
		    copySample(sample1copy,sample1,params);
		    tLweAddMulTo(sample1,p,sample2,params);

		    for (int i = 0; i <= k; ++i)
		    	for (int j = 0; j < N; ++j)
		    		ASSERT_EQ(sample1copy->a[i].coefsT[j] + p*sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
		    ASSERT_EQ(sample1copy->current_variance + p*p*sample2->current_variance, sample1->current_variance);

		    delete_LweSample(sample1);
		    delete_LweSample(sample2);
		    delete_LweSample(sample1copy);
		}
    }




    // result = result - p.sample
    TEST_F(TLweTest, tLweSubMulTo) {
		const int p = 3;
		vector<TLweKey*> all_keys = {key500, key750, key1024};
		for (TLweKey* key: all_keys) {
		    const TLweParams* params = key->params;
		    const int N = params->N;
			const int k = params->k;
		    TLweSample* sample1 = new_TLweSample(params);
		    TLweSample* sample2 = new_TLweSample(params);
		    TLweSample* sample1copy = new_TLweSample(params);
		    fillRandom(sample1,params);
		    fillRandom(sample2,params);
		    copySample(sample1copy,sample1,params);
		    tLweSubMulTo(sample1,p,sample2,params);

		    for (int i = 0; i <= k; ++i)
		    	for (int j = 0; j < N; ++j)
		    		ASSERT_EQ(sample1copy->a[i].coefsT[j] - p*sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
		    ASSERT_EQ(sample1copy->current_variance + p*p*sample2->current_variance, sample1->current_variance);

		    delete_LweSample(sample1);
		    delete_LweSample(sample2);
		    delete_LweSample(sample1copy);
		}
    }







/*ILA 
still not tested 
//Arithmetic operations on TLwe samples
EXPORT void tLweMulByXaiMinusOne(TLweSample* result, int ai, const TLweSample* bk, const TLweParams* params);
EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams);

//extractions TLwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
*/



}//namespace

