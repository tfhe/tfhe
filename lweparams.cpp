#include <cmath>
#include "lweparams.h"

using namespace std;

LWEParams::LWEParams(int n, double alpha_min, double alpha_max):
		n(n),
		alpha_min(alpha_min),
		alpha_max(alpha_max),
		log_alpha_min(int(ceil(-log(alpha_min)))) {}

LWEParams::~LWEParams() {}
