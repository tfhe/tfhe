// #ifndef TFHE_TESTING_ENVIRONMENT
#include <tfhe_io.h>
#include <map>
#include <string>
#include <tfhe_garbage_collector.h>

using namespace std;
// #else
// #undef EXPORT
// #define EXPORT
// #endif



/* ********************************************************
 * LWE
******************************************************** */

/* ****************************
 * LWE params
**************************** */

/**
 * This function prints the lwe parameters to a generic stream
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_lweParams(Ostream& F, const LweParams<TORUS>* lweparams) {
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
template<typename TORUS>
LweParams<TORUS>* IOFunctions<TORUS>::read_new_lweParams(Istream& F) {
  TextModeProperties* props = new_TextModeProperties_fromIstream(F);
  if (props->getTypeTitle() != string("LWEPARAMS")) abort();
  int n = props->getProperty_long("n");
  double alpha_min = props->getProperty_double("alpha_min");
  double alpha_max = props->getProperty_double("alpha_max");
  delete_TextModeProperties(props);
  return new_LweParams<TORUS>(n,alpha_min,alpha_max);
}


/* ****************************
 * LWE samples
**************************** */
template<typename TORUS>
void IOFunctions<TORUS>::read_lweSample(Istream& F, LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  const int n = params->n;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != LWE_SAMPLE_TYPE_UID) abort();
  F.fread(sample->a, sizeof(TORUS)*n);
  F.fread(&sample->b, sizeof(TORUS));
  F.fread(&sample->current_variance, sizeof(double));
}


template<typename TORUS>
void IOFunctions<TORUS>::write_lweSample(Ostream& F, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  const int n = params->n;
  F.fwrite(&LWE_SAMPLE_TYPE_UID, sizeof(int32_t));
  F.fwrite(sample->a, sizeof(TORUS)*n);
  F.fwrite(&sample->b, sizeof(TORUS));
  F.fwrite(&sample->current_variance, sizeof(double));
}


/* ****************************
 * LWE key
**************************** */

/**
 * reads the the coefficients of the key in an already initialized key structure
 * @param F the input stream
 * @param key the destination key
 */
template<typename TORUS>
void IOFunctions<TORUS>::read_lweKey_content(Istream& F, LweKey<TORUS>* key) {
  const int n = key->params->n;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != LWE_KEY_TYPE_UID) abort();
  F.fread(key->key, sizeof(int)*n);
}


/**
 * writes the the coefficients of the key (not the parameters) to the stream
 * @param F the output stream
 * @param key the input key
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_lweKey_content(Ostream& F, const LweKey<TORUS>* key) {
  const int n = key->params->n;
  F.fwrite(&LWE_KEY_TYPE_UID, sizeof(int32_t));
  F.fwrite(key->key, sizeof(int)*n);
}

/**
 * this reads a new lweKey (params + content). The read params are garbage-collected,
 * the output key needs to be deleted by the user (with delete_LweKey).
 * @param F the input stream
 * @Return the key
 */
template<typename TORUS>
LweKey<TORUS>* IOFunctions<TORUS>::read_new_lweKey(Istream& F, const LweParams<TORUS>* params) {
  if (params==0) {
    LweParams<TORUS>* tmp = read_new_lweParams(F);
    params=tmp;
    TfheGarbageCollector<TORUS>::register_param(tmp);
  }
  LweKey<TORUS>* key = new_LweKey<TORUS>(params);
  read_lweKey_content(F,key);
  return key;
}


