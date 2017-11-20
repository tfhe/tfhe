#ifndef TFHE_TESTING_ENVIRONMENT

#include <stdlib.h>
#include <tfhe_io.h>
#include <map>
#include <string>
#include "tfhe_generic_streams.h"
#include "tfhe_garbage_collector.h"
#include "lwe-functions.h"
#include "lwekeyswitch.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "polynomials_arithmetic.h"
#include "tfhe_gate_bootstrapping_structures.h"

using namespace std;
#else
#undef EXPORT
#define EXPORT
#endif



/* ********************************************************
 * LWE 
******************************************************** */

/* ****************************
 * LWE params
**************************** */

/**
 * This function prints the lwe parameters to a generic stream
 */
void write_lweParams(const Ostream &F, const LweParams *lweparams) {
    TextModeProperties *props = new_TextModeProperties_blank();
    props->setTypeTitle("LWEPARAMS");
    props->setProperty_int64_t("n", lweparams->n);
    props->setProperty_double("alpha_min", lweparams->alpha_min);
    props->setProperty_double("alpha_max", lweparams->alpha_max);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}

/**
 * This constructor function reads and creates a LWEParams from a generic stream. The result
 * must be deleted with delete_lweParams();
 */
LweParams *read_new_lweParams(const Istream &F) {
    TextModeProperties *props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("LWEPARAMS")) abort();
    int32_t n = props->getProperty_int64_t("n");
    double alpha_min = props->getProperty_double("alpha_min");
    double alpha_max = props->getProperty_double("alpha_max");
    delete_TextModeProperties(props);
    return new_LweParams(n, alpha_min, alpha_max);
}


/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweParams_toFile(FILE *F, const LweParams *lweparams) { write_lweParams(to_Ostream(F), lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT void export_lweParams_toStream(ostream &F, const LweParams *lweparams) {
    write_lweParams(to_Ostream(F), lweparams);
}

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams *new_lweParams_fromStream(std::istream &in) { return read_new_lweParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams *new_lweParams_fromFile(FILE *F) { return read_new_lweParams(to_Istream(F)); }


/* ****************************
 * LWE samples
**************************** */

void read_lweSample(const Istream &F, LweSample *sample, const LweParams *params) {
    const int32_t n = params->n;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != LWE_SAMPLE_TYPE_UID) abort();
    F.fread(sample->a, sizeof(Torus32) * n);
    F.fread(&sample->b, sizeof(Torus32));
    F.fread(&sample->current_variance, sizeof(double));
}


void write_lweSample(const Ostream &F, const LweSample *sample, const LweParams *params) {
    const int32_t n = params->n;
    F.fwrite(&LWE_SAMPLE_TYPE_UID, sizeof(int32_t));
    F.fwrite(sample->a, sizeof(Torus32) * n);
    F.fwrite(&sample->b, sizeof(Torus32));
    F.fwrite(&sample->current_variance, sizeof(double));
}


/**
 * This function prints the lwe sample to a file
 */
EXPORT void export_lweSample_toFile(FILE *F, const LweSample *lwesample, const LweParams *params) {
    write_lweSample(to_Ostream(F), lwesample, params);
}

/**
 * This function prints the lwe sample to a stream
 */
EXPORT void export_lweSample_toStream(ostream &F, const LweSample *lwesample, const LweParams *params) {
    write_lweSample(to_Ostream(F), lwesample, params);
}

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
EXPORT void import_lweSample_fromStream(std::istream &in, LweSample *lwesample, const LweParams *params) {
    read_lweSample(to_Istream(in), lwesample, params);
}

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
EXPORT void import_lweSample_fromFile(FILE *F, LweSample *lwesample, const LweParams *params) {
    read_lweSample(to_Istream(F), lwesample, params);
}






/* ****************************
 * LWE key
**************************** */

/**
 * reads the the coefficients of the key in an already initialized key structure
 * @param F the input stream
 * @param key the destination key
 */
void read_lweKey_content(const Istream &F, LweKey *key) {
    const int32_t n = key->params->n;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != LWE_KEY_TYPE_UID) abort();
    F.fread(key->key, sizeof(int32_t) * n);
}


/**
 * writes the the coefficients of the key (not the parameters) to the stream
 * @param F the output stream
 * @param key the input key
 */
void write_lweKey_content(const Ostream &F, const LweKey *key) {
    const int32_t n = key->params->n;
    F.fwrite(&LWE_KEY_TYPE_UID, sizeof(int32_t));
    F.fwrite(key->key, sizeof(int32_t) * n);
}

/**
 * this reads a new lweKey (params + content). The read params are garbage-collected, 
 * the output key needs to be deleted by the user (with delete_LweKey). 
 * @param F the input stream
 * @Return the key
 */
LweKey *read_new_lweKey(const Istream &F, const LweParams *params = 0x0) {
    if (params == 0) {
        LweParams *tmp = read_new_lweParams(F);
        params = tmp;
        TfheGarbageCollector::register_param(tmp);
    }
    LweKey *key = new_LweKey(params);
    read_lweKey_content(F, key);
    return key;
}


/**
 * this writes a lweKey (params + content) to a stream. 
 * @param F the output stream
 * @Return the key
 */
void write_lweKey(const Ostream &F, const LweKey *key, bool output_params = true) {
    if (output_params) write_lweParams(F, key->params);
    write_lweKey_content(F, key);
}



/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweKey_toFile(FILE *F, const LweKey *lwekey) { write_lweKey(to_Ostream(F), lwekey); }

/**
 * This function prints the lwe parameters to a stream
 */
EXPORT void export_lweKey_toStream(ostream &F, const LweKey *lwekey) { write_lweKey(to_Ostream(F), lwekey); }

/**
 * This constructor function reads and creates a LWEKey from a stream. The result
 * must be deleted with delete_lweKey();
 */
EXPORT LweKey *new_lweKey_fromStream(std::istream &in) { return read_new_lweKey(to_Istream(in)); }

/**
 * This constructor function reads and creates a LWEKey from a File. The result
 * must be deleted with delete_lweKey();
 */
EXPORT LweKey *new_lweKey_fromFile(FILE *F) { return read_new_lweKey(to_Istream(F)); }







/* ********************************************************
 * TLWE 
******************************************************** */

/* ****************************
 * TLWE params
**************************** */

/**
 * This function prints the tlwe parameters to a generic stream
 */
void write_tLweParams(const Ostream &F, const TLweParams *tlweparams) {
    TextModeProperties *props = new_TextModeProperties_blank();
    props->setTypeTitle("TLWEPARAMS");
    props->setProperty_int64_t("N", tlweparams->N);
    props->setProperty_int64_t("k", tlweparams->k);
    props->setProperty_double("alpha_min", tlweparams->alpha_min);
    props->setProperty_double("alpha_max", tlweparams->alpha_max);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}


/**
 * This constructor function reads and creates a TLweParams from a generic stream. The result
 * must be deleted with delete_TLweParams();
 */
TLweParams *read_new_tLweParams(const Istream &F) {
    TextModeProperties *props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("TLWEPARAMS")) abort();
    int32_t N = props->getProperty_int64_t("N");
    int32_t k = props->getProperty_int64_t("k");
    double alpha_min = props->getProperty_double("alpha_min");
    double alpha_max = props->getProperty_double("alpha_max");
    delete_TextModeProperties(props);
    return new_TLweParams(N, k, alpha_min, alpha_max);
}


