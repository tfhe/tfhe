namespace {
    struct FakeTGsw {
	static const long FAKE_TGSW_UID=123444802642375465l;
	const long fake_uid;
	IntPolynomial* message;
	double current_variance;
	FakeTGsw(int N):fake_uid(FAKE_TGSW_UID) {
	    message = new_IntPolynomial(N);
	    current_variance = 0.;
	}
	~FakeTGsw() {
	    if (fake_uid!=FAKE_TGSW_UID) abort();
	    delete_IntPolynomial(message);
	}
	FakeTGsw(const FakeTGsw&)=delete;
	void operator=(const FakeTGsw&)=delete;
    };

    inline FakeTGsw* fake(TGswSample* sample) {
	FakeTGsw* reps = (FakeTGsw*) sample;
	if (reps->fake_uid!=FakeTGsw::FAKE_TGSW_UID) abort();
	return reps; 
    }
    inline const FakeTGsw* fake(const TGswSample* sample) {
	const FakeTGsw* reps = (const FakeTGsw*) sample;
	if (reps->fake_uid!=FakeTGsw::FAKE_TGSW_UID) abort();
	return reps; 
    }

    inline TGswSample* fake_new_TGswSample(const TGswParams* params) {
	int N = params->tlwe_params->N;
	return (TGswSample*) new FakeTGsw(N);
    }
    inline void fake_delete_TGswSample(TGswSample* sample) {
	delete fake(sample);
    }
    inline void fake_tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key) {
	FakeTGsw* fres = fake(result);
	intPolynomialCopy(fres->message, message);
	fres->current_variance=alpha;
    }
    inline void fake_tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key) {
	FakeTGsw* fres = fake(result);
	intPolynomialClear(fres->message);
	fres->message->coefs[0]=message;
	fres->current_variance=alpha;
    }
    inline void fake_tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize) {
	intPolynomialCopy(result, fake(sample)->message);
    }
    //EXPORT int tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 
    //do we really decrypt Gsw samples?

    // support Functions for TGsw
    // Result = 0
    inline void fake_tGswClear(TGswSample* result, const TGswParams* params) {
	FakeTGsw* fres = fake(result);
	intPolynomialClear(fres->message);
	fres->current_variance=0.;
    }
    // Result += H
    inline void fake_tGswAddH(TGswSample* result, const TGswParams* params) {
	FakeTGsw* fres = fake(result);
	fres->message->coefs[0]+=1;
    }
    // Result += mu*H
    inline void fake_tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params) {
	FakeTGsw* fres = fake(result);
	intPolynomialAddTo(fres->message,message);
    }
    // Result += mu*H, mu integer
    inline void fake_tGswAddMuIntH(TGswSample* result, const int message, const TGswParams* params) {
	FakeTGsw* fres = fake(result);
	fres->message->coefs[0]+=message;
    }
    // Result = tGsw(0)
    inline void fake_tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key) {
	FakeTGsw* fres = fake(result);
	intPolynomialClear(fres->message);
	fres->current_variance=alpha;
    }

    //fonction de decomposition
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample, const TGswParams* params);
    //EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	
    // result= (X^ai-1)*bk (ligne 5 de l'algo)
    inline void fake_tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params) {
	FakeTGsw* fres = fake(result);
	const FakeTGsw* fbk = fake(bk);
	intPolynomialMulByXaiMinusOne(fres->message,ai,fbk->message);
	fres->current_variance=fbk->current_variance*2;
    }

    //ligne 5 algo,mult externe
    inline void fake_tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample,const TGswParams* params) {
	//const FakeTGsw* fsample = fake(sample);
	//fake_tlwe_mulRTo(accum,fsample->message, params->tlwe_params);
	//TODO: variance 
    }

    /** result = result + sample */
    inline void fake_tGswAddTo(TGswSample* result, const TGswSample* sample, const TGswParams* params) {
	FakeTGsw* fres = fake(result);
	const FakeTGsw* fsample = fake(sample);
	intPolynomialAddTo(fres->message, fsample->message);
	fres->current_variance += fsample->current_variance;
    }

    /** result = result - sample */
    //EXPORT void tGswSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
    /** result = result + p.sample */
    //EXPORT void tGswAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
    /** result = result - p.sample */
    //EXPORT void tGswSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);

    EXPORT void tfhe_createLweBootstrappingKey(LweBootstrappingKey* bk, const LweKey* key_in, const TGswKey* rgsw_key);
    EXPORT void tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu, const LweSample* x);

}
