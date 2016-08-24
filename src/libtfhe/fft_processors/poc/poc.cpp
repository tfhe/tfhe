#include <stdint.h>
#include <cmath>

#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <complex>

using namespace std;


//reverse the bits of i
int rev(int i, int n) {
    int reps=0;
    for (int j=1; j<n; j*=2) {
	reps = 2*reps + (i%2);
	i/=2;
    }
    return reps;
}

bool very_close(const complex<double>& a,const complex<double>& b) {
    bool reps = (abs(a-b)<1e-5);
    if (!reps) {
	cerr << "not close: " << a << " vs. " << b << endl;
    }
    return reps;
}



typedef struct  {
    uint64_t n;
    double* trig_tables;
} IFFT_PRECOMP;

//trig_tables:
//|cos0,cos1,cos2,cos3|sin0,sin1,sin2,sin3|cos4,cos5,cos6,cos7|.... -> n/4
//|cos0,cos2,cos4,cos6|sin0,sin2,sin4,sin6|cos8,cos10,cos12,cos14|.... -> n/4
//...
//|cos0,cos2,cos4,cos6|sin0,sin2,sin4,sin6|cos8,cos10,cos12,cos14|.... -> n/4

inline void dotp4(double* __restrict res, const double*  __restrict a, const double*  __restrict b) {
    for (int i=0; i<4; i++) res[i]=a[i]*b[i];
}

inline void add4(double*  __restrict res, const double*  __restrict a, const double*  __restrict b) {
    for (int i=0; i<4; i++) res[i]=a[i]+b[i];
}

inline void sub4(double*  __restrict res, const double*  __restrict a, const double*  __restrict b) {
    for (int i=0; i<4; i++) res[i]=a[i]-b[i];
}

inline void copy4(double*  __restrict res, const double*  __restrict a) {
    for (int i=0; i<4; i++) res[i]=a[i];
}


void require(bool condition, const string& message="[condition]") {
    if (!condition) {
	cerr << "unmet condition: " << condition << endl;
	abort();
    }
}

double accurate_cos(int i,int n) { //cos(2pi*i/n)
    i = ((i%n) + n)%n;
    if (i>=3*n/4) return cos(2.*M_PI*(n-i)/double(n));
    if (i>=2*n/4) return -cos(2.*M_PI*(i-n/2)/double(n));
    if (i>=1*n/4) return -cos(2.*M_PI*(n/2-i)/double(n));
    return cos(2.*M_PI*(i)/double(n));
}

double accurate_sin(int i,int n) { //sin(2pi*i/n)
    i = ((i%n) + n)%n;
    if (i>=3*n/4) return -sin(2.*M_PI*(n-i)/double(n));
    if (i>=2*n/4) return -sin(2.*M_PI*(i-n/2)/double(n));
    if (i>=1*n/4) return sin(2.*M_PI*(n/2-i)/double(n));
    return sin(2.*M_PI*(i)/double(n));
}

void* new_ifft_table(int nn) {
    require(nn >= 16, "n must be >=16");
    require((nn & (nn-1)) == 0, "n must be a power of 2");
    int n = 2*nn;
    int ns4 = n/4;
    IFFT_PRECOMP* reps = new IFFT_PRECOMP;
    assert(((uint64_t)reps)%32==0); //verify alignment
    reps->n = n;
    reps->trig_tables = new double[ns4*2*2];
    double* ptr = reps->trig_tables;
    //first iteration
    for (int j=0; j<ns4; j+=4) {
	for (int k=0; k<4; k++)
	    *(ptr++)=accurate_cos(j+k,n);
	for (int k=0; k<4; k++)
	    *(ptr++)=accurate_sin(j+k,n);
    }
    //subsequent iterations
    for (int nn=ns4; nn>=8; nn/=2) {
	int halfnn = nn/2;
	int j = n/nn;
	cerr << "- b: " << nn  << "(offset: " << (ptr-reps->trig_tables) << ", mult: " << j << ")" << endl;
	for (int i=0; i<halfnn; i+=4) {
	    cerr << "--- i: " << i << endl;
	    for (int k=0; k<4; k++)
		*(ptr++)=accurate_cos(j*(i+k),n);
	    for (int k=0; k<4; k++)
		*(ptr++)=accurate_sin(j*(i+k),n);
	}
    }
    return reps;
}