/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tLweParams_toFile(FILE *F, const TLweParams *tlweparams) {
    write_tLweParams(to_Ostream(F), tlweparams);
}

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT void export_tLweParams_toStream(ostream &F, const TLweParams *tlweparams) {
    write_tLweParams(to_Ostream(F), tlweparams);
}

/**
 * This constructor function reads and creates a TLWEParams from a stream. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams *new_tLweParams_fromStream(std::istream &in) { return read_new_tLweParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams *new_tLweParams_fromFile(FILE *F) { return read_new_tLweParams(to_Istream(F)); }


/* ****************************
 * TLWE samples
**************************** */

void read_tLweSample(const Istream &F, TLweSample *sample, const TLweParams *params) {
    const int32_t N = params->N;
    const int32_t k = params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TLWE_SAMPLE_TYPE_UID) abort();
    for (int32_t i = 0; i <= k; ++i) {
        F.fread(sample->a[i].coefsT, sizeof(Torus32) * N);
    }
    F.fread(&sample->current_variance, sizeof(double));
}

void write_tLweSample(const Ostream &F, const TLweSample *sample, const TLweParams *params) {
    const int32_t N = params->N;
    const int32_t k = params->k;
    F.fwrite(&TLWE_SAMPLE_TYPE_UID, sizeof(int32_t));
    for (int32_t i = 0; i <= k; ++i) {
        F.fwrite(sample->a[i].coefsT, sizeof(Torus32) * N);
    }
    F.fwrite(&sample->current_variance, sizeof(double));
}


/**
 * This function prints the tlwe sample to a file
 */
EXPORT void export_tlweSample_toFile(FILE *F, const TLweSample *tlwesample, const TLweParams *params) {
    write_tLweSample(to_Ostream(F), tlwesample, params);
}

/**
 * This function reads a TLWESample from a stream in an
 * already allocated tlwesample.
 */
EXPORT void import_tlweSample_fromFile(FILE *F, TLweSample *tlwesample, const TLweParams *params) {
    read_tLweSample(to_Istream(F), tlwesample, params);
}


/**
 * This function prints the tlwe sample to a stream
 */
EXPORT void export_tlweSample_toStream(std::ostream &F, const TLweSample *tlwesample, const TLweParams *params) {
    write_tLweSample(to_Ostream(F), tlwesample, params);
}

/**
 * This function reads a TLWESample from a stream in an
 * already allocated tlwesample.
 */
EXPORT void import_tlweSample_fromStream(std::istream &F, TLweSample *tlwesample, const TLweParams *params) {
    read_tLweSample(to_Istream(F), tlwesample, params);
}



/* ****************************
 * TLWE FFT samples
**************************** */
/*
void read_tLweSampleFFT(const Istream& F, TLweSampleFFT* sample, const TLweParams* params) {
    const int32_t k = params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TLWE_SAMPLE_FFT_TYPE_UID) abort();
    for (int32_t i = 0; i <= k; ++i) {
        F.fread(&sample->a[i], sizeof(LagrangeHalfCPolynomial));
    }
    F.fread(&sample->current_variance, sizeof(double));
}


void write_tLweSampleFFT(const Ostream& F, const TLweSampleFFT* sample, const TLweParams* params) {
    const int32_t k = params->k;
    F.fwrite(&TLWE_SAMPLE_FFT_TYPE_UID, sizeof(int32_t));
    for (int32_t i = 0; i <= k; ++i) {
        F.fwrite(&sample->a[i], sizeof(LagrangeHalfCPolynomial));
    }
    F.fwrite(&sample->current_variance, sizeof(double));
}
*/




