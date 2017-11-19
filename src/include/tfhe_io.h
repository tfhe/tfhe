#ifndef TFHE_IO_H
#define TFHE_IO_H
///@file
///@brief This file declares the Input/Output functions

#include "tfhe_core.h"

#ifdef __cplusplus

#include <cstdio>
#include <iosfwd>

#else
#include <stdio.h>
#endif



/* ********************************************************
 * LWE 
******************************************************** */

/* ****************************
 * LWE params
**************************** */

/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweParams_toFile(FILE *F, const LweParams *lweparams);

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams *new_lweParams_fromFile(FILE *F);


#ifdef __cplusplus

/**
 * This function prints a lweParams to a stream
 */
EXPORT void export_lweParams_toStream(std::ostream &out, const LweParams *lweparams);

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams *new_lweParams_fromStream(std::istream &in);

#endif

/* ****************************
 * LWE sample
**************************** */

/**
 * This function prints the lwe sample to a file
 */
EXPORT void export_lweSample_toFile(FILE *F, const LweSample *lwesample, const LweParams *params);

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
EXPORT void import_lweSample_fromFile(FILE *F, LweSample *lwesample, const LweParams *params);

#ifdef __cplusplus

/**
 * This function prints the lwe sample to a stream
 */
EXPORT void export_lweSample_toStream(std::ostream &F, const LweSample *lwesample, const LweParams *params);

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
EXPORT void import_lweSample_fromStream(std::istream &in, LweSample *lwesample, const LweParams *params);

#endif

/* ****************************
 * LWE key
**************************** */

/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweKey_toFile(FILE *F, const LweKey *lwekey);

/**
 * This constructor function reads and creates a LWEKey from a File. The result
 * must be deleted with delete_lweKey();
 */
EXPORT LweKey *new_lweKey_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function prints the lwe parameters to a stream
 */
EXPORT void export_lweKey_toStream(std::ostream &F, const LweKey *lwekey);

/**
 * This constructor function reads and creates a LWEKey from a stream. The result
 * must be deleted with delete_lweKey();
 */
EXPORT LweKey *new_lweKey_fromStream(std::istream &in);

#endif


/* ********************************************************
 * TLWE 
******************************************************** */

/* ****************************
 * TLWE params
**************************** */

/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tLweParams_toFile(FILE *F, const TLweParams *tlweparams);

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams *new_tLweParams_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT void export_tLweParams_toStream(std::ostream &F, const TLweParams *tlweparams);

/**
 * This constructor function reads and creates a TLWEParams from a stream. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams *new_tLweParams_fromStream(std::istream &in);

#endif


/* ****************************
 * TLWE sample
**************************** */

/**
 * This function prints the tlwe sample to a file
 */
EXPORT void export_tlweSample_toFile(FILE *F, const TLweSample *tlwesample, const TLweParams *params);

/**
 * This function reads a TLWESample from a stream in an
 * already allocated tlwesample.
 */
EXPORT void import_tlweSample_fromFile(FILE *F, TLweSample *tlwesample, const TLweParams *params);

#ifdef __cplusplus

/**
 * This function prints the tlwe sample to a stream
 */
EXPORT void export_tlweSample_toStream(std::ostream &F, const TLweSample *tlwesample, const TLweParams *params);

/**
 * This function reads a TLWESample from a stream in an
 * already allocated tlwesample.
 */
EXPORT void import_tlweSample_fromStream(std::istream &in, TLweSample *tlwesample, const TLweParams *params);

#endif

/* ****************************
 * TLWE key
**************************** */

/**
 * This function prints the tlwe parameters to a file
 */
EXPORT void export_tlweKey_toFile(FILE *F, const TLweKey *tlwekey);

/**
 * This constructor function reads and creates a TLWEKey from a File. The result
 * must be deleted with delete_tlweKey();
 */
EXPORT TLweKey *new_tlweKey_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function prints the tlwe parameters to a stream
 */
EXPORT void export_tlweKey_toStream(std::ostream &F, const TLweKey *tlwekey);

/**
 * This constructor function reads and creates a TLWEKey from a stream. The result
 * must be deleted with delete_tlweKey();
 */
EXPORT TLweKey *new_tlweKey_fromStream(std::istream &F);

#endif




