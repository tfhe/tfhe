namespace {

    // Fake TLWE structure 
    struct FakeTLweFFT {
	static const int FAKE_TYPE_UID = 816458235; // precaution: distinguish fakes from trues
	const int fake_uid;
	TorusPolynomial* message;
	double current_variance;

	//this padding is here to make sure that FakeTLwe and TLweSample have the same size
	char unused_padding[sizeof(TLweSampleFFT)-sizeof(long)-sizeof(TorusPolynomial*)-sizeof(double)];

	// construct
	FakeTLwe(int N):fake_uid(FAKE_TYPE_UID) {
	    message = new_TorusPolynomial(N);
	    current_variance = 0.;
	}

	// delete
	~FakeTLwe() {
	    if (fake_uid!=FAKE_TYPE_UID) abort();
	    delete_TorusPolynomial(message);
	}
	FakeTLwe(const FakeTLwe&)=delete;
	void operator=(const FakeTLwe&)=delete;
    };



    // At compile time, we verify that the two structures have exactly the same size
    static_assert (sizeof(FakeTLweFFT) == sizeof(TLweSampleFFT), "Error: Size is not correct");

    // fake functons 
    inline FakeTLweFFT* fake(TLweSampleFFT* sample) {
	FakeTLweFFT* reps = (FakeTLweFFT*) sample;
	if (reps->fake_uid!=FakeTLweFFT::FAKE_TYPE_UID) abort();
	return reps; 
    }
    inline const FakeTLweFFT* fake(const TLweSampleFFT* sample) {
	const FakeTLweFFT* reps = (const FakeTLweFFT*) sample;
	if (reps->fake_uid!=FakeTLweFFT::FAKE_TYPE_UID) abort();
	return reps; 
    }


    //-----------------------------------------
    //  constructor/destructor
    //-----------------------------------------

    #define USE_FAKE_new_TLweSample_array \
    inline TLweSampleFFT* new_TLweSample_array(int nbelts, const TLweParams* params) { \
        return fake_new_TLweSample_array(nbelts,params); \
    }

    inline void fake_delete_TLweSample_array(int nbelts, TLweSampleFFT* sample) {
        FakeTLweFFT* arr = fake(sample);
        for (int i=0; i<nbelts; i++) (arr+i)->~FakeTLweFFT();
        free(arr);
    }

    // 
    #define USE_FAKE_delete_TLweSample_array \
    inline void delete_TLweSample_array(int nbelts, TLweSampleFFT* samples) { \
        fake_delete_TLweSample_array(nbelts,samples); \
    }



    inline TLweSampleFFT* fake_new_TLweSampleFFT(const TLweParams* params) {
        int N = params->N;
        FakeTLweFFT* reps = (FakeTLweFFT*) malloc(sizeof(FakeTLweFFT));
        new(reps) FakeTLweFFT(N);
        return (TLweSampleFFT*) reps;
    }

    // 
    #define USE_FAKE_new_TLweSampleFFT \
    inline TLweSampleFFT* new_TLweSampleFFT(const TLweParams* params) { \
        return fake_new_TLweSampleFFT(params); \
    }

    inline void fake_delete_TLweSampleFFT(TLweSampleFFT* sample) {
        FakeTLweFFT* ptr = fake(sample);
        (ptr)->~FakeTLweFFT();
        free(ptr);
    }

    // 
#define USE_FAKE_delete_TLweSampleFFT \
    inline void delete_TLweSampleFFT(TLweSampleFFT* sample) { \
	fake_delete_TLweSampleFFT(sample); \
    }


    //-----------------------------------------------
    //  functions
    //-----------------------------------------------


    // Computes the inverse FFT of the coefficients of the TLWE sample
    inline void fake_tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params){
	const FakeTLwe* fs = fake(source);
	FakeTLweFFT* fres = fake(result);

	TorusPolynomial_copy(fres->message, fs->message);
	fres->current_variance=fs->current_variance;
    }

#define USE_FAKE_tLweToFFTConvert \
    inline void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params){ \
	fake_tLweToFFTConvert(result, source, params); \
    }


    // Computes the FFT of the coefficients of the TLWEfft sample
    inline void fake_tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params){
	FakeTLweFFT* fs = fake(source);
	FakeTLwe* fres = fake(result);

	TorusPolynomial_copy(fres->message, fs->message);
	fres->current_variance=fs->current_variance;
    }


#define  tLweFromFFTConvert \
    inline void tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params) { \
	fake_tLweFromFFTConvert(result, source, params); \
    }

    //Arithmetic operations on TLwe samples
    /** result = (0,0) */
    inline void fake_tLweFFTClear(TLweSampleFFT* result, const TLweParams* params){
	FakeTLweFFT* fres = fake(result);

	TorusPolynomial_clear(fres->message);
	fres->current_variance=0;
    }

#define USE_FAKE_tLweFFTClear \
    inline void tLweFFTClear(TLweSampleFFT* result, const TLweParams* params) { \
	fake_tLweFFTClear(result, params); \
    }


    /** result = (0,mu) where mu is a torus polynomial */
    inline void fake_tLweFFTNoiselessTrivial(TLweSampleFFT* result, const TorusPolynomial* mu, const TLweParams* params){
	FakeTLweFFT* fres = fake(result);
	
	TorusPolynomial_copy(fres->message, mu);
	fres->current_variance = 0.;
    }

#define USE_FAKE_tLweFFTNoiselessTrivial \
    inline void tLweFFTNoiselessTrivial(TLweSampleFFT* result, const TorusPolynomial* mu, const TLweParams* params) { \
	fake_tLweFFTNoiselessTrivial(result, mu, params); \
    }

    /** result = (0,mu) where mu is constant*/
    EXPORT void tLweFFTNoiselessTrivialT(TLweSampleFFT* result, const Torus32 mu, const TLweParams* params){
	FakeTLweFFT* fres = fake(result);
	
	TorusPolynomial_clear(fres->message);
	fres->message.coefsT[0]=mu;
	fres->current_variance = 0.;
    }

    /** result = result + sample */
    //EXPORT void tLweFFTAddTo(TLweSampleFFT* result, const TLweSampleFFT* sample, const TLweParams* params);
    //Let's postpone this to make sure we actually need it

    /** result = result - sample */
    //EXPORT void tLweFFTSubTo(TLweSampleFFT* result, const TLweSampleFFT* sample, const TLweParams* params);
    //Let's postpone this to make sure we actually need it

    /** result = result + p.sample */
    //EXPORT void tLweFFTAddMulZTo(TLweSampleFFT* result, int p, const TLweSampleFFT* sample, const TLweParams* params);
    //Let's postpone this to make sure we actually need it

    /** result = result - p.sample */
    //EXPORT void tLweFFTSubMulZTo(TLweSampleFFT* result, int p, const TLweSampleFFT* sample, const TLweParams* params);
    //Let's postpone this to make sure we actually need it


    // result = result + p*sample
    EXPORT void tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
	const int k = params->k;

	for (int i=0; i<=k; i++)
	    LagrangeHalfCPolynomialAddMul(result->a+i,p,sample->a+i);
    }

    // result = p*sample
    EXPORT void tLweFFTMulR(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
	const int k = params->k;

	for (int i=0; i<=k; i++)
	    LagrangeHalfCPolynomialMul(result->a+i,p,sample->a+i);
    }

    // result = result - p*sample
    EXPORT void tLweFFTSubMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
	const int k = params->k;

	for (int i=0; i<=k; i++)
	    LagrangeHalfCPolynomialSubMul(result->a+i,p,sample->a+i);
    }


}