/* ****************************
 * TLWE key
**************************** */

/**
 * reads the coefficients of a tlweKey, which has been previously be instantiated (the parameters are already given)
 */
void read_tLweKey_content(const Istream &F, TLweKey *key) {
    const int32_t N = key->params->N;
    const int32_t k = key->params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TLWE_KEY_TYPE_UID) abort();
    for (int32_t i = 0; i < k; ++i) {
        F.fread(key->key[i].coefs, sizeof(int32_t) * N);
    }
}

/**
 * writes the coefficients of a tlweKey
 */
void write_tLweKey_content(const Ostream &F, const TLweKey *key) {
    const int32_t N = key->params->N;
    const int32_t k = key->params->k;
    F.fwrite(&TLWE_KEY_TYPE_UID, sizeof(int32_t));
    for (int32_t i = 0; i < k; ++i) {
        F.fwrite(key->key[i].coefs, sizeof(int32_t) * N);
    }
}

/**
 * reads and creates the parameters and coefficients of a tlweKey.
 * The result needs to be deleted by the user (with delete_TLweKey)
 */
TLweKey *read_new_tLweKey(const Istream &F) {
    TLweParams *params = read_new_tLweParams(F);
    TfheGarbageCollector::register_param(params);
    TLweKey *key = new_TLweKey(params);
    read_tLweKey_content(F, key);
    return key;
}

/**
 * writes the coefficients of a tlweKey
 */
void write_tLweKey(const Ostream &F, const TLweKey *key) {
    write_tLweParams(F, key->params);
    write_tLweKey_content(F, key);
}




/**
 * This function prints the tlwe parameters to a file
 */
EXPORT void export_tlweKey_toFile(FILE *F, const TLweKey *tlwekey) { write_tLweKey(to_Ostream(F), tlwekey); }

/**
 * This constructor function reads and creates a TLWEKey from a File. The result
 * must be deleted with delete_tlweKey();
 */
EXPORT TLweKey *new_tlweKey_fromFile(FILE *F) { return read_new_tLweKey(to_Istream(F)); }


/**
 * This function prints the tlwe parameters to a stream
 */
EXPORT void export_tlweKey_toStream(std::ostream &F, const TLweKey *tlwekey) { write_tLweKey(to_Ostream(F), tlwekey); }

/**
 * This constructor function reads and creates a TLWEKey from a stream. The result
 * must be deleted with delete_tlweKey();
 */
EXPORT TLweKey *new_tlweKey_fromStream(std::istream &F) { return read_new_tLweKey(to_Istream(F)); }





/* ********************************************************
 * TGSW
******************************************************** */

/* ****************************
 * TGSW params
**************************** */

/**
 * This function prints the tGsw parameters to a generic stream
 * It only prints the TGSW section, not the Tlwe parameters
 */
void write_tGswParams_section(const Ostream &F, const TGswParams *tgswparams) {
    TextModeProperties *props = new_TextModeProperties_blank();
    props->setTypeTitle("TGSWPARAMS");
    props->setProperty_int64_t("l", tgswparams->l);
    props->setProperty_int64_t("Bgbit", tgswparams->Bgbit);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}

/**
 * This function prints the tGsw parameters to a generic stream
 */
void write_tGswParams(const Ostream &F, const TGswParams *tgswparams) {
    write_tLweParams(F, tgswparams->tlwe_params);
    write_tGswParams_section(F, tgswparams);
}

/**
 * This constructor function reads and creates a TGswParams from a generic stream, and an TlweParams object. 
 * The result must be deleted with delete_TGswParams();
 */
TGswParams *read_new_tGswParams_section(const Istream &F, const TLweParams *tlwe_params) {
    TextModeProperties *props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("TGSWPARAMS")) abort();
    int32_t l = props->getProperty_int64_t("l");
    int32_t Bgbit = props->getProperty_int64_t("Bgbit");
    // ATTENTION ici!!!
    delete_TextModeProperties(props);
    return new_TGswParams(l, Bgbit, tlwe_params);
}

/**
 * This wrapper constructor function reads and creates a TGswParams from a generic stream. 
 * The result must be deleted with delete_TGswParams(), but the inner
 * TlweParams will be garbage-collected automatically;
 */
TGswParams *read_new_tGswParams(const Istream &F) {
    TLweParams *tlwe_params = read_new_tLweParams(F);
    TfheGarbageCollector::register_param(tlwe_params);
    return read_new_tGswParams_section(F, tlwe_params);
}


/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tGswParams_toFile(FILE *F, const TGswParams *tgswparams) {
    write_tGswParams(to_Ostream(F), tgswparams);
}

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT void export_tGswParams_toStream(ostream &F, const TGswParams *tgswparams) {
    write_tGswParams(to_Ostream(F), tgswparams);
}

