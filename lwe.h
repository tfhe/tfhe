#ifndef LWE_H
#define LWE_H

struct LWEParams;

struct LWEKey;

struct LWESample;

struct LWEKeySwitchKey;

struct RingLWEParams;

struct RingLWEKey;

struct RingLWESamples;

struct RingGSWParams;

struct RingGSWKey;

struct RingGSWSamples;

struct LWEBootstrappingKey;

void lweKeyGen(LWEKey* result, const LWEParams* params);
void lweSymEncrypt(LWESample* result, double message, const LWEKey* key);
double lweSymDecrypt(const LWESample* sample, const LWEKey* key);

void lweLinearCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample sample);

// Ring
void ringLweKeyGen(LWEKey* result);
void ringLweSymEncrypt(LWESample* result, double message, const LWEKey* key);
double ringLweSymDecrypt(const LWESample* sample, const LWEKey* key);

void ringLwePolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

// GSW
void ringGswKeyGen(LWEKey* result);
void ringGswSymEncrypt(LWESample* result, double message, const LWEKey* key);
double ringGswSymDecrypt(const LWESample* sample, const LWEKey* key);

void ringGswPolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

//extractions Ring LWE -> LWE
void keyExtract(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
void sampleExtract(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> GSW
void gswKeyExtract(RingLWEKey* result, const RingGSWKey* key); //sans doute un param supplémentaire
void gswSampleExtract(RingLWESample* result, const RingGSWSample* x);

//bootstrapping
void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, double mu1, double mu0, const LWESample* x);


#endif //LWE_H
