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

## reading and decrypting the final result

Finally, the owner of the secret key retrieves, decrypts and prints the final answer as follow: 

~~~c
int main() {

    //reads the cloud key from file
    FILE* secret_key = fopen("secret.key","rb");
    TFheGateBootstrappingSecretKeySet* key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
    fclose(secret_key);
 
    //if necessary, the params are inside the key
    const TFheGateBootstrappingParameterSet* params = key->params;

    //read the 16 ciphertexts of the result
    LweSample* answer = new_gate_bootstrapping_ciphertext_array(16, params);

    //import the 32 ciphertexts from the answer file
    FILE* answer_data = fopen("answer.data","rb");
    for (int i=0; i<16; i++) 
        import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i], params);
    fclose(answer_data);

    //decrypt and rebuild the 16-bit plaintext answer
    int16_t int_answer = 0;
    for (int i=0; i<16; i++) {
        int ai = bootsSymDecrypt(&answer[i], key);
        int_answer |= (ai<<i);
    }

    printf("And the result is: %d\n",int_answer);
    printf("I hope you remember what was the question!\n");

    //clean up all pointers
    delete_gate_bootstrapping_ciphertext_array(16, answer);
    delete_gate_bootstrapping_secret_keyset(key);
}
~~~

[tutorial intro](coding.html) - 
[Step 1: alice.c](tuto-alice.html) - 
[Step 2: cloud.c](tuto-cloud.html) - 
[Step 3: verif.c](tuto-verif.html) - 
[the full API](gate-bootstrapping-api.html) 