/**
 * This constructor function reads and creates a TGSWParams from a stream. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams *new_tGswParams_fromStream(std::istream &in) { return read_new_tGswParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams *new_tGswParams_fromFile(FILE *F) { return read_new_tGswParams(to_Istream(F)); }


/* ****************************
 * TGSW samples
**************************** */

void read_tGswSample(const Istream &F, TGswSample *sample, const TGswParams *params) {
    const int32_t kpl = params->kpl;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TGSW_SAMPLE_TYPE_UID) abort();

    for (int32_t i = 0; i < kpl; ++i) {
        read_tLweSample(F, &sample->all_sample[i], params->tlwe_params);
    }
}

void write_tGswSample(const Ostream &F, const TGswSample *sample, const TGswParams *params) {
    const int32_t kpl = params->kpl;
    F.fwrite(&TGSW_SAMPLE_TYPE_UID, sizeof(int32_t));

    for (int32_t i = 0; i < kpl; ++i) {
        write_tLweSample(F, &sample->all_sample[i], params->tlwe_params);
    }
}

/**
 * This function prints the tgsw sample to a file
 */
EXPORT void export_tgswSample_toFile(FILE *F, const TGswSample *tgswsample, const TGswParams *params) {
    write_tGswSample(to_Ostream(F), tgswsample, params);
}

/**
 * This function reads a TGSWSample from a stream in an
 * already allocated tgswsample.
 */
EXPORT void import_tgswSample_fromFile(FILE *F, TGswSample *tgswsample, const TGswParams *params) {
    read_tGswSample(to_Istream(F), tgswsample, params);
}


/**
 * This function prints the tgsw sample to a stream
 */
EXPORT void export_tgswSample_toStream(std::ostream &F, const TGswSample *tgswsample, const TGswParams *params) {
    write_tGswSample(to_Ostream(F), tgswsample, params);
}

/**
 * This function reads a TGSWSample from a stream in an
 * already allocated tgswsample.
 */
EXPORT void import_tgswSample_fromStream(std::istream &F, TGswSample *tgswsample, const TGswParams *params) {
    read_tGswSample(to_Istream(F), tgswsample, params);
}



/* ****************************
 * TGSW FFT samples
**************************** */
/*
void read_tGswSampleFFT(const Istream& F, TGswSampleFFT* sample, const TGswParams* params) {
    const int32_t kpl = params->kpl;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TGSW_SAMPLE_FFT_TYPE_UID) abort();

    for (int32_t i = 0; i < kpl; ++i) {
        read_tLweSampleFFT(F, &sample->all_samples[i], params->tlwe_params);
    }
}


void write_tGswSampleFFT(const Ostream& F, const TGswSampleFFT* sample, const TGswParams* params) {
    const int32_t kpl = params->kpl;
    F.fwrite(&TGSW_SAMPLE_FFT_TYPE_UID, sizeof(int32_t));

    for (int32_t i = 0; i < kpl; ++i) {
        write_tLweSampleFFT(F, &sample->all_samples[i], params->tlwe_params);
    }
}
*/




/* ****************************
 * TGSW key
**************************** */

/**
 * reads a tgsw key coefficients in a previously defined structure
 */
void read_tGswKey_content(const Istream &F, TGswKey *key) {
    const int32_t N = key->params->tlwe_params->N;
    const int32_t k = key->params->tlwe_params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TGSW_KEY_TYPE_UID) abort();

    for (int32_t i = 0; i < k; ++i) {
        F.fread(key->key[i].coefs, sizeof(int32_t) * N);
    }
}


/**
 * writes the tgsw key coefficients
 */
void write_tGswKey_content(const Ostream &F, const TGswKey *key) {
    const int32_t N = key->params->tlwe_params->N;
    const int32_t k = key->params->tlwe_params->k;
    F.fwrite(&TGSW_KEY_TYPE_UID, sizeof(int32_t));

    for (int32_t i = 0; i < k; ++i) {
        F.fwrite(key->key[i].coefs, sizeof(int32_t) * N);
    }
}

/**
 * reads a tgsw key coefficients in a previously defined structure
 */
TGswKey *read_new_tGswKey(const Istream &F, const TGswParams *params = 0) {
    if (params == 0) {
        TGswParams *tmp = read_new_tGswParams(F);
        TfheGarbageCollector::register_param(tmp);
        params = tmp;
    }
    TGswKey *key = new_TGswKey(params);
    read_tGswKey_content(F, key);
    return key;
}

/**
 * writes the tgsw key coefficients
 */
void write_tGswKey(const Ostream &F, const TGswKey *key, bool output_params = true) {
    if (output_params) write_tGswParams(F, key->params);
    write_tGswKey_content(F, key);
}

/**
 * This function prints the tgsw parameters to a file
 */
EXPORT void export_tgswKey_toFile(FILE *F, const TGswKey *tgswkey) { write_tGswKey(to_Ostream(F), tgswkey); }

/**
 * This constructor function reads and creates a TGSWKey from a File. The result
 * must be deleted with delete_tgswKey();
 */
EXPORT TGswKey *new_tgswKey_fromFile(FILE *F) { return read_new_tGswKey(to_Istream(F)); }


/**
 * This function prints the tgsw parameters to a stream
 */
EXPORT void export_tgswKey_toStream(std::ostream &F, const TGswKey *tgswkey) { write_tGswKey(to_Ostream(F), tgswkey); }

