#include <gtest/gtest.h>
#include "../include/polynomials_arithmetic.h"
#include "../include/numeric_functions.h"

using namespace std;

namespace {

    class PolynomialTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
        }

        virtual void TearDown() {
        }

        const set<int32_t> dimensions = {500, 750, 1024, 2000};
        const set<int32_t> powers_of_two_dimensions = {512, 1024, 2048};
    };

    //  TorusPolynomial = random 
    //EXPORT void torusPolynomialUniform(TorusPolynomial* result);
    TEST_F(PolynomialTest, torusPolynomialUniform) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 10;
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(NB_TRIALS, N);
            for (int32_t i = 0; i < NB_TRIALS; i++) {
                torusPolynomialUniform(pols + i);
            }
            for (int32_t j = 0; j < N; j++) {
                set<Torus32> testset;
                for (int32_t i = 0; i < NB_TRIALS; i++) {
                    testset.insert(pols[i].coefsT[j]);
                }
                ASSERT_GE(testset.size(), 0.9 * NB_TRIALS);
            }
            delete_TorusPolynomial_array(NB_TRIALS, pols);
        }
    }


    //  TorusPolynomial = 0 
    //EXPORT void torusPolynomialClear(TorusPolynomial* result);
    TEST_F(PolynomialTest, torusPolynomialClear) {
        for (int32_t N: dimensions) {
            TorusPolynomial *pol = new_TorusPolynomial(N);
            torusPolynomialUniform(pol);
            torusPolynomialClear(pol);
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(0, pol->coefsT[j]);
            }
            delete_TorusPolynomial(pol);
        }
    }


    //  TorusPolynomial = TorusPolynomial 
    //EXPORT void torusPolynomialCopy(TorusPolynomial* result, const TorusPolynomial*  sample);
    TEST_F(PolynomialTest, torusPolynomialCopy) {
        for (int32_t N: dimensions) {
            TorusPolynomial *pol = new_TorusPolynomial(N);
            TorusPolynomial *polc = new_TorusPolynomial(N);
            torusPolynomialUniform(pol);
            torusPolynomialUniform(polc);
            Torus32 pol0 = pol->coefsT[0];
            Torus32 pol1 = pol->coefsT[1];
            torusPolynomialCopy(polc, pol);
            //check that the copy is in the right direction
            ASSERT_EQ(pol0, polc->coefsT[0]);
            ASSERT_EQ(pol1, polc->coefsT[1]);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(pol->coefsT[j], polc->coefsT[j]);
            }
            delete_TorusPolynomial(pol);
            delete_TorusPolynomial(polc);
        }
    }


    //  TorusPolynomial + TorusPolynomial 
    //EXPORT void torusPolynomialAdd(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAdd) {
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(5, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            TorusPolynomial *polc = pols + 4;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialAdd(polc, pola, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polacopy->coefsT[j], pola->coefsT[j]);
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(polc->coefsT[j], pola->coefsT[j] + polb->coefsT[j]);
            }
            delete_TorusPolynomial_array(5, pols);
        }
    }

    //  TorusPolynomial += TorusPolynomial 
    //EXPORT void torusPolynomialAddTo(TorusPolynomial* result, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAddTo) {
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(4, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialAddTo(pola, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(pola->coefsT[j], polacopy->coefsT[j] + polbcopy->coefsT[j]);
            }
            delete_TorusPolynomial_array(4, pols);
        }
    }

    //  TorusPolynomial - TorusPolynomial 
    //EXPORT void torusPolynomialSub(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSub) {
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(5, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            TorusPolynomial *polc = pols + 4;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialSub(polc, pola, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polacopy->coefsT[j], pola->coefsT[j]);
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(polc->coefsT[j], pola->coefsT[j] - polb->coefsT[j]);
            }
            delete_TorusPolynomial_array(5, pols);
        }
    }

    //  TorusPolynomial -= TorusPolynomial 
    //EXPORT void torusPolynomialSubTo(TorusPolynomial* result, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSubTo) {
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(4, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialSubTo(pola, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(pola->coefsT[j], polacopy->coefsT[j] - polbcopy->coefsT[j]);
            }
            delete_TorusPolynomial_array(4, pols);
        }
    }

    //  TorusPolynomial + p*TorusPolynomial 
    //EXPORT void torusPolynomialAddMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, int32_t p, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAddMulZ) {
        //TODO: parallelization
        static const int32_t p = uniformTorus32_distrib(generator);
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(5, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            TorusPolynomial *polc = pols + 4;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialAddMulZ(polc, pola, p, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polacopy->coefsT[j], pola->coefsT[j]);
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(polc->coefsT[j], pola->coefsT[j] + p * polb->coefsT[j]);
            }
            delete_TorusPolynomial_array(5, pols);
        }
    }

    //  TorusPolynomial += p*TorusPolynomial 
    //EXPORT void torusPolynomialAddMulZTo(TorusPolynomial* result, const int32_t p, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAddMulZTo) {
        //TODO: parallelization
        static const int32_t p = uniformTorus32_distrib(generator);
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(4, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialAddMulZTo(pola, p, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(pola->coefsT[j], polacopy->coefsT[j] + p * polbcopy->coefsT[j]);
            }
            delete_TorusPolynomial_array(4, pols);
        }
    }

    //  TorusPolynomial - p*TorusPolynomial 
    //EXPORT void torusPolynomialSubMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, int32_t p, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSubMulZ) {
        //TODO: parallelization
        static const int32_t p = uniformTorus32_distrib(generator);
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(5, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            TorusPolynomial *polc = pols + 4;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialSubMulZ(polc, pola, p, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polacopy->coefsT[j], pola->coefsT[j]);
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(polc->coefsT[j], pola->coefsT[j] - p * polb->coefsT[j]);
            }
            delete_TorusPolynomial_array(5, pols);
        }
    }

    //  TorusPolynomial -= p*TorusPolynomial 
    //EXPORT void torusPolynomialSubMulZTo(TorusPolynomial* result, const int32_t p, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSubMulZTo) {
        //TODO: parallelization
        static const int32_t p = uniformTorus32_distrib(generator);
        for (int32_t N: dimensions) {
            TorusPolynomial *pols = new_TorusPolynomial_array(4, N);
            TorusPolynomial *pola = pols + 0;
            TorusPolynomial *polacopy = pols + 1;
            TorusPolynomial *polb = pols + 2;
            TorusPolynomial *polbcopy = pols + 3;
            torusPolynomialUniform(pola);
            torusPolynomialUniform(polb);
            torusPolynomialCopy(polacopy, pola);
            torusPolynomialCopy(polbcopy, polb);
            torusPolynomialSubMulZTo(pola, p, polb);
            //check equality
            for (int32_t j = 0; j < N; j++) {
                ASSERT_EQ(polbcopy->coefsT[j], polb->coefsT[j]);
                ASSERT_EQ(pola->coefsT[j], polacopy->coefsT[j] - p * polbcopy->coefsT[j]);
            }
            delete_TorusPolynomial_array(4, pols);
        }
    }

    int32_t anticyclic_get(int32_t *tab, int32_t a, int32_t N) {
        int32_t agood = ((a % (2 * N)) + (2 * N)) % (2 * N);
        if (agood < N)
            return tab[agood];
        else
            return -tab[agood - N];
    }

    void random_small_ints(int32_t *tab, int32_t bound, int32_t N) {
        for (int32_t j = 0; j < N; j++) {
            tab[j] = (uniformTorus32_distrib(generator) % bound);
        }
    }

    void int_tab_copy(int32_t *dest, const int32_t *tab, int32_t N) {
        for (int32_t j = 0; j < N; j++) {
            dest[j] = tab[j];
        }
    }

    //  TorusPolynomial = X^a * TorusPolynomial 
    //EXPORT void torusPolynomialMulByXai(TorusPolynomial* result, int32_t a, const TorusPolynomial* bk)
    TEST_F(PolynomialTest, torusPolynomialMulByXai) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 50;
        for (int32_t N: dimensions) {
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                //TODO: parallelization
                static const int32_t a = (uniformTorus32_distrib(generator) % 1000000) - 500000;
                static const int32_t ai = ((a % (2 * N)) + (2 * N)) % (2 * N);
                TorusPolynomial *pols = new_TorusPolynomial_array(3, N);
                TorusPolynomial *pola = pols + 0;
                TorusPolynomial *polacopy = pols + 1;
                TorusPolynomial *polb = pols + 2;
                torusPolynomialUniform(pola);
                torusPolynomialUniform(polb);
                torusPolynomialCopy(polacopy, pola);
                torusPolynomialMulByXai(polb, ai, pola);
                //check equality
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(polacopy->coefsT[j], pola->coefsT[j]);
                    ASSERT_EQ(polb->coefsT[j], anticyclic_get(polacopy->coefsT, j - ai, N));
                }
                delete_TorusPolynomial_array(3, pols);
            }
        }
    }

    //  intPolynomial = (X^ai-1) * intPolynomial 
    //EXPORT void intPolynomialMulByXaiMinusOne(IntPolynomial* result, int32_t a, const IntPolynomial* bk)
    TEST_F(PolynomialTest, intPolynomialMulByXaiMinusOne) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 50;
        for (int32_t N: dimensions) {
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                //TODO: parallelization
                static const int32_t a = (uniformTorus32_distrib(generator) % 1000000) - 500000;
                static const int32_t ai = ((a % (2 * N)) + (2 * N)) % (2 * N);
                IntPolynomial *pols = new_IntPolynomial_array(3, N);
                IntPolynomial *pola = pols + 0;
                IntPolynomial *polacopy = pols + 1;
                IntPolynomial *polb = pols + 2;
                //fill the polynomial with random coefs
                for (int32_t j = 0; j < N; j++) {
                    pola->coefs[j] = uniformTorus32_distrib(generator);
                    polb->coefs[j] = uniformTorus32_distrib(generator);
                }
                intPolynomialCopy(polacopy, pola);
                intPolynomialMulByXaiMinusOne(polb, ai, pola);
                //check equality
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(polacopy->coefs[j], pola->coefs[j]);
                    ASSERT_EQ(polb->coefs[j],
                              anticyclic_get(polacopy->coefs, j - ai, N) - anticyclic_get(polacopy->coefs, j, N));
                }
                delete_IntPolynomial_array(3, pols);
            }
        }
    }

    //  TorusPolynomial = (X^ai-1) * TorusPolynomial 
    //EXPORT void torusPolynomialMulByXaiMinusOne(TorusPolynomial* result, int32_t a, const TorusPolynomial* bk)
    TEST_F(PolynomialTest, torusPolynomialMulByXaiMinusOne) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 50;
        for (int32_t N: dimensions) {
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                //TODO: parallelization
                static const int32_t a = (uniformTorus32_distrib(generator) % 1000000) - 500000;
                static const int32_t ai = ((a % (2 * N)) + (2 * N)) % (2 * N);
                TorusPolynomial *pols = new_TorusPolynomial_array(3, N);
                TorusPolynomial *pola = pols + 0;
                TorusPolynomial *polacopy = pols + 1;
                TorusPolynomial *polb = pols + 2;
                //fill the polynomial with random coefs
                torusPolynomialUniform(pola);
                torusPolynomialUniform(polb);
                torusPolynomialCopy(polacopy, pola);
                torusPolynomialMulByXaiMinusOne(polb, ai, pola);
                //check equality
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(polacopy->coefsT[j], pola->coefsT[j]);
                    ASSERT_EQ(polb->coefsT[j],
                              anticyclic_get(polacopy->coefsT, j - ai, N) - anticyclic_get(polacopy->coefsT, j, N));
                }
                delete_TorusPolynomial_array(3, pols);
            }
        }
    }

    //  Norme Euclidienne d'un IntPolynomial 
    //EXPORT double intPolynomialNormSq2(const IntPolynomial* poly);
    TEST_F(PolynomialTest, intPolynomialNormSq2) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 50;
        for (int32_t N: dimensions) {
            IntPolynomial *pols = new_IntPolynomial_array(2, N);
            IntPolynomial *a = pols + 0;
            IntPolynomial *acopy = pols + 1;
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                double norm2 = 0;
                for (int32_t j = 0; j < N; j++) {
                    int32_t r = (uniformTorus32_distrib(generator) % 1000) - 500;
                    a->coefs[j] = r;
                    acopy->coefs[j] = r;
                    norm2 += r * r;
                }
                double value = intPolynomialNormSq2(a);
                ASSERT_EQ(norm2, value);
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(a->coefs[j], acopy->coefs[j]);
                }
            }
            delete_IntPolynomial_array(2, pols);
        }
    }

    // This is the naive external multiplication of an integer polynomial
    // with a torus polynomial. (this function should yield exactly the same
    // result as the karatsuba or fft version, but should be slower) 
    //EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialMultNaive) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 5;
        for (int32_t N: dimensions) {
            IntPolynomial *ipols = new_IntPolynomial_array(2, N);
            TorusPolynomial *tpols = new_TorusPolynomial_array(3, N);
            IntPolynomial *a = ipols + 0;
            IntPolynomial *acopy = ipols + 1;
            TorusPolynomial *b = tpols + 0;
            TorusPolynomial *bcopy = tpols + 1;
            TorusPolynomial *c = tpols + 2;
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                torusPolynomialUniform(b);
                torusPolynomialUniform(c);
                torusPolynomialCopy(bcopy, b);
                random_small_ints(a->coefs, 100000, N);
                int_tab_copy(acopy->coefs, a->coefs, N);
                torusPolynomialMultNaive(c, a, b);
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(acopy->coefs[j], a->coefs[j]);
                    ASSERT_EQ(bcopy->coefsT[j], b->coefsT[j]);
                    Torus32 r = 0;
                    for (int32_t k = 0; k < N; k++) {
                        r += bcopy->coefsT[k] * anticyclic_get(acopy->coefs, j - k, N);
                    }
                    ASSERT_EQ(r, c->coefsT[j]);
                }
            }
            delete_IntPolynomial_array(2, ipols);
            delete_TorusPolynomial_array(3, tpols);
        }
    }

    // This is the karatsuba external multiplication of an integer polynomial
    // with a torus polynomial. 
    // WARNING: for karatsuba, N must be a power of 2
    //EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialMultKaratsuba) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 5;
        for (int32_t N: powers_of_two_dimensions) {
            IntPolynomial *ipols = new_IntPolynomial_array(2, N);
            TorusPolynomial *tpols = new_TorusPolynomial_array(3, N);
            IntPolynomial *a = ipols + 0;
            IntPolynomial *acopy = ipols + 1;
            TorusPolynomial *b = tpols + 0;
            TorusPolynomial *bcopy = tpols + 1;
            TorusPolynomial *c = tpols + 2;
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                torusPolynomialUniform(b);
                torusPolynomialUniform(c);
                torusPolynomialCopy(bcopy, b);
                random_small_ints(a->coefs, 100000, N);
                int_tab_copy(acopy->coefs, a->coefs, N);
                torusPolynomialMultKaratsuba(c, a, b);
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(acopy->coefs[j], a->coefs[j]);
                    ASSERT_EQ(bcopy->coefsT[j], b->coefsT[j]);
                    Torus32 r = 0;
                    for (int32_t k = 0; k < N; k++) {
                        r += bcopy->coefsT[k] * anticyclic_get(acopy->coefs, j - k, N);
                    }
                    ASSERT_EQ(r, c->coefsT[j]);
                }
            }
            delete_IntPolynomial_array(2, ipols);
            delete_TorusPolynomial_array(3, tpols);
        }
    }

    // result += poly1 * poly2 (via karatsuba)
    // WARNING: N must be a power of 2 to use this function. Else, the
    // behaviour is unpredictable
    //EXPORT void torusPolynomialAddMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialAddMulRKaratsuba) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 5;
        for (int32_t N: powers_of_two_dimensions) {
            IntPolynomial *ipols = new_IntPolynomial_array(2, N);
            TorusPolynomial *tpols = new_TorusPolynomial_array(4, N);
            IntPolynomial *a = ipols + 0;
            IntPolynomial *acopy = ipols + 1;
            TorusPolynomial *b = tpols + 0;
            TorusPolynomial *bcopy = tpols + 1;
            TorusPolynomial *c = tpols + 2;
            TorusPolynomial *ccopy = tpols + 3;
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                torusPolynomialUniform(b);
                torusPolynomialUniform(c);
                torusPolynomialCopy(bcopy, b);
                torusPolynomialCopy(ccopy, c);
                random_small_ints(a->coefs, 100000, N);
                int_tab_copy(acopy->coefs, a->coefs, N);
                torusPolynomialAddMulRKaratsuba(c, a, b);
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(acopy->coefs[j], a->coefs[j]);
                    ASSERT_EQ(bcopy->coefsT[j], b->coefsT[j]);
                    Torus32 r = ccopy->coefsT[j];
                    for (int32_t k = 0; k < N; k++) {
                        r += bcopy->coefsT[k] * anticyclic_get(acopy->coefs, j - k, N);
                    }
                    ASSERT_EQ(r, c->coefsT[j]);
                }
            }
            delete_IntPolynomial_array(2, ipols);
            delete_TorusPolynomial_array(4, tpols);
        }
    }

    // result -= poly1 * poly2 (via karatsuba)
    // WARNING: N must be a power of 2 to use this function. Else, the
    // behaviour is unpredictable
    //EXPORT void torusPolynomialAddMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
    TEST_F(PolynomialTest, torusPolynomialSubMulRKaratsuba) {
        //TODO: parallelization
        static const int32_t NB_TRIALS = 5;
        for (int32_t N: powers_of_two_dimensions) {
            IntPolynomial *ipols = new_IntPolynomial_array(2, N);
            TorusPolynomial *tpols = new_TorusPolynomial_array(4, N);
            IntPolynomial *a = ipols + 0;
            IntPolynomial *acopy = ipols + 1;
            TorusPolynomial *b = tpols + 0;
            TorusPolynomial *bcopy = tpols + 1;
            TorusPolynomial *c = tpols + 2;
            TorusPolynomial *ccopy = tpols + 3;
            for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
                torusPolynomialUniform(b);
                torusPolynomialUniform(c);
                torusPolynomialCopy(bcopy, b);
                torusPolynomialCopy(ccopy, c);
                random_small_ints(a->coefs, 100000, N);
                int_tab_copy(acopy->coefs, a->coefs, N);
                torusPolynomialSubMulRKaratsuba(c, a, b);
                for (int32_t j = 0; j < N; j++) {
                    ASSERT_EQ(acopy->coefs[j], a->coefs[j]);
                    ASSERT_EQ(bcopy->coefsT[j], b->coefsT[j]);
                    Torus32 r = ccopy->coefsT[j];
                    for (int32_t k = 0; k < N; k++) {
                        r -= bcopy->coefsT[k] * anticyclic_get(acopy->coefs, j - k, N);
                    }
                    ASSERT_EQ(r, c->coefsT[j]);
                }
            }
            delete_IntPolynomial_array(2, ipols);
            delete_TorusPolynomial_array(4, tpols);
        }
    }


#if 0

#define torusPolynomialMulR torusPolynomialMultFFT
#define torusPolynomialAddMulR torusPolynomialAddMulRFFT
#define torusPolynomialSubMulR torusPolynomialSubMulRFFT

#endif


}
