#ifndef TFHE_IO_H
#define TFHE_IO_H
///@file
///@brief This file declares the Input/Output functions

#include "tfhe_core.h"
#include "lwekey.h"
#include "lweparams.h"
#include "lwesamples.h"
#include "tlwe.h"
#include "tgsw.h"
#include "tfhe_gate.h" //TODO proprify dependencies
#include <tfhe_generic_streams.h>

#include <cstdio>
#include <iostream>
#include <stdio.h>

template<typename TORUS>
struct IOFunctions
{
  /* ****************************
   * LWE params
  **************************** */
  /**
   * This function prints a lweParams to a stream
   */
  static void write_lweParams(Ostream& out, const LweParams<TORUS>* lweparams);

  /**
   * This constructor function reads and creates a LWEParams from a stream. The result
   * must be deleted with delete_lweParams();
   */
  static LweParams<TORUS>* read_new_lweParams(Istream& in);


  /* ****************************
   * LWE samples
  **************************** */
  /**
   * This function prints the lwe sample to a stream
   */
  static void write_lweSample(Ostream& F, const LweSample<TORUS>* lwesample, const LweParams<TORUS>* params);

  /**
   * This function reads a LWESample from a stream in an
   * already allocated lwesample.
   */
  static void read_lweSample(Istream& in, LweSample<TORUS>* lwesample, const LweParams<TORUS>* params);


  /* ****************************
   * LWE key
  **************************** */
  static void read_lweKey_content(Istream& F, LweKey<TORUS>* key);
  static void write_lweKey_content(Ostream& F, const LweKey<TORUS>* key);

  /**
   * This function prints the lwe parameters to a stream
   */
  static void write_lweKey(Ostream& F, const LweKey<TORUS>* lwekey, bool output_params=true);

  /**
   * This constructor function reads and creates a LWEKey from a stream. The result
   * must be deleted with delete_lweKey();
   */
  static LweKey<TORUS>* read_new_lweKey(Istream& F, const LweParams<TORUS>* params=0x0);


  /* ****************************
   * TLWE params
  **************************** */
  /**
   * This constructor function reads and creates a TLWEParams from a File. The result
   * must be deleted with delete_TLweParams();
   */
  static void write_tLweParams(Ostream& F, const TLweParams<TORUS>* tlweparams);

  /**
   * This constructor function reads and creates a TLWEParams from a stream. The result
   * must be deleted with delete_TLweParams();
   */
  static TLweParams<TORUS>* read_new_tLweParams(Istream& in);


  /* ****************************
   * TLWE samples
  **************************** */
  /**
   * This function prints the tlwe sample to a stream
   */
  static void write_tLweSample(Ostream& F, const TLweSample<TORUS>* tlwesample, const TLweParams<TORUS>* params);

  /**
   * This function reads a TLWESample from a stream in an
   * already allocated tlwesample.
   */
  static void read_tLweSample(Istream& in, TLweSample<TORUS>* tlwesample, const TLweParams<TORUS>* params);


  /* ****************************
   * TLWE key
  **************************** */
  static void read_tLweKey_content(Istream& F, TLweKey<TORUS>* key);
  static void write_tLweKey_content(Ostream& F, const TLweKey<TORUS>* key);

  /**
   * This function prints the tlwe parameters to a stream
   */
  static void write_tLweKey(Ostream& F, const TLweKey<TORUS>* tlwekey);

  /**
   * This constructor function reads and creates a TLWEKey from a stream. The result
   * must be deleted with delete_tlweKey();
   */
  static TLweKey<TORUS>* read_new_tLweKey(Istream& F);


  /* ****************************
   * TGSW params
  **************************** */
  static void write_tGswParams_section(Ostream& F, const TGswParams<TORUS>* tgswparams);
  static TGswParams<TORUS>* read_new_tGswParams_section(Istream& F, const TLweParams<TORUS>* tlwe_params);

  /**
   * This constructor function reads and creates a TGSWParams from a File. The result
   * must be deleted with delete_TGswParams();
   */
  static void write_tGswParams(Ostream& F, const TGswParams<TORUS>* tgswparams);

