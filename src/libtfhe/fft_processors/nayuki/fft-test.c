/* 
 * Fast Fourier transform test
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

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "fft.h"


// Private function prototypes
static double test_fft_log_error(int32_t n);
static void naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int32_t inverse, int32_t n);
static int64_t benchmark_time(const void *fttTables, double *real, double *imag, uint64_t iterations);
static double log10_rms_err(const double *xreal, const double *ximag, const double *yreal, const double *yimag, int32_t n);
static double *random_reals(int32_t n);
static void *memdup(const void *src, size_t n);


/*---- Function implementations ----*/

int32_t main(int32_t argc, char **argv) {
	// Self-test to check correct computation of values
	srand(time(NULL));
	int32_t i;
	for (i = 2; i <= 10; i++) {  // Test FFT sizes 4, 8, 16, ..., 512, 1024
		if (test_fft_log_error(1 << i) > -10) {
			printf("Self-test failed\n");
			return 1;
		}
	}
	printf("Self-test passed\n");
	
	// Speed benchmark
	const int64_t TARGET_TIME = 100000000;  // In nanoseconds
	const int32_t TRIALS = 10;
	printf("%9s    %s\n", "Size", "Time per FFT (ns)");
	size_t n;
	for (n = 4; n <= (size_t)1 << 26; n *= 2) {
		// Initialize data sets
		void *fftTables = fft_init(n);
		double *real = random_reals(n);
		double *imag = random_reals(n);
		if (fftTables == NULL || real == NULL || imag == NULL) {
			printf("Memory allocation failed\n");
			return 1;
		}
		
		// Determine number of iterations to run to spend TARGET_TIME
		uint64_t iterations = 1;
		while (1) {
			int64_t time = benchmark_time(fftTables, real, imag, iterations);
			if (time >= TARGET_TIME) {
				iterations = (uint64_t)((double)TARGET_TIME / time * iterations + 0.5);
				if (iterations == 0)
					iterations = 1;
				break;
			}
			iterations *= 2;
		}
		
		// Run trials and store timing
		double *runtimes = malloc(TRIALS * sizeof(double));
		int32_t i;
		for (i = 0; i < TRIALS; i++)
			runtimes[i] = (double)benchmark_time(fftTables, real, imag, iterations) / iterations;
		fft_destroy(fftTables);
		free(real);
		free(imag);
		
		// Compute statistics
		double min = 1e300;
		double sum = 0;
		for (i = 0; i < TRIALS; i++) {
			double t = runtimes[i];
			if (t < min)
				min = t;
			sum += t;
		}
		double mean = sum / TRIALS;
		double sqrdiffsum = 0;
		for (i = 0; i < TRIALS; i++) {
			double t = runtimes[i];
			sqrdiffsum += (t - mean) * (t - mean);
		}
		double stddev = sqrt(sqrdiffsum / TRIALS);
		free(runtimes);
		printf("%9zu    min=%"PRIu64"  mean=%"PRIu64"  sd=%.2f%%\n",
			n, (uint64_t)(min + 0.5), (uint64_t)(mean + 0.5), stddev / mean * 100);
	}
	return 0;
}


// Tests the FFT implementation against the naive DFT, returning the base-10 logarithm of the RMS error. This number should be less than -10.
static double test_fft_log_error(int32_t n) {
	double *inputreal, *inputimag;
	double *refoutreal, *refoutimag;
	double *actualoutreal, *actualoutimag;
	
	inputreal = random_reals(n);
	inputimag = random_reals(n);
	
	refoutreal = malloc(n * sizeof(double));
	refoutimag = malloc(n * sizeof(double));
	naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, n);
	
	actualoutreal = memdup(inputreal, n * sizeof(double));
	actualoutimag = memdup(inputimag, n * sizeof(double));
	void *fftTables = fft_init(n);
	if (fftTables == NULL)
		return 99;
	fft_transform(fftTables, actualoutreal, actualoutimag);
	fft_destroy(fftTables);
	double result = log10_rms_err(refoutreal, refoutimag, actualoutreal, actualoutimag, n);
	
	free(inputreal);
	free(inputimag);
	free(refoutreal);
	free(refoutimag);
	free(actualoutreal);
	free(actualoutimag);
	return result;
}


// Computes the discrete Fourier transform using the naive O(n^2) time algorithm.
static void naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int32_t inverse, int32_t n) {
	double coef = (inverse ? 2 : -2) * M_PI;
	int32_t k;
	for (k = 0; k < n; k++) {  // For each output element
		double sumreal = 0;
		double sumimag = 0;
		int32_t t;
		for (t = 0; t < n; t++) {  // For each input element
			double angle = coef * ((int64_t)t * k % n) / n;
			sumreal += inreal[t]*cos(angle) - inimag[t]*sin(angle);
			sumimag += inreal[t]*sin(angle) + inimag[t]*cos(angle);
		}
		outreal[k] = sumreal;
		outimag[k] = sumimag;
	}
}


// Returns the number of nanoseconds to run the given number of iterations of the given FFT size.
static int64_t benchmark_time(const void *fftTables, double *real, double *imag, uint64_t iterations) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	int64_t starttime = ts.tv_sec * INT64_C(1000000000) + ts.tv_nsec;
	uint64_t i;
	for (i = 0; i < iterations; i++)
		fft_transform(fftTables, real, imag);
	clock_gettime(CLOCK_REALTIME, &ts);
	int64_t endtime = ts.tv_sec * INT64_C(1000000000) + ts.tv_nsec;
	return endtime - starttime;
}


// Returns log10(sqrt(sum(|x[i] - y[i]|^2) / n)).
static double log10_rms_err(const double *xreal, const double *ximag, const double *yreal, const double *yimag, int32_t n) {
	double err = 0;
	int32_t i;
	for (i = 0; i < n; i++)
		err += (xreal[i] - yreal[i]) * (xreal[i] - yreal[i]) + (ximag[i] - yimag[i]) * (ximag[i] - yimag[i]);
	
	err /= n > 0 ? n : 1;
	err = sqrt(err);  // Now this is a root mean square (RMS) error
	err = err > 0 ? log10(err) : -99.0;
	return err;
}


// Allocates and fills an array of random doubles in the range [-1.0, 1.0]. Must be free()'d by the caller.
static double *random_reals(int32_t n) {
	double *result = malloc(n * sizeof(double));
	int32_t i;
	for (i = 0; i < n; i++)
		result[i] = (rand() / (RAND_MAX + 1.0)) * 2 - 1;
	return result;
}


// Allocates a new chunk of memory with the same contents as the given chunk. Must be free()'d by the caller.
static void *memdup(const void *src, size_t n) {
	void *dest = malloc(n);
	if (dest != NULL)
		memcpy(dest, src, n);
	return dest;
}
