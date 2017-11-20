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


void full_adder_MUX(LweSample *sum, const LweSample *x, const LweSample *y, const int32_t nb_bits,
                    const TFheGateBootstrappingSecretKeySet *keyset) {
    const LweParams *in_out_params = keyset->params->in_out_params;
    // carries
    LweSample *carry = new_LweSample_array(2, in_out_params);
    bootsSymEncrypt(carry, 0, keyset); // first carry initialized to 0
    // temps
    LweSample *temp = new_LweSample_array(2, in_out_params);

    for (int32_t i = 0; i < nb_bits; ++i) {
        //sumi = xi XOR yi XOR carry(i-1) 
        bootsXOR(temp, x + i, y + i, &keyset->cloud); // temp = xi XOR yi
        bootsXOR(sum + i, temp, carry, &keyset->cloud);

        // carry = MUX(xi XOR yi, carry(i-1), xi AND yi)
        bootsAND(temp + 1, x + i, y + i, &keyset->cloud); // temp1 = xi AND yi
        bootsMUX(carry + 1, temp, carry, temp + 1, &keyset->cloud);

        bool mess1 = bootsSymDecrypt(temp, keyset);
        bool mess2 = bootsSymDecrypt(carry, keyset);
        bool mess3 = bootsSymDecrypt(temp + 1, keyset);
        bool messmux = bootsSymDecrypt(carry + 1, keyset);

        if (messmux != (mess1 ? mess2 : mess3)) {
            cout << "ERROR!!! " << i << " - ";
            cout << t32tod(lwePhase(temp, keyset->lwe_key)) << " - ";
            cout << t32tod(lwePhase(carry, keyset->lwe_key)) << " - ";
            cout << t32tod(lwePhase(temp + 1, keyset->lwe_key)) << " - ";
            cout << t32tod(lwePhase(carry + 1, keyset->lwe_key)) << endl;
        }

        bootsCOPY(carry, carry + 1, &keyset->cloud);
    }
    bootsCOPY(sum + nb_bits, carry, &keyset->cloud);

    delete_LweSample_array(2, temp);
    delete_LweSample_array(2, carry);
}


void full_adder(LweSample *sum, const LweSample *x, const LweSample *y, const int32_t nb_bits,
                const TFheGateBootstrappingSecretKeySet *keyset) {
    const LweParams *in_out_params = keyset->params->in_out_params;
    // carries
    LweSample *carry = new_LweSample_array(2, in_out_params);
    bootsSymEncrypt(carry, 0, keyset); // first carry initialized to 0
    // temps
    LweSample *temp = new_LweSample_array(3, in_out_params);

    for (int32_t i = 0; i < nb_bits; ++i) {
        //sumi = xi XOR yi XOR carry(i-1) 
        bootsXOR(temp, x + i, y + i, &keyset->cloud); // temp = xi XOR yi
        bootsXOR(sum + i, temp, carry, &keyset->cloud);

        // carry = (xi AND yi) XOR (carry(i-1) AND (xi XOR yi))
        bootsAND(temp + 1, x + i, y + i, &keyset->cloud); // temp1 = xi AND yi
        bootsAND(temp + 2, carry, temp, &keyset->cloud); // temp2 = carry AND temp
        bootsXOR(carry + 1, temp + 1, temp + 2, &keyset->cloud);
        bootsCOPY(carry, carry + 1, &keyset->cloud);
    }
    bootsCOPY(sum + nb_bits, carry, &keyset->cloud);

    delete_LweSample_array(3, temp);
    delete_LweSample_array(2, carry);
}


void comparison_MUX(LweSample *comp, const LweSample *x, const LweSample *y, const int32_t nb_bits,
                    const TFheGateBootstrappingSecretKeySet *keyset) {
    const LweParams *in_out_params = keyset->params->in_out_params;
    // carries
    LweSample *carry = new_LweSample_array(2, in_out_params);
    bootsSymEncrypt(carry, 1, keyset); // first carry initialized to 1
    // temps
    LweSample *temp = new_LweSample(in_out_params);

    for (int32_t i = 0; i < nb_bits; ++i) {
        bootsXOR(temp, x + i, y + i, &keyset->cloud); // temp = xi XOR yi
        bootsMUX(carry + 1, temp, y + i, carry, &keyset->cloud);
        bootsCOPY(carry, carry + 1, &keyset->cloud);
    }
    bootsCOPY(comp, carry, &keyset->cloud);

    delete_LweSample(temp);
    delete_LweSample_array(2, carry);
}









//EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
//EXPORT void tfhe_createLweBootstrappingKeyFFT(LweBootstrappingKeyFFT* bk, const LweKey* key_in, const TGswKey* rgsw_key);
//EXPORT void tfhe_bootstrapFFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


#ifndef NDEBUG
extern const TLweKey *debug_accum_key;
extern const LweKey *debug_extract_key;
extern const LweKey *debug_in_key;
#endif