  /**
   * This constructor function reads and creates a TGSWParams from a stream. The result
   * must be deleted with delete_TGswParams();
   */
  static TGswParams<TORUS>* read_new_tGswParams(Istream& in);


  /* ****************************
   * TGSW samples
  **************************** */
  /**
   * This function prints the tgsw sample to a stream
   */
  static void write_tGswSample(Ostream& F, const TGswSample<TORUS>* tgswsample, const TGswParams<TORUS>* params);

  /**
   * This function reads a TGSWSample from a stream in an
   * already allocated tgswsample.
   */
  static void read_tGswSample(Istream& F, TGswSample<TORUS>* tgswsample, const TGswParams<TORUS>* params);


  /* ****************************
   * TGSW key
  **************************** */
  static void read_tGswKey_content(Istream& F, TGswKey<TORUS>* key);
  static void write_tGswKey_content(Ostream& F, const TGswKey<TORUS>* key);

  /**
   * This function prints the tgsw parameters to a stream
   */
  static void write_tGswKey(Ostream& F, const TGswKey<TORUS>* tgswkey, bool output_params=true);

  /**
   * This constructor function reads and creates a TGSWKey from a stream. The result
   * must be deleted with delete_tgswKey();
   */
  static TGswKey<TORUS>* read_new_tGswKey(Istream& F, const TGswParams<TORUS>* params=0x0);


  /* ****************************
   * Lwe Keyswitch key
   **************************** */
  struct LweKeySwitchParameters {
    int n;
    int t;
    int basebit;
  };

  static void write_LweKeySwitchParameters_section(Ostream& F, const LweKeySwitchKey<TORUS>* ks);
  static void read_lweKeySwitchParameters_section(Istream& F, LweKeySwitchParameters* reps);
  static void write_LweKeySwitchKey_content(Ostream& F, const LweKeySwitchKey<TORUS>* ks);
  static void read_lweKeySwitchKey_content(Istream& F, LweKeySwitchKey<TORUS>* ks);

  /**
   * This function exports a lwe keyswitch key (in binary) to a file
   */
  static void write_lweKeySwitchKey(Ostream& F, const LweKeySwitchKey<TORUS>* ks, bool output_LweParams=true);

  /**
   * This constructor function reads and creates a LweKeySwitchKey from a File. The result
   * must be deleted with delete_LweKeySwitchKey();
   */
  static LweKeySwitchKey<TORUS>* read_new_lweKeySwitchKey(Istream& F, const LweParams<TORUS>* out_params=0);


  /* ****************************
   * Lwe Bootstrapping key
   **************************** */
  static void write_LweBootstrappingKey_content(Ostream& F, const LweBootstrappingKey<TORUS>* bk);
  static void read_LweBootstrappingKey_content(Istream& F, LweBootstrappingKey<TORUS>* bk);

  /**
   * This function exports a lwe bootstrapping key (in binary) to a file
   */
  static void write_lweBootstrappingKey(Ostream& F, const LweBootstrappingKey<TORUS>* bk, bool write_inout_params=true, bool write_bk_params=true);

  /**
   * This constructor function reads and creates a LweBootstrappingKey from a File. The result
   * must be deleted with delete_LweBootstrappingKey();
   */
  static LweBootstrappingKey<TORUS>* read_new_lweBootstrappingKey(Istream& F, const LweParams<TORUS>* in_out_params=0, const TGswParams<TORUS>* bk_params=0);


  /* ****************************
   * TFheGateBootstrappingParameterSet key
   **************************** */
  static void write_tfheGateBootstrappingProperParameters_section(Ostream& F, const TFheGateBootstrappingParameterSet<TORUS>* params);
  static void read_tfheGateBootstrappingProperParameters_section(Istream& F, int& ks_t, int& ks_basebit);

  /**
   * This function prints the tfhe gate bootstrapping parameter set to a file
   */
  static void write_tfheGateBootstrappingParameters(Ostream& F, const TFheGateBootstrappingParameterSet<TORUS>* params);

