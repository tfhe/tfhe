#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "lwe.h"
#include "multiplication.h"
#include "polynomials.h"

#include "lwesamples.h"
#include "lweparams.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************



int main(int argc, char** argv) {
    
    LWEParams* params = new_LWEParams(1024, 0.2, 0.5); //les deux alpha mis un peu au hasard
    int n = params->n;
    LWEKey* key = new_LWEKey(params);
    LWESample* cipher = new_LWESample(params);
    Torus32 mu = pow(2,30);
    double alpha = 0.0625;
    Torus32 phi;
    double message;  
    int Msize = 2;  

    lweKeyGen(key);
    lweSymEncrypt(cipher, mu, alpha, key);
    cout << "C = [";
    for (int i = 0; i < n; ++i) cout << i << "-" << cipher->a[i] << ", ";
    cout << "b-" << cipher->b << "]" << endl;

    phi = lwePhase(cipher, key);
    cout << "phi = " << phi << endl;
    message = lweSymDecrypt(cipher, key, Msize);
    cout << "message = " << message << endl;

    return 0;
}
