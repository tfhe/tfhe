#include <cmath>
#include "lweparams.h"

using namespace std;

LWEParams::LWEParams(int n, double alpha):
		n(n),
		alpha(alpha),
		log_alpha(int(ceil(-log(alpha)))) {}

LWEParams::~LWEParams() {}