/**
 * this writes a lweKey (params + content) to a stream.
 * @param F the output stream
 * @Return the key
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_lweKey(Ostream& F, const LweKey<TORUS>* key, bool output_params) {
  if (output_params) write_lweParams(F,key->params);
  write_lweKey_content(F,key);
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
template<typename TORUS>
void IOFunctions<TORUS>::write_tLweParams(Ostream& F, const TLweParams<TORUS>* tlweparams) {
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
template<typename TORUS>
TLweParams<TORUS>* IOFunctions<TORUS>::read_new_tLweParams(Istream& F) {
  TextModeProperties* props = new_TextModeProperties_fromIstream(F);
  if (props->getTypeTitle() != string("TLWEPARAMS")) abort();
  int N = props->getProperty_long("N");
  int k = props->getProperty_long("k");
  double alpha_min = props->getProperty_double("alpha_min");
  double alpha_max = props->getProperty_double("alpha_max");
  delete_TextModeProperties(props);
  return new_TLweParams<TORUS>(N,k,alpha_min,alpha_max);
}


/* ****************************
 * TLWE samples
**************************** */
template<typename TORUS>
void IOFunctions<TORUS>::read_tLweSample(Istream& F, TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  const int N = params->N;
  const int k = params->k;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != TLWE_SAMPLE_TYPE_UID) abort();
  for (int i = 0; i <= k; ++i) {
    F.fread(sample->a[i].coefsT, sizeof(TORUS)*N);
  }
  F.fread(&sample->current_variance, sizeof(double));
}

template<typename TORUS>
void IOFunctions<TORUS>::write_tLweSample(Ostream& F, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  const int N = params->N;
  const int k = params->k;
  F.fwrite(&TLWE_SAMPLE_TYPE_UID, sizeof(int32_t));
  for (int i = 0; i <= k; ++i) {
    F.fwrite(sample->a[i].coefsT, sizeof(TORUS)*N);
  }
  F.fwrite(&sample->current_variance, sizeof(double));
}


/* ****************************
 * TLWE FFT samples
**************************** */
/*
template<typename TORUS>
void IOFunctions<TORUS>::read_tLweSampleFFT(Istream& F, TLweSampleFFT<TORUS>* sample, const TLweParams<TORUS>* params) {
  const int k = params->k;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != TLWE_SAMPLE_FFT_TYPE_UID) abort();
  for (int i = 0; i <= k; ++i) {
    F.fread(&sample->a[i], sizeof(LagrangeHalfCPolynomial));
  }
  F.fread(&sample->current_variance, sizeof(double));
}


template<typename TORUS>
void IOFunctions<TORUS>::write_tLweSampleFFT(Ostream& F, const TLweSampleFFT<TORUS>* sample, const TLweParams<TORUS>* params) {
  const int k = params->k;
  F.fwrite(&TLWE_SAMPLE_FFT_TYPE_UID, sizeof(int32_t));
  for (int i = 0; i <= k; ++i) {
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
template<typename TORUS>
void IOFunctions<TORUS>::read_tLweKey_content(Istream& F, TLweKey<TORUS>* key) {
  const int N = key->params->N;
  const int k = key->params->k;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != TLWE_KEY_TYPE_UID) abort();
  for (int i = 0; i < k; ++i) {
    F.fread(key->key[i].coefs, sizeof(int)*N);
  }
}

/**
 * writes the coefficients of a tlweKey
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_tLweKey_content(Ostream& F, const TLweKey<TORUS>* key) {
  const int N = key->params->N;
  const int k = key->params->k;
  F.fwrite(&TLWE_KEY_TYPE_UID, sizeof(int32_t));
  for (int i = 0; i < k; ++i) {
    F.fwrite(key->key[i].coefs, sizeof(int)*N);
  }
}

/**
 * reads and creates the parameters and coefficients of a tlweKey.
 * The result needs to be deleted by the user (with delete_TLweKey)
 */
template<typename TORUS>
TLweKey<TORUS>* IOFunctions<TORUS>::read_new_tLweKey(Istream& F) {
  TLweParams<TORUS>* params = read_new_tLweParams(F);
  TfheGarbageCollector<TORUS>::register_param(params);
  TLweKey<TORUS>* key = new_TLweKey<TORUS>(params);
  read_tLweKey_content(F,key);
  return key;
}

