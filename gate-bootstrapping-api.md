---
layout: page
title: TFHE: Gate Bootstrapping API
---

[back: tutorial](coding.html) - 
[home](index.html) -

# The Gate Bootstrapping API

The gate bootstrapping api of TFHE has been kept as simple as possible.
The four components are
* Parameter sets: TFheGateBootstrappingParameterSet
* Secret keys: TFheGateBootstrappingSecretKeySet
* Cloud keys: TFheGateBootstrappingCloudKeySet
* Ciphertexts: LweSample

### Parameters

~~~cpp
/** generate default gate bootstrapping parameters */
TFheGateBootstrappingParameterSet* new_default_gate_bootstrapping_parameters(int minimum_lambda);

/** These functions print the tfhe gate bootstrapping parameter set to a file */
void export_tfheGateBootstrappingParameterSet_toFile(FILE* F, const TFheGateBootstrappingParameterSet* params);
void export_tfheGateBootstrappingParameterSet_toStream(std::ostream& F, const TFheGateBootstrappingParameterSet* params);

/** read and create a tfhe gate bootstrapping parameter set from a File. */
TFheGateBootstrappingParameterSet* new_tfheGateBootstrappingParameterSet_fromFile(FILE* F);
TFheGateBootstrappingParameterSet* new_tfheGateBootstrappingParameterSet_fromStream(std::istream& F);

/** deletes gate bootstrapping parameters */
void delete_gate_bootstrapping_parameters(TFheGateBootstrappingParameterSet* params);
~~~

### Secret keyset

~~~cpp
/** (if needed) seeds the random number generator whith this array of values */
void tfhe_random_generator_setSeed(uint32_t* values, int size);
/** generate a random gate bootstrapping secret key */
TFheGateBootstrappingSecretKeySet* new_random_gate_bootstrapping_secret_keyset(const TFheGateBootstrappingParameterSet* params);

/** This function prints the tfhe gate bootstrapping parameter set to a file */
void export_tfheGateBootstrappingSecretKeySet_toFile(FILE* F, const TFheGateBootstrappingSecretKeySet* params);
void export_tfheGateBootstrappingSecretKeySet_toStream(std::ostream& F, const TFheGateBootstrappingSecretKeySet* params);

/** This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. */
TFheGateBootstrappingSecretKeySet* new_tfheGateBootstrappingSecretKeySet_fromFile(FILE* F);
TFheGateBootstrappingSecretKeySet* new_tfheGateBootstrappingSecretKeySet_fromStream(std::istream& F);

/** deletes a gate bootstrapping secret key */
void delete_gate_bootstrapping_secret_keyset(TFheGateBootstrappingSecretKeySet* keyset);
~~~

### Cloud keyset

The cloud keyset can be obtained from a secret key structure by accessing ```key->cloud```.

~~~cpp
/** prints the tfhe gate bootstrapping cloud key to a file */
void export_tfheGateBootstrappingCloudKeySet_toFile(FILE* F, const TFheGateBootstrappingCloudKeySet* params);
void export_tfheGateBootstrappingCloudKeySet_toStream(std::ostream& F, const TFheGateBootstrappingCloudKeySet* params);

/** reads and creates a tfhe gate bootstrapping cloud key from a File. */
TFheGateBootstrappingCloudKeySet* new_tfheGateBootstrappingCloudKeySet_fromFile(FILE* F);
TFheGateBootstrappingCloudKeySet* new_tfheGateBootstrappingCloudKeySet_fromStream(std::istream& F);

/** deletes a gate bootstrapping cloud key */
void delete_gate_bootstrapping_cloud_keyset(TFheGateBootstrappingCloudKeySet* keyset);
~~~

### Ciphertexts

~~~cpp
/** generate a new unititialized ciphertext (or an array of ciphertexts) */
LweSample* new_gate_bootstrapping_ciphertext(const TFheGateBootstrappingParameterSet* params);
LweSample* new_gate_bootstrapping_ciphertext_array(int nbelems, const TFheGateBootstrappingParameterSet* params);

/** deletes a ciphertext (or an array of ciphertexts) */
void delete_gate_bootstrapping_ciphertext(LweSample* sample);
void delete_gate_bootstrapping_ciphertext_array(int nbelems, LweSample* samples);

/** Prints a gate bootstrapping ciphertext to a file */
void export_gate_bootstrapping_ciphertext_toFile(FILE* F, const LweSample* sample, const TFheGateBootstrappingParameterSet* params);
void export_gate_bootstrapping_ciphertext_toStream(std::ostream& F, const LweSample* sample, const TFheGateBootstrappingParameterSet* params);

/** reads a tfhe gate bootstrapping ciphertext from a File. */
void import_gate_bootstrapping_ciphertext_fromFile(FILE* F, LweSample* sample, const TFheGateBootstrappingParameterSet* params);
void import_gate_bootstrapping_ciphertext_fromStream(std::istream& F, LweSample* sample, const TFheGateBootstrappingParameterSet* params);
~~~

### Encrypt decrypt

~~~cpp
/** encrypts a boolean */
void bootsSymEncrypt(LweSample* result, int message, const TFheGateBootstrappingSecretKeySet* params);

/** decrypts a boolean */
int bootsSymDecrypt(const LweSample* sample, const TFheGateBootstrappingSecretKeySet* params);
~~~

### Homomorphic operations

~~~cpp
/** bootstrapped Constant (true or false) trivial Gate */ 
void bootsCONSTANT(LweSample* result, int value, const TFheGateBootstrappingCloudKeySet* bk);

/** bootstrapped Not Gate: result = not(a) */ 
void bootsNOT(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Copy Gate: result = a */ 
void bootsCOPY(LweSample* result, const LweSample* ca, const TFheGateBootstrappingCloudKeySet* bk);

/** bootstrapped Nand Gate */ 
void bootsNAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Or Gate:  */ 
void bootsOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped And Gate: result = a and b */ 
void bootsAND(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Xor Gate: result = a xor b */ 
void bootsXOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Xnor Gate: result = (a==b) */ 
void bootsXNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped Nor Gate: result = not(a or b) */ 
void bootsNOR(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped AndNY Gate: not(a) and b */ 
void bootsANDNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped AndYN Gate: a and not(b) */ 
void bootsANDYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped OrNY Gate: not(a) or b */ 
void bootsORNY(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);
/** bootstrapped OrYN Gate: a or not(b) */ 
void bootsORYN(LweSample* result, const LweSample* ca, const LweSample* cb, const TFheGateBootstrappingCloudKeySet* bk);

/** bootstrapped Mux(a,b,c) = a?b:c */ 
void bootsMUX(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* c, const TFheGateBootstrappingCloudKeySet* bk);
~~~
