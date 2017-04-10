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


## Generating and saving parameters and keys

In order to use the library, you first need to generate a secret key and
a cloud key. The following C example code shows how to do it.
The secret key (here, the file secret.key) must remain private, 
whereas its corresponding cloud key (cloud.key) can safely be 
sent to the cloud.

~~~c
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>

int main() {
    //generate a keyset
    const int minimum_lambda = 110;
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);

    //generate a random key
    uint32_t seed[] = { 314, 1592, 657 };
    tfhe_random_generator_setSeed(seed,3);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);

    //export the secret key to file for later use
    FILE* secret_key = fopen("secret.key","wb");
    export_tfheGateBootstrappingSecretKeySet_toFile(secret_key, key);
    fclose(secret_key);

    //export the cloud key to a file (for the cloud)
    FILE* cloud_key = fopen("cloud.key","wb");
    export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
    fclose(cloud_key);
   
    //you can put additional instructions here!!
    //...

    //clean up all pointers
    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);

}
~~~

## Encrypting data and exporting ciphertexts

The owner of the secret key can encrypt bits. The following C code shows
how to encrypt the 16-bit integer 2017 and 42, and to export the ciphertexts to
a file (here, cloud.data). You may for instance put this code inside the previous
file.

~~~c
    //generate encrypt the 16 bits of 2017
    int16_t plaintext1 = 2017;
    LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(16, params);
    for (int i=0; i<16; i++) {
        bootsSymEncrypt(&ciphertext1[i], (plaintext1>>i)&1, key);
    }

    //generate encrypt the 16 bits of 42
    int16_t plaintext2 = 42;
    LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(16, params);
    for (int i=0; i<16; i++) {
        bootsSymEncrypt(&ciphertext2[i], (plaintext2>>i)&1, key);
    }

    printf("Hi there! Today, I will ask the cloud what is the minimum between %d and %d\n",plaintext1, plaintext2);
    
    //export the 2x16 ciphertexts to a file (for the cloud)
    FILE* cloud_data = fopen("cloud.data","wb");
    for (int i=0; i<16; i++) 
        export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext1[i], params);
    for (int i=0; i<16; i++) 
        export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext2[i], params);
    fclose(cloud_data);

    //clean up all pointers
    delete_gate_bootstrapping_ciphertext_array(16, ciphertext2);
    delete_gate_bootstrapping_ciphertext_array(16, ciphertext1);
~~~


[tutorial intro](coding.html) - 
[Step 1: alice.c](tuto-alice.html) - 
[Step 2: cloud.c](tuto-cloud.html) - 
[Step 3: verif.c](tuto-verif.html) - 
[the full API](gate-bootstrapping-api.html) 