/**
 * writes the coefficients of a tlweKey
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_tLweKey(Ostream& F, const TLweKey<TORUS>* key) {
  write_tLweParams(F,key->params);
  write_tLweKey_content(F,key);
}

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
template<typename TORUS>
void IOFunctions<TORUS>::write_tGswParams_section(Ostream& F, const TGswParams<TORUS>* tgswparams) {
  TextModeProperties* props = new_TextModeProperties_blank();
  props->setTypeTitle("TGSWPARAMS");
  props->setProperty_long("l", tgswparams->l);
  props->setProperty_long("Bgbit", tgswparams->Bgbit);
  print_TextModeProperties_toOStream(F, props);
  delete_TextModeProperties(props);
}

/**
 * This function prints the tGsw parameters to a generic stream
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_tGswParams(Ostream& F, const TGswParams<TORUS>* tgswparams) {
  write_tLweParams(F, tgswparams->tlwe_params);
  write_tGswParams_section(F, tgswparams);
}

/**
 * This constructor function reads and creates a TGswParams from a generic stream, and an TlweParams object.
 * The result must be deleted with delete_TGswParams();
 */
template<typename TORUS>
TGswParams<TORUS>* IOFunctions<TORUS>::read_new_tGswParams_section(Istream& F, const TLweParams<TORUS>* tlwe_params) {
  TextModeProperties* props = new_TextModeProperties_fromIstream(F);
  if (props->getTypeTitle() != string("TGSWPARAMS")) abort();
  int l = props->getProperty_long("l");
  int Bgbit = props->getProperty_long("Bgbit");
  // ATTENTION ici!!!
  delete_TextModeProperties(props);
  return new_TGswParams<TORUS>(l,Bgbit,tlwe_params);
}

/**
 * This wrapper constructor function reads and creates a TGswParams from a generic stream.
 * The result must be deleted with delete_TGswParams(), but the inner
 * TlweParams will be garbage-collected automatically;
 */
template<typename TORUS>
TGswParams<TORUS>* IOFunctions<TORUS>::read_new_tGswParams(Istream& F) {
  TLweParams<TORUS>* tlwe_params = read_new_tLweParams(F);
  TfheGarbageCollector<TORUS>::register_param(tlwe_params);
  return read_new_tGswParams_section(F, tlwe_params);
}




/* ****************************
 * TGSW samples
**************************** */
template<typename TORUS>
void IOFunctions<TORUS>::read_tGswSample(Istream& F, TGswSample<TORUS>* sample, const TGswParams<TORUS>* params) {
  const int kpl = params->kpl;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != TGSW_SAMPLE_TYPE_UID) abort();

  for (int i = 0; i < kpl; ++i) {
    read_tLweSample(F, &sample->all_sample[i], params->tlwe_params);
  }
}

template<typename TORUS>
void IOFunctions<TORUS>::write_tGswSample(Ostream& F, const TGswSample<TORUS>* sample, const TGswParams<TORUS>* params) {
  const int kpl = params->kpl;
  F.fwrite(&TGSW_SAMPLE_TYPE_UID, sizeof(int32_t));

  for (int i = 0; i < kpl; ++i) {
    write_tLweSample(F, &sample->all_sample[i], params->tlwe_params);
  }
}


