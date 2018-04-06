#include <gmp.h>
#include <cassert>
#include "BigTorus.h"

BigTorus::BigTorus(const ZModuleParams<BigTorus> *params, Allocator *alloc) :
        data(alloc->newArray<mp_limb_t>(params->max_nbLimbs)) {}

void BigTorus::destroy(const ZModuleParams<BigTorus> *params, Allocator *alloc) {
    alloc->deleteArray(params->max_nbLimbs, data);
}

void add(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams<BigTorus> *params) {
    mpn_add_n(res->data, a->data, b->data, params->max_nbLimbs);
}

void sub(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams<BigTorus> *params) {
    mpn_sub_n(res->data, a->data, b->data, params->max_nbLimbs);
}

void mul(BigTorus *res, int64_t a, const BigTorus *b, const ZModuleParams<BigTorus> *params) {
    if (a >= 0) {
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, uint64_t(a));
    } else {
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, uint64_t(-a));
        mpn_neg(res->data, res->data, params->max_nbLimbs);
    }
}

static void mul_no_overlap(BigTorus *res, const BigInt *a, const BigTorus *b, const ZModuleParams<BigTorus> *params) {
    assert(res != b);
    if (a->data->_mp_size > 0) { //positive
        const int m = a->data->_mp_size;
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, a->data->_mp_d[0]);
        for (int i = 1; i < m; i++) {
            mpn_addmul_1(res->data + i, b->data, params->max_nbLimbs - i, a->data->_mp_d[i]);
        }
    } else { //negative case
        const int m = -a->data->_mp_size;
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, a->data->_mp_d[0]);
        for (int i = 1; i < m; i++) {
            mpn_addmul_1(res->data + i, b->data, params->max_nbLimbs - i, a->data->_mp_d[i]);
        }
        mpn_neg(res->data, res->data, params->max_nbLimbs);
    }
}

void mul(BigTorus *res, const BigInt *a, const BigTorus *b, const ZModuleParams<BigTorus> *params, Allocator alloc) {
    if (res!=b) {
        mul_no_overlap(res, a, b, params);
    } else {
        BigTorus* tmp = alloc.newObject<BigTorus>(params, &alloc);
        mpn_copyi(tmp->data, b->data, params->max_nbLimbs);
        mul_no_overlap(res, a, tmp, params);
        alloc.deleteObject(tmp, params, &alloc);
    }
}

void neg(BigTorus *res, BigTorus *a, const ZModuleParams<BigTorus> *params) {
    mpn_neg(res->data, a->data, params->max_nbLimbs);
}

void from_double(BigTorus *reps, const double d, const ZModuleParams<BigTorus> *params) {
    //dissect the input double: extract sign, exponent, mantissa
    static constexpr uint64_t mantissa_msb = (uint64_t(1) << 52);
    static constexpr uint64_t mantissa_mask = mantissa_msb - 1;
    static constexpr uint64_t expo_mask = (uint64_t(1) << 11) - 1;
    uint64_t* dip = (uint64_t *)&d;  //get the bits of d
    uint64_t di = *dip;
    uint64_t mantissa = (di & mantissa_mask) | mantissa_msb;
    uint64_t expo = (di >> 52) & expo_mask;
    uint64_t sign = (di >> 63);
    // printf("mantissa: %lu\n", mantissa);
    // printf("expo: %lu\n", expo);
    // printf("sign: %lu\n", sign);

    //position of the lsb of the mantissa in the bigtorus
    int64_t lsbmantissapos = (int64_t(expo) - 1023l + params->p) - 52l;
    // printf("newlsbpos: %ld\n", lsbmantissapos);

    //set answer to 0
    zero(reps, params);

    //copy the mantissa at the right place
    if (lsbmantissapos >= 0) { //left shift by lsbmantissapos positions
        int64_t blockshift = (lsbmantissapos >> 6); // /64
        int64_t inshift = (lsbmantissapos & 63);    // %64
        if (blockshift < params->max_nbLimbs) {
            reps->data[blockshift] = mantissa << inshift;
            // printf("put %lu at block %ld\n", (mantissa << inshift), blockshift);
            if (blockshift + 1 < params->max_nbLimbs) {
                reps->data[blockshift + 1] = mantissa >> (64 - inshift);
                // printf("put %lu at block %ld\n", mantissa >> ((64 - inshift)), blockshift+1);
            }
        }
    } else { //right shift by -lsbmantissapos positions
        int64_t inshift = (-lsbmantissapos);
        if (inshift < 64) {
            reps->data[0] = (mantissa >> inshift);
            // printf("put %lu at block %ld\n", (mantissa >> inshift), 0l);
        }
    }

    //deal with the sign
    if (sign) {
        //printf("negate all\n");
        neg(reps, reps, params);
    }
}

