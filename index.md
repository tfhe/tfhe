---
layout: default
title: TFHE Fast Fully Homomorphic Encryption over the Torus
---

# TFHE: Fast Fully Homomorphic Encryption over the Torus

**version 1.0-rc1** -- *first release candidate date: 2017.04.05*

TFHE is an open-source library for fully homomorphic encryption, 
distributed under the terms of the Apache 2.0 license. 

The underlying scheme is described in best paper of the IACR
conference Asiacrypt 2016: "Faster fully homomorphic encryption: 
Bootstrapping in less than 0.1 seconds", presented by 
Ilaria Chillotti, Nicolas Gama, Mariya Georgieva and Malika Izabachène.

### Description 

TFHE is a C/C++ library which implements a very fast gate-by-gate
bootstrapping, based on [CGGI16]. The library allows to evaluate an
arbitrary boolean circuit composed of binary gates, over encrypted data,
without revealing any information on the data.

The library supports the homomorphic evaluation of the 10 binary gates (And, Or, Xor, 
Nand, Nor, etc...), as well as the negation and the Mux gate. Each binary gate takes 
about 20 milliseconds single-core time to evaluate, which improves [DM15] by a factor 30, 
and the mux gate takes about 40 CPU-ms.

Unlike other libraries, the gate-bootstrapping mode of TFHE has no
restriction on the number of gates or on their composition. This allows
to perform any computation over encrypted data, even if the actual
function that will be applied is not yet known when the data is
encrypted. The library is easy to use with either manually crafted
circuits, or with the output of automated circuit generation tools. 

From the user point of view, the library can:
* generate a secret-keyset and a cloud-keyset. The secret keyset is
  private, and provides encryption/decryption abilities. The cloud-keyset 
  can be exported to the cloud, and allows to operate over encrypted
  data. 
* With the secret keyset, the library allows to encrypt and decrypt data.
  The encrypted data can safely be outsourced to the cloud, in order to perform
  secure homomorphic computations. 
* With the cloud-keyset, the library can evaluate a net-list of
  binary gates homomorphically at a rate of about 50 gates per second per
  core, without decrypting its input. It suffices to provide the sequence
  of gates, as well as ciphertexts of the input bits. And the
  library computes ciphertexts of the output bits.

### Under the hood

The library implements a Ring-variant of the GSW [GSW13]
cryptosystem and makes many optimizations described in [DM15] and
[CGGI16]. 

It also implements a dedicated Fast Fourier
Transformation for the anticyclic ring \\(\mathbb{R}[X]/(X^N+1)\\), and uses AVX
assembly vectorization instructions. 
The default parameter set achieves a 120-bit cryptographic security,
based on ideal lattice assumptions.


Since the running time per gate seems to be the bottleneck of fully
homomorphic encryption, an optimal circuits for TFHE is most likely a circuit 
with the smallest possible number of gates, and to a lesser extent, the 
possibility to evaluate them in parallel. 


### Dependencies 


The library interface can be used in a regular C code. However, to
compile the core of the library you will need a standard C++11 compiler.
Currently, the project has only been tested with the g++ compiler under
Linux. In the future, we plan to extend the compatibility to other
compilers, platforms and operating systems.

At least one FFT processor is needed to run the project:

* The default processor comes from Project Nayuki, who proposes two
  implementations of the fast Fourier transform - one in portable C, and
  the other using the AVX assembly instructions.
  This component is licensed under the MIT license, and we added the code
  of the reverse FFT (both in C and in assembly). Original source:
  [https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly](https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly){:target="_blank"}
* we provide another processor, named the spqlios processor, which is
  written in AVX and FMA assembly in the style of the nayuki processor,
  and which is dedicated to the ring \\(\mathbb{R}[X]/(X^N+1)\\) for \\(N\\) a power of 2.

<!--
* We also provide a connector for the FFTW3 library: http://www.fftw.org.
  With this library, the performance of the FFT is between 2 and 3 times
  faster than the default Nayuki implementation. However, you should keep
  in mind that the library FFTW is published under the GPL License. If you
  choose to use this library in a final product, this product may have to
  be released under GPL License as well (other commercial licenses are
  available on their web site)