/* ********************************************************
 * TGSW 
******************************************************** */

/* ****************************
 * TGSW params
**************************** */

/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tGswParams_toFile(FILE *F, const TGswParams *tgswparams);

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams *new_tGswParams_fromFile(FILE *F);


#ifdef __cplusplus

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT void export_tGswParams_toStream(std::ostream &F, const TGswParams *tgswparams);

/**
 * This constructor function reads and creates a TGSWParams from a stream. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams *new_tGswParams_fromStream(std::istream &in);


#endif


/* ****************************
 * TGSW sample
**************************** */

/**
 * This function prints the tgsw sample to a file
 */
EXPORT void export_tgswSample_toFile(FILE *F, const TGswSample *tgswsample, const TGswParams *params);

/**
 * This function reads a TGSWSample from a stream in an
 * already allocated tgswsample.
 */
EXPORT void import_tgswSample_fromFile(FILE *F, TGswSample *tgswsample, const TGswParams *params);

#ifdef __cplusplus

/**
 * This function prints the tgsw sample to a stream
 */
EXPORT void export_tgswSample_toStream(std::ostream &F, const TGswSample *tgswsample, const TGswParams *params);

/**
 * This function reads a TGSWSample from a stream in an
 * already allocated tgswsample.
 */
EXPORT void import_tgswSample_fromStream(std::istream &F, TGswSample *tgswsample, const TGswParams *params);

#endif

/* ****************************
 * TGSW key
**************************** */

/**
 * This function prints the tgsw parameters to a file
 */
EXPORT void export_tgswKey_toFile(FILE *F, const TGswKey *tgswkey);

/**
 * This constructor function reads and creates a TGSWKey from a File. The result
 * must be deleted with delete_tgswKey();
 */
EXPORT TGswKey *new_tgswKey_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function prints the tgsw parameters to a stream
 */
EXPORT void export_tgswKey_toStream(std::ostream &F, const TGswKey *tgswkey);

/**
 * This constructor function reads and creates a TGSWKey from a stream. The result
 * must be deleted with delete_tgswKey();
 */
EXPORT TGswKey *new_tgswKey_fromStream(std::istream &F);

#endif

/* ****************************
 * Lwe Keyswitch key
**************************** */

/**
 * This function exports a lwe keyswitch key (in binary) to a file
 */
EXPORT void export_lweKeySwitchKey_toFile(FILE *F, const LweKeySwitchKey *ks);

/**
 * This constructor function reads and creates a LweKeySwitchKey from a File. The result
 * must be deleted with delete_LweKeySwitchKey();
 */
EXPORT LweKeySwitchKey *new_lweKeySwitchKey_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function exports a lwe keyswitch key (in binary) to a file
 */
EXPORT void export_lweKeySwitchKey_toStream(std::ostream &F, const LweKeySwitchKey *ks);

/**
 * This constructor function reads and creates a LweKeySwitchKey from a File. The result
 * must be deleted with delete_LweKeySwitchKey();
 */
EXPORT LweKeySwitchKey *new_lweKeySwitchKey_fromStream(std::istream &F);

#endif

/* ****************************
 * Lwe Bootstrapping key
**************************** */

/**
 * This function exports a lwe bootstrapping key (in binary) to a file
 */
EXPORT void export_lweBootstrappingKey_toFile(FILE *F, const LweBootstrappingKey *bk);

/**
 * This constructor function reads and creates a LweBootstrappingKey from a File. The result
 * must be deleted with delete_LweBootstrappingKey();
 */
EXPORT LweBootstrappingKey *new_lweBootstrappingKey_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function exports a lwe bootstrapping key (in binary) to a file
 */
EXPORT void export_lweBootstrappingKey_toStream(std::ostream &F, const LweBootstrappingKey *bk);

/**
 * This constructor function reads and creates a LweBootstrappingKey from a File. The result
 * must be deleted with delete_LweBootstrappingKey();
 */
EXPORT LweBootstrappingKey *new_lweBootstrappingKey_fromStream(std::istream &F);

#endif

/* ****************************
 * TFheGateBootstrappingParameterSet key
**************************** */

/**
 * This function prints the tfhe gate bootstrapping parameter set to a file
 */
EXPORT void export_tfheGateBootstrappingParameterSet_toFile(FILE *F, const TFheGateBootstrappingParameterSet *params);

