#include <gtest/gtest.h>
#include <tfhe_io.h>
#include <set>
#include "polynomials_arithmetic.h"

using namespace std;

namespace {

    const LweParams* lweparams500 = new_LweParams(500,0.1,0.3);
    const set<const LweParams*> allparams = { lweparams500 };

    const TLweParams* tlweparams1024_1 = new_TLweParams(1024,1,0.1,0.3);
    const set<const TLweParams*> allparams_tlwe = { tlweparams1024_1 };

    const TGswParams* tgswparams1024_1 = new_TGswParams(3,15,tlweparams1024_1);
    const set<const TGswParams*> allparams_tgsw = { tgswparams1024_1 };

    //generate a random lwekey
    LweKey* new_random_lwe_key(const LweParams* params) {
	const int n = params->n;
	LweKey* key = new_LweKey(params);
	for (int i=0; i<n; i++) key->key[i]=rand()%2;
	return key;
    }

    //generate a random tlwekey
    TLweKey* new_random_tlwe_key(const TLweParams* params) {
	const int N = params->N;
	const int k = params->k;
	TLweKey* key = new_TLweKey(params);
	for (int i=0; i<k; i++) 
	    for (int j=0; j<N; j++)
		key->key[i].coefs[j]=rand()%2;
	return key;
    }

    //generate a random tgswkey
    TGswKey* new_random_tgsw_key(const TGswParams* params) {
	const int N = params->tlwe_params->N;
	const int k = params->tlwe_params->k;
	TGswKey* key = new_TGswKey(params);
	for (int i=0; i<k; i++) 
	    for (int j=0; j<N; j++)
		key->key[i].coefs[j]=rand()%2;
	return key;
    }

    const LweKey* lwekey500 = new_random_lwe_key(lweparams500);
    const set<const LweKey*> allkey = { lwekey500 };

    const TLweKey* tlwekey1024_1 = new_random_tlwe_key(tlweparams1024_1);
    const set<const TLweKey*> allkey_tlwe = { tlwekey1024_1 };

    const TGswKey* tgswkey1024_1 = new_random_tgsw_key(tgswparams1024_1);
    const set<const TGswKey*> allkey_tgsw = { tgswkey1024_1 };

    //equality test for parameters
    void assert_equals(const LweParams* a, const LweParams* b) {
	ASSERT_EQ(a->n, b->n);	
	ASSERT_DOUBLE_EQ(a->alpha_min, b->alpha_min);	
	ASSERT_DOUBLE_EQ(a->alpha_max, b->alpha_max);
    }

    //equality test for parameters
    void assert_equals(const TLweParams* a, const TLweParams* b) {
	ASSERT_EQ(a->N, b->N);  
	ASSERT_EQ(a->k, b->k);  
	ASSERT_DOUBLE_EQ(a->alpha_min, b->alpha_min);   
	ASSERT_DOUBLE_EQ(a->alpha_max, b->alpha_max);
    }

    //equality test for parameters
    void assert_equals(const TGswParams* a, const TGswParams* b) {
	ASSERT_EQ(a->l, b->l);  
	ASSERT_EQ(a->Bgbit, b->Bgbit);  
	assert_equals(a->tlwe_params, b->tlwe_params);
    }	

    //equality test for keys
    void assert_equals(const LweKey* a, const LweKey* b) {
	assert_equals(a->params,b->params);
	const int n = a->params->n;
	for (int i=0; i<n; i++)
	    ASSERT_EQ(a->key[i], b->key[i]);	
    }

    //equality test for keys
    void assert_equals(const TLweKey* a, const TLweKey* b) {
	assert_equals(a->params,b->params);
	const int N = a->params->N;
	const int k = a->params->k;
	for (int i=0; i<k; i++)
	    for (int j=0; j<N; j++)
		ASSERT_EQ(a->key[i].coefs[j], b->key[i].coefs[j]);	
    }

    //equality test for keys
    void assert_equals(const TGswKey* a, const TGswKey* b) {
	assert_equals(a->params,b->params);
	assert_equals(&a->tlwe_key,&b->tlwe_key);
    }

    //generate a random sample
    void lweSampleUniform(LweSample* a, const LweParams* params) {
	const int n = params->n;
	for (int i=0; i<n; i++) a->a[i]=rand();
	a->b=rand();
	a->current_variance=rand()/double(RAND_MAX);
    }

    //generate a random sample
    void tlweSampleUniform(TLweSample* a, const TLweParams* params) {
	const int k = params->k;
	for (int i=0; i<=k; i++) 
	    torusPolynomialUniform(a->a+i);
	a->current_variance=rand()/double(RAND_MAX);
    }

    //generate a random sample
    void tgswSampleUniform(TGswSample* a, const TGswParams* params) {
	const int kpl = params->kpl;
	const TLweParams* tlwe_params = params->tlwe_params;
	for (int i=0; i<kpl; i++) 
	    tlweSampleUniform(a->all_sample+i,tlwe_params);
    }

    //equality test for samples
    void assert_equals(const LweSample* a, const LweSample* b, const LweParams* params) {
	const int n = params->n;
	for (int i=0; i<n; i++) ASSERT_EQ(a->a[i], b->a[i]);
	ASSERT_EQ(a->b,b->b);
	ASSERT_DOUBLE_EQ(a->current_variance,b->current_variance);	
    }

