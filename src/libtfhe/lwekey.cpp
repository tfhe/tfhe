#include <cstdlib>
#include "lwekey.h"
#include "lweparams.h"

using namespace std;

LweKey::LweKey(const LweParams* params) {
    this->params = params;
    this->key = new int32_t[params->n];
}

LweKey::~LweKey() {
    delete[] key;
}

