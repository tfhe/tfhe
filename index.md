---
layout: default
title: TFHE Fast Fully Homomorphic Encryption over the Torus
---

# TFHE: Fast Fully Homomorphic Encryption over the Torus

<div>
{% for release in site.data.revisions %}
<strong>{{ release.name }}</strong> -- <em>{{ release.title }}</em>, date: {{ release.date }}<br>
{% endfor %}
</div>

TFHE is an open-source library for fully homomorphic encryption, 
distributed under the terms of the Apache 2.0 license. 

The underlying scheme is described in best paper of the IACR
conference Asiacrypt 2016: "*Faster fully homomorphic encryption: 
Bootstrapping in less than 0.1 seconds*", presented by 
Ilaria Chillotti, Nicolas Gama, Mariya Georgieva and Malika Izabachène.

### Description 

TFHE is a C/C++ library which implements a very fast gate-by-gate
bootstrapping, based on [CGGI16] and [CGGI17]. The library allows to evaluate an
arbitrary boolean circuit composed of binary gates, over encrypted data,
without revealing any information on the data.

The library supports the homomorphic evaluation of the 10 binary gates (And, Or, Xor, 
Nand, Nor, etc...), as well as the negation and the Mux gate. Each binary gate takes 
about 13 milliseconds single-core time to evaluate, which improves [DM15] by a factor 53, 
and the mux gate takes about 26 CPU-ms.

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
  binary gates homomorphically at a rate of about 76 gates per second per
  core, without decrypting its input. It suffices to provide the sequence
  of gates, as well as ciphertexts of the input bits. And the
  library computes ciphertexts of the output bits.

### Under the hood

The library implements a Ring-variant of the GSW [GSW13]
cryptosystem and makes many optimizations described in [DM15],
[CGGI16] and [CGGI17]. 

It also implements a dedicated Fast Fourier
Transformation for the anticyclic ring \\(\mathbb{R}[X]/(X^N+1)\\), and uses AVX
assembly vectorization instructions. 
The default parameter set achieves a 110-bit cryptographic security,
based on ideal lattice assumptions.


Since the running time per gate seems to be the bottleneck of fully
homomorphic encryption, an optimal circuits for TFHE is most likely a circuit 
with the smallest possible number of gates, and to a lesser extent, the 
possibility to evaluate them in parallel. 


### Dependencies 

The library interface can be used in a regular C code. However, to
compile the core of the library you will need a standard x86_64 C++11 compiler.
The project has been tested and reported to work with the GNU compiler 
g++/gcc (>=5.2) as well as the clang compiler (>=3.8) on both Linux and MacOS platforms.
Compilation under Windows is not supported at this time. 

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
* We also provide a connector for the FFTW3 library: http://www.fftw.org.
  With this library, the performance of the FFT is between 2 and 3 times
  faster than the default Nayuki implementation. However, you should keep
  in mind that the library FFTW is published under the GPL License. If you
  choose to use this library in a final product, this product may have to
  be released under GPL License as well (other commercial licenses are
  available on their web site)
* We plan to add other connectors in the future (for instance the Intel’s
  IPP Fourier Transform, which should be 1.5× faster than FFTW for 1D
  real data)


### References

`[CGGI16]`: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. Faster fully homomorphic encryption: Bootstrapping in less than 0.1 seconds. In Asiacrypt 2016 (Best Paper), pages 3-33.  [<span>PDF</span>](https://eprint.iacr.org/2016/870.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"}

`[DM15]`:   L. Ducas and D. Micciancio.  FHEW: Bootstrapping homomorphic encryption in less than a second.  In Eurocrypt 2015, pages 617-640. [<span>PDF</span>](https://eprint.iacr.org/2014/816.pdf){:target="_blank"}

`[GSW13]`:  C. Gentry, A. Sahai, and B. Waters. Homomorphic encryption from learning with errors:  Conceptually-simpler,  asymptotically-faster,  attribute-based. In Crypto 2013, pages 75-92. [<span>PDF</span>](https://eprint.iacr.org/2013/340.pdf){:target="_blank"}


### Future releases based on

`[CGGI17]`: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. Faster Packed Homomorphic Operations and Efficient Circuit Bootstrapping for TFHE. ASIACRYPT (1) 2017: 377-408. [<span>PDF</span>](https://eprint.iacr.org/2017/430.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"} 

`[CGGI18]`: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. TFHE: Fast Fully Homomorphic Encryption over the Torus. IACR Cryptology ePrint Archive 2018: 421 (2018) (Invited JoC). [<span>PDF</span>](https://eprint.iacr.org/2018/421.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"} 

`[BGG18]`: C. Boura, N. Gama, M. Georgieva: Chimera: a unified framework for B/FV, TFHE and HEAAN fully homomorphic encryption and predictions for deep learning. IACR Cryptology ePrint Archive 2018: 758 (2018). [<span>PDF</span>](https://eprint.iacr.org/2018/758.pdf){:target="_blank"}
[<span>Slides</span>](https://tfhe.github.io/tfhe/assets/pres/pdf/pres-chimera.pdf){:target="_blank"} 

`[CIM19]`: S. Carpov, M. Izabachène, V. Mollimard: New Techniques for Multi-value Input Homomorphic Evaluation and Applications. CT-RSA 2019: 106-126. [<span>PDF</span>](https://eprint.iacr.org/2018/622.pdf){:target="_blank"} 


### Applications and open source projects based on TFHE: 

`[CGGT-P19]`:	S. Carpov, N. Gama, M. Georgieva, J.R. Troncoso-Pastoriza: Privacy-preserving semi-parallel logistic regression training with Fully Homomorphic Encryption.(among the winners Idash 2018)  IACR Cryptology ePrint Archive 2019: 101 (2019) [<span>PDF</span>](https://eprint.iacr.org/2019/101.pdf){:target="_blank"} [<span>Slides</span>](){:target="_blank"} 

`[CCS19]`: H. Chen, I. Chillotti, Y. Song: Multi-Key Homomophic Encryption from TFHE. IACR Cryptology ePrint Archive 2019: 116 (2019). [<span>PDF</span>](https://eprint.iacr.org/2019/116.pdf){:target="_blank"}

`[BMMP18]`: F. Bourse, M. Minelli, M. Minihold, P. Paillier: Fast Homomorphic Evaluation of Deep Discretized Neural Networks. 
CRYPTO (3) 2018: 483-512. [<span>PDF</span>](https://eprint.iacr.org/2017/1114.pdf){:target="_blank"}

`[CGGI16]`: I. Chillotti, N. Gama, M. Georgieva, M. Izabachène: A Homomorphic LWE Based E-voting Scheme. PQCrypto 2016: 245-265. [<span>PDF</span>](https://ilachill.github.io/papers/CGGI16a-An_homomorphic_LWE_based_E-voting_Scheme.pdf){:target="_blank"} 
[<span>Slides</span>](https://pqcrypto2016.jp/data/Chillotti-Gama_Georgieva_Izabachene-An_Homomorphic_LWE-based_E-voting_Scheme-less-pauses.pdf
){:target="_blank"} 

`[cuFHE]`: CUDA-accelerated Fully Homomorphic Encryption Library: [<span>PDF</span>](https://github.com/vernamlab/cuFHE){:target="_blank"}

_(Please contact us to add your work based on TFHE)_


### Use of TFHE in the industry: 
* Inpher
* CryptoExperts
* NuCipher


