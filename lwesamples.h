#ifndef LWESAMPLES_H
#define LWESAMPLES_H

struct LWESample {
   double* a;
   double b;

   LWESample(const LWEParams* params);
   ~LWESample();
   LWESample(const LWESample&)=delete;
   LWESample& operator=(const LWESample&)=delete;
};

#endif //LWESAMPLES_H

