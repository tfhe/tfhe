#ifndef NUMERIC_FUNCTIONS_H
#define NUMERIC_FUNCTIONS_H

///@file
///@brief This file contains the operations on numerical types (especially TORUS)

#include "tfhe_core.h"

#include <limits>
#include <type_traits>
#include <math.h>

#define USE_2X_BIT_INT //TODO: check how to define this as a function of compiler support (need  128-bit ints)

template
<
  typename TORUS,
  typename enable=
    typename std::enable_if<std::is_integral<TORUS>::value && std::is_signed<TORUS>::value>::type
>
class TorusUtils
{
public:
  static const TORUS    min_val = std::numeric_limits<TORUS>::min();
  static const TORUS    max_val = std::numeric_limits<TORUS>::max();
  static const uint32_t bit_cnt = std::numeric_limits<TORUS>::digits + std::numeric_limits<TORUS>::is_signed;
  typedef typename std::make_unsigned<TORUS>::type UTORUS;
  typedef typename std::make_signed<TORUS>::type STORUS;

private:
  static constexpr double   dtot_factor = 4.0 * (TORUS(1) << (bit_cnt-2));

  typedef typename std::conditional<
    (bit_cnt<=32),
    uint64_t,
    __uint128_t>::type torus_2x_type;
  static const uint32_t bit_cnt_2x = (bit_cnt<=32) ? 64 : 128;

public:
  /** conversion from double to torus */
  static TORUS from_double(double d) {
    static double intpart;
    double temp = modf(d, &intpart) / 2.0; //divide by 2 in order to avoid overflow in next line
    return TORUS(temp * dtot_factor) << 1;
  }

  /** conversion from torus to double */
  static double to_double(TORUS x) {
    return double(x) / dtot_factor;
  }


#ifdef USE_2X_BIT_INT
  /**
  *  Used to approximate the phase to the nearest multiple of  1/Msize
  */
  static TORUS approxPhase(TORUS phase, int Msize) {
    torus_2x_type half_interv = ((torus_2x_type(1)<<(bit_cnt_2x-1))/Msize);
    torus_2x_type interv = half_interv << 1;
    torus_2x_type phase_big = (torus_2x_type(phase)<<(bit_cnt_2x - bit_cnt)) + half_interv;
    phase_big -= phase_big % interv;
    return TORUS(phase_big >> (bit_cnt_2x - bit_cnt));
  }

  /**
   *  computes roundToNearestInteger(Msize*phase)
   */
  static int modSwitchFromTorus(TORUS phase, int Msize) {
    torus_2x_type half_interv = ((torus_2x_type(1)<<(bit_cnt_2x-1))/Msize);
    torus_2x_type interv = half_interv << 1;
    torus_2x_type phase_big = (torus_2x_type(phase)<<(bit_cnt_2x - bit_cnt)) + half_interv;
    return phase_big / interv;
  }

  /**
   *  converts mu/Msize to a torus for mu in [0,Msize[
   */
  static TORUS modSwitchToTorus(int mu, int Msize) {
    torus_2x_type half_interv = ((torus_2x_type(1)<<(bit_cnt_2x-1))/Msize);
    torus_2x_type phase_big = ((mu%Msize) * 2) * half_interv;
    return phase_big >> (bit_cnt_2x-bit_cnt);
  }

#else //USE_2X_BIT_INT

  /**
  *  Used to approximate the phase to the nearest multiple of  1/Msize
  */
  static TORUS approxPhase(TORUS phase, int Msize) {
    UTORUS half_interv = ((UTORUS(1) << (bit_cnt-1)) / Msize); // width of each interval
    UTORUS interv = half_interv << 1; // width of each interval
    UTORUS phase_temp = UTORUS(phase) + half_interv;
    phase_temp -= phase_temp%interv;
    return (TORUS)phase_temp;
  }

  /**
   *  computes roundToNearestInteger(Msize*phase)
   */
  static int modSwitchFromTorus(TORUS phase, int Msize) {
    UTORUS half_interv = ((UTORUS(1) << (bit_cnt-1)) / Msize);
    UTORUS interv = half_interv << 1; // width of each interval
    UTORUS phase_temp = UTORUS(phase) + half_interv;
    return TORUS(phase_temp / interv);
  }

  /**
   *  converts mu/Msize to a torus for mu in [0,Msize[
   */
  static TORUS modSwitchToTorus(int mu, int Msize) {
    UTORUS half_interv = ((UTORUS(1) << (bit_cnt-1)) / Msize);
    UTORUS phase_temp = ((mu%Msize) * 2) * half_interv;
    return phase_temp;
  }
#endif //USE_2X_BIT_INT
};


#endif //NUMERIC_FUNCTIONS_H
