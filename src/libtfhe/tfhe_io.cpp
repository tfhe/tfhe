#include <tfhe_io.h>
#include <map>
#include <string>
#include <tfhe_generic_streams.h>

using namespace std;



/* ********************************************************
 * LWE 
******************************************************** */

/* ****************************
 * LWE params
**************************** */

/**
 * This function prints the lwe parameters to a generic stream
 */
void export_lweParams(const Ostream& F, const LweParams* lweparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("LWEPARAMS");
    props->setProperty_long("n", lweparams->n);
    props->setProperty_double("alpha_min", lweparams->alpha_min);
    props->setProperty_double("alpha_max", lweparams->alpha_max);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}

/**
 * This constructor function reads and creates a LWEParams from a generic stream. The result
 * must be deleted with delete_lweParams();
 */
LweParams* read_new_lweParams(const Istream& F) {
    TextModeProperties* props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("LWEPARAMS")) abort();
    int n = props->getProperty_long("n");
    double alpha_min = props->getProperty_double("alpha_min");
    double alpha_max = props->getProperty_double("alpha_max");
    delete_TextModeProperties(props);
    return new_LweParams(n,alpha_min,alpha_max);
}


/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweParams_toFile(FILE* F, const LweParams* lweparams) { export_lweParams(to_Ostream(F),lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT void export_lweParams_toStream(ostream& F, const LweParams* lweparams) { export_lweParams(to_Ostream(F),lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromStream(std::istream& in) { return read_new_lweParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromFile(FILE* F)  { return read_new_lweParams(to_Istream(F)); }




/* ****************************
 * LWE samples
**************************** */

void read_lweSample(const Istream& F, LweSample* sample, const LweParams* params) {
    const int n = params->n;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != LWE_SAMPLE_TYPE_UID) abort();
    F.fread(sample->a, sizeof(Torus32)*n);
    F.fread(&sample->b, sizeof(Torus32));
    F.fread(&sample->current_variance, sizeof(double));
}


void write_lweSample(const Ostream& F, const LweSample* sample, const LweParams* params) {
    const int n = params->n;
    F.fwrite(&LWE_SAMPLE_TYPE_UID, sizeof(int32_t));
    F.fwrite(sample->a, sizeof(Torus32)*n);
    F.fwrite(&sample->b, sizeof(Torus32));
    F.fwrite(&sample->current_variance, sizeof(double));
}






/* ****************************
 * LWE key
**************************** */
// the key has been previously defined, and the parameters are already given 

void read_lweKey(const Istream& F, LweKey* key) {
    const int n = key->params->n;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != LWE_KEY_TYPE_UID) abort();
    F.fread(key->key, sizeof(int)*n);
}


void write_lweKey(const Ostream& F, const LweKey* key) {
    const int n = key->params->n;
    F.fwrite(&LWE_KEY_TYPE_UID, sizeof(int32_t));
    F.fwrite(key->key, sizeof(int)*n);
}























/* ********************************************************
 * TLWE 
******************************************************** */

/* ****************************
 * TLWE params
**************************** */

/**
 * This function prints the tlwe parameters to a generic stream
 */
void export_tLweParams(const Ostream& F, const TLweParams* tlweparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("TLWEPARAMS");
    props->setProperty_long("N", tlweparams->N);
    props->setProperty_long("k", tlweparams->k);
    props->setProperty_double("alpha_min", tlweparams->alpha_min);
    props->setProperty_double("alpha_max", tlweparams->alpha_max);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}


/**
 * This constructor function reads and creates a TLweParams from a generic stream. The result
 * must be deleted with delete_TLweParams();
 */
TLweParams* read_new_tLweParams(const Istream& F) {
    TextModeProperties* props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("TLWEPARAMS")) abort();
    int N = props->getProperty_long("N");
    int k = props->getProperty_long("k");
    double alpha_min = props->getProperty_double("alpha_min");
    double alpha_max = props->getProperty_double("alpha_max");
    delete_TextModeProperties(props);
    return new_TLweParams(N,k,alpha_min,alpha_max);
}


/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tLweParams_toFile(FILE* F, const TLweParams* tlweparams) { export_tLweParams(to_Ostream(F),tlweparams); }

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT void export_tLweParams_toStream(ostream& F, const TLweParams* tlweparams) { export_tLweParams(to_Ostream(F),tlweparams); }

/**
 * This constructor function reads and creates a TLWEParams from a stream. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams* new_tLweParams_fromStream(std::istream& in) { return read_new_tLweParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams* new_tLweParams_fromFile(FILE* F)  { return read_new_tLweParams(to_Istream(F)); }





/* ****************************
 * TLWE samples
**************************** */

void read_tLweSample(const Istream& F, TLweSample* sample, const TLweParams* params) {
    const int N = params->N;
    const int k = params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TLWE_SAMPLE_TYPE_UID) abort();
    for (int i = 0; i <= k; ++i) {
        F.fread(&sample->a[i], sizeof(Torus32)*N);
    }
    F.fread(&sample->current_variance, sizeof(double));
}

void write_tLweSample(const Ostream& F, const TLweSample* sample, const TLweParams* params) {
    const int N = params->N;
    const int k = params->k;
    F.fwrite(&TLWE_SAMPLE_TYPE_UID, sizeof(int32_t));
    for (int i = 0; i <= k; ++i) {
        F.fwrite(&sample->a[i], sizeof(Torus32)*N);
    }
    F.fwrite(&sample->current_variance, sizeof(double));
}






/* ****************************
 * TLWE FFT samples
**************************** */

void read_tLweSampleFFT(const Istream& F, TLweSampleFFT* sample, const TLweParams* params) {
    const int k = params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TLWE_SAMPLE_FFT_TYPE_UID) abort();
    for (int i = 0; i <= k; ++i) {
        F.fread(&sample->a[i], sizeof(LagrangeHalfCPolynomial));
    }
    F.fread(&sample->current_variance, sizeof(double));
}


