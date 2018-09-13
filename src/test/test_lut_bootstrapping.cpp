#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <random>
#include <chrono>
#include "tfhe_core.h"
#include "tfhe_gate.h"
#include "tfhe_functions.h"
#include "tfhe_alloc.h"
#include "lwebootstrappingkey.h"

// #define VERBOSE
// #define VERBOSE_ACC

using namespace std;

#define Torus Torus64

struct TFheLutBootstrappingParameterSet: public TFheGateBootstrappingParameterSet<Torus>
{
  const uint nb_inps;
  const uint msg_space_size;
  const uint msg_space_size_2;

  TFheLutBootstrappingParameterSet(
    const uint nb_inps,
    const int ks_t,
    const int ks_basebit,
    const LweParams<Torus>* const params_in_out,
    const TGswParams<Torus>* const params_bk)
    :
      TFheGateBootstrappingParameterSet<Torus>::TFheGateBootstrappingParameterSet(ks_t, ks_basebit, params_in_out, params_bk),
      nb_inps(nb_inps),
      msg_space_size(1<<nb_inps),
      msg_space_size_2(msg_space_size<<1)
    { }
};

struct TFheLutBootstrappingCloudKeySet: public TFheGateBootstrappingCloudKeySet<Torus> {
  const TFheLutBootstrappingParameterSet* const params;
  TFheLutBootstrappingCloudKeySet(
    const TFheLutBootstrappingParameterSet* const params,
    const LweBootstrappingKey<Torus>* const bk,
    const LweBootstrappingKeyFFT<Torus>* const bkFFT):
  TFheGateBootstrappingCloudKeySet(params, bk, bkFFT), params(params) {}
};

struct TFheLutBootstrappingSecretKeySet: public TFheGateBootstrappingSecretKeySet<Torus> {
  const TFheLutBootstrappingParameterSet* params;
  TFheLutBootstrappingSecretKeySet(
    const TFheLutBootstrappingParameterSet* const params,
    const LweBootstrappingKey<Torus>* const bk,
    const LweBootstrappingKeyFFT<Torus>* const bkFFT,
    const LweKey<Torus>* lwe_key,
    const TGswKey<Torus>* tgsw_key):
  TFheGateBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key),
  params(params) {}
};


static TFheLutBootstrappingSecretKeySet* keyset = NULL;
static TFheLutBootstrappingParameterSet* params = NULL;

constexpr inline double mulBySqrtTwoOverPi(double x) { return x*sqrt(2./M_PI); }

TFheLutBootstrappingParameterSet* new_default_lut_bootstrapping_parameters(uint nb_inps) {
    // const int N = 1024;
    const int N = 1024*16;
    const int k = 1;
    const int n = 803;

    const int bk_l = 8;
    const int bk_Bgbit = 6;

    const int ks_t = 4;
    const int ks_basebit = 4;

    const double ks_stdev = pow(2.,-20);   //standard deviation
    const double bk_stdev = pow(2.,-50);          //standard deviation
    const double max_stdev = mulBySqrtTwoOverPi(pow(2.,-8)/4.);

    // const int nb_divs = (1<<(nb_inps+2));
    // const double ks_stdev = pow(2.,-nb_divs-200);   //standard deviation
    // const double bk_stdev = pow(2.,-500);          //standard deviation
    // const double max_stdev = mulBySqrtTwoOverPi(pow(2.,-8)/4.);

    LweParams<Torus>* params_in_out = new_obj<LweParams<Torus>>(n, ks_stdev, max_stdev);
    TLweParams<Torus>* params_accum = new_obj<TLweParams<Torus>>(N, k, bk_stdev, max_stdev);
    TGswParams<Torus>* params_bk = new_obj<TGswParams<Torus>>(bk_l, bk_Bgbit, params_accum);

    return new TFheLutBootstrappingParameterSet(nb_inps, ks_t, ks_basebit, params_in_out, params_bk);
}

