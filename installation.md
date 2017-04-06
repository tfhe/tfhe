---
layout: page
title: TFHE Fast Fully Homomorphic Encryption over the Torus
---
[back: homepage](index.html) -
[next: installation](coding.html) 

# Installing the library

## Prerequisites

The code of the library contains various optimizations which highly depend on
your cpu infrastructure. Therefore it is recommended to compile an optimized 
version of the library directly on the cloud environment where homomorphic
computations will be run. 

To build the library, you need at least cmake is already installed on the
system, and an up-to-date c++ compiler (i.e. g++ >=5.2) as well.

On Debian/Ubuntu, you can get these tools by issuing:

~~~sh
sudo apt-get install build-essential cmake cmake-curses-gui
~~~

If you plan to compile the test suite, or to use the optional fftw processor,
you will need to install other optional dependencies, described in the advanced
section below. 

## Configuring and building the library

Get the latest version of the library either from the archive download link above, or from the git repository:

~~~sh
#clone the tfhe repository
git clone https://github.com/tfhe/tfhe.git
cd tfhe
~~~

Then create an empty build folder and configure the build options, for instance
with the ```ccmake``` configuration tool

~~~sh
#configure the build options
mkdir build
cd build
ccmake ../src
~~~

If you have a recent Core i7 processor, enabling the SPQLIOS_FMA version in
optimized mode is very likely to be the fastest results. If you have an older
CPU that do not support the latest FMA opcodes, you can revert to other fft
processors. The nayuki-portable processor is slower, but will run on any
hardware.

Currently, only static libraries are generated. The default installation prefix
(```CMAKE_INSTALL_PREFIX```) is ```/usr/local```. After compilation, all static libraries 
will be installed to ```CMAKE_INSTALL_PREFIX/lib```, and headers will be installed to  
```CMAKE_INSTALL_PREFIX/include```

If you want to choose different or additional compile options (i.e. debug mode, 
tests, fftw), you need to run cmake manually and pass the desired options, which 
are described in the advanced section below.

Once you exit the configuration tool, you can build and install the library.

~~~sh
#build the library
make

#install the library
make install #(with sudo if the install prefix requires root) 
~~~

## Using the library

To use the library, you need to tell your compiler how to find the recently installed library and headers. 
If you are using gcc/g++, you can do it on the current shell by setting these environment variables.

~~~sh
#configure environment variables
TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
export LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
~~~

To make these changes persistent, you may define these variables in your
```.bashrc``` or ```.profile``` file.

To compile a C++ file on the terminal that knows these environment variables,
you can do

~~~sh
# compile a c++ source code
g++ your-program.cpp -o your-program -ltfhe-spqlios-fma 
~~~

Finally, since tfhe uses c++ functions, even if you write C code that you separately compile with gcc, 
you will need to link the final executable with g++. For instance,

~~~sh
# compile a c source code
gcc -c your-program.c -o your-program.o
g++ your-program.o -o your-program -ltfhe-spqlios-fma 
~~~

## advanced options

The available build options are the following:

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


