#include "lwesamples.h"
#include "lweparams.h"

using namespace std;

LWESample::LWESample(const LWEParams* params) 
{
	this->a = new Torus32[params->n];
    this->b = 0;
    this->alpha = 0.;
}

LWESample::~LWESample() {
    delete[] a;
}