/* ****************************
 * TGSW FFT samples
**************************** */
/*
template<typename TORUS>
void IOFunctions<TORUS>::read_tGswSampleFFT(Istream& F, TGswSampleFFT<TORUS>* sample, const TGswParams<TORUS>* params) {
  const int kpl = params->kpl;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != TGSW_SAMPLE_FFT_TYPE_UID) abort();

  for (int i = 0; i < kpl; ++i) {
    read_tLweSampleFFT(F, &sample->all_samples[i], params->tlwe_params);
  }
}


template<typename TORUS>
void IOFunctions<TORUS>::write_tGswSampleFFT(Ostream& F, const TGswSampleFFT<TORUS>* sample, const TGswParams<TORUS>* params) {
  const int kpl = params->kpl;
  F.fwrite(&TGSW_SAMPLE_FFT_TYPE_UID, sizeof(int32_t));

  for (int i = 0; i < kpl; ++i) {
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
template<typename TORUS>
void IOFunctions<TORUS>::read_tGswKey_content(Istream& F, TGswKey<TORUS>* key) {
  const int N = key->params->tlwe_params->N;
  const int k = key->params->tlwe_params->k;
  int32_t type_uid;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid != TGSW_KEY_TYPE_UID) abort();

  for (int i = 0; i < k; ++i) {
    F.fread(key->key[i].coefs, sizeof(int)*N);
  }
}


/**
 * writes the tgsw key coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_tGswKey_content(Ostream& F, const TGswKey<TORUS>* key) {
  const int N = key->params->tlwe_params->N;
  const int k = key->params->tlwe_params->k;
  F.fwrite(&TGSW_KEY_TYPE_UID, sizeof(int32_t));

  for (int i = 0; i < k; ++i) {
    F.fwrite(key->key[i].coefs, sizeof(int)*N);
  }
}

/**
 * reads a tgsw key coefficients in a previously defined structure
 */
template<typename TORUS>
TGswKey<TORUS>* IOFunctions<TORUS>::read_new_tGswKey(Istream& F, const TGswParams<TORUS>* params) {
  if (params==0) {
    TGswParams<TORUS>* tmp = read_new_tGswParams(F);
    TfheGarbageCollector<TORUS>::register_param(tmp);
    params=tmp;
  }
  TGswKey<TORUS>* key = new_TGswKey<TORUS>(params);
  read_tGswKey_content(F, key);
  return key;
}

/**
 * writes the tgsw key coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_tGswKey(Ostream& F, const TGswKey<TORUS>* key, bool output_params) {
  if (output_params) write_tGswParams(F,key->params);
  write_tGswKey_content(F,key);
}

/* ****************************
 * Lwe Keyswitch key
 **************************** */
/**
 * This function prints the keyswitch parameters to a generic stream
 * It only prints the parameters section, not the keyswitch coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_LweKeySwitchParameters_section(Ostream& F, const LweKeySwitchKey<TORUS>* ks) {
  TextModeProperties* props = new_TextModeProperties_blank();
  props->setTypeTitle("LWEKSPARAMS");
  props->setProperty_long("n", ks->n);
  props->setProperty_long("t", ks->t);
  props->setProperty_long("basebit", ks->basebit);
  print_TextModeProperties_toOStream(F, props);
  delete_TextModeProperties(props);
}

/**
 * This constructor function reads and creates a TGswParams from a generic stream, and an TlweParams object.
 * The result must be deleted with delete_TGswParams();
 */
template<typename TORUS>
void IOFunctions<TORUS>::read_lweKeySwitchParameters_section(Istream& F, LweKeySwitchParameters* reps) {
  TextModeProperties* props = new_TextModeProperties_fromIstream(F);
  if (props->getTypeTitle() != string("LWEKSPARAMS")) abort();
  reps->n = props->getProperty_long("n");
  reps->t = props->getProperty_long("t");
  reps->basebit = props->getProperty_long("basebit");
  delete_TextModeProperties(props);
}

