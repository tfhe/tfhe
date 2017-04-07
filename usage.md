---
layout: page
title: TFHE Fast Fully Homomorphic Encryption over the Torus
---
[back: installation](installation.html) -
[home](index.html) -
[next: coding](coding.html) 

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
(gcc, g++, g77, ...). However, you **must always** link the final binary with a c++
compiler (like g++).

# More details

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


