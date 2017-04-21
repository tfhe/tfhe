#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe.h"

using namespace std;





// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************



int main(int argc, char** argv) {

if (argc==2) {
    uint32_t seed = atoi(argv[1]);
    srand(seed);
    tfhe_random_generator_setSeed(&seed, 1);
} 


    static const int NAND_GATE = 0;
    static const int OR_GATE = 1;
    static const int AND_GATE = 2;
    static const int XOR_GATE = 3;
    static const int XNOR_GATE = 4;
    static const int NOR_GATE = 5;
    static const int ANDNY_GATE = 6;
    static const int ANDYN_GATE = 7;
    static const int ORNY_GATE = 8;
    static const int ORYN_GATE = 9;
    // static const int MUX_GATE = 10;
    
    static const int nb_test_gates = 1000000; // number of gates to be tested 
    static const int nb_samples = 10; // number of samples to be tested 
    static const Torus32 MU = modSwitchToTorus32(1,8);

    // Parameters
    int minimum_lambda = 100;
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);
    const LweParams* in_out_params = params->in_out_params;
    // Secret keyset
    TFheGateBootstrappingSecretKeySet* keyset = new_random_gate_bootstrapping_secret_keyset(params);
    const TFheGateBootstrappingCloudKeySet* key_cloud = &keyset->cloud;

    // Generate an array containing nb_samples Lwe samples, all initially encoding false
    LweSample* test = new_LweSample_array(nb_samples, in_out_params);
    for (int i = 0; i < nb_samples; ++i) {
        bootsSymEncrypt(test+i, 0, keyset);
    }
    

    for (int i = 0; i < nb_test_gates; ++i) {
        int gate = rand()%11; // randomly chose a gate between the 10 binary gates and the MUX

        // randomply chose 2/3 inputs and the output between the samples
        int in1 = rand()%nb_samples; 
        int in2 = rand()%nb_samples; 
        int in3 = rand()%nb_samples; 
        int out = rand()%nb_samples; 
        // randomly apply a not to the inputs 
        if (rand()%2 == 1){ bootsNOT(test+in1, test+in1, key_cloud); }
        if (rand()%2 == 1){ bootsNOT(test+in2, test+in2, key_cloud); }
        if (rand()%2 == 1){ bootsNOT(test+in3, test+in3, key_cloud); }
        // messages in input
        bool mess1 = bootsSymDecrypt(test+in1, keyset);
        bool mess2 = bootsSymDecrypt(test+in2, keyset);
        bool mess3 = bootsSymDecrypt(test+in3, keyset);
        bool mess;       

        switch(gate)
        {
            /** bootstrapped Nand Gate */ 
            // bootsNAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (NAND_GATE):
                // test the gate 
                bootsNAND(test+out, test+in1, test+in2, key_cloud);
                mess = 1 - (mess1 && mess2);
                break;
            

            /** bootstrapped Or Gate:  */ 
            // bootsOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (OR_GATE):
                // test the gate 
                bootsOR(test+out, test+in1, test+in2, key_cloud);
                mess = (mess1 || mess2);
                break;


            /** bootstrapped And Gate: result = a and b */ 
            // bootsAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (AND_GATE):
                // test the gate 
                bootsAND(test+out, test+in1, test+in2, key_cloud);
                mess = (mess1 && mess2);
                break;


            /** bootstrapped Xor Gate: result = a xor b */ 
            // bootsXOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (XOR_GATE):
                // test the gate 
                bootsXOR(test+out, test+in1, test+in2, key_cloud);
                mess = (mess1 ^ mess2);
                break;


            /** bootstrapped Xnor Gate: result = (a==b) */ 
            // bootsXNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (XNOR_GATE):
                // test the gate 
                bootsXNOR(test+out, test+in1, test+in2, key_cloud);
                mess = (mess1==mess2);
                break;


            /** bootstrapped Nor Gate: result = not(a or b) */ 
            // bootsNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (NOR_GATE):
                // test the gate 
                bootsNOR(test+out, test+in1, test+in2, key_cloud);
                mess = 1 - (mess1 || mess2);
                break;


            /** bootstrapped AndNY Gate: not(a) and b */ 
            // bootsANDNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (ANDNY_GATE):
                // test the gate 
                bootsANDNY(test+out, test+in1, test+in2, key_cloud);
                mess = ((1 - mess1) && mess2);
                break;


            /** bootstrapped AndYN Gate: a and not(b) */ 
            // bootsANDYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (ANDYN_GATE):
                // test the gate 
                bootsANDYN(test+out, test+in1, test+in2, key_cloud);
                mess = (mess1 && (1 - mess2));
                break;


            /** bootstrapped OrNY Gate: not(a) or b */ 
            // bootsORNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (ORNY_GATE):
                // test the gate 
                bootsORNY(test+out, test+in1, test+in2, key_cloud);
                mess = ((1 - mess1) || mess2);
                break;


            /** bootstrapped OrYN Gate: a or not(b) */ 
            // bootsORYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (ORYN_GATE):
                // test the gate 
                bootsORYN(test+out, test+in1, test+in2, key_cloud);
                mess = (mess1 || (1 - mess2));
                break;

            
            /** bootstrapped Mux(a,b,c) = a?b:c */ 
            // bootsMUX(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* c, const TFheGateBootstrappingCloudKeySet* bk);
            default:    
                // test the gate 
                bootsMUX(test+out, test+in1, test+in2, test+in3, key_cloud);
                mess = (mess1?mess2:mess3);   
        }

        // verification 
        if (bootsSymDecrypt(test+out, keyset) != mess){
            cerr << i << " " << gate << " " << t32tod(lwePhase(test+out, keyset->lwe_key) - (mess?MU:(-MU))) << endl; 
        }
        cout << i << " " << gate << " " << t32tod(lwePhase(test+out, keyset->lwe_key) - (mess?MU:(-MU))) << endl;

    }



    delete_LweSample_array(nb_samples,test);
    delete_gate_bootstrapping_secret_keyset(keyset);
    delete_gate_bootstrapping_parameters(params);


    return 0;
}