  /**
   * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
   * must be deleted with delete_tfheGateBootstrappingParameterSet();
   */
  static TFheGateBootstrappingParameterSet<TORUS>* read_new_tfheGateBootstrappingParameters(Istream& F);


  /* ****************************
   * TFheGateBootstrappingCloudKeySet
   **************************** */
  /**
   * This function prints the tfhe gate bootstrapping cloud key to a file
   */
  static void write_tfheGateBootstrappingCloudKeySet(Ostream& F, const TFheGateBootstrappingCloudKeySet<TORUS>* params, bool output_gbparams=true);

  /**
   * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
   * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
   */
  static TFheGateBootstrappingCloudKeySet<TORUS>* read_new_tfheGateBootstrappingCloudKeySet(Istream& F, const TFheGateBootstrappingParameterSet<TORUS>* params=0);


  /* ****************************
   * TFheGateBootstrappingSecretKeySet
   **************************** */
  /**
   * This function prints the tfhe gate bootstrapping secret key to a file
   */
  static void write_tfheGateBootstrappingSecretKeySet(Ostream& F, const TFheGateBootstrappingSecretKeySet<TORUS>* params, bool output_gbparams=true);

  /**
   * This constructor function reads and creates a TGSWKey from a stream. The result
   * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
   */
  static TFheGateBootstrappingSecretKeySet<TORUS>* read_new_tfheGateBootstrappingSecretKeySet(Istream& F, const TFheGateBootstrappingParameterSet<TORUS>* params=0);
};

template struct IOFunctions<Torus32>;
template struct IOFunctions<Torus64>;


/* ********************************************************
 * LWE
******************************************************** */

/* ****************************
 * LWE params
**************************** */

/**
 * This function prints the lwe parameters to a file
 */
template<typename TORUS>
inline void export_lweParams_toFile(FILE* F, const LweParams<TORUS>* lweparams) {
  IOFunctions<TORUS>::write_lweParams(to_Ostream(F));
}

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
template<typename TORUS>
inline LweParams<TORUS>* new_lweParams_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_lweParams(to_Istream(F));
}

/**
 * This function prints a lweParams to a stream
 */
template<typename TORUS>
inline void export_lweParams_toStream(std::ostream& out, const LweParams<TORUS>* lweparams) {
  IOFunctions<TORUS>::write_lweParams(to_Ostream(out));
}

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
template<typename TORUS>
inline LweParams<TORUS>* new_lweParams_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_lweParams(to_Istream(in));
}

/* ****************************
 * LWE sample
**************************** */

/**
 * This function prints the lwe sample to a file
 */
template<typename TORUS>
inline void export_lweSample_toFile(FILE* F, const LweSample<TORUS>* lwesample, const LweParams<TORUS>* params) {
  IOFunctions<TORUS>::write_lweSample(to_Ostream(F));
}

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
template<typename TORUS>
inline void import_lweSample_fromFile(FILE* F, LweSample<TORUS>* lwesample, const LweParams<TORUS>* params) {
  IOFunctions<TORUS>::read_lweSample(to_Istream(F));
}

/**
 * This function prints the lwe sample to a stream
 */
template<typename TORUS>
inline void export_lweSample_toStream(std::ostream& out, const LweSample<TORUS>* lwesample, const LweParams<TORUS>* params) {
  IOFunctions<TORUS>::write_lweSample(to_Ostream(out));
}

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
template<typename TORUS>
inline void import_lweSample_fromStream(std::istream& in, LweSample<TORUS>* lwesample, const LweParams<TORUS>* params) {
  IOFunctions<TORUS>::read_lweSample(to_Istream(in));
}

/* ****************************
 * LWE key
**************************** */

/**
 * This function prints the lwe parameters to a file
 */
template<typename TORUS>
inline void export_lweKey_toFile(FILE* F, const LweKey<TORUS>* lwekey) {
  IOFunctions<TORUS>::write_lweKey(to_Ostream(F));
}

/**
 * This constructor function reads and creates a LWEKey from a File. The result
 * must be deleted with delete_lweKey();
 */