void write_tLweSampleFFT(const Ostream& F, const TLweSampleFFT* sample, const TLweParams* params) {
    const int k = params->k;
    F.fwrite(&TLWE_SAMPLE_FFT_TYPE_UID, sizeof(int32_t));
    for (int i = 0; i <= k; ++i) {
        F.fwrite(&sample->a[i], sizeof(LagrangeHalfCPolynomial));
    }
    F.fwrite(&sample->current_variance, sizeof(double));
}





/* ****************************
 * TLWE key
**************************** */
// the key has been previously defined, and the parameters are already given 

void read_tLweKey(const Istream& F, TLweKey* key) {
    const int N = key->params->N;
    const int k = key->params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TLWE_KEY_TYPE_UID) abort();
    for (int i = 0; i < k; ++i) {
        F.fread(&key->key[i], sizeof(int)*N);
    }
    
}

void write_tLweKey(const Ostream& F, const TLweKey* key) {
    const int N = key->params->N;
    const int k = key->params->k;
    F.fwrite(&TLWE_KEY_TYPE_UID, sizeof(int32_t));
    for (int i = 0; i < k; ++i) {
        F.fwrite(&key->key[i], sizeof(int)*N);
    }
}




















/* ********************************************************
 * TGSW
******************************************************** */

/* ****************************
 * TGSW params
**************************** */

/**
 * This function prints the tGsw parameters to a generic stream
 */
void export_tGswParams(const Ostream& F, const TGswParams* tgswparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("TGSWPARAMS");
    props->setProperty_long("l", tgswparams->l);
    props->setProperty_long("Bgbit", tgswparams->Bgbit);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
    export_tLweParams(F, tgswparams->tlwe_params);
}

/**
 * This constructor function reads and creates a TGswParams from a generic stream. The result
 * must be deleted with delete_TGswParams();
 */
TGswParams* read_new_tGswParams(const Istream& F) {
    TextModeProperties* props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("TGSWPARAMS")) abort();
    int l = props->getProperty_long("l");
    int Bgbit = props->getProperty_long("Bgbit");
    TLweParams* tlweparams = read_new_tLweParams(F);
    delete_TextModeProperties(props);
    return new_TGswParams(l,Bgbit,tlweparams);
}


/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tGswParams_toFile(FILE* F, const TGswParams* tgswparams) { export_tGswParams(to_Ostream(F),tgswparams); }

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT void export_tGswParams_toStream(ostream& F, const TGswParams* tgswparams) { export_tGswParams(to_Ostream(F),tgswparams); }

/**
 * This constructor function reads and creates a TGSWParams from a stream. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams* new_tGswParams_fromStream(std::istream& in) { return read_new_tGswParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams* new_tGswParams_fromFile(FILE* F)  { return read_new_tGswParams(to_Istream(F)); }








/* ****************************
 * TGSW samples
**************************** */

void read_tGswSample(const Istream& F, TGswSample* sample, const TGswParams* params) {
    const int kpl = params->kpl;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TGSW_SAMPLE_TYPE_UID) abort();

    for (int i = 0; i < kpl; ++i) {
        read_tLweSample(F, &sample->all_sample[i], params->tlwe_params);
    }
}

void write_tGswSample(const Ostream& F, const TGswSample* sample, const TGswParams* params) {
    const int kpl = params->kpl;
    F.fwrite(&TGSW_SAMPLE_TYPE_UID, sizeof(int32_t));

    for (int i = 0; i < kpl; ++i) {
        write_tLweSample(F, &sample->all_sample[i], params->tlwe_params);
    }
}





/* ****************************
 * TGSW FFT samples
**************************** */

void read_tGswSampleFFT(const Istream& F, TGswSampleFFT* sample, const TGswParams* params) {
    const int kpl = params->kpl;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TGSW_SAMPLE_FFT_TYPE_UID) abort();

    for (int i = 0; i < kpl; ++i) {
        read_tLweSampleFFT(F, &sample->all_samples[i], params->tlwe_params);
    }
}


void write_tGswSampleFFT(const Ostream& F, const TGswSampleFFT* sample, const TGswParams* params) {
    const int kpl = params->kpl;
    F.fwrite(&TGSW_SAMPLE_FFT_TYPE_UID, sizeof(int32_t));

    for (int i = 0; i < kpl; ++i) {
        write_tLweSampleFFT(F, &sample->all_samples[i], params->tlwe_params);
    }
}





/* ****************************
 * TGSW key
**************************** */
// the key has been previously defined, and the parameters are already given 

void read_tGswKey(const Istream& F, TGswKey* key) {
    const int N = key->params->tlwe_params->N;
    const int k = key->params->tlwe_params->k;
    int32_t type_uid;
    F.fread(&type_uid, sizeof(int32_t));
    if (type_uid != TGSW_KEY_TYPE_UID) abort();

    for (int i = 0; i < k; ++i) {
        F.fread(&key->key[i], sizeof(int)*N);
    }
}


void write_tGswKey(const Ostream& F, const TGswKey* key) {
    const int N = key->params->tlwe_params->N;
    const int k = key->params->tlwe_params->k;
    F.fwrite(&TGSW_KEY_TYPE_UID, sizeof(int32_t));
    
    for (int i = 0; i < k; ++i) {
        F.fwrite(&key->key[i], sizeof(int)*N);
    }
}


