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

#include <stdint.h>
#include <stdlib.h>
#include "fft.h"


// Private data structure
struct FftTables {
	uint64_t n;
	uint64_t *bit_reversed;
	double *trig_tables;
};


/*---- Function implementations ----*/

// This is a C implementation that models the x86-64 AVX implementation.
void fft_transform(const void *tables, double *real, double *imag) {
	struct FftTables *tbl = (struct FftTables *)tables;
	uint64_t n = tbl->n;
	
	// Bit-reversed addressing permutation
	uint64_t i;
	uint64_t *bitreversed = tbl->bit_reversed;
	for (i = 0; i < n; i++) {
		uint64_t j = bitreversed[i];
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
	
	// Size 2 merge (special)
	if (n >= 2) {
		for (i = 0; i < n; i += 2) {
			double tpre = real[i];
			double tpim = imag[i];
			real[i] += real[i + 1];
			imag[i] += imag[i + 1];
			real[i + 1] = tpre - real[i + 1];
			imag[i + 1] = tpim - imag[i + 1];
		}
	}
	
	// Size 4 merge (special)
	if (n >= 4) {
		for (i = 0; i < n; i += 4) {
			// Even indices
			double tpre, tpim;
			tpre = real[i];
			tpim = imag[i];
			real[i] += real[i + 2];
			imag[i] += imag[i + 2];
			real[i + 2] = tpre - real[i + 2];
			imag[i + 2] = tpim - imag[i + 2];
			// Odd indices
			tpre = real[i + 1];
			tpim = imag[i + 1];
			real[i + 1] += imag[i + 3];
			imag[i + 1] -= real[i + 3];
			tpre -= imag[i + 3];
			tpim += real[i + 3];
			real[i + 3] = tpre;
			imag[i + 3] = tpim;
		}
	}
	
	// Size 8 and larger merges (general)
	double *trigtables = tbl->trig_tables;
	uint64_t size;
	for (size = 8; size <= n; size <<= 1) {
		uint64_t halfsize = size >> 1;
		uint64_t i;
		for (i = 0; i < n; i += size) {
			uint64_t j, off;
			for (j = 0, off = 0; j < halfsize; j += 4, off += 8) {
				uint64_t k;
				for (k = 0; k < 4; k++) {  // To simulate x86 AVX 4-vectors
					uint64_t vi = i + j + k;  // Vector index
					uint64_t ti = off + k;    // Table index
					double re = real[vi + halfsize];
					double im = imag[vi + halfsize];
					double tpre = re * trigtables[ti] + im * trigtables[ti + 4];
					double tpim = im * trigtables[ti] - re * trigtables[ti + 4];
					real[vi + halfsize] = real[vi] - tpre;
					imag[vi + halfsize] = imag[vi] - tpim;
					real[vi] += tpre;
					imag[vi] += tpim;
				}
			}
		}
		if (size == n)
			break;
		trigtables += size;
	}
}


// This is a C implementation that models the x86-64 AVX implementation.
void fft_transform_reverse(const void *tables, double *real, double *imag) {
	struct FftTables *tbl = (struct FftTables *)tables;
	uint64_t n = tbl->n;
	
	// Bit-reversed addressing permutation
	uint64_t i;
	uint64_t *bitreversed = tbl->bit_reversed;
	for (i = 0; i < n; i++) {
		uint64_t j = bitreversed[i];
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
	
	// Size 2 merge (special)
	if (n >= 2) {
		for (i = 0; i < n; i += 2) {
			double tpre = real[i];
			double tpim = imag[i];
			real[i] += real[i + 1];
			imag[i] += imag[i + 1];
			real[i + 1] = tpre - real[i + 1];
			imag[i + 1] = tpim - imag[i + 1];
		}
	}
	
	// Size 4 merge (special)
	if (n >= 4) {
		for (i = 0; i < n; i += 4) {
			// Even indices
			double tpre, tpim;
			tpre = real[i];
			tpim = imag[i];
			real[i] += real[i + 2];
			imag[i] += imag[i + 2];
			real[i + 2] = tpre - real[i + 2];
			imag[i + 2] = tpim - imag[i + 2];
			// Odd indices
			tpre = real[i + 1];
			tpim = imag[i + 1];
			real[i + 1] -= imag[i + 3];
			imag[i + 1] += real[i + 3];
			tpre += imag[i + 3];
			tpim -= real[i + 3];
			real[i + 3] = tpre;
			imag[i + 3] = tpim;
		}
	}
	
	// Size 8 and larger merges (general)
	double *trigtables = tbl->trig_tables;
	uint64_t size;
	for (size = 8; size <= n; size <<= 1) {
		uint64_t halfsize = size >> 1;
		uint64_t i;
		for (i = 0; i < n; i += size) {
			uint64_t j, off;
			for (j = 0, off = 0; j < halfsize; j += 4, off += 8) {
				uint64_t k;
				for (k = 0; k < 4; k++) {  // To simulate x86 AVX 4-vectors
					uint64_t vi = i + j + k;  // Vector index
					uint64_t ti = off + k;    // Table index
					double re = real[vi + halfsize];
					double im = imag[vi + halfsize];
					double tpre = re * trigtables[ti] + im * trigtables[ti + 4];
					double tpim = im * trigtables[ti] - re * trigtables[ti + 4];
					real[vi + halfsize] = real[vi] - tpre;
					imag[vi + halfsize] = imag[vi] - tpim;
					real[vi] += tpre;
					imag[vi] += tpim;
				}
			}
		}
		if (size == n)
			break;
		trigtables += size;
	}
}
