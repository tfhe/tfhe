-------------------------------------------------------
Fast Fourier transform in x86 assembly - Project Nayuki
-------------------------------------------------------


This library calculates the fast Fourier transform. It uses the same equation as the implementation on the page Free small FFT in multiple languages, and the FFT is unscaled. The key feature is that a hand-optimized assembly language implementation is provided for x86-64 AVX (supported by Intel Sandy Bridge CPUs and later).
Source code

    fft-test.c: Runnable main test program
    fft.h: Exported function prototypes for all FFT implementations
    fft-portable.c: FFT implementation for all CPU architectures
    fft-x8664-avx.s: FFT core implementation for x86-64 AVX
    fft-x8664-avx-aux.c: Auxiliary FFT functions for x86-64 AVX
    fft-model-of-x8664-avx.c: C code illustrating how the x86-64 AVX code works

To compile, use one of these commands:

    gcc -O1 -o fft-test fft-test.c fft-portable.c -lm
    gcc -O1 -o fft-test fft-test.c fft-x8664-avx-aux.c fft-x8664-avx.s -lm
    gcc -O1 -o fft-test fft-test.c fft-x8664-avx-aux.c fft-model-of-x8664-avx.c -lm

License: MIT (open source)

Source code notes:

    The x86-64 AVX implementation only covers the FFT core. The table initialization and destruction are still implemented in C (fft-x8664-avx-aux.c) because they’re not time-critical.

    The key idea behind the x86-64 AVX implementation is that all processing happens on vectors of four 64-bit floats, rather than individual scalar floats. In theory this could be 4× as fast as scalar code, but is limited by memory bandwidth, FPU execution resources, inherently serial loop control code, etc. Also, the cosine/sine tables are interleaved in blocks of 4 elements, and the table values for each FFT size are packed together so that the memory access stride is always contiguous.

    The portable C implementation is the same as that found on my free FFT page, except that the table computation is separated out to an initialization phase (fft_init()).

    As a bonus, the x86-64 AVX auxiliary C code has an implementation of a more accurate sine function by doing argument reduction to a smaller domain (1/8th of a circle instead of a half circle).

    The pure-C model of the x86-64 AVX code logic is in fact portable. The code exists to make it easier for the reader to understand what the AVX code is doing in a more familiar notation and without having to look up instruction names in the Intel manual.

Benchmark

For each implementation and FFT size, the number is the execution time in nanoseconds per iteration.
FFT size        C impl, -O0     C impl, -O1     C model of AVX, -O1     AVX impl        Speedup
4               81              33              15                      19              0.79×
8               195             66              35                      29              1.21×
16              468             134             87                      46              1.89×
32              1 087           278             210                     92              2.28×
64              2 500           594             494                     207             2.39×
128             5 638           1 298           1 138                   507             2.24×
256             13 542          2 949           2 634                   1 680           1.57×
512             29 910          6 997           6 560                   3 277           2.00×
1 024           65 865          15 715          15 005                  7 286           2.06×
2 048           144 452         36 746          33 859                  17 075          1.98×
4 096           317 598         86 162          80 733                  47 581          1.70×
8 192           687 521         198 965         189 643                 112 089         1.69×
16 384          1 497 747       405 952         410 043                 256 435         1.60×
32 768          3 330 258       1 055 371       928 485                 666 325         1.39×
65 536          7 282 305       2 426 037       1 951 704               1 435 606       1.36×
131 072         16 035 397      5 546 165       4 142 270               3 028 641       1.37×
262 144         34 795 636      11 811 311      8 978 397               6 598 887       1.36×
524 288         90 042 669      35 911 574      23 751 996              18 866 343      1.26×
1 048 576       270 490 724     151 551 517     71 873 200              65 876 359      1.09×
2 097 152       658 375 195     375 815 597     155 215 482             152 884 652     1.02×
4 194 304       1 361 986 528   782 913 943     329 475 964             323 074 038     1.02×
8 388 608       3 225 336 375   1 879 716 450   685 629 776             660 085 400     1.04×
16 777 216      7 300 920 662   4 457 742 453   1 427 182 753           1 379 910 712   1.03×
33 554 432      15 711 324 858  9 599 613 860   2 982 921 542           2 808 719 671   1.06×
67 108 864      34 594 570 323  21 420 158 806  6 379 494 508           5 967 280 476   1.07×

The benchmark results are a mixed bag, but a few conclusions can be drawn from the numbers:

    Compiling C code with optimizations enabled is obviously a good idea. The portable C FFT code is 1.5× to 4× faster when compiled with -O1 compared to -O0.

    The pure-C model of the AVX code has interesting behavior: It starts off being as slow as the portable C code, yet for large FFT sizes it approaches the speed of the true AVX implementation. This suggests that the AVX code does computations more efficiently than the C code due to vectorization. But it also suggests that at large sizes, the FFT is limited by memory bandwidth; furthermore the large access strides used in the portable C code (but not in the AVX code or its C model) is probably quite inefficient.

    The hand-coded x86-64 AVX code is from 1.0× to 2.4× faster than the best C code (which is the AVX model code). This speedup was not as impressive as hoped.

    All numbers are obtained on the Intel Xeon E3-1270 (Sandy Bridge) 3.40 GHz CPU. Each number is the mean of 30 trials, retrying if the standard deviation exceeded 0.1%.

Categories: Programming, C/C++, x86 assembly

Last updated: 2015-05-11

Copyright © Project Nayuki  
https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly
