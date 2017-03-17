#include <gtest/gtest.h>
#include <tfhe_io.h>
#include <set>

using namespace std;

const LweParams* lweparams500 = new_LweParams(500,0.1,0.3);
const set<const LweParams*> allparams = { lweparams500 };

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