void zero(BigTorus *res, const ZModuleParams<BigTorus> *params) {
    mpn_zero(res->data, params->max_nbLimbs);
}

void setPowHalf(BigTorus *res, const int k, const ZModuleParams<BigTorus> *params) {
    zero(res, params);
    int idx = params->p - k;
    if (k <= 0 || idx < 0) return;
    //res->data[idx/64] = (1ul << (idx % 64));
    res->data[idx >> 6] = (1ul << (idx & 63ul));
}

namespace {
    /**
     * This class automates the handling of BigTorus on a message space of multiples of 1/MSize
     * It acts as a precomputed adaptor. This class is not exposed to the PublicAPI
     *
     * Note: This class handles generic message spaces. A much faster variant can be designed
     * if the message space is a power of 2. (we leave this to future dev, if this piece of code becomes
     * critical
     */
    class TorusMsgSpace {
    public:
        const int nbLimbs; ///< number of limbs in all BigTorus
        const uint64_t Msize;   ///< Message space size
        Allocator *const alloc;    ///< allocator pointer (for auto-delete)
        mp_limb_t *const hinterv;  ///< represents 1/2Msize mod 1
        mp_limb_t *const interv;   ///< represents 1/Msize mod 1
        mp_limb_t *const buf;      ///< buffer for temp results

        /**
         * Get a precomputed message space adaptor
         * @param Msize size of the message space
         * @param params BigTorus parameters
         * @param alloc current allocator
         */
        TorusMsgSpace(const uint64_t Msize, const ZModuleParams<BigTorus> *params, Allocator *alloc) :
                nbLimbs(params->max_nbLimbs),
                Msize(Msize),
                alloc(alloc),
                hinterv(alloc->newArray<mp_limb_t>(nbLimbs)),
                interv(alloc->newArray<mp_limb_t>(nbLimbs)),
                buf(alloc->newArray<mp_limb_t>(nbLimbs)) {
            // hinterv = [1 / 2.MSize]
            mpn_zero(hinterv, nbLimbs);
            hinterv[nbLimbs - 1] = (UINT64_C(1) << 63);
            mpn_divmod_1(hinterv, hinterv, nbLimbs, Msize);
            // interv = 2*hinterv = [1/MSize]
            mpn_lshift(interv, hinterv, nbLimbs, 1);
        }

        /**
         * decrypts the phase and return the message in [0, Msize[
         * @param phase the phase to decrypt
         * @return the message in [0, Msize[
         */
        uint64_t decrypt(const BigTorus *phase) {
            // message = floor((phase + 1/2Msize mod 1)*Msize)
            mpn_add_n(buf, phase->data, hinterv, nbLimbs);
            return mpn_mul_1(buf, buf, nbLimbs, Msize);
        }

        /**
         * encrypt the give message in [0, Msize[
         * @param res resulting phase
         * @param message message to encrypt. WARNING: it must be in [0, Msize[
         */
        void encryptTrivial(BigTorus *res, const uint64_t message) {
            // res = message * [1/Msize]
            mpn_mul_1(res->data, interv, nbLimbs, message);
        }

        /**
         * round the input phase to the nearest multiple of 1/Msize
         * @param res result
         * @param phase input phase
         */
        void roundPhase(BigTorus *res, const BigTorus *phase) {
            uint64_t message = decrypt(phase);
            encryptTrivial(res, message);
        }

        ~TorusMsgSpace() {
            // clean up
            alloc->deleteArray(nbLimbs, buf);
            alloc->deleteArray(nbLimbs, interv);
            alloc->deleteArray(nbLimbs, hinterv);
        }
    };
}

void approxPhase(BigTorus *res, const BigTorus *phase, uint64_t Msize, ZModuleParams<BigTorus> *params, Allocator alloc) {
    TorusMsgSpace msgSpace(Msize, params, &alloc);
    msgSpace.roundPhase(res, phase);
}

uint64_t modSwitchFromTorus(const BigTorus *phase, uint64_t Msize, ZModuleParams<BigTorus> *params, Allocator alloc) {
    TorusMsgSpace msgSpace(Msize, params, &alloc);
    return msgSpace.decrypt(phase);
}

void modSwitchToTorus(BigTorus *res, const uint64_t message, const uint64_t Msize, ZModuleParams<BigTorus> *params, Allocator alloc) {
    const uint64_t mu = ((message % Msize)+Msize)%Msize; //between 0 and Msize-1
    TorusMsgSpace msgSpace(Msize, params, &alloc);
    msgSpace.encryptTrivial(res, mu);
}

double to_double(const BigTorus *a, const ZModuleParams<BigTorus> *params) {
    // quick and dirty
    return double(int64_t(a->data[params->max_nbLimbs-1]))*pow(0.5,64);
}