void delete_lut_bootstrapping_parameters(TFheLutBootstrappingParameterSet* params) {
  delete (TFheGateBootstrappingParameterSet<Torus>*)params;
}

TFheLutBootstrappingSecretKeySet* new_random_lut_bootstrapping_secret_keyset(const TFheLutBootstrappingParameterSet* params) {
  LweKey<Torus>* lwe_key = new_obj<LweKey<Torus>>(params->in_out_params);
  printf("\tLWE key generation\n");
  lweKeyGen(lwe_key);
  TGswKey<Torus>* tgsw_key = new_obj<TGswKey<Torus>>(params->tgsw_params);
  printf("\tTGSW key generation\n");
  tGswKeyGen(tgsw_key);

  printf("\tBootstrapping key generation\n");
  LweBootstrappingKey<Torus>* bk = new_obj<LweBootstrappingKey<Torus>>(params->ks_t, params->ks_basebit, params->in_out_params, params->tgsw_params);
  TfheFunctions<Torus>::createLweBootstrappingKey(bk, lwe_key, tgsw_key);
  LweBootstrappingKeyFFT<Torus>* bkFFT = new_obj<LweBootstrappingKeyFFT<Torus>>(bk);
  return new TFheLutBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key);
}

void delete_lut_bootstrapping_secret_keyset(TFheLutBootstrappingSecretKeySet* keyset) {
    LweKey<Torus>* lwe_key = (LweKey<Torus>*) keyset->lwe_key;
    TGswKey<Torus>* tgsw_key = (TGswKey<Torus>*) keyset->tgsw_key;
    LweBootstrappingKey<Torus>* bk = (LweBootstrappingKey<Torus>*) keyset->cloud.bk;
    LweBootstrappingKeyFFT<Torus>* bkFFT = (LweBootstrappingKeyFFT<Torus>*) keyset->cloud.bkFFT;
    if (bkFFT) del_obj(bkFFT);
    if (bk) del_obj(bk);
    del_obj(tgsw_key);
    del_obj(lwe_key);
    delete keyset;
}

void bootsSymEncrypt(LweSample<Torus>* result, int message, const TFheLutBootstrappingSecretKeySet* key) {
  // assert(0 <= message and message < key->params->msg_space_size);
  Torus tmsg = TorusUtils<Torus>::modSwitchToTorus(message, key->params->msg_space_size_2);
  double alpha = key->params->in_out_params->alpha_min;
  lweSymEncrypt(result, tmsg, alpha, key->lwe_key);
}

int bootsSymDecrypt(const LweSample<Torus>* sample, const LweKey<Torus>* lwe_key, const uint msg_space_size_2) {
  Torus phase = lwePhase(sample, lwe_key);
  return TorusUtils<Torus>::modSwitchFromTorus(phase, msg_space_size_2);
}

int bootsSymDecrypt(const LweSample<Torus>* sample, const TFheLutBootstrappingSecretKeySet* key) {
  return bootsSymDecrypt(sample, key->lwe_key, key->params->msg_space_size_2);
}


void print_decrypt_tlwe_sample(const TLweSample<Torus>* acc)
{
  const int N=keyset->cloud.bkFFT->accum_params->N;
  TorusPolynomial<Torus>* acc_dec = new_obj<TorusPolynomial<Torus>>(N);
  tLwePhase(acc_dec, acc, &(keyset->tgsw_key->tlwe_key));
  for (int i = 0; i < N; ++i)
    printf("%ld ", acc_dec->coefsT[i]);
  printf("\n");
}

void print_torus_poly(const TorusPolynomial<Torus>* poly)
{
  for (int i = 0; i < poly->N; ++i)
    printf("%ld ", poly->coefsT[i]);
  printf("\n");
}