    //equality test for samples
    void assert_equals(const TLweSample* a, const TLweSample* b, const TLweParams* params) {
	const int k = params->k;
	for (int i=0; i<=k; i++) 
	    ASSERT_EQ(torusPolynomialNormInftyDist(a->a+i, b->a+i),0);
	ASSERT_DOUBLE_EQ(a->current_variance,b->current_variance);	
    }

    //equality test for samples
    void assert_equals(const TGswSample* a, const TGswSample* b, const TGswParams* params) {
	const int kpl = params->kpl;
	const TLweParams* tlwe_params = params->tlwe_params;
	for (int i=0; i<kpl; i++) 
	    assert_equals(a->all_sample+i,b->all_sample+i,tlwe_params);
    }


    TEST(IOTest, LweParamsIO) {
	for (const LweParams* params: allparams) {
	    ostringstream oss;
	    export_lweParams_toFile(stdout, params);
	    export_lweParams_toStream(oss, params);
	    string result = oss.str();
	    istringstream iss(result);
	    LweParams* blah = new_lweParams_fromStream(iss);
	    assert_equals(params, blah);
	    delete_LweParams(blah);
	}	
    }



    TEST(IOTest, TLweParamsIO) {
	for (const TLweParams* params: allparams_tlwe) {
	    ostringstream oss;
	    export_tLweParams_toFile(stdout, params);
	    export_tLweParams_toStream(oss, params);
	    string result = oss.str();
	    istringstream iss(result);
	    TLweParams* blah = new_tLweParams_fromStream(iss);
	    assert_equals(params, blah);
	    delete_TLweParams(blah);
	}   
    }



    TEST(IOTest, TGswParamsIO) {
	for (const TGswParams* params: allparams_tgsw) {
	    ostringstream oss;
	    export_tGswParams_toFile(stdout, params);
	    export_tGswParams_toStream(oss, params);
	    string result = oss.str();
	    istringstream iss(result);
	    TGswParams* blah = new_tGswParams_fromStream(iss);
	    assert_equals(params, blah);
	    delete_TGswParams(blah);
	}   
    }


    TEST(IOTest, LweKeyIO) {
	for (const LweKey* key: allkey) {
	    ostringstream oss;
	    //export_lweKey_toFile(stdout, key);
	    export_lweKey_toStream(oss, key);
	    string result = oss.str();
	    istringstream iss(result);
	    LweKey* blah = new_lweKey_fromStream(iss);
	    assert_equals(key, blah);
	    delete_LweKey(blah);
	}	
    }



    TEST(IOTest, TLweKeyIO) {
	for (const TLweKey* key: allkey_tlwe) {
	    ostringstream oss;
	    //export_tLweKey_toFile(stdout, key);
	    export_tlweKey_toStream(oss, key);
	    string result = oss.str();
	    istringstream iss(result);
	    TLweKey* blah = new_tlweKey_fromStream(iss);
	    assert_equals(key, blah);
	    delete_TLweKey(blah);
	}   
    }



    TEST(IOTest, TGswKeyIO) {
	for (const TGswKey* key: allkey_tgsw) {
	    ostringstream oss;
	    //export_tGswKey_toFile(stdout, key);
	    export_tgswKey_toStream(oss, key);
	    string result = oss.str();
	    istringstream iss(result);
	    TGswKey* blah = new_tgswKey_fromStream(iss);
	    assert_equals(key, blah);
	    delete_TGswKey(blah);
	}   
    }


    TEST(IOTest, LweSampleIO) {
	for (const LweParams* params: allparams) {
	    LweSample* sample = new_LweSample(params);
	    LweSample* blah = new_LweSample(params);
	    lweSampleUniform(sample, params);
	    lweSampleUniform(blah, params);
	    ostringstream oss;
	    //export_lweSample_toFile(stdout, sample);
	    export_lweSample_toStream(oss, sample, params);
	    string result = oss.str();
	    istringstream iss(result);
	    import_lweSample_fromStream(iss, blah, params);
	    assert_equals(sample, blah, params);
	    delete_LweSample(blah);
	    delete_LweSample(sample);
	}	
    }


    TEST(IOTest, TLweSampleIO) {
	for (const TLweParams* params: allparams_tlwe) {
	    TLweSample* sample = new_TLweSample(params);
	    TLweSample* blah = new_TLweSample(params);
	    tlweSampleUniform(sample, params);
	    tlweSampleUniform(blah, params);
	    ostringstream oss;
	    //export_lweSample_toFile(stdout, sample);
	    export_tlweSample_toStream(oss, sample, params);
	    string result = oss.str();
	    istringstream iss(result);
	    import_tlweSample_fromStream(iss, blah, params);
	    assert_equals(sample, blah, params);
	    delete_TLweSample(blah);
	    delete_TLweSample(sample);
	}	
    }


    TEST(IOTest, TGswSampleIO) {
	for (const TGswParams* params: allparams_tgsw) {
	    TGswSample* sample = new_TGswSample(params);
	    TGswSample* blah = new_TGswSample(params);
	    tgswSampleUniform(sample, params);
	    tgswSampleUniform(blah, params);
	    ostringstream oss;
	    //export_gswSample_toFile(stdout, sample);
	    export_tgswSample_toStream(oss, sample, params);
	    string result = oss.str();
	    istringstream iss(result);
	    import_tgswSample_fromStream(iss, blah, params);
	    assert_equals(sample, blah, params);
	    delete_TGswSample(blah);
	    delete_TGswSample(sample);
	}	
    }


}
