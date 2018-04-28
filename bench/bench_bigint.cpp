#include <benchmark/benchmark.h>
#include <core/arithmetic/big_int.h>
#include <core/allocator/allocator.h>


class BigInt_Bench : public benchmark::Fixture {
public:
    BigInt_Bench() {
        gmp_randinit_default(rnd_state);
    }

    ~BigInt_Bench() {
        gmp_randclear(rnd_state);
    }

    void SetUp(const ::benchmark::State &state) {
        const auto nbLimbsFrac = state.range(0);

        params = alloc.newObject<ZModuleParams<BigTorus>>(nbLimbsFrac);
        big_int_arr = alloc.newArray<BigInt>(big_int_arr_size);
        for (int i = 0; i < big_int_arr_size; ++i)
            mpz_urandomb(big_int_arr[i].data, rnd_state, params->p);
    }

    void TearDown(const ::benchmark::State &state) {
        alloc.deleteArray(big_int_arr_size, big_int_arr);
        alloc.deleteObject(params);
    }

    gmp_randstate_t rnd_state;
    Allocator alloc;
    ZModuleParams<BigTorus> *params = nullptr;

    const int big_int_arr_size = 3;
    BigInt *big_int_arr = nullptr;
};

// void add(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigInt_Bench, add_II)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::add(big_int_arr + 2, big_int_arr + 1, big_int_arr + 0, params);
    }
}
BENCHMARK_REGISTER_F(BigInt_Bench, add_II)->Range(1, 128);

// void add(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigInt_Bench, add_iI)(benchmark::State &state) {
    int64_t a = rand();
    for (auto _ : state) {
        tfhe_backend::add(big_int_arr + 2, a, big_int_arr + 0,params);
    }
}
BENCHMARK_REGISTER_F(BigInt_Bench, add_iI)->Range(1, 128);

// void mul(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigInt_Bench, mul_II)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul(big_int_arr + 2, big_int_arr + 1, big_int_arr + 0, params);
    }
}
BENCHMARK_REGISTER_F(BigInt_Bench, mul_II)->Range(1, 128);

// void mul(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigInt_Bench, mul_iI)(benchmark::State &state) {
    int64_t a = rand();
    for (auto _ : state) {
        tfhe_backend::mul(big_int_arr + 2, a, big_int_arr + 0,params);
    }
}
BENCHMARK_REGISTER_F(BigInt_Bench, mul_iI)->Range(1, 128);