Torus phase_lwe_sample_non_extracted(const LweSample<Torus>* sample) {
  const TLweKey<Torus>* accum_key = &(keyset->tgsw_key->tlwe_key);
  LweKey<Torus>* extracted_key = new_obj<LweKey<Torus>>(&(keyset->tgsw_key->params->tlwe_params->extracted_lweparams));
  tLweExtractKey(extracted_key, accum_key);
  return lwePhase(sample, extracted_key);
}

int decrypt_lwe_sample_non_extracted(const LweSample<Torus>* sample) {
  const TLweKey<Torus>* accum_key = &(keyset->tgsw_key->tlwe_key);
  LweKey<Torus>* extracted_key = new_obj<LweKey<Torus>>(&(keyset->tgsw_key->params->tlwe_params->extracted_lweparams));
  tLweExtractKey(extracted_key, accum_key);
  return bootsSymDecrypt(sample, extracted_key, params->msg_space_size_2);
}


void tfhe_blindrotate_lut_FFT(
    TLweSample<Torus>* acc,
    const Torus mu,
    const LweSample<Torus>* input,
    const LweBootstrappingKeyFFT<Torus>* bk
    )
{
  const TLweParams<Torus>* accum_params = bk->accum_params;
  const int N=accum_params->N;
  const uint Nx2=N<<1;
  const int n = bk->in_out_params->n;

  // Modulus switching
  int* bara = new int[N];
  int barb = TorusUtils<Torus>::modSwitchFromTorus(input->b,Nx2);
  uint sum = barb;
  Torus phase = input->b;
  // printf("%4d barb=%4d sum=%4d\n", -1, barb, sum);
  for (int i=0; i<n; i++) {
    bara[i]=TorusUtils<Torus>::modSwitchFromTorus(input->a[i],Nx2);
    sum -= bara[i] * keyset->lwe_key->key[i];
    phase -= input->a[i] * keyset->lwe_key->key[i];
    sum %= Nx2;
  }
#ifdef VERBOSE
  int64_t phase_bar = TorusUtils<Torus>::modSwitchFromTorus(phase, Nx2);
  printf("\nbarb=%d bara[0]=%d\n", barb, bara[0]);
  printf("sum(bara*sk)-barb=%d phase=%ld barphase=%d error=%d\n", sum, phase, phase_bar, sum - phase_bar);
  // printf("barphase=%ld barphase<<53=%ld\n", phase_bar, phase_bar<<53);
#endif

  // the initial testvec = [mu,mu,mu,...,mu]
  TorusPolynomial<Torus>* testvect_init = new_obj<TorusPolynomial<Torus>>(N);
  for (int i=0;i<N;i++) testvect_init->coefsT[i]=mu;

  // testvector = X^{-barb}*v
  TorusPolynomial<Torus>* testvect = new_obj<TorusPolynomial<Torus>>(N);
  torusPolynomialMulByXai(testvect, (-barb)%Nx2, testvect_init);
  del_obj(testvect_init);

  // Accumulator blind rotate by \sum bara_i * sk_i
  tLweNoiselessTrivial(acc, testvect, accum_params);
  tfhe_blindRotate_FFT(acc, bk->bkFFT, bara, n, bk->bk_params);
  del_obj(testvect);

  delete[] bara;
}

