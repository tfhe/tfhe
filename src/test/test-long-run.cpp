#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe.h"

using namespace std;




#ifndef NDEBUG
extern const TLweKey* debug_accum_key;
extern const LweKey* debug_extract_key;
extern const LweKey* debug_in_key;
#endif


// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


int main(int argc, char** argv) {
#ifndef NDEBUG
cout << "DEBUG MODE!" << endl;
#endif
    
    int nb_test_gates = 1000000; // number of gates to be tested 
    int nb_samples = 10; // number of samples to be tested 

    // Parameters
    int minimum_lambda = 100;
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);
    const LweParams* in_out_params = params->in_out_params;
    // Secret keyset
    TFheGateBootstrappingSecretKeySet* keyset = new_random_gate_bootstrapping_secret_keyset(params);

    // Generate an array containing nb_samples Lwe samples, all initially encoding false
    LweSample* test = new_LweSample_array(nb_samples, in_out_params);
    for (int i = 0; i < nb_samples; ++i) {
        bootsSymEncrypt(test+i, 0, keyset);
    }
    

    for (int i = 0; i < nb_test_gates; ++i) {
        int gate = rand()%11; // randomly chose a gate between the 10 binary gates and the MUX
        int in1, in2, in3, out; // indexes
        bool mess1, mess2, mess3, mess; // messages in clear

        switch(gate)
        {
            /** bootstrapped Nand Gate */ 
            // bootsNAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (0):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsNAND(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != 1 - (mess1 && mess2)){
                    cout << "ERROR NAND!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;
            

            /** bootstrapped Or Gate:  */ 
            // bootsOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (1):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsOR(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != mess1 || mess2){
                    cout << "ERROR OR!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped And Gate: result = a and b */ 
            // bootsAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (2):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsAND(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != mess1 && mess2){
                    cout << "ERROR AND!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped Xor Gate: result = a xor b */ 
            // bootsXOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (3):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsXOR(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != (mess1 ^ mess2)){
                    cout << "ERROR XOR!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped Xnor Gate: result = (a==b) */ 
            // bootsXNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (4):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsXNOR(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != (mess1==mess2)){
                    cout << "ERROR XNOR!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped Nor Gate: result = not(a or b) */ 
            // bootsNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (5):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsNOR(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != 1 - (mess1 || mess2)){
                    cout << "ERROR NOR!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped AndNY Gate: not(a) and b */ 
            // bootsANDNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (6):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsANDNY(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != (1 - mess1) && mess2){
                    cout << "ERROR ANDNY!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped AndYN Gate: a and not(b) */ 
            // bootsANDYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (7):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsANDYN(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != mess1 && (1 - mess2)){
                    cout << "ERROR ANDYN!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped OrNY Gate: not(a) or b */ 
            // bootsORNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (8):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsORNY(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != (1 - mess1) || mess2){
                    cout << "ERROR ORNY!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;


            /** bootstrapped OrYN Gate: a or not(b) */ 
            // bootsORYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
            case (9):
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                // test the gate 
                bootsORYN(test+out, test+in1, test+in2, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != (mess1 || (1 - mess2))){
                    cout << "ERROR ORYN!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
                break;

            
            /** bootstrapped Mux(a,b,c) = a?b:c */ 
            // bootsMUX(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* c, const TFheGateBootstrappingCloudKeySet* bk);
            default:    
                // randomply chose 2 inputs and the output between the samples
                in1 = rand()%nb_samples; 
                in2 = rand()%nb_samples; 
                in3 = rand()%nb_samples; 
                out = rand()%nb_samples; 
                // messages in input
                mess1 = bootsSymDecrypt(test+in1, keyset);
                mess2 = bootsSymDecrypt(test+in2, keyset);
                mess3 = bootsSymDecrypt(test+in3, keyset);
                // test the gate 
                bootsMUX(test+out, test+in1, test+in2, test+in3, &keyset->cloud);
                mess = bootsSymDecrypt(test+out, keyset);

                // verification 
                if (mess != mess1?mess2:mess3){
                    cout << "ERROR MUX!!! " << i << "," << t32tod(lwePhase(test+out, keyset->lwe_key)) << endl; 
                }
        }





        /** bootstrapped Not Gate: result = not(a) */ 
        // EXPORT void bootsNOT(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk);

    }



    return 0;
}