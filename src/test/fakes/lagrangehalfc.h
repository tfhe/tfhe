#ifndef FAKES_LAGRANGEHALFC_H
#define FAKES_LAGRANGEHALFC_H

    // Fake TLWE structure 
    struct FakeLagrangeHalfCPolynomial {
	static const int32_t FAKE_TYPE_UID = 213579845; // precaution: distinguish fakes from trues
	const int32_t fake_uid;
	int32_t isInt; // -1: nsp, 0: torus, 1: int 
	union innerPolynomial {
	    IntPolynomial* as_intPolynomial;
	    TorusPolynomial* as_torusPolynomial;
	    void* as_nothing;
	};
	innerPolynomial value;

	// construct
	FakeLagrangeHalfCPolynomial(int N):fake_uid(FAKE_TYPE_UID) {
	    isInt=-1;
	    value.as_nothing=0;
	}

	void set_intPolynomial(const IntPolynomial* p) {
	    if (isInt!=-1) abort();
	    isInt=1;
	    value.as_intPolynomial=new_IntPolynomial(p->N);
	    intPolynomialCopy(value.as_intPolynomial, p);
	}

	void set_torusPolynomial(const TorusPolynomial* p) {
	    if (isInt==1 || isInt==0) abort();
	    if (isInt==-1) {
		isInt=0;
		value.as_torusPolynomial=new_TorusPolynomial(p->N);
	    }
	    torusPolynomialCopy(value.as_torusPolynomial, p);
	}

	IntPolynomial* getIntPolynomialPtr() { 
	    if (isInt!=1) abort(); 
	    return value.as_intPolynomial;
	}

	const IntPolynomial* getIntPolynomialPtr() const { 
	    if (isInt!=1) abort(); 
	    return value.as_intPolynomial;
	}

	TorusPolynomial* getTorusPolynomialPtr() { 
	    if (isInt!=0) abort(); 
	    return value.as_torusPolynomial;
	}

	const TorusPolynomial* getTorusPolynomialPtr() const { 
	    if (isInt!=0) abort(); 
	    return value.as_torusPolynomial;
	}

	// delete
	~FakeLagrangeHalfCPolynomial() {
	    if (fake_uid!=FAKE_TYPE_UID) abort();
	    switch(isInt) {
		case 0:
		    delete_TorusPolynomial(value.as_torusPolynomial);
		    break;
		case 1:
		    delete_IntPolynomial(value.as_intPolynomial);
		    break;
	    }
	}
	FakeLagrangeHalfCPolynomial(const FakeLagrangeHalfCPolynomial&)=delete;
	void operator=(const FakeLagrangeHalfCPolynomial&)=delete;
    };

    FakeLagrangeHalfCPolynomial* fake(LagrangeHalfCPolynomial* ptr) {
	FakeLagrangeHalfCPolynomial* reps = (FakeLagrangeHalfCPolynomial*) ptr;
	if (reps->fake_uid!=FakeLagrangeHalfCPolynomial::FAKE_TYPE_UID) abort();
	return reps;
    }

    const FakeLagrangeHalfCPolynomial* fake(const LagrangeHalfCPolynomial* ptr) {
	const FakeLagrangeHalfCPolynomial* reps = (const FakeLagrangeHalfCPolynomial*) ptr;
	if (reps->fake_uid!=FakeLagrangeHalfCPolynomial::FAKE_TYPE_UID) abort();
	return reps;
    }

#endif // LAGRANGEHALFC
