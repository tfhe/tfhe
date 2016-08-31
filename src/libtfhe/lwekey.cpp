#include <cstdlib>
#include "lwekey.h"
#include "lweparams.h"

using namespace std;

LWEKey::LWEKey(const LWEParams* params) {
    this->params = params;
    this->key = new int[params->n];
}

LWEKey::~LWEKey() {
    delete[] key;
}

