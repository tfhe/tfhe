#ifndef TFHE_GATE_FUNCTIONS_H
#define TFHE_GATE_FUNCTIONS_H

template<typename TORUS>
struct TfheGateFunctions
{
  /** generate a new unititialized ciphertext */
  static LweSample<TORUS>* new_ctxt(const TFheGateBootstrappingParameterSet<TORUS>* params);

  /** generate a new unititialized ciphertext array of length nbelems */
  static LweSample<TORUS>* new_ctxt_array(int nbelems, const TFheGateBootstrappingParameterSet<TORUS>* params);

  /** deletes a ciphertext */
  static void del_ctxt(LweSample<TORUS>* sample);

  /** deletes a ciphertext array of length nbelems */
  static void del_ctxt_array(int nbelems, LweSample<TORUS>* samples);

  /** encrypts a boolean */
  static void SymEncrypt(LweSample<TORUS>* result, int message, const TFheGateBootstrappingSecretKeySet<TORUS>* params);

  /** decrypts a boolean */
  static int SymDecrypt(const LweSample<TORUS>* sample, const TFheGateBootstrappingSecretKeySet<TORUS>* params);

  /** bootstrapped Constant (true or false) trivial Gate */
  static void CONSTANT(LweSample<TORUS>* result, int value, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);

  /** bootstrapped Nand Gate */
  static void NAND(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped Or Gate:  */
  static void OR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped And Gate: result = a and b */
  static void AND(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped Xor Gate: result = a xor b */
  static void XOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped Xnor Gate: result = (a==b) */
  static void XNOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped Not Gate: result = not(a) */
  static void NOT(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped Copy Gate: result = a */
  static void COPY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped Nor Gate: result = not(a or b) */
  static void NOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped AndNY Gate: not(a) and b */
  static void ANDNY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped AndYN Gate: a and not(b) */
  static void ANDYN(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped OrNY Gate: not(a) or b */
  static void ORNY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
  /** bootstrapped OrYN Gate: a or not(b) */
  static void ORYN(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);

  /** bootstrapped Mux(a,b,c) = a?b:c */
  static void MUX(LweSample<TORUS>* result, const LweSample<TORUS>* a, const LweSample<TORUS>* b, const LweSample<TORUS>* c, const TFheGateBootstrappingCloudKeySet<TORUS>* bk);
};

template struct TfheGateFunctions<Torus32>;
template struct TfheGateFunctions<Torus64>;


/** generate a new unititialized ciphertext */
template<typename TORUS>
inline LweSample<TORUS>* new_gate_bootstrapping_ciphertext(const TFheGateBootstrappingParameterSet<TORUS>* params) {
  return TfheGateFunctions<TORUS>::new_ctxt(params);
}

/** generate a new unititialized ciphertext array of length nbelems */
template<typename TORUS>
inline LweSample<TORUS>* new_gate_bootstrapping_ciphertext_array(int nbelems, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  return TfheGateFunctions<TORUS>::new_ctxt_array(nbelems, params);
}

/** deletes a ciphertext */
template<typename TORUS>
inline void delete_gate_bootstrapping_ciphertext(LweSample<TORUS>* sample) {
  TfheGateFunctions<TORUS>::del_ctxt(sample);
}

/** deletes a ciphertext array of length nbelems */
template<typename TORUS>
inline void delete_gate_bootstrapping_ciphertext_array(int nbelems, LweSample<TORUS>* samples) {
  TfheGateFunctions<TORUS>::del_ctxt_array(nbelems, samples);
}

/** encrypts a boolean */
template<typename TORUS>
inline void bootsSymEncrypt(LweSample<TORUS>* result, int message, const TFheGateBootstrappingSecretKeySet<TORUS>* params) {
  TfheGateFunctions<TORUS>::SymEncrypt(result, message, params);
}

/** decrypts a boolean */
template<typename TORUS>
inline int bootsSymDecrypt(const LweSample<TORUS>* sample, const TFheGateBootstrappingSecretKeySet<TORUS>* params) {
  return TfheGateFunctions<TORUS>::SymDecrypt(sample, params);
}

/** bootstrapped Constant (true or false) trivial Gate */
template<typename TORUS>
inline void bootsCONSTANT(LweSample<TORUS>* result, int value, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::CONSTANT(result, value, bk);
}

/** bootstrapped Nand Gate */
template<typename TORUS>
inline void bootsNAND(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::NAND(result, ca, cb, bk);
}
/** bootstrapped Or Gate:  */
template<typename TORUS>
inline void bootsOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::OR(result, ca, cb, bk);
}
/** bootstrapped And Gate: result = a and b */
template<typename TORUS>
inline void bootsAND(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::AND(result, ca, cb, bk);
}
/** bootstrapped Xor Gate: result = a xor b */
template<typename TORUS>
inline void bootsXOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::XOR(result, ca, cb, bk);
}
/** bootstrapped Xnor Gate: result = (a==b) */
template<typename TORUS>
inline void bootsXNOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::XNOR(result, ca, cb, bk);
}
/** bootstrapped Not Gate: result = not(a) */
template<typename TORUS>
inline void bootsNOT(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::NOT(result, ca, bk);
}
/** bootstrapped Copy Gate: result = a */
template<typename TORUS>
inline void bootsCOPY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::COPY(result, ca, bk);
}
/** bootstrapped Nor Gate: result = not(a or b) */
template<typename TORUS>
inline void bootsNOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::NOR(result, ca, cb, bk);
}
/** bootstrapped AndNY Gate: not(a) and b */
template<typename TORUS>
inline void bootsANDNY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::ANDNY(result, ca, cb, bk);
}
/** bootstrapped AndYN Gate: a and not(b) */
template<typename TORUS>
inline void bootsANDYN(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::ANDYN(result, ca, cb, bk);
}
/** bootstrapped OrNY Gate: not(a) or b */
template<typename TORUS>
inline void bootsORNY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::ORNY(result, ca, cb, bk);
}
/** bootstrapped OrYN Gate: a or not(b) */
template<typename TORUS>
inline void bootsORYN(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::ORYN(result, ca, cb, bk);
}

/** bootstrapped Mux(a,b,c) = a?b:c */
template<typename TORUS>
inline void bootsMUX(LweSample<TORUS>* result, const LweSample<TORUS>* a, const LweSample<TORUS>* b, const LweSample<TORUS>* c, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  TfheGateFunctions<TORUS>::MUX(result, a, b, c, bk);
}


#endif //TFHE_GATE_FUNCTIONS_H