template<typename TORUS>
inline LweKey<TORUS>* new_lweKey_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_lweKey(to_Istream(F));
}

/**
 * This function prints the lwe parameters to a stream
 */
template<typename TORUS>
inline void export_lweKey_toStream(std::ostream& out, const LweKey<TORUS>* lwekey) {
  IOFunctions<TORUS>::write_lweKey(to_Ostream(out));
}

/**
 * This constructor function reads and creates a LWEKey from a stream. The result
 * must be deleted with delete_lweKey();
 */
template<typename TORUS>
inline LweKey<TORUS>* new_lweKey_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_lweKey(to_Istream(in));
}

/* ********************************************************
 * TLWE
******************************************************** */

/* ****************************
 * TLWE params
**************************** */

/**
 * This function prints the tLwe parameters to a file
 */
template<typename TORUS>
inline void export_tLweParams_toFile(FILE* F, const TLweParams<TORUS>* tlweparams) {
  IOFunctions<TORUS>::write_tLweParams(to_Ostream(F));
}

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
template<typename TORUS>
inline TLweParams<TORUS>* new_tLweParams_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tLweParams(to_Istream(F));
}


/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
template<typename TORUS>
inline void export_tLweParams_toStream(std::ostream& out, const TLweParams<TORUS>* tlweparams) {
  IOFunctions<TORUS>::write_tLweParams(to_Ostream(out));
}

/**
 * This constructor function reads and creates a TLWEParams from a stream. The result
 * must be deleted with delete_TLweParams();
 */
template<typename TORUS>
inline TLweParams<TORUS>* new_tLweParams_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tLweParams(to_Istream(in));
}

/* ****************************
 * TLWE sample
**************************** */

/**
 * This function prints the tlwe sample to a file
 */
template<typename TORUS>
inline void export_tlweSample_toFile(FILE* F, const TLweSample<TORUS>* tlwesample, const TLweParams<TORUS>* params) {
  IOFunctions<TORUS>::write_tLweSample(to_Ostream(F));
}

/**
 * This function reads a TLWESample from a stream in an
 * already allocated tlwesample.
 */
template<typename TORUS>
inline void import_tlweSample_fromFile(FILE* F, TLweSample<TORUS>* tlwesample, const TLweParams<TORUS>* params) {
  IOFunctions<TORUS>::read_tLweSample(to_Istream(F));
}

/**
 * This function prints the tlwe sample to a stream
 */
template<typename TORUS>
inline void export_tlweSample_toStream(std::ostream& out, const TLweSample<TORUS>* tlwesample, const TLweParams<TORUS>* params) {
  IOFunctions<TORUS>::write_tLweSample(to_Ostream(out));
}

/**
 * This function reads a TLWESample from a stream in an
 * already allocated tlwesample.
 */
template<typename TORUS>
inline void import_tlweSample_fromStream(std::istream& in, TLweSample<TORUS>* tlwesample, const TLweParams<TORUS>* params) {
  IOFunctions<TORUS>::read_tLweSample(to_Istream(in));
}

/* ****************************
 * TLWE key
**************************** */

/**
 * This function prints the tlwe parameters to a file
 */
template<typename TORUS>
inline void export_tlweKey_toFile(FILE* F, const TLweKey<TORUS>* tlwekey) {
  IOFunctions<TORUS>::write_tLweKey(to_Ostream(F));
}

/**
 * This constructor function reads and creates a TLWEKey from a File. The result
 * must be deleted with delete_tlweKey();
 */
template<typename TORUS>
inline TLweKey<TORUS>* new_tlweKey_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tLweKey(to_Istream(F));
}

/**
 * This function prints the tlwe parameters to a stream
 */
template<typename TORUS>
inline void export_tlweKey_toStream(std::ostream& out, const TLweKey<TORUS>* tlwekey) {
  IOFunctions<TORUS>::write_tLweKey(to_Ostream(out));
}

/**
 * This constructor function reads and creates a TLWEKey from a stream. The result
 * must be deleted with delete_tlweKey();
 */
