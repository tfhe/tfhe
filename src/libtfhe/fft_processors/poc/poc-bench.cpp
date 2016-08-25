#include <stdint.h>
#include <cmath>

#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <complex>

#include "poc-fft.h"

using namespace std;

namespace {

    bool very_close(const complex<double>& a,const complex<double>& b) {
	bool reps = (abs(a-b)<1e-5);
	if (!reps) {
	    cerr << "not close: " << a << " vs. " << b << endl;
	}
	return reps;
    }

    bool very_close(const double& a,const double& b) {
	bool reps = (abs(a-b)<1e-5);
	if (!reps) {
	    cerr << "not close: " << a << " vs. " << b << endl;
	}
	return reps;
    }


    void require(bool condition, const string& message="[condition]") {
	if (!condition) {
	    cerr << "unmet condition: " << condition << endl;
	    abort();
	}
    }
}

int main(int argc, char** argv) {
    static const int nn = 1024;
    void* tables = new_fft_table(nn);
    void* itables = new_ifft_table(nn);
    double* a = new double[nn];
    double* a2 = new double[nn];
    double* b = new double[nn];
    for (int i=0; i<nn; i++) a[i]=i;
    for (int i=0; i<nn; i++) a2[i]=i;
    for (int i=0; i<nn; i++) b[i]=a[i];
    ifft_model(itables,a);
    ifft(itables,a2);
    for (int i=0; i<nn; i++) 
	require(very_close(a[i],a2[i]));
    fft_model(tables,a);
    fft(tables,a2);
    for (int i=0; i<nn; i++) 
	require(very_close(a[i],a2[i]));
    for (int i=0; i<nn; i++) 
	require(very_close(a[i],nn/2*b[i]));

#ifdef NDEBUG
    int nbtrials = 10000;
    long begin = clock();
    for (int trials=0; trials<nbtrials; trials++)
	fft(tables,a);
    long end = clock();
    cout << "fft_time " << double(end-begin)/nbtrials << endl;
    begin = clock();
    for (int trials=0; trials<nbtrials; trials++)
	ifft(tables,a);
    end = clock();
    cout << "ifft_time " << double(end-begin)/nbtrials << endl;
    begin = clock();
    for (int trials=0; trials<nbtrials; trials++)
	fft_model(tables,a);
    end = clock();
    cout << "fft_model_time " << double(end-begin)/nbtrials << endl;
    begin = clock();
    for (int trials=0; trials<nbtrials; trials++)
	ifft_model(tables,a);
    end = clock();
    cout << "ifft_model_time " << double(end-begin)/nbtrials << endl;
#endif
}