/**
 * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
 * must be deleted with delete_tfheGateBootstrappingParameterSet();
 */
EXPORT TFheGateBootstrappingParameterSet *new_tfheGateBootstrappingParameterSet_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function prints the tfhe gate bootstrapping parameter set to a file
 */
EXPORT void
export_tfheGateBootstrappingParameterSet_toStream(std::ostream &F, const TFheGateBootstrappingParameterSet *params);

/**
 * This constructor function reads and creates a tfhe gate bootstrapping parameter set from a File. The result
 * must be deleted with delete_tfheGateBootstrappingParameterSet();
 */
EXPORT TFheGateBootstrappingParameterSet *new_tfheGateBootstrappingParameterSet_fromStream(std::istream &F);

#endif

/* ****************************
 * TFheGateBootstrappingCloudKeySet
**************************** */

/**
 * This function prints the tfhe gate bootstrapping cloud key to a file
 */
EXPORT void export_tfheGateBootstrappingCloudKeySet_toFile(FILE *F, const TFheGateBootstrappingCloudKeySet *params);

/**
 * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
 */
EXPORT TFheGateBootstrappingCloudKeySet *new_tfheGateBootstrappingCloudKeySet_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function prints the tfhe gate bootstrapping cloud key to a file
 */
EXPORT void
export_tfheGateBootstrappingCloudKeySet_toStream(std::ostream &F, const TFheGateBootstrappingCloudKeySet *params);

/**
 * This constructor function reads and creates a tfhe gate bootstrapping cloud key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingCloudKeySet();
 */
EXPORT TFheGateBootstrappingCloudKeySet *new_tfheGateBootstrappingCloudKeySet_fromStream(std::istream &F);

#endif

/* ****************************
 * TFheGateBootstrappingSecretKeySet
**************************** */

/**
 * This function prints the tfhe gate bootstrapping secret key to a file
 */
EXPORT void export_tfheGateBootstrappingSecretKeySet_toFile(FILE *F, const TFheGateBootstrappingSecretKeySet *params);

/**
 * This constructor function reads and creates a tfhe gate bootstrapping secret key from a File. The result
 * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
 */
EXPORT TFheGateBootstrappingSecretKeySet *new_tfheGateBootstrappingSecretKeySet_fromFile(FILE *F);

#ifdef __cplusplus

/**
 * This function prints the tfhe gate bootstrapping secret key to a file
 */
EXPORT void
export_tfheGateBootstrappingSecretKeySet_toStream(std::ostream &F, const TFheGateBootstrappingSecretKeySet *params);

/**
 * This constructor function reads and creates a TGSWKey from a stream. The result
 * must be deleted with delete_tfheGateBootstrappingSecretKeySet();
 */
EXPORT TFheGateBootstrappingSecretKeySet *new_tfheGateBootstrappingSecretKeySet_fromStream(std::istream &F);

#endif

/* ****************************
 * TFheGateBootstrappingCiphertext
**************************** */

/**
 * This function prints a gate bootstrapping ciphertext to a file
 * (wrapper to export LweSample)
 */
EXPORT void export_gate_bootstrapping_ciphertext_toFile(FILE *F, const LweSample *sample,
                                                        const TFheGateBootstrappingParameterSet *params);

/**
 * This function reads a tfhe gate bootstrapping ciphertext from a File.
 * wrapper to import LweSample
 */
EXPORT void import_gate_bootstrapping_ciphertext_fromFile(FILE *F, LweSample *sample,
                                                          const TFheGateBootstrappingParameterSet *params);

#ifdef __cplusplus

/**
 * This function prints a gate bootstrapping ciphertext to a file
 * (wrapper to export LweSample)
 */
EXPORT void export_gate_bootstrapping_ciphertext_toStream(std::ostream &F, const LweSample *sample,
                                                          const TFheGateBootstrappingParameterSet *params);

/**
 * This function reads a tfhe gate bootstrapping ciphertext from a File.
 * wrapper to import LweSample
 */
EXPORT void import_gate_bootstrapping_ciphertext_fromStream(std::istream &F, LweSample *sample,
                                                            const TFheGateBootstrappingParameterSet *params);

#endif


#endif // TFHE_IO_H

