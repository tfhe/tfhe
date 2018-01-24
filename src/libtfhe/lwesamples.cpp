#include "lwesamples.h"
#include "lweparams.h"

using namespace std;

template<typename TORUS>
LweSample<TORUS>::LweSample(const LweParams<TORUS>* params)
{
	a = new TORUS[params->n];
  b = TORUS(0);
  current_variance = 0.;
}

template<typename TORUS>
LweSample<TORUS>::~LweSample() {
  delete[] a;
}
