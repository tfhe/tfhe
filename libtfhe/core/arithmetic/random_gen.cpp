#include "random_gen.h"

using namespace std;

default_random_engine RandomGen::generator;

uniform_int_distribution <int32_t> RandomGen::uniform_bool_distrib(0, 1);
normal_distribution<double> RandomGen::std_normal_distrib;
