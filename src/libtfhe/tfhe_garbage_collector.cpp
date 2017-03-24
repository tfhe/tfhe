#include "tfhe_garbage_collector.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"

using namespace std;

/**
 * this class implements a simple garbage collector for simple parameter pointers
 */
void TfheGarbageCollector::register_param(LweParams* param) { lwe_params.push_back(param); }
void TfheGarbageCollector::register_param(TLweParams* param) { tlwe_params.push_back(param); }
void TfheGarbageCollector::register_param(TGswParams* param) { tgsw_params.push_back(param); }

TfheGarbageCollector::TfheGarbageCollector() {}
TfheGarbageCollector::~TfheGarbageCollector() {
    for (LweParams* param: lwe_params) delete_LweParams(param);
    for (TLweParams* param: tlwe_params) delete_TLweParams(param);
    for (TGswParams* param: tgsw_params) delete_TGswParams(param);
    lwe_params.clear();
    tlwe_params.clear();
    tgsw_params.clear();
}

TfheGarbageCollector global_tfheGarbageCollector;