/**
 * This constructor function reads and creates a TGSWKey from a stream. The result
 * must be deleted with delete_tgswKey();
 */
EXPORT TGswKey *new_tgswKey_fromStream(std::istream &F) { return read_new_tGswKey(to_Istream(F)); }



/* ****************************
 * Lwe Keyswitch key
 **************************** */

struct LweKeySwitchParameters {
    int32_t n;
    int32_t t;
    int32_t basebit;
};

/**
 * This function prints the keyswitch parameters to a generic stream
 * It only prints the parameters section, not the keyswitch coefficients
 */
void write_LweKeySwitchParameters_section(const Ostream &F, const LweKeySwitchKey *ks) {
    TextModeProperties *props = new_TextModeProperties_blank();
    props->setTypeTitle("LWEKSPARAMS");
    props->setProperty_int64_t("n", ks->n);
    props->setProperty_int64_t("t", ks->t);
    props->setProperty_int64_t("basebit", ks->basebit);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}

/**
 * This constructor function reads and creates a TGswParams from a generic stream, and an TlweParams object. 
 * The result must be deleted with delete_TGswParams();
 */
void read_lweKeySwitchParameters_section(const Istream &F, LweKeySwitchParameters *reps) {
    TextModeProperties *props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("LWEKSPARAMS")) abort();
    reps->n = props->getProperty_int64_t("n");
    reps->t = props->getProperty_int64_t("t");
    reps->basebit = props->getProperty_int64_t("basebit");
    delete_TextModeProperties(props);
}

/**
 * This function prints the keyswitch coefficients
 */
void write_LweKeySwitchKey_content(const Ostream &F, const LweKeySwitchKey *ks) {
    const LweParams *out_params = ks->out_params;
    const int32_t N = ks->n;
    const int32_t t = ks->t;
    const int32_t base = ks->base;
    const int32_t n = out_params->n;
    double current_variance = -1;

    //computes the maximum variance
    for (int32_t i = 0; i < N; i++)
        for (int32_t j = 0; j < t; j++)
            for (int32_t k = 0; k < base; k++) {
                const LweSample &sample = ks->ks[i][j][k];
                if (sample.current_variance > current_variance)
                    current_variance = sample.current_variance;
            }
    F.fwrite(&LWE_KEYSWITCH_KEY_TYPE_UID, sizeof(int32_t));
    //write the variance once
    F.fwrite(&current_variance, sizeof(double));
    //and dump the coefficients
    for (int32_t i = 0; i < N; i++)
        for (int32_t j = 0; j < t; j++)
            for (int32_t k = 0; k < base; k++) {
                const LweSample &sample = ks->ks[i][j][k];
                //bin output the sample coefs
                F.fwrite(sample.a, n * sizeof(Torus32));
                F.fwrite(&sample.b, 1 * sizeof(Torus32));
            }
}

/**
 * This function reads the keyswitch coefficients
 */
void read_lweKeySwitchKey_content(const Istream &F, LweKeySwitchKey *ks) {
    const LweParams *out_params = ks->out_params;
    const int32_t N = ks->n;
    const int32_t t = ks->t;
    const int32_t base = ks->base;
    const int32_t n = out_params->n;
    double current_variance = -1;

    int32_t type_uid = -1;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != LWE_KEYSWITCH_KEY_TYPE_UID)
        die_dramatically("Trying to read something that is not a LWE Keyswitch!");
    //reads the variance only once in the end
    F.fread(&current_variance, sizeof(double));
    //and read the coefficients
    for (int32_t i = 0; i < N; i++)
        for (int32_t j = 0; j < t; j++)
            for (int32_t k = 0; k < base; k++) {
                LweSample &sample = ks->ks[i][j][k];
                //bin output the sample coefs
                F.fread(sample.a, n * sizeof(Torus32));
                F.fread(&sample.b, 1 * sizeof(Torus32));
                sample.current_variance = current_variance;
            }
}

/**
 * This function prints the keyswitch coefficients
 */
void write_lweKeySwitchKey(const Ostream &F, const LweKeySwitchKey *ks, bool output_LweParams = true) {
    if (output_LweParams)
        write_lweParams(F, ks->out_params);
    write_LweKeySwitchParameters_section(F, ks);
    write_LweKeySwitchKey_content(F, ks);
}

/**
 * This function reads the keyswitch coefficients
 */
LweKeySwitchKey *read_new_lweKeySwitchKey(const Istream &F, const LweParams *out_params = 0) {
    if (out_params == 0) {
        LweParams *tmp = read_new_lweParams(F);
        out_params = tmp;
        TfheGarbageCollector::register_param(tmp);
    }
    LweKeySwitchParameters ksparams;
    read_lweKeySwitchParameters_section(F, &ksparams);
    LweKeySwitchKey *reps = new_LweKeySwitchKey(ksparams.n, ksparams.t, ksparams.basebit, out_params);
    read_lweKeySwitchKey_content(F, reps);
    return reps;
}


/**
 * This function exports a lwe keyswitch key (in binary) to a file
 */
EXPORT void export_lweKeySwitchKey_toFile(FILE *F, const LweKeySwitchKey *ks) {
    write_lweKeySwitchKey(to_Ostream(F), ks);
}

/**
 * This constructor function reads and creates a LweKeySwitchKey from a File. The result
 * must be deleted with delete_LweKeySwitchKey();
 */
