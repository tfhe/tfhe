/* 
 * tlwe_fft_test.cpp
 * Tests the fft functions defined in /src/include/tlwe_functions.h
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
#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    /* Tolerance factor for the equality between two TorusPolynomial */
    const double toler = 1e-8; 

    class TLweFFTTest: public ::testing::Test {
	public:
    };



    /* ***************************************************************
     *************************** TESTS ********************************
     *************************************************************** */ 



    // Computes the inverse FFT of the coefficients of the TLWE sample
    //EXPORT void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params)
    TEST_F(TLweFFTTest, tLweToFFTConvert) {
	//TODO: moking or direct test?
    }


    // Computes the FFT of the coefficients of the TLWEfft sample
    //EXPORT void fake_tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params);
    TEST_F(TLweFFTTest, tLweFromFFTConvert) {
	//TODO: moking or direct test?
    }


    //Arithmetic operations on TLwe samples
    /** result = (0,0) */
    //EXPORT void fake_tLweFFTClear(TLweSampleFFT* result, const TLweParams* params);
    TEST_F(TLweFFTTest, tLweFFTClear) {
	//TODO: direct test?
    }


    // result = result + p*sample
    //EXPORT void fake_tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params);
    TEST_F(TLweFFTTest, tLweAddMulRTo) {
	//TODO: moking or direct test?
    }


}//namespace

