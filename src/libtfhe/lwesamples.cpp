#include "lwesamples.h"
#include "lweparams.h"


LweSample::LweSample(const LweParams* params)
{
	this->a = new Torus32[params->n];
    this->b = 0;
    this->current_variance = 0.;
}

LweSample::~LweSample() {
    delete[] a;
}
