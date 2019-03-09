---
layout: page
title: TFHE Fast Fully Homomorphic Encryption over the Torus
previousPage: index.html
nextPage: usage.html
---

# Installing the library

## Prerequisites

The code of the library contains various optimizations which highly depend on
your cpu infrastructure. Therefore it is recommended to compile an optimized 
version of the library directly on the cloud environment where homomorphic
computations will be run. 

To build the library, you need at least cmake is already installed on the
system, and an up-to-date c++ compiler (i.e. g++ >=5.2 or clang >= 3.8) as well.

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
git clone --recurse-submodules --branch=master https://github.com/tfhe/tfhe.git
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

From the configuration tool, you can choose your installation folder
(```CMAKE_INSTALL_PREFIX```). The default is ```/usr/local```. 
After compilation, all libraries will be installed to 
```CMAKE_INSTALL_PREFIX/lib```, and headers will be installed to  
```CMAKE_INSTALL_PREFIX/include```.

You may then configure which fft processor to use. If you have a recent Core i7 
processor, enabling the SPQLIOS_FMA version in optimized mode whil likely 
give you the fastest running times. If you have an older
CPU that does not support the latest FMA opcodes, you can revert to other fft
processors. The nayuki-portable processor is slower, but will run on any
hardware.

You may choose different or additional compile options (i.e. debug mode, 
tests, fftw), which are described in the advanced section at the bottom
of this page.

Once you exit the configuration tool (by choosing configure and generate of course),
you can build and install the library.

~~~sh
#build the library
make

#install the library
make install #(with sudo if the install prefix requires root) 
~~~

Once the installation is finished, you can start [using the library](usage.html).


## Advanced options (for expert users)

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
      <td><em>on/off</em> compiles the library’s unit tests and sample applications in the test/ folder.<br>
<strong>Important:</strong> Please run <code>git submodule init; git submodule update</code> to checkout the last version of 
googletest before running cmake if you plan to compile the tests.<br>
(Then, after compilation, use <code class="highlighter-rouge">ctest</code> to run all unittests)</td>
    </tr>
    <tr>
      <td>ENABLE_FFTW</td>
      <td><em>on/off</em> compiles libtfhe-fftw.so, using FFTW3 (GPL licence) for fast FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_NAYUKI_PORTABLE</td>
      <td><em>on/off</em> compiles libtfhe-nayuki-portable.so, using the fast C version of nayuki for FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_NAYUKI_AVX</td>
      <td><em>on/off</em> compiles libtfhe-nayuki-avx.so, using the avx assembly version of nayuki for FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_SPQLIOS_AVX</td>
      <td><em>on/off</em> compiles libtfhe-spqlios-avx.so, using tfhe’s dedicated avx assembly version for FFT computations</td>
    </tr>
    <tr>
      <td>ENABLE_SPQLIOS_FMA</td>
      <td><em>on/off</em> compiles libtfhe-spqlios-fma.so, using tfhe’s dedicated fma assembly version for FFT computations</td>
    </tr>
  </tbody>
</table>


