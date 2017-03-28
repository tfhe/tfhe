#include <gtest/gtest.h>
#include <tfhe_io.h>
#include <set>

using namespace std;

const LweParams* lweparams500 = new_LweParams(500,0.1,0.3);
const set<const LweParams*> allparams = { lweparams500 };

const TLweParams* tlweparams1024_1 = new_TLweParams(1024,1,0.1,0.3);
const set<const TLweParams*> allparams_tlwe = { tlweparams1024_1 };

const TGswParams* tgswparams1024_1 = new_TGswParams(3,15,tlweparams1024_1);
const set<const TGswParams*> allparams_tgsw = { tgswparams1024_1 };




TEST(IOTest, LweParamsIO) {
    for (const LweParams* params: allparams) {
	ostringstream oss;
	export_lweParams_toStream(cout, params);
	export_lweParams_toStream(oss, params);
	string result = oss.str();
	istringstream iss(result);
	LweParams* blah = new_lweParams_fromStream(iss);
        ASSERT_EQ(blah->n, params->n);	
        ASSERT_DOUBLE_EQ(blah->alpha_min, params->alpha_min);	
        ASSERT_DOUBLE_EQ(blah->alpha_max, params->alpha_max);
	delete_LweParams(blah);
    }	
}



TEST(IOTest, TLweParamsIO) {
    for (const TLweParams* params: allparams_tlwe) {
	ostringstream oss;

	export_tLweParams_toStream(cout, params);
	export_tLweParams_toStream(oss, params);
	string result = oss.str();
	istringstream iss(result);
	TLweParams* blah = new_tLweParams_fromStream(iss);
	ASSERT_EQ(blah->N, params->N);  
	ASSERT_EQ(blah->k, params->k);  
	ASSERT_DOUBLE_EQ(blah->alpha_min, params->alpha_min);   
	ASSERT_DOUBLE_EQ(blah->alpha_max, params->alpha_max);
	delete_TLweParams(blah);
    }   
}




TEST(IOTest, TGswParamsIO) {
    for (const TGswParams* params: allparams_tgsw) {
	ostringstream oss;

	export_tGswParams_toStream(cout, params);
	export_tGswParams_toStream(oss, params);
	string result = oss.str();
	istringstream iss(result);
	TGswParams* blah = new_tGswParams_fromStream(iss);
	ASSERT_EQ(blah->l, params->l);  
	ASSERT_EQ(blah->Bgbit, params->Bgbit);  
	ASSERT_EQ(blah->tlwe_params->N, params->tlwe_params->N);  
	ASSERT_EQ(blah->tlwe_params->k, params->tlwe_params->k); 
	ASSERT_DOUBLE_EQ(blah->tlwe_params->alpha_min, params->tlwe_params->alpha_min);   
	ASSERT_DOUBLE_EQ(blah->tlwe_params->alpha_max, params->tlwe_params->alpha_max);
	delete_TGswParams(blah);
    }   
}
