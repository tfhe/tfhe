#include <gtest/gtest.h>
#include "../include/polynomials_arithmetic.h"

using namespace std;

namespace {

    class PolynomialTest: public ::testing::Test {
	protected:
	    virtual void SetUp() {
	    }
	    virtual void TearDown() {
	    }

	    const set<int> dimensions = {500,750,1024,2000};
    };

    //  TorusPolynomial = random 
    //EXPORT void torusPolynomialUniform(TorusPolynomial* result);
    TEST_F(PolynomialTest, torusPolynomialUniform) {
	static const int NB_TRIALS = 10;
        for (int N: dimensions)	{
	    TorusPolynomial* pols = new_TorusPolynomial_array(NB_TRIALS, N);
	    for (int i=0; i<NB_TRIALS; i++) {
		torusPolynomialUniform(pols+i);
	    }
	    for (int j=0; j<N; j++) {
		set<Torus32> testset;
		for (int i=0; i<NB_TRIALS; i++) {
		    testset.insert(pols[i].coefsT[j]);
		}
		ASSERT_GE(testset.size(),0.9*NB_TRIALS);
	    }
	    delete_TorusPolynomial_array(NB_TRIALS, pols);
	}
    }


    //  TorusPolynomial = 0 
    //EXPORT void torusPolynomialClear(TorusPolynomial* result);
    TEST_F(PolynomialTest, torusPolynomialClear) {
        for (int N: dimensions)	{
	    TorusPolynomial* pol = new_TorusPolynomial(N);
	    torusPolynomialUniform(pol);
	    torusPolynomialClear(pol);
	    for (int j=0; j<N; j++) {
		ASSERT_EQ(0,pol->coefsT[j]);
	    }
	    delete_TorusPolynomial(pol);
	}
    }


    //  TorusPolynomial = TorusPolynomial 
    //EXPORT void torusPolynomialCopy(TorusPolynomial* result, const TorusPolynomial*  sample);
    TEST_F(PolynomialTest, torusPolynomialCopy) {
        for (int N: dimensions)	{
	    TorusPolynomial* pol = new_TorusPolynomial(N);
	    TorusPolynomial* polc = new_TorusPolynomial(N);
	    torusPolynomialUniform(pol);
	    torusPolynomialUniform(polc);
	    Torus32 pol0 = pol->coefsT[0];
	    Torus32 pol1 = pol->coefsT[1];
	    torusPolynomialCopy(polc, pol);
	    //check that the copy is in the right direction
	    ASSERT_EQ(pol0,polc->coefsT[0]);
	    ASSERT_EQ(pol1,polc->coefsT[1]);
	    //check equality
	    for (int j=0; j<N; j++) {
		ASSERT_EQ(pol->coefsT[j],polc->coefsT[j]);
	    }
	    delete_TorusPolynomial(pol);
	    delete_TorusPolynomial(polc);
	}
    }


