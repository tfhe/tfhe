#include <cmath>
#include "lweparams.h"

using namespace std;

LweParams::LweParams(int32_t n, double alpha_min, double alpha_max):
		n(n),
		alpha_min(alpha_min),
		alpha_max(alpha_max) {}

LweParams::~LweParams() {}
