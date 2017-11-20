#ifndef FAKES_LAGRANGEHALFC_H
#define FAKES_LAGRANGEHALFC_H

namespace {

    // Fake TLWE structure 
    struct FakeLagrangeHalfCPolynomial {
        //TODO: parallelization
        static const int32_t FAKE_TYPE_UID = 213579845; // precaution: distinguish fakes from trues
        const int32_t fake_uid;
        int32_t isInt; // -1: nsp, 0: torus, 1: int32_t
        union innerPolynomial {
            IntPolynomial *as_intPolynomial;
            TorusPolynomial *as_torusPolynomial;
            void *as_nothing;
        };
        innerPolynomial value;

        // construct
        FakeLagrangeHalfCPolynomial(int32_t N) : fake_uid(FAKE_TYPE_UID) {
            isInt = -1;
            value.as_nothing = 0;
        }

        void set_intPolynomial(const IntPolynomial *p) {
            if (isInt == 0) abort();
            if (isInt == -1) {
                isInt = 1;
                value.as_intPolynomial = new_IntPolynomial(p->N);
            }
            if (p != 0)
                intPolynomialCopy(value.as_intPolynomial, p);
            else
                intPolynomialClear(value.as_intPolynomial);
        }

        void set_torusPolynomial(const TorusPolynomial *p) {
            if (isInt == 1) abort(); //should not mix int32_t/torus
            if (isInt == -1) {
                isInt = 0;
                value.as_torusPolynomial = new_TorusPolynomial(p->N);
            }
            if (p != 0)
                torusPolynomialCopy(value.as_torusPolynomial, p);
            else
                torusPolynomialClear(value.as_torusPolynomial);
        }

        IntPolynomial *getIntPolynomialPtr() {
            if (isInt != 1) abort();
            return value.as_intPolynomial;
        }

        const IntPolynomial *getIntPolynomialPtr() const {
            if (isInt != 1) abort();
            return value.as_intPolynomial;
        }

        TorusPolynomial *getTorusPolynomialPtr() {
            if (isInt != 0) abort();
            return value.as_torusPolynomial;
        }

        const TorusPolynomial *getTorusPolynomialPtr() const {
            if (isInt != 0) abort();
            return value.as_torusPolynomial;
        }

        // delete
        ~FakeLagrangeHalfCPolynomial() {
            if (fake_uid != FAKE_TYPE_UID) abort();
            switch (isInt) {
                case 0:
                    delete_TorusPolynomial(value.as_torusPolynomial);
                    break;
                case 1:
                    delete_IntPolynomial(value.as_intPolynomial);
                    break;
            }
        }

        FakeLagrangeHalfCPolynomial(const FakeLagrangeHalfCPolynomial &) = delete;

        void operator=(const FakeLagrangeHalfCPolynomial &)= delete;
    };

    inline FakeLagrangeHalfCPolynomial *fake(LagrangeHalfCPolynomial *ptr) {
        FakeLagrangeHalfCPolynomial *reps = (FakeLagrangeHalfCPolynomial *) ptr;
        if (reps->fake_uid != FakeLagrangeHalfCPolynomial::FAKE_TYPE_UID) abort();
        return reps;
    }

    inline const FakeLagrangeHalfCPolynomial *fake(const LagrangeHalfCPolynomial *ptr) {
        const FakeLagrangeHalfCPolynomial *reps = (const FakeLagrangeHalfCPolynomial *) ptr;
        if (reps->fake_uid != FakeLagrangeHalfCPolynomial::FAKE_TYPE_UID) abort();
        return reps;
    }

    inline LagrangeHalfCPolynomial *fake_new_LagrangeHalfCPolynomial_array(int32_t nbelts, int32_t N) {
        FakeLagrangeHalfCPolynomial *reps = (FakeLagrangeHalfCPolynomial *) malloc(
                nbelts * sizeof(FakeLagrangeHalfCPolynomial));
        for (int32_t i = 0; i < nbelts; i++)
            new(reps + i) FakeLagrangeHalfCPolynomial(N);
        return (LagrangeHalfCPolynomial *) reps;
    }

#define USE_FAKE_new_LagrangeHalfCPolynomial_array \
    inline LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial_array(int32_t nbelts, int32_t N) { \
    return fake_new_LagrangeHalfCPolynomial_array(nbelts, N); \
    }

    inline void fake_delete_LagrangeHalfCPolynomial_array(int32_t nbelts, LagrangeHalfCPolynomial *ptr) {
        FakeLagrangeHalfCPolynomial *fptr = fake(ptr);
        for (int32_t i = 0; i < nbelts; i++)
            (fptr + i)->~FakeLagrangeHalfCPolynomial();
        free(ptr);
    }

#define USE_FAKE_delete_LagrangeHalfCPolynomial_array \
    inline void delete_LagrangeHalfCPolynomial_array(int32_t nbelts, LagrangeHalfCPolynomial* ptr) { \
    fake_delete_LagrangeHalfCPolynomial_array(nbelts, ptr); \
    }

