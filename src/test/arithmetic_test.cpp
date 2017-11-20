#include <gtest/gtest.h>
#include <numeric_functions.h>

using namespace std;  

namespace {

    // The fixture for testing class Arithmetic.
    class ArithmeticTest : public ::testing::Test {
	protected:
	    //ArithmeticTest() {} // You can do set-up work for each test here.
	    //virtual ~ArithmeticTest() {} // You can do clean-up work that doesn't throw exceptions here.
	    //// If the constructor and destructor are not enough for setting up
	    //// and cleaning up each test, you can define the following methods:
	    //virtual void SetUp() {}
	    //virtual void TearDown() {}
    };

    //this function return the absolute value of the (centered) fractional part of d
    //i.e. the distance between d and its nearest integer
    double absfrac(double d) {
	return abs(d-rint(d));
    }

    // modular gaussian distribution of standard deviation sigma centered on
    // the message, on the Torus32
    TEST_F(ArithmeticTest, gaussian32) {
	static const Torus32 MESSAGE1 = 123456789;
	static const Torus32 MESSAGE2 = 987654321;
	
	Torus32 reps1 = gaussian32(MESSAGE1,0);
	Torus32 reps2 = gaussian32(MESSAGE2,0);
	ASSERT_EQ(MESSAGE1,reps1);
	ASSERT_EQ(MESSAGE2,reps2);
	reps1 = gaussian32(MESSAGE1, 0.01);
	reps2 = gaussian32(MESSAGE2, 0.5);
	ASSERT_NE(MESSAGE1,reps1);
	ASSERT_NE(MESSAGE2,reps2);
	ASSERT_LE(abs(MESSAGE1-reps1),80000000);
    }

    // conversion from double to Torus32 
    // conversion from Torus32 to double
    TEST_F(ArithmeticTest, dtot32) {
	ASSERT_EQ(0,dtot32(0));
	ASSERT_EQ(1<<31,dtot32(0.5));
	ASSERT_EQ(1<<31,dtot32(-0.5));
	ASSERT_EQ(1<<30,dtot32(0.25));
	ASSERT_EQ(int32_t(0xC0000000),dtot32(-0.25));
    }

    //  Used to approximate the phase to the nearest multiple of  1/Msize 
    TEST_F(ArithmeticTest, approxPhase) {
	for (int32_t i=2; i<200; i++) {
	    Torus32 v = uniformTorus32_distrib(generator);
	    Torus32 w = approxPhase(v,i);
	    double dv = t32tod(v);
	    double dw = t32tod(w);
	    //printf("%d, %lf, %lf, %lf\n", i, dv,dw,i*dw);
	    ASSERT_LE(absfrac(dv-dw),1./(2.*i)+1e-40);
	    ASSERT_LE(absfrac(i*dw),i*1e-9);
	}
    }


    // computes rountToNearestInteger(Msize*phase)
    TEST_F(ArithmeticTest, modSwitchFromTorus32) {
	for (int32_t i=2; i<200; i++) {
	    Torus32 v = uniformTorus32_distrib(generator);
	    int32_t w = modSwitchFromTorus32(v,i);
	    double dv = t32tod(v);
	    double dw = double(w)/double(i);
	    ASSERT_LE(absfrac(dv-dw),1./(2.*i)+1e-40);
	}
    }

    // converts mu/Msize to a Torus32 for mu in [0,Msize[
    TEST_F (ArithmeticTest,modSwitchToTorus32) {
	for (int32_t i=2; i<200; i++) {
	    int32_t j = uniformTorus32_distrib(generator)%i;
	    Torus32 v = modSwitchToTorus32(j,i);
	    double dv = t32tod(v);
	    //printf("%d, %d, %lf, %lf\n", j, i, dv, double(j)/i);
	    ASSERT_LE(absfrac(dv-double(j)/double(i)),1./(2.*i)+1e-40);
	}
    }

}  // namespace