EXPORT LweKeySwitchKey *new_lweKeySwitchKey_fromFile(FILE *F) { return read_new_lweKeySwitchKey(to_Istream(F)); }

#ifdef __cplusplus

/**
 * This function exports a lwe keyswitch key (in binary) to a file
 */
EXPORT void export_lweKeySwitchKey_toStream(std::ostream &F, const LweKeySwitchKey *ks) {
    write_lweKeySwitchKey(to_Ostream(F), ks);
}

/**
 * This constructor function reads and creates a LweKeySwitchKey from a File. The result
 * must be deleted with delete_LweKeySwitchKey();
 */
EXPORT LweKeySwitchKey *new_lweKeySwitchKey_fromStream(std::istream &F) {
    return read_new_lweKeySwitchKey(to_Istream(F));
}

#endif

/* ****************************
 * Lwe Bootstrapping key
 **************************** */


/**
 * This function prints the bootstrapping the coefficients (tgsw array section only)
 */
void write_LweBootstrappingKey_content(const Ostream &F, const LweBootstrappingKey *bk) {
    const int32_t n = bk->in_out_params->n;
    const int32_t kpl = bk->bk_params->kpl;
    const int32_t k = bk->bk_params->tlwe_params->k;
    const int32_t N = bk->bk_params->tlwe_params->N;
    double max_variance = -1;
    for (int32_t i = 0; i < n; i++)
        for (int32_t j = 0; j < kpl; j++) {
            TLweSample &sample = bk->bk[i].all_sample[j];
            if (sample.current_variance > max_variance)
                max_variance = sample.current_variance;
        }
    F.fwrite(&LWE_BOOTSTRAPPING_KEY_TYPE_UID, sizeof(int32_t));
    //print the variance once
    F.fwrite(&max_variance, sizeof(double));
    //then print all the coefficients
    for (int32_t i = 0; i < n; i++)
        for (int32_t j = 0; j < kpl; j++) {
            TLweSample &sample = bk->bk[i].all_sample[j];
            for (int32_t l = 0; l <= k; l++) {
                F.fwrite(sample.a[l].coefsT, N * sizeof(Torus32));
            }
        }
}

/**
 * This function reads the bootstrapping the coefficients (tgsw array section only)
 */
void read_LweBootstrappingKey_content(const Istream &F, LweBootstrappingKey *bk) {
    const int32_t n = bk->in_out_params->n;
    const int32_t kpl = bk->bk_params->kpl;
    const int32_t k = bk->bk_params->tlwe_params->k;
    const int32_t N = bk->bk_params->tlwe_params->N;
    double max_variance = -1;
    int32_t type_uid = -1;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != LWE_BOOTSTRAPPING_KEY_TYPE_UID)
        die_dramatically("Trying to read something that is not a BK content");
    F.fread(&max_variance, sizeof(double));
    for (int32_t i = 0; i < n; i++)
        for (int32_t j = 0; j < kpl; j++) {
            TLweSample &sample = bk->bk[i].all_sample[j];
            for (int32_t l = 0; l <= k; l++) {
                F.fread(sample.a[l].coefsT, N * sizeof(Torus32));
            }
            sample.current_variance = max_variance;
        }
}


/**
 * This function prints the bootstrapping parameters to a generic stream
 * It only prints the parameters section, not the coefficients
 */
void write_lweBootstrappingKey(const Ostream &F, const LweBootstrappingKey *bk, bool write_inout_params = true,
                               bool write_bk_params = true) {
    if (write_inout_params) write_lweParams(F, bk->in_out_params);
    if (write_bk_params) write_tGswParams(F, bk->bk_params);
    write_LweKeySwitchParameters_section(F, bk->ks);
    write_LweKeySwitchKey_content(F, bk->ks);
    write_LweBootstrappingKey_content(F, bk);
}

/**
 * This constructor function reads and creates a TGswParams from a generic stream, and an TlweParams object. 
 * The result must be deleted with delete_TGswParams();
 */
LweBootstrappingKey *
read_new_lweBootstrappingKey(const Istream &F, const LweParams *in_out_params = 0, const TGswParams *bk_params = 0) {
    if (in_out_params == 0) {
        LweParams *tmp = read_new_lweParams(F);
        in_out_params = tmp;
        TfheGarbageCollector::register_param(tmp);
    }
    if (bk_params == 0) {
        TGswParams *tmp = read_new_tGswParams(F);
        bk_params = tmp;
        TfheGarbageCollector::register_param(tmp);
    }
    LweKeySwitchParameters ksparams;
    read_lweKeySwitchParameters_section(F, &ksparams);
    if (ksparams.n != bk_params->tlwe_params->N * bk_params->tlwe_params->k)
        die_dramatically("Wrong dimension in bootstrapping key");
    LweBootstrappingKey *reps = new_LweBootstrappingKey(ksparams.t, ksparams.basebit, in_out_params, bk_params);
    read_lweKeySwitchKey_content(F, reps->ks);
    read_LweBootstrappingKey_content(F, reps);
    return reps;
}



/**
 * This function exports a lwe bootstrapping key (in binary) to a file
 */
EXPORT void export_lweBootstrappingKey_toFile(FILE *F, const LweBootstrappingKey *bk) {
    write_lweBootstrappingKey(to_Ostream(F), bk);
}