template<typename TORUS>
inline TLweKey<TORUS>* new_tlweKey_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tLweKey(to_Istream(in));
}

/* ********************************************************
 * TGSW
******************************************************** */

/* ****************************
 * TGSW params
**************************** */

/**
 * This function prints the tLwe parameters to a file
 */
template<typename TORUS>
inline void export_tGswParams_toFile(FILE* F, const TGswParams<TORUS>* tgswparams) {
  IOFunctions<TORUS>::write_tGswParams(to_Ostream(F));
}

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
template<typename TORUS>
inline TGswParams<TORUS>* new_tGswParams_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tGswParams(to_Istream(F));
}

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
template<typename TORUS>
inline void export_tGswParams_toStream(std::ostream& out, const TGswParams<TORUS>* tgswparams) {
  IOFunctions<TORUS>::write_tGswParams(to_Ostream(out));
}

/**
 * This constructor function reads and creates a TGSWParams from a stream. The result
 * must be deleted with delete_TGswParams();
 */
template<typename TORUS>
inline TGswParams<TORUS>* new_tGswParams_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tGswParams(to_Istream(in));
}

/* ****************************
 * TGSW sample
**************************** */

/**
 * This function prints the tgsw sample to a file
 */
template<typename TORUS>
inline void export_tgswSample_toFile(FILE* F, const TGswSample<TORUS>* tgswsample, const TGswParams<TORUS>* params) {
  IOFunctions<TORUS>::write_tGswSample(to_Ostream(F));
}

/**
 * This function reads a TGSWSample from a stream in an
 * already allocated tgswsample.
 */
template<typename TORUS>
inline void import_tgswSample_fromFile(FILE* F, TGswSample<TORUS>* tgswsample, const TGswParams<TORUS>* params) {
  IOFunctions<TORUS>::read_tGswSample(to_Istream(F));
}


/**
 * This function prints the tgsw sample to a stream
 */
template<typename TORUS>
inline void export_tgswSample_toStream(std::ostream& out, const TGswSample<TORUS>* tgswsample, const TGswParams<TORUS>* params) {
  IOFunctions<TORUS>::write_tGswSample(to_Ostream(out));
}

/**
 * This function reads a TGSWSample from a stream in an
 * already allocated tgswsample.
 */
template<typename TORUS>
inline void import_tgswSample_fromStream(std::istream& in, TGswSample<TORUS>* tgswsample, const TGswParams<TORUS>* params) {
  IOFunctions<TORUS>::read_tGswSample(to_Istream(in));
}

/* ****************************
 * TGSW key
**************************** */

/**
 * This function prints the tgsw parameters to a file
 */
template<typename TORUS>
inline void export_tgswKey_toFile(FILE* F, const TGswKey<TORUS>* tgswkey) {
  IOFunctions<TORUS>::write_tGswKey(to_Ostream(F));
}

/**
 * This constructor function reads and creates a TGSWKey from a File. The result
 * must be deleted with delete_tgswKey();
 */
template<typename TORUS>
inline TGswKey<TORUS>* new_tgswKey_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tGswKey(to_Istream(F));
}

/**
 * This function prints the tgsw parameters to a stream
 */
template<typename TORUS>
inline void export_tgswKey_toStream(std::ostream& out, const TGswKey<TORUS>* tgswkey) {
  IOFunctions<TORUS>::write_tGswKey(to_Ostream(out));
}

/**
 * This constructor function reads and creates a TGSWKey from a stream. The result
 * must be deleted with delete_tgswKey();
 */
template<typename TORUS>
inline TGswKey<TORUS>* new_tgswKey_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tGswKey(to_Istream(in));
}

/* ****************************
 * Lwe Keyswitch key
**************************** */

/**
 * This function exports a lwe keyswitch key (in binary) to a file
 */
template<typename TORUS>
inline void export_lweKeySwitchKey_toFile(FILE* F, const LweKeySwitchKey<TORUS>* ks) {
  IOFunctions<TORUS>::write_lweKeySwitchKey(to_Ostream(F));
}

