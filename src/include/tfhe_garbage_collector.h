#ifndef TFHE_GARBAGE_COLLECTOR_H
#define TFHE_GARBAGE_COLLECTOR_H

///@file
///@brief This file declares a small garbage collector used for constant parameters

#ifndef __cplusplus
#error This file should only be included in a C++ file, for internal use only
#endif
#include <vector>
#include "lweparams.h"

/**
 * this class implements a simple garbage collector for simple parameter pointers
 */
class TfheGarbageCollector {
    std::vector<LweParams*> lwe_params;
    std::vector<TLweParams*> tlwe_params;
    std::vector<TGswParams*> tgsw_params;
    public:
    void register_param(LweParams* param); 
    void register_param(TLweParams* param);
    void register_param(TGswParams* param);

    TfheGarbageCollector();
    TfheGarbageCollector(const TfheGarbageCollector&)=delete;
    void operator=(const TfheGarbageCollector&)=delete;
    ~TfheGarbageCollector();
};

extern TfheGarbageCollector global_tfheGarbageCollector;

#endif //TFHE_GARBAGE_COLLECTOR_H

