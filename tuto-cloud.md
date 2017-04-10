---
layout: page
title: TFHE Fast Fully Homomorphic Encryption over the Torus
---

[back: using the library](usage.html) - 
[home](index.html) -
[tutorial intro](coding.html) - 
[Step 1: alice.c](tuto-alice.html) - 
[Step 2: cloud.c](tuto-cloud.html) - 
[Step 3: verif.c](tuto-verif.html) - 
[the full API](gate-bootstrapping-api.html) 

## evaluating a circuit

The owner of the cloud key can evaluate boolean gates over ciphertexts. 
The simplest way is to construct your full circuit hierarchically. 
The following C code shows how you can code a circuit that does the
minimum between two 16-bit integers.

~~~c
// elementary full comparator gate that is used to compare the i-th bit:
//   input: ai and bi the i-th bit of a and b
//          lsb_carry: the result of the comparison on the lowest bits
//   algo: if (a==b) return lsb_carry else return b 
void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk) {
    bootsXNOR(tmp, a, b, bk);
    bootsMUX(result, tmp, lsb_carry, a, bk);
}

// this function compares two multibit words, and puts the max in result
void minimum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
    LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    
    //initialize the carry to 0
    bootsCONSTANT(&tmps[0], 0, bk);
    //run the elementary comparator gate n times
    for (int i=0; i<nb_bits; i++) {
        compare_bit(&tmps[0], &a[i], &b[i], &tmps[0], &tmps[1], bk);
    }
    //tmps[0] is the result of the comparaison: 0 if a is larger, 1 if b is larger
    //select the max and copy it to the result
    for (int i=0; i<nb_bits; i++) {
        bootsMUX(&result[i], &tmps[0], &b[i], &a[i], bk);
    }

    delete_gate_bootstrapping_ciphertext_array(2, tmps);    
}
~~~


## importing ciphertexts, and performing computations

Now that we have a circuit, let us put everything together in a program
that first imports the coud key and encrypted input data, calls the
homomorphic circuit, and exports the result to a file.

~~~c
int main() {
    
    //reads the cloud key from file
    FILE* cloud_key = fopen("cloud.key","rb");
    TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);
    fclose(cloud_key);
 
    //if necessary, the params are inside the key
    const TFheGateBootstrappingParameterSet* params = bk->params;

    //read the 2x16 ciphertexts
    LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(16, params);
    LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(16, params);

    //reads the 2x16 ciphertexts from the cloud file
    FILE* cloud_data = fopen("cloud.data","rb");
    for (int i=0; i<16; i++) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext1[i], params);
    for (int i=0; i<16; i++) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext2[i], params);
    fclose(cloud_data);

    //do some operations on the ciphertexts: here, we will compute the
    //minimum of the two
    LweSample* result = new_gate_bootstrapping_ciphertext_array(16, params);
    minimum(result, ciphertext1, ciphertext2, 16, bk);

    //export the 32 ciphertexts to a file (for the cloud)
    FILE* answer_data = fopen("answer.data","wb");
    for (int i=0; i<16; i++) export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i], params);
    fclose(answer_data);

    //clean up all pointers
    delete_gate_bootstrapping_ciphertext_array(16, result);
    delete_gate_bootstrapping_ciphertext_array(16, ciphertext2);
    delete_gate_bootstrapping_ciphertext_array(16, ciphertext1);
    delete_gate_bootstrapping_cloud_keyset(bk);

}
~~~

[tutorial intro](coding.html) - 
[Step 1: alice.c](tuto-alice.html) - 
[Step 2: cloud.c](tuto-cloud.html) - 
[Step 3: verif.c](tuto-verif.html) - 
[the full API](gate-bootstrapping-api.html) 


