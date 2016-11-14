#include "tfhe_core.h"
#include "tlwe.h"
#include "tgsw.h"
#include "polynomials_arithmetic.h"



TGswParams::TGswParams(int l, int Bgbit, const TLweParams* tlwe_params):
    l(l),
    Bgbit(Bgbit),
    Bg(1<<Bgbit),
    halfBg(Bg/2),
    maskMod(Bg-1),
    tlwe_params(tlwe_params),
    kpl(int((tlwe_params->k+1)*l))
    {
        h = new Torus32[l];
        for (int i = 0; i < l; ++i) {
    	    int kk = (32-(i+1)*Bgbit);
    	    h[i] = 1 << kk; // 1/(Bg^(i+1)) as a Torus32
	    }	

        // offset = Bg/2 * (2^(32-Bgbit) + 2^(32-2*Bgbit) + ... + 2^(32-l*Bgbit)) 
        uint32_t temp1 = 0;
        for (int i = 0; i < l; ++i) {
            uint32_t temp0 = 1 << (32-(i+1)*Bgbit);
            temp1 += temp0;
        }
        offset = temp1*halfBg;

    }


TGswParams::~TGswParams() {
    delete[] h;
}



// same key as in TLwe
TGswKey::TGswKey(const TGswParams* params):
    params(params),tlwe_params(params->tlwe_params),tlwe_key(tlwe_params)
{
    key = tlwe_key.key;
}

TGswKey::~TGswKey() {
}



TGswSample::TGswSample(const TGswParams* params): k(params->tlwe_params->k), l(params->l) {
    all_sample = new_TLweSample_array((k+1)*l,params->tlwe_params); // tous les samples comme un vecteur ligne
    bloc_sample = new TLweSample*[k+1]; // blocs horizontaux (l lignes) de la matrice TGsw

    for (int p = 0; p < k+1; ++p)
	    bloc_sample[p] = all_sample + p*l;

	// current_variance = 0;
}

TGswSample::~TGswSample() {
    delete_TLweSample_array((k+1)*l,all_sample);
    delete[] bloc_sample;
}

TGswSampleFFT::TGswSampleFFT(const TGswParams* params): k(params->tlwe_params->k), l(params->l) {
    all_samples = new_TLweSampleFFT_array((k+1)*l,params->tlwe_params);
    sample = new TLweSampleFFT*[(k+1)*l];

    for (int p = 0; p < (k+1); ++p)
	sample[p] = all_samples + p*l;

    //current_variance = 0;
}

TGswSampleFFT::~TGswSampleFFT() {
    delete_TLweSampleFFT_array((k+1)*l,all_samples);
    delete[] sample;
}