/**
 * This function prints the keyswitch coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_LweKeySwitchKey_content(Ostream& F, const LweKeySwitchKey<TORUS>* ks) {
  const LweParams<TORUS>* out_params = ks->out_params;
  const int N = ks->n;
  const int t = ks->t;
  const int base = ks->base;
  const int n = out_params->n;
  double current_variance=-1;

  //computes the maximum variance
  for (int i=0; i<N; i++)
    for (int j=0; j<t; j++)
      for (int k=0; k<base; k++) {
        const LweSample<TORUS>& sample = ks->ks[i][j][k];
        if (sample.current_variance>current_variance)
          current_variance=sample.current_variance;
      }
  F.fwrite(&LWE_KEYSWITCH_KEY_TYPE_UID, sizeof(int32_t));
  //write the variance once
  F.fwrite(&current_variance,sizeof(double));
  //and dump the coefficients
  for (int i=0; i<N; i++)
    for (int j=0; j<t; j++)
      for (int k=0; k<base; k++) {
        const LweSample<TORUS>& sample = ks->ks[i][j][k];
        //bin output the sample coefs
        F.fwrite(sample.a,n*sizeof(TORUS));
        F.fwrite(&sample.b,1*sizeof(TORUS));
      }
}

/**
 * This function reads the keyswitch coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::read_lweKeySwitchKey_content(Istream& F, LweKeySwitchKey<TORUS>* ks) {
  const LweParams<TORUS>* out_params = ks->out_params;
  const int N = ks->n;
  const int t = ks->t;
  const int base = ks->base;
  const int n = out_params->n;
  double current_variance=-1;

  int32_t type_uid=-1;
  F.fread(&type_uid,sizeof(int32_t));
  if (type_uid!=LWE_KEYSWITCH_KEY_TYPE_UID)
    die_dramatically("Trying to read something that is not a LWE Keyswitch!");
  //reads the variance only once in the end
  F.fread(&current_variance,sizeof(double));
  //and read the coefficients
  for (int i=0; i<N; i++)
    for (int j=0; j<t; j++)
      for (int k=0; k<base; k++) {
        LweSample<TORUS>& sample = ks->ks[i][j][k];
        //bin output the sample coefs
        F.fread(sample.a,n*sizeof(TORUS));
        F.fread(&sample.b,1*sizeof(TORUS));
        sample.current_variance=current_variance;
      }
}

/**
 * This function prints the keyswitch coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_lweKeySwitchKey(Ostream& F, const LweKeySwitchKey<TORUS>* ks, bool output_LweParams) {
  if (output_LweParams)
    write_lweParams(F, ks->out_params);
  write_LweKeySwitchParameters_section(F, ks);
  write_LweKeySwitchKey_content(F, ks);
}

/**
 * This function reads the keyswitch coefficients
 */
template<typename TORUS>
LweKeySwitchKey<TORUS>* IOFunctions<TORUS>::read_new_lweKeySwitchKey(Istream& F, const LweParams<TORUS>* out_params) {
  if (out_params==0) {
    LweParams<TORUS>* tmp = read_new_lweParams(F);
    out_params = tmp;
    TfheGarbageCollector<TORUS>::register_param(tmp);
  }
  LweKeySwitchParameters ksparams;
  read_lweKeySwitchParameters_section(F, &ksparams);
  LweKeySwitchKey<TORUS>* reps = new_LweKeySwitchKey<TORUS>(ksparams.n,ksparams.t,ksparams.basebit, out_params);
  read_lweKeySwitchKey_content(F, reps);
  return reps;
}

/* ****************************
 * Lwe Bootstrapping key
 **************************** */


/**
 * This function prints the bootstrapping the coefficients (tgsw array section only)
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_LweBootstrappingKey_content(Ostream& F, const LweBootstrappingKey<TORUS>* bk) {
  const int n = bk->in_out_params->n;
  const int kpl = bk->bk_params->kpl;
  const int k = bk->bk_params->tlwe_params->k;
  const int N = bk->bk_params->tlwe_params->N;
  double max_variance = -1;
  for (int i=0; i<n; i++)
    for (int j=0; j<kpl; j++) {
      TLweSample<TORUS>& sample = bk->bk[i].all_sample[j];
      if (sample.current_variance > max_variance)
        max_variance = sample.current_variance;
    }
  F.fwrite(&LWE_BOOTSTRAPPING_KEY_TYPE_UID, sizeof(int32_t));
  //print the variance once
  F.fwrite(&max_variance, sizeof(double));
  //then print all the coefficients
  for (int i=0; i<n; i++)
    for (int j=0; j<kpl; j++) {
      TLweSample<TORUS>& sample = bk->bk[i].all_sample[j];
      for (int l=0; l<=k; l++) {
        F.fwrite(sample.a[l].coefsT, N*sizeof(TORUS));
      }
    }
}

/**
 * This function reads the bootstrapping the coefficients (tgsw array section only)
 */
