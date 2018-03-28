#ifndef NUMERIC_UTILS_H
#define NUMERIC_UTILS_H

#include "torus.h"

#include <limits>
#include <type_traits>
#include <cstdint>
#include <cmath>

/**
 * Numeric utils for Torus data types
 * @tparam TORUS type of torus
 */
template<typename TORUS>
class TorusUtils {
public:
    static const TORUS min_val = std::numeric_limits<TORUS>::min();
    static const TORUS max_val = std::numeric_limits<TORUS>::max();
    static const uint32_t bit_cnt = std::numeric_limits<TORUS>::digits + std::numeric_limits<TORUS>::is_signed;
    typedef typename std::make_unsigned<TORUS>::type UTORUS;
    typedef typename std::make_signed<TORUS>::type STORUS;

private:
    static constexpr double dtot_factor = 4.0 * (TORUS(1) << (bit_cnt - 2));

public:
    /**
     * Converts real number to torus
     * @param d real to convert
     * @return torus value
     */
    static TORUS from_double(double d) {
        static double intpart;
        double temp = modf(d, &intpart) / 2.0; //divide by 2 in order to avoid overflow in next line
        return TORUS(temp * dtot_factor) << 1;
    }

    /**
     * Converts torus to real number
     * @param x torus element to convert
     * @return real number
    */
    static double to_double(TORUS x) {
        return double(x) / dtot_factor;
    }

    /**
     * Approximates torus value to the nearest multiple of 1/Msize
     * @param phase torus value
     * @param Msize discrete space size
     * @return approximated torus value
     */
    static TORUS approxPhase(TORUS phase, int32_t Msize) {
        UTORUS half_interv = ((UTORUS(1) << (bit_cnt - 1)) / Msize); // half width of each interval
        UTORUS interv = half_interv << 1; // width of each interval
        UTORUS phase_temp = UTORUS(phase) + half_interv;
        phase_temp -= phase_temp % interv;
        return (TORUS) phase_temp;
    }

    /**
     * Converts torus to discrete message space
     *  i.e. computes roundToNearestInteger(Msize*phase)
     * @param phase torus value to convert
     * @param Msize discrete space size
     * @return discrete space value
     */
    static int32_t modSwitchFromTorus(TORUS phase, int32_t Msize) {
        UTORUS half_interv = ((UTORUS(1) << (bit_cnt - 1)) / Msize);
        UTORUS interv = half_interv << 1; // width of each interval
        UTORUS phase_temp = UTORUS(phase) + half_interv;
        return TORUS(phase_temp / interv);
    }

    /**
     * Converts discrete message space to torus
     *  i.e. value mu/Msize to a torus for mu
     * @param mu discrete space value (from [0,Msize[) to convert
     * @param Msize discrete space size
     * @return torus value
     */
    static TORUS modSwitchToTorus(int32_t mu, int32_t Msize) {
        UTORUS half_interv = ((UTORUS(1) << (bit_cnt - 1)) / Msize);
        UTORUS phase_temp = ((mu % Msize) * 2) * half_interv;
        return phase_temp;
    }
};

#endif //NUMERIC_FUNCTIONS_H
