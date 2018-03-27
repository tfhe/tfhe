#ifndef RANDOM_GEN_H
#define RANDOM_GEN_H

#include "numeric_functions.h"

#include <random>

class RandomGen {
private:

    static std::uniform_int_distribution<int> uniform_bool_distrib;
    static std::normal_distribution<double> std_normal_distrib;

public:
    static std::default_random_engine generator;

    /** sets the seed of the random number generator to the given values */
    static void set_seed(uint32_t *values, int size) {
        std::seed_seq seeds(values, values + size);
        generator.seed(seeds);
    }

    static void set_seed(uint32_t seed) {
        set_seed(&seed, 1);
    }

    /** generate an uniformly distributed integer from range [0,1] */
    static int uniform_bool() {
        return uniform_bool_distrib(generator);
    }

    /** generate a normally distributed double with std deviation \c sigma */
    static double gaussian(const double sigma) {
        return std_normal_distrib(generator) * sigma;
    }
};

template<typename TORUS>
class RandomGenTorus : public RandomGen {
private:
    static std::uniform_int_distribution <TORUS> uniform_distrib;

public:
    /** generate an uniformly distributed \c TORUS element */
    static TORUS uniform() {
        return uniform_distrib(generator);
    }

    /** generate an uniformly distributed \c TORUS element */
    static TORUS gaussian(const TORUS message, const double sigma) {
        return message + TorusUtils<TORUS>::from_double(RandomGen::gaussian(sigma));
    }
};

template<typename TORUS>
std::uniform_int_distribution <TORUS> RandomGenTorus<TORUS>::uniform_distrib =
        std::uniform_int_distribution<TORUS>(TorusUtils<TORUS>::min_val, TorusUtils<TORUS>::max_val);


#endif //RANDOM_GEN_H