/**
 * This constructor function reads and creates a LweKeySwitchKey from a File. The result
 * must be deleted with delete_LweKeySwitchKey();
 */
template<typename TORUS>
inline LweKeySwitchKey<TORUS>* new_lweKeySwitchKey_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_lweKeySwitchKey(to_Istream(F));
}

/**
 * This function exports a lwe keyswitch key (in binary) to a file
 */
template<typename TORUS>
inline void export_lweKeySwitchKey_toStream(std::ostream& out, const LweKeySwitchKey<TORUS>* ks) {
  IOFunctions<TORUS>::write_lweKeySwitchKey(to_Ostream(out));
}

/**
 * This constructor function reads and creates a LweKeySwitchKey from a File. The result
 * must be deleted with delete_LweKeySwitchKey();
 */
template<typename TORUS>
inline LweKeySwitchKey<TORUS>* new_lweKeySwitchKey_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_lweKeySwitchKey(to_Istream(in));
}

/* ****************************
 * Lwe Bootstrapping key
**************************** */

/**
 * This function exports a lwe bootstrapping key (in binary) to a file
 */
template<typename TORUS>
inline void export_lweBootstrappingKey_toFile(FILE* F, const LweBootstrappingKey<TORUS>* bk) {
  IOFunctions<TORUS>::write_lweBootstrappingKey(to_Ostream(F));
}

/**
 * This constructor function reads and creates a LweBootstrappingKey from a File. The result
 * must be deleted with delete_LweBootstrappingKey();
 */
template<typename TORUS>
inline LweBootstrappingKey<TORUS>* new_lweBootstrappingKey_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_lweBootstrappingKey(to_Istream(F));
}

/**
 * This function exports a lwe bootstrapping key (in binary) to a file
 */
template<typename TORUS>
inline void export_lweBootstrappingKey_toStream(std::ostream& out, const LweBootstrappingKey<TORUS>* bk) {
  IOFunctions<TORUS>::write_lweBootstrappingKey(to_Ostream(out));
}

/**
 * This constructor function reads and creates a LweBootstrappingKey from a File. The result
 * must be deleted with delete_LweBootstrappingKey();
 */
template<typename TORUS>
inline LweBootstrappingKey<TORUS>* new_lweBootstrappingKey_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_lweBootstrappingKey(to_Istream(in));
}

/* ****************************
 * TFheGateBootstrappingParameterSet key
**************************** */

/**
 * This function prints the tfhe gate bootstrapping parameter set to a file
 */
template<typename TORUS>
inline void export_tfheGateBootstrappingParameterSet_toFile(FILE* F, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  IOFunctions<TORUS>::write_tfheGateBootstrappingParameters(to_Ostream(F));
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
 * must be deleted with delete_tfheGateBootstrappingParameterSet();
 */
template<typename TORUS>
inline TFheGateBootstrappingParameterSet<TORUS>* new_tfheGateBootstrappingParameterSet_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tfheGateBootstrappingParameters(to_Istream(F));
}

/**
 * This function prints the tfhe gate bootstrapping parameter set to a file
 */
template<typename TORUS>
inline void export_tfheGateBootstrappingParameterSet_toStream(std::ostream& out, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  IOFunctions<TORUS>::write_tfheGateBootstrappingParameters(to_Ostream(out));
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
 * must be deleted with delete_tfheGateBootstrappingParameterSet();
 */
template<typename TORUS>
inline TFheGateBootstrappingParameterSet<TORUS>* new_tfheGateBootstrappingParameterSet_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tfheGateBootstrappingParameters(to_Istream(in));
}

/* ****************************
 * TFheGateBootstrappingCloudKeySet
**************************** */

/**
 * This function prints the tfhe gate bootstrapping cloud key to a file
 */
template<typename TORUS>
inline void export_tfheGateBootstrappingCloudKeySet_toFile(FILE* F, const TFheGateBootstrappingCloudKeySet<TORUS>* params) {
  IOFunctions<TORUS>::write_tfheGateBootstrappingCloudKeySet(to_Ostream(F));
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
 */
template<typename TORUS>
inline TFheGateBootstrappingCloudKeySet<TORUS>* new_tfheGateBootstrappingCloudKeySet_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tfheGateBootstrappingCloudKeySet(to_Istream(F));
}

