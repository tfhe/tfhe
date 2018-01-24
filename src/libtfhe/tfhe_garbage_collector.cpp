#include "tfhe_garbage_collector.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"

using namespace std;

/**
 * this class implements a simple garbage collector for simple parameter pointers
 */

template<typename TORUS>
void TfheGarbageCollector<TORUS>::m_register_param(LweParams<TORUS>* param) { lwe_params.push_back(param); }

template<typename TORUS>
void TfheGarbageCollector<TORUS>::m_register_param(TLweParams<TORUS>* param) { tlwe_params.push_back(param); }

template<typename TORUS>
void TfheGarbageCollector<TORUS>::m_register_param(TGswParams<TORUS>* param) { tgsw_params.push_back(param); }

template<typename TORUS>
void TfheGarbageCollector<TORUS>::m_register_param(TFheGateBootstrappingParameterSet<TORUS>* param) { gb_params.push_back(param); }


template<typename TORUS>
void TfheGarbageCollector<TORUS>::register_param(LweParams<TORUS>* param) { init(); global_tfheGarbageCollector->m_register_param(param); }


template<typename TORUS>
void TfheGarbageCollector<TORUS>::register_param(TLweParams<TORUS>* param) { init(); global_tfheGarbageCollector->m_register_param(param); }

template<typename TORUS>
void TfheGarbageCollector<TORUS>::register_param(TGswParams<TORUS>* param) { init(); global_tfheGarbageCollector->m_register_param(param); }

template<typename TORUS>
void TfheGarbageCollector<TORUS>::register_param(TFheGateBootstrappingParameterSet<TORUS>* param) { init(); global_tfheGarbageCollector->m_register_param(param); }


template<typename TORUS>
void TfheGarbageCollector<TORUS>::init() { if (!global_tfheGarbageCollector) global_tfheGarbageCollector = new TfheGarbageCollector(); }

template<typename TORUS>
void TfheGarbageCollector<TORUS>::finalize() { if (global_tfheGarbageCollector) delete global_tfheGarbageCollector; global_tfheGarbageCollector=0; }


template<typename TORUS>
TfheGarbageCollector<TORUS>::TfheGarbageCollector() {}

template<typename TORUS>
TfheGarbageCollector<TORUS>::~TfheGarbageCollector() {
  for (LweParams<TORUS>* param: lwe_params) delete_LweParams(param);
  for (TLweParams<TORUS>* param: tlwe_params) delete_TLweParams(param);
  for (TGswParams<TORUS>* param: tgsw_params) delete_TGswParams(param);
  for (TFheGateBootstrappingParameterSet<TORUS>* param: gb_params) delete_gate_bootstrapping_parameters(param);
  lwe_params.clear();
  tlwe_params.clear();
  tgsw_params.clear();
  gb_params.clear();
}


template<typename TORUS>
TfheGarbageCollector<TORUS>* TfheGarbageCollector<TORUS>::global_tfheGarbageCollector(0);