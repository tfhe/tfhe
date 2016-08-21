# tfhe
Fast Fully Homomorphic Encryption Library over the Torus

**version 0.5** -- *Release date: 2016.08.18*

TFHE is open-source software distributed under the terms of the Apache 2.0 license. 
The scheme is described in the paper “Faster fully homomorphic encryption: Bootstrapping in less than 0.1 seconds” submitted at the IACR conference Asiacrypt 2016 by Ilaria Chillotti <> 
Nicolas Gama <>, Mariya Georgieva <>, Malika Izabachène <>.


### Dependencies 


The core of the library only depends on the standard C++11 library. However, the library interface can be used in a regular C code. 
Currently, the project has been tested with the g++ compiler under Linux. In the future, we plan to extend the compatibility to other compilers, platforms and operating systems.

At least one FFT processor is needed to run the project:

* The default processor comes from Project Nayuki, who proposes two implementations of the fast Fourier transform - one in portable C, and the other using the AVX assembly instructions. https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly
This component is licensed under the MIT license, and we added the code of the reverse FFT (both in C and in assembly).
* We also provide a connector for the FFTW3 library: http://www.fftw.org. With this library, the performance of the FFT is between 2 and 3 times faster than the default Nayuki implementation. However, you should keep in mind that the library FFTW is published under the GPL License. If you choose to use this library in a final product, this product will have to be under GPL License as well (other commercial licenses are available on their web site)
* We plan to add other connectors in the future (for instance the Intel’s IPP Fourier Transform, which should be 1.5× faster than FFTW for 1D real data)


### Installation

To build the library, just run ```make optim```.  This will produce a few test/example programs. 


The fast bootstrapping is shown in the program test-bootstrapping-fft2-optim, which generates a random bootstrapping key, a random message, performs a bootstrapping operation, and prints the execution time.

To test the library with the FFTW3 library, just run ```make optim FFT_PROCESSOR=FFTW_FFT_PROCESSOR```. 


