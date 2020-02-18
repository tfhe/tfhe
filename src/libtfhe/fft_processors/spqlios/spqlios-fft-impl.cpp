#include <stdint.h>
#include <cmath>

#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>

#ifdef DEBUG_ALL
#include <complex>
#endif

#include "spqlios-fft.h"

using namespace std;

namespace {
#ifdef DEBUG_ALL
    //reverse the bits of i (mod n)
    int32_t rev(int32_t i, int32_t n) {
        int32_t reps=0;
        for (int32_t j=1; j<n; j*=2) {
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

    bool very_close(const double& a,const double& b) {
        bool reps = (abs(a-b)<1e-5);
        if (!reps) {
        cerr << "not close: " << a << " vs. " << b << endl;
        }
        return reps;
    }
#endif


    typedef struct {
        uint64_t n;
        double *aligned_trig_tables;
        double *aligned_data;
        void *buf;
    } FFT_PRECOMP;

    typedef struct {
        uint64_t n;
        double *aligned_trig_tables;
        double *aligned_data;
        void *buf;
    } IFFT_PRECOMP;

//trig_tables:
//|cos0,cos1,cos2,cos3|sin0,sin1,sin2,sin3|cos4,cos5,cos6,cos7|.... -> n/4
//|cos0,cos2,cos4,cos6|sin0,sin2,sin4,sin6|cos8,cos10,cos12,cos14|.... -> n/4
//...
//|cos0,cos2,cos4,cos6|sin0,sin2,sin4,sin6|cos8,cos10,cos12,cos14|.... -> n/4


//trig_tables:
//|cos0,cos1,cos2,cos3|sin0,sin1,sin2,sin3|cos4,cos5,cos6,cos7|.... -> n/4
//|cos0,cos2,cos4,cos6|sin0,sin2,sin4,sin6|cos8,cos10,cos12,cos14|.... -> n/4
//...
//|cos0,cos2,cos4,cos6|sin0,sin2,sin4,sin6|cos8,cos10,cos12,cos14|.... -> n/4

    inline void dotp4(double *__restrict res, const double *__restrict a, const double *__restrict b) {
        for (int32_t i = 0; i < 4; i++) res[i] = a[i] * b[i];
    }

    inline void add4(double * res, const double * a, const double * b) {
        for (int32_t i = 0; i < 4; i++) res[i] = a[i] + b[i];
    }

    inline void sub4(double * res, const double * a, const double * b) {
        for (int32_t i = 0; i < 4; i++) res[i] = a[i] - b[i];
    }

    inline void copy4(double * res, const double * a) {
        for (int32_t i = 0; i < 4; i++) res[i] = a[i];
    }


    void require(bool condition, const string &message = "[condition]") {
        if (!condition) {
            cerr << "unmet condition: " << condition << endl;
            abort();
        }
    }

    double accurate_cos(int32_t i, int32_t n) { //cos(2pi*i/n)
        i = ((i % n) + n) % n;
        if (i >= 3 * n / 4) return cos(2. * M_PI * (n - i) / double(n));
        if (i >= 2 * n / 4) return -cos(2. * M_PI * (i - n / 2) / double(n));
        if (i >= 1 * n / 4) return -cos(2. * M_PI * (n / 2 - i) / double(n));
        return cos(2. * M_PI * (i) / double(n));
    }

    double accurate_sin(int32_t i, int32_t n) { //sin(2pi*i/n)
        i = ((i % n) + n) % n;
        if (i >= 3 * n / 4) return -sin(2. * M_PI * (n - i) / double(n));
        if (i >= 2 * n / 4) return -sin(2. * M_PI * (i - n / 2) / double(n));
        if (i >= 1 * n / 4) return sin(2. * M_PI * (n / 2 - i) / double(n));
        return sin(2. * M_PI * (i) / double(n));
    }


#ifdef DEBUG_ALL
    //at the beginning of iteration halfnn:
    //   m=n/halfnn
    //   P_{j%m}(omb^i)
    //   for j in [rev(1,m) to rev(3,m)[
    //   and i in [0,halfnn[
    void fft_check(
        int32_t n, int32_t halfnn, int32_t table_offset,
            const double* pre, const double* pim,
        const vector<complex<double> >& p,
        const vector<complex<double> >& powombar
        ) {
            int32_t ns4=n/4;
        cerr << "Checking iteration " << halfnn  << " (offset: "<< (table_offset) << ")" << endl;
        for (int32_t i=0; i<ns4; i++) {
            cout << "i: " << i << "   " << pre[i] << " : " << pim[i] << endl;
        }
        int32_t m = n/halfnn;
        int32_t rev1m = rev(1,m);
        int32_t rev3m = rev(3,m);
        int32_t idex = 0;
        for (int32_t revj=rev1m; revj<rev3m; revj++) {
            int32_t j = rev(revj,m);
            cerr << "check-- j: " << j << "(mod " << m << ")" << endl;
            for (int32_t i=0; i<halfnn; i++) {
            cerr << "check--- i: " << i << endl;
            complex<double> test_cur(pre[idex],pim[idex]);
            //P_sum_[k=j%m] p_k omb^ik-j
            complex<double> pij = 0;
            for (int32_t k=j; k<n; k+=m) {
                if (halfnn==8 && j==1 && i==1) cerr << "pij(" << pij << ")" << "+= p_"<<k<<"("<<p[k]<<") * omb["<<i*(k-j)<<"]("<< powombar[(i*(k-j)) % n] <<")" << endl;
                pij += p[k] * powombar[(i*(k-j)) % n];
            }
            assert(very_close(test_cur,pij));
            idex++;
            }
        }
    }
#endif

}

extern "C" void *new_fft_table(int32_t nn) {
    require(nn >= 16, "n must be >=16");
    require((nn & (nn - 1)) == 0, "n must be a power of 2");
    int32_t n = 2 * nn;
    int32_t ns4 = n / 4;
    FFT_PRECOMP *reps = new FFT_PRECOMP;
    void *buf = malloc(32 + n * 8 + nn * 8);
    uint64_t aligned_addr = (uint64_t(buf) + 31) & 0xFFFFFFFFFFFFFFE0l;
    //assert(((uint64_t)reps)%32==0); //verify alignment
    reps->n = n;
    reps->aligned_trig_tables = (double *) aligned_addr;
    reps->aligned_data = (double *) (aligned_addr + n * 8);
    reps->buf = buf;
    double *ptr = reps->aligned_trig_tables;
    //subsequent iterations
    for (int32_t halfnn = 4; halfnn < ns4; halfnn *= 2) {
        int32_t nn = 2 * halfnn;
        int32_t j = n / nn;
        //cerr << "- b: " << halfnn  << "(offset: " << (ptr-reps->trig_tables) << ", mult: " << j << ")" << endl;
        for (int32_t i = 0; i < halfnn; i += 4) {
            //cerr << "--- i: " << i << endl;
            for (int32_t k = 0; k < 4; k++)
                *(ptr++) = accurate_cos(-j * (i + k), n);
            for (int32_t k = 0; k < 4; k++)
                *(ptr++) = accurate_sin(-j * (i + k), n);
        }
    }
    //last iteration
    for (int32_t i = 0; i < ns4; i += 4) {
        for (int32_t k = 0; k < 4; k++)
            *(ptr++) = accurate_cos(-(i + k), n);
        for (int32_t k = 0; k < 4; k++)
            *(ptr++) = accurate_sin(-(i + k), n);
    }
    return reps;
}
extern "C" double *fft_table_get_buffer(const void *tables) {
    FFT_PRECOMP *reps = (FFT_PRECOMP *) tables;
    return reps->aligned_data;
}
extern "C" double *ifft_table_get_buffer(const void *tables) {
    IFFT_PRECOMP *reps = (IFFT_PRECOMP *) tables;
    return reps->aligned_data;
}

//c has size n/2
extern "C" void fft_model(const void *tables) {
    double tmp0[4];
    double tmp1[4];
    double tmp2[4];
    double tmp3[4];
    FFT_PRECOMP *fft_tables = (FFT_PRECOMP *) tables;
    const int32_t n = fft_tables->n;
    const double *trig_tables = fft_tables->aligned_trig_tables;
    double *c = fft_tables->aligned_data;

    int32_t ns4 = n / 4;
    double *pre = c;     //size n/4
    double *pim = c + ns4; //size n/4

#ifdef DEBUG_ALL
    vector<complex<double> > p; p.resize(n);
    vector<complex<double> > powombar; powombar.resize(n);
    for (int32_t i=0; i<n; i++)
    powombar[i]=complex<double>(accurate_cos(-i,n),accurate_sin(-i,n));
    for (int32_t i=0; i<n; i+=2)
    p[i]=0;
    int32_t rev1n = rev(1,n);
    int32_t rev3n = rev(3,n);
    for (int32_t revi=rev1n; revi<rev3n; revi++) {
    int32_t i = rev(revi,n);
    p[i]=complex<double>(pre[revi-rev1n],pim[revi-rev1n]);
    p[n-i]=conj(p[i]);
    cerr << "p_" << i << " = " << p[i] << endl;
    }
#endif

    //general loop
    //size 2
    {
#ifdef DEBUG_ALL
        int32_t halfnn = 1;
        //int32_t nn = 2;
        int32_t table_offset = 0;
        fft_check(n, halfnn, table_offset, pre, pim, p, powombar);
#endif
        //[1  1]
        //[1 -1]
        //     [1  1]
        //     [1 -1]
        for (int32_t block = 0; block < ns4; block += 4) {
            double *d0 = pre + block;
            double *d1 = pim + block;
            tmp0[0] = d0[0];
            tmp0[1] = d0[0];
            tmp0[2] = d0[2];
            tmp0[3] = d0[2];
            tmp1[0] = d0[1];
            tmp1[1] = -d0[1];
            tmp1[2] = d0[3];
            tmp1[3] = -d0[3];
            add4(d0, tmp0, tmp1);
            tmp0[0] = d1[0];
            tmp0[1] = d1[0];
            tmp0[2] = d1[2];
            tmp0[3] = d1[2];
            tmp1[0] = d1[1];
            tmp1[1] = -d1[1];
            tmp1[2] = d1[3];
            tmp1[3] = -d1[3];
            add4(d1, tmp0, tmp1);
        }
    }


    //size 4
    //[1  0  1  0]
    //[0  1  0 -i]
    //[1  0 -1  0]
    //[0  1  0  i]
    // r0 + r2    i0 + i2
    // r1 + i3    i1 - r3
    // r0 - r2    i0 - i2
    // r1 - i3    i1 + r3
    {
#ifdef DEBUG_ALL
        int32_t halfnn = 2;
        //int32_t nn = 4;
        int32_t table_offset = 0;
        fft_check(n, halfnn, table_offset, pre, pim, p, powombar);
#endif
        for (int32_t block = 0; block < ns4; block += 4) {
            double *re = pre + block;
            double *im = pim + block;
            tmp0[0] = re[0];
            tmp0[1] = re[1];
            tmp0[2] = re[0];
            tmp0[3] = re[1];
            tmp1[0] = re[2];
            tmp1[1] = im[3];
            tmp1[2] = -re[2];
            tmp1[3] = -im[3];
            tmp2[0] = im[0];
            tmp2[1] = im[1];
            tmp2[2] = im[0];
            tmp2[3] = im[1];
            tmp3[0] = im[2];
            tmp3[1] = -re[3];
            tmp3[2] = -im[2];
            tmp3[3] = re[3];
            add4(re, tmp0, tmp1);
            add4(im, tmp2, tmp3);
        }
    }

    //general loop
    const double *cur_tt = trig_tables;
    for (int32_t halfnn = 4; halfnn < ns4; halfnn *= 2) {
        int32_t nn = 2 * halfnn;
#ifdef DEBUG_ALL
        int32_t m = n/halfnn;
        fft_check(n, halfnn, cur_tt-trig_tables, pre, pim, p, powombar);
#endif
        for (int32_t block = 0; block < ns4; block += nn) {
#ifdef DEBUG_ALL
            int32_t j = rev(rev(1,m)+block,m);
            cerr << "-- j: " << j  << "(mod: " << m << ")" << endl;
#endif
            for (int32_t off = 0; off < halfnn; off += 4) {
#ifdef DEBUG_ALL
                cerr << "--- i: " << off << endl;
#endif
                double *re0 = pre + block + off;
                double *im0 = pim + block + off;
                double *re1 = pre + block + halfnn + off;
                double *im1 = pim + block + halfnn + off;
                const double *tcs = cur_tt + 2 * off;
                const double *tsn = tcs + 4;
#ifdef DEBUG_ALL
                assert(very_close(tcs[0],accurate_cos(-(n/nn)*off,n)));
                assert(very_close(tcs[1],accurate_cos(-(n/nn)*(off+1),n)));
                assert(very_close(tcs[2],accurate_cos(-(n/nn)*(off+2),n)));
                assert(very_close(tcs[3],accurate_cos(-(n/nn)*(off+3),n)));
                assert(very_close(tsn[0],accurate_sin(-(n/nn)*off,n)));
                assert(very_close(tsn[1],accurate_sin(-(n/nn)*(off+1),n)));
                assert(very_close(tsn[2],accurate_sin(-(n/nn)*(off+2),n)));
                assert(very_close(tsn[3],accurate_sin(-(n/nn)*(off+3),n)));
#endif
                dotp4(tmp0, re1, tcs); // re*cos
                dotp4(tmp1, re1, tsn); // re*sin
                dotp4(tmp2, im1, tcs); // im*cos
                dotp4(tmp3, im1, tsn); // im*sin
                sub4(tmp0, tmp0, tmp3); // re2
                add4(tmp1, tmp1, tmp2); // im2
                add4(tmp2, re0, tmp0); // re + re
                add4(tmp3, im0, tmp1); // im + im
                sub4(tmp0, re0, tmp0); // re - re
                sub4(tmp1, im0, tmp1); // im - im
                copy4(re0, tmp2);
                copy4(im0, tmp3);
                copy4(re1, tmp0);
                copy4(im1, tmp1);
            }
        }
        cur_tt += nn;
    }

    {
#ifdef DEBUG_ALL
        int32_t halfnn = ns4;
        //int32_t nn = n/2;
        fft_check(n, halfnn, cur_tt-trig_tables, pre, pim, p, powombar);
#endif
    }

    //multiply by omb^j
    for (int32_t j = 0; j < ns4; j += 4) {
        const double *r0 = cur_tt + 2 * j;
        const double *r1 = r0 + 4;
#ifdef DEBUG_ALL
        assert(very_close(r0[0],accurate_cos(-j,n)));
        assert(very_close(r0[1],accurate_cos(-(j+1),n)));
        assert(very_close(r0[2],accurate_cos(-(j+2),n)));
        assert(very_close(r0[3],accurate_cos(-(j+3),n)));
        assert(very_close(r1[0],accurate_sin(-j,n)));
        assert(very_close(r1[1],accurate_sin(-(j+1),n)));
        assert(very_close(r1[2],accurate_sin(-(j+2),n)));
        assert(very_close(r1[3],accurate_sin(-(j+3),n)));
#endif
        //(re*cos-im*sin) + i (im*cos+re*sin)
        double *d0 = pre + j;
        double *d1 = pim + j;
        dotp4(tmp0, d0, r0); //re*cos
        dotp4(tmp1, d1, r0); //im*cos
        dotp4(tmp2, d0, r1); //re*sin
        dotp4(tmp3, d1, r1); //im*sin
        sub4(d0, tmp0, tmp3);
        add4(d1, tmp1, tmp2);
    }
}


extern "C" void *new_ifft_table(int32_t nn) {
    require(nn >= 16, "n must be >=16");
    require((nn & (nn - 1)) == 0, "n must be a power of 2");
    int32_t n = 2 * nn;
    int32_t ns4 = n / 4;
    IFFT_PRECOMP *reps = new IFFT_PRECOMP;
    //assert(((uint64_t)reps)%32==0); //verify alignment
    void *buf = malloc(32 + n * 8 + nn * 8);
    uint64_t aligned_addr = (uint64_t(buf) + 31) & 0xFFFFFFFFFFFFFFE0l;
    //assert(((uint64_t)reps)%32==0); //verify alignment
    reps->n = n;
    reps->aligned_trig_tables = (double *) aligned_addr;
    reps->aligned_data = (double *) (aligned_addr + n * 8);
    reps->buf = buf;
    reps->n = n;
    double *ptr = reps->aligned_trig_tables;
    //first iteration
    for (int32_t j = 0; j < ns4; j += 4) {
        for (int32_t k = 0; k < 4; k++)
            *(ptr++) = accurate_cos(j + k, n);
        for (int32_t k = 0; k < 4; k++)
            *(ptr++) = accurate_sin(j + k, n);
    }
    //subsequent iterations
    for (int32_t nn = ns4; nn >= 8; nn /= 2) {
        int32_t halfnn = nn / 2;
        int32_t j = n / nn;
        //cerr << "- b: " << nn  << "(offset: " << (ptr-reps->trig_tables) << ", mult: " << j << ")" << endl;
        for (int32_t i = 0; i < halfnn; i += 4) {
            //cerr << "--- i: " << i << endl;
            for (int32_t k = 0; k < 4; k++)
                *(ptr++) = accurate_cos(j * (i + k), n);
            for (int32_t k = 0; k < 4; k++)
                *(ptr++) = accurate_sin(j * (i + k), n);
        }
    }
    return reps;
}

namespace {
#ifdef DEBUG_ALL
    void ifft_check(int32_t n, int32_t nn, int32_t table_offset, const double* are, const double* aim, const vector<complex<double> >& a, const vector<complex<double> >& powomega) {
        int32_t ns4=n/4;
        cerr << "Checking iteration " << nn  << " (offset: "<< (table_offset) << ")" << endl;
        for (int32_t i=0; i<ns4; i++) {
        cout << "i: " << i << "   " << are[i] << " : " << aim[i] << endl;
        }
        int32_t m = n/nn;
        int32_t rev1m = rev(1,m);
        int32_t rev3m = rev(3,m);
        int32_t idex = 0;
        for (int32_t revj=rev1m; revj<rev3m; revj++) {
        int32_t j = rev(revj,m);
        cerr << "check-- j: " << j << endl;
        for (int32_t i=0; i<nn; i++) {
            cerr << "check--- i: " << i << "(mod " << nn << ")" << endl;
            complex<double> test_cur(are[idex],aim[idex]);
            //sum_[t=i%nn] a_t omega^jt
            complex<double> pij = 0;
            for (int32_t k=i; k<n; k+=nn)
            pij += a[k] * powomega[(k*j) % n];
            assert(very_close(test_cur,pij));
            idex++;
        }
        }
    }
#endif
}
//c has size n/2
extern "C" void ifft_model(void *tables) {
    double tmp0[4];
    double tmp1[4];
    double tmp2[4];
    double tmp3[4];
    IFFT_PRECOMP *fft_tables = (IFFT_PRECOMP *) tables;
    const int32_t n = fft_tables->n;
    const double *trig_tables = fft_tables->aligned_trig_tables;
    double *c = fft_tables->aligned_data;

#ifdef DEBUG_ALL
    vector<complex<double> > a; a.resize(n);
    vector<complex<double> > powomega; powomega.resize(n);
    for (int32_t i=0; i<n; i++)
    powomega[i]=complex<double>(accurate_cos(i,n),accurate_sin(i,n));
    for (int32_t i=0; i<n/2; i++)
    a[i]=c[i]/2.;
    for (int32_t i=0; i<n/2; i++)
    a[n/2+i]=-c[i]/2.;
#endif


    int32_t ns4 = n / 4;
    double *are = c;    //size n/4
    double *aim = c + ns4; //size n/4

    //multiply by omega^j
    for (int32_t j = 0; j < ns4; j += 4) {
        const double *r0 = trig_tables + 2 * j;
        const double *r1 = r0 + 4;
#ifdef DEBUG_ALL
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
        double *d0 = are + j;
        double *d1 = aim + j;
        dotp4(tmp0, d0, r0); //re*cos
        dotp4(tmp1, d1, r0); //im*cos
        dotp4(tmp2, d0, r1); //re*sin
        dotp4(tmp3, d1, r1); //im*sin
        sub4(d0, tmp0, tmp3);
        add4(d1, tmp1, tmp2);
    }


    //at the beginning of iteration nn
    // a_{j,i} has P_{i%nn}(omega^j) 
    // where j between [rev(1) and rev(3)[
    // and i between [0 and nn[
    const double *cur_tt = trig_tables;
    for (int32_t nn = ns4; nn >= 8; nn /= 2) {
        int32_t halfnn = nn / 2;
        cur_tt += 2 * nn;
#ifdef DEBUG_ALL
        int32_t m = n/nn;
        ifft_check(n, nn, cur_tt-trig_tables, are, aim, a, powomega);
#endif
        for (int32_t block = 0; block < ns4; block += nn) {
#ifdef DEBUG_ALL
            int32_t j = rev(rev(1,m)+block,m);
            cerr << "-- j: " << j  << "(offset: " << (cur_tt-trig_tables) << ")" << endl;
#endif
            for (int32_t off = 0; off < halfnn; off += 4) {
#ifdef DEBUG_ALL
                cerr << "--- i: " << off << endl;
#endif
                double *d00 = are + block + off;
                double *d01 = aim + block + off;
                double *d10 = are + block + halfnn + off;
                double *d11 = aim + block + halfnn + off;
                add4(tmp0, d00, d10); // re + re
                add4(tmp1, d01, d11); // im + im
                sub4(tmp2, d00, d10); // re - re
                sub4(tmp3, d01, d11); // im - im
                copy4(d00, tmp0);
                copy4(d01, tmp1);
                const double *r0 = cur_tt + 2 * off;
                const double *r1 = r0 + 4;
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
                dotp4(tmp0, tmp2, r0); //re*cos
                dotp4(tmp1, tmp3, r1); //im*sin
                sub4(d10, tmp0, tmp1);
                dotp4(tmp0, tmp2, r1); //re*sin
                dotp4(tmp1, tmp3, r0); //im*cos
                add4(d11, tmp0, tmp1);
            }
        }
    }

    //size 4
    {
#ifdef DEBUG_ALL
        int32_t nn = 4;
        //int32_t halfnn = 2;
        ifft_check(n, nn, cur_tt-trig_tables, are, aim, a, powomega);
#endif
        for (int32_t block = 0; block < ns4; block += 4) {
            double *d0 = are + block;
            double *d1 = aim + block;
            tmp0[0] = d0[0];
            tmp0[1] = d0[1];
            tmp0[2] = d0[0];
            tmp0[3] = -d1[1];
            tmp1[0] = d0[2];
            tmp1[1] = d0[3];
            tmp1[2] = -d0[2];
            tmp1[3] = d1[3];
            tmp2[0] = d1[0];
            tmp2[1] = d1[1];
            tmp2[2] = d1[0];
            tmp2[3] = d0[1];
            tmp3[0] = d1[2];
            tmp3[1] = d1[3];
            tmp3[2] = -d1[2];
            tmp3[3] = -d0[3];
            add4(d0, tmp0, tmp1);
            add4(d1, tmp2, tmp3);
        }
    }

    //size 2
    {
#ifdef DEBUG_ALL
        int32_t nn = 2;
        //int32_t halfnn = 1;
        ifft_check(n, nn, cur_tt-trig_tables, are, aim, a, powomega);
#endif
        for (int32_t block = 0; block < ns4; block += 4) {
            double *d0 = are + block;
            double *d1 = aim + block;
            tmp0[0] = d0[0];
            tmp0[1] = d0[0];
            tmp0[2] = d0[2];
            tmp0[3] = d0[2];
            tmp1[0] = d0[1];
            tmp1[1] = -d0[1];
            tmp1[2] = d0[3];
            tmp1[3] = -d0[3];
            add4(d0, tmp0, tmp1);
            tmp0[0] = d1[0];
            tmp0[1] = d1[0];
            tmp0[2] = d1[2];
            tmp0[3] = d1[2];
            tmp1[0] = d1[1];
            tmp1[1] = -d1[1];
            tmp1[2] = d1[3];
            tmp1[3] = -d1[3];
            add4(d1, tmp0, tmp1);
        }
    }
    {
#ifdef DEBUG_ALL
        int32_t nn = 1;
        ifft_check(n, nn, cur_tt-trig_tables, are, aim, a, powomega);
#endif
    }
}

