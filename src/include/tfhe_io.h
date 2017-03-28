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

#ifdef __cplusplus
#include <cstdio>
#include <iostream>
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
EXPORT void export_lweParams_toFile(FILE* F, const LweParams* lweparams);

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromFile(FILE* F);


#ifdef __cplusplus

/**
 * This function prints a lweParams to a stream
 */
EXPORT void export_lweParams_toStream(std::ostream& out, const LweParams* lweparams);

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromStream(std::istream& in);

#endif

/* ****************************
 * LWE sample
**************************** */

/**
 * This function prints the lwe sample to a file
 */
EXPORT void export_lweSample_toFile(FILE* F, const LweSample* lwesample, const LweParams* params);

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
EXPORT void import_lweSample_fromFile(FILE* F, LweSample* lwesample, const LweParams* params);

#ifdef __cplusplus

/**
 * This function prints the lwe sample to a stream
 */
EXPORT void export_lweSample_toStream(std::ostream& F, const LweSample* lwesample, const LweParams* params);

/**
 * This function reads a LWESample from a stream in an
 * already allocated lwesample.
 */
EXPORT void import_lweSample_fromStream(std::istream& in, LweSample* lwesample, const LweParams* params);

#endif


/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweKey_toFile(FILE* F, const LweKey* lwekey);

/**
 * This constructor function reads and creates a LWEKey from a File. The result
 * must be deleted with delete_lweKey();
 */
EXPORT LweKey* new_lweKey_fromFile(FILE* F);

#ifdef __cplusplus

/**
 * This function prints the lwe parameters to a stream
 */
EXPORT void export_lweKey_toStream(std::ostream& F, const LweKey* lwekey);

/**
 * This constructor function reads and creates a LWEKey from a stream. The result
 * must be deleted with delete_lweKey();
 */
EXPORT LweKey* new_lweKey_fromStream(std::istream& in);

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
EXPORT void export_tLweParams_toFile(FILE* F, const TLweParams* tlweparams);

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams* new_tLweParams_fromFile(FILE* F);

#ifdef __cplusplus

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT void export_tLweParams_toStream(std::ostream& F, const TLweParams* tlweparams);

/**
 * This constructor function reads and creates a TLWEParams from a stream. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams* new_tLweParams_fromStream(std::istream& in);

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
EXPORT void export_tGswParams_toFile(FILE* F, const TGswParams* tgswparams);

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams* new_tGswParams_fromFile(FILE* F);



#ifdef __cplusplus

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT void export_tGswParams_toStream(std::ostream& F, const TGswParams* tgswparams);

/**
 * This constructor function reads and creates a TGSWParams from a stream. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams* new_tGswParams_fromStream(std::istream& in);


#endif






#endif // TFHE_IO_H