void tfhe_bootstrap_lut_FFT_no_KS(LweSample<Torus>* outputs,
    const LweBootstrappingKeyFFT<Torus>* bk,
    const Torus mu,
    const LweSample<Torus>* input,
    const int nb_divisions,
    const Torus* const divisions,   //size = nb_divisions
    const int nb_luts,
    const int** const gammas                 //size = nb_luts x nb_divisions
    )
{
  const TLweParams<Torus>* accum_params = bk->accum_params;
  const LweParams<Torus>* extract_params = &accum_params->extracted_lweparams;
  const uint N=accum_params->N;
  const uint Nx2=N<<1;

  TLweSample<Torus>* acc = new_obj<TLweSample<Torus>>(accum_params);
  tfhe_blindrotate_lut_FFT(acc, mu, input, bk);

#ifdef VERBOSE_ACC
  printf("ACC:\n");
  print_decrypt_tlwe_sample(acc);
  printf("\n");
#endif

  //Compute bart[j] = round(2*N*t[j])
  int* bart = new int[nb_divisions];
  for (int i = 0; i < nb_divisions; ++i) {
    bart[i] = TorusUtils<Torus>::modSwitchFromTorus(divisions[i], Nx2);
  }

  LweSample<Torus>** acc_coefs = new LweSample<Torus>*[nb_divisions] {NULL};

  for (int idx_lut = 0; idx_lut < nb_luts; ++idx_lut) {
    int sum_gamma = 0;
    for (int i = 0; i < nb_divisions; ++i) sum_gamma += gammas[idx_lut][i];

    LweSample<Torus>* result = outputs+idx_lut;
    lweNoiselessTrivial(result, mu * sum_gamma, extract_params);

    for (int i = 0; i < nb_divisions; ++i) {
      if (gammas[idx_lut][i] == 0) continue;

      if (acc_coefs[i] == NULL) {
        TLweSample<Torus>* acc_cpy = new_obj<TLweSample<Torus>>(accum_params);
        TLweFunctions<Torus>::MulByXai(acc_cpy, bart[i], acc, accum_params);

#ifdef VERBOSE_ACC
        printf("acc_coefs[%d] bart=%d:\n", i, bart[i]);
        print_decrypt_tlwe_sample(acc_cpy);
        printf("\n");
#endif

        acc_coefs[i] = new_obj<LweSample<Torus>>(extract_params);
        tLweExtractLweSample(acc_coefs[i], acc_cpy, extract_params, accum_params);
        del_obj(acc_cpy);
      }

      // lweAddMulTo(result, gammas[idx_lut][i], acc_coefs[i], extract_params);

#ifdef VERBOSE
      printf("Aresult before %ld, coef %ld, gamma %d\n", phase_lwe_sample_non_extracted(result), phase_lwe_sample_non_extracted(acc_coefs[i]), gammas[idx_lut][i]);
      printf("Bresult before %d, coef %d\n", decrypt_lwe_sample_non_extracted(result), decrypt_lwe_sample_non_extracted(acc_coefs[i]), gammas[idx_lut][i]);
#endif
      if (gammas[idx_lut][i] == 1) {
        lweAddTo(result, acc_coefs[i], extract_params);
      } else if (gammas[idx_lut][i] == -1) {
        lweSubTo(result, acc_coefs[i], extract_params);
      }

#ifdef VERBOSE
      printf("Aresult after %ld\n", phase_lwe_sample_non_extracted(result));
      printf("Bresult after %d\n", decrypt_lwe_sample_non_extracted(result));
#endif
    }
  }

  for (int i = 0; i < nb_divisions; ++i)
    if (acc_coefs[i] != NULL)
      del_obj(acc_coefs[i]);
  delete[] acc_coefs;
  delete[] bart;
  del_obj(acc);
}


void test_enc_dec() {
  printf("Test encryption/decryption\n");

  const TFheLutBootstrappingParameterSet* params = keyset->params;
  const LweParams<Torus>* in_out_params = params->in_out_params;

  default_random_engine generator(42);
  uniform_int_distribution<int> distribution(0,params->msg_space_size-1);
  for (int i = 0; i < 10; ++i) {
    int msg = distribution(generator);
    LweSample<Torus>* tmp = new_obj<LweSample<Torus>>(in_out_params);
    bootsSymEncrypt(tmp, msg, keyset);
    int msg_decr = bootsSymDecrypt(tmp, keyset);
    if (i<4 or msg != msg_decr) {
      printf("\n");
      printf("Encrypting message %d\n", msg);
      printf("Decrypting it: %d\n", msg_decr);
      Torus phase = lwePhase(tmp, keyset->lwe_key);
      printf("Phase: %f\n", TorusUtils<Torus>::to_double(phase));
    }
    del_obj(tmp);
  }
}


