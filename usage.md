---
layout: page
title: TFHE Fast Fully Homomorphic Encryption over the Torus
previousPage: installation.html
nextPage: gate-bootstrapping-api.html
---

# Using the library

In short, you need these includes to use TFHE 

~~~c++
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
~~~

And you need to link your program with ```-ltfhe-spqlios-fma``` (provided
that you compiled the SPQLIOS_FMA version of tfhe)

All the external API use C naming convention, which means that you can use 
any language to code your functions (C, C++, Fortran, ...) 
and do separate compilation with the corresponding compiler
(gcc, g++, g77, ...).

# More details

To use the library, you need to tell your compiler how to find the recently installed library and headers. 
If you are using gcc/g++, you can do it on the current shell by setting these environment variables.

~~~sh
#configure environment variables
TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
export LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib
~~~

To make these changes persistent, you may define these variables in your
```.bashrc``` or ```.profile``` file.

To compile a C++ file on the terminal that knows these environment variables,
you can do

~~~sh
# compile a c++ source code
g++ your-program.cpp -o your-program -ltfhe-spqlios-fma 
~~~

If you have a C source file, you can do:

~~~sh
# compile a c source code
gcc your-program.c -o -ltfhe-spqlios-fma 
~~~


