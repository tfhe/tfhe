#include <benchmark/benchmark.h>
#include <core/arithmetic/big_int.h>
#include <core/allocator/allocator.h>


class BigIntBench : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state) {
        const auto nbLimbsFrac = state.range(0);

        params = alloc.newObject<ZModuleParams<BigTorus>>(nbLimbsFrac);
        big_int_arr = alloc.newArray<BigInt>(big_int_arr_size);
        gmp_randstate_t rnd_state;
        gmp_randinit_default(rnd_state);
        for (int i = 0; i < big_int_arr_size; ++i)
            mpz_urandomb(big_int_arr[i].data, rnd_state, params->p);
    }

    void TearDown(const ::benchmark::State &state) {
        alloc.deleteArray(big_int_arr_size, big_int_arr);
        alloc.deleteObject(params);
    }

    Allocator alloc;
    ZModuleParams<BigTorus> *params = nullptr;

    const int big_int_arr_size = 3;
    BigInt *big_int_arr = nullptr;
};

BENCHMARK_DEFINE_F(BigIntBench, add)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::add(big_int_arr + 2, big_int_arr + 1, big_int_arr + 0, params);
    }
}

BENCHMARK_REGISTER_F(BigIntBench, add)->Ranges({{1, 64},
                                                {1, 64}});

BENCHMARK_DEFINE_F(BigIntBench, mul)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul(big_int_arr + 2, big_int_arr + 1, big_int_arr + 0, params);
    }
}

BENCHMARK_REGISTER_F(BigIntBench, mul)->Ranges({{1, 64},
                                                {1, 64}});