void comp_gamma_for_func(int* gamma, const int* const func_val, const uint msg_space) {
  gamma[0] = func_val[0];
  for (uint i = 1; i < msg_space; ++i)
    gamma[i] = func_val[i]-func_val[i-1];
}

void init_lut_boots(Torus& mu, Torus*& divisions, const uint msg_space_size) {
  double mu_f = 1/2./msg_space_size;
  mu = TorusUtils<Torus>::from_double(mu_f);

  printf("mu = %lf (%ld) msg_space %d\n", mu_f, mu/2, msg_space_size);

  printf("Divisions: ");
  for (uint i = 0; i < msg_space_size; ++i) {
    printf("%lf ", (mu_f*i) - mu_f/2);
    divisions[i] = TorusUtils<Torus>::from_double((i*mu_f) - mu_f/2);
  }
  printf("\n");
}

void create_random_luts(int**& lut_vals, int**& gammas, const uint nb_luts, const uint msg_space, int seed=42) {
  default_random_engine generator(seed);
  uniform_int_distribution<int> distribution(0,1);

  for (uint i = 0; i < nb_luts; ++i) {
    for (uint j = 0; j < msg_space; ++j)
      lut_vals[i][j] = distribution(generator);
    comp_gamma_for_func(gammas[i], lut_vals[i], msg_space);
  }
}

void print_lut_vals(int** const lut_vals, const uint nb_luts, const uint msg_space) {
  for (uint j = 0; j < msg_space; ++j) {
    printf("%4d: ", j);
    for (uint i = 0; i < nb_luts; ++i)
      printf("%d ", lut_vals[i][j]);
    printf("\n");
  }
}

void test_lut_boots(uint nb_luts) {
  printf("Test LUT bootstrapping\n");

  const uint msg_space_size = params->msg_space_size;

  const TFheLutBootstrappingParameterSet* params = keyset->params;
  const LweParams<Torus>* in_out_params = params->in_out_params;
  const LweBootstrappingKeyFFT<Torus>* bk = keyset->cloud.bkFFT;
  const TLweParams<Torus>* accum_params = bk->accum_params;


  Torus mu;
  Torus* divisions = new Torus[msg_space_size];;
  init_lut_boots(mu, divisions, msg_space_size);

  int** lut_vals = new int*[nb_luts];
  int** gammas = new int*[nb_luts];
  for (uint i = 0; i < nb_luts; ++i) {
    lut_vals[i] = new int[msg_space_size];
    gammas[i] = new int[msg_space_size];
  }
  create_random_luts(lut_vals, gammas, nb_luts, msg_space_size);
#ifdef VERBOSE
  printf("lut:\n");
  for (int i = 0; i < msg_space_size; ++i)
    printf("%2d ", lut_vals[0][i]);
  printf("\n");
  printf("gammas:\n");
  for (int i = 0; i < msg_space_size; ++i)
    printf("%2d ", gammas[0][i]);
  printf("\n");
#endif

  printf("Randomly generated LUTs truth tables:\n");
  print_lut_vals(lut_vals, nb_luts, msg_space_size);

  int** lut_vals_boots = new int*[nb_luts];
  for (uint i = 0; i < nb_luts; ++i) lut_vals_boots[i] = new int[msg_space_size];

  LweSample<Torus>* input = new_obj<LweSample<Torus>>(in_out_params);
  LweSample<Torus>* outputs = new_obj_array<LweSample<Torus>>(nb_luts, &accum_params->extracted_lweparams);
  for (uint msg_inp = 0; msg_inp < msg_space_size; ++msg_inp)
  {
    bootsSymEncrypt(input, msg_inp, keyset);

#ifdef VERBOSE_ACC
    if (msg_inp != 0) continue;
    // printf("msg %d: \n", msg_inp);
#endif

    tfhe_bootstrap_lut_FFT_no_KS(outputs,
      keyset->cloud.bkFFT,
      mu/2,
      input,
      msg_space_size,
      divisions,
      nb_luts,
      (const int**)gammas
      );

    for (uint i = 0; i < nb_luts; ++i) {
      LweSample<Torus>* tmp = new_obj<LweSample<Torus>>(in_out_params);
      lweKeySwitch(tmp, bk->ks, outputs+i);
      lut_vals_boots[i][msg_inp] = bootsSymDecrypt(tmp, keyset);
    }
    printf(".");
    fflush(stdout);
  }
  printf("\n");
  del_obj(input);
  del_obj_array(nb_luts, outputs);

  printf("LUT bootstrap computed truth tables:\n");
  print_lut_vals(lut_vals_boots, nb_luts, msg_space_size);

  uint error_cnt = 0;
  for (uint i = 0; i < nb_luts; ++i)
    for (uint j = 0; j < msg_space_size; ++j)
      error_cnt += (lut_vals[i][j] != lut_vals_boots[i][j]);
  if (error_cnt > 0) {
    printf("ERROR: number of errors = %d\n", error_cnt);
  } else {
    printf("No errors found\n");
  }

  for (uint i = 0; i < nb_luts; ++i) {
    delete[] gammas[i];
    delete[] lut_vals[i];
    delete[] lut_vals_boots[i];
  }
  delete[] gammas;
  delete[] lut_vals;
  delete[] lut_vals_boots;
  delete[] divisions;
}