/**
 * This constructor function reads and creates a LweBootstrappingKey from a File. The result
 * must be deleted with delete_LweBootstrappingKey();
 */
EXPORT LweBootstrappingKey *new_lweBootstrappingKey_fromFile(FILE *F) {
    return read_new_lweBootstrappingKey(to_Istream(F));
}

#ifdef __cplusplus

/**
 * This function exports a lwe bootstrapping key (in binary) to a file
 */
EXPORT void export_lweBootstrappingKey_toStream(std::ostream &F, const LweBootstrappingKey *bk) {
    write_lweBootstrappingKey(to_Ostream(F), bk);
}

/**
 * This constructor function reads and creates a LweBootstrappingKey from a File. The result
 * must be deleted with delete_LweBootstrappingKey();
 */
EXPORT LweBootstrappingKey *new_lweBootstrappingKey_fromStream(std::istream &F) {
    return read_new_lweBootstrappingKey(to_Istream(F));
}

#endif

/* ****************************
 * TFheGateBootstrappingParameterSet key
 **************************** */


void
write_tfheGateBootstrappingProperParameters_section(const Ostream &F, const TFheGateBootstrappingParameterSet *params) {
    TextModeProperties *props = new_TextModeProperties_blank();
    props->setTypeTitle("GATEBOOTSPARAMS");
    props->setProperty_int64_t("ks_t", params->ks_t);
    props->setProperty_int64_t("ks_basebit", params->ks_basebit);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}

void read_tfheGateBootstrappingProperParameters_section(const Istream &F, int32_t &ks_t, int32_t &ks_basebit) {
    TextModeProperties *props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("GATEBOOTSPARAMS")) abort();
    ks_t = props->getProperty_int64_t("ks_t");
    ks_basebit = props->getProperty_double("ks_basebit");
    delete_TextModeProperties(props);
}

void write_tfheGateBootstrappingParameters(const Ostream &F, const TFheGateBootstrappingParameterSet *params) {
    write_tfheGateBootstrappingProperParameters_section(F, params);
    write_lweParams(F, params->in_out_params);
    write_tGswParams(F, params->tgsw_params);
}

TFheGateBootstrappingParameterSet *read_new_tfheGateBootstrappingParameters(const Istream &F) {
    int32_t ks_t, ks_basebit;
    read_tfheGateBootstrappingProperParameters_section(F, ks_t, ks_basebit);
    LweParams *in_out_params = read_new_lweParams(F);
    TGswParams *bk_params = read_new_tGswParams(F);
    TfheGarbageCollector::register_param(in_out_params);
    TfheGarbageCollector::register_param(bk_params);
    return new TFheGateBootstrappingParameterSet(ks_t, ks_basebit, in_out_params, bk_params);
}

/**
 * This function prints the tfhe gate bootstrapping parameter set to a file
 */
