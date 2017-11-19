/* 
 * Fast Fourier transform for x86-64 AVX (C)
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

// M_PI isn't defined with C99 for whatever reason
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Private data structure
struct FftTables {
	uint64_t n;
	uint64_t *bit_reversed;
	double *trig_tables;
};


// Private function prototypes
static double accurate_sine(uint64_t i, uint64_t n);
static int32_t floor_log2(uint64_t n);
static uint64_t reverse_bits(uint64_t x, uint32_t n);


/*---- Function implementations ----*/

// Returns a pointer to an opaque structure of FFT tables. n must be a power of 2 and n >= 4.
void *fft_init(size_t n) {
	// Check size argument
	if (n < 4 || n > UINT64_MAX || (n & (n - 1)) != 0)
		return NULL;  // Error: Size is too small or is not a power of 2
	if (n - 4 > SIZE_MAX / sizeof(double) / 2 || n > SIZE_MAX / sizeof(size_t))
		return NULL;  // Error: Size is too large, which makes memory allocation impossible
	
	// Allocate structure
	struct FftTables *tables = malloc(sizeof(struct FftTables));
	if (tables == NULL)
		return NULL;
	tables->n = n;
	
	// Allocate arrays
	tables->bit_reversed = malloc(n * sizeof(size_t));
	tables->trig_tables = malloc((n - 4) * 2 * sizeof(double));
	if (tables->bit_reversed == NULL || tables->trig_tables == NULL) {
		free(tables->bit_reversed);
		free(tables->trig_tables);
		free(tables);
		return NULL;
	}
	
	// Precompute bit reversal table
	int32_t levels = floor_log2(n);
	uint64_t i;
	for (i = 0; i < n; i++)
		tables->bit_reversed[i] = reverse_bits(i, levels);
	
	// Precompute the packed trigonometric table for each FFT internal level
	uint64_t size;
	uint64_t k = 0;
	for (size = 8; size <= n; size *= 2) {
		for (i = 0; i < size / 2; i += 4) {
			uint64_t j;
			for (j = 0; j < 4; j++, k++)
				tables->trig_tables[k] = accurate_sine(i + j + size / 4, size);  // Cosine
			for (j = 0; j < 4; j++, k++)
				tables->trig_tables[k] = accurate_sine(i + j, size);  // Sine
		}
		if (size == n)
			break;
	}
	return tables;
}

// Returns a pointer to an opaque structure of FFT tables. n must be a power of 2 and n >= 4.
void *fft_init_reverse(size_t n) {
	// Check size argument
	if (n < 4 || n > UINT64_MAX || (n & (n - 1)) != 0)
		return NULL;  // Error: Size is too small or is not a power of 2
	if (n - 4 > SIZE_MAX / sizeof(double) / 2 || n > SIZE_MAX / sizeof(size_t))
		return NULL;  // Error: Size is too large, which makes memory allocation impossible
	
	// Allocate structure
	struct FftTables *tables = malloc(sizeof(struct FftTables));
	if (tables == NULL)
		return NULL;
	tables->n = n;
	
	// Allocate arrays
	tables->bit_reversed = malloc(n * sizeof(size_t));
	tables->trig_tables = malloc((n - 4) * 2 * sizeof(double));
	if (tables->bit_reversed == NULL || tables->trig_tables == NULL) {
		free(tables->bit_reversed);
		free(tables->trig_tables);
		free(tables);
		return NULL;
	}
	
	// Precompute bit reversal table
	int32_t levels = floor_log2(n);
	uint64_t i;
	for (i = 0; i < n; i++)
		tables->bit_reversed[i] = reverse_bits(i, levels);
	
	// Precompute the packed trigonometric table for each FFT internal level
	uint64_t size;
	uint64_t k = 0;
	for (size = 8; size <= n; size *= 2) {
		for (i = 0; i < size / 2; i += 4) {
			uint64_t j;
			for (j = 0; j < 4; j++, k++)
				tables->trig_tables[k] = accurate_sine(i + j + size / 4, size);  // Cosine
			for (j = 0; j < 4; j++, k++)
				tables->trig_tables[k] = -accurate_sine(i + j, size);  // Sine
		}
		if (size == n)
			break;
	}
	return tables;
}



// Deallocates the given structure of FFT tables.
void fft_destroy(void *tables) {
	if (tables == NULL)
		return;
	struct FftTables *tbl = (struct FftTables *)tables;
	free(tbl->bit_reversed);
	free(tbl->trig_tables);
	memset(tbl, 0, sizeof(struct FftTables));  // Prevent accidental memory reuse
	free(tbl);
}


// Returns sin(2 * pi * i / n), for n that is a multiple of 4.
static double accurate_sine(uint64_t i, uint64_t n) {
	if (n % 4 != 0)
		return NAN;
	else {
		int32_t neg = 0;  // Boolean
		// Reduce to full cycle
		i %= n;
		// Reduce to half cycle
		if (i >= n / 2) {
			neg = 1;
			i -= n / 2;
		}
		// Reduce to quarter cycle
		if (i >= n / 4)
			i = n / 2 - i;
		// Reduce to eighth cycle
		double val;
		if (i * 8 < n)
			val = sin(2 * M_PI * i / n);
		else
			val = cos(2 * M_PI * (n / 4 - i) / n);
		// Apply sign
		return neg ? -val : val;
	}
}


// Returns the largest i such that 2^i <= n.
static int32_t floor_log2(uint64_t n) {
	int32_t result = 0;
	for (; n > 1; n /= 2)
		result++;
	return result;
}


// Returns the bit reversal of the n-bit unsigned integer x.
static uint64_t reverse_bits(uint64_t x, uint32_t n) {
	uint64_t result = 0;
	uint32_t i;
	for (i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1);
	return result;
}