template<typename TORUS>
void IOFunctions<TORUS>::read_LweBootstrappingKey_content(Istream& F, LweBootstrappingKey<TORUS>* bk) {
  const int n = bk->in_out_params->n;
  const int kpl = bk->bk_params->kpl;
  const int k = bk->bk_params->tlwe_params->k;
  const int N = bk->bk_params->tlwe_params->N;
  double max_variance = -1;
  int32_t type_uid = -1;
  F.fread(&type_uid, sizeof(int32_t));
  if (type_uid!=LWE_BOOTSTRAPPING_KEY_TYPE_UID)
    die_dramatically("Trying to read something that is not a BK content");
  F.fread(&max_variance, sizeof(double));
  for (int i=0; i<n; i++)
    for (int j=0; j<kpl; j++) {
      TLweSample<TORUS>& sample = bk->bk[i].all_sample[j];
      for (int l=0; l<=k; l++) {
        F.fread(sample.a[l].coefsT, N*sizeof(TORUS));
      }
      sample.current_variance=max_variance;
    }
}



/**
 * This function prints the bootstrapping parameters to a generic stream
 * It only prints the parameters section, not the coefficients
 */
template<typename TORUS>
void IOFunctions<TORUS>::write_lweBootstrappingKey(Ostream& F, const LweBootstrappingKey<TORUS>* bk, bool write_inout_params, bool write_bk_params) {
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
template<typename TORUS>
LweBootstrappingKey<TORUS>* IOFunctions<TORUS>::read_new_lweBootstrappingKey(Istream& F, const LweParams<TORUS>* in_out_params, const TGswParams<TORUS>* bk_params) {
  if (in_out_params==0) {
    LweParams<TORUS>* tmp = read_new_lweParams(F);
    in_out_params = tmp;
    TfheGarbageCollector<TORUS>::register_param(tmp);
  }
  if (bk_params==0) {
    TGswParams<TORUS>* tmp = read_new_tGswParams(F);
    bk_params = tmp;
    TfheGarbageCollector<TORUS>::register_param(tmp);
  }
  LweKeySwitchParameters ksparams;
  read_lweKeySwitchParameters_section(F, &ksparams);
  if (ksparams.n != bk_params->tlwe_params->N*bk_params->tlwe_params->k)
    die_dramatically("Wrong dimension in bootstrapping key");
  LweBootstrappingKey<TORUS>* reps = new_LweBootstrappingKey<TORUS>(ksparams.t,ksparams.basebit, in_out_params, bk_params);
  read_lweKeySwitchKey_content(F, reps->ks);
  read_LweBootstrappingKey_content(F, reps);
  return reps;
}

/* ****************************
 * TFheGateBootstrappingParameterSet key
 **************************** */


template<typename TORUS>
void IOFunctions<TORUS>::write_tfheGateBootstrappingProperParameters_section(Ostream& F, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  TextModeProperties* props = new_TextModeProperties_blank();
  props->setTypeTitle("GATEBOOTSPARAMS");
  props->setProperty_long("ks_t", params->ks_t);
  props->setProperty_long("ks_basebit", params->ks_basebit);
  print_TextModeProperties_toOStream(F, props);
  delete_TextModeProperties(props);
}

template<typename TORUS>
void IOFunctions<TORUS>::read_tfheGateBootstrappingProperParameters_section(Istream& F, int& ks_t, int& ks_basebit) {
  TextModeProperties* props = new_TextModeProperties_fromIstream(F);
  if (props->getTypeTitle() != string("GATEBOOTSPARAMS")) abort();
  ks_t = props->getProperty_long("ks_t");
  ks_basebit = props->getProperty_double("ks_basebit");
  delete_TextModeProperties(props);
}

template<typename TORUS>
void IOFunctions<TORUS>::write_tfheGateBootstrappingParameters(Ostream& F, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  write_tfheGateBootstrappingProperParameters_section(F,params);
  write_lweParams(F, params->in_out_params);
  write_tGswParams(F, params->tgsw_params);
}

template<typename TORUS>
TFheGateBootstrappingParameterSet<TORUS>* IOFunctions<TORUS>::read_new_tfheGateBootstrappingParameters(Istream& F) {
  int ks_t,ks_basebit;
  read_tfheGateBootstrappingProperParameters_section(F,ks_t,ks_basebit);
  LweParams<TORUS>* in_out_params = read_new_lweParams(F);
  TGswParams<TORUS>* bk_params = read_new_tGswParams(F);
  TfheGarbageCollector<TORUS>::register_param(in_out_params);
  TfheGarbageCollector<TORUS>::register_param(bk_params);
  return new TFheGateBootstrappingParameterSet<TORUS>(ks_t,ks_basebit,in_out_params,bk_params);
}

/* ****************************
 * TFheGateBootstrappingCloudKeySet
 **************************** */

template<typename TORUS>
TFheGateBootstrappingCloudKeySet<TORUS>* IOFunctions<TORUS>::read_new_tfheGateBootstrappingCloudKeySet(Istream& F, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  if (params==0) {
    TFheGateBootstrappingParameterSet<TORUS>* tmp = read_new_tfheGateBootstrappingParameters(F);
    TfheGarbageCollector<TORUS>::register_param(tmp);
    params=tmp;
  }
  LweBootstrappingKey<TORUS>* bk = read_new_lweBootstrappingKey(F,params->in_out_params, params->tgsw_params);
  LweBootstrappingKeyFFT<TORUS>* bkFFT = new_LweBootstrappingKeyFFT<TORUS>(bk);
  return new TFheGateBootstrappingCloudKeySet<TORUS>(params, bk, bkFFT);
}

template<typename TORUS>
void IOFunctions<TORUS>::write_tfheGateBootstrappingCloudKeySet(Ostream& F, const TFheGateBootstrappingCloudKeySet<TORUS>* key, bool output_gbparams) {
  if (output_gbparams) write_tfheGateBootstrappingParameters(F, key->params);
  write_lweBootstrappingKey(F, key->bk, false, false);
}

/* ****************************
 * TFheGateBootstrappingSecretKeySet
 **************************** */

template<typename TORUS>
TFheGateBootstrappingSecretKeySet<TORUS>* IOFunctions<TORUS>::read_new_tfheGateBootstrappingSecretKeySet(Istream& F, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  if (params==0) {
    TFheGateBootstrappingParameterSet<TORUS>* tmp = read_new_tfheGateBootstrappingParameters(F);
    TfheGarbageCollector<TORUS>::register_param(tmp);
    params=tmp;
  }
  LweBootstrappingKey<TORUS>* bk = read_new_lweBootstrappingKey(F,params->in_out_params, params->tgsw_params);
  LweKey<TORUS>* lwe_key = read_new_lweKey(F,params->in_out_params);
  TGswKey<TORUS>* tgsw_key = read_new_tGswKey(F,params->tgsw_params);
  LweBootstrappingKeyFFT<TORUS>* bkFFT = new_LweBootstrappingKeyFFT<TORUS>(bk);
  return new TFheGateBootstrappingSecretKeySet<TORUS>(params, bk, bkFFT, lwe_key, tgsw_key);
}

template<typename TORUS>
void IOFunctions<TORUS>::write_tfheGateBootstrappingSecretKeySet(Ostream& F, const TFheGateBootstrappingSecretKeySet<TORUS>* key, bool output_gbparams) {
  if (output_gbparams) write_tfheGateBootstrappingParameters(F, key->params);
  write_lweBootstrappingKey(F, key->cloud.bk, false, false);
  write_lweKey(F, key->lwe_key, false);
  write_tGswKey(F, key->tgsw_key, false);
}


