#ifndef TFHE_IO_H
#define TFHE_IO_H

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

