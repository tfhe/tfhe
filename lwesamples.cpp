#include "lwesamples.h"
#include "lweparams.h"

using namespace std;

LWESample::LWESample(const LWEParams* params) {
    a = new Torus32[params->n];
}

LWESample::~LWESample() {
    delete[] a;
}
