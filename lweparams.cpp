#include <cmath>
#include "lweparams.h"

using namespace std;

//allocates and initialize lweParams
LWEParams* new_lweParams(int n, double alpha) {
	return new LWEParams(n,alpha);
}

//destructs and frees a lweParams structure
void delete_lweParams(LWEParams* ptr) {
	delete ptr;
}

LWEParams::LWEParams(int n, double alpha):
		n(n),
		alpha(alpha),
		log_alpha(int(ceil(-log(alpha)))) {}