-->

* We plan to add other connectors in the future (for instance the Intel’s
  IPP Fourier Transform, which should be 1.5× faster than FFTW for 1D
  real data)


### Installation

To build the library with the default options, run ```make``` from the
top level directory of the TFHE project.
This assumes that the standard tool cmake is already installed on the
system, and an up-to-date c++ compiler (i.e. g++ >=5.2) as well.
It will compile the library in optimized mode in the build/libtfhe
folder.
Currently, only static libraries are generated. The default installation prefix
(```CMAKE_INSTALL_PREFIX```) is ```/usr/local```. After compilation, all static libraries 
will be installed to ```CMAKE_INSTALL_PREFIX/lib```, and headers will be installed to  
```CMAKE_INSTALL_PREFIX/include```

If you want to choose different or additional compile options (i.e. debug mode, 
tests, fftw), you need to run cmake manually and pass the desired options, which 
are described in the table .....

~~~sh
#clone the tfhe repository
git clone https://github.com/tfhe/tfhe.git
cd tfhe

#build the library
mkdir build
cd build
cmake ../src CMAKE_INSTALL_PREFIX=/usr/local
make

#install the library
make install #(with sudo if the install prefix requires root) 
~~~

### Using the library

#### Generating and saving parameters and keys

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

#### Encrypting data and exporting ciphertexts

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

#### importing ciphertexts, and performing computations

This code shows how to import a cloud key, and input ciphertexts.
The minimum function will compute the minimum between two 16-bit integers
and stores the answer in result. The end of the code exports the answer
to a file.

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

#### evaluating a circuit

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

#### reading and decrypting the final result

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

Now that you have your circuit, you can call it on encrypted data that
was sent by the client.

## Bla Bla Bla

The available options are the following:

<table>
  <thead>
    <tr>
      <th>Variable Name</th>
      <th>values</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>CMAKE_BUILD_TYPE</td>
      <td>
        <em>optim</em> enables compiler's optimization flags, including native 
             architecture specific optimizations<br>
        <em>debug</em> disables any optimization and include all debugging 
             info (-g3 -O0)
      </td>
    </tr>
    <tr>
      <td>ENABLE_TESTS</td>
      <td><em>on/off</em> compiles the library’s unit tests and sample applications in the test/ folder. This assumes that googletest&gt;1.8 is installed on the system. (use <code class="highlighter-rouge">ctest</code> to run all tests)</td>
    </tr>
    <tr>
      <td>ENABLE_FFTW</td>
      <td><em>on/off</em> compiles libtfhe-fftw.a, using FFTW3 (GPL licence) for fast FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_NAYUKI_PORTABLE</td>
      <td><em>on/off</em> compiles libtfhe-nayuki-portable.a, using the fast C version of nayuki for FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_NAYUKI_AVX</td>
      <td><em>on/off</em> compiles libtfhe-nayuki-avx.a, using the avx assembly version of nayuki for FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_SPQLIOS_AVX</td>
      <td><em>on/off</em> compiles libtfhe-spqlios-avx.a, using tfhe’s dedicated avx assembly version for FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_SPQLIOS_FMA</td>
      <td><em>on/off</em> compiles libtfhe-spqlios-fma.a, using tfhe’s dedicated fma assembly version for FFT computations</td>
    </tr>
  </tbody>
</table>

### References

[DM15](#): L. Ducas and D. Micciancio.  FHEW: Bootstrapping homomorphic
encryption in less than a second.  In Eurocrypt 2015, pages 617-640.

[GSW13](#):  C. Gentry, A. Sahai, and B. Waters. Homomorphic encryption from
learning with errors:  Conceptually-simpler,  asymptotically-faster,
attribute-based. In Crypto 2013, pages 75-92

[CGGI16](https://eprint.iacr.org/2016/870){:target="_blank"}: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. Faster fully homomorphic encryption: Bootstrapping in less than 0.1 seconds. Asiacrypt 2016. Cryptology ePrint Archive, report 2016/870.

