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
     * @brief Set seed from an array of integers
     * @param values array of integers
     * @paramr size number of array elements
     */
    static void set_seed(uint32_t *values, int32_t size) {
        std::seed_seq seeds(values, values + size);
        generator.seed(seeds);
    }

    /**
     * @brief Set seed from a single integer
     * @param seed integer seed
     */
    static void set_seed(uint32_t seed) {
        set_seed(&seed, 1);
    }

    /**
     * @brief Generate an uniformly distributed integer from range [0,1]
     */
    static int32_t uniform_bool() {
        return uniform_bool_distrib(generator);
    }

    /**
     * @brief Generate a normally distributed double with zero mean and given stdandard deviation
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
     * @brief Generate an uniformly distributed torus element
     */
    static TORUS uniform(
            const ZModuleParams<TORUS> *params);

    /**
     * @brief Generate a normally distributed torus element with given mean and standard deviation
     * @param mean normal distribution mean value
     * @param sigma normal distribution standard deviation
     */
    static TORUS gaussian(
            const TORUS mean,
            const double sigma,
            const ZModuleParams<TORUS> *params);
};

/**
 * Instantiation for BigTorus type
 */
template<>
class RandomGenTorus<BigTorus> : public RandomGen {
protected:

public:
    using TORUS = BigTorus;

    /**
     * @brief Generate an uniformly distributed torus element
     * @param dst output variable
     */
    static void uniform(
            TORUS *dst,
            const ZModuleParams<TORUS> *params);

    /**
     * @brief Generate a normally distributed big torus element with given mean and standard deviation
     * @param dst output variable
     * @param mean normal distribution mean value
     * @param sigma normal distribution standard deviation
     */
    static void gaussian(
            TORUS *dst,
            const TORUS *mean,
            const double sigma,
            const ZModuleParams<TORUS> *params);
};

#endif //RANDOM_GEN_H
