/* 
 * tgsw_fft_test.cpp
 * Tests the functions defined in /src/include/tgsw_functions.h
 * A set of parameters ad keys is defined in the beginning (N=512,1024,2048 and k=1,2)
 */


#include <gtest/gtest.h>
#include <tlwe.h>
#include <tlwe_functions.h>
#include <tgsw_functions.h>
#include <numeric_functions.h>
#include <polynomials_arithmetic.h>
#include <lagrangehalfc_arithmetic.h>
#include "fakes/tlwe.h"
#include "fakes/tlwe-fft.h"
#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    /* Tolerance factor for the equality between two TorusPolynomial */
    const double toler = 1e-8; 

    class TGswFFTTest: public ::testing::Test {
	public:
    };



    /* ***************************************************************
     *************************** TESTS ********************************
     *************************************************************** */ 

    //EXPORT void tGswToFFTConvert(TGswSampleFFT* result, const TGswSample* source, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswToFFTConvert) {
	//TODO
    }
    //EXPORT void tGswFromFFTConvert(TGswSample* result, const TGswSampleFFT* source, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFromFFTConvert) {
	//TODO
    }
    //EXPORT void tGswFFTAddH(TGswSampleFFT* result, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTAddH) {
	//TODO
    }
    //EXPORT void tGswFFTClear(TGswSampleFFT* result, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTClear) {
	//TODO
    }
    //EXPORT void tGswFFTExternMulToTLwe(TLweSample* accum, TGswSampleFFT* gsw, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTExternMulToTLwe) {
	//TODO
    }
    //EXPORT void tGswFFTMulByXaiMinusOne(TGswSampleFFT* result, const int ai, const TGswSampleFFT* bki, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTMulByXaiMinusOne) {
	//TODO
    }

}//namespace

