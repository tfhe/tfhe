all:
	gcc -O2 fft-test.c fft-x8664-avx-aux.c fft-x8664-avx.s -o fft-avx -lm
	gcc -O2 fft-test.c fft-x8664-avx-aux.c fft-model-of-x8664-avx.c -o fft-avx-model -lm
	gcc -O2 fft-test.c fft-portable.c -o fft-portable -lm
