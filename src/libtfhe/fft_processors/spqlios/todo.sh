CFLAGS="-g3 -O0 -Wall -DNDEBUG"
CFLAGS="-g0 -O2 -DNDEBUG"

g++ -c $CFLAGS -o spqlios-bench.o spqlios-bench.cpp
g++ -c $CFLAGS -o spqlios-fft-impl.o spqlios-fft-impl.cpp
g++ -c $CFLAGS -o spqlios-fft.o spqlios-fft.s
g++ -c $CFLAGS -o spqlios-ifft.o spqlios-ifft.s
g++ $CFLAGS -o test spqlios-bench.o spqlios-fft-impl.o spqlios-fft.o spqlios-ifft.o