//c has size n/2
void ifft_model(void* tables, double* c) {
    double tmp0[4];
    double tmp1[4];
    double tmp2[4];
    double tmp3[4];
    IFFT_PRECOMP* fft_tables = (IFFT_PRECOMP*) tables;
    const int n = fft_tables->n;
    const double* trig_tables = fft_tables->trig_tables;

#ifndef NDEBUG
    vector<complex<double> > a; a.resize(n);
    vector<complex<double> > powomega; powomega.resize(n);
    for (int i=0; i<n; i++)
	powomega[i]=complex<double>(accurate_cos(i,n),accurate_sin(i,n));
    for (int i=0; i<n/2; i++)
	a[i]=c[i]/2.;
    for (int i=0; i<n/2; i++)
	a[n/2+i]=-c[i]/2.;
#endif


    int ns4 = n/4;
    double* are = c;    //size n/4
    double* aim = c+ns4; //size n/4

    //multiply by omega^j
    for (int j=0; j<ns4; j+=4) {
	const double* r0 = trig_tables+2*j;
	const double* r1 = r0+4;
#ifndef NDEBUG
	assert(very_close(r0[0],accurate_cos(j,n)));
	assert(very_close(r0[1],accurate_cos(j+1,n)));
	assert(very_close(r0[2],accurate_cos(j+2,n)));
	assert(very_close(r0[3],accurate_cos(j+3,n)));
	assert(very_close(r1[0],accurate_sin(j,n)));
	assert(very_close(r1[1],accurate_sin(j+1,n)));
	assert(very_close(r1[2],accurate_sin(j+2,n)));
	assert(very_close(r1[3],accurate_sin(j+3,n)));
#endif
	//(re*cos-im*sin) + i (im*cos+re*sin)
	double* d0 = are+j;
	double* d1 = aim+j;
	dotp4(tmp0,d0,r0); //re*cos
	dotp4(tmp1,d1,r0); //im*cos
	dotp4(tmp2,d0,r1); //re*sin
	dotp4(tmp3,d1,r1); //im*sin
	sub4(d0,tmp0,tmp3);
	add4(d1,tmp1,tmp2);
    }

    //at the beginning of iteration nn
    // a_{j,i} has P_{i%nn}(omega^j) 
    // where j between [rev(1) and rev(3)[
    // and i between [0 and nn[
    const double* cur_tt = trig_tables;
    for (int nn=ns4; nn>=8; nn/=2) { //todo
	int halfnn = nn/2;
	cur_tt += 2*nn;
#ifndef NDEBUG
	cerr << "Checking iteration " << nn  << " (offset: "<< (cur_tt-trig_tables) << ")" << endl;
	for (int i=0; i<ns4; i++) {
	    cout << "i: " << i << "   " << are[i] << " : " << aim[i] << endl;
	}
	int m = n/nn;
	int rev1m = rev(1,m);
	int rev3m = rev(3,m);
	int idex = 0;
	for (int revj=rev1m; revj<rev3m; revj++) {
	    int j = rev(revj,m);
	    cerr << "check-- j: " << j << endl;
	    for (int i=0; i<nn; i++) {
		cerr << "check--- i: " << i << "(mod " << nn << ")" << endl;
		complex<double> test_cur(are[idex],aim[idex]);
		//sum_[t=i%nn] a_t omega^jt
		complex<double> pij = 0;	
		for (int k=i; k<n; k+=nn)
		    pij += a[k] * powomega[(k*j) % n];
		assert(very_close(test_cur,pij));
		idex++;
	    }
	}
#endif
	for (int block=0; block<ns4; block+=nn) { //todo
#ifndef NDEBUG
	    int j = rev(rev(1,m)+block,m);
	    cerr << "-- j: " << j  << "(offset: " << (cur_tt-trig_tables) << ")" << endl;
#endif
	    for (int off=0; off<halfnn; off+=4) { //todo
#ifndef NDEBUG
		cerr << "--- i: " << off << endl;
#endif
		double* d00 = are + block + off;
		double* d01 = aim + block + off;
		double* d10 = are + block + halfnn + off;
		double* d11 = aim + block + halfnn + off;
		add4(tmp0,d00,d10); // re + re
		add4(tmp1,d01,d11); // im + im
		sub4(tmp2,d00,d10); // re - re
		sub4(tmp3,d01,d11); // im - im
		copy4(d00,tmp0);
		copy4(d01,tmp1);
		const double* r0 = cur_tt+2*off;
		const double* r1 = r0+4;
#ifdef NDEBUG
		assert(very_close(r0[0],accurate_cos((ns4/halfnn)*off,n)));
		assert(very_close(r0[1],accurate_cos((ns4/halfnn)*(off+1),n)));
		assert(very_close(r0[2],accurate_cos((ns4/halfnn)*(off+2),n)));
		assert(very_close(r0[3],accurate_cos((ns4/halfnn)*(off+3),n)));
		assert(very_close(r1[0],accurate_sin((ns4/halfnn)*off,n)));
		assert(very_close(r1[1],accurate_sin((ns4/halfnn)*(off+1),n)));
		assert(very_close(r1[2],accurate_sin((ns4/halfnn)*(off+2),n)));
		assert(very_close(r1[3],accurate_sin((ns4/halfnn)*(off+3),n)));
#endif
		dotp4(tmp0,tmp2,r0); //re*cos
		dotp4(tmp1,tmp3,r1); //im*sin
		sub4(d10,tmp0,tmp1);
		dotp4(tmp0,tmp2,r1); //re*sin
		dotp4(tmp1,tmp3,r0); //im*cos
		add4(d11,tmp0,tmp1);
	    }
	}
    }

    //size 4
    {
#ifndef NDEBUG
	int nn = 4;
	//int halfnn = 2;
	cerr << "Checking iteration " << nn  << " (offset: "<< (cur_tt-trig_tables) << ")" << endl;
	for (int i=0; i<ns4; i++) {
	    cout << "i: " << i << "   " << are[i] << " : " << aim[i] << endl;
	}
	int m = n/nn;
	int rev1m = rev(1,m);
	int rev3m = rev(3,m);
	int idex = 0;
	for (int revj=rev1m; revj<rev3m; revj++) {
	    int j = rev(revj,m);
	    cerr << "check-- j: " << j << endl;
	    for (int i=0; i<nn; i++) {
		cerr << "check--- i: " << i << "(mod " << nn << ")" << endl;
		complex<double> test_cur(are[idex],aim[idex]);
		//sum_[t=i%nn] a_t omega^jt
		complex<double> pij = 0;	
		for (int k=i; k<n; k+=nn)
		    pij += a[k] * powomega[(k*j) % n];
		assert(very_close(test_cur,pij));
		idex++;
	    }
	}
#endif
	for (int block=0; block<ns4; block+=4) {
	    double* d0 = are+block;
	    double* d1 = aim+block;
	    tmp0[0]=d0[0];
	    tmp0[1]=d0[1];
	    tmp0[2]=d0[0];
	    tmp0[3]=-d1[1];
	    tmp1[0]=d0[2];
	    tmp1[1]=d0[3];
	    tmp1[2]=-d0[2];
	    tmp1[3]=d1[3];
	    tmp2[0]=d1[0];
	    tmp2[1]=d1[1];
	    tmp2[2]=d1[0];
	    tmp2[3]=d0[1];
	    tmp3[0]=d1[2];
	    tmp3[1]=d1[3];
	    tmp3[2]=-d1[2];
	    tmp3[3]=-d0[3];
	    add4(d0,tmp0,tmp1);
	    add4(d1,tmp2,tmp3);
	}
    }

    //size 2
    {
#ifndef NDEBUG
	int nn = 2;
	//int halfnn = 1;
	cerr << "Checking iteration " << nn  << " (offset: "<< (cur_tt-trig_tables) << ")" << endl;
	for (int i=0; i<ns4; i++) {
	    cout << "i: " << i << "   " << are[i] << " : " << aim[i] << endl;
	}
	int m = n/nn;
	int rev1m = rev(1,m);
	int rev3m = rev(3,m);
	int idex = 0;
	for (int revj=rev1m; revj<rev3m; revj++) {
	    int j = rev(revj,m);
	    cerr << "check-- j: " << j << endl;
	    for (int i=0; i<nn; i++) {
		cerr << "check--- i: " << i << "(mod " << nn << ")" << endl;
		complex<double> test_cur(are[idex],aim[idex]);
		//sum_[t=i%nn] a_t omega^jt
		complex<double> pij = 0;	
		for (int k=i; k<n; k+=nn)
		    pij += a[k] * powomega[(k*j) % n];
		assert(very_close(test_cur,pij));
		idex++;
	    }
	}
#endif
	for (int block=0; block<ns4; block+=4) {
	    double* d0 = are+block;
	    double* d1 = aim+block;
	    tmp0[0]=d0[0];
	    tmp0[1]=d0[0];
	    tmp0[2]=d0[2];
	    tmp0[3]=d0[2];
	    tmp1[0]=d0[1];
	    tmp1[1]=-d0[1];
	    tmp1[2]=d0[3];
	    tmp1[3]=-d0[3];
	    add4(d0,tmp0,tmp1);
	    tmp0[0]=d1[0];
	    tmp0[1]=d1[0];
	    tmp0[2]=d1[2];
	    tmp0[3]=d1[2];
	    tmp1[0]=d1[1];
	    tmp1[1]=-d1[1];
	    tmp1[2]=d1[3];
	    tmp1[3]=-d1[3];
	    add4(d1,tmp0,tmp1);
	}
    }
    {
#ifndef NDEBUG
	int nn = 1;
	cerr << "Checking iteration " << nn  << " (offset: "<< (cur_tt-trig_tables) << ")" << endl;
	for (int i=0; i<ns4; i++) {
	    cout << "i: " << i << "   " << are[i] << " : " << aim[i] << endl;
	}
	int m = n/nn;
	int rev1m = rev(1,m);
	int rev3m = rev(3,m);
	int idex = 0;
	for (int revj=rev1m; revj<rev3m; revj++) {
	    int j = rev(revj,m);
	    cerr << "check-- j: " << j << endl;
	    for (int i=0; i<nn; i++) {
		cerr << "check--- i: " << i << "(mod " << nn << ")" << endl;
		complex<double> test_cur(are[idex],aim[idex]);
		//sum_[t=i%nn] a_t omega^jt
		complex<double> pij = 0;	
		for (int k=i; k<n; k+=nn)
		    pij += a[k] * powomega[(k*j) % n];
		assert(very_close(test_cur,pij));
		idex++;
	    }
	}
#endif
    }
}

extern "C" void ifft(const void* tables, double* data);

int main(int argc, char** argv) {
    static const int nn = 1024;
    void* tables = new_ifft_table(nn);
    double* a = new double[nn];
    double* b = new double[nn];
    for (int i=0; i<nn; i++) a[i]=i;
    for (int i=0; i<nn; i++) b[i]=i;
    cerr << "ifft " << endl;
    ifft(tables,a);
    cerr << "ifft_c " << endl;
    ifft_model(tables,b);
    for (int i=0; i<nn/2; i++) {
	cerr << "checking " << i << endl;
	assert(very_close(complex<double>(a[i],a[i+nn/2]),complex<double>(b[i],b[i+nn/2])));
    }

    int nbtrials = 1000;
    long begin = clock();
    for (int trials=0; trials<nbtrials; trials++)
	ifft(tables,a);
    long end = clock();
    cout << "time " << double(end-begin)/nbtrials << endl;
}
