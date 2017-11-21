/* 
 * Fast Fourier transform (C)
 * 
 * Copyright (c) 2016 Project Nayuki
 * https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly
 * 
 * (MIT License)
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fft.h"


// Private data structure
struct FftTables {
	size_t n;
	size_t *bit_reversed;
	double *cos_table;
	double *sin_table;
};


// Private function prototypes
static int32_t floor_log2(size_t n);
static size_t reverse_bits(size_t x, uint32_t n);


/*---- Function implementations ----*/

// Returns a pointer to an opaque structure of FFT tables. n must be a power of 2.
void *fft_init(size_t n) {
	// Check size argument
	if (n <= 0 || (n & (n - 1)) != 0)
		return NULL;  // Error: Size is not a power of 2
	if (n / 2 > SIZE_MAX / sizeof(double) || n > SIZE_MAX / sizeof(size_t))
		return NULL;  // Error: Size is too large, which makes memory allocation impossible
	
	// Allocate structure
	struct FftTables *tables = malloc(sizeof(struct FftTables));
	if (tables == NULL)
		return NULL;
	tables->n = n;
	
	// Allocate arrays
	tables->bit_reversed = malloc(n * sizeof(size_t));
	tables->cos_table = malloc(n / 2 * sizeof(double));
	tables->sin_table = malloc(n / 2 * sizeof(double));
	if (tables->bit_reversed == NULL || tables->cos_table == NULL || tables->sin_table == NULL) {
		free(tables->bit_reversed);
		free(tables->cos_table);
		free(tables->sin_table);
		free(tables);
		return NULL;
	}
	
	// Precompute values and store to tables
	size_t i;
	int32_t levels = floor_log2(n);
	for (i = 0; i < n; i++)
		tables->bit_reversed[i] = reverse_bits(i, levels);
	for (i = 0; i < n / 2; i++) {
		double angle = 2 * M_PI * i / n;
		tables->cos_table[i] = cos(angle);
		tables->sin_table[i] = sin(angle);
	}
	return tables;
}


// Performs a forward FFT in place on the given arrays. The length is given by the tables struct.
void fft_transform(const void *tables, double *real, double *imag) {
	struct FftTables *tbl = (struct FftTables *)tables;
	size_t n = tbl->n;
	
	// Bit-reversed addressing permutation
	size_t *bitreversed = tbl->bit_reversed;
	size_t i;
	for (i = 0; i < n; i++) {
		size_t j = bitreversed[i];
		if (i < j) {
			double tp0re = real[i];
			double tp0im = imag[i];
			double tp1re = real[j];
			double tp1im = imag[j];
			real[i] = tp1re;
			imag[i] = tp1im;
			real[j] = tp0re;
			imag[j] = tp0im;
		}
	}
	
	// Cooley-Tukey decimation-in-time radix-2 FFT
	double *costable = tbl->cos_table;
	double *sintable = tbl->sin_table;
	size_t size;
	for (size = 2; size <= n; size *= 2) {
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (i = 0; i < n; i += size) {
			size_t j, k;
			for (j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
				double tpre =  real[j+halfsize] * costable[k] + imag[j+halfsize] * sintable[k];
				double tpim = -real[j+halfsize] * sintable[k] + imag[j+halfsize] * costable[k];
				real[j + halfsize] = real[j] - tpre;
				imag[j + halfsize] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}


// Deallocates the given structure of FFT tables.
void fft_destroy(void *tables) {
	if (tables == NULL)
		return;
	struct FftTables *tbl = (struct FftTables *)tables;
	free(tbl->bit_reversed);
	free(tbl->cos_table);
	free(tbl->sin_table);
	memset(tbl, 0, sizeof(struct FftTables));  // Prevent accidental memory reuse
	free(tbl);
}


// Returns the largest i such that 2^i <= n.
static int32_t floor_log2(size_t n) {
	int32_t result = 0;
	for (; n > 1; n /= 2)
		result++;
	return result;
}


// Returns the bit reversal of the n-bit unsigned integer x.
static size_t reverse_bits(size_t x, uint32_t n) {
	size_t result = 0;
	uint32_t i;
	for (i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1);
	return result;
}