void test_lut_boots_performance_no_KS(uint nb_luts, uint iter_cnt) {
  printf("Test LUT bootstrapping no KS timings\n");

  const uint msg_space_size = params->msg_space_size;

  const TFheLutBootstrappingParameterSet* params = keyset->params;
  const LweParams<Torus>* in_out_params = params->in_out_params;
  const LweBootstrappingKeyFFT<Torus>* bk = keyset->cloud.bkFFT;
  const TLweParams<Torus>* accum_params = bk->accum_params;


  Torus mu;
  Torus* divisions = new Torus[msg_space_size];;
  init_lut_boots(mu, divisions, msg_space_size);

  int** lut_vals = new int*[nb_luts];
  int** gammas = new int*[nb_luts];
  for (uint i = 0; i < nb_luts; ++i) {
    lut_vals[i] = new int[msg_space_size];
    gammas[i] = new int[msg_space_size];
  }
  create_random_luts(lut_vals, gammas, nb_luts, msg_space_size);

  LweSample<Torus>* input = new_obj<LweSample<Torus>>(in_out_params);
  bootsSymEncrypt(input, 1, keyset);

  LweSample<Torus>* outputs = new_obj_array<LweSample<Torus>>(nb_luts, &accum_params->extracted_lweparams);

  auto start = chrono::system_clock::now();
  for (uint iter = 0; iter < iter_cnt; ++iter)
  {
    tfhe_bootstrap_lut_FFT_no_KS(outputs,
      keyset->cloud.bkFFT,
      mu/2,
      input,
      msg_space_size,
      divisions,
      nb_luts,
      (const int**)gammas
      );
  }
  auto end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end-start;
  printf("%d LUT bootstrapping took %lf, one bootstrapping %lf\n", iter_cnt, elapsed_seconds.count(), elapsed_seconds.count() / iter_cnt);

  del_obj(input);
  del_obj_array(nb_luts, outputs);

  for (uint i = 0; i < nb_luts; ++i) {
    delete[] gammas[i];
    delete[] lut_vals[i];
  }
  delete[] gammas;
  delete[] lut_vals;
  delete[] divisions;
}

