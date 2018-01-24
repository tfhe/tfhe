#ifndef TFHE_GARBAGE_COLLECTOR_H
#define TFHE_GARBAGE_COLLECTOR_H

///@file
///@brief This file declares a small garbage collector used for constant parameters

#include <vector>
#include "tfhe_gate.h"

/**
 * this class implements a simple garbage collector for simple parameter pointers
 */
template<typename TORUS>
class TfheGarbageCollector {
  std::vector<LweParams<TORUS>*> lwe_params;
  std::vector<TLweParams<TORUS>*> tlwe_params;
  std::vector<TGswParams<TORUS>*> tgsw_params;
  std::vector<TFheGateBootstrappingParameterSet<TORUS>*> gb_params;

  void m_register_param(LweParams<TORUS>* param);
  void m_register_param(TLweParams<TORUS>* param);
  void m_register_param(TGswParams<TORUS>* param);
  void m_register_param(TFheGateBootstrappingParameterSet<TORUS>* param);

  static TfheGarbageCollector<TORUS>* global_tfheGarbageCollector;
  public:
  /** initialize the garbage collector (this will automatically be
   * called by register_param */
  static void init();
  /** adds a parameter to the list of managed parameters */
  static void register_param(LweParams<TORUS>* param);
  /** adds a parameter to the list of managed parameters */
  static void register_param(TLweParams<TORUS>* param);
  /** adds a parameter to the list of managed parameters */
  static void register_param(TGswParams<TORUS>* param);
  /** adds a parameter to the list of managed parameters */
  static void register_param(TFheGateBootstrappingParameterSet<TORUS>* param);

  /** destroy all managed parameters */
  static void finalize();

  TfheGarbageCollector();
  TfheGarbageCollector(const TfheGarbageCollector<TORUS>&)=delete;
  void operator=(const TfheGarbageCollector<TORUS>&)=delete;
  ~TfheGarbageCollector();

};

template struct TfheGarbageCollector<Torus32>;
template struct TfheGarbageCollector<Torus64>;


#endif //TFHE_GARBAGE_COLLECTOR_H

