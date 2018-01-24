#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <random>
#include "tfhe_gate.h"
#include "tfhe_alloc.h"
#include "random_gen.h"
// #include "tlwe.h"

using namespace std;

#define Torus Torus32


// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************
double approxEquals(Torus a, Torus b) { return abs(a-b)<10; }

int main(int argc, char** argv) {
  for (int i=0; i<20000; i++) RandomGenTorus<Torus>::uniform(); // Ila ?????
  
  const int N = 1024;
  const int k = 2;
  const int alpha_min = 0.01;
  const int alpha_max = 0.071;
  const int Msize = 7; // taille de l'espace des coeffs du polynome du message
  const double alpha = 0.02;
  static uniform_int_distribution<int> distribution(0,Msize-1);

  // PARAMETERS
  TLweParams<Torus>* params = new_TLweParams<Torus>(N, k, alpha_min, alpha_max); //les deux alpha mis un peu au hasard
  // KEY
  TLweKey<Torus>* key = new_TLweKey(params);
  // CIPHERTEXTS
  TLweSample<Torus>* cipher = new_TLweSample(params);
  TLweSample<Torus>* cipherT = new_TLweSample(params);


  //the probability that a sample with stdev alpha decrypts wrongly on
  //the a Msize message space.
  double expected_error_proba = 1.-erf(1./(sqrt(2.)*2.*Msize*alpha));


  cout << "-------------" << endl;
  cout << "WARNING:" << endl;
  cout << "All the tests below are supposed to fail with proba: " << expected_error_proba << endl; 
  cout << "It is normal and it is part of the test!" << endl; 
  cout << "-------------" << endl;

  //MESSAGE
  TorusPolynomial<Torus>* mu = new_TorusPolynomial<Torus>(N);
  for (int i = 0; i < N; ++i){
    int temp = distribution(RandomGen::generator);
    mu->coefsT[i] = TorusUtils<Torus>::modSwitchToTorus(temp, Msize);
    //cout << mu->coefsT[i] << endl;
  }
  // PHASE, DECRYPTION
  TorusPolynomial<Torus>* phi = new_TorusPolynomial<Torus>(N);
  TorusPolynomial<Torus>* dechif = new_TorusPolynomial<Torus>(N);

  
  tLweKeyGen(key); // KEY GENERATION
  tLweSymEncrypt(cipher, mu, alpha, key); // ENCRYPTION
  tLwePhase(phi, cipher, key); // PHASE COMUPTATION
  tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

  cout << "Test LweSymDecrypt :" << endl;
  for (int i = 0; i < N; ++i)
  {
    if (dechif->coefsT[i] != mu->coefsT[i])
      cout << i << " - " << dechif->coefsT[i] << " =? " << mu->coefsT[i] << " error!!!" << endl;     
  }
  cout << "----------------------" << endl;



  TorusPolynomial<Torus>* phiT = new_TorusPolynomial<Torus>(N);

  for (int trial=1; trial<1000; trial++) {
    Torus muT = TorusUtils<Torus>::modSwitchToTorus(distribution(RandomGen::generator), Msize);
    Torus dechifT = 0;

    tLweSymEncryptT(cipherT, muT, alpha, key);
    tLwePhase(phiT, cipherT, key);
    dechifT = tLweSymDecryptT(cipherT, key, Msize);
    
    if (dechifT != muT) {
      cout << "Test LweSymDecryptT: trial " << trial << endl;
      cout << dechifT << " =? " << muT << " Error!!!" << endl; 
      cout << "----------------------" << endl;
    }    
  }

    














  


  // TEST ADD, SUB, LINEAR COMBINATION, POLYNOMIAL COMBINATIONS 

  cout << endl;
  cout << endl;
  cout << "-------------------------" << endl;
  cout << "TEST Operations TLwe :" << endl;
  cout << "-------------------------" << endl;

  // CIPHERTEXTS
  TLweSample<Torus>* cipher0 = new_TLweSample<Torus>(params);
  TLweSample<Torus>* cipher1 = new_TLweSample<Torus>(params);
      
  // MESSAGES
  TorusPolynomial<Torus>* mu0 = new_TorusPolynomial<Torus>(N);
  for (int i = 0; i < N; ++i){
    int temp = distribution(RandomGen::generator);
    mu0->coefsT[i] = TorusUtils<Torus>::modSwitchToTorus(temp, Msize);
  }
  TorusPolynomial<Torus>* mu1 = new_TorusPolynomial<Torus>(N);
  for (int i = 0; i < N; ++i){
    int temp = distribution(RandomGen::generator);
    mu1->coefsT[i] = TorusUtils<Torus>::modSwitchToTorus(temp, Msize);
  }


  int p = 1;
  IntPolynomial* poly = new_IntPolynomial(N);
  for (int i = 0; i < N; ++i){
    poly->coefs[i] = distribution(RandomGen::generator);
  }
  


  int decInt = 0;
  int muInt = 0;
  


  for (int trial=1; trial<2; trial++) {

    tLweSymEncrypt(cipher0, mu0, alpha, key); // ENCRYPTION
    tLweSymEncrypt(cipher1, mu1, alpha, key); // ENCRYPTION


    // cipher = cipher0 + cipher1 
    tLweCopy(cipher, cipher0, params);
    tLweAddTo(cipher, cipher1, params);
    torusPolynomialAdd(mu, mu0, mu1); // mu = mu0 + mu1
    tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
    
    cout << "Test tLweAddTo Trial : " << trial << endl;
    for (int i = 0; i < N; ++i)
    {
      decInt = TorusUtils<Torus>::modSwitchFromTorus(dechif->coefsT[i], Msize);
      muInt = TorusUtils<Torus>::modSwitchFromTorus(mu->coefsT[i], Msize);
      if (decInt != muInt)
        cout << decInt << " =? " << muInt << " error!!!" << endl;     
    }
    cout << cipher->current_variance << endl;
    cout << "----------------------" << endl;



    // cipher = cipher0 - cipher1 
    tLweCopy(cipher, cipher0, params);
    tLweSubTo(cipher, cipher1, params);
    torusPolynomialSub(mu, mu0, mu1); // mu = mu0 - mu1
    tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
    
    cout << "Test tLweSubTo Trial : " << trial << endl;
    for (int i = 0; i < N; ++i)
    {
      decInt = TorusUtils<Torus>::modSwitchFromTorus(dechif->coefsT[i], Msize);
      muInt = TorusUtils<Torus>::modSwitchFromTorus(mu->coefsT[i], Msize);
      if (decInt != muInt)
        cout << decInt << " =? " << muInt << " error!!!" << endl;     
    }
    cout << cipher->current_variance << endl;
    cout << "----------------------" << endl;
    

    
    // cipher = cipher0 + p.cipher1 
    tLweCopy(cipher, cipher0, params);
    tLweAddMulTo(cipher, p, cipher1, params);
    torusPolynomialAddMulZ(mu, mu0, p, mu1); // mu = mu0 + p.mu1
    tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
    
    cout << "Test tLweAddMulTo Trial :" << trial << endl;
    for (int i = 0; i < N; ++i)
    {
      decInt = TorusUtils<Torus>::modSwitchFromTorus(dechif->coefsT[i], Msize);
      muInt = TorusUtils<Torus>::modSwitchFromTorus(mu->coefsT[i], Msize);
      if (decInt != muInt)
        cout << decInt << " =? " << muInt << " error!!!" << endl;     
    }
    cout << cipher->current_variance << endl;
    cout << "----------------------" << endl;
    
    


    // cipher = cipher0 - p.cipher1 
    tLweCopy(cipher, cipher0, params);
    tLweSubMulTo(cipher, p, cipher1, params);
    torusPolynomialSubMulZ(mu, mu0, p, mu1); // mu = mu0 - p.mu1
    tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
    
    cout << "Test tLweSubMulTo Trial :" << trial << endl;
    for (int i = 0; i < N; ++i)
    {
      decInt = TorusUtils<Torus>::modSwitchFromTorus(dechif->coefsT[i], Msize);
      muInt = TorusUtils<Torus>::modSwitchFromTorus(mu->coefsT[i], Msize);
      if (decInt != muInt)
        cout << decInt << " =? " << muInt << " error!!!" << endl;     
    }
    cout << cipher->current_variance << endl;
    cout << "----------------------" << endl;
    

    /*
    // result = result + poly.sample 
    tLweCopy(cipher, cipher0, params);
    tLweAddMulRTo(cipher, poly, cipher1, params);
    // mu = mu0 + poly.mu1
    torusPolynomialCopy(mu, mu0);
    torusPolynomialAddMulRKaratsuba(mu, poly, mu1);

    tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
    
    cout << "Test tLweAddMulRTo Trial:" << trial << endl;
    for (int i = 0; i < N; ++i)
    {
      decInt = TorusUtils<Torus>::modSwitchFromTorus(dechif->coefsT[i], Msize);
      muInt = TorusUtils<Torus>::modSwitchFromTorus(mu->coefsT[i], Msize);
      if (decInt != muInt)
        cout << decInt << " =? " << muInt << " error!!!" << endl;     
    }
    cout << cipher->current_variance << endl;
    cout << "----------------------" << endl;
    */
  }

  

  delete_TLweSample(cipher0);
  delete_TLweSample(cipher1);
  delete_TorusPolynomial(mu0);
  delete_TorusPolynomial(mu1);
  delete_IntPolynomial(poly);











  // TEST ADD, SUB, LINEAR COMBINATION, POLYNOMIAL COMBINATIONS 

  cout << endl;
  cout << endl;
  cout << "-----------------------------------------------" << endl;
  cout << "TEST Operations TLwe with Torus messages :" << endl;
  cout << "-----------------------------------------------" << endl;

  // CIPHERTEXTS
  TLweSample<Torus>* cipherT0 = new_TLweSample<Torus>(params);
  TLweSample<Torus>* cipherT1 = new_TLweSample<Torus>(params);
      
  
  int pT = 1;
  
  for (int trial=1; trial<1000; trial++) {

    // MESSAGES
    Torus muT0 = TorusUtils<Torus>::modSwitchToTorus(distribution(RandomGen::generator), Msize);
    Torus muT1 = TorusUtils<Torus>::modSwitchToTorus(distribution(RandomGen::generator), Msize);
     
    Torus muT = 0;    
    Torus dechifT = 0;

    tLweSymEncryptT(cipherT0, muT0, alpha, key); // ENCRYPTION
    tLweSymEncryptT(cipherT1, muT1, alpha, key); // ENCRYPTION



    
    // cipher = cipher0 + cipher1 
    tLweCopy(cipherT, cipherT0, params);
    tLweAddTo(cipherT, cipherT1, params);
    muT = muT0 + muT1;
    dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION

    decInt = TorusUtils<Torus>::modSwitchFromTorus(dechifT, Msize);
    muInt = TorusUtils<Torus>::modSwitchFromTorus(muT, Msize);
    if (decInt != muInt) {
      cout << "Test tLweAddTo Trial : " << trial << endl;
      cout << decInt << " =? " << muInt << " Error!!!" << endl; 
      cout << cipherT->current_variance << endl;
      cout << "----------------------" << endl;
    } 
    

    
    // cipher = cipher0 - cipher1 
    tLweCopy(cipherT, cipherT0, params);
    tLweSubTo(cipherT, cipherT1, params);
    muT = muT0 - muT1;
    dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION
    
    decInt = TorusUtils<Torus>::modSwitchFromTorus(dechifT, Msize);
    muInt = TorusUtils<Torus>::modSwitchFromTorus(muT, Msize);
    if (decInt != muInt) {
      cout << "Test tLweSubTo Trial : " << trial << endl;
      cout << decInt << " =? " << muInt << " Error!!!" << endl; 
      cout << cipherT->current_variance << endl;
      cout << "----------------------" << endl;
    }  

    


    // cipher = cipher0 + p.cipher1 
    tLweCopy(cipherT, cipherT0, params);
    tLweAddMulTo(cipherT, pT, cipherT1, params);
    muT = muT0 + pT*muT1;
    dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION
    
    decInt = TorusUtils<Torus>::modSwitchFromTorus(dechifT, Msize);
    muInt = TorusUtils<Torus>::modSwitchFromTorus(muT, Msize);
    if (decInt != muInt) {
      cout << "Test tLweAddMulTo Trial : " << trial << endl;
      cout << decInt << " =? " << muInt << " Error!!!" << endl; 
      cout << cipherT->current_variance << endl;
      cout << "----------------------" << endl;
    } 




    // result = result - p.sample 
    tLweCopy(cipherT, cipherT0, params);
    tLweSubMulTo(cipherT, pT, cipherT1, params);
    muT = muT0 - pT*muT1;
    dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION
    
    decInt = TorusUtils<Torus>::modSwitchFromTorus(dechifT, Msize);
    muInt = TorusUtils<Torus>::modSwitchFromTorus(muT, Msize);
    if (decInt != muInt) {
      cout << "Test tLweSubMulTo Trial : " << trial << endl;
      cout << decInt << " =? " << muInt << " Error!!!" << endl; 
      cout << cipherT->current_variance << endl;
      cout << "----------------------" << endl;
    } 
  }

  

  delete_TLweSample(cipherT0);
  delete_TLweSample(cipherT1);















  delete_TLweKey(key);
  delete_TLweSample(cipher);
  delete_TLweSample(cipherT);

  delete_TorusPolynomial(mu);
  delete_TorusPolynomial(phi);
  delete_TorusPolynomial(phiT);
  delete_TorusPolynomial(dechif);

  //ATTENTION, le params est utilisé dans divers destructeurs, il faut l'effacer en dernier 
  delete_TLweParams(params); 
  
  return 0;
}