    //  TorusPolynomial + TorusPolynomial 
    //EXPORT void torusPolynomialAdd(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAdd) {
        for (int N: dimensions)	{
	    TorusPolynomial* pols = new_TorusPolynomial_array(5,N);
	    TorusPolynomial* pola = pols+0;
	    TorusPolynomial* polacopy = pols+1;
	    TorusPolynomial* polb = pols+2;
	    TorusPolynomial* polbcopy = pols+3;
	    TorusPolynomial* polc = pols+4;
	    torusPolynomialUniform(pola);
	    torusPolynomialUniform(polb);
	    torusPolynomialCopy(polacopy, pola);
	    torusPolynomialCopy(polbcopy, polb);
	    torusPolynomialAdd(polc,pola,polb);
	    //check equality
	    for (int j=0; j<N; j++) {
		ASSERT_EQ(polacopy->coefsT[j],pola->coefsT[j]);
		ASSERT_EQ(polbcopy->coefsT[j],polb->coefsT[j]);
		ASSERT_EQ(polc->coefsT[j],pola->coefsT[j]+polb->coefsT[j]);
	    }
	    delete_TorusPolynomial_array(5,pols);
	}
    }

    //  TorusPolynomial += TorusPolynomial 
    //EXPORT void torusPolynomialAddTo(TorusPolynomial* result, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAddTo) {
        for (int N: dimensions)	{
	    TorusPolynomial* pols = new_TorusPolynomial_array(4,N);
	    TorusPolynomial* pola = pols+0;
	    TorusPolynomial* polacopy = pols+1;
	    TorusPolynomial* polb = pols+2;
	    TorusPolynomial* polbcopy = pols+3;
	    torusPolynomialUniform(pola);
	    torusPolynomialUniform(polb);
	    torusPolynomialCopy(polacopy, pola);
	    torusPolynomialCopy(polbcopy, polb);
	    torusPolynomialAddTo(pola,polb);
	    //check equality
	    for (int j=0; j<N; j++) {
		ASSERT_EQ(polbcopy->coefsT[j],polb->coefsT[j]);
		ASSERT_EQ(pola->coefsT[j],polacopy->coefsT[j]+polbcopy->coefsT[j]);
	    }
	    delete_TorusPolynomial_array(4,pols);
	}
    }

    //  TorusPolynomial - TorusPolynomial 
    //EXPORT void torusPolynomialSub(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSub) {
        for (int N: dimensions)	{
	    TorusPolynomial* pols = new_TorusPolynomial_array(5,N);
	    TorusPolynomial* pola = pols+0;
	    TorusPolynomial* polacopy = pols+1;
	    TorusPolynomial* polb = pols+2;
	    TorusPolynomial* polbcopy = pols+3;
	    TorusPolynomial* polc = pols+4;
	    torusPolynomialUniform(pola);
	    torusPolynomialUniform(polb);
	    torusPolynomialCopy(polacopy, pola);
	    torusPolynomialCopy(polbcopy, polb);
	    torusPolynomialSub(polc,pola,polb);
	    //check equality
	    for (int j=0; j<N; j++) {
		ASSERT_EQ(polacopy->coefsT[j],pola->coefsT[j]);
		ASSERT_EQ(polbcopy->coefsT[j],polb->coefsT[j]);
		ASSERT_EQ(polc->coefsT[j],pola->coefsT[j]-polb->coefsT[j]);
	    }
	    delete_TorusPolynomial_array(5,pols);
	}
    }

    //  TorusPolynomial -= TorusPolynomial 
    //EXPORT void torusPolynomialSubTo(TorusPolynomial* result, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSubTo) {
        for (int N: dimensions)	{
	    TorusPolynomial* pols = new_TorusPolynomial_array(4,N);
	    TorusPolynomial* pola = pols+0;
	    TorusPolynomial* polacopy = pols+1;
	    TorusPolynomial* polb = pols+2;
	    TorusPolynomial* polbcopy = pols+3;
	    torusPolynomialUniform(pola);
	    torusPolynomialUniform(polb);
	    torusPolynomialCopy(polacopy, pola);
	    torusPolynomialCopy(polbcopy, polb);
	    torusPolynomialSubTo(pola,polb);
	    //check equality
	    for (int j=0; j<N; j++) {
		ASSERT_EQ(polbcopy->coefsT[j],polb->coefsT[j]);
		ASSERT_EQ(pola->coefsT[j],polacopy->coefsT[j]-polbcopy->coefsT[j]);
	    }
	    delete_TorusPolynomial_array(4,pols);
	}
    }




#if 0


    //  TorusPolynomial + p*TorusPolynomial 
    EXPORT void torusPolynomialAddMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2);

    //  TorusPolynomial += p*TorusPolynomial 
    EXPORT void torusPolynomialAddMulZTo(TorusPolynomial* result, const int p, const TorusPolynomial* poly2);

    //  TorusPolynomial - p*TorusPolynomial 
    EXPORT void torusPolynomialSubMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, const int p, const TorusPolynomial* poly2);

    //  TorusPolynomial -= p*TorusPolynomial 
    EXPORT void torusPolynomialSubMulZTo(TorusPolynomial* result, int p, const TorusPolynomial* poly2);

    //  TorusPolynomial = X^a * TorusPolynomial 
    EXPORT void torusPolynomialMulByXai(TorusPolynomial* result, int a, const TorusPolynomial* bk);

    //  Norme Euclidienne d'un IntPolynomial 
    EXPORT double intPolynomialNormSq2(const IntPolynomial* poly);

    // multiplication Karatsuba 
    EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

    // multiplication naive  
    EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);



    //
    // This is the naive external multiplication of an integer polynomial
    // with a torus polynomial. (this function should yield exactly the same
    // result as the karatsuba or fft version, but should be slower) 
    //
    EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);



    //
    // This function multiplies 2 polynomials (an integer poly and a torus poly)
    // by using Karatsuba
    //
    EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
    EXPORT void torusPolynomialAddMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
    EXPORT void torusPolynomialSubMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

    //#define torusPolynomialMulR torusPolynomialMultKaratsuba
    //#define torusPolynomialAddMulR torusPolynomialAddMulRKaratsuba
    //#define torusPolynomialSubMulR torusPolynomialSubMulRKaratsuba

#define torusPolynomialMulR torusPolynomialMultFFT
#define torusPolynomialAddMulR torusPolynomialAddMulRFFT
#define torusPolynomialSubMulR torusPolynomialSubMulRFFT


#endif //POLYNOMIALS_ARITHMETIC_H


}
