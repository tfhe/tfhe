#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe.h"
#include "polynomials.h"
#include "lwesamples.h"
#include "lwekey.h"
#include "lweparams.h"
#include "tlwe.h"
#include "tgsw.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}


//EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
//EXPORT void tfhe_createLweBootstrappingKeyFFT(LweBootstrappingKeyFFT* bk, const LweKey* key_in, const TGswKey* rgsw_key);
//EXPORT void tfhe_bootstrapFFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


#ifndef NDEBUG
extern const TLweKey* debug_accum_key;
extern const LweKey* debug_extract_key;
extern const LweKey* debug_in_key;
#endif

int main(int argc, char** argv) {
#ifndef NDEBUG
    cout << "DEBUG MODE!" << endl;
#endif
    const int nb_bits = 64; 
    const int nb_trials = 10;
    
    // generate params 
    int minimum_lambda = 100;
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);
    const LweParams* in_out_params = params->in_out_params;
    // generate the secret keyset
    TFheGateBootstrappingSecretKeySet* keyset = new_random_gate_bootstrapping_secret_keyset(params);


    for (int trial = 0; trial < nb_trials; ++trial) {

        // generate samples
        LweSample* x = new_LweSample_array(nb_bits, in_out_params); 
        LweSample* y = new_LweSample_array(nb_bits, in_out_params); 
        for (int i = 0; i < nb_bits; ++i) {
            bootsSymEncrypt(x+i, rand()%2, keyset);
            bootsSymEncrypt(y+i, rand()%2, keyset);
        }
        // output sum
        LweSample* sum = new_LweSample_array(nb_bits+1, in_out_params);
        // carries
        LweSample* carry = new_LweSample_array(2, in_out_params);
        bootsSymEncrypt(carry, 0, keyset); // first carry initialized to 0
        // temps
        LweSample* temp = new_LweSample_array(3, in_out_params);


        // evaluate the addition circuit 
        cout << "starting bootstrapping addition circuit (FA in MUX version)...trial " << trial << endl;
        clock_t begin = clock();
        for (int i = 0; i < nb_bits; ++i) {
            //sumi = xi XOR yi XOR carry(i-1) 
            bootsXOR(temp, x+i, y+i, &keyset->cloud);
            bootsXOR(sum+i, temp, carry, &keyset->cloud);

            // carry = MUX(carry(i-1), xi OR yi, xi AND yi)
            bootsOR(temp+1, x+i, y+i, &keyset->cloud);
            bootsAND(temp+2, x+i, y+i, &keyset->cloud);
            bootsMUX(carry+1, carry, temp+1, temp+2, &keyset->cloud);
            bootsCOPY(carry, carry+1, &keyset->cloud);
        }
        bootsCOPY(sum+nb_bits, carry, &keyset->cloud);
        clock_t end = clock();
        cout << "finished bootstrappings addition circuit (FA in MUX version)" << endl;
        cout << "total time (microsecs)... " << (end-begin) << endl;


        // verification
        bool messCarry = 0;
        for (int i = 0; i < nb_bits; ++i) {
            bool messX = bootsSymDecrypt(x+i, keyset);
            bool messY = bootsSymDecrypt(y+i, keyset);
            bool messSum = bootsSymDecrypt(sum+i, keyset);
            
            if (messSum != (messX+messY+messCarry)){
                cout << "ERROR!!! " << trial << "," << i << endl; 
            }

            messCarry = messCarry?(messX || messY):(messX && messY);            
        }
        bool messSum = bootsSymDecrypt(sum+nb_bits, keyset);
        if (messSum != messCarry){
            cout << "ERROR!!! " << trial << "," << nb_bits << endl; 
        }


        delete_LweSample_array(3,temp);
        delete_LweSample_array(2,carry);
        delete_LweSample_array(nb_bits+1,sum);
        delete_LweSample_array(nb_bits,y);
        delete_LweSample_array(nb_bits,x);
    }
    
    delete_gate_bootstrapping_secret_keyset(keyset);
    delete_gate_bootstrapping_parameters(params);

    return 0;
}
