#include <cassert>
#include <cmath>
#include <cstdlib>
#include "tfhe_core.h"
#include "polynomials.h"
// #include "polynomials_arithmetic.h"
// #include "lagrangehalfc_arithmetic.h"

using namespace std;

IntPolynomial::IntPolynomial(const int N)
:
  N(N)
{
  this->coefs = new int[N];
}

IntPolynomial::~IntPolynomial() {
  delete[] coefs;
}

template<typename TORUS>
TorusPolynomial<TORUS>::TorusPolynomial(const int N)
:
  N(N)
{
 this->coefsT = new TORUS[N];
}

template<typename TORUS>
TorusPolynomial<TORUS>::~TorusPolynomial() {
  delete[] coefsT;
}
