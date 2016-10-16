#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwekeyswitch.h"

using namespace std;




/*
 * Homomorphic NAND gate
 * Takes in input 2 LWE samples (with message space [0,1/4]vs[-1/8,1/8], noise<1/16)
 * Outputs an LWE sample (with message space [0,1/2]vs[?,?], noise<1/4)
*/
void homNAND(LweSample* result, LweSample* c1, LweSample* c2, LweParams* params) {
  
}

