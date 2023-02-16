# TFHE
Fast Fully Homomorphic Encryption Library over the Torus

This library is the original version of TFHE that implements the base arithmetic and functionalities (bootstrapped and leveled).
If you need an enhanced API with some additional and more recent features of TFHE, you can consult the [<span>TFHE-rs library</span>](https://github.com/zama-ai/tfhe-rs){:target="_blank"}.

**version 1.1** -- *Updated security parameters release date: 2020.02.21*

**version 1.0** -- *first release date: 2017.05.02*

**version 1.0-rc1** -- *first pre-release date: 2017.04.05*

**version 0.1** -- *Proof of concept release date: 2016.08.18*

TFHE is open-source software distributed under the terms of the Apache 2.0 license. 
The scheme is described in the paper "Faster fully homomorphic encryption: Bootstrapping in less than 0.1 seconds" presented at the IACR conference Asiacrypt 2016 by Ilaria Chillotti, Nicolas Gama, Mariya Georgieva, Malika Izabachène.


### Description 

The TFHE library implements a very fast gate-by-gate bootstrapping, based on [CGGI16]. Namely, any binary 
gate is evaluated homomorphically in about 13 milliseconds on a single
core which improves [DM15] by a factor 50, and the mux gate takes about 26 CPU-ms (or 13ms on 2 cores). 

The library implements a Ring-variant of the GSW [GSW13]
cryptosystem and makes many optimizations described in [DM15] and [CGGI16]. 

It also implements a dedicated Fast Fourier
Transformation for the anticyclic ring R[X]/(X^N+1), and uses AVX, AVX2 and FMA assembly vectorization instructions. 
The default parameter set achieves at least 110-bit of cryptographic security, based on ideal lattice assumptions.

From the user point of view, the library can evaluate a net-list of binary gates homomorphically at a rate of about 50 gates per second per core, without decrypting its input. It suffices to provide the sequence of gates, as well as ciphertexts of the input bits. And the
library computes ciphertexts of the output bits.

Unlike other libraries, TFHE has no restriction on the number of gates or on their composition. This makes the library usable with either
manually crafted circuits, or with the output of automated circuit generation tools. For TFHE, optimal circuits have the smallest possible number of gates, 
and to a lesser extent, the possibility to evaluate them in parallel. 



### Dependencies 


The library interface can be used in a regular C code. However, to compile the core of the library you will need a standard C++11 compiler.
Currently, the project has been tested with the g++ >= 5.2 compiler and clang >=3.8 under Linux, as well as clang under MacOS. In the future, we plan to extend the compatibility to other compilers, platforms and operating systems.

At least one FFT processor is needed to run the project:

* The default processor comes from Project Nayuki, who proposes two implementations of the fast Fourier transform - one in portable C, and the other using the AVX assembly instructions.
This component is licensed under the MIT license, and we added the code of the reverse FFT (both in C and in assembly). Original source: https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly
* we provide another processor, named the spqlios processor, which is written in AVX and FMA assembly in the style of the nayuki processor, and which is dedicated to the ring R[X]/(X^N+1) for N a power of 2.
* We also provide a connector for the FFTW3 library: http://www.fftw.org. With this library, the performance of the FFT is between 2 and 3 times faster than the default Nayuki implementation. However, you should keep in mind that the library FFTW is published under the GPL License. If you choose to use this library in a final product, this product may have to be released under GPL License as well (other commercial licenses are available on their web site)
* We plan to add other connectors in the future (for instance the Intel’s IPP Fourier Transform, which should be 1.5× faster than FFTW for 1D real data)


### Installation

To build the library with the default options, run ```make``` and ```make install``` from the top level directory of the TFHE project. This assumes that the standard tool cmake is already installed on the system, and an
up-to-date c++ compiler (i.e. g++ >=5.2 or clang >= 3.8) as well.
It will compile the shared library in optimized mode, and install it to the ```/usr/local/lib``` folder.

If you want to choose additional compile options (i.e. other installation folder, debug mode, tests, fftw), you need to run cmake manually and pass the desired options:
```
mkdir build
cd build
cmake ../src -DENABLE_TESTS=on -DENABLE_FFTW=on -DCMAKE_BUILD_TYPE=debug
make
```
The available options are the following:

| Variable Name          | values           | 
|------------------------|-------|
| CMAKE_INSTALL_PREFIX   | */usr/local* installation folder (libs go in lib/ and headers in include/) | 
| CMAKE_BUILD_TYPE       | <ul><li>*optim* enables compiler's optimization flags, including native architecture specific optimizations</li><li>*debug* disables any optimization and include all debugging info (-g3 -O0)</li> | 
| ENABLE_TESTS           | *on/off* compiles the library's unit tests and sample applications in the test/ folder. To enable this target, you first need to download google test sources: ```git submodule init; git submodule update``` (then, use ```ctest``` to run all unittests) | 
| ENABLE_FFTW            | *on/off* compiles libtfhe-fftw.a, using FFTW3 (GPL licence) for fast FFT computations |
| ENABLE_NAYUKI_PORTABLE | *on/off* compiles libtfhe-nayuki-portable.a, using the fast C version of nayuki for FFT computations |
| ENABLE_NAYUKI_AVX      | *on/off* compiles libtfhe-nayuki-avx.a, using the avx assembly version of nayuki for FFT computations |
| ENABLE_SPQLIOS_AVX     | *on/off* compiles libtfhe-spqlios-avx.a, using tfhe's dedicated avx assembly version for FFT computations |
| ENABLE_SPQLIOS_FMA     | *on/off* compiles libtfhe-spqlios-fma.a, using tfhe's dedicated fma assembly version for FFT computations |




### Security estimates and parameter choices.

The current parameters implemented in the TFHE library have been updated from the ones proposend in the original TFHE paper [CGGI16], 
according to the new estimates done in the JoC paper [CGGI19], and new attack models integrated in [<span>LWE estimator</span>](https://bitbucket.org/malb/lwe-estimator/src/master/){:target="_blank"}. 
The implementation uses two sets of keys on two different noise levels, both required to execute the gate bootstrapping. 

|                                 | ciphertext dimension `n`	  | noise rate (stdev) `sd`      | security bits $\lambda$ |
| ------------------------------- | ------- | --------- | -------------- |
| Key-Switching key (LWE)         | 630     | $2^{-15}$ | 128 bits     |
| Bootstrapping key (Ring-LWE)    | 1024    | $2^{-25}$ | 130 bits	 |
| **Overall security**            |         |           | 128 bits       |

With these parameters, the gate bootstrapping runs in about `10-20 ms`, depending on the machine: as instance, one bootstrapped binary gate takes about `13 ms` on a Intel i9-9900k CPU and about `17 ms` on an average i7 Xeon processor (single core).

Our security estimates are made by using the [<span>LWE estimator</span>](https://bitbucket.org/malb/lwe-estimator/src/master/){:target="_blank"}. 
The estimates can change according to the new attacks proposed in the litterature and the updates of the estimator itself.
If you want to use safe parameters on the library in production, please double check the estimates and update your code with the new parameters.

The code to use in the LWE estimator to estimate hardness for the standard deviation `sd` ($2^{-25}$ in the example) and dimension `n` 
(1024 in the example) is provided below. We recommend to target at least 128-bits of security. 
In our implementation, we use 32 bits integers (`q=2**32`) and binary keys. 
For the choice of all the other TFHE parameters, please refer to the noise formulas in [CGGI19].


_Note:_ we estimate the parameters by using some of the models listed in the [<span>Estimate all the LWE and NTRU schemes</span>](https://estimate-all-the-lwe-ntru-schemes.github.io/docs/){:target="_blank"}.
In particular, we consider the classical cost of BKZ-beta on a lattice of dimension `d` to be `2^(0.292*beta + 16.4 + log(8*d,2))`.
To obtain more conservative parameters, we suggest using the core-SVP methodology using classical cost `2^(0.292*beta)` and quantum cost `2^(0.265*beta)`.



#### ESTIMATOR CODE

```python
# To reproduce the estimate run this snippet on http://aleph.sagemath.org/
from sage.all import load, sqrt, RR, ZZ, pi, oo
load('https://bitbucket.org/malb/lwe-estimator/raw/HEAD/estimator.py')

n = 1024                 # ciphertext dimension (also, key entropy)
sd = 2**(-25)            # noise standard deviation
alpha = sqrt(2*pi)*sd    # estimator defines noise rate = sqrt(2pi).stdev
q = 2**32                # for compatibility only
m = oo                   # the attacker can use as many samples he wishes 
secret_distribution = (0,1)
success_probability = 0.99


# Chosen cost model 
# BKZ cost models: CLASSICAL - 0.292*beta + 16.4 + log(8*d,2) - primal
# i.e. BKZ.sieve =  lambda beta, d, B: ZZ(2)**RR(0.292*beta + 16.4 + log(8*d,2))
print("CLASSICAL PRIMAL")
print(primal_usvp(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=BKZ.sieve))
# BKZ cost models: CLASSICAL - 0.292*beta + 16.4 + log(8*d,2) - dual
# i.e. BKZ.sieve =  lambda beta, d, B: ZZ(2)**RR(0.292*beta + 16.4 + log(8*d,2))
print("CLASSICAL DUAL")
print(dual_scale(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=BKZ.sieve))


# For more conservative parameters, both classical and quantum  
# BKZ cost models: CLASSICAL - 0.292 beta - primal
reduction_cost_model =  lambda beta, d, B: ZZ(2)**RR(0.292*beta)
print("CLASSICAL PRIMAL (conservative)")
print(primal_usvp(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
# BKZ cost models: CLASSICAL - 0.292 beta - dual
print("CLASSICAL DUAL (conservative)")
print(dual_scale(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
# BKZ cost models: QUANTUM - 0.265 beta - primal
reduction_cost_model =  lambda beta, d, B: ZZ(2)**RR(0.265*beta)
print("QUANTUM PRIMAL (conservative)")
print(primal_usvp(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
# BKZ cost models: QUANTUM - 0.265 beta - dual
print("QUANTUM DUAL (conservative)")
print(dual_scale(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
```

_We would like to thank [<span>Fernando Virdia</span>](https://fundamental.domains/){:target="_blank"} for the help in the estimation of the security parameters._








### References

[CGGI19]: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. TFHE: Fast Fully Homomorphic Encryptionover the Torus. In Journal of Cryptology, volume 33, pages 34–91 (2020). [<span>PDF</span>](https://eprint.iacr.org/2018/421.pdf){:target="_blank"} 

[CGGI16]: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. Faster fully homomorphic encryption: Bootstrapping in less than 0.1 seconds. In Asiacrypt 2016 (Best Paper), pages 3-33.  [<span>PDF</span>](https://eprint.iacr.org/2016/870.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"}

[DM15]:   L. Ducas and D. Micciancio.  FHEW: Bootstrapping homomorphic encryption in less than a second.  In Eurocrypt 2015, pages 617-640. [<span>PDF</span>](https://eprint.iacr.org/2014/816.pdf){:target="_blank"}

[GSW13]:  C. Gentry, A. Sahai, and B. Waters. Homomorphic encryption from learning with errors:  Conceptually-simpler,  asymptotically-faster,  attribute-based. In Crypto 2013, pages 75-92. [<span>PDF</span>](https://eprint.iacr.org/2013/340.pdf){:target="_blank"}


### Future releases based on

[CGGI17]: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. Faster Packed Homomorphic Operations and Efficient Circuit Bootstrapping for TFHE. ASIACRYPT (1) 2017: 377-408. [<span>PDF</span>](https://eprint.iacr.org/2017/430.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"} 

[CGGI18]: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. TFHE: Fast Fully Homomorphic Encryption over the Torus. IACR Cryptology ePrint Archive 2018: 421 (2018) (Invited JoC). [<span>PDF</span>](https://eprint.iacr.org/2018/421.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"} 

[BGG18]: C. Boura, N. Gama, M. Georgieva: Chimera: a unified framework for B/FV, TFHE and HEAAN fully homomorphic encryption and predictions for deep learning. IACR Cryptology ePrint Archive 2018: 758 (2018). [<span>PDF</span>](https://eprint.iacr.org/2018/758.pdf){:target="_blank"}
[<span>Slides</span>](){:target="_blank"} 

[CIM19]: S. Carpov, M. Izabachène, V. Mollimard: New Techniques for Multi-value Input Homomorphic Evaluation and Applications. CT-RSA 2019: 106-126. [<span>PDF</span>](https://eprint.iacr.org/2018/622.pdf){:target="_blank"} 


### Applications and open source projects based on TFHE: 

[Google FHE]: Fully Homomorphic Encryption (FHE) [<span>github</span>](https://github.com/google/fully-homomorphic-encryption)
  
[Concrete]: Concrete Operates oN Ciphertexts Rapidly by Extending TfhE. [<span>github</span>](https://github.com/zama-ai/concrete)

[Cingulata]: Compilation toolchain and run-time environment targeting TFHE [<span>github</span>](https://github.com/CEA-LIST/Cingulata)

[CGGT-P19]:	S. Carpov, N. Gama, M. Georgieva, J.R. Troncoso-Pastoriza: Privacy-preserving semi-parallel logistic regression training with Fully Homomorphic Encryption.(among the winners Idash 2018)  IACR Cryptology ePrint Archive 2019: 101 (2019) [<span>PDF</span>](https://eprint.iacr.org/2019/101.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"} 

[CCS19]: H. Chen, I. Chillotti, Y. Song: Multi-Key Homomophic Encryption from TFHE. IACR Cryptology ePrint Archive 2019: 116 (2019). [<span>PDF</span>](https://eprint.iacr.org/2019/116.pdf){:target="_blank"}

[BMMP18]: F. Bourse, M. Minelli, M. Minihold, P. Paillier: Fast Homomorphic Evaluation of Deep Discretized Neural Networks. 
CRYPTO (3) 2018: 483-512. [<span>PDF</span>](https://eprint.iacr.org/2017/1114.pdf){:target="_blank"}

[CGGI16]: I. Chillotti, N. Gama, M. Georgieva, M. Izabachène: A Homomorphic LWE Based E-voting Scheme. PQCrypto 2016: 245-265. [<span>PDF</span>](https://ilachill.github.io/papers/CGGI16a-An_homomorphic_LWE_based_E-voting_Scheme.pdf){:target="_blank"} 
[<span>Slides</span>](https://pqcrypto2016.jp/data/Chillotti-Gama_Georgieva_Izabachene-An_Homomorphic_LWE-based_E-voting_Scheme-less-pauses.pdf
){:target="_blank"} 

[cuFHE]: CUDA-accelerated Fully Homomorphic Encryption Library: [<span>PDF</span>](https://github.com/vernamlab/cuFHE){:target="_blank"}

_(Please contact us to add your work based on TFHE)_


### Use of TFHE in the industry: 
* Inpher
* CryptoExperts
* NuCipher
* Zama











