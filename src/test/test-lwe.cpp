#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe_gate.h"

using namespace std;

#define Torus Torus32

// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************
double approxEquals(Torus a, Torus b) { return abs(a-b)<TorusUtils<Torus>::from_double(1e-9); }

int main(int argc, char** argv) {
  LweParams<Torus>* params = new_LweParams<Torus>(512, 0.2, 0.5); //les deux alpha mis un peu au hasard
  int n = params->n;
  LweKey<Torus>* key = new_LweKey(params);
  LweSample<Torus>* cipher = new_obj<LweSample<Torus>>(params);
  Torus mu = TorusUtils<Torus>::from_double(0.5);
  //Attention, 1<<30 correspond au message 0.25!! Ila: t'as raison!
  double alpha = 0.0625;
  Torus phi;
  double message;  
  int Msize = 2;  

  lweKeyGen(key);
  lweSymEncrypt(cipher, mu, alpha, key);
  cout << "a = [";
  for (int i = 0; i < n-1; ++i) cout << TorusUtils<Torus>::to_double(cipher->a[i]) << ", ";
  cout << TorusUtils<Torus>::to_double(cipher->a[n-1]) << "]" << endl;
  cout << "b = " << TorusUtils<Torus>::to_double(cipher->b) << endl;

  phi = lwePhase(cipher, key);
  cout << "phi = " << TorusUtils<Torus>::to_double(phi) << endl;
  message = lweSymDecrypt(cipher, key, Msize);
  cout << "message = " << TorusUtils<Torus>::to_double(message) << endl;

  //lwe crash test
  int failures = 0;
  int trials = 1000;
  for (int i=0; i<trials; i++) {
    Torus input = TorusUtils<Torus>::from_double((i%3)/3.);
    lweKeyGen(key);
    lweSymEncrypt(cipher, input, 0.047, key); // Ila: attention au niveau de bruit!!! à voir (0.06 n'est pas le bon je crois, 0.047 marche parfaitement)
    phi = lwePhase(cipher, key);
    Torus decrypted = lweSymDecrypt(cipher, key, 3);
    if ( !approxEquals(input,decrypted) ) {
      cerr << input << " " << decrypted << endl;
      cerr << "WARNING: the msg " << TorusUtils<Torus>::to_double(input) << " gave phase " << TorusUtils<Torus>::to_double(phi) << " and was incorrectly decrypted to " << TorusUtils<Torus>::to_double(decrypted) << endl;
      failures++;
    }
  }
  cout << "There were " << failures << " failures out of " << trials << " trials" << endl;
  cout << "(it might be normal)" << endl;

  delete_LweParams(params); //les deux alpha mis un peu au hasard
  delete_LweKey(key);
  delete_LweSample(cipher);

  return 0;
}