EXPORT void export_tfheGateBootstrappingParameterSet_toFile(FILE *F, const TFheGateBootstrappingParameterSet *params) {
    write_tfheGateBootstrappingParameters(to_Ostream(F), params);
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
 * must be deleted with delete_tfheGateBootstrappingParameterSet();
 */
EXPORT TFheGateBootstrappingParameterSet *new_tfheGateBootstrappingParameterSet_fromFile(FILE *F) {
    return read_new_tfheGateBootstrappingParameters(to_Istream(F));
}

#ifdef __cplusplus

/**
 * This function prints the tfhe gate bootstrapping parameter set to a file
 */
EXPORT void export_tfheGateBootstrappingParameterSet_toStream(std::ostream &F,
                                                              const TFheGateBootstrappingParameterSet *params) {
    write_tfheGateBootstrappingParameters(to_Ostream(F), params);
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
 * must be deleted with delete_tfheGateBootstrappingParameterSet();
 */
EXPORT TFheGateBootstrappingParameterSet *new_tfheGateBootstrappingParameterSet_fromStream(std::istream &F) {
    return read_new_tfheGateBootstrappingParameters(to_Istream(F));
}

#endif

/* ****************************
 * TFheGateBootstrappingCloudKeySet
 **************************** */


TFheGateBootstrappingCloudKeySet *
read_new_tfheGateBootstrappingCloudKeySet(const Istream &F, const TFheGateBootstrappingParameterSet *params = 0) {
    if (params == 0) {
        TFheGateBootstrappingParameterSet *tmp = read_new_tfheGateBootstrappingParameters(F);
        TfheGarbageCollector::register_param(tmp);
        params = tmp;
    }
    LweBootstrappingKey *bk = read_new_lweBootstrappingKey(F, params->in_out_params, params->tgsw_params);
    LweBootstrappingKeyFFT *bkFFT = new_LweBootstrappingKeyFFT(bk);
    return new TFheGateBootstrappingCloudKeySet(params, bk, bkFFT);
}

void write_tfheGateBootstrappingCloudKeySet(const Ostream &F, const TFheGateBootstrappingCloudKeySet *key,
                                            bool output_gbparams = true) {
    if (output_gbparams) write_tfheGateBootstrappingParameters(F, key->params);
    write_lweBootstrappingKey(F, key->bk, false, false);
}


/**
 * This function prints the tfhe gate bootstrapping cloud key to a file
 */
EXPORT void export_tfheGateBootstrappingCloudKeySet_toFile(FILE *F, const TFheGateBootstrappingCloudKeySet *keyset) {
    write_tfheGateBootstrappingCloudKeySet(to_Ostream(F), keyset);
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
 */
EXPORT TFheGateBootstrappingCloudKeySet *new_tfheGateBootstrappingCloudKeySet_fromFile(FILE *F) {
    return read_new_tfheGateBootstrappingCloudKeySet(to_Istream(F));
}

#ifdef __cplusplus

/**
 * This function prints the tfhe gate bootstrapping cloud key to a file
 */
EXPORT void export_tfheGateBootstrappingCloudKeySet_toStream(std::ostream &F,
                                                             const TFheGateBootstrappingCloudKeySet *keyset) {
    write_tfheGateBootstrappingCloudKeySet(to_Ostream(F), keyset);
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
 */
EXPORT TFheGateBootstrappingCloudKeySet *new_tfheGateBootstrappingCloudKeySet_fromStream(std::istream &F) {
    return read_new_tfheGateBootstrappingCloudKeySet(to_Istream(F));
}

#endif

/* ****************************
 * TFheGateBootstrappingSecretKeySet
 **************************** */


TFheGateBootstrappingSecretKeySet *
read_new_tfheGateBootstrappingSecretKeySet(const Istream &F, const TFheGateBootstrappingParameterSet *params = 0) {
    if (params == 0) {
        TFheGateBootstrappingParameterSet *tmp = read_new_tfheGateBootstrappingParameters(F);
        TfheGarbageCollector::register_param(tmp);
        params = tmp;
    }
    LweBootstrappingKey *bk = read_new_lweBootstrappingKey(F, params->in_out_params, params->tgsw_params);
    LweKey *lwe_key = read_new_lweKey(F, params->in_out_params);
    TGswKey *tgsw_key = read_new_tGswKey(F, params->tgsw_params);
    LweBootstrappingKeyFFT *bkFFT = new_LweBootstrappingKeyFFT(bk);
    return new TFheGateBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key);
}

void write_tfheGateBootstrappingSecretKeySet(const Ostream &F, const TFheGateBootstrappingSecretKeySet *key,
                                             bool output_gbparams = true) {
    if (output_gbparams) write_tfheGateBootstrappingParameters(F, key->params);
    write_lweBootstrappingKey(F, key->cloud.bk, false, false);
    write_lweKey(F, key->lwe_key, false);
    write_tGswKey(F, key->tgsw_key, false);
}



/**
 * This function prints the tfhe gate bootstrapping secret key to a file
 */
EXPORT void export_tfheGateBootstrappingSecretKeySet_toFile(FILE *F, const TFheGateBootstrappingSecretKeySet *keyset) {
    write_tfheGateBootstrappingSecretKeySet(to_Ostream(F), keyset);
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping secret key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
 */
EXPORT TFheGateBootstrappingSecretKeySet *new_tfheGateBootstrappingSecretKeySet_fromFile(FILE *F) {
    return read_new_tfheGateBootstrappingSecretKeySet(to_Istream(F));
}

#ifdef __cplusplus

/**
 * This function prints the tfhe gate bootstrapping secret key to a file
 */
EXPORT void export_tfheGateBootstrappingSecretKeySet_toStream(std::ostream &F,
                                                              const TFheGateBootstrappingSecretKeySet *keyset) {
    write_tfheGateBootstrappingSecretKeySet(to_Ostream(F), keyset);
}

/**
 * This constructor function reads and creates a TGSWKey from a stream. The result
 * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
 */
EXPORT TFheGateBootstrappingSecretKeySet *new_tfheGateBootstrappingSecretKeySet_fromStream(std::istream &F) {
    return read_new_tfheGateBootstrappingSecretKeySet(to_Istream(F));
}

#endif


/* ****************************
 * TFheGateBootstrappingCiphertext
**************************** */

/**
 * This function prints a gate bootstrapping ciphertext to a file
 * (wrapper to export LweSample)
 */
EXPORT void export_gate_bootstrapping_ciphertext_toFile(FILE *F, const LweSample *sample,
                                                        const TFheGateBootstrappingParameterSet *params) {
    export_lweSample_toFile(F, sample, params->in_out_params);
}

/**
 * This function reads a tfhe gate bootstrapping ciphertext from a File.
 * wrapper to import LweSample
 */
EXPORT void import_gate_bootstrapping_ciphertext_fromFile(FILE *F, LweSample *sample,
                                                          const TFheGateBootstrappingParameterSet *params) {
    import_lweSample_fromFile(F, sample, params->in_out_params);
}

#ifdef __cplusplus

/**
 * This function prints a gate bootstrapping ciphertext to a file
 * (wrapper to export LweSample)
 */
EXPORT void export_gate_bootstrapping_ciphertext_toStream(std::ostream &F, const LweSample *sample,
                                                          const TFheGateBootstrappingParameterSet *params) {
    export_lweSample_toStream(F, sample, params->in_out_params);
}

/**
 * This function reads a tfhe gate bootstrapping ciphertext from a File.
 * wrapper to import LweSample
 */
EXPORT void import_gate_bootstrapping_ciphertext_fromStream(std::istream &F, LweSample *sample,
                                                            const TFheGateBootstrappingParameterSet *params) {
    import_lweSample_fromStream(F, sample, params->in_out_params);
}

#endif