void test_lut_performance(uint nb_luts, uint iter_cnt) {
  const uint msg_space_size = params->msg_space_size;
  const TFheLutBootstrappingParameterSet* params = keyset->params;
  const LweParams<Torus>* in_out_params = params->in_out_params;
  const LweBootstrappingKeyFFT<Torus>* bk = keyset->cloud.bkFFT;
  const TLweParams<Torus>* accum_params = bk->accum_params;

  printf("Test LUT %d->%d bootstrapping timings\n", params->nb_inps, nb_luts);

  Torus mu;
  Torus* divisions = new Torus[msg_space_size];;
  init_lut_boots(mu, divisions, msg_space_size);

  LweSample<Torus>* input = new_obj<LweSample<Torus>>(in_out_params);
  bootsSymEncrypt(input, 1, keyset);

  int** lut_vals = new int*[nb_luts];
  int** gammas = new int*[nb_luts];

  LweSample<Torus>* outputs = new_obj_array<LweSample<Torus>>(nb_luts, &accum_params->extracted_lweparams);
  LweSample<Torus>* sum = new_obj<LweSample<Torus>>(&accum_params->extracted_lweparams);
  LweSample<Torus>* output = new_obj<LweSample<Torus>>(in_out_params);

  auto start = chrono::system_clock::now();
  for (uint iter = 0; iter < iter_cnt; ++iter)
  {
    for (uint i = 0; i < nb_luts; ++i) {
      lut_vals[i] = new int[msg_space_size];
      gammas[i] = new int[msg_space_size];
    }
    create_random_luts(lut_vals, gammas, nb_luts, msg_space_size, iter);

    tfhe_bootstrap_lut_FFT_no_KS(outputs,
      keyset->cloud.bkFFT,
      mu/2,
      input,
      msg_space_size,
      divisions,
      nb_luts,
      (const int**)gammas
      );

    // \sum_i=0..r-1 2^i * f_i
    lweClear(sum, &accum_params->extracted_lweparams);
    for (uint i = 0; i < (nb_luts > 6 ? 6 : nb_luts); ++i)
      lweAddMulTo(sum, 1<<i, outputs+i, &accum_params->extracted_lweparams);
    lweKeySwitch(output, bk->ks, sum);
    printf("iter %d mv output %d\n", iter, bootsSymDecrypt(output, keyset));
  }

  auto end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end-start;
  printf("%d LUT bootstrapping took %lf, one bootstrapping %lf\n", iter_cnt, elapsed_seconds.count(), elapsed_seconds.count() / iter_cnt);

  del_obj(input);
  del_obj_array(nb_luts, outputs);

  for (uint i = 0; i < nb_luts; ++i) {
    delete[] gammas[i];
    delete[] lut_vals[i];
  }
  delete[] gammas;
  delete[] lut_vals;
  delete[] divisions;
}

void test_gaussian() {
  for (int i = 0; i < 100; ++i)
  {
    Torus t = RandomGen::gaussian(params->in_out_params->alpha_min);
    printf("%ld %.24lf\n", t, TorusUtils<Torus>::to_double(t));
  }
}

int main(int argc, char const *argv[])
{
  int nb_inps = 6;
  uint nb_luts = 3;

  auto start = chrono::system_clock::now();
  printf("Params:\n");
  params = new_default_lut_bootstrapping_parameters(nb_inps);
  printf("Keys:\n");
  keyset = new_random_lut_bootstrapping_secret_keyset(params);
  auto end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end-start;
  printf("Key generation took: %lf\n", elapsed_seconds.count());

  printf("LUT #inputs %d #outputs %d\n", params->nb_inps, nb_luts);
  printf("Message space size %d\n", params->msg_space_size);

  // test_enc_dec();
  // test_lut_boots(nb_luts);
  test_lut_boots_performance_no_KS(nb_luts, 5);
  // test_lut_performance(nb_luts, 5);
  // test_gaussian();

  return 0;

  del_obj(keyset);
  del_obj(params);

}