/**
 * This function prints the tfhe gate bootstrapping cloud key to a file
 */
template<typename TORUS>
inline void export_tfheGateBootstrappingCloudKeySet_toStream(std::ostream& out, const TFheGateBootstrappingCloudKeySet<TORUS>* params) {
  IOFunctions<TORUS>::write_tfheGateBootstrappingCloudKeySet(to_Ostream(out));
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
 */
template<typename TORUS>
inline TFheGateBootstrappingCloudKeySet<TORUS>* new_tfheGateBootstrappingCloudKeySet_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tfheGateBootstrappingCloudKeySet(to_Istream(in));
}

/* ****************************
 * TFheGateBootstrappingSecretKeySet
**************************** */

/**
 * This function prints the tfhe gate bootstrapping secret key to a file
 */
template<typename TORUS>
inline void export_tfheGateBootstrappingSecretKeySet_toFile(FILE* F, const TFheGateBootstrappingSecretKeySet<TORUS>* params) {
  IOFunctions<TORUS>::write_tfheGateBootstrappingSecretKeySet(to_Ostream(F));
}

/**
 * This constructor function reads and creates a tfhe gate bootstrapping secret key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
 */
template<typename TORUS>
inline TFheGateBootstrappingSecretKeySet<TORUS>* new_tfheGateBootstrappingSecretKeySet_fromFile(FILE* F) {
  IOFunctions<TORUS>::read_new_tfheGateBootstrappingSecretKeySet(to_Istream(F));
}

/**
 * This function prints the tfhe gate bootstrapping secret key to a file
 */
template<typename TORUS>
inline void export_tfheGateBootstrappingSecretKeySet_toStream(std::ostream& out, const TFheGateBootstrappingSecretKeySet<TORUS>* params) {
  IOFunctions<TORUS>::write_tfheGateBootstrappingSecretKeySet(to_Ostream(out));
}

/**
 * This constructor function reads and creates a TGSWKey from a stream. The result
 * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
 */
template<typename TORUS>
inline TFheGateBootstrappingSecretKeySet<TORUS>* new_tfheGateBootstrappingSecretKeySet_fromStream(std::istream& in) {
  IOFunctions<TORUS>::read_new_tfheGateBootstrappingSecretKeySet(to_Istream(in));
}

/* ****************************
 * TFheGateBootstrappingCiphertext
**************************** */

/**
 * This function prints a gate bootstrapping ciphertext to a file
 * (wrapper to export LweSample)
 */
template<typename TORUS>
inline void export_gate_bootstrapping_ciphertext_toFile(FILE* F, const LweSample<TORUS>* sample, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  IOFunctions<TORUS>::export_lweSample(to_Ostream(F),sample,params->in_out_params);
}

/**
 * This function reads a tfhe gate bootstrapping ciphertext from a File.
 * wrapper to import LweSample
 */
template<typename TORUS>
inline void import_gate_bootstrapping_ciphertext_fromFile(FILE* F, LweSample<TORUS>* sample, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  IOFunctions<TORUS>::import_lweSample(to_Istream(F),sample,params->in_out_params);
}

/**
 * This function prints a gate bootstrapping ciphertext to a file
 * (wrapper to export LweSample)
 */
template<typename TORUS>
inline void export_gate_bootstrapping_ciphertext_toStream(std::ostream& out, const LweSample<TORUS>* sample, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  IOFunctions<TORUS>::export_lweSample(to_Ostream(out),sample,params->in_out_params);
}

/**
 * This function reads a tfhe gate bootstrapping ciphertext from a File.
 * wrapper to import LweSample
 */
template<typename TORUS>
inline void import_gate_bootstrapping_ciphertext_fromStream(std::istream& in, LweSample<TORUS>* sample, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  IOFunctions<TORUS>::import_lweSample(to_Istream(in),sample,params->in_out_params);
}

#endif // TFHE_IO_H