int32_t main(int32_t argc, char **argv) {
#ifndef NDEBUG
    cout << "DEBUG MODE!" << endl;
#endif
    const int32_t nb_bits = 16;
    const int32_t nb_trials = 10;

    // generate params 
    int32_t minimum_lambda = 100;
    TFheGateBootstrappingParameterSet *params = new_default_gate_bootstrapping_parameters(minimum_lambda);
    const LweParams *in_out_params = params->in_out_params;
    // generate the secret keyset
    TFheGateBootstrappingSecretKeySet *keyset = new_random_gate_bootstrapping_secret_keyset(params);


    for (int32_t trial = 0; trial < nb_trials; ++trial) {

        // generate samples
        LweSample *x = new_LweSample_array(nb_bits, in_out_params);
        LweSample *y = new_LweSample_array(nb_bits, in_out_params);
        for (int32_t i = 0; i < nb_bits; ++i) {
            bootsSymEncrypt(x + i, rand() % 2, keyset);
            bootsSymEncrypt(y + i, rand() % 2, keyset);
        }
        // output sum
        LweSample *sum = new_LweSample_array(nb_bits + 1, in_out_params);



        // evaluate the addition circuit 
        cout << "starting bootstrapping " << nb_bits << "-bits addition circuit (FA in MUX version)...trial " << trial
             << endl;
        clock_t begin1 = clock();
        full_adder_MUX(sum, x, y, nb_bits, keyset);
        clock_t end1 = clock();
        cout << "finished bootstrappings " << nb_bits << "-bits addition circuit (FA in MUX version)" << endl;
        cout << "total time (microsecs)... " << (end1 - begin1) << endl;


        // verification
        bool messCarry = 0;
        for (int32_t i = 0; i < nb_bits; ++i) {
            bool messX = bootsSymDecrypt(x + i, keyset);
            bool messY = bootsSymDecrypt(y + i, keyset);
            bool messSum = bootsSymDecrypt(sum + i, keyset);

            if (messSum != (messX ^ messY ^ messCarry)) {
                cout << "ERROR!!! " << trial << "," << i << " - ";
                cout << t32tod(lwePhase(x + i, keyset->lwe_key)) << " - ";
                cout << t32tod(lwePhase(y + i, keyset->lwe_key)) << " - ";
                cout << t32tod(lwePhase(sum + i, keyset->lwe_key)) << endl;
            }

            messCarry = messCarry ? (messX || messY) : (messX && messY);
        }
        bool messSum = bootsSymDecrypt(sum + nb_bits, keyset);
        if (messSum != messCarry) {
            cout << "ERROR!!! " << trial << "," << nb_bits << endl;
        }





        // evaluate the addition circuit 
        cout << "starting bootstrapping " << nb_bits << "-bits addition circuit (FA)...trial " << trial << endl;
        clock_t begin2 = clock();
        full_adder(sum, x, y, nb_bits, keyset);
        clock_t end2 = clock();
        cout << "finished bootstrappings " << nb_bits << "-bits addition circuit (FA)" << endl;
        cout << "total time (microsecs)... " << (end2 - begin2) << endl;


        // verification
        {
            bool messCarry = 0;
            for (int32_t i = 0; i < nb_bits; ++i) {
                bool messX = bootsSymDecrypt(x + i, keyset);
                bool messY = bootsSymDecrypt(y + i, keyset);
                bool messSum = bootsSymDecrypt(sum + i, keyset);

                if (messSum != (messX ^ messY ^ messCarry)) {
                    cout << "ERROR!!! " << trial << "," << i << endl;
                }

                messCarry = messCarry ? (messX || messY) : (messX && messY);
            }
            bool messSum = bootsSymDecrypt(sum + nb_bits, keyset);
            if (messSum != messCarry) {
                cout << "ERROR!!! " << trial << "," << nb_bits << endl;
            }
        }


        LweSample *comp = new_LweSample(in_out_params);
        // evaluate the addition circuit 
        cout << "starting bootstrapping " << nb_bits << "-bits comparison...trial " << trial << endl;
        clock_t begin3 = clock();
        comparison_MUX(comp, x, y, nb_bits, keyset);
        clock_t end3 = clock();
        cout << "finished bootstrappings " << nb_bits << "-bits comparison" << endl;
        cout << "total time (microsecs)... " << (end3 - begin3) << endl;

        // verification
        {
            bool messCarry = 1;
            for (int32_t i = 0; i < nb_bits; ++i) {
                bool messX = bootsSymDecrypt(x + i, keyset);
                bool messY = bootsSymDecrypt(y + i, keyset);

                messCarry = (messX ^ messY) ? messY : messCarry;
            }
            bool messComp = bootsSymDecrypt(comp, keyset);
            if (messComp != messCarry) {
                cout << "ERROR!!! " << trial << "," << nb_bits << endl;
            }
        }


        delete_LweSample_array(nb_bits + 1, sum);
        delete_LweSample_array(nb_bits, y);
        delete_LweSample_array(nb_bits, x);
    }

    delete_gate_bootstrapping_secret_keyset(keyset);
    delete_gate_bootstrapping_parameters(params);

    return 0;
}
