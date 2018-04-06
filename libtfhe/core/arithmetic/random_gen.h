#ifndef RANDOM_GEN_H
#define RANDOM_GEN_H

#include "torus_utils.h"

#include <random>

/**
 * Random number generator
 */
class RandomGen {
private:

    static std::uniform_int_distribution <int32_t> uniform_bool_distrib;
    static std::normal_distribution<double> std_normal_distrib;

public:
    /**
     * @todo move default_random_engine to context class
     */
    static std::default_random_engine generator;

    /**
     * Set seed from an array of integers
     * @param values array of integers
     * @paramr size number of array elements
     */
    static void set_seed(uint32_t *values, int32_t size) {
        std::seed_seq seeds(values, values + size);
        generator.seed(seeds);
    }

    /**
     * Set seed from a single integer
     * @param seed integer seed
     */
    static void set_seed(uint32_t seed) {
        set_seed(&seed, 1);
    }

    /**
     * Generate an uniformly distributed integer from range [0,1]
     */
    static int32_t uniform_bool() {
        return uniform_bool_distrib(generator);
    }

    /**
     * Generate a normally distributed double with zero mean and given stdandard deviation
     * @param sigma standard deviation
     */
    static double gaussian(const double sigma) {
        return std_normal_distrib(generator) * sigma;
    }
};

/**
 * Specific random number generator for torus types
 * @tparam TORUS torus data-type
 */
template<typename TORUS>
class RandomGenTorus : public RandomGen {
private:
    static std::uniform_int_distribution <TORUS> uniform_distrib;

public:
    /**
     * Generate an uniformly distributed torus element
     */
    static TORUS uniform() {
        return uniform_distrib(generator);
    }

    /**
     * Generate a normally distributed torus element with given mean and standard deviation
     * @param mean normal distribution mean value
     * @param sigma normal distribution standard deviation
     */
    static TORUS gaussian(const TORUS mean, const double sigma) {
        return mean + TorusUtils<TORUS>::from_double(RandomGen::gaussian(sigma));
    }
};

template<typename TORUS>
std::uniform_int_distribution <TORUS> RandomGenTorus<TORUS>::uniform_distrib =
        std::uniform_int_distribution<TORUS>(TorusUtils<TORUS>::min_val, TorusUtils<TORUS>::max_val);

template<>
class RandomGenTorus<BigTorus> : public RandomGen {
};

#endif //RANDOM_GEN_H