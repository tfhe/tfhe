#ifndef TFHE_IO_H
#define TFHE_IO_H

#include "tfhe_core.h"
#include "lwekey.h"
#include "lweparams.h"
#include "lwesamples.h"

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif


/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweParams_toFile(FILE* F, const LweParams* lweparams);

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT const LweParams* new_lweParams_fromFile(FILE* F);


#ifdef __cplusplus
#include <iostream>

/**
 * This function prints a lweParams to a stream
 */
EXPORT void export_lweParams_toStream(std::ostream& out, const LweParams* lweparams);

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT const LweParams* new_lweParams_fromStream(std::istream& in);

#endif

#endif // TFHE_IO_H