    inline LagrangeHalfCPolynomial *fake_new_LagrangeHalfCPolynomial(int32_t N) {
        return fake_new_LagrangeHalfCPolynomial_array(1, N);
    }

#define USE_FAKE_new_LagrangeHalfCPolynomial \
    inline LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial(int32_t N) { \
    return fake_new_LagrangeHalfCPolynomial(N); \
    }

    inline void fake_delete_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial *ptr) {
        fake_delete_LagrangeHalfCPolynomial_array(1, ptr);
    }

#define USE_FAKE_delete_LagrangeHalfCPolynomial \
    inline void delete_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* ptr) { \
    fake_delete_LagrangeHalfCPolynomial(ptr); \
    }


    inline void fake_IntPolynomial_ifft(LagrangeHalfCPolynomial *result, const IntPolynomial *p) {
        FakeLagrangeHalfCPolynomial *fres = fake(result);
        fres->set_intPolynomial(p);
    }

#define USE_FAKE_IntPolynomial_ifft \
    inline void IntPolynomial_ifft(LagrangeHalfCPolynomial* result, const IntPolynomial* p) { \
    fake_IntPolynomial_ifft(result, p); \
    }


    inline void fake_TorusPolynomial_ifft(LagrangeHalfCPolynomial *result, const TorusPolynomial *p) {
        FakeLagrangeHalfCPolynomial *fres = fake(result);
        fres->set_torusPolynomial(p);
    }

#define USE_FAKE_TorusPolynomial_ifft \
    inline  void TorusPolynomial_ifft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p) { \
    fake_TorusPolynomial_ifft(result, p); \
    }

    inline void fake_TorusPolynomial_fft(TorusPolynomial *result, const LagrangeHalfCPolynomial *p) {
        const FakeLagrangeHalfCPolynomial *fp = fake(p);
        torusPolynomialCopy(result, fp->getTorusPolynomialPtr());
    }

#define USE_FAKE_TorusPolynomial_fft \
    inline void TorusPolynomial_fft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p) { \
    fake_TorusPolynomial_fft(result, p); \
    }

//MISC OPERATIONS
/** sets to zero */
    inline void fake_LagrangeHalfCPolynomialClear(LagrangeHalfCPolynomial *result) {
        FakeLagrangeHalfCPolynomial *fres = fake(result);
        fres->set_torusPolynomial(0);
    }

#define USE_FAKE_LagrangeHalfCPolynomialClear \
    inline void LagrangeHalfCPolynomialClear(LagrangeHalfCPolynomial* result) { \
    fake_LagrangeHalfCPolynomialClear(result); \
    }

//EXPORT void LagrangeHalfCPolynomialSetTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu);
//EXPORT void LagrangeHalfCPolynomialAddTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 cst);
//EXPORT void LagrangeHalfCPolynomialSetXaiMinusOne(LagrangeHalfCPolynomial* result, const int32_t ai);

//EXPORT void LagrangeHalfCPolynomialMul(LagrangeHalfCPolynomial* result, const LagrangeHalfCPolynomial* a, const LagrangeHalfCPolynomial* b);

//EXPORT void LagrangeHalfCPolynomialAddTo(LagrangeHalfCPolynomial* accum, const LagrangeHalfCPolynomial* a);

    inline void fake_LagrangeHalfCPolynomialAddMul(LagrangeHalfCPolynomial *accum, const LagrangeHalfCPolynomial *a,
                                                   const LagrangeHalfCPolynomial *b) {
        const FakeLagrangeHalfCPolynomial *fa = fake(a);
        const FakeLagrangeHalfCPolynomial *fb = fake(b);
        FakeLagrangeHalfCPolynomial *facc = fake(accum);

        TorusPolynomial *tmp = new_TorusPolynomial(fa->getIntPolynomialPtr()->N);
        torusPolynomialMultKaratsuba(tmp, fa->getIntPolynomialPtr(), fb->getTorusPolynomialPtr());
        torusPolynomialAddTo(facc->getTorusPolynomialPtr(), tmp);

        delete_TorusPolynomial(tmp);
    }

#define USE_FAKE_LagrangeHalfCPolynomialAddMul \
    inline void LagrangeHalfCPolynomialAddMul(LagrangeHalfCPolynomial* accum, const LagrangeHalfCPolynomial* a, const LagrangeHalfCPolynomial* b) { \
    fake_LagrangeHalfCPolynomialAddMul(accum, a, b); \
    }

//EXPORT void LagrangeHalfCPolynomialSubMul(LagrangeHalfCPolynomial* accum, const LagrangeHalfCPolynomial* a, const LagrangeHalfCPolynomial* b);

}//end namespace

#endif // FAKES_LAGRANGEHALFC_H
